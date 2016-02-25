#ifndef COMMON_H_
#define COMMON_H_

#include "MC_Controller.h"
	
#define CONVEYOR_MODULE
//#define ELEVATOR_MODULE
#define MAX_THREAD_NUM 7

#ifdef CONVEYOR_MODULE
	#define CVR_MODULE_TYPE 'C'
	#define CVR_MAX_THREAD_NUM 7
	#define MQUEUE_RECEIVER_THREAD_NUM 6
	#define SUBSCRIBE_TOPIC_SELF_IP "192.168.1.12"
	#define CLIENTID    "Module Control:Elevator_One"
	static char *CVR_MQUEUE_NAME[CVR_MAX_THREAD_NUM] = 
	{
		"/COV_Position1_CmdQueue",
		"/COV_Position2_CmdQueue",
		"/COV_Position3_CmdQueue",
		"/COV_Position4_CmdQueue",
		"/COV_Position5_CmdQueue",
		"/COV_Button_CmdQueue",
		"/COV_CMD_Dispatch"
	};
#endif	

#ifdef ELEVATOR_MODULE
	#define ELV_MODULE_TYPE 'E'
	#define ELV_MAX_THREAD_NUM 3
	#define MQUEUE_RECEIVER_THREAD_NUM 2
	#define SUBSCRIBE_TOPIC_SELF_IP "192.168.1.11"
	#define CLIENTID    "Module Control:Conveyor_One"
	static char *ELV_MQUEUE_NAME[ELV_MAX_THREAD_NUM] = 
	{
		"/ELEV_Position1_CmdQueue",
		"/ELEV_Button_CmdQueue",
		"/ELEV_CMD_Dispatch"
	};	
#endif

typedef struct 
{
	MQTTClient *pClient;
	bool bThread_exit[MAX_THREAD_NUM];
	mqd_t *mqueueClientArray;      // a pointer to mqd_t array
	mqd_t *mqueueServerArray;
	int mode;                                     // Normal = 0, Diag = 1
	char moduleType;
	int maxThreadNum;
}MC_Context_Struct;

typedef void (*MC_CMD_RECEIVER_SERVER_FNP)( void *pContext );
int MC_CreateCmdReceiverServerThread(MC_Context_Struct *pMc_context, MC_CMD_RECEIVER_SERVER_FNP fnServerThreadList[], int threadNum);
char getModuleType();

#endif
