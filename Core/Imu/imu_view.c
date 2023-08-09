#include "stdint.h"
#include "imu_view.h"
#include "main.h"
#include "usart.h"

#define RESET (0)

/* 发送字节数据到UART */
void usart1_SendByte(uint8_t data)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
} 

/****************** 发送八位数组************************/
void usart1_SendArray(uint8_t *array, uint16_t num)
{
    uint8_t i;
    for (i = 0; i < num; i++)
    { /* ·发送一个字节数据到USART */
        usart1_SendByte(array[i]);
    }
}

uint8_t testdatatosend[50]; /*波形显示函数*/

void Vofa_view(int16_t roll_,int16_t pitch_,int16_t yaw_)
{
    printf("%d,%d,%d\n",roll_,pitch_,yaw_);
}

void Test_Send_User(uint16_t data1,
                    uint16_t data2,
                    uint16_t data3,
                    uint16_t data4,
                    uint16_t data5,
                    uint16_t data6,
                    uint16_t data7,
                    uint16_t data8,
                    uint16_t data9,
                    uint16_t data10)
{
    uint8_t _cnt = 0;
    uint8_t sum = 0;
    uint8_t i = 0;

    testdatatosend[_cnt++] = 0x88;
    testdatatosend[_cnt++] = 0xA1;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = data1 >> 8;
    testdatatosend[_cnt++] = data1 & 0xff;
    testdatatosend[_cnt++] = data2 >> 8;
    testdatatosend[_cnt++] = data2 & 0xff;
    testdatatosend[_cnt++] = data3 >> 8;
    testdatatosend[_cnt++] = data3 & 0xff;
    testdatatosend[_cnt++] = data4 >> 8;
    testdatatosend[_cnt++] = data4 & 0xff;
    testdatatosend[_cnt++] = data5 >> 8;
    testdatatosend[_cnt++] = data5 & 0xff;
    testdatatosend[_cnt++] = data6 >> 8;
    testdatatosend[_cnt++] = data6 & 0xff;
    testdatatosend[_cnt++] = data7 >> 8;
    testdatatosend[_cnt++] = data7 & 0xff;
    testdatatosend[_cnt++] = data8 >> 8;
    testdatatosend[_cnt++] = data8 & 0xff;
    testdatatosend[_cnt++] = data9 >> 8;
    testdatatosend[_cnt++] = data9 & 0xff;
    testdatatosend[_cnt++] = data10 >> 8;
    testdatatosend[_cnt++] = data10 & 0xff;
    testdatatosend[2] = _cnt - 3;

    for (i = 0; i < _cnt; i++)
        sum += testdatatosend[i];

    testdatatosend[_cnt++] = sum;

    usart1_SendArray(testdatatosend, _cnt);
}

/******下面的函数主要用于无人机调试，也可用于其他pid调试*********************

 * ******************************/
