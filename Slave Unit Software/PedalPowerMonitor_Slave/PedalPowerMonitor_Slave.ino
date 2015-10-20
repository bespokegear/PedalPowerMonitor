/*

  Pedal Power Monitoring Unit
  SLAVE UNIT
  This is design to lsiten to serial commands at 115200 baud.
  When received it will read curret/voltage from 3 inputs.
  It will the calculate the exact Current and Voltage and Power.

  The unit will then send back the data in the following format:
  COMMANDS:
  Where XX is the device ID
  aXXCHIDYY---
  This will change the device ID from XX to YY

  aXXV?-------
  This will return the voltages as:
  aXXVVVVVVVVV
  Where the first 3 V are the voltages from inputs 1 2 3
  Voltages will always be in the format VVV = VV.V volts

  aXXI?-------
  This will return the currents as:
  aXXIIIIIIIII
  Where the first 3 I are the currents from inputs 1 2 3
  Currents will always be in the format III = II.I amps

  aXXP?-------
  This will return the powers as:
  aXXPPPPPPPPP
  Where the first 3 P are the powers from inputs 1 2 3 
  Powers will always be in the format PPP = PPP Watts 

  aXXB1?-------
  This will return the data from Bike 1
  aXXVVVIIIPPP

  aXXB2?-------
  This will return the data from Bike 2
  aXXVVVIIIPPP

  aXXB3?-------
  This will return the data from Bike 3
  aXXVVVIIIPPP
  

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Matt Little
 matt@re-innovation.co.uk
 www.re-innovation.co.uk

 This example code is in the public domain.

 */

/************ External Libraries*****************************/

#include <stdlib.h>
#include <avr/pgmspace.h>  // Library for putting data into program memory
#include <EEPROM.h>        // For writing values to the EEPROM
#include <avr/eeprom.h>    // For writing values to EEPROM


// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin0 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin1 = A1;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = A2;  // Analog input pin that the potentiometer is attached to
const int analogInPin3 = A3;  // Analog input pin that the potentiometer is attached to
const int analogInPin4 = A4;  // Analog input pin that the potentiometer is attached to
const int analogInPin5 = A5;  // Analog input pin that the potentiometer is attached to

const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot

float voltage1 = 0;
float voltage2 = 0;
float voltage3 = 0;

float current1 = 0;
float current2 = 0;
float current3 = 0;

float power1 = 0;
float power2 = 0;
float power3 = 0;

int outputValue = 0;        // value output to the PWM (analog out)

// ****** Serial Data Read***********
// Variables for the serial data read
char inByte;         // incoming serial char
String str_buffer = "";  // This is the holder for the string which we will display
#define MAX_STRING 80      // Sets the maximum length of string probably could be lower
char stringBuffer[MAX_STRING];  // A buffer to hold the string when pulled from program memory

char deviceID[3]; // A buffer to hold the device ID

// Varibales for writing to EEPROM
int hiByte;      // These are used to store longer variables into EERPRPROM
int loByte;
long int number;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);

  // Read in the values from EEPROM
  // Read the device ID
  deviceID[0] = char(EEPROM.read(0));
  deviceID[1] = char(EEPROM.read(1)); 
  Serial.print("Device ID is: ");
  Serial.print(deviceID[0]);
  Serial.println(deviceID[1]);
  
}

void loop() {
  // read the analog in value:
  voltage1 = analogRead(analogInPin0);
  voltage2 = analogRead(analogInPin2);
  voltage3 = analogRead(analogInPin4);

  current1 = analogRead(analogInPin1);
  current2 = analogRead(analogInPin3);
  current3 = analogRead(analogInPin5); 
        
  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 285);
  voltage1 = ((voltage1 * 5.0)*57.0) / 1023.0; //calculates voltage which comes through a potential divider.
  voltage2 = ((voltage2 * 5.0)*57.0) / 1023.0;
  voltage3 = ((voltage3 * 5.0)*57.0) / 1023.0;

  current1 = ((((current1 * 5000.0)) / 1023.0)-600.0)/(60.0/1000.0); //calculates current from hall sensors
  current2 = ((((current2 * 5000.0)) / 1023.0)-600.0)/(60.0/1000.0);
  current3 = ((((current3 * 5000.0)) / 1023.0)-600.0)/(60.0/1000.0);

  power1 = (voltage1)*(current1/1000);
  if(power1<0)
  {
    power1=0;
  }
  power2 = (voltage2)*(current2/1000);
  if(power2<0)
  {
    power2=0;
  }  
  power3 = (voltage3)*(current3/1000);
  if(power3<0)
  {
    power3=0;
  }
  
  // print the results to the serial monitor:
//  Serial.print("Voltage 1 = ");
//  Serial.println(voltage1);
//  Serial.print("Current 1 = ");
//  Serial.println(current1); 
//  Serial.print("Voltage 2 = ");
//  Serial.println(voltage2);
//  Serial.print("Current 2 = ");
//  Serial.println(current2);
//  Serial.print("Power 2 = ");
//  Serial.println(power2);
//  Serial.print("Voltage 3 = ");
//  Serial.println(voltage3);
//  Serial.print("Current 3 = ");
//  Serial.println(current3);

//  Serial.print("aAAV");
//  Serial.print(outputValue);
//  Serial.println("0000-----");
  
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:

  getData();
  delay(50);
  
}

// **********************GET DATA SUBROUTINE*****************************************
// This sub-routine picks up and serial string sent to the device and sorts out a power string if there is one
// All values are global, hence nothing is sent/returned

