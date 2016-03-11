#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <mqueue.h>
#include "MQTTClient.h"
#include "include/MC_MqttClient.h"
#include "include/MC_Logic_Thread.h"
#include "include/MC_Cmd_Format.h"
// WiringPi
#include "hardware/rbpIac.h"
#include "include/common.h"
#include "include/MC_Mqueue.h"
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#ifdef ELEVATOR_MODULE

bool ELV_checkTopic(char *topicName, char *compare)
{
	if(strcmp(topicName, compare)==0)
		return true;
	else
		return false;
}

unsigned int ELV_eventPayloadFormat(unsigned int *aMsg32)
{	
	unsigned int eventPayload32 = 0;
	unsigned int moduleType = 1;
	unsigned int eventModuleID = MUDULE_ID;
	unsigned int sid = 0;
	unsigned int eventType = 0;
	unsigned int actionType = 0;
	unsigned int NA = 0;
	
	unsigned int computeTypeAndAction = ((*aMsg32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);
	unsigned int computeSID = ((*aMsg32 & BIT_MASK_SEQUENCE_ID) >> SHIFT_SEQUENCE_ID);

	switch(computeTypeAndAction)
	{
		case VALUE_ELEV_TOP_MOTOR_FORWARD:
			NA = 0 << SHIFT_NA;
			actionType = ACTION_TOP_MOVE_FORWARD_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
		case VALUE_ELEV_TOP_MOTOR_REVERSE:
			NA = 0 << SHIFT_NA;
			actionType = ACTION_TOP_MOVE_REVERSE_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
		case VALUE_ELEV_BOT_MOTOR_FORWARD:
			NA = 0 << SHIFT_NA;
			actionType = ACTION_BOT_MOVE_FORWARD_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
		case VALUE_ELEV_BOT_MOTOR_REVERSE:
			NA = 0 << SHIFT_NA;
			actionType = ACTION_BOT_MOVE_REVERSE_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : payload32 = 0x%08x\n", eventPayload32);
			break;
	 	case VALUE_ELEV_UP :
			NA = 0 << SHIFT_NA;
			actionType = ACTION_ELEVATOR_UP_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;			
		case VALUE_ELEV_DOWN :
			NA = 0 << SHIFT_NA;
			actionType = ACTION_ELEVATOR_DOWN_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID | sid | eventType | actionType | NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
		default:
			break;
	}
	return eventPayload32;
}

int ELV_publishEvent(void *aPContext, unsigned int aPyload32)
{
	unsigned int payload32 = aPyload32;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)aPContext;
	int rc = publishMsg(*(pMcContext->pClient), PUBLISH_TOPIC_EVENT, (char *)(&payload32), sizeof(payload32));
	printf("publishEvent : rc = %d\n", rc);
	return rc;
}

