#include <stdio.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


int Raspberry_Pi2_PWM(void)
{
  printf ("Raspberry Pi wiringPi test program(2015-11-13(25)).\n") ;

  if (wiringPiSetupGpio() == -1)
    return -1;

  pinMode(18,PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS); 
  pwmSetClock(384); //clock at 50kHz (20us tick)
  pwmSetRange(1000); //range at 1000 ticks (20ms)
  //pwmWrite(18, 125);  //(180 degree)2.5 ms//theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
 // pwmWrite(18, 75);  //1.5 ms//theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
  pwmWrite(18, 70);  // 90 degree
 //pwmWrite(18, 50); //1 ms //theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
  //pwmWrite(18, 30); //0.6 ms //theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
 // pwmWrite(18, 30);  //0 degree
  //pwmWrite(18, 25); //0.5 ms 
  //pwmWrite(18, 20); //0.4 ms 
  //pwmWrite(18, 15); //0.3 ms 
  //pwmWrite(18, 10); //0.2 ms 

  //for (;;) delay (1000) ;
  return 0;
}
void Set_sleepPCA9685(int i2c_fd )
{
	unsigned char val;
	
	  val = wiringPiI2CReadReg8(i2c_fd , 0x0 );
	  val = val | 0x10; // set bit 4 for enabled sleep mode 
	  wiringPiI2CWriteReg8( i2c_fd , 0x00 ,val );
}	

void Unset_sleepPCA9685(int i2c_fd )
{
	unsigned char val;
	
		val = wiringPiI2CReadReg8(i2c_fd , 0x0 );
		val = val & 0xef; // set bit 4 for enabled sleep mode 
		wiringPiI2CWriteReg8( i2c_fd , 0x00 ,val );
}	

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
void stop_PWM_OFF_PCA9685(int i2c_fd, int ch)
{
		unsigned char val;
		int reg_low_byte = 0x08 + 4 * ch;
		
		val = wiringPiI2CReadReg8(i2c_fd , reg_low_byte+1 );
		val = val | 0x10 ;
		wiringPiI2CWriteReg8( i2c_fd , reg_low_byte+1 ,val );
}	

void Reset_PCA9685(int i2c_fd )
{
	unsigned char val;
	
		val = wiringPiI2CReadReg8(i2c_fd , 0x0 );
		val = val | 0x80; // set bit 4 for enabled sleep mode 
		wiringPiI2CWriteReg8( i2c_fd , 0x00 ,val );
}	


int PCA9685_PWM(void)
{
	int val ;
	int i2c_fd;
	int  number;
	int test_ch = 0;
	
	
	if ((i2c_fd= wiringPiI2CSetup (0x40)) == -1 ) {
	  printf("\r\n Initialize PCA9685 fail !!\r\n");
	  return ;
	}else
		printf("\r\n Initialize PCA9685 success !!\r\n");	

  Reset_PCA9685(i2c_fd );
  delay(500);
	Set_sleepPCA9685( i2c_fd );
	wiringPiI2CWriteReg8( i2c_fd , 0xfe ,6 ); // 1000 hz
	//wiringPiI2CWriteReg8( i2c_fd , 0xfe ,3 ); // 2000 hz
	//wiringPiI2CWriteReg8( i2c_fd , 0xfe ,1 ); // 2000 hz
	Unset_sleepPCA9685( i2c_fd );
	set_PWM_ON_PCA9685( i2c_fd , test_ch , 0x0 );
	
do {
	printf("\r\n 1.180 degree");
	printf("\r\n 2.90 degree");
	printf("\r\n 3.0 degree");
	printf("\r\n 4. exit");
	printf("\r\nType in a select \n");
	
  scanf("%d", &number);
  switch ( number ) {
  	case 1:
  	     printf("\r\n <180 degree>");
  			// wiringPiI2CWriteReg8( i2c_fd , 0x08 ,0x00 );
			  // wiringPiI2CWriteReg8( i2c_fd , 0x09 ,0x02 );
			  //set_PWM_OFF_PCA9685( i2c_fd , test_ch , 0x200 );
			  set_PWM_OFF_PCA9685( i2c_fd , test_ch , 0x800 ); // 50% ON , 50% OFF
			  //delay(30);
			 // stop_PWM_OFF_PCA9685( i2c_fd , test_ch );
  	     break;
  	case 2:
  		    printf("\r\n <90 degree>");
  	     //wiringPiI2CWriteReg8( i2c_fd , 0x08 ,0x28 );
		     //wiringPiI2CWriteReg8( i2c_fd , 0x09 ,0x01 );
		     set_PWM_OFF_PCA9685( i2c_fd , test_ch , 0x128 );
  	     break;
  	case 3:
  		   printf("\r\n <0 degree>");
  		  // wiringPiI2CWriteReg8( i2c_fd , 0x08 ,0x75 );
			  // wiringPiI2CWriteReg8( i2c_fd , 0x09 ,0x00 );
			  set_PWM_OFF_PCA9685( i2c_fd , test_ch , 0x0075 );
  	     break;
  	case 4:
  	     Set_sleepPCA9685( i2c_fd );
  	     	//close( i2c_fd );
  	     	return ;
  	       
	    	     
	}
	
	
}while(1);
//	close( i2c_fd );
	return 0;
}	
void testInput( void )
{
	int i=0;
	printf ("Raspberry Pi blink\n") ;
  if ( wiringPiSetup ()  == -1 )
     printf("\r\n wiringPiSetup fail !!");
  else
      printf("\r\n wiringPiSetup success !!\r\n");
  pinMode (1, INPUT) ; 
  //pullUpDnControl (7, PUD_DOWN) ;
  
  for (;;)
  {
    
    delay (500) ;
    printf("\r\n i=%d , val_0=0x%x",++i,digitalRead(1));
    
  }
}
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

