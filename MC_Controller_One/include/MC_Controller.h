#ifndef MC_CONTRLLER_H
#define MC_CONTRLLER_H
#include "MQTTClient.h"
#include <stdbool.h>
#include <mqueue.h>
#include "common.h"

void mqueue_client( char buffer[ ] );
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void MC_CMD_Dispatch_Thread( void *pContext);

#endif
