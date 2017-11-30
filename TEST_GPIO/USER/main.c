#include "stm32f10x.h"
#include "RCC.h"
#include "GPIO.h"

/** @addtogroup TEST_GPIO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define Delay(n) while((n--))
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(){
	vu32 n = 2000000;
	//uint16_t p[2] = {GPIO_Pin_13, GPIO_Pin_14}; 
	//uint16_t *__GPIO = &p[0];
	extern GPIO_InitTypeDef GPIO_InitStructure;
	RCC_Configuration_HSE();
	//GPIO_InitStructure = GPIO_Configuration_OPP(__GPIO,2);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	Delay(n);

	GPIO_ResetBits(GPIOA,GPIO_Pin_13);
	GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	while(1);
	
	
}
