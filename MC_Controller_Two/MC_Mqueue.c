#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include "include/MC_Mqueue.h"
#include "include/MC_Logic_Thread.h"

int CreateMqueue( mqd_t *hClientArray, mqd_t *hServerArray, char *MQUEUE_NAME[], int mqueueNum)
{ 
	mqd_t mq, mqServer, mqClient;
	int i, j, k;

	struct mq_attr attr[mqueueNum];
	char buffer[MAX_SIZE + 1];

	//clean last execution mqueue temp file
	FILE *pp;    
	if ((pp = popen("sudo rm -rf /dev/mqueue/*", "r")) == NULL) {  
		printf("CreateMqueue : popen() error!\n");  
		return -1;  
	}else
		printf("Delete /dev/mqueue/* Success\n");  
	pclose(pp);
	
	for(i=0; i<mqueueNum; i++)
	{
		attr[i].mq_flags = 0;
		attr[i].mq_maxmsg = 10;
		attr[i].mq_msgsize = MAX_SIZE;
		attr[i].mq_curmsgs = 0;
		
		hServerArray[i] = mq_open(MQUEUE_NAME[i], O_CREAT | O_RDONLY, 0644, &attr[i]);
		if(hServerArray[i]==-1)
		{
			printf("mq_open Server MQUEUE_NAME Failed\n");
			return -1;
		}
		hClientArray[i] = mq_open(MQUEUE_NAME[i] , O_WRONLY);
		if(hClientArray[i]==-1)
		{
			printf("mq_open Client MQUEUE_NAME Failed\n");
			return -1;
		}
	}
	printf("mq_open Server&Client Success\n");
	return 0;
}

int DestroyMqueue( mqd_t *hClientArray,  mqd_t *hServerArray, int mqueueNum)
{
	int i = 0;
	for(i=0; i<mqueueNum; i++)
	{
		mq_close(hClientArray[i]);
		mq_close(hServerArray[i]);
	}
	return 0;
}
