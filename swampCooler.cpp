//This is the code for the arduino swamp cooler design
//Group Name: thodges
//Tiff Hodges


//include files for libraries
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <RTClib.h>

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
#define temperature 20
#define water 50
#define ON true
#define OFF false
#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13
#define red 1
#define blue 2
#define green 3
#define yellow 4
#define start true

//global variables

float temp = 0;
float water = 0;
float humid = 0;
int ledstate = 0;
int waterHeight = 0;
int steps = 2038;
int ventAngle = 0;


int count = 0;
int state = 1; //each of the four states correspond to a number

LiquidCrystal lcd(d7, d6, d5, d4, e, r, rs, vo, vdd, vss);
Stepper stepper(steps, 10, 11, 12, 13);

void setup(){//initial setup 

	Serial.begin(9600); //initialize serial

	myDDR_B |= (3 << PB5);//set as output pin
	myDDR_B &= ~(1 << PB5);//set as input pin
	myDDR_C |= (1 << PC5);//set as output pin
	myDDR_D |= (7 << PD5);//set as output pin
	myDDR_G |= (1 << PG5);//set as output pin
	myDDR_H |= (4 << PH5);//set as output pin
	
	//setting up the clock
	clock();
	
	//setting up the vent
	controlVent();
	
	//setting up the adc
	adc_init();
	
	//setting up the dht

	stepper.setSpeed(25);
}

void loop(){
	
	//run the function to determine if the system is on or off
	ISR();
	
	//reset the lcd screen so that new information can be displayed
	lcd.clear();
	lcd.write("Swamp Cooler");
	
	//run the stepper function for the vent control
	stepper.step(steps);
	delay(1000);
	

}

//function to check water level and its current attributes. prints error message when out of threshold
void monitorWater(){
	
	//can only check the water level if the sensor is turned on
	pinFunctions(PORT_B, A0, ON);
	waterHeight = adc_read(); //read the value from the water sensor
	//must turn the water sensor back off
	pinFunctions(PORT_B, A0, ON);
	
	//print the water levels to the user
	Serial.print("Water Level: ");
	Serial.print(waterHeight);
	return waterHeight;
	
	//must check the state of the water because if it does not meet the threshold an alert pops up
	if(waterHeight < 50){
		error();
	}
}

void timeAndDate(){//prints all of the coresponding time and date when the motor is on or off
	//call the clock function within to print the date and time
	//must check the state of the program. can only print when the state is running
	if(state == 1){
		Serial.print("On");
		clock();
	}
	else{
		Serial.print("Off");
		clock();
	}
	
}
void controlVent(){//allows the user to control the angle of the vent
	int potValue = adc_read(A0);
	ventAngle = map(potValue, 0, 200, 500, 1000);
	delay(500); //add a delay to ensure this part functions smoother
	
	//print the results to the user
	Serial.print("Angle of the vent: ");
	Serial.print(ventAngle);
}
void airAndHumidity(){//monitors the air temp and humidity and displays that on the lcd screen
	//can only check the temp and humidity level if the sensor is turned on
	pinFunctions(PORT_B, 6, ON);//pin for DHT11
	temp = adc_read();//read the value from the sensor
	humid = adc_read();//read the value from the sensor
	
	//this information must be displayed on the lcd screen
	lcd.print("Temp and Humidity");
	lcd.print(temp);
	lcd.print("//");
	lcd.print(humid);
	
	return temp, humid;

}
void fanMotor(){//starts or stops the fan motor in terms of when a threshold is met or not met
	if(temp < 20 || temp > 70){
		pinFunctions(PORT_B, 21, OFF);//pin for fan so that it turns it off if it goes out of threshold
	}
	else{
		pinFunctions(PORT_B, 21, ON);//allows the fan to be on because it meets the criteria
	}

}
void clock(){//basic clock function that records date and time
	rtc.set(0, 42, 16, 6, 2, 5, 15);
	rtc.refresh(); //update the clock
	
	Serial.print("Date: ");
	Serial.print(rtc.month());
	Serial.print("-");
	Serial.print(rtc.day());
	Serial.print("-");
	Serial.print(rtc.year());
	
	Serial.print("Time: ");
	Serial.print(rtc.hour());
	Serial.print(":");
	Serial.print(rtc.minute());
	Serial.print(":");
	Serial.print(rtc.second());
}

