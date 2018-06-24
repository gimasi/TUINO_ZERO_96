#include <Arduino.h>

#include "at_client.h"

Uart *ATSerial;

//
// Define this low level functions based on device
//

void serialEvent1(){
  while (ATSerial->available()) {
    
    // get the new byte:
    char inChar = (char)ATSerial->read();
   }
}

void at_serial_init(Uart *serial, uint32_t uart_speed)
{
  ATSerial = serial;
  
  ATSerial->begin(uart_speed);
}

bool _serial_available() 
{
  return ATSerial->available();
}

char _serial_read() 
{
  char c;
  
  c = ATSerial->read();
 //  SerialUSB.print(c);
  return c;
}

void _serial_write(char c) 
{
  ATSerial->write(c);
}


void _serial_print(const char *string) 
{
  int i;
  int len = strlen(string);

 SerialUSB.print("Sending ->");
  for (i=0;i<len;i++)
  {
    _serial_write(string[i]);
    delay(1);
    SerialUSB.write(string[i]);
  }
SerialUSB.println("<-");
}

void at_serial_clear_buffers()
{
  char c;
  while( _serial_available()) 
  {
    c=_serial_read();
  }
}

void at_send_char(char data)
{
  _serial_write(data);
}



void at_send_command(char *command ) 
{
  char cmd[256];  
  
  at_serial_clear_buffers();
  sprintf(cmd,"%s%s",command,AT_COMMAND_CR);  
  _serial_print(cmd);

}

uint8_t at_read_for_response_single( char* response_string, uint32_t timeout_ms) 
{
  
  // Index for how many characters have been matched.
  uint8_t matched_chars = 0;
  uint8_t response_length = strlen(response_string);
  char c;
  
  // Store the start time for detecting a timeout.
  unsigned long start_time = millis();
  /*
Serial.println(response_string);
Serial.println(response_length);
Serial.println("Read For Response ->");
  */
  // Loop until the timeout is reached.
  while (millis() < (start_time + timeout_ms)) 
  {
    // Only proceed if a character is available.
    if (_serial_available()) 
    {
      c = _serial_read();

      //Serial.print(c);
      
      // If the character matches the expected character in the response,
      // increment the pointer.  If not, reset things.
      if (c == response_string[matched_chars]) 
      {
      
        matched_chars++;
       if (matched_chars == response_length) 
        {
         // Serial.println("<- SUCCESS");
          return AT_COMMAND_SUCCESS;
        }
      } 
      else 
      {
        // Character did not match - reset.
        matched_chars = 0;
      }
    }
  }

  // Timeout reached with no match found.
  //Serial.println("<- TIMEOUT");
  return AT_COMMAND_TIMEOUT;  
}

// Same as above, but with two sets of counters/indexes/etc.
uint8_t read_for_responses_dual(char* pass_string, char* fail_string, uint32_t timeout_ms) 
{
  uint8_t pass_matched_chars = 0, fail_matched_characters = 0;
  uint8_t pass_response_length = strlen(pass_string);
  uint8_t fail_response_length = strlen(fail_string);

  // Store the start time for timeout purposes.
  unsigned long start_time = millis();
/*
Serial.println(fail_string);
Serial.println(fail_response_length);
Serial.println("Read For Response Dual ->");
*/

  // Loop until the timout is reached.
  while (millis() < (start_time + timeout_ms)) 
  {
    if (_serial_available()) 
    {
      char next_character = _serial_read();

//Serial.print(next_character);

      // Check and update the "pass" case.
      if (next_character == pass_string[pass_matched_chars]) 
      {
        pass_matched_chars++;
        
        if (pass_matched_chars == pass_response_length) 
        {
          //Serial.println("<- SUCCESS");
          return AT_COMMAND_SUCCESS;
        }
      } 
      else 
      {
        pass_matched_chars = 0;
      }

      // Check and update the "fail" case.
      if (next_character == fail_string[fail_matched_characters]) 
      {
        fail_matched_characters++;
        
        if (fail_matched_characters == fail_response_length) 
        {
         // Serial.println("<- FAILURE");
          return AT_COMMAND_FAILURE;
        }
      } 
      else 
      {
        fail_matched_characters = 0;
      }
    }
  }

//Serial.println("<- TIMEOUT");
  // Timeout reached - return timeout.
  return AT_COMMAND_TIMEOUT;  
}


