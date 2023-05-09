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

//definitions

//global variables

void setup(){}
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

