#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

void IIC_W_SCL(uint8_t value)
{
    HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, (GPIO_PinState)value);
}

void IIC_W_SDA(uint8_t value)
{
    HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, (GPIO_PinState)value);
}

//在主机发送完数据后，从机会有个ACK的应答位，因此需要SDA来读取这个应答位
uint8_t IIC_R_SDA(void)
{
    uint8_t Bitvalue;
    Bitvalue = HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin);
    return Bitvalue;
}

void IIC_Init(void)
{
    HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
}

//IIC开始
void IIC_Start(void)
{
	IIC_W_SDA(1);							//释放SDA，确保SDA为高电平
	IIC_W_SCL(1);							//释放SCL，确保SCL为高电平
	IIC_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
	IIC_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

//IIC终止
void IIC_Stop(void)
{
	IIC_W_SDA(0);							//拉低SDA，确保SDA为低电平
	IIC_W_SCL(1);							//释放SCL，使SCL呈现高电平
	IIC_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
}

//IIC发送一个字节   范围：0x00~0xFF
void IIC_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		/*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
		IIC_W_SDA(!!(Byte & (0x80 >> i)));//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		IIC_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		IIC_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}

//IIC接收一个字节   范围：0x00~0xFF
uint8_t IIC_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	IIC_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		IIC_W_SCL(1);						//释放SCL，主机机在SCL高电平期间读取SDA
		if (IIC_R_SDA()){Byte |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
													//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		IIC_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

//IIC发送应答位 范围：0~1，0表示应答，1表示非应答
void IIC_SendAck(uint8_t AckBit)
{
	IIC_W_SDA(AckBit);	//				//主机把应答位数据放到SDA线
	IIC_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	IIC_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

//IIC接收应答位 范围：0~1，0表示应答，1表示非应答
uint8_t IIC_ReceiveAck(void)
{
	uint8_t AckBit;							//定义应答位变量
	IIC_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	IIC_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = IIC_R_SDA();					//将应答位存储到变量里
	IIC_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}