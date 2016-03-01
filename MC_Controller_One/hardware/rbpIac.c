#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <pthread.h>
#include <stdbool.h>
#include "rbpIac.h"

int i2c_ext, i2c_pwm;
int fd;
int mode, delay_time = 4000;
static int dir = FORWARD;
static pthread_mutex_t pinMutex ;
static volatile int    pinPass = -1 ;
static void (*isrHandler [32])(void) ;
static void (*callbackFun [32])(void) ;

void set_PWM_ON_PCA9685(int i2c_fd, int ch, int val )
{
	int low_byte_val  = val & 0x00FF; 
	int high_byte_val = (val & 0x0F00) >> 8;
	int reg_low_byte = 0x06 + 4 * ch;
 
 	wiringPiI2CWriteReg8( i2c_fd , reg_low_byte	  ,low_byte_val );
 	wiringPiI2CWriteReg8( i2c_fd , reg_low_byte+1 ,high_byte_val );
}	

void set_PWM_OFF_PCA9685(int i2c_fd, int ch, int val )
{
	int low_byte_val  = val & 0x00FF; 
	int high_byte_val = (val & 0x0F00) >> 8;
	int reg_low_byte = 0x08 + 4 * ch;
 
 	wiringPiI2CWriteReg8( i2c_fd , reg_low_byte	  ,low_byte_val );
 	wiringPiI2CWriteReg8( i2c_fd , reg_low_byte+1 ,high_byte_val );	
}

void set_LOW_PCF8575(int i2c_fd,int pins )
{
 int low_byte_val,high_byte_val;
 int val, ctrl_bit = 0x01, i=0;
 
 val = wiringPiI2CReadReg16( i2c_fd , 0x00 );
 for ( i = 0 ; i < 16 ; i++ ) {
 	 if ( ctrl_bit & pins ) 
  	 	   val = (val & (~ctrl_bit));
 	 
 	 ctrl_bit=ctrl_bit<<1;
 }
 
 low_byte_val = val & 0x00FF; 
 high_byte_val = (val & 0xFF00) >> 8;
 wiringPiI2CWriteReg8( i2c_fd , low_byte_val, high_byte_val );
}	

void set_HIGH_PCF8575(int i2c_fd,int pins )
{
 int low_byte_val,high_byte_val;
 int val, ctrl_bit = 0x01, i=0;
 
 val = wiringPiI2CReadReg16( i2c_fd , 0x00 );
 for ( i = 0 ; i < 16 ; i++ ) {
 	 if ( ctrl_bit & pins ) 
  	 	   val = val|ctrl_bit ;
 	 
 	 ctrl_bit=ctrl_bit<<1;
 }
 
 low_byte_val = val & 0x00FF; 
 high_byte_val = (val & 0xFF00) >> 8;
 wiringPiI2CWriteReg8( i2c_fd , low_byte_val, high_byte_val );
}	

void init_PCA9685(int i2c_fd , convSpeed sed)
{
	wiringPiI2CWriteReg8( i2c_fd , 0x00 ,0x80 );
	delay(500);
	wiringPiI2CWriteReg8( i2c_fd , 0x00 ,0x10 );
	wiringPiI2CWriteReg8( i2c_fd , 0xfe ,sed );//Prescale
	wiringPiI2CWriteReg8( i2c_fd , 0x00 ,0x01 );
}

//Wumin
void cbConvSensor_13()
{
	delay(50);
	printf("cbSensor 13\n");
	delay(4000);
	set_PWM_OFF_PCA9685( i2c_pwm , CONV_MOTOR5 , 0x0 );
}

void rbpButtonInit(convSensor nPin, void (*function)(void))
{
	callbackFun[nPin] = function;
	//wiringPiISR (nPin, INT_EDGE_RISING, (void *)isrHandler[nPin]) ;
}

// David
int DoFalling( int num )
{
  int debounceTime =0;
  static int button_cnt=0;
	//printf ("\r\n");
	while (digitalRead (num) == LOW){
	   delay (10) ;
	  // printf ("\r\npin %d:", digitalRead (1)) ;
	  //printf ("L");
	  if ( debounceTime++ > 20 ) {
      // ++globalCounter [7] ;	  
		callbackFun[num]();
	//	printf ("nbutton %d,%d\r\n",num,++button_cnt);
	//	fflush (stdout) ;
	    break;
	  }
	} 
  //++globalCounter [1] ; 
  debounceTime =0;
	while (digitalRead (num) == LOW) {//wait key release
	   delay(10);
	   if ( ++debounceTime > 50)
		   break;
	}
	//printf ("H");
	//fflush (stdout) ;
}

