# TUINO 096

TUINO 096 is an Arduino ZERO/M0 PRO compatible board which has been developed for NBIoT applications. On the same form factor of the original Arduino board we have added a Quectel BG96 which brings NB1/CATM1/2G connectivity and GNSS functionality, delivering the best development platform for testing the new NBIoT networks.<br/>
<br/>
<img src="/docs/tuino_096.jpg"/>
<br/>
To add the BG96, since an additional serial and control pins where needed, we have removed the EDBG chip present in the original board design, this limits the debugging feature of the Tuino 096, but mantains full compatibility for all of the other functions. In specific the BG96 is connected to the following pins:
* BG96 POWER PIN      => PIN 12
* BG96 RESET PIN      => PIN 11
* BG96 INTERRUPT PIN  => PIN 38
* BG96 Serial         => EDBG Serial, which is accessible a 'Serial' in the Arudino Sketch 
<br/>
As you can see from the picture the Tuino 096 has 2 SMA connectors one for RF ( the one near the LEDs) and one for GNSS ( the one near the SIM card slot). A complete tracking solution has never been easier to develop.
<br>
<br/>

## INITIAL SETUP
A specific board manager file for the Tuino 096 is coming soon... In the meantime for those that have received a pre-series sample you can program it by selecting Arduino/Genuino M0 as board.

## DIFFERENCES WITH ARDUINO M0 
Apart from the BG96 on board, we have decided to use the legacy SPI on D10/D11/D12. And some pin port numbers are different from the original Arduino M0.<br>
Here a list of defines for the D0-13 pin numbering ( again this wil not be necessary as soon as we deliver the specific Tuino 096 board manager file )

```c
// You need to select Arduino M0 for this pin numbering
#define   D0   0
#define   D1   1
#define   D2   8
#define   D3   9
#define   D4   2
#define   D5   3
#define   D6   4
#define   D7   5
#define   D8   6
#define   D9   7
#define   D10  10
#define   D11  21
#define   D12  18
#define   D13  20

#define   A0  24
#define   A1  25
#define   A2  26
#define   A3  27
#define   A4  28
#define   A5  19


```

## EXAMPLE CODE
Here a first example of the code to control the BG96 via the USB port:


```c

#define BG96_RESET  11
#define BG96_PWR    12

#
void setup() {
 	
 	 // initialize digital pin LED_BUILTIN as an output.
 
   	SerialUSB.begin(9600);
   	SerialUSB.println("Starting..");

   	Serial.begin(115200);

    pinMode(BG96_RESET, OUTPUT);
    pinMode(BG96_PWR, OUTPUT);

 	  digitalWrite(BG96_RESET,HIGH);
    
    // 
  	digitalWrite(BG96_PWR,HIGH);
  	delay(500);
  	digitalWrite(BG96_PWR,LOW);
  	delay(1500);
  	digitalWrite(BG96_PWR,HIGH);
  
}

// the loop function runs over and over again forever
void loop() {
   byte ch;
   
 if (Serial.available()>0) 
  {
    SerialUSB.write(Serial.read());
  }

  if (SerialUSB.available()>0) 
  {
    ch = SerialUSB.read();
    Serial.write(ch);
  }
}

```

Find the complete script in the utilities directory.

## Library

