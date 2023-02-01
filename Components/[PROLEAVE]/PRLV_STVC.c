#include "PRLV_STVC.h"

int32_t ledBright[9];
int32_t ledSR[9];
int32_t ledTemp[9];
	   
uint32_t step_max[2];
uint32_t inCalibration=0;


uint32_t cur_uart7;

// *********RGB-светодиод*********
void tim7_delay(uint32_t time){
  TIM7->CNT =0;
  while(TIM7->CNT<time); 
}

void RGBsetColor(uint32_t r,uint32_t g,uint32_t b){//      Изменение цвета RBG светодиода
  TIM8->CCR1 = r;
  TIM8->CCR2 = b; 
  TIM8->CCR3 = g; 
}

void RGBsignal_OK(uint32_t state){//Мигание зеленым(state=1), красным(state=0) или желтым(state=2) 5 раз, сигнализирующее как успешно прошел предыдущий тест    
    uint32_t r,g,b;
    if(state==1){r=0;g=255;b=0;}          //Зеленый
    else if(state==0){r=255;g=0;b=0;}//Красный
    else if(state==2){r=0,g=255;b=255;}   //Желтый   
    for(int i=0;i<5;i++){
    RGBsetColor(r,g,b);
    HAL_Delay(20);  
    RGBsetColor(0,0,0);
    HAL_Delay(80);
    }
}

// *********Питание 5в и 12в*********

uint32_t PON5_set(uint32_t state){	
  	GPIO_PinState pinState;
  	if(state)pinState = GPIO_PIN_RESET;	//Инверсия! при SET выключаем напр
	else pinState = GPIO_PIN_SET;
  
	HAL_GPIO_WritePin(PON5_GPIO_Port,PON5_Pin,pinState); 
  	HAL_Delay(3);
	
	printf("	@PWR 5v is ");
	if(state==1)printf("ON\n");else printf("OFF\n");
	
	if((HAL_GPIO_ReadPin(PGOOD5_GPIO_Port,PGOOD5_Pin)==state)){
	  return 1;
	  
	}else{
//	  printf("---PG5 ERROR---\n");	 
	  return 0;
	}
	
}

uint32_t PON12_set(uint32_t state){	
    GPIO_PinState pinState;
  	if(state)pinState = GPIO_PIN_RESET;	//Инверсия! при SET выключаем напр
	else pinState = GPIO_PIN_SET;
  
	HAL_GPIO_WritePin(PON12_GPIO_Port,PON12_Pin,pinState);
  	HAL_Delay(3);
	printf("	@PWR 12v is ");
	if(state==1)printf("ON\n");else printf("OFF\n");
	
	if((HAL_GPIO_ReadPin(PGOOD12_GPIO_Port,PGOOD12_Pin)==state)){
	  return 1;
	}else{
	  printf("---PG12 ERROR---\n");	 
	  return 0;
	}
}

uint32_t ETH_PWR_set(uint32_t state){	
    GPIO_PinState pinState;
  	if(state)pinState = GPIO_PIN_SET;	
	else pinState = GPIO_PIN_RESET;
  
	HAL_GPIO_WritePin(SHDN16_GPIO_Port,SHDN16_Pin,pinState);
  	HAL_Delay(3);
	printf("	@ETH PWR is ");
	if(state==1)printf("ON\n");else printf("OFF\n");
	
	if((HAL_GPIO_ReadPin(FLT16_GPIO_Port,FLT16_Pin)==pinState)){
	  
	  return 1;
	}else{
//	  printf("---ETH PWR ERROR---\n");	 
	  return 0;
	}
}






// *********IP-камеры*********

