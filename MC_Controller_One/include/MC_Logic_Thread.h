#ifndef MC_LOGIC_THREAD_H
#define MC_LOGIC_THREAD_H
#include "MC_Controller.h"
#include "MQTTClient.h"
#include "MC_MqttClient.h"
#include <stdbool.h>

#define CMD_BUFFER_SIZE 4

#ifdef CONVEYOR_MODULE
	void CVR_MC_Station1_Logic_Thread( void *pContext);
	void CVR_MC_Station2_Logic_Thread( void *pContext);
	void CVR_MC_Station3_Logic_Thread( void *pContext);
	void CVR_MC_Station4_Logic_Thread( void *pContext);
	void CVR_MC_Station5_Logic_Thread( void *pContext);
	void CVR_MC_Sensor_Key_Detected_Thread( void *pContext );
	void CVR_MC_CMD_Dispatch_Thread(void *pContext);
	void cbConvSensor_PositionOneArrived();
	unsigned int initPalletStatus();
#endif
//#elif defined ELEVATOR_MODULE
#ifdef ELEVATOR_MODULE
	void ELV_MC_Station1_Logic_Thread( void *pContext);
	void ELV_MC_Sensor_Key_Detected_Thread( void *pContext );
	void ELV_MC_CMD_Dispatch_Thread(void *pContext);
#endif
	
#endif
