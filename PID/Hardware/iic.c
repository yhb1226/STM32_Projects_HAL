#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

void IIC_W_SCL(uint8_t value)
{
    HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, (GPIO_PinState)value);
}

void IIC_W_SDA(uint8_t value)
{
    HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, (GPIO_PinState)value)
}

uint8_t IIC_R_SDA(void)
{
    uint8_t Bitvalue;
    Bitvalue = HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)

}