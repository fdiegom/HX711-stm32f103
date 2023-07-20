#include "stm32f10x.h"
#include "usart_setup.h"
#include <math.h>
#include <stdint.h>

/*
  USART1
		TX1 --> PA9		(OUT)
		RX1 --> PA10	(IN)
		
	USART2
		TX2 --> PA2		(OUT)
		RX2 --> PA3		(IN)
		
	USART3
		TX3 --> PB10	(OUT)
		RX3 --> PB11	(IN)
*/

float ussart_div   = 0;
int   div_M = 0, div_F = 0;

void usart_init(int usart_port, int baud_rate, int rx_int_en)
{
	if(usart_port == 1)
	{
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

		// A9  como TX de USART1 
		GPIOA->CRH |=  GPIO_CRH_MODE9;
		GPIOA->CRH |=  GPIO_CRH_CNF9_1;
		GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
		// A10 como RX de USART1 
		GPIOA->CRH &= ~GPIO_CRH_MODE10;
		GPIOA->CRH |=  GPIO_CRH_CNF10_1;
		GPIOA->CRH &= ~GPIO_CRH_CNF10_0;
		
		ussart_div = 72000000.0/(16.0*baud_rate);
		div_M = 72000000/(16*baud_rate);
		div_F = ceil(16.0*(ussart_div - div_M));
		
		USART1->BRR = ((div_M<<4)|div_F);
		USART1->CR1 |= USART_CR1_TE;
		USART1->CR1 |= USART_CR1_RE;
		USART1->CR1 |= USART_CR1_UE;
		if(rx_int_en == 1)
		{
				USART1->CR1 |= USART_CR1_RXNEIE;
				NVIC_EnableIRQ(USART1_IRQn);
		}	
	}
	if(usart_port == 2)
	{
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		
		// A2 como TX de USART2 
		GPIOA->CRL |=  GPIO_CRL_MODE2;
		GPIOA->CRL |=  GPIO_CRL_CNF2_1;
		GPIOA->CRL &= ~GPIO_CRL_CNF2_0;
		// A3 como RX de USART2
		GPIOA->CRL &= ~GPIO_CRL_MODE3;
		GPIOA->CRL |=  GPIO_CRL_CNF3_1;
		GPIOA->CRL &= ~GPIO_CRL_CNF3_0;
		
		ussart_div = 36000000.0/(16.0*baud_rate);
		div_M = 36000000/(16*baud_rate);
		div_F = ceil(16.0*(ussart_div - div_M));
		
		USART2->BRR = ((div_M<<4)|div_F);
		USART2->CR1 |= USART_CR1_TE;
		USART2->CR1 |= USART_CR1_RE;
		USART2->CR1 |= USART_CR1_UE;		
		if(rx_int_en == 1)
		{
				USART2->CR1 |= USART_CR1_RXNEIE;
				NVIC_EnableIRQ(USART2_IRQn);
		}	
	}
	if(usart_port == 3)
	{
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

		// B10 como TX de USART3 
		GPIOB->CRH |=  GPIO_CRH_MODE10;
		GPIOB->CRH |=  GPIO_CRH_CNF10_1;
		GPIOB->CRH &= ~GPIO_CRH_CNF10_0;
		// B11 como RX de USART3
		GPIOB->CRH &= ~GPIO_CRH_MODE11;
		GPIOB->CRH |=  GPIO_CRH_CNF11_1;
		GPIOB->CRH &= ~GPIO_CRH_CNF11_0;
		
		ussart_div = 36000000.0/(16.0*baud_rate);
		div_M = 36000000/(16*baud_rate);
		div_F = ceil(16.0*(ussart_div - div_M));
		
		USART3->BRR = ((div_M<<4)|div_F);
		USART3->CR1 |= USART_CR1_TE;
		USART3->CR1 |= USART_CR1_RE;
		USART3->CR1 |= USART_CR1_UE;		
		if(rx_int_en == 1)
		{
				USART3->CR1 |= USART_CR1_RXNEIE;
				NVIC_EnableIRQ(USART3_IRQn);
		}	
	}
}


void usart_1_tx(char  tx_data)
{
		while(!(USART1->SR & USART_SR_TC)){};
		USART1->DR = tx_data;
}

void usart_2_tx(char  tx_data)
{
		while(!(USART2->SR & USART_SR_TC)){};
		USART2->DR = tx_data;
}
void usart_3_tx(char  tx_data)
{
		while(!(USART3->SR & USART_SR_TC)){};
		USART3->DR = tx_data;
}

void serialprint_1(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_1_tx(data_string[i]);
			i++;
		}
}

void serialprintln_1(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_1_tx(data_string[i]);
			i++;
		}
		usart_1_tx(0x0A);
		usart_1_tx(0x0D);
}

void serialprint_2(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_2_tx(data_string[i]);
			i++;
		}
}

void serialprintln_2(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_2_tx(data_string[i]);
			i++;
		}
		usart_2_tx(0x0A);
		usart_2_tx(0x0D);
}


void serialprint_3(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_3_tx(data_string[i]);
			i++;
		}
}

void serialprintln_3(char * data_string)
{
	int i = 0;
		while(data_string[i] != '\0')
		{
			usart_3_tx(data_string[i]);
			i++;
		}
		usart_3_tx(0x0A);
		usart_3_tx(0x0D);
}

char usart_1_rx()
{
		while(!(USART1->SR & USART_SR_IDLE)){};
		return USART1->DR;
}
char usart_2_rx()
{
		while(!(USART2->SR & USART_SR_IDLE)){};
		return USART2->DR;
}
char usart_3_rx()
{
		while(!(USART3->SR & USART_SR_IDLE)){};
		return USART3->DR;
}