# myVoice Serial Communicator
This repository contains the code for the myVoice Raspberry Pi Application. This application communicates with the Arduino, processes the data and saves it in files with a cosistent format. 

### Table of Contents
* [Getting Started](#getting-started)
* [Configuration](#configuration)
* [Code Overview](#code-overview)

# Getting Started
These instructions will get a copy of the project running on your Raspberry Pi. 

### Using pre-built binary
1. Ensure that you are on a Raspberry Pi system.
2. Download the binary from [this link](https://github.com/iankirsch/serial-communicator/releases/download/v1.0.0/serial-communicator).
3. Execute it by running `./serial-communicator`.

### Building from Source
1. Ensure that you are on a Raspberry Pi system.
2. Clone the repository to an empty folder on your system.
3. Navigate to newly created folder.
4. Run `./build`.
5. You can now execute the binary by running `./serial-communicator`.

# Configuration
The serial communicator software can be configured using the following command-line flags:

## `--device`
Allows the configuration of a specific port to use for communicating with the Arduino. 

**Example usage: `--device /dev/ttyS1`**  
**Default value: `/dev/ttyS0`**  

## `--rate`
Allows the configuration the baud rate used for communicating with the Arduino. 

**Example usage: `--rate 115200`**  
**Default value: `1000000`**  

## `--granularity`
Configures the "granularity" of the recording in milliseconds. The granularity specifies the amount of time between averaging all received data from the Arduino. 

For example, when the granularity is set to 200ms, all values received from the Arduino are averaged every 200ms. This setting allows us to ensure that all recordings have exactly the same line count when the same granularity and recording time are set. For example, with a recording time of 2000ms and granularity of 200ms this would result in a file with 10 lines.

**Example usage: `--granularity 200`**  
**Default value: `100`**  

## `--recordingTime`
Configures the length of each recording in milliseconds. 

**Example usage: `--recordingTime 1000`**  
**Default value: `2000`**  

## `--noLogo`
When specified, hides the Serial Communicator startup screen.

**Example usage: `--noLogo`**  
**Default value: unset**  

## `--verbose` | `-v`
Enables verbose mode. When enabled, more debug output is logged, including all received data from the Arduino.

**Example usage: `--verbose` or  `-v`**  
**Default value: unset**  


# Code Overview

#### Main functionality
* **`/main.cpp`** - Program Entry Point  
&nbsp;&nbsp;&nbsp;→ Handles the main program logic  
&nbsp;&nbsp;&nbsp;→ Connecting to the Arduino  
&nbsp;&nbsp;&nbsp;→ Processing the received data  
&nbsp;&nbsp;&nbsp;→ Saving processed data to disk  

#### Shared functionality
* **`/lib/common.cpp`** - Shared additional functionality  
&nbsp;&nbsp;&nbsp;→ Splitting a string into a vector  
&nbsp;&nbsp;&nbsp;→ Converting a string vector to an int vector  
&nbsp;&nbsp;&nbsp;→ Formatting received values    

* **`/lib/logger.cpp`** - Logging functionality    

#### WiringPi Library Headers
* **`/wiringPi/*.h`**   
&nbsp;&nbsp;&nbsp;→  Header files for wiringPi library  
&nbsp;&nbsp;&nbsp;→  Used for communicating to the Arduino over serial using UART.

###### Note: WiringPi library is included on the Raspbian OS, the header files are included in the repository for linting purposes, when cross-compiling from other systems.
