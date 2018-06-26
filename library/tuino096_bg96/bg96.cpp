
#include "tuino096.h"
#include "at_client.h"
#include "bg96.h"

#define DEBUG_BG96 1

void _log(String data )
{
  #ifdef DEBUG_BG96
    Serial.println(data);
  #endif
}

void _BG96Reset()
{
 
  // Reset 
  digitalWrite(PIN_BG96_RESET,LOW);
  delay(200);
  digitalWrite(PIN_BG96_RESET,HIGH);
  delay(500);
  digitalWrite(PIN_BG96_RESET,LOW);
}

void _BG96PowerCycle()
{

  // 
  digitalWrite(PIN_BG96_PWRKEY,LOW);
  delay(500);
  digitalWrite(PIN_BG96_PWRKEY,HIGH);
  delay(500);
  digitalWrite(PIN_BG96_PWRKEY,LOW);
}


void _GPIOInit()
{
 
    pinMode(BG96_RESET,OUTPUT);
    pinMode(PIN_BG96_PWRKEY,OUTPUT);
    
    digitalWrite(BG96_RESET,LOW);
    digitalWrite(PIN_BG96_PWRKEY,LOW);   
}


void _strip_all_newlines(char *data )
{
  
  String _temp= String(data);

  _temp.replace("\n","");
  _temp.replace("\r","");
  
  _temp.toCharArray(data,_temp.length()+1);

}

/*
 *  Public Functions
 */


uint8_t BG96_init() 
{

    // Init AT Interface
    at_serial_init(&Serial2, 115200);

   _log("BG96 Init");
   
    // Check if module is ON
    at_send_command("AT");
    if ( at_read_for_response_single("OK",BG96_DEFAULT_TIMEOUT) == 0 )
    {
      // it Is ON
      _log("BG96 Is ON");
      _BG96PowerCycle();
      delay(1000);
    }
 
    _BG96PowerCycle();
    delay(500);
   
    _BG96Reset();
    if ( at_read_for_response_single("RDY",BG96_BOOT_TIMEOUT) != 0 )
    {
      return BG96_KO;
    }

    _log("BG96 Boot OK !!!");

    at_send_command("ATE0");

    if ( at_read_for_response_single("OK",BG96_DEFAULT_TIMEOUT) != 0 )
    {
      return BG96_KO;
    }

    _log("BG96 AT OK !!!");

     // Delay for SIM reading
     delay(2000);
     
     return BG96_OK;
}



