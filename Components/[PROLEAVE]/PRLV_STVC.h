#ifndef _STVC_H
#define STVC_H
#include "main.h"

//Пользовательские настройки
#define STVC_DEBUG				// Включить режим отладки



// Команды для управление лампами по RS-485
#define	LED_TURNOFF	0
#define LED_TURNON	1
#define	LED_SETLVL	2
#define LED_GETINFO	3

//Направление поворотов
#define STEP_DIR_STANDART	1					
#define STEP_DIR_REVERSE	!STEP_DIR_STANDART

// Максимальный поворот шагового двигателя, после достижения которого калибровка останавливается с ошибкой
#define MAX_STEPS_PER_CALIBRATION 10000		

//Настройки делителей шагов для двигателей
#define M0_1_STATE GPIO_PIN_SET
#define 	M1_1_STATE GPIO_PIN_RESET
#define M0_2_STATE GPIO_PIN_RESET
#define 	M1_2_STATE GPIO_PIN_RESET
#define M0_3_STATE GPIO_PIN_RESET
#define 	M1_3_STATE GPIO_PIN_RESET



int32_t systemChecking();

void tim7_delay(uint32_t time);
//RGB-индикация
void RGBsetColor(uint32_t r,uint32_t g,uint32_t b);
void RGBsignal_OK(uint32_t state);
 
//	Функции, включающие питание с проверкой системы	
uint32_t PON5_set(uint32_t state);
uint32_t PON12_set(uint32_t state);
uint32_t ETH_PWR_set(uint32_t state);
uint32_t tvsEn(uint32_t camera_num,uint32_t state);
uint32_t tvs_check(uint32_t camera_num);
   


int32_t angle2step(uint32_t motor_num,float angle);


//	Функция принимает на вход номер концевика(1, 2 или 3);
//		на выходе дает:
//		0 - концевики не нажаты;
//		1 - нажат концевик L;
//		-1 - нажат концевик R.
int32_t limiter_check(uint32_t num);


void stepEn(uint32_t stepper_num,uint32_t state);
uint32_t step_check(uint32_t stepper_num);
void stepDir(uint32_t stepper_num,uint32_t state);

uint32_t getCurStep(uint32_t stepper_num);
int32_t goStep(uint32_t stepper_num,int32_t steps); 
void goToStep(uint32_t stepper_num,uint32_t new_step);
uint32_t stepCalibrate(uint32_t stepper_num);














extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

#define ledOn(ledNum)				ledTxData(ledNum,LED_TURNON,0)
#define ledOff(ledNum)				ledTxData(ledNum,LED_TURNOFF,0)
#define ledSetLvl(ledNum,level)		ledTxData(ledNum,LED_SETLVL,level)

uint32_t ledEn(uint32_t led_num,uint32_t state);
uint32_t led_check(uint32_t led_num);


int32_t ledTxData(uint32_t led_num,uint32_t command,uint32_t data);
uint16_t ledGetInfo(uint32_t led_num);

void changeUART7(uint32_t led_num);
UART_HandleTypeDef getUartFromNum(uint32_t led_num);
void setUartDE(uint32_t led_num,uint32_t state);






uint32_t fluid_check(uint32_t fluid_num);

uint32_t laserSet(uint32_t state);
  
#endif //___________________________________________________________________________________________________________________________________________________________________________