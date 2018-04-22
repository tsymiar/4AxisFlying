#ifndef __MATH_COMPUTE_H
#define __MATH_COMPUTE_H

#include "common_functions.h"
#include "pwm_set.h"

#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4

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

void Trans_to_euler(float acce_x, float acce_y, float acce_z, float gyro_x, float gyro_y, float gyro_z, float magn_x, float magn_y, float magn_z);
								
#endif /* __MATH_COMPUTE_H */
