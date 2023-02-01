
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"

/*ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   8
#define IP_ADDR3   111
   
/*PORT*/  
#define SERVER_PORT	16969   
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   8
#define GW_ADDR3   1   
   
  
// МОЕ
#include <stdio.h>
#include <stdint.h>
   
#define LED_B_Pin GPIO_PIN_6
#define LED_B_GPIO_Port GPIOI
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOI
#define TVS_FLT3_Pin GPIO_PIN_5
#define TVS_FLT3_GPIO_Port GPIOK
#define FLT16_Pin GPIO_PIN_10
#define FLT16_GPIO_Port GPIOG
#define SHDN16_Pin GPIO_PIN_9
#define SHDN16_GPIO_Port GPIOG
#define FLUID2_Pin GPIO_PIN_1
#define FLUID2_GPIO_Port GPIOI
#define FLUID1_Pin GPIO_PIN_0
#define FLUID1_GPIO_Port GPIOI
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOI
#define LIM_L2_Pin GPIO_PIN_1
#define LIM_L2_GPIO_Port GPIOE
#define TVS_SHDN3_Pin GPIO_PIN_4
#define TVS_SHDN3_GPIO_Port GPIOK
#define FLUID3_Pin GPIO_PIN_2
#define FLUID3_GPIO_Port GPIOI
#define SHDN9_Pin GPIO_PIN_15
#define SHDN9_GPIO_Port GPIOH
#define FLT9_Pin GPIO_PIN_14
#define FLT9_GPIO_Port GPIOH
#define LIM_L3_Pin GPIO_PIN_2
#define LIM_L3_GPIO_Port GPIOE
#define LIM_L1_Pin GPIO_PIN_0
#define LIM_L1_GPIO_Port GPIOE
#define FLUID4_Pin GPIO_PIN_3
#define FLUID4_GPIO_Port GPIOI
#define LIM_R3_Pin GPIO_PIN_5
#define LIM_R3_GPIO_Port GPIOE
#define LIM_R2_Pin GPIO_PIN_4
#define LIM_R2_GPIO_Port GPIOE
#define LIM_R1_Pin GPIO_PIN_3
#define LIM_R1_GPIO_Port GPIOE
#define FAULT3_Pin GPIO_PIN_12
#define FAULT3_GPIO_Port GPIOJ
#define EN1_Pin GPIO_PIN_9
#define EN1_GPIO_Port GPIOI
#define STEP1_Pin GPIO_PIN_8
#define STEP1_GPIO_Port GPIOI
#define DIR1_Pin GPIO_PIN_11
#define DIR1_GPIO_Port GPIOI
#define FLT7_Pin GPIO_PIN_7
#define FLT7_GPIO_Port GPIOG
#define STEP2_Pin GPIO_PIN_1
#define STEP2_GPIO_Port GPIOF
#define FAULT2_Pin GPIO_PIN_0
#define FAULT2_GPIO_Port GPIOF
#define FAULT1_Pin GPIO_PIN_12
#define FAULT1_GPIO_Port GPIOI
#define M1_1_Pin GPIO_PIN_14
#define M1_1_GPIO_Port GPIOI
#define M0_2_Pin GPIO_PIN_3
#define M0_2_GPIO_Port GPIOF
#define SHDN7_Pin GPIO_PIN_4
#define SHDN7_GPIO_Port GPIOG
#define FLT8_Pin GPIO_PIN_3
#define FLT8_GPIO_Port GPIOG
#define SHDN8_Pin GPIO_PIN_2
#define SHDN8_GPIO_Port GPIOG
#define FLT5_Pin GPIO_PIN_2
#define FLT5_GPIO_Port GPIOK
#define M1_2_Pin GPIO_PIN_5
#define M1_2_GPIO_Port GPIOF
#define DIR2_Pin GPIO_PIN_4
#define DIR2_GPIO_Port GPIOF
#define SHDN5_Pin GPIO_PIN_1
#define SHDN5_GPIO_Port GPIOK
#define FLT6_Pin GPIO_PIN_11
#define FLT6_GPIO_Port GPIOJ
#define SHDN6_Pin GPIO_PIN_10
#define SHDN6_GPIO_Port GPIOJ
#define M1_3_Pin GPIO_PIN_0
#define M1_3_GPIO_Port GPIOJ
#define FLT3_Pin GPIO_PIN_7
#define FLT3_GPIO_Port GPIOJ
#define SHDN3_Pin GPIO_PIN_6
#define SHDN3_GPIO_Port GPIOJ
#define M0_1_Pin GPIO_PIN_15
#define M0_1_GPIO_Port GPIOI
#define M0_3_Pin GPIO_PIN_1
#define M0_3_GPIO_Port GPIOJ
#define TVS_SHDN4_Pin GPIO_PIN_10
#define TVS_SHDN4_GPIO_Port GPIOH
#define TVS_FLT2_Pin GPIO_PIN_11
#define TVS_FLT2_GPIO_Port GPIOH
#define FLT4_Pin GPIO_PIN_14
#define FLT4_GPIO_Port GPIOD
#define PGOOD12_Pin GPIO_PIN_6
#define PGOOD12_GPIO_Port GPIOA
#define TVS_FLT4_Pin GPIO_PIN_9
#define TVS_FLT4_GPIO_Port GPIOH
#define TVS_SHDN2_Pin GPIO_PIN_12
#define TVS_SHDN2_GPIO_Port GPIOH
#define SHDN4_Pin GPIO_PIN_12
#define SHDN4_GPIO_Port GPIOD
#define FLT1_Pin GPIO_PIN_13
#define FLT1_GPIO_Port GPIOD
#define PGOOD5_Pin GPIO_PIN_5
#define PGOOD5_GPIO_Port GPIOA
#define DIR3_Pin GPIO_PIN_2
#define DIR3_GPIO_Port GPIOJ
#define EN2_Pin GPIO_PIN_6
#define EN2_GPIO_Port GPIOH
#define TVS_SHDN1_Pin GPIO_PIN_8
#define TVS_SHDN1_GPIO_Port GPIOH
#define SHDN2_Pin GPIO_PIN_12
#define SHDN2_GPIO_Port GPIOB
#define SHDN1_Pin GPIO_PIN_10
#define SHDN1_GPIO_Port GPIOD
#define PON5_Pin GPIO_PIN_3
#define PON5_GPIO_Port GPIOA
#define PON12_Pin GPIO_PIN_4
#define PON12_GPIO_Port GPIOA
#define STEP3_Pin GPIO_PIN_3
#define STEP3_GPIO_Port GPIOJ
#define EN3_Pin GPIO_PIN_4
#define EN3_GPIO_Port GPIOJ
#define TVS_FLT1_Pin GPIO_PIN_7
#define TVS_FLT1_GPIO_Port GPIOH
#define FLT2_Pin GPIO_PIN_13
#define FLT2_GPIO_Port GPIOB


   
   
 
static void hardware_init(void);

static void startThreads(void);
static void WebThread(void const * argument);
static void MotorsThread(void const * argument);
static void SysInfoThread(void const * argument);

static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Netif_Config(void);
void Error_Handler(void);

static void MX_GPIO_Init(void);

static void MX_TIM8_Init(void);
static void MX_TIM7_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

static void MX_UART8_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_UART7_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);

//ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ
static void stepSetM01(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