void BTN_1_Interrupt (void) 
{
	printf("BTN_1_Interrupt\n");
	DoFalling(CONV_BUTTON1);
}
void BTN_2_Interrupt (void) 
{
	printf("BTN_2_Interrupt\n");
	DoFalling(CONV_BUTTON2);
}
void BTN_3_Interrupt (void) 
{
	printf("BTN_3_Interrupt\n");
	DoFalling(CONV_BUTTON3);
}
void BTN_4_Interrupt (void) 
{
	printf("BTN_4_Interrupt\n");
	DoFalling(CONV_BUTTON4);
}

void rbpCylinderFn(int num,int *cyl_swt)
{
int cmd=0;

	    switch ( num ) {
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
			//conflag = false;
		     break;
	     }
	     if(*cyl_swt){
			rbpCylinder(cmd, ON);
			*cyl_swt = 0;
	     }else {
			*cyl_swt=1;
			rbpCylinder(cmd, OFF);
	    }
        	
}
void button1_event1(void)
{
	static int cyl_swt=1;
    rbpCylinderFn(1,&cyl_swt);	
}
void button2_event1(void)
{
	static int cyl_swt=1;
    rbpCylinderFn(2,&cyl_swt);	
}
void button3_event1(void)
{
	static int cyl_swt=1;
	rbpCylinderFn(3,&cyl_swt);	
}
void button4_event1(void)
{
	static int cyl_swt=1;
	rbpCylinderFn(4,&cyl_swt);	
}

void init_InterruptSensor_1(void (*pf)(void))
{
	printf("init_InterruptSensor_1\n");
	wiringPiISR (CONV_SENSOR1, INT_EDGE_RISING, pf);
}

void deinit_InterruptSensor_1(void (*pf)(void))
{
	printf("deinit_InterruptSensor_1\n");
	wiringPiISR (CONV_SENSOR1, INT_EDGE_SETUP, pf);
}

int init_Conveyer()
{	
	if ((i2c_ext= wiringPiI2CSetup (0x20)) == -1 ) {
		printf("Initialize PCF8575 fail !!\n");
		return -1;
	}

	if ((i2c_pwm= wiringPiI2CSetup (0x40)) == -1 ) {
		printf("Initialize PCA9685 fail !!\n");
		return -1;
	}

	if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
	{
		printf("Unable to open serial device tty\n") ;
		return -1;
	}

	init_PCA9685( i2c_pwm, CONV_SPEED1 );
	wiringPiSetup () ;

	pullUpDnControl (7, PUD_DOWN);
	pullUpDnControl (10, PUD_DOWN);
	pullUpDnControl (11, PUD_DOWN);
	pullUpDnControl (29, PUD_DOWN);
	pullUpDnControl (30, PUD_DOWN);
	pullUpDnControl (31, PUD_DOWN); 
	pullUpDnControl (CONV_BUTTON1, PUD_UP);
	pullUpDnControl (CONV_BUTTON2, PUD_UP);
	pullUpDnControl (CONV_BUTTON3, PUD_UP);
	pullUpDnControl (CONV_BUTTON4, PUD_UP);
	pullUpDnControl (CONV_BUTTON5, PUD_UP);
	pullUpDnControl (CONV_BUTTON6, PUD_UP);

	set_LOW_PCF8575( i2c_ext, CONV_CYLINDER1 );
	set_LOW_PCF8575( i2c_ext, CONV_CYLINDER2);
	set_LOW_PCF8575( i2c_ext, CONV_CYLINDER3 );
	set_LOW_PCF8575( i2c_ext, CONV_CYLINDER4 );
	set_LOW_PCF8575( i2c_ext, CONV_LED1 );
	set_LOW_PCF8575( i2c_ext, CONV_LED2 );
	set_LOW_PCF8575( i2c_ext, CONV_LED3 );

	ISR_HANDLER_F(CONV_BUTTON1);
	ISR_HANDLER_F(CONV_BUTTON2);
	ISR_HANDLER_F(CONV_BUTTON3);
	ISR_HANDLER_F(CONV_BUTTON4);
	ISR_HANDLER_F(CONV_BUTTON5);
	ISR_HANDLER_R(CONV_BUTTON6);
	ISR_HANDLER_R(CONV_SENSOR1);
	ISR_HANDLER_R(CONV_SENSOR2);
	ISR_HANDLER_R(CONV_SENSOR3);
	ISR_HANDLER_R(CONV_SENSOR4);
	ISR_HANDLER_R(CONV_SENSOR5);
	ISR_HANDLER_R(CONV_SENSOR6);
	ISR_HANDLER_R(CONV_SENSOR7);
	ISR_HANDLER_R(CONV_SENSOR8);
	ISR_HANDLER_R(CONV_SENSOR9);
	ISR_HANDLER_R(CONV_SENSOR10);
	ISR_HANDLER_R(CONV_SENSOR11);
	ISR_HANDLER_R(CONV_SENSOR12);
	ISR_HANDLER_R(CONV_SENSOR13);
	ISR_HANDLER_R(CONV_SENSOR14);

	mode = MODE_CONV;
	//Wumin : Bot Conveyor interrupt stop
	wiringPiISR (CONV_SENSOR13, INT_EDGE_RISING, &cbConvSensor_13);
	
	// David's button call back
	wiringPiISR (CONV_BUTTON1, INT_EDGE_FALLING, &BTN_1_Interrupt) ;
	rbpButtonInit(CONV_BUTTON1,button1_event1);
	
	wiringPiISR (CONV_BUTTON2, INT_EDGE_FALLING, &BTN_2_Interrupt) ;
	rbpButtonInit(CONV_BUTTON2,button2_event1);
	
	wiringPiISR (CONV_BUTTON3, INT_EDGE_FALLING, &BTN_3_Interrupt) ;
	rbpButtonInit(CONV_BUTTON3,button3_event1);
	
	wiringPiISR (CONV_BUTTON4, INT_EDGE_FALLING, &BTN_4_Interrupt) ;
	rbpButtonInit(CONV_BUTTON4,button4_event1);

	return 0;
}

