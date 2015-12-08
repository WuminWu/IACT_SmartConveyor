// MC_MQTTClient.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
//#pragma comment(lib, "paho-mqtt3c.lib")      // load Library

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <iostream> 
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Publish Msg from Host"
#define QOS         1
#define TIMEOUT     10000L

using namespace std;
/*
A value representing an MQTT message. 
A delivery token is returned to the client application when a message is published. 
The token can then be used to check that the message was successfully delivered to its destination

typedef int MQTTClient_deliveryToken
*/
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	//void* payloadptr;
	char str[256] = { 0 };

	printf("Message arrived\n");
	printf("topic: %s\n", topicName);
	printf("message: ");

	// Wumin : pointer void -> pointer char
	// payloadptr = message->payload;
	memcpy(str, message->payload, message->payloadlen);	
	printf("Msg length = %d\n", message->payloadlen);
	printf("Msg = %s\n", str);

	//int result = memcmp();

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

int publishMsg(MQTTClient client, char *str)
{
	/*Test Publish*/
	//#define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	pubmsg.payload = str;
	pubmsg.payloadlen = strlen(str);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

	/* int MQTTClient_waitForCompletion(MQTTClient handle, MQTTClient_deliveryToken dt, unsigned long timeout); */
	int rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

	return 1;
}

void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
	MQTTClient client;
	//#define MQTTClient_connectOptions_initializer   { {'M', 'Q', 'T', 'C'}, 4, 60, 1, 1, NULL, NULL, NULL, 30, 20, NULL, 0, NULL, 0}
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;

	//int MQTTClient_create(MQTTClient* handle, const char* serverURI, const char* clientId, 
	//						int persistence_type, void* persistence_context);
	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	//int MQTTClient_setCallbacks(MQTTClient handle, void* context, MQTTClient_connectionLost* cl,
	//							  MQTTClient_messageArrived* ma, MQTTClient_deliveryComplete* dc);
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	printf("Subscribing to topic %s\nfor client %s using QoS : %d\n\n"
		"Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);

	/*Wumin: publish test*/
	publishMsg(client, "Publish Test from host");

	do
	{
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}