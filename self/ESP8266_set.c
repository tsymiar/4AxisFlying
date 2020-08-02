#include "ESP8266_set.h"

#define ESPOK (u8*)("OK")
#define MAX_RCV_LEN 1024

volatile u16 USART2_RX_STA = 0;
u8 USART2_RX_BUF[MAX_RCV_LEN];

const u8 EspArguments[17][32] = {
    "AT",
    "ATE0", // close echo
    "AT+RST", // reset
    "AT+CWMODE=1", // WIFI STA
    "AT+CWMODE=0", // WIFI AP
    "AT+CIPMODE=1", // passthrough
    "AT+CIPMODE=0", // normal
    "AT+CWJAP=\"%s\",\"%s\",1,3", // set adapter params
    "AT+CWJAP=\"%s\",\"%s\"", // connect to adapter
    "AT+CIPMUX=0", // single connect
    "AT+CIPMUX=1", // multiple connect
    "AT+CIPSTART=\"TCP\",\"%s\",%d",
    "AT+CIPSTART=\"UDP\",\"%s\",%d",
    "AT+CIPSEND" // sends begins
    "AT+CIPSEND=%d", // send %d bytes
    "AT+CIPSEND=0,%d", // same above
    "AT+CIPSERVER=1" // start as server
};

/*modle init:
    1.AT command test
    2.close echo
    3.set as client
    4.connect to adapter
    5.connect to server
    6.set as passthrough=1
    7.begin passthrough
*/
u8 at_test(void);
u8 close_echo(void);
u8 set_as_client(void);
u8 connect_to_adapter(u8* ssid, u8* password);
u8 connect_to_server(u8* ip, u16 port, u8 mode);
u8 set_trans_mode(u8 mode);
u8 start_trans(u8* data, u8 mode);

u8* esp_cmd_check(u8 *str)
{
    char *strx = 0;
    if (USART2_RX_STA & 0X8000) {
        USART2_RX_BUF[USART2_RX_STA & 0X7FFF] = 0;
        strx = strstr((const char*)USART2_RX_BUF, (const char*)str);
    }
    return (u8*)strx;
}

void atk_8266_at_response(u8 mode)
{
    if (USART2_RX_STA & 0X8000) // recieved data at time
    {
        USART2_RX_BUF[USART2_RX_STA & 0X7FFF] = 0;// add '\0'
        printf("%s", USART2_RX_BUF);    // send to serial port
        if (mode)USART2_RX_STA = 0;
    }
}

void Send_Joystick(uint8_t Keys, USART_TypeDef * USARTn)
{
    //Config USART2 to send data to serial port
    USART_ClearFlag(USART2, USART_FLAG_TC);        //clear bit zone to aviod missing the primacy. 
    USART_SendData(USARTn, Keys);
    LED_CatchSignal(Green, LED_ON);                                    //which lamp blinks.
    Delay_us(5);
}

u8 Send_Cmd_Wait(const u8 *cmd, u8* ack, u32 waittime)
{
    u8 res = 0;
    USART2_RX_STA = 0;
    u2printf("%s\r\n", cmd);
    if (ack&&waittime) { // wait for a response
        while (--waittime) {
            Delay_us(10);
            if (USART2_RX_STA & 0X8000) {// recieved
                if (esp_cmd_check(ack)) {
                    printf("ACK: %s\r\n", (u8*)ack);
                    res = 1;
                    break;
                }
                USART2_RX_STA = 0;
            }
        }
    }
    return res;
}

u8 esp_send_bytes(u8 *data, u8 *ack, u16 waittime)
{
    u8 res = 0;
    USART2_RX_STA = 0;
    u2printf("%s", data);
    if (ack&&waittime)
    {
        while (--waittime)
        {
            Delay_us(10);
            if (USART2_RX_STA & 0X8000)
            {
                if (esp_cmd_check(ack))
                    break;
                USART2_RX_STA = 0;
            }
        }
        if (waittime == 0)res = 1;
    }
    return res;
}

u8 Close_Trans()
{
    while ((USART2->ISR & 0X40) == 0);    // wait for null
    USART2->RDR = '+';
    Delay_us(15);                    // greater than (10ms)
    while ((USART2->ISR & 0X40) == 0);
    USART2->RDR = '+';
    Delay_us(15);
    while ((USART2->ISR & 0X40) == 0);
    USART2->RDR = '+';
    Delay_us(500);
    Send_Cmd_Wait((u8*)"AT", ESPOK, 20);
    return Send_Cmd_Wait(EspArguments[0], ESPOK, 20);
}

u8 at_test()
{
    return Send_Cmd_Wait(EspArguments[0], ESPOK, 20);
}

u8 close_echo()
{
    return Send_Cmd_Wait(EspArguments[1], ESPOK, 20);
}

u8 set_as_client()
{
    return Send_Cmd_Wait(EspArguments[3], ESPOK, 20);
}

u8 connect_to_adapter(u8* ssid, u8* password)
{
    u8 args[32];
    sprintf((char*)args, (char*)EspArguments[8], ssid, password);
    return Send_Cmd_Wait(args, (u8*)"WIFI GOT IP", 20);
}

u8 connect_to_server(u8* ip, u16 port, u8 mode)
{
    u8 args[32];
    if (mode & 0xff) { // tcp
        sprintf((char*)args, (char*)EspArguments[11], ip, port);
    } else { // udp
        sprintf((char*)args, (char*)EspArguments[12], ip, port);
    }
    return Send_Cmd_Wait(args, ESPOK, 20);
}

u8 set_trans_mode(u8 mode)
{
    if (mode & 0xff) {
        return Send_Cmd_Wait(EspArguments[5], ESPOK, 20);
    } else {
        return Send_Cmd_Wait(EspArguments[6], ESPOK, 20);
    }
}

u8 start_trans(u8* data, u8 mode)
{
    u8 args[32];
    if (mode & 0xff) { // client
        Close_Trans();
        mode = Send_Cmd_Wait(EspArguments[13], ESPOK, 20);
        u2printf("%s", data);
        return mode;
    } else {
        sprintf((char*)args, (char*)EspArguments[15], strlen((char*)data) + 1);
        Delay_us(200);
        return esp_send_bytes(args, ESPOK, 200);
    }
}

void Esp_Usart_Config(u32 bound) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4/*TX,PA4*/);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_5/*RX,PA5*/);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    USART_InitStructure.USART_BaudRate = bound; //baud rate
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //
    USART_InitStructure.USART_Parity = USART_Parity_No; //
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //
    USART_Init(USART2, &USART_InitStructure); //

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART2, ENABLE); //
}

void ESP8266_Init(u32 bound)
{
    Esp_Usart_Config(bound);
}

u8 ESP8266_Test()
{
    return at_test() & close_echo() & set_as_client();
}
