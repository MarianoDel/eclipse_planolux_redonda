/**
  ******************************************************************************
  * @file    Template_2/stm32f0_uart.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   UART functions.
  ******************************************************************************
  * @attention
  *
  * Use this functions to configure serial comunication interface (UART).
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hard.h"
#include "stm32f0xx.h"
#include "uart.h"

#include <string.h>




//--- Private typedef ---//
//--- Private define ---//
//--- Private macro ---//

//#define USE_USARTx_TIMEOUT



//--- VARIABLES EXTERNAS ---//

//extern volatile unsigned char Packet_Detected_Flag;
//extern volatile unsigned char dmx_receive_flag;
//extern volatile unsigned short DMX_channel_received;
//extern volatile unsigned short DMX_channel_selected;
//extern volatile unsigned char DMX_channel_quantity;
//extern volatile unsigned char data1[];
////static unsigned char data_back[10];
//extern volatile unsigned char data[];


//#define data512		data1		//en rx es la trama recibida; en tx es la trama a enviar
//#define data256		data		//en rx son los valores del channel elegido
//volatile unsigned char * pdmx;

extern volatile unsigned char tx1buff[];
extern volatile unsigned char rx1buff[];

extern volatile unsigned char tx2buff[];
extern volatile unsigned char rx2buff[];

//--- Private variables ---//
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;

volatile unsigned char * ptx2;
volatile unsigned char * ptx2_pckt_index;

//Reception buffer.

//Transmission buffer.

//--- Private function prototypes ---//
//--- Private functions ---//


void USART1_IRQHandler(void)
{
	unsigned char dummy;

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART1->ISR & USART_ISR_RXNE)
	{

		//RX DMX
		dummy = USART1->RDR & 0x0FF;

	}

	/* USART in mode Transmitter -------------------------------------------------*/

	if (USART1->CR1 & USART_CR1_TXEIE)
	{
		if (USART1->ISR & USART_ISR_TXE)
		{
			if ((ptx1 < &tx1buff[SIZEOF_DATA]) && (ptx1 < ptx1_pckt_index))
			{
				USART2->TDR = *ptx1;
				ptx1++;
			}
			else
			{
				ptx1 = tx1buff;
				ptx1_pckt_index = tx1buff;
				USART1->CR1 &= ~USART_CR1_TXEIE;
			}
		}
	}

	if ((USART1->ISR & USART_ISR_ORE) || (USART1->ISR & USART_ISR_NE) || (USART1->ISR & USART_ISR_FE))
	{
		USART1->ICR |= 0x0e;
		dummy = USART1->RDR;
	}
}

void USART2_IRQHandler(void)
{
	unsigned char dummy;

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART2->ISR & USART_ISR_RXNE)
	{
		//RX WIFI
		dummy = USART2->RDR & 0x0FF;


	}

	/* USART in mode Transmitter -------------------------------------------------*/

	if (USART2->CR1 & USART_CR1_TXEIE)
	{
		if (USART2->ISR & USART_ISR_TXE)
		{
			if ((ptx2 < &tx2buff[SIZEOF_DATA]) && (ptx2 < ptx2_pckt_index))
			{
				USART2->TDR = *ptx2;
				ptx2++;
			}
			else
			{
				ptx2 = tx2buff;
				ptx2_pckt_index = tx2buff;
				USART2->CR1 &= ~USART_CR1_TXEIE;
			}
		}
	}

	if ((USART2->ISR & USART_ISR_ORE) || (USART2->ISR & USART_ISR_NE) || (USART2->ISR & USART_ISR_FE))
	{
		USART2->ICR |= 0x0e;
		dummy = USART2->RDR;
	}
}

void Usart2Send (char * send)
{
	unsigned char i;

	i = strlen(send);
	Usart2SendUnsigned((unsigned char *) send, i);
}

void Usart2SendUnsigned(unsigned char * send, unsigned char size)
{
	if ((ptx2_pckt_index + size) < &tx2buff[SIZEOF_DATA])
	{
		memcpy((unsigned char *)ptx2_pckt_index, send, size);
		ptx2_pckt_index += size;
		USART2->CR1 |= USART_CR1_TXEIE;
	}
}

void Usart1Send (char * send)
{
	unsigned char i;

	i = strlen(send);
	Usart1SendUnsigned((unsigned char *) send, i);
}

void Usart1SendUnsigned(unsigned char * send, unsigned char size)
{
	if ((ptx1_pckt_index + size) < &tx1buff[SIZEOF_DATA])
	{
		memcpy((unsigned char *)ptx1_pckt_index, send, size);
		ptx1_pckt_index += size;
		USART1->CR1 |= USART_CR1_TXEIE;
	}
}



void USART2Config(void)
{
	if (!USART2_CLK)
		USART2_CLK_ON;

	GPIOA->AFR[0] |= 0x0001100;	//PA2 -> AF1 PA3 -> AF1

	ptx2 = tx2buff;
	ptx2_pckt_index = tx2buff;
	//prx2 = rx2buff;

	USART2->BRR = USART_115200;
	USART2->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 7);
}

void USART1Config(void)
{
	if (!USART1_CLK)
		USART1_CLK_ON;

#ifdef VER_1_0
	//para empezar con el GPS
	GPIOB->AFR[0] |= 0x00000000;	//PB7 -> AF0 PB6 -> AF0
	//para empezar con el GSM
	GPIOA->AFR[1] |= 0x00000110;	//PA10 -> AF1 PA9 -> AF1
#endif

	ptx1 = tx1buff;
	ptx1_pckt_index = tx1buff;
	//prx1 = rx1buff;

	USART1->BRR = USART_9600;
//	USART1->CR2 |= USART_CR2_STOP_1;	//2 bits stop
//	USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
//	USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
	USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 5);
}


//--- end of file ---//
