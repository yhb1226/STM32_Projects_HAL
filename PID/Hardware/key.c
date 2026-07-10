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

    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
    //HAL_Delay(10);
    if(HAL_GPIO_ReadPin(Column1_GPIO_Port, Column1_Pin) == 0){number = 1;}
    if(HAL_GPIO_ReadPin(Column2_GPIO_Port, Column2_Pin) == 0){number = 2;}
    

    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);
    //HAL_Delay(10);
    if(HAL_GPIO_ReadPin(Column1_GPIO_Port, Column1_Pin) == 0){number = 3;}
    if(HAL_GPIO_ReadPin(Column2_GPIO_Port, Column2_Pin) == 0){number = 4;}

    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);
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