/*发送陀螺仪数据给上位机*/
void Test_Send_User1(int16_t acc_x_,
                     int16_t acc_y_,
                     int16_t acc_z_,
                     int16_t gyro_x_,
                     int16_t gyro_y_,
                     int16_t gyro_z_,
                     int16_t roll_,
                     int16_t pitch_,
                     int16_t yaw_)
{
    uint8_t _cnt = 0;
    uint8_t sum = 0;
    uint8_t i = 0;

    roll_ = roll_ * 100;
    pitch_ = pitch_ * 100;
    yaw_ = yaw_ * 10;

    testdatatosend[_cnt++] = 0x88;
    testdatatosend[_cnt++] = 0xAF;
    testdatatosend[_cnt++] = 0x1C;
    testdatatosend[_cnt++] = acc_x_ >> 8;
    testdatatosend[_cnt++] = acc_x_ & 0xff;
    testdatatosend[_cnt++] = acc_y_ >> 8;
    testdatatosend[_cnt++] = acc_y_ & 0xff;
    testdatatosend[_cnt++] = acc_z_ >> 8;
    testdatatosend[_cnt++] = acc_z_ & 0xff;
    testdatatosend[_cnt++] = gyro_x_ >> 8;
    testdatatosend[_cnt++] = gyro_x_ & 0xff;
    testdatatosend[_cnt++] = gyro_y_ >> 8;
    testdatatosend[_cnt++] = gyro_y_ & 0xff;
    testdatatosend[_cnt++] = gyro_z_ >> 8;
    testdatatosend[_cnt++] = gyro_z_ & 0xff;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = 0;
    testdatatosend[_cnt++] = roll_ >> 8;
    testdatatosend[_cnt++] = roll_ & 0xff;
    testdatatosend[_cnt++] = pitch_ >> 8;
    testdatatosend[_cnt++] = pitch_ & 0xff;
    testdatatosend[_cnt++] = yaw_ >> 8;
    testdatatosend[_cnt++] = yaw_ & 0xff;
    testdatatosend[_cnt++] = 0x00;
    testdatatosend[_cnt++] = 0x00;
    testdatatosend[_cnt++] = 0x00;
    testdatatosend[_cnt++] = 0x00;
    testdatatosend[2] = _cnt - 3;

    for (i = 0; i < _cnt; i++)
        sum += testdatatosend[i];

    testdatatosend[_cnt++] = sum;

    usart1_SendArray(testdatatosend, _cnt);
} 

/*发送电机pwm和飞控电池电压给上位机*/
void Test_Send_User2(uint16_t throt,
                     uint16_t yaw_2,
                     uint16_t roll_2,
                     uint16_t pitch_2,
                     uint16_t aux_1,
                     uint16_t aux_2,
                     uint16_t aux_3,
                     uint16_t aux_4,
                     uint16_t aux_5,
                     uint16_t pwm1,
                     uint16_t pwm2,
                     uint16_t pwm3,
                     uint16_t pwm4,
                     uint16_t votage)
{
    uint8_t _cnt = 0;
    uint8_t sum = 0;
    uint8_t i = 0;
    testdatatosend[_cnt++] = 0x88;
    testdatatosend[_cnt++] = 0xAE;
    testdatatosend[_cnt++] = 0x12;
    testdatatosend[_cnt++] = throt >> 8;
    testdatatosend[_cnt++] = throt & 0xff;
    testdatatosend[_cnt++] = yaw_2 >> 8;
    testdatatosend[_cnt++] = yaw_2 & 0xff;
    testdatatosend[_cnt++] = roll_2 >> 8;
    testdatatosend[_cnt++] = roll_2 & 0xff;
    testdatatosend[_cnt++] = pitch_2 >> 8;
    testdatatosend[_cnt++] = pitch_2 & 0xff;
    testdatatosend[_cnt++] = aux_1 >> 8;
    testdatatosend[_cnt++] = aux_1 & 0xff;
    testdatatosend[_cnt++] = aux_2 >> 8;
    testdatatosend[_cnt++] = aux_2 & 0xff;
    testdatatosend[_cnt++] = aux_3 >> 8;
    testdatatosend[_cnt++] = aux_3 & 0xff;
    testdatatosend[_cnt++] = aux_4 >> 8;
    testdatatosend[_cnt++] = aux_4 & 0xff;
    testdatatosend[_cnt++] = aux_5 >> 8;
    testdatatosend[_cnt++] = aux_5 & 0xff;
    testdatatosend[_cnt++] = pwm1 >> 8;
    testdatatosend[_cnt++] = pwm1 & 0xff;
    testdatatosend[_cnt++] = pwm2 >> 8;
    testdatatosend[_cnt++] = pwm2 & 0xff;
    testdatatosend[_cnt++] = pwm3 >> 8;
    testdatatosend[_cnt++] = pwm3 & 0xff;
    testdatatosend[_cnt++] = pwm4 >> 8;
    testdatatosend[_cnt++] = pwm4 & 0xff;
    testdatatosend[_cnt++] = votage >> 8;
    testdatatosend[_cnt++] = votage & 0xff;
    testdatatosend[2] = _cnt - 3;

    for (i = 0; i < _cnt; i++)
        sum += testdatatosend[i];
    testdatatosend[_cnt++] = sum;

    usart1_SendArray(testdatatosend, _cnt);
}

