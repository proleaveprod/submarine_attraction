#ifndef _STVC_ETHERNET_H
#define STVC_H

#include "main.h"

#define PROTOCOL_HEADER				0x55AA77CC
#define PROTOCOL_ID_GETSTATES		0x0001
#define PROTOCOL_ID_SETMOTORS		0x0002
#define PROTOCOL_ID_SETOFFMOTORS	0x0003
#define PROTOCOL_ID_SETLAMPS		0x0004
#define PROTOCOL_ID_SETLASER		0x0005
#define PROTOCOL_ID_CALIBRATION		0x0006


#define COLOR_SETMOTORS			255,30,0	// Оранжевый
#define COLOR_SETOFFMOTORS		255,150,00	// Желтевато-зеленый
#define COLOR_SETCALIBRATION	255,0,50	// Розовый
#define COLOR_GETSTATES			65,105,255	// Синеватый
#define COLOR_SETLAMPS			0,0,255		// Синий 
#define COLOR_SETLASER			255,0,255	// Фиолетовый
#define COLOR_NULL				0,0,0		// Черный




// ID = 0x0001
void responceGZ(uint8_t *databuff);
void responceRTNPA(uint8_t *databuff);

// ID = 0x0002
void setMotors(uint32_t *dataRx,uint8_t *databuff);

// ID = 0x0003
void setOffsetMotors(uint32_t *dataRx);

// ID = 0x0004
void setLamps(uint32_t *dataRx,uint8_t *databuff);

// ID = 0x0005
void setLaser(uint32_t *dataRx,uint8_t *databuff);

// ID = 0x0006
void setCalibration(uint32_t *dataRx,uint8_t *databuff);





void updateInfoGZ(void);
void updateInfoRTNPA(void);





















//________CТРУКТУРЫ_________________CТРУКТУРЫ_________________CТРУКТУРЫ_________________CТРУКТУРЫ_________________CТРУКТУРЫ_________________CТРУКТУРЫ_________________CТРУКТУРЫ_________

typedef struct {	//prlv_header_struct
  uint32_t header;
  uint8_t lenght;
  uint8_t reserved;
  uint16_t id;
}prlv_header_struct;


typedef struct {	//prlv_data_setmotors_struct
  float yaw;
  float pitch;
  uint32_t reserve;
}prlv_data_setmotors_struct;

typedef struct{		//prlv_data_offsetmotors_struct
  float d_yaw;
  float d_pitch;
  uint32_t reserve; 
}prlv_data_offsetmotors_struct;

typedef struct{		//prlv_data_setlamps_struct
  uint8_t id;
  uint8_t bright;
  uint16_t reserved;
  
}prlv_data_setlamps_struct;

typedef struct{		//prlv_data_setlaser_struct
  uint8_t state;
  uint8_t reserved[3];
}prlv_data_setlaser_struct;

typedef struct{		//prlv_data_calibrating_struct
  uint8_t state;
  uint8_t reserved[3];
}prlv_data_calibrating_struct;

typedef struct{		//prlv_response_gz
  	float yaw;
  
	uint8_t L1_state;				
	uint8_t L1_bright;
	uint8_t L2_state;
	uint8_t L2_bright;
	
	uint8_t L3_state;
	uint8_t L3_bright;
	int8_t 	CB_temp;//градусы
	uint8_t CB_humid;//проценты
	
	uint32_t power_status;
	
	uint8_t flowing;
	uint8_t calibrating;
	uint8_t reserved[2];
	
}prlv_response_gz_struct;

typedef struct{		//prlv_response_rtnpa
  	float yaw;
	
	float pitch;
	
	uint8_t L1_state;				
	uint8_t L1_bright;
	uint8_t L2_state;
	uint8_t L2_bright;
	
	uint8_t L3_state;				
	uint8_t L3_bright;
	uint8_t L4_state;
	uint8_t L4_bright;
	
	uint8_t L5_state;				
	uint8_t L5_bright;
	uint8_t L6_state;
	uint8_t L6_bright;
	
	uint8_t L7_state;				
	uint8_t L7_bright;
	uint8_t L8_state;
	uint8_t L8_bright;
	
	uint8_t L9_state;				
	uint8_t L9_bright;
	uint8_t CB_temp;
	uint8_t CB_humid;
	
	uint32_t power_status;
	
	uint8_t flowing;
	uint8_t laser_state;
	uint8_t calibrating;
	uint8_t reserved;
	 
}prlv_response_rtnpa_struct;


#endif //___________________________________________________________________________________________________________________________________________________________________________