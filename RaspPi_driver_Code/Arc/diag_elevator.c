#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "rbpIac.h"

enum{
	ELEV_CMD_MOTOR_UP = 1,
	ELEV_CMD_MOTOR_DOWN,
	ELEV_CMD_LED,
	ELEV_CMD_BUZZER,
};

CALL_BACK(button,1);
CALL_BACK(button,2);
CALL_BACK(sensor,1);
CALL_BACK(sensor,2);

int main (void)
{
	int cmd;
	bool conflag;

	init_Elevator();

	rbpButton(ELEV_BUTTON1, &button1_event);
	rbpButton(ELEV_BUTTON1, &button2_event);
	rbpSensor(ELEV_SENSOR1, &sensor1_event);
	rbpSensor(ELEV_SENSOR2, &sensor2_event);

	do {
		printf("\nFunction test >\n"
			"   1. Motor up \n"
			"   2. Motor down \n"
			"   3. Led \n"
			"   4. buzzer \n");
		
		scanf("%d", &cmd);
		conflag = true;
		switch ( cmd ) {
			case ELEV_CMD_MOTOR_UP:
				printf("device[1-3]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = ELEV_MOTOR1;
						break;
					case 2:
						cmd = ELEV_MOTOR2;
						break;
					case 3:
						cmd = ELEV_MOTOR3;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag)
					rbpElevator(cmd,UP);
				break;

			case ELEV_CMD_MOTOR_DOWN:
				printf("device[1-3]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = ELEV_MOTOR1;
						break;
					case 2:
						cmd = ELEV_MOTOR2;
						break;
					case 3:
						cmd = ELEV_MOTOR3;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag)
					rbpElevator(cmd,DOWN);
				break;
				
			case ELEV_CMD_LED:
				printf("device[1-3]:");
				scanf("%d", &cmd);
				switch ( cmd ) {
					case 1:
						cmd = ELEV_LED1;
						break;
					case 2:
						cmd = ELEV_LED2;
						break;
					case 3:
						cmd = ELEV_LED3;
						break;
					default:
						conflag = false;
						break;
				}
				if(conflag)
					rbpLed(cmd, ON);
					delay(3000);
					rbpLed(cmd, OFF);
				break;
				
			case ELEV_CMD_BUZZER:
				rbpBuzzer(ON);
				delay(3000);
				rbpBuzzer(OFF);
				break;

			default:
				return 0;
		}
		
		
	}while(1);
	
	return 0;
}	

