#include "main.h"
#include "tim.h"

int16_t Encoder_Get(uint8_t n)
{
    int16_t Temp;
    if (n == 1)
    {
        Temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim3); // 读取计数器
        __HAL_TIM_SET_COUNTER(&htim3, 0);               // 清零计数器
        return Temp;
    }
    else if (n == 2)
    {
        Temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        return Temp;
    }
    return 0;
}
