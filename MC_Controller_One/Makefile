all:  
	gcc MC_Controller.c common.c MC_MqttClient.c MC_Logic_Thread_CVR.c MC_Logic_Thread_ELV.c MC_Mqueue.c hardware/rbpIac.c -o main -lpthread -lpaho-mqtt3c -lrt -lwiringPi -lwiringPiDev
clean:
	rm -f main