uint8_t at_read_dual_and_copy_to_buffer(char *buffer,char* pass_string, char* fail_string, uint16_t max_bytes,  uint32_t timeout_ms) 
{

  unsigned long start_time = millis();
  uint16_t bytes_read = 0;
  uint8_t pass_matched_chars = 0, fail_matched_characters = 0;
  uint8_t pass_response_length = strlen(pass_string);
  uint8_t fail_response_length = strlen(fail_string);


  // Loop until timeout.
  while (millis() < (start_time + timeout_ms)) {
    if (_serial_available()) 
    {

       char next_character = _serial_read();
       
      buffer[bytes_read] = next_character;
      bytes_read++;

      if (bytes_read >= (max_bytes - 1)) {
        buffer[bytes_read] = 0;
        return AT_COMMAND_LENGTH_EXCEEDED;
      }


      // Check and update the "pass" case.
      if (next_character == pass_string[pass_matched_chars]) 
      {
        pass_matched_chars++;

        bytes_read--;
        buffer[bytes_read] = 0;
        
        if (pass_matched_chars == pass_response_length) 
        {
          buffer[bytes_read] = 0;
          return AT_COMMAND_SUCCESS;
        }
      } 
      else 
      {
        pass_matched_chars = 0;
      }
      
      // Check and update the "fail" case.
      if (next_character == fail_string[fail_matched_characters]) 
      {
        fail_matched_characters++;

        bytes_read--;
        buffer[bytes_read] = 0;

        if (fail_matched_characters == fail_response_length) 
        {
          buffer[bytes_read] = 0;
          return AT_COMMAND_FAILURE;
        }
      } 
      else 
      {
        fail_matched_characters = 0;
      }
              
    }
  }

  // Timeout reached - return timeout.
  return AT_COMMAND_TIMEOUT;  
}


uint8_t at_copy_serial_to_buffer(char *buffer, char read_until, uint16_t max_bytes,  uint32_t timeout_ms) 
{

  unsigned long start_time = millis();
  uint16_t bytes_read = 0;
//Serial.println( "COPY->");

  // Loop until timeout.
  while (millis() < (start_time + timeout_ms)) {
    if (_serial_available()) {
      buffer[bytes_read] = _serial_read();

//Serial.println( buffer );

      /**
       * Check to see if the character just read matches the read_until
       * character.  If so, stomp that character with a null terminating byte
       * and return success.
       */
      if (buffer[bytes_read] == read_until) {
        buffer[bytes_read] = 0;
//        Serial.println( "<- SUCCESS");
        return AT_COMMAND_SUCCESS;
      }

      /**
       * Increment bytes_read, and check to see if we're out of space.
       * 
       * If max_bytes is 4, offsets 0, 1, 2 can be used for characters, but
       * offset 3 must be saved for the null terminator.
       * 
       * If the length is exceeded, null terminate what has been read and return
       * the proper error.  Note that the remaining data in the serial buffer is
       * NOT read - that can be read by the calling code again, if they wish.
       */
      bytes_read++;

      if (bytes_read >= (max_bytes - 1)) {
        buffer[bytes_read] = 0;
        return AT_COMMAND_LENGTH_EXCEEDED;
      }
    }
  }

  //Serial.println( "<- TIMEOUT");

  // Timeout reached - return timeout.
  return AT_COMMAND_TIMEOUT;  
}



uint8_t at_read_until(char read_until, uint16_t timeout_ms) 
{
  unsigned long start_time = millis();

  while (millis() < (start_time + timeout_ms)) 
  {
    if (_serial_available()) 
    {
      // If the character matches the expected termination character, return.
      if (read_until == _serial_read() ) 
      {
        return AT_COMMAND_SUCCESS;
      }
    }
  }

  return AT_COMMAND_TIMEOUT;  
}