/*发送OFFSET给上位机*/
void Test_Send_User3(int16_t acc_x_3,
                     int16_t acc_y_3,
                     int16_t acc_z_3,
                     int16_t gyro_x_3,
                     int16_t gyro_y_3,
                     int16_t gyro_z_3)
{
    uint8_t _cnt = 0;
    uint8_t sum = 0;
    uint8_t i = 0;
    testdatatosend[_cnt++] = 0x88;
    testdatatosend[_cnt++] = 0xAC;
    testdatatosend[_cnt++] = 0x1C;
    testdatatosend[_cnt++] = 0xAC;
    testdatatosend[_cnt++] = acc_x_3 >> 8;
    testdatatosend[_cnt++] = acc_x_3 & 0xff;
    testdatatosend[_cnt++] = acc_y_3 >> 8;   //
    testdatatosend[_cnt++] = acc_y_3 & 0xff; //
    testdatatosend[_cnt++] = acc_z_3 >> 8;
    testdatatosend[_cnt++] = acc_z_3 & 0xff;
    testdatatosend[_cnt++] = gyro_x_3 >> 8;
    testdatatosend[_cnt++] = gyro_x_3 & 0xff;
    testdatatosend[_cnt++] = gyro_y_3 >> 8;
    testdatatosend[_cnt++] = gyro_y_3 & 0xff;
    testdatatosend[_cnt++] = gyro_z_3 >> 8;
    testdatatosend[_cnt++] = gyro_z_3 & 0xff; //
    testdatatosend[2] = _cnt - 3;

    for (i = 0; i < _cnt; i++)
        sum += testdatatosend[i];
    testdatatosend[31] = sum;

    usart1_SendArray(testdatatosend, 32);
} 

// 给上位机发送PID参数*/
void Test_Send_User4(uint16_t rol_p,
                     uint16_t rol_i,
                     uint16_t rol_d,
                     uint16_t pit_p,
                     uint16_t pit_i,
                     uint16_t pit_d,
                     uint16_t yaw_p,
                     uint16_t yaw_i,
                     uint16_t yaw_d)
{
    uint8_t _cnt = 0;
    uint8_t sum = 0;
    uint8_t i = 0;
    testdatatosend[_cnt++] = 0x88;
    testdatatosend[_cnt++] = 0xAC;
    testdatatosend[_cnt++] = 0x1C;
    testdatatosend[_cnt++] = 0xAD;
    testdatatosend[_cnt++] = rol_p >> 8;
    testdatatosend[_cnt++] = rol_p & 0xff;
    testdatatosend[_cnt++] = rol_i >> 8;
    testdatatosend[_cnt++] = rol_i & 0xff;
    testdatatosend[_cnt++] = rol_d >> 8;
    testdatatosend[_cnt++] = rol_d & 0xff;
    testdatatosend[_cnt++] = pit_p >> 8;
    testdatatosend[_cnt++] = pit_p & 0xff;
    testdatatosend[_cnt++] = pit_i >> 8;
    testdatatosend[_cnt++] = pit_i & 0xff;
    testdatatosend[_cnt++] = pit_d >> 8;
    testdatatosend[_cnt++] = pit_d & 0xff;
    testdatatosend[_cnt++] = yaw_p >> 8;
    testdatatosend[_cnt++] = yaw_p & 0xff;
    testdatatosend[_cnt++] = yaw_i >> 8;
    testdatatosend[_cnt++] = yaw_i & 0xff;
    testdatatosend[_cnt++] = yaw_d >> 8;
    testdatatosend[_cnt++] = yaw_d & 0xff;
    testdatatosend[2] = _cnt - 3;

    for (i = 0; i < _cnt; i++)
        sum += testdatatosend[i];
    testdatatosend[31] = sum;

    usart1_SendArray(testdatatosend, 32);
}
