#ifndef __RBPIAC_H
#define __RBPIAC_H

#define  PCF8575_GPIO_0 0x0001
#define  PCF8575_GPIO_1 0x0002
#define  PCF8575_GPIO_2 0x0004
#define  PCF8575_GPIO_3 0x0008
#define  PCF8575_GPIO_4 0x0010
#define  PCF8575_GPIO_5 0x0020
#define  PCF8575_GPIO_6 0x0040
#define  PCF8575_GPIO_7 0x0080

#define  PCF8575_GPIO_8  0x0100
#define  PCF8575_GPIO_9  0x0200
#define  PCF8575_GPIO_10 0x0400
#define  PCF8575_GPIO_11 0x0800
#define  PCF8575_GPIO_12 0x1000
#define  PCF8575_GPIO_13 0x2000
#define  PCF8575_GPIO_14 0x4000
#define  PCF8575_GPIO_15 0x8000

#define ON 	1
#define OFF 	0

#define FORWARD 	1
#define BACKWARD 	0

#define UP 	1
#define DOWN 	0

#define MODE_CONV 	1
#define MODE_ELEV 	2

typedef enum{
	CONV_MOTOR1 = 0,
	CONV_MOTOR2 = 1,
	CONV_MOTOR3 = 2,
	CONV_MOTOR4 = 3,
	CONV_MOTOR5 = 4,
	CONV_NONE,
} convMotor;

#define CONV_MOTOR_DIRC	PCF8575_GPIO_7

typedef enum{
	CONV_CYLINDER1 = PCF8575_GPIO_0,
	CONV_CYLINDER2 = PCF8575_GPIO_1,
	CONV_CYLINDER3 = PCF8575_GPIO_2,
	CONV_CYLINDER4 = PCF8575_GPIO_3,
} convCylinder;

typedef enum{
	CONV_LED1 = PCF8575_GPIO_4,
	CONV_LED2 = PCF8575_GPIO_5,
	CONV_LED3 = PCF8575_GPIO_6,
} convLed;

#define SENSOR1		1
#define SENSOR2		2
#define SENSOR3		3
#define SENSOR4		4
#define SENSOR5		6
#define SENSOR6		5
#define SENSOR7		7
#define SENSOR8		28  //8
#define SENSOR9		29  //9
#define SENSOR10	10
#define SENSOR11	11
#define SENSOR12	12	
#define SENSOR13	13
#define SENSOR14	14

typedef enum{
	CONV_SENSOR1 = SENSOR1,
	CONV_SENSOR2 = SENSOR2,
	CONV_SENSOR3 = SENSOR3,
	CONV_SENSOR4 = SENSOR4,
	CONV_SENSOR5 = SENSOR5,
	CONV_SENSOR6 = SENSOR6,
	CONV_SENSOR7 = SENSOR7,
	CONV_SENSOR8 = SENSOR8,
	CONV_SENSOR9 = SENSOR9,
	CONV_SENSOR10 = SENSOR10,
	CONV_SENSOR11 = SENSOR11,
	CONV_SENSOR12 = SENSOR12,
	CONV_SENSOR13 = SENSOR13,
	CONV_SENSOR14 = SENSOR14,
} convSensor;

#define BUTTON1	21
#define BUTTON2	22
#define BUTTON3	23
#define BUTTON4	24
#define BUTTON5	25
#define BUTTON6	26

typedef enum{
	CONV_BUTTON1 = BUTTON1,
	CONV_BUTTON2 = BUTTON2,
	CONV_BUTTON3 = BUTTON3,
	CONV_BUTTON4 = BUTTON4,
	CONV_BUTTON5 = BUTTON5,
	CONV_BUTTON6 = BUTTON6,
} convButton;

typedef enum{
	CONV_SPEED1 = 3,
	CONV_SPEED2 = 5,
	CONV_SPEED3 = 8,
	CONV_SPEED4 = 11,
	CONV_SPEED5 = 15,
} convSpeed;

#define CONV_BUZZER PCF8575_GPIO_8

typedef enum{
	ELEV_MOTOR1 = 0,
	ELEV_MOTOR2 = 1,
	ELEV_MOTOR3 = 2,
	ELEV_NONE,
} elevMotor;

typedef enum{
	ELEV_LED1 = 12,
	ELEV_LED2 = 13,
	ELEV_LED3 = 14,
} elevLed;

typedef enum{
	ELEV_BUTTON1 = BUTTON1,
	ELEV_BUTTON2 = BUTTON2,
} elevButton;

enum{
	ELEV_MOTOR_DIR1 = 0,
	ELEV_MOTOR_DIR2 = 1,
	ELEV_MOTOR_DIR3 = 2,
};

enum{
	ELEV_SENSOR1 = 10,
	ELEV_SENSOR2 = 11,
	ELEV_SEN_RAIL_R = 3,
	ELEV_SEN_RAIL_F = 4,
	ELEV_SEN_ELE_H = 5,
	ELEV_SEN_ELE_L = 6,
};

#define ELEV_BUZZER 8

#define ISR_HANDLER_R(num) _ISR_HANDLER_R(num)

#define _ISR_HANDLER_R(num)	\
void	      						\
handler##num() {    			\
	delay(100);				\
	if(digitalRead (num) == LOW)	\
	return;						\
	if(callbackFun[num] != NULL)	\
	callbackFun[num]();		\
	delay(800);				\
	return;					\
}							\
isrHandler[num] = &handler##num;

#define ISR_HANDLER_F(num) _ISR_HANDLER_F(num)
					
#define _ISR_HANDLER_F(num)	\
void	      						\
handler##num() {    			\
	delay(100);				\
	if(digitalRead (num) == HIGH)	\
	return;						\
	if(callbackFun[num] != NULL)	\
	callbackFun[num]();		\
	delay(800);				\
	return;					\
}							\
isrHandler[num] = &handler##num;

#define CALL_BACK(txt, num)	\
void	      						\
txt##num##_event() {    		\
	printf(" " #txt #num " event\n");	\
	fflush (stdout);			\
}

#define rbpSensorRead(nPin) digitalRead(nPin)

int init_Conveyer();

int init_Elevator();

void rbpMotor(convMotor nPin);

void rbpMotorDirection(int state);

void rbpMotorSpeed(convSpeed state);

void rbpElevator(elevMotor nPin, int dir);

void rbpCylinder(convCylinder ndev, int state);

void rbpLed(convLed ndev, int state);

void rbpBuzzer(int state);

void rbpSensor(convSensor nPin, void (*function)(void));

void rbpButton(convButton nPin, void (*function)(void));

void rbpRFID(char* name);

#endif

