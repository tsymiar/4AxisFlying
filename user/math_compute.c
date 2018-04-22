/**
  ******************************************************************************
  * @file    math_compute.c
  * @author  TsyQi
  * @version V1.0.0
  * @date    5-March-2015
  * @brief   Many extrnal applications wrote mainly by myself. 
	* Including calculating functions such as euler single translation, PID controlation etc.
  ******************************************************************************
**/

#include "math_compute.h"

//#define PI                         (float)     3.14159265f
#define Fly_Enable 1			//cmd to flying

S_FLOAT_ANGLE Q_ANGLE;
attitude_error G_Attitude_Error;
data_filter Data_Filter;

float Q[4]={0.0f};
float g_motor1_PWM, g_motor2_PWM, g_motor3_PWM, g_motor4_PWM;
float Base_PWM=0, Max_PWM=255;

float Pitch_I_Min=20.0, Pitch_I_Max, Roll_I_Max;

float PID_Pitch_Kp, PID_Pitch_Ki, PID_Pitch_Kd, PID_Roll_Kp, PID_Roll_Ki, PID_Roll_Kd, PID_Yaw_Kp;

//construct to output posture angle
/* static float P[2][2] = {
       { 1, 0 },              
       { 0, 1 }
      };
*/
float radpers = PI / 180;

/**
  * @brief  To fix the sensor data after I finished installing perpheral.
	* @param  Here may be, not complete.
  * @retval None.
  */
void Data_prepare(void)
{  

}

/**
  * @brief  Transform quaternion and eulerian angle mutually.
	* @param  acce_: accelerator measure, gyro_: gyroscope measure, magn_:magnetometer measure.
  * @retval None.
  */
