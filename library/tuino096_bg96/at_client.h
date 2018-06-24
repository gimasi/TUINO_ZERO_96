#ifndef _AT_CLIENT_H_
#define _AT_CLIENT_H_

#include <Arduino.h>

#define AT_COMMAND_CR "\r"

#define AT_COMMAND_RESPONSE_TIMEOUT 1000

#define AT_COMMAND_SUCCESS 0
#define AT_COMMAND_FAILURE 1
#define AT_COMMAND_TIMEOUT 2
#define AT_COMMAND_LENGTH_EXCEEDED 3

void at_serial_init(Uart *serial, uint32_t uart_speed);
void at_send_command(char *command );
uint8_t at_read_for_response_single( char* response_string, uint32_t timeout_ms);
uint8_t at_copy_serial_to_buffer(char *buffer, char read_until, uint16_t max_bytes,  uint32_t timeout_ms); 
uint8_t at_read_dual_and_copy_to_buffer(char *buffer,char* pass_string, char* fail_string, uint16_t max_bytes,  uint32_t timeout_ms); 
uint8_t read_for_responses_dual(char* pass_string, char* fail_string, uint32_t timeout_ms);

void at_serial_clear_buffers();
void at_send_char(char data);
void at_set_echo( bool on_off );


#endif 
