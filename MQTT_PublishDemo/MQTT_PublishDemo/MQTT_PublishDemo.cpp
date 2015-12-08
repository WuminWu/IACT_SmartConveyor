// MQTT_PublishDemo.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Here is Publish Client PAYLOAD"
#define PAYLOAD2    "Here is Publish Client Second PAYLOAD"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
	MQTTClient client;
	// #define MQTTClient_connectOptions_initializer   { {'M', 'Q', 'T', 'C'}, 4, 60, 1, 1, NULL, NULL, NULL, 30, 20, NULL, 0, NULL, 0}
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	// #define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	int rc;

	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	pubmsg.payload = PAYLOAD;
	pubmsg.payloadlen = strlen(PAYLOAD);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	printf("Waiting for up to : %d seconds\nfor publication of : %s\n"
		"on topic : %s for client \nwith ClientID: %s\n",
		(int)(TIMEOUT / 1000), PAYLOAD, TOPIC, CLIENTID);

/*Test*/
	printf("Message with delivery token %d delivered\n", token);
	pubmsg.payload = PAYLOAD2;
	pubmsg.payloadlen = strlen(PAYLOAD2);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

  /* int MQTTClient_waitForCompletion(MQTTClient handle, MQTTClient_deliveryToken dt, unsigned long timeout); */
	rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
	printf("Message with delivery token %d delivered\n", token);
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

	system("pause");
	return rc;
}

