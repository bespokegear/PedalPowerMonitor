// Serial read data
//
// By Matt Little
// 24/7/13
//
// This code reads in data streamed to the serial port at 115200 baud.
// The data is parsed to get the Voltage, Current and Power.
// The data is then displayed upon the screen


// Original example by Tom Igoe
// Modified:
// 24/7/13  Matt Little  Changed baud rate
// 24/7/13  Matt Little  Added int to char conversion

import processing.serial.*;

// ****** Serial Data Read***********
// Variables for the serial data read
Serial myPort;  // The serial port
int inByte;         // incoming serial char
String str_buffer = "";  // This is the holder for the string which we will display

boolean errorFlag = false;  // This is a flag which is set if there is an error  
String deviceID = "";  // This holds the read-in substring
String deviceIDcheck = "AA";  // This holds the device ID we are looking for
String Str1 = "0";  // These hold the string values
String Str2= "0";
String Str3= "0";

int Int1 = 0;
int Int2 = 0;
int Int3 = 0;


void setup() {
  
  size(800, 360);  // Size of the display area
  background(0);   // Set background colour
  // Create the font
  textFont(createFont("Georgia", 36));
  noStroke();  
  // List all the available serial ports
  println(Serial.list());
  
  if(Serial.list().length >= 1)
  {
    // Open the port you are using at the rate you want:
    myPort = new Serial(this, Serial.list()[1], 115200);
  }
  else{
    println("Error - no serial connected");
    errorFlag=true;
  }
}

void draw() {
  if(errorFlag==false)
  {
    
    background(0);   // Blank the display
    
    //// Ask for data from the slave units
    //myPort.write("a");
    //myPort.write(deviceIDcheck);
    //myPort.write("P?------");
    //delay(50);
    //// First want to try and get any data which may appear on the serial port
    //getData();
    //// Now we want to display the data:
    //// First blank the display

    //text("Power 1: "+Str1, 50, 50); 
    //text("Power 2: "+Str2, 50, 100); 
    //text("Power 3: "+Str3, 50, 150); 
    
    //delay(100);
    
    // Ask for data from the slave units
    myPort.write("a");
    myPort.write(deviceIDcheck);
    myPort.write("V?-------");
    delay(100);
    // First want to try and get any data which may appear on the serial port
    getData();

   
    //text(currentStr, 50, 50);    
    //text(voltageStr, 50, 100);
    //text(powerStr, 50, 150); 
        
    Int1 = Integer.parseInt(Str1);
    Int2 = Integer.parseInt(Str2);
    Int3 = Integer.parseInt(Str3);
    

    
    // Draws a circle depeding upon the voltage
    
    // Scale the mouseX value from 0 to 640 to a range between 0 and 175
    float c = map(Int1, 0, width, 0, 255);
    // Scale the mouseX value from 0 to 640 to a range between 40 and 300
    float d = map(Int1, 0, width, 40, 1000);
    
    fill(Int1, Int2, Int3);
    ellipse(width/2, height/2, d, d);

    
    text("Voltage 1: "+Str1, 20, 50); 
    text("Voltage 2: "+Str2, 20, 100); 
    text("Voltage 3: "+Str3, 20, 150); 
    
    // Sloow everything down here:
    delay(200);  
    
  }
  else if(errorFlag==true)
  {  
    text("ERROR - No Serial Connection", 50, 50);
  }
}

// This subroutine checks for and gets any data on the serial port.
// We also send the data to the 'sortData' routine to strip out the useful data
void getData()
{
  if (myPort.available() > 0) 
  {
    inByte = myPort.read(); // Read whatever is happening on the serial port
    
    // str_buffer+=(char)inByte;
    // for(int i = 0; i<13;i++)  // Read in the next 11 chars - this is the data
    // {
    //   inByte = myPort.read(); 
    //   str_buffer+=(char)inByte;
    // }
    //println(str_buffer);  // TEST - print the str_buffer data (if it has arrived)     
    //// Here we clear the serial data ready for the next set
    //myPort.clear(); 
    
    if(inByte=='a')    // If we see an 'a' then read in the next 11 chars into a buffer.
    {   
     str_buffer+=(char)inByte;
     for(int i = 0; i<12;i++)  // Read in the next 11 chars - this is the data
     {
       inByte = myPort.read(); 
       str_buffer+=(char)inByte;
     }
      
     // Here we clear the serial data ready for the next set
     myPort.clear();
      
     println(str_buffer);  // TEST - print the str_buffer data (if it has arrived) 
     sortData();    // Sort the data - check if the strings match   
     str_buffer="";  // Reset the buffer to be filled again   
    }
  }
}



// **********************SORT DATA SUBROUTINE*****************************************
// This sub-routine takes the read-in data string (12 char, starting with a) and does what is required with it
// The str-buffer is global so we do not need to send it to the routine
void sortData()
{ 
  deviceID = str_buffer.substring(1,3);
  // We first want to check if the device ID matches.
  // If it does not then we disregard the command (as it was not meant for monitoring)     
  if(deviceID.equals(deviceIDcheck) == true)
  {
    // If yes then we can do further checks on ths data
    // This is where we do all of the checks on the incomming serial command:
    
    // Now we strip out the interesting data points.
    Str1 = str_buffer.substring(3,6);
    Str2 = str_buffer.substring(6,9);
    Str3 = str_buffer.substring(9,12);

  }
  else
  {
    text("Device ID does not match", 50, 200);
    println("Device ID does not match");  // We do not have matching device IDs)
  }
}