# TUINO 096

TUINO 096 is an Arduino ZERO/M0 PRO compatible board which has been developed for NBIoT applications. On the same form factor of the original Arduino board we have added a Quectel BG96 which brings NB1/CATM1/2G connectivity and GNSS functionality, delivering the best development platform for testing the new NBIoT networks.<br/>
<br/>
<img src="/docs/tuino_096.jpg"/>
<br/>
To add the BG96, since an additional serial and control pins where needed, we have removed the EDBG chip present in the original board design, this limits the debugging feature of the Tuino 096, but mantains full compatibility for all of the other functions. In specific the BG96 is connected to the following pins:
<br/>
* BG96 POWER PIN<br/>
Available on Pin 39 or as <b>PIN_BG96_PWRKEY</b>
* BG96 RESET PIN<br/>
Available on Pin 40 or as <b>PIN_BG96_RESET</b>
* BG96 INTERRUPT<br/> 
Available on Pin 41 or as <b>PIN_BG96_INT</b>
* BG96 Serial<br/>
Accessible a 'Serial2' in the Arudino Sketch<br/>

<br/>
As you can see from the picture the Tuino 096 has 2 SMA connectors one for RF ( the one near the LEDs) and one for GNSS ( the one near the SIM card slot). A complete tracking solution has never been easier to develop.
<br>
<br/>

## INITIAL SETUP
A specific board manager file is available here: <br/>
https://raw.githubusercontent.com/gimasi/TUINO_ZERO_96/master/arduino_ide/package_tuino0_index.json
<br/>
<br/>
Once you have installed it, you will find the Tuino 096 Board in the board list.

## SERIAL PORTS
On the Tuino 096 we have 3 serial port:
* USB<br/>
We have aliased the SerialUSB to Serial, so to mantain compatibility with all code that writes to 'Serial'<br/>

* Serial1 <br/>
This is the serial port avialable on D0/D1<br/>

* Serial2 <br/>
This is the serial port that is connected to the BG96<br/>

## DIFFERENCES WITH ARDUINO M0 
Apart from the BG96 on board, we have decided to use the legacy SPI on D10/D11/D12. And some pin port numbers are different from the original Arduino M0.<br>

## EXAMPLE CODE
Here a first example of the code to control the BG96 via the USB port:


```c
#
void setup() {
 	
 	 // initialize digital pin LED_BUILTIN as an output.
 
   	Serial.begin(9600);
   	Serial.println("Starting..");

   	Serial2.begin(115200);

    pinMode(PIN_BG96_RESET, OUTPUT);
    pinMode(PIN_BG96_PWRKEY, OUTPUT);

 	  digitalWrite(PIN_BG96_RESET,HIGH);
    
    // 
  	digitalWrite(PIN_BG96_PWRKEY,HIGH);
  	delay(500);
  	digitalWrite(PIN_BG96_PWRKEY,LOW);
  	delay(1500);
  	digitalWrite(PIN_BG96_PWRKEY,HIGH);
  
}

// the loop function runs over and over again forever
void loop() {
   byte ch;
   
 if (Serial2.available()>0) 
  {
    Serial.write(Serial.read());
  }

  if (Serial.available()>0) 
  {
    ch = Serial.read();
    Serial2.write(ch);
  }
}

```

Find the complete script <b>bg96_proxy</b> in the utilities directory, which verifies if the BG96 is actually on, since the power sequence can turn off the chip if on.

## BG96 LIBRARY

A initial library to connect and send data via UDP is present in the library directory.