void Trans_to_euler(float acce_x, float acce_y, float acce_z, float gyro_x, float gyro_y, float gyro_z, float magn_x, float magn_y, float magn_z)
{
  float norm;
  float refe_x, refe_y, refe_z, upsh_x, upsh_z;			//refe_:reference value, upsh_:
  float dire_x, dire_y, dire_z, flux_x, flux_y, flux_z;
  float cser_x, cser_y, cser_z, halfT;
  static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;	
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;   
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
 
  float delta_2=0;

  const static float FACTOR = 0.002;

  norm = sqrt(acce_x*acce_x + acce_y*acce_y + acce_z*acce_z);       
  acce_x /= norm;
  acce_y /= norm;
  acce_z /= norm;

  norm = sqrt(magn_x*magn_x + magn_y*magn_y + magn_z*magn_z);          
  magn_x /= norm;
  magn_y /= norm;
  magn_z /= norm;
	
	//---http://en.wikipedia.org/wiki/Cross_product 	Mnemonic
  // compute reference direction of flux
  refe_x = 2*magn_x*(0.5f - q2q2 - q3q3) + 2*magn_y*(q1q2 - q0q3) + 2*magn_z*(q1q3 + q0q2);
  refe_y = 2*magn_x*(q1q2 + q0q3) + 2*magn_y*(0.5f - q1q1 - q3q3) + 2*magn_z*(q2q3 - q0q1);
  refe_z = 2*magn_x*(q1q3 - q0q2) + 2*magn_y*(q2q3 + q0q1) + 2*magn_z*(0.5f - q1q1 - q2q2);       
  upsh_x = sqrt((refe_x*refe_x) + (refe_y*refe_y));
  upsh_z = refe_z;
  
  //---estimated direction of gravity and flux (dire_ and flux_)
  dire_x = 2*(q1q3 - q0q2);
  dire_y = 2*(q0q1 + q2q3);
  dire_z = q0q0 - q1q1 - q2q2 + q3q3;
	// flux_x = (float)(2*upsh_x*(0.5 - q2q2 - q3q3) + 2*upsh_z*(q1q3 - q0q2));
  flux_y = 2*upsh_x*(q1q2 - q0q3) + 2*upsh_z*(q0q1 + q2q3);
	// flux_z = (float)(2*upsh_x*(q0q2 + q1q3) + 2*upsh_z*(0.5 - q1q1 - q2q2));  
  
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  cser_x = (acce_y*dire_z - acce_z*dire_y) + (magn_y*flux_z - magn_z*flux_y);
  cser_y = (acce_z*dire_x - acce_x*dire_z) + (magn_z*flux_x - magn_x*flux_z);
  cser_z = (acce_x*dire_y - acce_y*dire_x) + (magn_x*flux_y - magn_y*flux_x);

	/**	.   1      
		*	q = - * q X Omega 	  left, .q=1/quaternion, 'X' is rule of quaternion multiplication, Omega£ºvalue of gyroscope(palstance).
		*	    2
		*	 .
		* [q0] 		[0	-wx	-wy	-wz]		[q0]
		*	 .				
		*	[q1]		[wx	0		wz  -wy]		[q1]
		*	 .   =  			     				* 
		*	[q2]		[wy	-wz	0		wx ]		[q2]
		*	 . 			
		*	[q3]		[wz	wy	-wx		0]		[q3]
		*/
	// halfT=GET_NOWTIME();			  //---interval seconds after last calculation¡£

  gyro_x = gyro_x + cser_x*FACTOR/halfT; 					//---fix gyroscope data/let cross product erorr fix gyorscope zero drift of PI.						
  gyro_y = gyro_y + cser_y*FACTOR/halfT; 
  gyro_z = gyro_z + cser_z*FACTOR/halfT;	 
	  
  delta_2=(2*halfT*gyro_x)*(2*halfT*gyro_x)+(2*halfT*gyro_y)*(2*halfT*gyro_y)+(2*halfT*gyro_z)*(2*halfT*gyro_z);	
 
  q0 = (1-delta_2/8)*q0 + (-q1*gyro_x - q2*gyro_y - q3*gyro_z)*halfT;			//---conform digit rate of quaternion/differential equation/update arithmetic/second order±Ï¿¨method¡£
  q1 = (1-delta_2/8)*q1 + (q0*gyro_x + q2*gyro_z - q3*gyro_y)*halfT;
  q2 = (1-delta_2/8)*q2 + (q0*gyro_y - q1*gyro_z + q3*gyro_x)*halfT;
  q3 = (1-delta_2/8)*q3 + (q0*gyro_z + q1*gyro_y - q2*gyro_x)*halfT;	
	
/* 												   
  q0 = q0 + (-q1*gyro_x - q2*gyro_y - q3*gyro_z)*halfT;			//---conform digit rate of quaternion/differential equation/update arithmetic/first orderÁú¿âmethod¡£
  q1 = q1 + (q0*gyro_x + q2*gyro_z - q3*gyro_y)*halfT;
  q2 = q2 + (q0*gyro_y - q1*gyro_z + q3*gyro_x)*halfT;
  q3 = q3 + (q0*gyro_z + q1*gyro_y - q2*gyro_x)*halfT;			   	   
*/
										   
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);		//---normaling the quaternion
  q0 /= norm;
  q1 /= norm;
  q2 /= norm;
  q3 /= norm;

	//---transform to euler angle.Z-Y-X:http://www.swarthmore.edu/NatSci/mzucker1/e27/diebel2006attitude.pdf P24, 8.2  Euler Angle Sequence (1,2,3).
  Q_ANGLE.Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 180 / PI;
  Q_ANGLE.Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1) / radpers;
  Q_ANGLE.Yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, q0 * q0 + q1 * q1 - q2*q2 - q3 * q3) / radpers;
}

/**
  * @brief  Kalman Filter.
	* @param  angle_m:angle measure, sensor_m: sensor(accelerator,gyroscope,magnetometer) measure.
  * @retval No return value.
  */
void Kalman_Filter(float angle_m,float sensor_m)   
{    
	
}

/**
  * @brief  implement PID arithmetic.
	* @param  Expe_X:expect value.
  * @retval None.
  */
