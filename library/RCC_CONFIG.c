#include "RCC.h"
#include "stm32f10x_conf.h"
void RCC_Configuration_HSE(){
	ErrorStatus HSEStartUpStatus;//HSEStartUpStatus 定义枚举类型
	RCC_DeInit();//复位系统时钟
	RCC_HSEConfig(RCC_HSE_ON);//开启HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待HSE起振并稳定
	
	if(HSEStartUpStatus == SUCCESS){
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//选择HCLK(AHB)作为SYSCLK 1分频
		RCC_PCLK2Config(RCC_HCLK_Div1);//选择PCLK2时钟源为HCLK1分频
		RCC_PCLK1Config(RCC_HCLK_Div2);//选择PCLK1时钟源为HCLK(AHB)2分频
		FLASH_SetLatency(FLASH_Latency_2);//设置FLASH等待时间为2周期
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//使能Flash预取缓存
		
		/* 选择PLL时钟源为HSE1分频，倍频率为9,则PLL = 9×8=72MHz */
		RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);//enable PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL输出稳定
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//选择SYSCLK时钟源为PLL
		while(RCC_GetSYSCLKSource() != 0x08);//等待PLL成为SYSCLK时钟源
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开APB2总线上的GPIOA时钟
}
