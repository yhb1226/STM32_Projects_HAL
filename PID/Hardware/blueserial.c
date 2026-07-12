#include "blueserial.h"   // 用自己对应的头文件
#include "main.h"        // 包含 huart1
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usart.h"

// ------------------------------
// 全局变量定义
// ------------------------------
char BlueSerial_RxPacket[100];
volatile uint8_t BlueSerial_RxFlag;
volatile uint8_t BlueSerial_RxByte;   // 单字节接收缓冲区

// ------------------------------
// 发送函数
// ------------------------------
void BlueSerial_SendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

void BlueSerial_SendArray(uint8_t *Array, uint16_t Length)
{
    HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

void BlueSerial_SendString(char *String)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)String, strlen(String), HAL_MAX_DELAY);
}

uint32_t BlueSerial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) Result *= X;
    return Result;
}

void BlueSerial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        BlueSerial_SendByte(Number / BlueSerial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void BlueSerial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsnprintf(String, sizeof(String), format, arg);
    va_end(arg);
    BlueSerial_SendString(String);
}
