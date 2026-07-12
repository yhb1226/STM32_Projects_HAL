#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#define BLUESERIAL_RX_PACKET_SIZE  100
#include <stdio.h>
#include "stdint.h"

// 外部变量
extern char BlueSerial_RxPacket[];
extern volatile uint8_t BlueSerial_RxFlag;

// 发送函数
void BlueSerial_SendByte(uint8_t Byte);
void BlueSerial_SendArray(uint8_t *Array, uint16_t Length);
void BlueSerial_SendString(char *String);
void BlueSerial_SendNumber(uint32_t Number, uint8_t Length);
void BlueSerial_Printf(char *format, ...);

#endif