uint32_t tvsEn(uint32_t camera_num,uint32_t state){
	GPIO_PinState pinState;
  	if(state)pinState = GPIO_PIN_SET;	
  	else pinState = GPIO_PIN_RESET;
	GPIO_TypeDef* SHDNport;
  	uint32_t SHDNpin; 	
	
	switch(camera_num){
		case 1:
	  		SHDNpin = TVS_SHDN1_Pin;
			SHDNport = TVS_SHDN1_GPIO_Port;
	  		break;
		case 2:
  			SHDNpin = TVS_SHDN2_Pin;
			SHDNport = TVS_SHDN2_GPIO_Port;
	  		break;
		case 3:
  			SHDNpin = TVS_SHDN3_Pin;
			SHDNport = TVS_SHDN3_GPIO_Port;
	  		break;
		case 4:
  			SHDNpin = TVS_SHDN4_Pin;
			SHDNport = TVS_SHDN4_GPIO_Port;
	  		break;
	}
	
	HAL_GPIO_WritePin(SHDNport,SHDNpin,pinState);
//	HAL_Delay(5);
	printf("	@TVS%d PWR is ",camera_num);
	if(state==1)printf("ON\n");else printf("OFF\n");
	
//	uint32_t test_tvs  = tvs_check(camera_num);
//	printf("		test_tvs = %d\n",test_tvs);
	
	return tvs_check(camera_num);
}

uint32_t tvs_check(uint32_t camera_num){
  GPIO_TypeDef* FLTport;
  uint32_t FLTpin; 	 
  	switch(camera_num){
		case 1:
		  	FLTport = TVS_FLT1_GPIO_Port;
			FLTpin = TVS_FLT1_Pin;
	  		break;
		case 2:
			FLTport = TVS_FLT2_GPIO_Port;
			FLTpin = TVS_FLT2_Pin;
	  		break;
		case 3:
			FLTport = TVS_FLT3_GPIO_Port;
			FLTpin = TVS_FLT3_Pin;
	  		break;
		case 4:
			FLTport = TVS_FLT4_GPIO_Port;
			FLTpin = TVS_FLT4_Pin;
	  		break;
	}
//	printf("tvs_check%d = %d\n",camera_num,HAL_GPIO_ReadPin(FLTport,FLTpin));
  return HAL_GPIO_ReadPin(FLTport,FLTpin);  
}

// *********Шаговые двигатели*********

int32_t angle2step(uint32_t motor_num,float angle){
  
  motor_num=motor_num-1; // у нас все от 1 идет
  
  angle = angle/180;
  angle = angle * step_max[motor_num];
  
  //printf("		angle2step=%d\n",(int32_t)angle);
  return (int32_t)angle;
}



int32_t limiter_check(uint32_t num){	
  int32_t result=0;
  uint32_t pin_l,pin_r;
  switch (num){
	case 1:
	  	pin_l = LIM_L1_Pin;
		pin_r = LIM_R1_Pin;
		break;
	case 2:
	  	pin_l = LIM_L2_Pin;
		pin_r = LIM_R2_Pin;
	  	break;
  	case 3:
	  	pin_l = LIM_L3_Pin;
		pin_r = LIM_R3_Pin;
	  	break;	  
  }
  if(!(LIM_L1_GPIO_Port->IDR & pin_l))result = 1;		
  else if(!(GPIOE->IDR & pin_r))result = -1;
  else result = 0;
  return result;	
}




void stepEn(uint32_t stepper_num,uint32_t state){
  GPIO_PinState pinState;
  if(state)pinState = GPIO_PIN_RESET;	//Инверсия! при SET выключаем напр
  else pinState = GPIO_PIN_SET;
  
  switch(stepper_num){
  	case 1:
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,pinState);
		break;
  	case 2:
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,pinState);
		break;
  	case 3:
		HAL_GPIO_WritePin(EN3_GPIO_Port,EN3_Pin,pinState);
		break;
  }
}

uint32_t step_check(uint32_t stepper_num){
  uint32_t result;
  switch(stepper_num){
  	case 1:
	  	if(GPIOI->IDR & GPIO_IDR_ID12)result=1;//pi12
		break;
  	case 2:
		if(GPIOF->IDR & GPIO_IDR_ID0)result=1;//pf0
		break;
  	case 3:
		if(GPIOJ->IDR & GPIO_IDR_ID12)result=1;//pj12
		break;
  	default:
		result =0;
		break;
  }	
		
//	printf("step_check%d = %d\n",stepper_num,result);
	return result;
		
}