void lightsOn(char ledColor){//determines what lights turn on 
	switch(ledColor){
		case 'blue':
			Serial.print("blue is on");
			//need to turn the led on
			pinFunctions(myPORT_B, 1, ON);
			break;
		case 'red':
			Serial.print("red is on");
			//need to turn the led on
			pinFunctions(myPORT_B, 2, ON);
			break;
		case 'yellow':
			Serial.print("yellow is on");
			//need to turn the led on
			pinFunctions(myPORT_B, 3, ON);
			break;
		case 'green':	
			Serial.print("green is on");
			//need to turn the led on
			pinFunctions(myPORT_B, 1, ON);
			break;
			
			
	}

}
void lightsOff(volatile unsigned char* port, unsigned char pin){//shuts off all of the lights
	if(ledstate == 1){//on
		*port |= 0x01 << pin; //shift pin 
	}
	else{//off
		*port &= ~(0x01 << pin); //shift pin 
	}
}

void pinFunctions(volatile unsigned char* port, unsigned char pin, bool function){//basic functions for the pins that converts them to high or low states
	if(state == 1){//on
		*port |= 0x01 << pin; //shift pin 
	}
	else{//off
		*port &= ~(0x01 << pin); //shift pin 
	}
}

//functions for the states 
void running(){
	 state = 1;
	
}
void idle(){
	 state = 2;
	
	
	
}
void error(){
	 state = 3;
	
	if(water != 50){//print an error message if the water is too low
		Serial.Print(Water is too low!!);
		lcd.print(Water is too low!!);
	}
	
	
}
void disable(){
	 state = 0;
	
	//run the function to turn off lights so we can then turn on correct one
	lightsOff();
	
	//turn on yellow led
	lightsOn("yellow");
	
	//must turn motor offso we call fan function that deals with fan operations
	fanMotor(OFF);
	
}

ISR(INT4){//using an ISR function to handle disabling and enabling the system
	if(INT4 == 1){
		PORT_D |= (1 << 8);//set the pin HIGH so that the system starts
	}
	else{
		PORT_D &= ~(1 << 8); //set the pin LOW so that the system shuts off
	}
}

void adc_init(){
	//setup the A register
  	*my_ADCSRA |= 0b10000000; // set bit 7 to 1 to enable the ADC
  	*my_ADCSRA &= 0b11011111; // clear bit 6 to 0 to disable the ADC trigger mode
  	*my_ADCSRA &= 0b11110111; // clear bit 5 to 0 to disable the ADC interrupt
  	*my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading
  	// setup the B register
  	*my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits
  	*my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode
  	// setup the MUX Register
  	*my_ADMUX  &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference
  	*my_ADMUX  |= 0b01000000; // set bit   6 to 1 for AVCC analog reference
  	*my_ADMUX  &= 0b11011111; // clear bit 5 to 0 for right adjust result
  	*my_ADMUX  &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits
}

unsigned int adc_read(unsigned char adc_channel_num){
  
	//clear the channel selection bits (MUX 4:0)
  	*my_ADMUX  &= 0b11100000;
  	//clear the channel selection bits (MUX 5)
  	*my_ADCSRB &= 0b11110111;
  	//set the channel number
  	if(adc_channel_num > 7){
    		//set the channel selection bits, but remove the most significant bit (bit 3)
    		adc_channel_num -= 8;
    		// set MUX bit 5
    		*my_ADCSRB |= 0b00001000;
  	}
  	//set the channel selection bits
  	*my_ADMUX  += adc_channel_num;
  	//set bit 6 of ADCSRA to 1 to start a conversion
  	*my_ADCSRA |= 0x40;
  	//wait for the conversion to complete
  	while((*my_ADCSRA & 0x40) != 0);
  		//return the result in the ADC data register
  		return *my_ADC_DATA;
	}

