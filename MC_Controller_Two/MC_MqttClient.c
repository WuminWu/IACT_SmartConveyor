#include "include/MC_MqttClient.h"
#include "include/common.h"

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered( void *context, MQTTClient_deliveryToken dt )
{
	//printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

int publishMsg ( MQTTClient client, char *topic, char *payload, int len)
{
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	char *strTopic = topic;
	pubmsg.payload = payload;
	pubmsg.payloadlen = len;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, strTopic, &pubmsg, &token);

	int rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
	printf("Publish waitComplete rc = %d\n", rc);
	return rc;
}

void connlost( void *context, char *cause )
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

// move to MC_Controller.c
/*
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	char str[1024] = { 0 };
	int result = 0;

	printf("Message arrived\n");
	printf("topic: %s\n", topicName);
	printf("message: ");

	memcpy(str, message->payload, message->payloadlen);
	printf("Get Msg String = %s\n", str);

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}
*/

int MqttClient_Connect ( MQTTClient *ptrClient )
{
	int rc, setCallBack, create;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if((create = MQTTClient_create(ptrClient, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
	{
		printf("ERROR : MQTTClient_create, create = %d\n", create);
		return create;		
	}

	if((setCallBack = MQTTClient_setCallbacks(*ptrClient, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
	{
		printf("ERROR : MQTTClient_setCallbacks, setCallBack = %d\n", setCallBack);
		return setCallBack;
	}

	if ((rc = MQTTClient_connect(*ptrClient, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("ERROR : MQTTClient_connect, rc = %d\n", rc);
		return rc;
	}
	
	MQTTClient_subscribe(*ptrClient, SUBSCRIBE_TOPIC_FROM_SCC_INIT, QOS);
	MQTTClient_subscribe(*ptrClient, SUBSCRIBE_TOPIC_SELF_IP, QOS);
	MQTTClient_subscribe(*ptrClient, SUBSCRIBE_TOPIC_DIAG, QOS);
	return 0;
}

int MqttClient_Disconnect ( MQTTClient *ptrClient )
{
	MQTTClient_disconnect(*ptrClient, 10000);
	MQTTClient_destroy(ptrClient);
	return 0;
}
