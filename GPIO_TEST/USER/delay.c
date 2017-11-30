#include "stm32f10x.h"
#include "delay.h"

static u8 fac_us = 0;//us beichenshu
static u16 fac_ms = 0;//ms beichenshu

/* delay_init */
void delay_init(u8 SYSCLK){
	SysTick -> CTRL &= 0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us = SYSCLK>>3;
	fac_ms = (u16)fac_us*1000;
}

/* delay_us */
void delay_us(u32 nus){
	u32 temp;
	SysTick->LOAD = nus*fac_us;//load time
	SysTick->VAL = 0x00;//clean 
	SysTick->CTRL = 0x01;//start
	
	do{
		temp = SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL = 0x00;//close
	SysTick->VAL = 0x00;//clean
}

/* delay_ms */
void delay_ms(u16 nms){
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;//clean
	SysTick->CTRL = 0x01;//start
	
	do{
		temp = SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL = 0x00;//close
	SysTick->VAL = 0x00;//clean
		
}

