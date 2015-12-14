// MC_MQTTClient.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
//#pragma comment(lib, "paho-mqtt3c.lib")      // load Library

#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "IAC Demo"
//#define PAYLOAD     "Publish Msg from Host"
#define Msg         "Publish Msg from host"
#define Msg2        "Republish Msg from host again"
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
static int flag;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Get Msg\n");
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int publishMsg(MQTTClient client,char *topic, char *str)
{
	/*Test Publish*/
	//#define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	char *strTopic = topic;
	pubmsg.payload = str;
	pubmsg.payloadlen = strlen(str);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, strTopic, &pubmsg, &token);

	/* int MQTTClient_waitForCompletion(MQTTClient handle, MQTTClient_deliveryToken dt, unsigned long timeout); */
	int rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
	return rc;
}

void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	char str[1024] = { 0 };

	printf("Message arrived\n");
	printf("topic: %s\n", topicName);
	printf("message: ");

	memcpy(str, message->payload, message->payloadlen);
	printf("Get Msg String = %s\n", str);

	/*Wumin : Compare Msg what you received, and according it to decide action items. */
	if (int result = memcmp(str, Msg, sizeof(Msg)) == 0)
		flag = 1;
	else if (int result = memcmp(str, Msg2, sizeof(Msg2)) == 0)
		flag = 2;
	else
		flag = -1;

	printf("flag = %d\n\n\n", flag);

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

int main(int argc, char* argv[])
{
	int rc, ch, count=0;
	MQTTClient client;
	//#define MQTTClient_connectOptions_initializer   { {'M', 'Q', 'T', 'C'}, 4, 60, 1, 1, NULL, NULL, NULL, 30, 20, NULL, 0, NULL, 0}
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

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
	MQTTClient_subscribe(client, TOPIC, QOS);

	/* Wumin: publish test */
	publishMsg(client, TOPIC, Msg);

	/* Wumin : Switch case to repply SCC from arrived message */
	while (1)
	{
		switch (flag){
			case 1:
				publishMsg(client, TOPIC, Msg2);
				break;
			case 2 :
				publishMsg(client, TOPIC, "The End");
				break;
			default:
				break;
		}
	}

	do
	{
		ch = getchar();
	} while (ch != 'Q' && ch != 'q');

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	putchar('\n');
	system("pause");
	return rc;
}