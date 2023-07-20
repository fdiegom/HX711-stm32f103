#include "stm32f10x.h"
#include "HX711.h"
#include <stdbool.h>
#include "systick_timer.h"

	uint8_t PD_SCK;
	uint8_t DOUT;
	uint8_t GAIN;
	char Port;
	long Offset=0;
	float Scale=1;

//desactivar las interrupciones

uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;
		
    for(i = 0; i < 8; i++) {
				
				if(Port=='a'){
					GPIOA->ODR|=1<<clockPin;
					delayus(1);
					if(bitOrder == 0){
            value |= ((GPIOA->IDR & 1<<dataPin)>>dataPin) << i;
					}
					else {
            value |= ((GPIOA->IDR & 1<<dataPin)>>dataPin) << (7 - i);
					}
					GPIOA->ODR&=~(1<<clockPin);
					delayus(1);
				}
				
				if(Port=='b'){
					GPIOB->ODR|=1<<clockPin;
					delayus(1);
					if(bitOrder == 0){
            value |= ((GPIOB->IDR & 1<<dataPin)>>dataPin) << i;
					}
					else{
            value |= ((GPIOB->IDR & 1<<dataPin)>>dataPin) << (7 - i);
					}
					GPIOB->ODR&=~(1<<clockPin);
					delayus(1);
				}
		}
    return value;
		value=0;
}


void begin(uint8_t dout, uint8_t pd_sck, uint8_t gain,char p){
		
		systick_init();
		Port=p;
		PD_SCK=pd_sck;
		DOUT=dout;
		
		RCC->APB2ENR|=RCC_APB2ENR_IOPBEN|RCC_APB2ENR_IOPAEN;
		if(Port=='b'){
			if(PD_SCK<8){
				GPIOB->CRL=3<<PD_SCK*4;
			}
			else{
	  		GPIOB->CRH=3<<(PD_SCK-8)*4;
			}
			if(DOUT<8){
  			GPIOB->CRL|=8<<DOUT*4;
	  	}
			else{
				GPIOB->CRH|=8<<(DOUT-8)*4;
			}	
			GPIOB->ODR|=1<<DOUT;
			GPIOB->ODR&=~(1<<PD_SCK);
	 }
   else if(Port=='a'){
			if(PD_SCK<8){
				GPIOA->CRL=3<<PD_SCK*4;
  		}
			else{
  			GPIOA->CRH=3<<(PD_SCK-8)*4;
			}	
			if(dout<8){
				GPIOA->CRL|=8<<DOUT*4;
			}
			else{
				GPIOA->CRH|=8<<(DOUT-8)*4;
			}
				GPIOA->ODR|=1<<DOUT;
				GPIOA->ODR&=~(1<<PD_SCK);
	 }
	 set_gain(gain);
}



bool is_ready(){
	if(Port=='b'){
		if((GPIOB->IDR & 1<<DOUT)==0){
			return true;
		}
		else{
			return false;
		}
	}
	else if(Port=='a'){
			if((GPIOA->IDR & 1<<DOUT)==0){
			return true;
		}
		else{
			return false;
		}
	}
	return false;
}


void set_gain(uint8_t gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}

}

//corregir
long read(){

	wait_ready();
	unsigned long value=0;
	uint8_t data[3]={ 0 };
	uint8_t filler =0x00;
	
//	NVIC_DISABLE(USART1_IRQn);
//	NVIC_DISABLE(USART2_IRQn);
//	NVIC_DISABLE(USART3_IRQn);
//	NVIC_DISABLE(EXTI15_10_IRQn);
//	NVIC_DISABLE(EXTI9_5_IRQn);
//	NVIC_DISABLE(EXTI4_IRQn);
//	NVIC_DISABLE(EXTI3_IRQn);
//	NVIC_DISABLE(EXTI2_IRQn);
//	NVIC_DISABLE(EXTI1_IRQn);
	
	data[2] =shiftInSlow(DOUT,PD_SCK,1);
	data[1] =shiftInSlow(DOUT,PD_SCK,1);
	data[0] =shiftInSlow(DOUT,PD_SCK,1);
	
	int i;
	for(i=0;i<GAIN;i++){
		if(Port=='a'){
			GPIOA->ODR|=1<<PD_SCK;
			delayus(1);
			GPIOA->ODR&=~(1<<PD_SCK);
			delayus(1);
		}
		else if(Port=='b'){
			GPIOB->ODR|=1<<PD_SCK;
			delayus(1);
			GPIOB->ODR&=~(1<<PD_SCK);
			delayus(1);
		}
	}
	
//	NVIC_ENABLE(USART1_IRQn);
//	NVIC_ENABLE(USART2_IRQn);
//	NVIC_ENABLE(USART3_IRQn);
//	NVIC_ENABLE(EXTI15_10_IRQn);
//	NVIC_ENABLE(EXTI9_5_IRQn);
//	NVIC_ENABLE(EXTI4_IRQn);
//	NVIC_ENABLE(EXTI3_IRQn);
//	NVIC_ENABLE(EXTI2_IRQn);
//	NVIC_ENABLE(EXTI1_IRQn);
	
	if(data[2] & 0x80){
		filler=0xFF;
	}
	else {
		filler=0x00;
	}
	
	value=(filler<<24|
				 data[2]<<16|
				 data[1]<<8|
				 data[0]);	
return value;
}

void wait_ready(){
	while(!(is_ready())){}
}

bool wait_ready_retry(int retries , unsigned long delay_Ms ){
	int count = 0;
	while (count < retries) {
		if (is_ready()) {
			return true;
		}
		delayms(delay_Ms);
		count++;
	}
	return false;
}




long read_average(uint8_t times) {
	long sum = 0;
	uint8_t i;
	for ( i= 0; i < times; i++) {
		sum += read();
	}
	return sum / times;
}


double get_value(uint8_t times) {
	return read_average(times) - Offset;
}

float get_units(uint8_t times) {
	return get_value(times) / Scale;
}

void tare(uint8_t times) {
	double sum = read_average(times);
	set_offset(sum);
}

void set_scale(float scale) {
	Scale = scale;
}

float get_scale() {
	return Scale;
}

void set_offset(long offset) {
	Offset = offset;
}

long get_offset() {
	return Offset;
}

void power_down() {
	if(Port=='a'){
		GPIOA->ODR&=~(1<<PD_SCK);
		GPIOA->ODR|=(1<<PD_SCK);
	}
	else if(Port=='b'){
		GPIOB->ODR&=~(1<<PD_SCK);
		GPIOB->ODR|=(1<<PD_SCK);
	}
}

void power_up() {
	if(Port=='a'){
		GPIOA->ODR&=~(1<<PD_SCK);
	}
	else if(Port=='b'){
		GPIOB->ODR&=~(1<<PD_SCK);
	}
}