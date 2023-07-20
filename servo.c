#include "stm32f10x.h"

int CCR1,CCR2;
float coeff=2000/180;


void init_servo(){
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;
	
	GPIOB->CRH|=(GPIO_CRH_CNF9_1|GPIO_CRH_MODE9);
	GPIOB->CRH&=~(GPIO_CRH_CNF9_0);
	
	//t43c3 
	
	
	GPIOB->CRH|=(GPIO_CRH_CNF8_1|GPIO_CRH_MODE8);
	GPIOB->CRH&=~(GPIO_CRH_CNF8_0);
	
	TIM4->PSC=72-1;
	TIM4->ARR=20000;
	
	TIM4->CCER|=TIM_CCER_CC4E;
	TIM4->CCMR2|=TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2;
	TIM4->CCMR2&=~(TIM_CCMR2_OC4M_0);
	
	TIM4->CCER|=TIM_CCER_CC3E;
	TIM4->CCMR2|=TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2;
	TIM4->CCMR2&=~(TIM_CCMR2_OC3M_0);
	
	TIM4->CR1|=TIM_CR1_CEN;
}

void set_dc1(int compare1){
	TIM4->CCR4=compare1;
}

void set_angle1(float angle1){
	CCR1=(coeff*angle1+500);
	set_dc1(CCR1);
}

void set_dc2(int compare2){
	TIM4->CCR3=compare2;
}

void set_angle2(float angle2){
	CCR2=(coeff*angle2+500);
	set_dc2(CCR2);
}