int set_PCF8575(void)
{
int i2c_fd;
	
	if ((i2c_fd= wiringPiI2CSetup (0x20)) == -1 ) {
	  printf("\r\n Initialize PCF8575 fail !!\r\n");
	  return -1;
	}else
		printf("\r\n Initialize PCF8575 success !!\r\n");	
		
	return i2c_fd;
}	

int test_PCF8575(void)
{
	int val ;
	int i2c_fd;
	int  number;
	int test_ch = 0;
	int temp = 0xfffe;
	
	
	
	if ((i2c_fd= wiringPiI2CSetup (0x20)) == -1 ) {
	  printf("\r\n Initialize PCF8575 fail !!\r\n");
	  return ;
	}else
		printf("\r\n Initialize PCF8575 success !!\r\n");	

  val = wiringPiI2CReadReg16( i2c_fd , 0x00 ) ;
  printf("\t\n init_1 val=0x%02x\r\n",val); 
  //val = val & 0xfffe ;
 // wiringPiI2CWriteReg8( i2c_fd , 0xfe, 0xff );
  
  printf("\t\n init_2 val=0x%02x\r\n",wiringPiI2CReadReg16( i2c_fd , 0x00 )); 
  //return ;
  while (1)
  {
#if 0
  	delay (100) ;
  	printf("\t\n val=0x%02x",wiringPiI2CReadReg16( i2c_fd , 0x00 )); 
#endif    
  	
#if 1  	
  printf("\t\n val=0x%02x",wiringPiI2CReadReg16( i2c_fd , 0x00 )); 
  wiringPiI2CWriteReg8( i2c_fd , 0xff, 0xff );
  printf("\t\n high");
  delay (200) ;
  wiringPiI2CWriteReg8( i2c_fd , 0xfd, 0xff );
  printf("\t\n low");
  delay (300) ;
#endif  
  }
}	

#if 1
int main (void)
{
int i2c_fd;

	//PCA9685_PWM();	
	testInput();
	//test_PCF8575();
#if 0	
	i2c_fd = set_PCF8575();
	
	do{
	set_LOW_PCF8575( i2c_fd, PCF8575_GPIO_0|PCF8575_GPIO_1|
													 PCF8575_GPIO_2|PCF8575_GPIO_3 );
  delay (500) ;												 
  set_HIGH_PCF8575( i2c_fd, PCF8575_GPIO_0|PCF8575_GPIO_2|
  													PCF8575_GPIO_3);													 
  delay (300) ;												 											
  set_HIGH_PCF8575( i2c_fd, PCF8575_GPIO_1 );	
 delay (200) ;
 }while(1);													
#endif	
}	
#endif

/*
int main (void)
{
  printf ("Raspberry Pi wiringPi test program(2015-11-13).\n") ;

  if (wiringPiSetupGpio() == -1)
    return ;

  pinMode(18,PWM_OUTPUT);
 // pwmSetMode(PWM_MODE_MS); 
  pwmSetClock(2); //clock at 50kHz (20us tick)
  pwmSetRange(10); //range at 1000 ticks (20ms)
  pwmWrite(18, 5);  //theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok

  for (;;) delay (1000) ;
}
*/

/*

#define LED 0 //wiringPi pin 1 is BCM_GPIO 18.

void all_leds_off() {
    digitalWrite (LED,0);
}

void all_leds_on() {
    digitalWrite (LED,1);
}
void main(void)
{
  printf ("Raspberry Pi blink\n") ;
  if ( wiringPiSetup ()  == -1 )
     printf("\r\n wiringPiSetup fail !!");
  else
      printf("\r\n wiringPiSetup success !!\r\n");
  pinMode (LED, OUTPUT) ; 
  
  for (;;)
  {
    all_leds_on();   // On
    delay (500) ;               // mS
    all_leds_off();    // Off
    delay (500) ;
  }
    
}

*/