void cbRailForward()
{
	delay(50);	
	if(digitalRead(ELEV_SEN_RAIL_R) == 1){
		printf("cbRailForward\n");
		fflush (stdout);	
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR1 , 0 );
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR2 , 0 );
	}else if(digitalRead(ELEV_SENSOR1) == 1){
		printf("cbRailForward Motor1\n");
		fflush (stdout);	
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR1 , 0 );
	}else if(digitalRead(ELEV_SENSOR2) == 1){
		printf("cbRailForward Motor2\n");
		fflush (stdout);	
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR2 , 0 );
	}
}

void cbRailBackward()
{
	delay(50);
	if(digitalRead(ELEV_SEN_RAIL_F) == 1){
		printf("cbRailBackward\n");
		fflush (stdout);	
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR1 , 0 );
		set_PWM_OFF_PCA9685( i2c_pwm , ELEV_MOTOR2 , 0 );
	}
}

void cbElevatorUp()
{
	delay(50);
	if(digitalRead(ELEV_SEN_ELE_H) == 1){
		printf("cbElevatorUp\n");
		fflush (stdout);	
		pwmWrite (ELEV_MOTOR3_PWM, 0) ;
	}
}

void cbElevatorDown()
{
	delay(50);
	if(digitalRead(ELEV_SEN_ELE_L) == 1){
	printf("cbElevatorDown\n");
	fflush (stdout);	
		pwmWrite (ELEV_MOTOR3_PWM, 0) ;
	}
}

int init_Elevator()
{
	if ((i2c_pwm= wiringPiI2CSetup (0x40)) == -1 ) {
		printf("Initialize PCA9685 fail !!\n");
		return -1;
	}

	init_PCA9685( i2c_pwm, CONV_SPEED1 );
	wiringPiSetup () ;

	pinMode (ELEV_MOTOR3, PWM_OUTPUT) ;
	pwmSetClock (256) ;

	pinMode (ELEV_LED1, OUTPUT) ;
	pinMode (ELEV_LED2, OUTPUT) ;
	pinMode (ELEV_LED3, OUTPUT) ;
	pinMode (ELEV_MOTOR_DIR1, OUTPUT) ;
	pinMode (ELEV_MOTOR_DIR2, OUTPUT) ;
	pinMode (ELEV_MOTOR_DIR3, OUTPUT) ;

	pullUpDnControl (ELEV_BUTTON1, PUD_UP) ;
	pullUpDnControl (ELEV_BUTTON2, PUD_UP) ;
	pullUpDnControl (ELEV_SENSOR1, PUD_DOWN) ;
	pullUpDnControl (ELEV_SENSOR2, PUD_DOWN) ;
	pullUpDnControl (ELEV_SEN_RAIL_R, PUD_DOWN) ;
	pullUpDnControl (ELEV_SEN_RAIL_F, PUD_DOWN) ;
	pullUpDnControl (ELEV_SEN_ELE_H, PUD_DOWN) ;
	pullUpDnControl (ELEV_SEN_ELE_L, PUD_DOWN) ;
	
	ISR_HANDLER_R(ELEV_BUTTON1);
	ISR_HANDLER_F(ELEV_BUTTON2);
	ISR_HANDLER_R(ELEV_SENSOR1);
	ISR_HANDLER_R(ELEV_SENSOR2);
	//Wumin
//	wiringPiISR (ELEV_SEN_RAIL_R, INT_EDGE_RISING, &cbRailForward) ;
	wiringPiISR (ELEV_SENSOR1, INT_EDGE_RISING, &cbRailForward);
	wiringPiISR (ELEV_SENSOR2, INT_EDGE_RISING, &cbRailForward);
//	wiringPiISR (ELEV_SEN_RAIL_F, INT_EDGE_RISING, &cbRailBackward) ;
	wiringPiISR (ELEV_SEN_ELE_H, INT_EDGE_RISING, &cbElevatorUp) ;
	wiringPiISR (ELEV_SEN_ELE_L, INT_EDGE_RISING, &cbElevatorDown) ;
	
	mode = MODE_ELEV;
	
	return 0;
}