void stepDir(uint32_t stepper_num,uint32_t state){
  GPIO_PinState pinState;
  if(state)pinState = GPIO_PIN_SET;	
  else pinState = GPIO_PIN_RESET;
  switch(stepper_num){
  	case 1:
		HAL_GPIO_WritePin(DIR1_GPIO_Port,DIR1_Pin,pinState);
		break;
  	case 2:
		HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,pinState);
		break;
  	case 3:
		HAL_GPIO_WritePin(DIR3_GPIO_Port,DIR3_Pin,pinState);
		break;
  }
}



//uint32_t getCurStep(uint32_t stepper_num){  
//   return cur_pos[stepper_num];
//}

int32_t goStep(uint32_t stepper_num,int32_t steps){
  
  GPIO_TypeDef* step_port;
  uint16_t step_pin=0;
  uint32_t polarity=0;
  
  switch(stepper_num){
  case 1:
	step_pin = STEP1_Pin;
	step_port = GPIOI;
	break;
  case 2:
	step_pin = STEP2_Pin;
	step_port = GPIOF;
	break;
  case 3:
	step_pin = STEP3_Pin;
	step_port = GPIOJ;
	break;
  }
  
  if(steps>=0){
	stepDir(stepper_num,STEP_DIR_STANDART);
	polarity=0;
  }
  else {

	stepDir(stepper_num,STEP_DIR_REVERSE);
	steps=-steps;
	polarity=1;
  }
 
  stepEn(stepper_num,1);
  //printf("steps = ");
  while(steps!=0){
	//printf("%d\n",steps);
	
	HAL_GPIO_TogglePin(step_port,STEP1_Pin);
	tim7_delay(600);
	HAL_GPIO_TogglePin(step_port,step_pin);
	tim7_delay(600);
	steps--;
	
	
  }
  stepEn(stepper_num,0);
  if(polarity==0)return steps;
  else return -steps;
}

//void goToStep(uint32_t stepper_num,uint32_t new_step){
//  int32_t dStep = new_step-cur_pos[stepper_num]; // Мб наоборот
//  
//  goStep(stepper_num,dStep);
//  cur_pos[stepper_num] = new_step;
//}







uint32_t stepCalibrate(uint32_t stepper_num){
  inCalibration=1;
  printf("		____________________\n");
  printf("		Stepper%d calibrating\n",stepper_num);
  uint32_t _curStep=0;
  
  // Находим ноль
  printf("		Go to Zero\n");
  while(limiter_check(stepper_num)!=-1){		
	goStep(stepper_num,-1);
	_curStep++;
	if(_curStep>MAX_STEPS_PER_CALIBRATION){
	  printf("#B#	STEP%d ERROR : STEPS ENUMERATION OF MIN VALUE",stepper_num);
	  printf("STEPPER%d IS NOT CALIBRATED",stepper_num);
	  inCalibration=0;
	  return 0;
	}	
  }
  _curStep=0;
  
  // Находим максимум
  printf("		Go to Max\n");
  while(limiter_check(stepper_num)!=1){
	goStep(stepper_num,1);
	_curStep++;
	if(_curStep>MAX_STEPS_PER_CALIBRATION){
	  printf("#B#	STEP%d ERROR : STEPS ENUMERATION OF MIN VALUE",stepper_num);
	  printf("STEPPER%d IS NOT CALIBRATED",stepper_num);
	  inCalibration=0;
	  return 0;
	}	
  }
  // Даем новые значения мин-макс и выводим их
  step_max[stepper_num-1]=_curStep;
  //cur_pos[stepper_num] =_curStep;
  
  printf("		Stepper %d is Calibrated\n",stepper_num);
  printf("		Min: %d	Max: %d\n",0,step_max[stepper_num-1]);
  
  printf("		Go to center: %d\n", angle2step(stepper_num,90));
  printf("		____________________\n");
  
  // Переход в центр
  goStep(stepper_num,-angle2step(stepper_num,90));
  inCalibration=0;
  return 1;
}




// *********Светильники*********

