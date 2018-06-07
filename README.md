# TUINO 096

TUINO 096 is an Arduino Zero/M0/M0 PRO compatible board which has been developed for NBIoT applications. On the same form factor of the Arduino Zero we have added a Quectel BG96 which brings NB1/CATM1 and 2G connectivity.
<br/>
<img src="/docs/tuino_096.jpg"/>
<br/>
We have removed the Arduino Zero EDBG chip we have added the BG96 which is been controlled by the serial and some of the ports which were used for the EDBG. In specifi we have:
* BG96 POWER PIN = PIN 12
* BG96 RESET PIN = PIN 11
* BG96 INTERRUPT PIN => PIN
<br/>
As you can see from the picture on the Tuino 096 we have 2 SMA connectors one for the RF part and one for the GPS antenna since the BG96 has also a GNSS module inside. So you can develop a complete tracning solution.
<br>
<br/>

## INITIAL SETUP
A specific board manager file for the Tuino 096 is coming soon... In the meantime for those that have received a pre-series sample you can program it by selecting Arduino/Genuino MO Pro as board.

## DIFFERENCES WITH M0/M0 PRO
Apart from the BG96 on board, we have decided to use the legacy SPI on D10/D11/D12.


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
