#ifndef IMU_VIEW_H
#define IMU_VIEW_H

#include "stdint.h"

void Test_Send_User1(int16_t acc_x_,
                     int16_t acc_y_,
                     int16_t acc_z_,
                     int16_t gyro_x_,
                     int16_t gyro_y_,
                     int16_t gyro_z_,
                     int16_t roll_,
                     int16_t pitch_,
                     int16_t yaw_);

void Vofa_view(int16_t roll_,int16_t pitch_,int16_t yaw_);

#endif
