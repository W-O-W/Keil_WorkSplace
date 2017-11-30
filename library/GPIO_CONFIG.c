#include "GPIO.h"
GPIOSpeed_TypeDef GPIO_SPEED = GPIO_Speed_50MHz;
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitTypeDef GPIO_Configuration_OPP(uint16_t *GPIO,uint16_t len){
	uint16_t p = 0;
	int i;
	for(i = 0;i<len;i++){
		p |= *GPIO;
		GPIO+=sizeof(uint16_t);
	}
	/* 设置为推挽输出，最大翻转率为50MHz */
	GPIO_InitStructure.GPIO_Pin = p;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	return GPIO_InitStructure;
}


