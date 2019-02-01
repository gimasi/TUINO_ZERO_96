void setup() {
  char c;
  char wait_for_string[]="OK";
  int matched_chars = 0;
  bool bg96_is_on;
  unsigned long start_time = millis();

  delay(5000);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(PIN_BG96_RESET, OUTPUT);
  pinMode(PIN_BG96_PWRKEY, OUTPUT);

  // Blink Led
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
 
  
  Serial.begin(9600);
  Serial.println("Starting..");

  Serial2.begin(115200);
  //Serial2.begin(9600);

  // LOW is disabled
  digitalWrite(PIN_BG96_RESET,LOW);

  // Let's check if BG96 is on
  Serial2.println("AT");

  matched_chars = 0;
  bg96_is_on = false;
  start_time = millis();
  
  while (millis() < (start_time + 2000)) 
  {
    if (Serial2.available()>0) 
    {
       c = Serial2.read();

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
    Serial.println("Resetting BG96");
    
    digitalWrite(PIN_BG96_RESET,LOW);
    delay(500);
    digitalWrite(PIN_BG96_RESET,HIGH);
    delay(500);
    digitalWrite(PIN_BG96_RESET,LOW);
    
  }
  else
  {
   
    // Power Up 
    Serial.println("Powering UP BG96");
    
    digitalWrite(PIN_BG96_PWRKEY,LOW);
    delay(500);
    digitalWrite(PIN_BG96_PWRKEY,HIGH);
    delay(1500);
    digitalWrite(PIN_BG96_PWRKEY,LOW);
  }
  
}

// the loop function runs over and over again forever
void loop() {
   byte ch;
   
 if (Serial2.available()>0) 
  {
    Serial.write(Serial2.read());
  }

  if (Serial.available()>0) 
  {
    ch = Serial.read();
    Serial2.write(ch);
  }
}

