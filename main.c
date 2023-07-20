#include "stm32f10x.h"
#include "systick_timer.h"
#include "HX711.h"
#include "usart_setup.h"
#include <stdio.h>
#include "servo.h"

int i;

int main(){
	
	systick_init();
	usart_init(1,115200,0);
	begin(12,13,128,'b');
	GPIOA->CRL|=GPIO_CRL_MODE5;
	GPIOA->CRL&=~(GPIO_CRL_CNF5);
	GPIOA->ODR&=~(GPIO_ODR_ODR5);
	init_servo();
	set_scale(1);
	double peso;
	long v;
	set_scale(226.173);
	tare(20);
	
	while(1){
	
		peso=get_units(10);
		v=read();
		
//		set_angle1(60);
//		set_angle2(0);
//		delayms(100);
//		while(peso<0.02){
//			GPIOA->ODR|=(GPIO_ODR_ODR5);
//			peso=get_units(10);
//		}
//		
//		set_angle1(20);
//		while(peso<0.04){
//			GPIOA->ODR|=(GPIO_ODR_ODR5);
//			peso=get_units(10);
//		  
//		}
//		GPIOA->ODR&=~(GPIO_ODR_ODR5);
//		set_angle1(0);
		
		delayms(100);
	}
}