void rbpMotor(int nPin)
{
	static int convMotorStillFlag[4] = {0, 0, 0, 0};
	convMotor nPin2 = CONV_NONE;
	if(nPin == CONV_MOTOR1 && dir == FORWARD){
		nPin2 = CONV_MOTOR2;
		convMotorStillFlag[0]++;
		convMotorStillFlag[1]++; // motor 2's flag
	}
	
	if(nPin == CONV_MOTOR2 && dir == FORWARD){
		nPin2 = CONV_MOTOR3;
		convMotorStillFlag[1]++;
		convMotorStillFlag[2]++; 
	}
				
	if(nPin == CONV_MOTOR3 && dir == FORWARD){
		nPin2 = CONV_MOTOR4;
		convMotorStillFlag[2]++;
		convMotorStillFlag[3]++; 
	}
	
	if(nPin == CONV_MOTOR4 && dir == FORWARD){
		nPin2 = CONV_NONE;
		//convMotorStillFlag[2]++;
		convMotorStillFlag[3]++; 
	}	
	
	set_PWM_ON_PCA9685( i2c_pwm , nPin , 0x0 );
	set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0800 );
	
	delay(1000);
	set_PWM_OFF_PCA9685( i2c_pwm , nPin2 , 0x0800 );	
	delay(3050);
	
	int index = nPin;
	if(convMotorStillFlag[index] > 0){
		if(convMotorStillFlag[index]==1)
			set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0000 );	
		convMotorStillFlag[index]--;
	}
	int index2 = nPin2;
	if(convMotorStillFlag[index2] > 0){
		if(convMotorStillFlag[index2]==1)
			set_PWM_OFF_PCA9685( i2c_pwm , nPin2 , 0x0000 );
		convMotorStillFlag[index2]--;
	}

}

void rbpMotorDirection(int state)
{
	if(state == FORWARD){
		dir = FORWARD;
		set_HIGH_PCF8575( i2c_ext, PCF8575_GPIO_7 );
	}else{
		dir = BACKWARD;
		set_LOW_PCF8575( i2c_ext, PCF8575_GPIO_7 );
	}
}

void rbpMotorSpeed(int state)
{
	init_PCA9685( i2c_pwm, state );
	delay_time = (state + 1)*446;
}

void rbpElevator_Pallet(int nPin, int dir)
{
	if(nPin == ELEV_MOTOR1){
		if(dir == FORWARD){
			digitalWrite (ELEV_MOTOR_DIR1, FORWARD);
			if(digitalRead(ELEV_SEN_RAIL_R) == 1)
				return;
		}else
			digitalWrite (ELEV_MOTOR_DIR1, BACKWARD) ;
		
		set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0800 );
		delay(delay_time);
		set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0 );
	}

	if(nPin == ELEV_MOTOR2){
		if(dir == FORWARD){
			digitalWrite (ELEV_MOTOR_DIR2, FORWARD) ;
			if(digitalRead(ELEV_SEN_RAIL_R) == 1)
				return;
		}else
			digitalWrite (ELEV_MOTOR_DIR2, BACKWARD) ;
		
		set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0800 );
		delay(delay_time);
		set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0 );
	}
}

