#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

void Motor_SetPWM(uint8_t n, int8_t PWM)
{
	if (n == 1)			//指定左电机
	{
		if (PWM >= 0)							//如果设置正转的PWM
		{
            HAL_GPIO_WritePin(MOTOR_LEFT1_GPIO_Port, MOTOR_LEFT1_Pin, GPIO_PIN_SET);//PB12置高电平
            HAL_GPIO_WritePin(MOTOR_LEFT2_GPIO_Port, MOTOR_LEFT2_Pin, GPIO_PIN_RESET);//PB13置低电平
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM);//设置PWM占空比
		}
		else									//否则，即设置反转的PWM
		{
			HAL_GPIO_WritePin(MOTOR_LEFT1_GPIO_Port, MOTOR_LEFT1_Pin, GPIO_PIN_RESET);	//PB12置低电平
			HAL_GPIO_WritePin(MOTOR_LEFT2_GPIO_Port, MOTOR_LEFT2_Pin, GPIO_PIN_SET);	//PB13置高电平
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM);//设置PWM占空比
		}
	}
	else if (n == 2)	//指定右电机
	{
		if (PWM >= 0)							//如果设置正转的PWM
		{
			HAL_GPIO_WritePin(MOTOR_RIGHT1_GPIO_Port, MOTOR_RIGHT1_Pin, GPIO_PIN_RESET);	//PB14置低电平
			HAL_GPIO_WritePin(MOTOR_RIGHT2_GPIO_Port, MOTOR_RIGHT2_Pin, GPIO_PIN_SET);	//PB15置高电平
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM);				//设置PWM占空比
		}
		else									//否则，即设置反转的PWM
		{
			HAL_GPIO_WritePin(MOTOR_RIGHT1_GPIO_Port, MOTOR_RIGHT1_Pin, GPIO_PIN_SET);	//PB14置高电平
			HAL_GPIO_WritePin(MOTOR_RIGHT2_GPIO_Port, MOTOR_RIGHT2_Pin, GPIO_PIN_RESET);	//PB15置低电平
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM);				//设置PWM占空比
		}
	}
}