uint32_t ledEn(uint32_t led_num,uint32_t state){
  	GPIO_PinState pinState;
  	if(state)pinState = GPIO_PIN_SET;	
  	else pinState = GPIO_PIN_RESET;
	
	GPIO_TypeDef* SHDNport;
  	uint32_t SHDNpin; 
	switch(led_num){
		case 1:
	  		SHDNpin = SHDN1_Pin;
			SHDNport = SHDN1_GPIO_Port;
	  		break;
		case 2:
  			SHDNpin = SHDN2_Pin;
			SHDNport = SHDN2_GPIO_Port;
	  		break;
		case 3:
  			SHDNpin = SHDN3_Pin;
			SHDNport = SHDN3_GPIO_Port;
	  		break;
		case 4:
  			SHDNpin = SHDN4_Pin;
			SHDNport = SHDN4_GPIO_Port;
	  		break;
		case 5:
  			SHDNpin = SHDN5_Pin;
			SHDNport = SHDN5_GPIO_Port;
	  		break;
		case 6:
  			SHDNpin = SHDN6_Pin;
			SHDNport = SHDN6_GPIO_Port;
	  		break;
		case 7:
  			SHDNpin = SHDN7_Pin;
			SHDNport = SHDN7_GPIO_Port;
	  		break;
		case 8:
  			SHDNpin = SHDN8_Pin;
			SHDNport = SHDN8_GPIO_Port;
	  		break;
		case 9:
  			SHDNpin = SHDN9_Pin;
			SHDNport = SHDN9_GPIO_Port;
	  		break;
	}
	
  	HAL_GPIO_WritePin(SHDNport,SHDNpin,pinState);
	HAL_Delay(3);

	printf("	@LED%d PWR is ",led_num);
	if(state==1)printf("ON\n");else printf("OFF\n");
	
	
	// Необходимо для очистки буффера
	UART_HandleTypeDef led_huart = getUartFromNum(led_num);
	uint8_t rxData=0;
	HAL_StatusTypeDef statusRXprev = HAL_UART_Receive(&led_huart,&rxData,5,100);	// Принять 4 байта
	//if(statusRXprev==3){printf("AAAAAAAAAAAAAAAA\n");}
	
	return led_check(led_num);
}

uint32_t led_check(uint32_t led_num){
  	GPIO_TypeDef* FLTport;
  	uint32_t FLTpin; 	
	
	switch(led_num){
		case 1:
	  		FLTpin = FLT1_Pin;
			FLTport = FLT1_GPIO_Port;
	  		break;
		case 2:
  			FLTpin = FLT2_Pin;
			FLTport = FLT2_GPIO_Port;
	  		break;
		case 3:
  			FLTpin = FLT3_Pin;
			FLTport = FLT3_GPIO_Port;
	  		break;
		case 4:
  			FLTpin = FLT4_Pin;
			FLTport = FLT4_GPIO_Port;
	  		break;
		case 5:
  			FLTpin = FLT5_Pin;
			FLTport = FLT5_GPIO_Port;
	  		break;
		case 6:
  			FLTpin = FLT6_Pin;
			FLTport = FLT6_GPIO_Port;
	  		break;
		case 7:
  			FLTpin = FLT7_Pin;
			FLTport = FLT7_GPIO_Port;
	  		break;
		case 8:
  			FLTpin = FLT8_Pin;
			FLTport = FLT8_GPIO_Port;
	  		break;
		case 9:
  			FLTpin = FLT9_Pin;
			FLTport = FLT9_GPIO_Port;
	  		break;
	}
	uint32_t state = HAL_GPIO_ReadPin(FLTport,FLTpin);  
//	printf("led_check%d = %d\n",led_num,state);
  	return state;
}

