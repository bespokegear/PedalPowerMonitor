# PedalPowerMonitor

This is a modular Voltage/Current/Power/Energy monitor unit. Based upon the Arduino and Processing.
Please see www.re-innovation.co.uk for more details

# Overview

# Data Formats

Using a version of LLAP.
Commands and Returned Data is all in the format of a 12 Byte String.
They ALWAYS start with a lower case 'a'.

### ID

Each Slave unit has an ID which is two bytes which must be capitals.
For example "AA" or "AY". "aR" and "12" are INVALID.
In the examples below the ID of the Slave unit is "XX".
The ID of the Slave unit can be changed. It is up to the system installed to ensure that all the Slave IDs are unique.
The ID of the Slave units can be changed using the CHID command. 


### Commands and Returned Data

#### Change ID
aXXCHIDYY---
This changes the ID of slave XX to YY
It returns aYY---------

#### Get Voltages

aXXV?-------
This will return:
aXXVVVWWWXXX
Where VVV, WWW, XXX are the voltages from the three inputs in 100's of mV (ie 123 = 12.3V DC).

#### Get Currents

aXXI?-------
This will return:
aXXVVVWWWXXX
Where VVV, WWW, XXX are the currents from the three inputs in 100's of mA (ie 012 = 1.2A DC).

#### Get Powers

aXXP?-------
This will return:
aXXVVVWWWXXX
Where VVV, WWW, XXX are the Powers from the three inputs in Watts (ie 060 = 60W DC).

#### Get data from one bike

aXXB1?------
aXXB2?------
aXXB3?------
This will return (from bike inputs 1,2,3 respectively):
aXXVVVAAAWWW
Where VVV is the voltage in 100's mV (eg 456 = 45.6V)
AAA is the current in 100's mA (eg 103 = 10.3A)
WWW is the power in Watts (eg 012 = 12W).

# Folders

Here are the folders in the repository and their functions:

## Slave Hardware

The slave hardware is an Arduino Uno unit.
All 6 Analog pins have been used. These are:

A0 - Voltage 1
A1 - Current 1
A2 - Voltage 2
A3 - Current 2
A4 - Voltage 3
A5 - Current 3

Voltage is measured from a 560k - 10k potential divider (so can read up to around 285V (!). A 5.1V zener diode stops over voltage issues.

Current is measured using an ACS756 50A hall effect current sensor. More details about this are available here:

http://www.re-innovation.co.uk/web12/index.php/en/blog-2/361-acs756-current-measurements

Tx and Rx are connected to an XRF wireless radio unit from www.wirelesssthings.com.
This provides a radio based serial connection at 115200 baud (or whatever you set it at).
The radio conenction provides isolation to the monitoring master unit.

## Slave Software

The slave software is written using the Arduino IDE.
There may be some additional libraries to install to ensure it compiles correctly. Please see #includes in code.

## Master Hardware

At present the Master hardware is a computer (with Processing installed) and an SRF wireless radio unit from www.wirelessthings.com


## Master Software

This includes the software for the Master unit to read the slave units via a serial conenction.

The original software was written using Processing.

