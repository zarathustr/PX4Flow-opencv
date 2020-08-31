#include "micros.h"
#include "misc.h"

double Micros(void)
{
  return(((double)(TIM5->CNT)) * (1e-6));
}


void delay_us(uint32_t nTime)
{	

  for (int i=0;i<100000;++i)
  {
  }
}

void delay_ms(int nms)
{	 
  osDelay(nms);

}   


void InitMicros(void){
        RCC->APB1ENR |= 0x0008;

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
        
	TIM5->CR1 = 0x0080; 
	TIM5->CR2 = 0x0000;
	TIM5->CNT = 0x0000;
	TIM5->ARR = 0xFFFFFFFF;
	TIM5->PSC = 200 - 1;	
	TIM5->EGR = 0x0001;
	TIM5->CR1 |= 0x0001;
        TIM5->DIER |= 1 << 0;
}
   
   

