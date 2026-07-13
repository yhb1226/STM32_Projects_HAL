#include "main.h"
#include "key.h"
#include "stm32f1xx_hal_gpio.h"

uint8_t key_number = 0;
uint8_t key_buf = 0;     
uint8_t key_stable = 0;    
uint8_t key_last = 0;      

uint8_t Key_GetState(void)
{
    uint8_t number = 0;

    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0){number = 1;}
    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0){number = 2;}
    if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == 0){number = 3;}
    if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == 0){number = 4;}

     return number;
}

void Key_loop(void)
{
    key_buf = Key_GetState(); 

    if (key_buf == key_stable)
    {
        if (key_last == 0 && key_stable != 0)
        {
            key_number = key_stable; 
        }
        key_last = key_stable; 
    }
    else
    {
        key_stable = key_buf; 
    }
}

uint8_t key_get(void)
{
    uint8_t Number;
    Number = key_number;
    key_number = 0;
    return Number;
}
