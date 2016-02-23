#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "rbpIac.h"

enum{
	CONV_CMD_MOTOR = 1,
	CONV_CMD_CYLINDER,
	CONV_CMD_LED,
	CONV_CMD_BUZZER,
	CONV_CMD_RFID,
	CONV_CMD_READ,
};

CALL_BACK(button,1);
CALL_BACK(button,2);
CALL_BACK(button,3);
CALL_BACK(button,4);
CALL_BACK(button,5);
CALL_BACK(button,6);
CALL_BACK(sensor,1);
CALL_BACK(sensor,2);
CALL_BACK(sensor,3);
CALL_BACK(sensor,4);
CALL_BACK(sensor,5);
CALL_BACK(sensor,6);
CALL_BACK(sensor,7);
CALL_BACK(sensor,8);
CALL_BACK(sensor,9);
CALL_BACK(sensor,10);
CALL_BACK(sensor,11);
CALL_BACK(sensor,12);
CALL_BACK(sensor,13);
CALL_BACK(sensor,14);

int main (void)
{
	int cmd,tmp;
	bool conflag;
	char rfid[12];

	init_Conveyer();

	rbpSensor(CONV_BUTTON1, &button1_event);
	rbpSensor(CONV_BUTTON2, &button2_event);
	rbpSensor(CONV_BUTTON3, &button3_event);
	rbpSensor(CONV_BUTTON4, &button4_event);
	rbpSensor(CONV_BUTTON5, &button5_event);
	rbpSensor(CONV_BUTTON6, &button6_event);
	rbpSensor(CONV_SENSOR1, &sensor1_event);
	rbpSensor(CONV_SENSOR2, &sensor2_event);
	rbpSensor(CONV_SENSOR3, &sensor3_event);
	rbpSensor(CONV_SENSOR4, &sensor4_event);
	rbpSensor(CONV_SENSOR5, &sensor5_event);
	rbpSensor(CONV_SENSOR6, &sensor6_event);
	rbpSensor(CONV_SENSOR7, &sensor7_event);
	rbpSensor(CONV_SENSOR8, &sensor8_event);
	rbpSensor(CONV_SENSOR9, &sensor9_event);
	rbpSensor(CONV_SENSOR10, &sensor10_event);
	rbpSensor(CONV_SENSOR11, &sensor11_event);
	rbpSensor(CONV_SENSOR12, &sensor12_event);
	rbpSensor(CONV_SENSOR13, &sensor13_event);
	rbpSensor(CONV_SENSOR14, &sensor14_event);

	do {
		printf("\nFunction test >\n"
			"   1. Motor \n"
			"   2. Cylinder \n"
			"   3. Led \n"
			"   4. buzzer \n"
			"   5. RFID \n"
			"   6. Sensor read \n   >" );
		
		scanf("%d", &cmd);
		conflag = true;
		switch ( cmd ) {
			case CONV_CMD_MOTOR:
				printf("device[1-5]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = CONV_MOTOR1;
						break;
					case 2:
						cmd = CONV_MOTOR2;
						break;
					case 3:
						cmd = CONV_MOTOR3;
						break;
					case 4:
						cmd = CONV_MOTOR4;
						break;
					case 5:
						cmd = CONV_MOTOR5;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag)
					rbpMotor(cmd);
				break;
				
			case CONV_CMD_CYLINDER:
				printf("device[1-4]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = CONV_CYLINDER1;
						break;
					case 2:
						cmd = CONV_CYLINDER2;
						break;
					case 3:
						cmd = CONV_CYLINDER3;
						break;
					case 4:
						cmd = CONV_CYLINDER4;
						break;
					default:
						conflag = false;
						break;
				}
				
				if(conflag){
					rbpCylinder(cmd, ON);
					delay(3000);
					rbpCylinder(cmd, OFF);
				}
				break;
				
			case CONV_CMD_LED:
				printf("device[1-3]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = CONV_LED1;
						break;
					case 2:
						cmd = CONV_LED2;
						break;
					case 3:
						cmd = CONV_LED3;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag){
					rbpLed(cmd, ON);
					delay(3000);
					rbpLed(cmd, OFF);
				}
				break;
				
			case CONV_CMD_BUZZER:
				rbpBuzzer(ON);
				delay(3000);
				rbpBuzzer(OFF);
				break;

			case CONV_CMD_RFID:
				rbpRFID(rfid);
				printf(">> %10s\n",rfid);
				fflush (stdout) ;
				break;

			case CONV_CMD_READ:
				printf("Sensor[1-14]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = CONV_SENSOR1;
						break;
					case 2:
						cmd = CONV_SENSOR2;
						break;
					case 3:
						cmd = CONV_SENSOR3;
						break;
					case 4:
						cmd = CONV_SENSOR4;
						break;
					case 5:
						cmd = CONV_SENSOR5;
						break;
					case 6:
						cmd = CONV_SENSOR6;
						break;
					case 7:
						cmd = CONV_SENSOR7;
						break;
					case 8:
						cmd = CONV_SENSOR8;
						break;
					case 9:
						cmd = CONV_SENSOR9;
						break;
					case 10:
						cmd = CONV_SENSOR10;
						break;
					case 11:
						cmd = CONV_SENSOR11;
						break;
					case 12:
						cmd = CONV_SENSOR12;
						break;
					case 13:
						cmd = CONV_SENSOR13;
						break;
					case 14:
						cmd = CONV_SENSOR14;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag){
					tmp = rbpSensorRead(cmd);
					printf(">> %d\n",tmp);
				}
				break;
				
			default:
				return 0;
		}
		
		
	}while(1);
	
	return 0;
}	