void rbpElevator_UpDown(int dir)
{
	if(dir == UP){
		digitalWrite (ELEV_MOTOR_DIR3, UP) ;
		if(digitalRead(ELEV_SEN_ELE_H) == 0){
			pwmWrite (ELEV_MOTOR3_PWM, 500) ;
		}
	}else{
		digitalWrite (ELEV_MOTOR_DIR3, DOWN) ;
		if(digitalRead(ELEV_SEN_ELE_L) == 0){
			pwmWrite (ELEV_MOTOR3_PWM, 500) ;
		}
	}
}

void rbpCylinder(int nPin, int state)
{
	if(state == ON)
		set_HIGH_PCF8575( i2c_ext, nPin );
	else
		set_LOW_PCF8575( i2c_ext, nPin );
}

void rbpLed(int nPin, int state)
{
	if(state == ON)
		set_HIGH_PCF8575( i2c_ext, nPin );
	else
		set_LOW_PCF8575( i2c_ext, nPin );
}

void rbpBuzzer(int state)
{
	if(mode == MODE_ELEV){
		digitalWrite (ELEV_BUZZER, state) ;
	}else{
		if(state == ON)
			set_HIGH_PCF8575( i2c_ext, CONV_BUZZER );
		else
			set_LOW_PCF8575( i2c_ext, CONV_BUZZER );
	}
}

/*
void rbpSensor2(int nPin, void (*function)(void))
{
	callbackFun[nPin] = function;
	wiringPiISR (nPin, INT_EDGE_RISING, (void *)isrHandler[nPin]) ;
}

void rbpButton(int nPin, void (*function)(void))
{
	callbackFun[nPin] = function;
	wiringPiISR (nPin, INT_EDGE_FALLING, (void *)isrHandler[nPin]) ;
}
*/

static void *rbpButtonHandler (void *arg)
{
	int myPin,state ;

	(void)piHiPri (55) ;	// Only effective if we run as root

	myPin   = pinPass ;
	pinPass = -1 ;
	state = digitalRead (myPin);
	
	for (;;)
	{
		delay(30);
		if( state == 1){
			if( digitalRead (myPin) == LOW)
			{
				state = 0;
				callbackFun[myPin]();
			}
		}else{
			delay(500);
			if( digitalRead (myPin) == HIGH)
			{
				state = 1;
			}
		}	
	}

	return NULL ;
}

void rbpButton(int nPin, void (*function)(void))
{
	pthread_t threadId ;

	callbackFun[nPin] = function;

	pthread_mutex_lock (&pinMutex) ;
	pinPass = nPin ;
	pthread_create (&threadId, NULL, rbpButtonHandler, NULL) ;
	while (pinPass != -1)
		delay (1) ;
	pthread_mutex_unlock (&pinMutex) ;
}

static void *rbpSensorHandler (void *arg)
{
	int myPin,state ;

	(void)piHiPri (55) ;	// Only effective if we run as root

	myPin   = pinPass ;
	pinPass = -1 ;
	state = digitalRead (myPin);
	
	for (;;)
	{
		delay(30);
		if( state == 1){
			delay(500);
			if( digitalRead (myPin) == LOW)
			{
				state = 0;
				printf("Wumin : digitalRead = LOW\n");
			}
		}else{
			if( digitalRead (myPin) == HIGH)
			{
				state = 1;
				callbackFun[myPin]();
				printf("Wumin : digitalRead = HIGH\n");
			}
		}	
	}

	return NULL ;
}

void rbpSensor(int nPin, void (*function)(void))
{
	pthread_t threadId ;

	callbackFun[nPin] = function;

	pthread_mutex_lock (&pinMutex) ;
	pinPass = nPin ;
	pthread_create (&threadId, NULL, rbpSensorHandler, NULL) ;
	while (pinPass != -1)
		delay (1) ;
	pthread_mutex_unlock (&pinMutex) ;
}

void rbpRFID(char* buff)
{
	int i,ch;

	while(serialDataAvail(fd))
	{
		serialGetchar (fd);
	}
	for (i=0; i<10; i++)
	{
		ch = serialGetchar(fd);
		if((ch < 48) || (ch > 57))
		{
			i--;
		}
		else
		{
			buff[i] = ch;
		}
	}
}

void ConveyorMotorMove(int nPin)
{
	//set_PWM_ON_PCA9685( i2c_pwm , nPin , 0x0 );
	set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0800 );
}

void ConveyorMotorStop(int nPin)
{
	set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0 );
}

void ElevatorMotorMove(int nPin, int ELEV_MOTOR_DIR, int dir)
{
	digitalWrite (ELEV_MOTOR_DIR, dir) ;		
	set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0800 );
}

void ElevatorMotorStop(int nPin)
{
	set_PWM_OFF_PCA9685( i2c_pwm , nPin , 0x0 );
}