/*以下函数为0-180度舵机角度控制测试函数
	*	PWM 信号与0-180舵机的关系：
	*	0.5ms ---------------- 0度
	*	1ms   ---------------- 45度
	*	1.5ms ---------------- 90度
	*	2ms   ---------------- 135度
	*	2.5ms ---------------- 180度

	*	舵机频率与占空比的计算：
	*	设舵机的频率为50HZ，则PWM周期为20ms，0度对应的占空比为2.5%，即0.05ms的高电平输出。
 */

#include "motor.h"
#include "main.h"

extern TIM_HandleTypeDef htim14;

void SetMotorAngle(int angle)
{
	int Setvalue = 1.1*angle+50;
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, Setvalue);
}
