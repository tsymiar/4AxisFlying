#ifndef __MATH_CLAC_H
#define __MATH_CLAC_H

#include "func_utils.h"

/* Public define ------------------------------------------------------------*/
#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4
#define _PI_            (float)     3.14159265f
#define ABS(x)         (x < 0) ? (-x) : x   // get the absolute value.

typedef struct float_angle{
				float Pitch;
				float Roll;
				float Yaw;	
				}S_FLOAT_ANGLE;
typedef struct {
				float error_Pitch;
				float Integ_Pitch;
				float error_Roll;
				float Integ_Roll;
				float error_Yaw;
				float Integ_Yaw;
								} attitude_error;
typedef struct {
				float gyro_fiX;
				float gyro_fiY;
								} data_filter;
extern S_FLOAT_ANGLE Q_ANGLE;
extern attitude_error G_Attitude_Error;
extern void PWM_Set(int moto1, int moto2, int moto3, int moto4);

void Trans_to_euler(float acce_x, float acce_y, float acce_z, float gyro_x, float gyro_y, float gyro_z, float magn_x, float magn_y, float magn_z);
								
#endif /* __MATH_CLAC_H */
