#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <pthread.h>
// self define
#include "include/MC_Controller.h"
#include "include/MQTTClient.h"
#include "include/MC_MqttClient.h"
#include "include/MC_Logic_Thread.h"
#include "include/MC_Mqueue.h"
// WiringPi
#include "hardware/rbpIac.h"
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

MC_Context_Struct g_mc_context;	
MQTTClient client;
mqd_t g_mq_client[MAX_THREAD_NUM], g_mq_server[MAX_THREAD_NUM];

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	struct mqttMsg
	{
		char *topicName;
		int topicLen;
		MQTTClient_message *message;
	}msg;
	
	msg.topicName = topicName;
	msg.topicLen = topicLen;
	msg.message = message;
	int result = mq_send(g_mq_client[MQUEUE_RECEIVER_THREAD_NUM], (char *)(&msg), sizeof(msg), 0);
	if(result == -1)
		printf("msgarrvd : ERROR : mq_send Failed\n");
	else if(result == 0)
		printf("mq_send Success\n");
	printf("=================== New Command Incoming ===================\n");
	printf("msgarrvd : topic = %s\t\t payload = 0x%08x\n", msg.topicName,*((unsigned int*)msg.message->payload));
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

int main (int argc, char* argv[])
{
	memset(&g_mc_context, 0, sizeof(g_mc_context));
	#ifdef CONVEYOR_MODULE
		printf("== Conveyor Mode ==\n");
		if(getModuleType()=='C')
		{
			if(init_Conveyer()<0)
				return -1;
			printf("\n\n");
			g_mc_context.moduleType = 'C';
			g_mc_context.maxThreadNum = CVR_MAX_THREAD_NUM;
		}
	#endif
	#ifdef ELEVATOR_MODULE
		printf("== Elevator Mode ==\n");
		if(getModuleType()=='E')
		{
			if(init_Elevator()<0)
				return -1;
			printf("\n\n");	
			g_mc_context.moduleType = 'E';
			g_mc_context.maxThreadNum = ELV_MAX_THREAD_NUM;
		}
	#endif
	
	int i;
	MQTTClient client;
	int rc = MqttClient_Connect(&client);
	if ( rc != MQTTCLIENT_SUCCESS )
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}else
		printf("MQTT Connect Success\n");

	g_mc_context.pClient = &client;
	for(i=0;i<g_mc_context.maxThreadNum;i++)
		g_mc_context.bThread_exit[i]  = false;
	g_mc_context.mode = 0; 

	MC_CMD_RECEIVER_SERVER_FNP fnServerThreadList [g_mc_context.maxThreadNum];
	if(getModuleType()=='C')
	{
	#ifdef CONVEYOR_MODULE
		fnServerThreadList[0] = CVR_MC_Station1_Logic_Thread;
		fnServerThreadList[1] = CVR_MC_Station2_Logic_Thread;
		fnServerThreadList[2] = CVR_MC_Station3_Logic_Thread;
		fnServerThreadList[3] = CVR_MC_Station4_Logic_Thread;
		fnServerThreadList[4] = CVR_MC_Station5_Logic_Thread;
		fnServerThreadList[5] = CVR_MC_Sensor_Key_Detected_Thread;
		fnServerThreadList[6] = CVR_MC_CMD_Dispatch_Thread;
	#endif
	}else if(getModuleType()=='E')
	{
	#ifdef ELEVATOR_MODULE
		fnServerThreadList[0] = ELV_MC_Station1_Logic_Thread;
		fnServerThreadList[1] = ELV_MC_Sensor_Key_Detected_Thread;
		fnServerThreadList[2] = ELV_MC_CMD_Dispatch_Thread;	
	#endif
	}

	// Create array of mqueue handle 
	g_mc_context.mqueueServerArray = g_mq_server;
	g_mc_context.mqueueClientArray = g_mq_client;
	
    if(getModuleType()=='C')
	{
	#ifdef CONVEYOR_MODULE
		if(CreateMqueue(g_mc_context.mqueueClientArray, g_mc_context.mqueueServerArray,CVR_MQUEUE_NAME, g_mc_context.maxThreadNum)==-1)
			return -1;
	#endif
	}
	if(getModuleType()=='E')
	{
	#ifdef ELEVATOR_MODULE
		if(CreateMqueue(g_mc_context.mqueueClientArray, g_mc_context.mqueueServerArray,ELV_MQUEUE_NAME,g_mc_context.maxThreadNum)==-1)
			return -1;
	#endif	
	}

	// create threads
	MC_CreateCmdReceiverServerThread( &g_mc_context, fnServerThreadList,  g_mc_context.maxThreadNum);
	//and wait untill thread killed
	
	MqttClient_Disconnect (&client);
	DestroyMqueue(g_mq_client, g_mq_server, g_mc_context.maxThreadNum);
	return 0;
}

