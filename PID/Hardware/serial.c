#include "main.h"          // 包含 HAL 库和 huart1 句柄
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usart.h"

/* 全局变量 */
volatile uint8_t Serial_RxData;    // 接收到的数据
volatile uint8_t Serial_RxFlag;   // 接收完成标志

/* 发送一个字节（HAL 库自带，直接封装即可） */
void Serial_SendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

/* 发送数组 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

/* 发送字符串 */
void Serial_SendString(char *String)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)String, strlen(String), HAL_MAX_DELAY);
}

/* 计算整数次幂（保持不变） */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/* 发送数字 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/* printf 重定向到串口（HAL 库推荐方式） */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* 格式化打印（可直接使用 HAL 版本的 printf 重定向，这个函数可保留） */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsnprintf(String, sizeof(String), format, arg); // 建议用 vsnprintf 防溢出
    va_end(arg);
    Serial_SendString(String);
}

/* 接收完成标志 */
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

/* 获取接收数据 */
uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

/* 接收中断回调（自动由 HAL_UART_IRQHandler 调用） */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Serial_RxData = huart->Instance->DR;  // 或直接使用全局接收缓冲区
        Serial_RxFlag = 1;
        // 重新启动中断接收，准备接收下一个字节
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&Serial_RxData, 1);
    }
}