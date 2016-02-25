#include "include/common.h"
#include "include/MC_Controller.h"
#include <pthread.h>
#include <mqueue.h>

char getModuleType()
{
	// if(readGPIO==0)
	//	return 'C'
	// else if(readGPIO==1)
	//	return 'E'
	#ifdef CONVEYOR_MODULE
		return 'C';
	#elif defined ELEVATOR_MODULE
		return 'E';
	#endif
}

int MC_CreateCmdReceiverServerThread(MC_Context_Struct *pMc_context, MC_CMD_RECEIVER_SERVER_FNP fnServerThreadList[], int threadNum) 
{
	printf("MC_CreateCmdReceiverServerThread+++\n");
	int  iret [threadNum];
	int i = 0;
	pthread_t tid[threadNum];
	mqd_t mq;

	for( i=0; i<threadNum; i++)
		iret [i] = pthread_create( &tid[ i ], NULL, (void *)fnServerThreadList[i], (void *) pMc_context);
	for( i=0 ; i<threadNum ; i++)
		pthread_join( tid[ i ], NULL);
	
	printf("MC_CreateCmdReceiverServerThread---\n");
	return 0;
}