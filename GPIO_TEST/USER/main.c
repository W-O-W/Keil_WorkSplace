/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
#include "sys.h"
#include "math.h"
#include "MPU6050.h"


/** @addtogroup MPU6050
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/*
#ifdef USE_STM32100B_EVAL
  #define MESSAGE1   "STM32 MD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100B-EVAL    " 
#elif defined (USE_STM3210B_EVAL)
  #define MESSAGE1   "STM32 Medium Density" 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210B-EVAL    " 
#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
  #define MESSAGE1   "  STM32 XL Density  " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210E_EVAL)
  #define MESSAGE1   " STM32 High Density " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM3210E-EVAL    " 
#elif defined (USE_STM3210C_EVAL)
  #define MESSAGE1   " STM32 Connectivity " 
  #define MESSAGE2   " Line Device running" 
  #define MESSAGE3   " on STM3210C-EVAL   "
#elif defined (USE_STM32100E_EVAL)
  #define MESSAGE1   "STM32 HD Value Line " 
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM32100E-EVAL    "   
#endif
*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
SPI_InitTypeDef SPI_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
ErrorStatus HSEStartUpStatus;


/* Private function prototypes -----------------------------------------------*/
void USART_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void EXTI_Configuration(void);
void I2C_Configuration(void);


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	u8 i = 0;
	s16 accgyo[7] = {0};
	//u8 u[2] = {0};
	delay_init(72);
	NVIC_Configuration();
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	I2C_Configuration();

	MPU6050_Initialize();//init mpu6050

	while(1){
		MPU6050_GetRawAccelGyro(accgyo);
		printf("%10d%10d%10d%10d%10d%10d\r\n",accgyo[0],accgyo[1],accgyo[2],
			accgyo[3],accgyo[4],accgyo[5]);
		delay_ms(50);
	}
}
/**USART**/
void USART_Configuration(void){
	USART_InitStructure.USART_BaudRate = 38400;//set baudrate
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit tran
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//one stop label
	USART_InitStructure.USART_Parity = USART_Parity_No;//un check
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	//USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	//USART_ClockInitStructure.USART_CPOL = USART_CPOL_LOW;
	//USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	//USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	//USART_CLockInit(USART1,&USART_ClockInitStructure);
	
	/* Enable USART1 */
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

/** RCC **/
void RCC_Configuration(void){
	/* RCC System Reset(for debug purpose)*/
	RCC_DeInit();
	
	/*Enable HSE*/
	RCC_HSEConfig(RCC_HSE_ON);
	
	/*wait till HSE is ready*/
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS){
		
		/* Enable Prefetch Buffer*/
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* Flash 2 wait State */
		FLASH_SetLatency(FLASH_Latency_2);
		
		/*HCLK = SYSCLK*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		/*PCLK2 = HCLK*/
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/*PLLCLK = 8MHZ*9 = 72MHZ*/
		RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
		
		/*Enable PLL*/
		RCC_PLLCmd(ENABLE);
		
		/*wait till PLL is ready*/
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		/* Select PLL as System clock source*/
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* wait till PLL is used as System Clock Source*/
		while(RCC_GetSYSCLKSource() != 0x08);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
}

/* GPIO */
void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/* Configure USART1 Rx (PA.10) as Input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*GPIOB Configuration: Pin10 in Output LED1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*SPI*/
void SPI1_Init(void){
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// when no data
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//can't too fast or read wrong
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStructure);
	//enable SPI1
	SPI_Cmd(SPI1,ENABLE);
}

/* EXTI */
void EXTI_Configuration(void){
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
	/* Configuration EXTI Line11 to generate an interrupt on falling edge */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/* NVIC */
void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	#ifdef VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	#else /* VECT_TAB_FLASH */
	/* Set the Vector Table base location at 0x0800000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the EXTI15_10 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/* I2C */
void I2C_Configuration(void){
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0xc0;//MPU6050 7-bit address = 0x68,8-bit address = 0xD0
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = MPU6050_I2C_Speed;
	
	/* Apply I2C configuration after enabling it */
	I2C_Init(MPU6050_I2C,&I2C_InitStructure);
	/* I2C Peripheral Enable */
	I2C_Cmd(MPU6050_I2C,ENABLE);
}

/* define fputc ,printf to USART */

int fputc(int ch,FILE *f){
	/* Place your implementation of fputc here,
	e.g write a character to the usart */
	USART_SendData(USART1,(u8) ch);
	/* Loopp until the end of tranmission */
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	return ch;
}

#ifdef DEBUG
/*
assert_failed--Report the name of the source file and the source line number 
	where the assert_param error has occurred.
*/

void assert_failed(u8* file,u32 line){
	/*User can add his own implementation to report the file name and
	line number,ex:printf("Wrong parameters values:file %s on line %d\r\n",file,line) */
	/*Infinite loop*/
	while(1);
}
#endif
/**

  * @}

  */