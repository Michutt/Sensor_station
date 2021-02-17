/*
 * DHT11.c
 *
 *  Created on: 17 lut 2021
 *      Author: niemc
 */
#include "DHT11.h"

#define PORT GPIOA
#define PIN GPIO_PIN_0

void pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void delay(uint32_t delay)
{
	delay = delay * 0.8;
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
}

void DHT11_Init()
{
	pin_Output(PORT, PIN);
	HAL_GPIO_WritePin(PORT, PIN, 0);
	HAL_Delay(18); // 18ms
	HAL_GPIO_WritePin(PORT, PIN, 1);
 	delay(3000); // 30us
 	pin_Input(PORT, PIN);
}

uint8_t DHT11_Check_Response()
{
	uint8_t state = 0;
	delay(4000); // 40us
	if (!HAL_GPIO_ReadPin(PORT, PIN))
		state = 1;
	delay(8000); // 80us
	if (!HAL_GPIO_ReadPin(PORT, PIN) && state)
		state = 0;
	while (HAL_GPIO_ReadPin(PORT, PIN));

	return state;
}

uint8_t DHT11_Read_Byte()
{
	uint8_t i, data;
	for (i=0; i<8; i++)
	{
		while (!HAL_GPIO_ReadPin(PORT, PIN));
		delay(4000); // 40us
		if (HAL_GPIO_ReadPin(PORT, PIN))
			data |= (1 << (7-i));
		else
			data &= ~(1 << (7-i));
		while(HAL_GPIO_ReadPin(PORT, PIN));
	}
	return data;
}

void DHT11_Receive_Data()
{
	DHT11_Init();
	if (DHT11_Check_Response())
	{
		uint8_t humM = DHT11_Read_Byte();
		uint8_t humL = DHT11_Read_Byte();
		uint8_t tempM = DHT11_Read_Byte();
		uint8_t tempL = DHT11_Read_Byte();
		static char data[20];
		sprintf((char *)data, "-%i-%i-%i-%i\r\n", tempM, tempL, humM, humL);
		HAL_UART_Transmit(&huart2, data, sizeof(data), 100);
	}
}

