#ifndef USART_NVIC_DMA_H_INCLUDED
#define USART_NVIC_DMA_H_INCLUDED

#include <stm32.h>
#include <gpio.h>

// USART defines
#define USART_Mode_Rx_Tx (USART_CR1_RE | USART_CR1_TE)
#define USART_Enable USART_CR1_UE
#define USART_WordLength_8b 0x0000
#define USART_WordLength_9b USART_CR1_M
#define USART_Parity_No 0x0000
#define USART_Parity_Even USART_CR1_PCE
#define USART_Parity_Odd (USART_CR1_PCE | USART_CR1_PS)
#define USART_StopBits_1 0x0000
#define USART_StopBits_0_5 0x1000
#define USART_StopBits_2 0x2000
#define USART_StopBits_1_5 0x3000
#define USART_FlowControl_None 0x0000
#define USART_FlowControl_RTS USART_CR3_RTSE
#define USART_FlowControl_CTS USART_CR3_CTSE
#define HSI_HZ 16000000U
#define PCLK1_HZ HSI_HZ

void configureUSART();
void configureNVIC();
void configureDMA();

#endif