#include "oled.h"
#include "stm32f1xx_hal_gpio.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

//定义像素个数缓冲区，横128，竖64
//一字节控制8个像素->8*8=64
uint8_t OLED_DisplayBuf[8][128];

void OLED_W_SCL(uint8_t val)
{
	/*根据BitValue的值，将SCL置高电平或者低电平*/

	GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)BitValue);
    HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (GPIO_PinState)val);
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/

}

void OLED_W_SDA(uint8_t BitValue)
{
	/*根据BitValue的值，将SDA置高电平或者低电平*/
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/

}