void getData()
{
  // **********GET DATA*******************************************
  // We want to find the bit of interesting data in the serial data stream
  // As mentioned above, we are using LLAP for the data.
  // All the data arrives as serial commands via the serial interface.
  // All data is in format aXXDDDDDDDDD where XX is the device ID
  if (Serial.available() > 0) 
  {
    inByte = Serial.read(); // Read whatever is happening on the serial port
  
    if(inByte=='a')    // If we see an 'a' then read in the next 11 chars into a buffer.
    {   
      str_buffer+=inByte;
      for(int i = 0; i<11;i++)  // Read in the next 11 chars - this is the data
      {
        inByte = Serial.read(); 
        str_buffer+=inByte;
      }
      //Serial.println(str_buffer);  // TEST - print the str_buffer data (if it has arrived)
      sortData();
      str_buffer="";  // Reset the buffer to be filled again
    }
  }
}

// **********************SORT DATA SUBROUTINE*****************************************
// This sub-routine takes the read-in data string (12 char, starting with a) and does what is required with it
// The str-buffer is global so we do not need to send it to the routine

void sortData()
{ 
  // We first want to check if the device ID matches.
  // If it does not then we disregard the command (as it was not meant for this device      
  if(str_buffer.substring(1,3) == deviceID)
  {
    // If yes then we can do further checks on ths data
    // This is where we do all of the checks on the incomming serial command:
    
    Serial.println("OK");  // TEST - got into this routine
    
    // Change device ID:
    // Device ID
    // “aXXCHIDXXE--“
    // Where the last two values (XX) are the new device ID (from AA to ZZ).
    if(str_buffer.substring(3,7) == "CHID")
    {
      // First check if the NEW device ID is within the allowable range (AA-ZZ)
      // to do this we can convert to an int and check if it is within the OK levels
      // A -> int is 65, Z -. int is 90
      // So our new device ID as an int must be between 65 and 90 for it to be valid
      if(65<=int(str_buffer[7])&&int(str_buffer[7])<=90&&65<=int(str_buffer[8])&&int(str_buffer[8])<=90)   
      { // If is all OK then write the data
        // Change device ID
        Serial.print("Change Device ID to ");
        Serial.println(str_buffer.substring(7,9));  // This will change the device ID
        deviceID[0] = str_buffer[7];
        deviceID[1] = str_buffer[8];
        // Also want to store this into EEPROM
        EEPROM.write(0, deviceID[0]);    // Do this seperately
        EEPROM.write(1, deviceID[1]);
      }
      else
      {
        //Serial.println("Invalid device ID"); 
     }      
    } 
    
     // Ask voltage set-point
    //  To ask the set-point we can send:
    //  “aXXV?-----“ 
    else if(str_buffer.substring(3,5) == "V?")
    {
      // Show voltage
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertV(voltage1));
      Serial.print(convertV(voltage2));
      Serial.println(convertV(voltage3));
    } 

     // Ask for currents
    //  To ask the set-point we can send:
    //  “aXXI?-----“ 
    else if(str_buffer.substring(3,5) == "I?")
    {
      // Show current
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertI(current1));
      Serial.print(convertI(current2));
      Serial.println(convertI(current3));
    } 

         // Ask for Powers
    //  To ask the set-point we can send:
    //  “aXXP?-----“ 
    else if(str_buffer.substring(3,5) == "P?")
    {
      // Show voltage
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertP(power1));
      Serial.print(convertP(power2));
      Serial.println(convertP(power3));
    } 

         // Ask for Powers
    //  To ask the set-point we can send:
    //  “aXXB1?-----“ 
    else if(str_buffer.substring(3,6) == "B1?")
    {
      // Show voltage
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertV(voltage1));
      Serial.print(convertI(current1));
      Serial.println(convertP(power1));
    } 

         // Ask for Powers
    //  To ask the set-point we can send:
    //  “aXXB2?-----“ 
    else if(str_buffer.substring(3,6) == "B2?")
    {
      // Show voltage
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertV(voltage2));
      Serial.print(convertI(current2));
      Serial.println(convertP(power2));
    } 
         // Ask for Powers
    //  To ask the set-point we can send:
    //  “aXXB3?-----“ 
    else if(str_buffer.substring(3,6) == "B3?")
    {
      // Show voltage
      Serial.print("a");
      Serial.print(deviceID);
      Serial.print(convertV(voltage3));
      Serial.print(convertI(current3));
      Serial.println(convertP(power3));
    } 
    
  }
  else
  {
    //Serial.println("Not Device ID");
  }
}


// Convert the voltage floats into ints that are always the same length
String convertV(float inputData)
{
    String outputString;
    inputData = inputData*10; // Converts voltage into 100's of mV.
    
    if(inputData<10)
    {
       outputString += "00";
       outputString += String((int)inputData);     
    }
    else if(inputData<100)
    {
       outputString += "0";
       outputString += String((int)inputData);    
    } 
    else
    {
      outputString += String((int)inputData);
    }
    return(outputString);
}

// Convert the power floats into ints that are always the same length
String convertI(float inputData)
{
    String outputString;
    if(inputData<0.000)
    {
      inputData=0.0; // Gets rid of negative numbers
    }
    
    inputData = inputData/100.0; // Converts into 100's of mA
    
    if(inputData<10)
    {
       outputString += "00";
       outputString += String((int)inputData);     
    }
    else if(inputData<100)
    {
       outputString += "0";
       outputString += String((int)inputData);    
    } 
    else
    {
      outputString += String((int)inputData);
    }
    return(outputString);
}
// Convert the voltage floats into ints that are always the same length
String convertP(float inputData)
{
    String outputString;
    
    if(inputData<10)
    {
       outputString += "00";
       outputString += String((int)inputData);     
    }
    else if(inputData<100)
    {
       outputString += "0";
       outputString += String((int)inputData);    
    } 
    else
    {
      outputString += String((int)inputData);
    }
    return(outputString);
}



