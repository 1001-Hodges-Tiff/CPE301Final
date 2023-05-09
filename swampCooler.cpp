//This is the code for the arduino swamp cooler design
//Group Name: thodges
//Tiff Hodges


//include files for libraries
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <DHT.h>

//registers

//Timer 
volatile unsigned char *myTCCR1A  = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B  = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C  = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1  = (unsigned char *) 0x6F;
volatile unsigned char *myTIFR1   = (unsigned char *) 0x36;
volatile unsigned int  *myTCNT1   = (unsigned  int *) 0x84;

//ADC
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

//other
//PORTB corresponding to pins 8-15
volatile unsigned char *myPORT_B  = (unsigned char *) 0x25;
volatile unsigned char *myDDR_B  = (unsigned char *) 0x24;
volatile unsigned char *myPIN_B  = (unsigned char *) 0x23;

//PORTC corresponding to pins A8-A15, PC6 PC7
volatile unsigned char *myPORT_C  = (unsigned char *) 0x28;
volatile unsigned char *myDDR_C  = (unsigned char *) 0x27;
volatile unsigned char *myPIN_C  = (unsigned char *) 0x26;

//PORTD corresponding to pins 0-7
volatile unsigned char *myPORT_D  = (unsigned char *) 0x2B;
volatile unsigned char *myDDR_D  = (unsigned char *) 0x2A;
volatile unsigned char *myPIN_D  = (unsigned char *) 0x29;

//PORTE corresponding to pins 4-6
volatile unsigned char *myPORT_E  = (unsigned char *) 0x2E;
volatile unsigned char *myDDR_E  = (unsigned char *) 0x2D;
volatile unsigned char *myPIN_E  = (unsigned char *) 0x2C;

//PORTF corresponding to pins A0-A3
volatile unsigned char *myPORT_F  = (unsigned char *) 0x31;
volatile unsigned char *myDDR_F  = (unsigned char *) 0x30;
volatile unsigned char *myPIN_F  = (unsigned char *) 0x2F;

//PORTG corresponding to pins 2-6
volatile unsigned char *myPORT_G  = (unsigned char *) 0x34;
volatile unsigned char *myDDR_G  = (unsigned char *) 0x33;
volatile unsigned char *myPIN_G  = (unsigned char *) 0x32;

//PORTH corresponding to pins 7-14
volatile unsigned char *myPORT_H  = (unsigned char *) 0x102;
volatile unsigned char *myDDR_H  = (unsigned char *) 0x101;
volatile unsigned char *myPIN_H  = (unsigned char *) 0x100;

//PORTK corresponding to pins 62-69
volatile unsigned char *myPORT_K  = (unsigned char *) 0x25;
volatile unsigned char *myDDR_K  = (unsigned char *) 0x24;
volatile unsigned char *myPIN_K  = (unsigned char *) 0x23;

//PORTL corresponding to pins 48-52
volatile unsigned char *myPORT_L  = (unsigned char *) 0x10B;
volatile unsigned char *myDDR_L  = (unsigned char *) 0x10A;
volatile unsigned char *myPIN_L  = (unsigned char *) 0x109;

//Interuppt 
volatile unsigned char *myPCMSK1  = (unsigned char *) 0x6C;
volatile unsigned char *myPCICR  = (unsigned char *) 0x68;


//definitions

//global variables

void setup(){//initial setup 

	Serial.begin(9600); //initialize serial
	


}

void loop(){}
void monitorWater(){}
void currentState(){}
void idle(){}
void error(){}
void turnOff(){}
void timeAndDate(){}
void controlVent(){}
void airAndHumidity(){}
void monitorWater(){}
void fanMotor(){}
void clock(){}
void lightsOn(){}
void lightsOff(){}
void PIN_MODE(){}
void WRITE_PIN(){}
void adc_init(){}