uint8_t BG96_getIMEI(char *data, uint8_t max_size)
{
  
  at_send_command("AT+GSN");

  if ( at_read_dual_and_copy_to_buffer(data, "OK","ERROR", max_size,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;
  
   _strip_all_newlines( data );
   
  return BG96_OK;
}

uint8_t BG96_getIMSI(char *data, uint8_t max_size)
{
  
  at_send_command("AT+CIMI");
  
  if ( at_read_dual_and_copy_to_buffer(data, "OK","ERROR", max_size,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;

  _strip_all_newlines( data );
  
  
  return BG96_OK;
}

uint8_t BG96_setGSM(char *apn)
{
  char cgdcont[128];

  at_send_command("AT+QCFG=\"nwscanseq\",01,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    

  at_send_command("AT+QCFG=\"nwscanmode\",1,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    
  
  sprintf(cgdcont,"AT+CGDCONT=1,\"IP\",\"%s\"",apn);

  at_send_command(cgdcont);
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    


  sprintf(cgdcont,"AT+QICSGP=1,1,\"%s\",\"\",\"\",1",apn);
  at_send_command(cgdcont);
  
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;   
    
  return BG96_OK;
}

uint8_t BG96_setNBIoT(char *apn, char *band)
{
  char temp_string[128];

  at_send_command("AT+QCFG=\"nwscanseq\",03,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    
  
  at_send_command("AT+QCFG=\"nwscanmode\",0,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    
  
  at_send_command("AT+QCFG=\"iotopmode\",1,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    

  sprintf(temp_string,"AT+QCFG=\"band\",0,0,%s",band);
 
  at_send_command(temp_string);
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    

  sprintf(temp_string,"AT+QICSGP=1,1,\"%s\",\"\",\"\",1",apn);
  at_send_command(temp_string);
  
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;   
    
  return BG96_OK;
}

uint8_t BG96_setCatM1(char *apn)
{
  char cgdcont[128];

  at_send_command("AT+QCFG=\"nwscanseq\",02,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    

 at_send_command("AT+QCFG=\"nwscanmode\",3,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    
 
  at_send_command("AT+QCFG=\"iotopmode\",0,1");
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    


  sprintf(cgdcont,"AT+QICSGP=1,1,\"%s\",\"\",\"\",1",apn);
  at_send_command(cgdcont);
  
  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;   
    
  return BG96_OK;
}


uint8_t BG96_attach()
{

  at_send_command("AT+CGATT=1");

  if ( read_for_responses_dual("OK","ERROR",BG96_LONG_TIMEOUT) != 0 )
    return  BG96_KO;    
 
  return BG96_OK;
}

uint8_t BG96_setOperator(char *operator_code)
{
  char tmp_string[128];

  sprintf(tmp_string,"AT+COPS=1,2,\"%s\"",operator_code);

  at_send_command(tmp_string);
  if ( read_for_responses_dual("OK","ERROR",BG96_LONG_TIMEOUT) != 0 )
    return  BG96_KO;
 
  return BG96_OK;
}


uint8_t BG96_isNetworkAttached(int *attach_status, bool mode)
{
  char data[16];
  int network_status;

  if ( mode )
    at_send_command("AT+CREG?");
  else
    at_send_command("AT+CEREG?");
  
  memset(data,0,sizeof(data));
   
  if ( at_copy_serial_to_buffer(data, ',', 16,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;

  memset(data,0,sizeof(data));
  
  if ( at_copy_serial_to_buffer(data, '\r', 16,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;

  *attach_status = atoi(data);

  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;   

  return BG96_OK;
}


uint8_t BG96_signalQuality(int *rssi, int *ber )
{
  char data[16];
  char *pch;
  
  at_send_command("AT+CSQ");

  memset(data,0,sizeof(data));
  if ( at_copy_serial_to_buffer(data, ':', 16,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;

  memset(data,0,sizeof(data));
  if ( at_copy_serial_to_buffer(data, '\r', 16,  BG96_DEFAULT_TIMEOUT) != AT_COMMAND_SUCCESS )
    return BG96_KO;
    
  _strip_all_newlines( data );

  // we have 2 items
  pch = strtok (data,",");
  
  if (pch!=NULL)
  {
    *rssi = atoi( pch );
    pch = strtok (NULL,",");

    if ( pch != NULL )
    {
      *ber = atoi( pch );

      return BG96_OK;
    }
  }

   return  BG96_KO; 
}



uint8_t BG96_SocketOpen(char *ip, uint16_t port)
{

  at_send_command("AT+QIOPEN=1,0,\"UDP\",\"5.79.89.1\",9200,0,1\r");

  if ( at_read_for_response_single("+QIOPEN: 0,0",10000) != 0 )
        return  BG96_KO;   
 
  return BG96_OK;
}


uint8_t BG96_SocketClose(void)
{

  at_send_command("AT+QICLOSE=0");

  if ( read_for_responses_dual("OK","ERROR",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO;    
 
  return BG96_OK;
}


uint8_t BG96_TXData(char *data, int data_len)
{
  char tx_string[128];
  
  sprintf(tx_string,"AT+QISEND=0,%d",data_len);  
   at_send_command(tx_string);   
   

   if ( at_read_for_response_single(">",BG96_DEFAULT_TIMEOUT) != 0 )
        return  BG96_KO;   
        
  _log("Sending Data to Socket");

  for (int i=0; i<data_len; i++) 
  {
    at_send_char(data[i]);
    delay(1);
  }
  
  if ( read_for_responses_dual("SEND OK","SEND FAIL",BG96_DEFAULT_TIMEOUT) != 0 )
    return  BG96_KO; 

  return BG96_OK;
}
