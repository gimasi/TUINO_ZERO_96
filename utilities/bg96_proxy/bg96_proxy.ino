

#define BG96_RESET  11
#define BG96_PWR    12

void setup() {
  char c;
  char wait_for_string[]="OK";
  int matched_chars = 0;
  bool bg96_is_on;
  unsigned long start_time = millis();

  delay(5000);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BG96_RESET, OUTPUT);
  pinMode(BG96_PWR, OUTPUT);

  // Blink Led
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
 
  
  SerialUSB.begin(9600);
  SerialUSB.println("Starting..");

  Serial.begin(115200);

  // LOW is disabled
  digitalWrite(BG96_RESET,LOW);

  // Let's check if BG96 is on
  Serial.println("AT");

  matched_chars = 0;
  bg96_is_on = false;
  start_time = millis();
  
  while (millis() < (start_time + 2000)) 
  {
    if (Serial.available()>0) 
    {
       c = Serial.read();

       if (c == wait_for_string[matched_chars]) 
       {
         matched_chars++;
        if (matched_chars == 2) 
        {
          bg96_is_on = true;
          break;
        }
      } 
      else 
        matched_chars = 0;
   
    }
  }

  if ( bg96_is_on  )
  {
    // Reset 
    SerialUSB.println("Resetting BG96");
    
    digitalWrite(BG96_RESET,LOW);
    delay(500);
    digitalWrite(BG96_RESET,HIGH);
    delay(500);
    digitalWrite(BG96_RESET,LOW);
    
  }
  else
  {
   
    // Power Up 
    SerialUSB.println("Powering UP BG96");
    
    digitalWrite(BG96_PWR,LOW);
    delay(500);
    digitalWrite(BG96_PWR,HIGH);
    delay(1500);
    digitalWrite(BG96_PWR,LOW);
  }
  
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