int32_t ledTxData(uint32_t led_num,uint32_t command,uint32_t data){
  uint8_t rxData[5];
  uint8_t checkSum = command + data; 
  
  
  // Отправка
  UART_HandleTypeDef led_huart = getUartFromNum(led_num);
  uint8_t dataToSend[4] = {0xAA,command,data,checkSum};
  
  
  //if(statusRXprev==3){printf("- prev err-\n",led_num);return -1;}
  
  HAL_StatusTypeDef status = HAL_UART_Transmit(&led_huart,dataToSend,4,1000); // отправить 4 байта
 
  // Прием  
  HAL_StatusTypeDef statusRX = HAL_UART_Receive(&led_huart,rxData,5,1000);	// Принять 4 байта
  
  
  if(statusRX==3){printf("- LAMP_RX%d TIMEOUT!!! -\n",led_num);return -1;}
  //printf("LED%d sr:%d",led_num,statusRX);printf("	data: %d %d %d %d %d\n",rxData[0],rxData[1],rxData[2],rxData[3],rxData[4]);
  
  	if(rxData[0] == 85){
	
	checkSum = rxData[1] + rxData[2] + rxData[3]; //CheckSum
	if(checkSum ==	rxData[4]){			
	  
		ledSR[led_num-1] = rxData[1];
		ledBright[led_num-1] = rxData[2];
		ledTemp[led_num-1]   = rxData[3];
  	
		
//		printf("_____DATA TX LAMP%d_____\n",led_num);
//		printf("SR: %d\n",ledSR[led_num-1]);
//		printf("Bright: %d\n",ledBright[led_num-1]);
//		printf("Temp: %d\n",ledTemp[led_num-1]);
		
		
		}else{printf("- LAMP%d rxData bad checkSum!!! -\n",led_num);	return -2;}
	
  
	}else{printf("- LAMP%d rxData is shifted!!! -\n",led_num);	return -3;}
  
	return ledSR[led_num-1]; // В конце концов происходит вывод SR
}

uint16_t ledGetInfo(uint32_t led_num){
  
	HAL_StatusTypeDef status;
	uint8_t rxData[5];
  	uint8_t dataToSend[4] = {0xAA,LED_GETINFO,0,LED_GETINFO};
	
	UART_HandleTypeDef led_huart = getUartFromNum(led_num);
  
  	status = HAL_UART_Transmit(&led_huart,dataToSend,4,1000); // отправить 4 байта
	
	// Прием  
  	status = HAL_UART_Receive(&led_huart,rxData,5,1000);
	if(status==3){printf("- LAMP_RX%d TIMEOUT!!! -\n",led_num);return 0;}
	
	if(rxData[0] == 85){
	
		uint8_t checkSum = rxData[1] + rxData[2] + rxData[3]; //CheckSum
		if(checkSum ==	rxData[4]){			
	  
			  
		uint8_t statusReg = rxData[1];
		uint8_t statusBright = rxData[2];
		uint16_t ledInfo = 0;
		
		
		// Получается 2 байта. 		1й байт - СОСТОЯНИЕ ОП;		 2й-байт ЯРКОСТЬ ОП
		ledInfo = statusReg + (statusBright<<8);
		
		
		//printf("Bright = %d\n",statusBright);
		//printf("ledInfo = %d\n",ledInfo);
		
		
		return ledInfo;
		

		
		}else{printf("- LAMP%d rxData bad checkSum!!! -\n",led_num);	return 0;}
	
  
	}else{printf("- LAMP%d rxData is shifted!!! -\n",led_num);	return 0;}
	
	
	
	
	
	
	//if(ledSR == -1 || ledSR == -2 || ledSR == -3)return ledSR;		
	//broken_flag = ledSR & (1<<1);
	//hot65_flag = ledSR & (1<<2);
	//hot85_flag = ledSR & (1<<3);
	  
	//if(broken_flag)printf("- LAMP%d IS BROKEN -\n",led_num);
	//if(hot65_flag)printf("- LAMP%d HOT65 -\n",led_num);
	//if(hot85_flag)printf("- LAMP%d HOT85 -\n",led_num);
	
	//if(ledSR==1 || ledSR==0)printf("- LAMP%d is OK -\n\n",led_num);
	
}

void changeUART7(uint32_t led_num){
  if(cur_uart7== led_num)return;
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  if(led_num==3){
  cur_uart7 = led_num;
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
  	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;					// Вырубаем в input
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
  	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  
  }else if(led_num==4){
  cur_uart7 = led_num;
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;					// Вырубаем в input
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
  	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
  	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  }else{
	printf("changingUART7. WTF??? \n");
  }
  
  

}
    
