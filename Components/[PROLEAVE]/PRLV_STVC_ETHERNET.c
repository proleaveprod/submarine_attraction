#include "PRLV_STVC_ETHERNET.h"
#include "PRLV_STVC.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"

volatile float new_pitch;
volatile float new_yaw;
volatile float current_yaw;
volatile float current_pitch;
volatile uint8_t calibrate_status;
volatile uint32_t calibrateReq;

volatile prlv_response_gz_struct sysCheckData;
//volatile prlv_response_rtnpa_struct sysCheckData;

void responceGZ(uint8_t *databuff){
  RGBsetColor(COLOR_GETSTATES); 
  printf("GET_STATES GZ(0x01)\n");
  

  prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  //prlv_response_gz_struct* Data = (prlv_response_gz_struct*)(databuff+8);
  
  printf("	Data collecting\n");
  // Сбор инфы*******
   
  memcpy(&databuff[8],	(prlv_response_gz_struct*)&sysCheckData,	sizeof(sysCheckData));
 
  Head->header = PROTOCOL_HEADER;
  Head->lenght = 24;
  Head->id = 0x2001;
  Head->reserved=0;
  
}

void responceRTNPA(uint8_t *databuff){
  RGBsetColor(COLOR_GETSTATES);
  printf("GET_STATES RTNPA(0x01)\n");
  
  
  prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  prlv_response_rtnpa_struct* Data = (prlv_response_rtnpa_struct*)(databuff+8);
  
  
  printf("		Data collecting\n");
  Head->header = PROTOCOL_HEADER;
  Head->lenght = 44;
  Head->id = 0x2001;
  Head->reserved=0;
  
  // Сбор инфы*********
  Data->yaw = 45.5;
  Data->pitch = -100;
  Data->L1_state = 1;
  Data->L1_bright = 100;
  Data->L2_state = 1;
  Data->L2_bright = 100;
  Data->L3_state = 1;
  Data->L3_bright = 100;
  Data->L4_state = 1;
  Data->L4_bright = 100;
  Data->L5_state = 1;
  Data->L5_bright = 100;
  Data->L6_state = 1;
  Data->L6_bright = 100;
  Data->L7_state = 1;
  Data->L7_bright = 100;
  Data->L8_state = 1;
  Data->L8_bright = 100;
  Data->L9_state = 1;
  Data->L9_bright = 100; 
  Data->CB_temp = 30;
  Data->CB_humid = 10;
  Data->laser_state = 1;
  Data->calibrating = 3; //Все калиброваны
  
  Data->reserved = 0;
  //*******************
 
}

void setMotors(uint32_t *dataRx,uint8_t *databuff){
  RGBsetColor(COLOR_SETMOTORS);
  printf("SET_MOTORS(0x02)\n");
  
  prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  prlv_data_setmotors_struct* Data = (prlv_data_setmotors_struct*)&dataRx[2];
  
  printf("		Pitch = %.3f\n		Yaw = %.3f\n",Data->pitch,Data->yaw);
  
  new_pitch = Data->pitch;
  new_yaw = Data->yaw;
  

  
	
	
  //printf("***SEMAPHORE FUNCTION***\n");
  
  
  //printf("Responce collecting\n");
  
  Head->header = PROTOCOL_HEADER;
  Head->id = 0x2002;
  Head->lenght = 0;
  Head->reserved=0;
}

void setOffsetMotors(uint32_t *dataRx){
  RGBsetColor(COLOR_SETOFFMOTORS);
  printf("OFFSET_MOTORS GZ(0x03)\n");
  

  prlv_data_setmotors_struct* Data = (prlv_data_setmotors_struct*)&dataRx[2];
  
  printf("	Pitch = %f\n	Yaw = %f\n",Data->pitch,Data->yaw);
  
  // Получаем абсолютное значение в градусах
  float absPitch = current_pitch + Data->pitch;
  float absYaw = current_yaw + Data->yaw;
  
  // Исключаем выход за порог (-180;180)
  if(absPitch>180)absPitch=180;
  else if(absPitch<-180)absPitch=-180;
  if(absYaw>180)absYaw=180;
  else if(absYaw<-180)absYaw=-180;
   
  new_pitch = absPitch;
  new_yaw = absYaw;
  
}

void setLamps(uint32_t *dataRx,uint8_t *databuff){
  RGBsetColor(COLOR_SETLAMPS);
  printf("SET_LAMPS (0x04)\n");
  
  prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  prlv_data_setlamps_struct* Data = (prlv_data_setlamps_struct*)&dataRx[2];
  
  
  printf("	Lamp:%d	\n	Brightness:%d\n",Data->id,Data->bright);
  
  if(Data->bright!=0)ledOn(Data->id);
  else ledOff(Data->id);
  
  ledSetLvl(Data->id,Data->bright);
  
  Head->header = PROTOCOL_HEADER;
  Head->id = 0x2004;
  Head->lenght = 4;
  Head->reserved=0;
}