void PID_Ctrl(const float Expe_Pitch, const float Expe_Roll, const float Expe_Yaw)
{
	//---PID_Pitch_Kp, PID_Pitch_Ki, PID_Pitch_Kd, PID_Roll_Kp, PID_Roll_Ki, PID_Roll_Kd, PID_Yaw_Kp...Data_Filter.gyro_x, Data_Filter.gyro_x...PITCH_I_MAX, ROLL_I_MAX, g_motor1_PWM, g_motor2_PWM, g_motor3_PWM, g_motor4_PWM, g_BasePWM, g_MaxPWM...

	s16 outputPWM_Pitch, outputPWM_Roll, outputPWM_Yaw;
	
	//---by using the expect to decrease current value to get attitude error.
	G_Attitude_Error.error_Pitch = Expe_Pitch - Q_ANGLE.Pitch;
	G_Attitude_Error.error_Roll = Expe_Roll - Q_ANGLE.Roll;
	G_Attitude_Error.error_Yaw = Expe_Yaw - Q_ANGLE.Yaw;
	
	//---if dip angle out of expect, don't take action.
	if (fabs(G_Attitude_Error.error_Pitch) >= 55 || fabs(G_Attitude_Error.error_Roll) >= 55)
	{
		Lamp_CatchSignal(4, 1);								//toggle a blue signal lamp
		PWM_Set(0, 0, 0, 0);   				//power down
		return ;
	}
		Lamp_CatchSignal(4, 0);								//lamp off
	
	//---stable pilot lamp, yellow.if the angle do not bigger than 3¡ã, charge as stable on the whole, LED on.
	if (fabs(G_Attitude_Error.error_Pitch) <= 3 && fabs(G_Attitude_Error.error_Roll) <= 3)
		Lamp_CatchSignal(3, 1);
	else
		Lamp_CatchSignal(3, 0);
	
	//---calculate integral, error limiting.
	if (fabs(G_Attitude_Error.error_Pitch) <= Pitch_I_Min)			//---when the attitude error value smaller than 20¡ã, immit integral control.
	{	
			G_Attitude_Error.Integ_Pitch += G_Attitude_Error.error_Pitch;

		if (G_Attitude_Error.Integ_Pitch >= Pitch_I_Max)
			G_Attitude_Error.Integ_Pitch = Pitch_I_Max;
		
		else if (G_Attitude_Error.Integ_Pitch <= -Pitch_I_Max)
			G_Attitude_Error.Integ_Pitch = -Pitch_I_Max;	
	}
	if (fabs(G_Attitude_Error.error_Roll) <= Pitch_I_Min)
	{		
			G_Attitude_Error.Integ_Roll += G_Attitude_Error.error_Roll;
		
		if (G_Attitude_Error.Integ_Roll >= Roll_I_Max)
			G_Attitude_Error.Integ_Roll = Roll_I_Max;
		
		else if (G_Attitude_Error.Integ_Roll <= -Roll_I_Max)
			G_Attitude_Error.Integ_Roll = -Roll_I_Max;		
	}
	//---PID-->here, directly used the gyroscope data to replace the differential item, instead of using the D-value between current error and last error.
	//---it effects well, by using the most of hardwares, while gyroscope can increment number itself after all.
	outputPWM_Pitch = (s16)(PID_Pitch_Kp * G_Attitude_Error.error_Pitch + 
										PID_Pitch_Ki * G_Attitude_Error.Integ_Pitch - PID_Pitch_Kd * Data_Filter.gyro_fiX);
	outputPWM_Roll = (s16)(PID_Roll_Kp * G_Attitude_Error.error_Roll + 
										PID_Roll_Ki * G_Attitude_Error.Integ_Roll - PID_Roll_Kd * Data_Filter.gyro_fiY);
	//Yaw data use the magnetometer.
	outputPWM_Yaw = (s16)(PID_Yaw_Kp * G_Attitude_Error.error_Yaw);

        //---output PWM signal to 4 motos-->X-mode.
				//Warning:output is opposite, because error was oppsite either. 
        g_motor1_PWM = Base_PWM + outputPWM_Pitch + outputPWM_Roll + outputPWM_Yaw;
        g_motor2_PWM = Base_PWM - outputPWM_Pitch + outputPWM_Roll - outputPWM_Yaw;
        g_motor3_PWM = Base_PWM - outputPWM_Pitch - outputPWM_Roll + outputPWM_Yaw;
        g_motor4_PWM = Base_PWM + outputPWM_Pitch - outputPWM_Roll - outputPWM_Yaw;

	//---no oppsite motion, clear PWM
	if (g_motor1_PWM < 0)
		g_motor1_PWM = 0;
	if (g_motor2_PWM < 0)
		g_motor2_PWM = 0;
	if (g_motor3_PWM < 0)
		g_motor3_PWM = 0;
	if (g_motor4_PWM < 0)
		g_motor4_PWM = 0;
	
	//---PWM limitation.
	if (g_motor1_PWM >= Max_PWM)
		g_motor1_PWM = Max_PWM;
	if (g_motor2_PWM >= Max_PWM)
		g_motor2_PWM = Max_PWM;
	if (g_motor3_PWM >= Max_PWM)
		g_motor3_PWM = Max_PWM;
	if (g_motor4_PWM >= Max_PWM)
		g_motor4_PWM = Max_PWM;
		
	if (Fly_Enable)			//---flying
		PWM_Set(g_motor1_PWM, g_motor2_PWM, g_motor3_PWM, g_motor4_PWM);
	else
		PWM_Set(0, 0, 0, 0);
}
