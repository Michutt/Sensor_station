/*
 * DHT11.h
 *
 *  Created on: 17 lut 2021
 *      Author: niemc
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stdint.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

void pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void delay(uint32_t delay);
void DHT11_Init();
void DHT11_Receive_Data();
uint8_t DHT11_Check_Response();
uint8_t DHT11_Read_Byte();

#endif /* INC_DHT11_H_ */
