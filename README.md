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

aXX

# Folders

Here are the folders in the repository and their functions

## Hardware

## Slave Unit Code

## Master Unit Code