void setLaser(uint32_t *dataRx,uint8_t *databuff){
	RGBsetColor(COLOR_SETLASER);
  	printf("SET_LASER (0x05)\n");
	
  	prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  	prlv_data_setlaser_struct* Data = (prlv_data_setlaser_struct*)&dataRx[2];
	
	printf("	Laser: ");
	if(Data->state==1)printf("ON\n");else printf("OFF\n");
	
	laserSet(Data->state);
	
	Head->header = PROTOCOL_HEADER;
    Head->id = 0x2005;
    Head->lenght = 0;
	Head->reserved=0;
}

void setCalibration(uint32_t *dataRx,uint8_t *databuff){
  RGBsetColor(COLOR_SETCALIBRATION);
  printf("SET_CALIBRATION (0x06)\n");
  prlv_header_struct* Head = (prlv_header_struct*)(databuff);
  prlv_data_calibrating_struct* Data = (prlv_data_calibrating_struct*)&dataRx[2];

  printf("		Calibrating ID:%d\n",Data->state);
  calibrateReq = Data->state;
  
 
  //stepCalibrate(Data->state);

  //printf("***SEMAPHORE FUNCTION***\n");
  
  Head->header = PROTOCOL_HEADER;
  Head->id = 0x2006;
  Head->lenght = 0;
  Head->reserved=0;
}


void updateInfoGZ(void){
  	uint16_t ledInfo = 0;
  
  	sysCheckData.yaw = current_yaw;
	
	
	// Отправляем данные L1_STATE и L1_BRIGHT сразу 16-битным возвратом значения от функции ledGetInfo.
	
	ledInfo = ledGetInfo(1);
	memcpy((uint8_t*)&sysCheckData.L1_state,(uint8_t*)&ledInfo,2);


	ledInfo = ledGetInfo(2);
	memcpy((uint8_t*)&sysCheckData.L2_state,(uint8_t*)&ledInfo,2);
	
	ledInfo = 0x0000;//ledGetInfo(3);
	memcpy((uint8_t*)&sysCheckData.L3_state,(uint8_t*)&ledInfo,2);
	
	sysCheckData.CB_temp = 69; // СДЕЛАТЬ ПОЛУЧЕНИЕ ЗНАЧЕНИЯ ТЕМПЕРАТУРЫ
	sysCheckData.CB_humid = 0x00;
	
	
	sysCheckData.power_status = 0 ;
	
	
	for(int i=0;i<3;i++)sysCheckData.power_status |= ( tvs_check(i+1) << i); //ТВК1 ТВК2 ТВК3
	for(int i=0;i<3;i++)sysCheckData.power_status |= (led_check(i+1) << (i+3)); // ОП1 ОП2 ОП3
	sysCheckData.power_status |= (step_check(1) << 6); // МОТОР ПУ1
	
	sysCheckData.flowing = fluid_check(1);
	sysCheckData.calibrating = calibrate_status;
	sysCheckData.reserved[0]=0;
	sysCheckData.reserved[1]=0;
  
}
	
//void updateInfoRTNPA(void){
//  	uint16_t ledInfo = 0;
//  
//
//  	sysCheckData.yaw = current_yaw;
//	sysCheckData.pitch = current_pitch;
//	
//	
//	
//	// Отправляем данные L1_STATE и L1_BRIGHT сразу 16-битным возвратом значения от функции ledGetInfo.
//		
//	for(int i=0;i<9;i++){
//	ledInfo = ledGetInfo(i+1);
//	memcpy((uint8_t*)&(sysCheckData.L1_state)+4*i,(uint8_t*)&ledInfo,2);
//	}
//	
//	sysCheckData.CB_temp = 0x66; // СДЕЛАТЬ ПОЛУЧЕНИЕ ЗНАЧЕНИЯ ТЕМПЕРАТУРЫ
//	sysCheckData.CB_humid = 0x00;
//	
//	sysCheckData.power_status = 0xAAAA ;
//
//	// Здесь надо сделать сбор информации в зависимости от того надо получать с линий питания.
//	
////	for(int i=0;i<3;i++)sysCheckData.power_status |= ( tvs_check(i+1) << i); //ТВК1 ТВК2 ТВК3
////	for(int i=0;i<3;i++)sysCheckData.power_status |= (led_check(i+1) << (i+3)); // ОП1 ОП2 ОП3
////	sysCheckData.power_status |= (step_check(1) << 6); // МОТОР ПУ1
//	
//	sysCheckData.flowing = fluid_check(1);
//	sysCheckData.calibrating = calibrate_status;
//	sysCheckData.reserved=0;
//  
//}	
	
	

