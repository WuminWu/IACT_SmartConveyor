#ifndef MC_MQTTCLIENT_H
#define MC_MQTTCLIENT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
#include "MQTTClient.h"

#define ADDRESS     "192.168.1.32:1883"
//#define CLIENTID    "Module Control:Elevator"
#define SUBSCRIBE_TOPIC_FROM_SCC_INIT "SCC->MC:INIT"
// Conveyor here
//#define SUBSCRIBE_TOPIC_SELF_IP "192.168.1.42"
// Elev here
//#define SUBSCRIBE_TOPIC_SELF_IP "192.168.1.37"
#define SUBSCRIBE_TOPIC_DIAG "HMI->MC:DIAG"
#define PUBLISH_TOPIC_INIT "MC->SCC:INIT"
#define PUBLISH_TOPIC_EVENT "MC->SCC:EVENT"
#define QOS         1
#define TIMEOUT     10000L
#define PAYLOAD_LEN 4

void delivered(void *context, MQTTClient_deliveryToken dt);
void connlost(void *context, char *cause);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
int MqttClient_Connect (MQTTClient *ptrClient);
int MqttClient_Disconnect (MQTTClient *ptrClient);
int publishMsg ( MQTTClient client, char *topic, char *payload, int len);

#endif