UART_HandleTypeDef getUartFromNum(uint32_t led_num){
	UART_HandleTypeDef led_huart; 
	 // Это раскомментить когда будут синициализированные все uart в cubemx
  	switch(led_num){
		case 1:
	  		led_huart = huart3;
	  		break;
		case 2:
			led_huart = huart8;
	  		break;
		case 3:
			led_huart = huart7;
			changeUART7(led_num);
	  		break;
		case 4:
  			led_huart = huart7;
			changeUART7(led_num);
	  		break;
		case 5:
  			led_huart = huart5;
	  		break;
		case 6:
  			led_huart = huart2;
	  		break;
		case 7:
  			led_huart = huart4;
	  		break;
		case 8:
  			led_huart = huart1;
	  		break;
		case 9:
  			led_huart = huart6;
	  		break;		
	}
	
	return led_huart;	
}





// *********Датчики протечки*********

uint32_t fluid_check(uint32_t fluid_num){
  uint32_t FLUIDpin; 
  switch(fluid_num){
  	case 1:
	  	FLUIDpin = FLUID4_Pin;	
	  	break;
  	case 2:
	  	FLUIDpin = FLUID2_Pin;
		break;
	// Тут может быть что то другое. поставил пока 4 и 2,
	// тк это соответствует тестовым кнопкам
			
  	default:
		printf("Only f3 and f1!!!\n");
		break;
//  	case 3:
//	  	FLUIDpin = FLUID3_Pin; // Запас
//	  	break;
//	case 4:
//	  	FLUIDpin = FLUID4_Pin; // Запас
//		break;
  }
//  printf("fluid_check%d = %d \n",fluid_num,HAL_GPIO_ReadPin(FLUID1_GPIO_Port,FLUIDpin));
  return HAL_GPIO_ReadPin(FLUID1_GPIO_Port,FLUIDpin);
}

uint32_t laserSet(uint32_t state){
  
  printf(" $ LASER IS ");
  if(state)printf("ON $\n");
  else printf("OFF $\n");
  
  
  
  return 1;
}




// *********Проверка системы*********

//int32_t systemChecking(){
//uint32_t errors=0;
//	printf("System checking...\n");
//
//	for(int i=1;i<3;i++){						// Проверка датчиков жидкости
//	  if(fluid_check(i)==1){
//		printf("#FATAL#		SYSTEM ERROR: FLUID%d\n",i);
//		
//		PON5_set(0);
//		PON12_set(0);	// Выключаем линии 5вольт и 12вольт
//		for(int i=1;i<10;i++)ledEn(i,0);//Выключаем подачу питания на светильники
//		for(int i=1;i<4;i++)stepEn(i,0);// Выключаем подачу питания на шаговики
//		
//  		return -1;	// 						-1 значит пизда
//	  }
//	}
//	
//	// ЛИНИЯ 5 ВОЛЬТ_______________________________________________________________________@
//
//	for(int i=1;i<10;i++){						// Проверка модулей светодиодов FLT
//	  if(led_check(i)==1){
//		printf("#B#		SYSTEM ERROR: LED%d\n",i);
//		//ledEn(i,0);
//		errors++;
//	  }
//	}
//	for(int i=1;i<4;i++){						// Проверка модулей шаговых двигателей
//	  if(step_check(i)==1){
//		printf("#B#		SYSTEM ERROR: STEPPER%d\n",i);
//		PON5_set(0);	// Выключаем линию 5вольт
//		errors++;
//	  }  
//	}
//		
//	// ЛИНИЯ 12 ВОЛЬТ_______________________________________________________________________@
//	
//		//fault_check_cameras 10 12 14 15 по схеме (хз почему так)
//	
//	
//	
//	// ПРОВЕРКА LED-SR_____________________________________________________________________
//	for(int i=1;i<10;i++){
//		
//	  
//	  
//	}
//	
//	// ПРОВЕРКА CAMERA-SR___________________________________________________________________   
//	
//		//cameraGetInfo();
//	return errors;
//}