int ELV_DispatchThread(unsigned int *payload32, MC_Context_Struct *pMcContext)
{
    int computeThread = ((*payload32 & BIT_MASK_ControlType) >> SHIFT_ControlType);
	int result;
	switch(computeThread)
	{
	 	case VALUE_ELEV_STATION1_LOGIC_THREAD :
			printf("Here is VALUE_ELEV_STATION1_LOGIC_THREAD\n");
			result = mq_send(pMcContext->mqueueClientArray[0], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
		case VALUE_ELEV_KEY_SENSOR :
			printf("Here is VALUE_ELEV_KEY_SENSOR\n");
			result = mq_send(pMcContext->mqueueClientArray[1], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
	}
	printf("DispatchPosition Command is : 0x%08x , value = 0x%02x\n", *payload32, computeThread);	
	return 0;
}

void ELV_normalCmdParser(void *aPContext, unsigned int *payload32)
{
	unsigned int evtPayload32 = 0, result = 0;
	int computeTypeAndAction = ((*payload32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);

	switch(computeTypeAndAction)
	{
		case VALUE_ELEV_UP:
			if(!rbpSensorRead(ELEV_SEN_RAIL_F))
			{
				printf("Normal Mode : VALUE_ELEV_UP\n");
				rbpElevator_UpDown(UP);
				while(1)
				{
					if(rbpSensorRead(ELEV_SEN_ELE_H))
					{
						evtPayload32 = ELV_eventPayloadFormat(payload32);
						result = ELV_publishEvent(aPContext, evtPayload32);	
						printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
						break;
					}
				}
			}else
				printf("\nERROR: Pallet is STUCK\n");
		break;
		case VALUE_ELEV_DOWN:
			if(!rbpSensorRead(ELEV_SEN_RAIL_F))
			{
				printf("Normal Mode : VALUE_ELEV_DOWN\n");
				rbpElevator_UpDown(DOWN);
				while(1)
				{
					if(rbpSensorRead(ELEV_SEN_ELE_L))
					{
						evtPayload32 = ELV_eventPayloadFormat(payload32);
						result = ELV_publishEvent(aPContext, evtPayload32);	
						printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
						break;
					}
				}
			}else
				printf("\nERROR: Pallet is STUCK\n");
		break;
		case VALUE_ELEV_TOP_MOTOR_FORWARD:		
			break;
		case VALUE_ELEV_TOP_MOTOR_REVERSE:
			break;
		case VALUE_ELEV_TOP_MOTOR_STOP:
			break;
		case VALUE_ELEV_BOT_MOTOR_FORWARD:
			printf("Normal Mode : VALUE_ELEV_BOT_MOTOR_FORWARD\n");
			ElevatorMotorMove(ELEV_MOTOR2, ELEV_MOTOR_DIR2, FORWARD);
			// Motor stop when time's up
			delay(8000);
			evtPayload32 = ELV_eventPayloadFormat(payload32);
			result = ELV_publishEvent(aPContext, evtPayload32);	
			printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			ElevatorMotorStop(ELEV_MOTOR2);
			break;
		case VALUE_ELEV_BOT_MOTOR_REVERSE:
			printf("Normal Mode : VALUE_ELEV_BOT_MOTOR_REVERSE\n");
			ElevatorMotorMove(ELEV_MOTOR2, ELEV_MOTOR_DIR2, BACKWARD);
			// Motor stop when time's up
			delay(8000);
			evtPayload32 = ELV_eventPayloadFormat(payload32);
			result = ELV_publishEvent(aPContext, evtPayload32);	
			printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);	
			ElevatorMotorStop(ELEV_MOTOR2);
			break;
		case VALUE_ELEV_BOT_MOTOR_STOP:
			ElevatorMotorStop(ELEV_MOTOR2);
			break;
	}
	
}

int ELV_DispatchDiag(unsigned int *payload32, MC_Context_Struct *pMcContext)
{
	int computeTypeAndAction = ((*payload32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);
	int computePosition = ((*payload32 & BIT_MASK_PALLET_POSITION) >> SHIFT_PALLET_POSITION);
	switch(computeTypeAndAction)
	{
		case VALUE_ELEV_UP:
			rbpElevator_UpDown(UP);
			break;
		case VALUE_ELEV_DOWN:
			rbpElevator_UpDown(DOWN);
			break;
		case VALUE_ELEV_TOP_MOTOR_FORWARD:
			// top motor forward
			//rbpElevator_Pallet(3,UP);
			ElevatorMotorMove(ELEV_MOTOR1, ELEV_MOTOR_DIR1, FORWARD);
			break;
		case VALUE_ELEV_TOP_MOTOR_REVERSE:
			// top motor reverse
			//rbpElevator_Pallet(3,DOWN);
			ElevatorMotorMove(ELEV_MOTOR1, ELEV_MOTOR_DIR1, BACKWARD);
			break;
		case VALUE_ELEV_TOP_MOTOR_STOP:
			ElevatorMotorStop(ELEV_MOTOR1);
			break;
		case VALUE_ELEV_BOT_MOTOR_FORWARD:
			// bottom motor forward
			//rbpElevator_Pallet(4,UP);
			ElevatorMotorMove(ELEV_MOTOR2, ELEV_MOTOR_DIR2, FORWARD);
			break;
		case VALUE_ELEV_BOT_MOTOR_REVERSE:
			// bottom motor reverse
			//rbpElevator_Pallet(4,DOWN);
			ElevatorMotorMove(ELEV_MOTOR2, ELEV_MOTOR_DIR2, BACKWARD);
			break;
		case VALUE_ELEV_BOT_MOTOR_STOP:
			ElevatorMotorStop(ELEV_MOTOR2);
			break;
		case VALUE_READ_ELEVATOR_SENSOR:
			printf("VALUE_READ_ELEVATOR_SENSOR\n");
			if(computePosition==VALUE_SENSOR1)
				printf("VALUE_SENSOR_MODE : Sensor 1 = %d\n", rbpSensorRead(ELEV_SENSOR1));
			else if(computePosition==VALUE_SENSOR2)
				printf("VALUE_SENSOR_MODE : Sensor 2 = %d\n", rbpSensorRead(ELEV_SENSOR2));
			else if(computePosition==VALUE_SENSOR3)
				printf("VALUE_SENSOR_MODE : Sensor 3 = %d\n", rbpSensorRead(ELEV_SEN_RAIL_R));
			else if(computePosition==VALUE_SENSOR4)
				printf("VALUE_SENSOR_MODE : Sensor 4 = %d\n", rbpSensorRead(ELEV_SEN_RAIL_F));
			else if(computePosition==VALUE_SENSOR5)
				printf("VALUE_SENSOR_MODE : Sensor 5 = %d\n", rbpSensorRead(ELEV_SEN_ELE_H));
			else if(computePosition==VALUE_SENSOR6)
				printf("VALUE_SENSOR_MODE : Sensor 6 = %d\n", rbpSensorRead(ELEV_SEN_ELE_L));			
		default : 
			break;
	}
	printf("DispatchDiag Command is : 0x%08x , value = 0x%02x\n", *((unsigned int*)payload32),computeTypeAndAction);
	return 0;
}

void ELV_MC_Station1_Logic_Thread(void *pContext) 
{
	printf("MC_Station_Logic_Thread_ELV+++\n");
	char buffer[CMD_BUFFER_SIZE];
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext;
	do{
        ssize_t bytes_read = mq_receive(pMcContext->mqueueServerArray[0], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread_ELV\n");
		printf("mq_receive : msg_ptr = 0x%08x\n", *((unsigned int*)msg_ptr));
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");
		
		ELV_normalCmdParser(pContext ,msg_ptr);
		
	}while(!pMcContext -> bThread_exit[0]);
	printf("MC_Station1_Logic_Thread_ELV---\n");
}

void ELV_MC_Sensor_Key_Detected_Thread(void *pContext )
{
	printf("MC_Sensor_Key_Detected_Thread_ELV+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t	bytes_read = mq_receive(pMcContext->mqueueServerArray[1], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Sensor_Key_Detected_Thread\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");

	}while(!pMcContext -> bThread_exit[1]);
	printf("MC_Sensor_Key_Detected_Thread_ELV---\n");
}

unsigned int ELV_getInitPalletStatus()
{
	unsigned int palletStatus = rbpSensorRead(ELEV_SENSOR2);
	unsigned int elevPositionUp = rbpSensorRead(ELEV_SEN_ELE_H);
	unsigned int elevPositionDown = rbpSensorRead(ELEV_SEN_ELE_L);
	if(palletStatus)
	{
		if(elevPositionUp)
			return 0x02;
		else if(elevPositionDown)
			return 0x03;
	}
	else if(palletStatus==0)
	{
		if(elevPositionUp)
			return 0x00;
		else if(elevPositionDown)
			return 0x01;
	}
}

unsigned int ELV_getPalletStatsusEventPayloadFormat(unsigned int *aMsg32)
{		
	unsigned int computePosition = ELV_getInitPalletStatus();
	unsigned int computeSID = ((*aMsg32 & BIT_MASK_SEQUENCE_ID) >> SHIFT_SEQUENCE_ID);

	unsigned int NA = 0 << SHIFT_NA;
	unsigned int eventPosition = computePosition << SHIFT_POSITION;
	unsigned int NA_offset_7 = 0 << SHIFT_ACTION_DONE;
	unsigned int eventType = EVT_GetPalletStatus << SHIFT_EVENT_TYPE;
	unsigned int sid = computeSID << SHIFT_SID;
	unsigned int eventModuleID = MUDULE_ID << SHIFT_MODULE_ID;
	unsigned int moduleType = VALUE_MODULE_ELEVATOR_TYPE << SHIFT_MODULE_TYPE;		
	unsigned int eventPayload32 = moduleType | eventModuleID |sid | eventType | NA_offset_7 | eventPosition |NA;
	printf("CVR_getPalletStatsusEventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);

	return eventPayload32;
}

void ELV_MC_CMD_Dispatch_Thread(void *pContext)
{
	printf("MC_CMD_Dispatch_Thread_ELV+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext;
	
	struct mqttMsg
	{
		char *topicName;
		int topicLen;
		MQTTClient_message *message;
	}msg;

	do{
		ssize_t bytes_read;
		int i;
		
        /* receive the message */
		printf("\n\n");
        bytes_read = mq_receive(pMcContext->mqueueServerArray[MQUEUE_RECEIVER_THREAD_NUM], buffer, MAX_SIZE, NULL);
		if(bytes_read != sizeof(msg))
			printf("MC_CMD_Dispatch_Thread : ERROR : mq_receive Failed, bytes_read = %d\n", bytes_read);

		// copy structure from source mq_send
		memcpy(&msg, buffer, sizeof(msg));
		
		if(ELV_checkTopic(msg.topicName, SUBSCRIBE_TOPIC_FROM_SCC_INIT))
		{
			printf("MC_CMD_Dispatch_Thread : mq_receive : Topic = %s\n", msg.topicName);
			char *ip = "192.168.1.11,E";
			int rc = publishMsg(*(pMcContext->pClient), PUBLISH_TOPIC_INIT, ip, strlen(ip));
			printf("INIT : rc = %d\n", rc);
		}
		else if(ELV_checkTopic(msg.topicName, SUBSCRIBE_TOPIC_SELF_IP))
		{
			unsigned int *payload32 = msg.message->payload;
			if(msg.message->payloadlen != PAYLOAD_LEN)
			{
				printf("ERROR : payloadlen error\n");
				return;
			}
			
			int computeMode = ((*payload32 & BIT_MASK_MODE) >> SHIFT_MODE);
			int computeControlType = ((*payload32 & BIT_MASK_ControlType) >> SHIFT_ControlType);
			switch(computeMode)
			{
				case VALUE_MODE_DIAG : 
					pMcContext->mode = 1;
					printf("Here is Diag Mode\n");
					ELV_DispatchDiag(msg.message->payload, pMcContext);
					break;
				case VALUE_MODE_NORMAL : 
					pMcContext->mode = 0;
					printf("Here is Normal Mode\n");
					if(computeControlType==VALUE_CONV_GET_PALLET_STATUS)
					{
						unsigned int eventPayload = ELV_getPalletStatsusEventPayloadFormat(payload32);
						int result = ELV_publishEvent(pContext, eventPayload);
						if(result==0)
							printf("Publish VALUE_CONV_GET_PALLET_STATUS Success\n");
					}
					else
						ELV_DispatchThread(msg.message->payload, pMcContext);
					break;
				default : 
					break;
			}
		}
/*
		memcpy(buffer, msg.message->payload, msg.message->payloadlen);
		for(i=3; i>=0; i--)		
			printf("msg.payload = 0x%02x\n",buffer[i]);
*/

	}while(!pMcContext -> bThread_exit[2]);
	printf("MC_CMD_Dispatch_Thread_ELV---\n");	
}
#endif
