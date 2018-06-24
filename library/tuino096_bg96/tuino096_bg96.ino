
#include "tuino096.h"
#include "bg96.h"

#define DEBUG 1

void _debug_init()
{
   SerialUSB.begin(9600);
}

void _debug(char *string )
{
  SerialUSB.println(string);
}

char IMEI[32];
char IMSI[32];
bool network_attached;

// Variables for regular 'attach' check
long int timer_period_to_attach = 30000;
long int timer_millis_attach = 0;

// Variables for TX interval
long int timer_period_to_tx = 60000;
long int timer_millis_tx = 0;

unsigned char devAddress[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char SendBuffer[256];

/* PARAMETERS BASED ON YOUR OPERATOR */
#define OPERATOR_APN    "___YOUR_OPERATOR_APN___"
#define OPERATOR_CODE   "___YOUR_OPERATOR_CODE___"

/* ENDPOINT WHERE YOU WANT TO SEND DATA */
#define UDP_IP_ADDRESS  "1.1.1.1"
#define UDP_PORT        1234


void setup() {
    // put your setup code here, to run once:

     // Initial Delay is helpful for debugging
     delay(5000);

    _debug_init();
    _debug("Starting");
    
    if ( BG96_init() ==  0 )
     _debug("BG96 OK!");


    if ( BG96_getIMEI( IMEI, 32 ) == 0 )
    {
        char imei_buffer[32];
      
        _debug("IMEI_OK");
        _debug(IMEI);

        sprintf(imei_buffer,"0%s",IMEI);
 
        for (int i=0;i<8;i++)
        {
          uint8_t high,low;

          high = (imei_buffer[i*2]-0x30) * 16;
          low =  (imei_buffer[(i*2)+1]-0x30);

          devAddress[i] = high + low;
        }

    } 

    if ( BG96_getIMSI( IMSI, 32 ) == 0 )
    {
      _debug("IMSI_OK");
      _debug(IMSI);
    } 
    else
    {
      _debug("IMSI_KO");
    }



    if ( BG96_setNBIoT(OPERATOR_APN, BG96_NB1_BAND_ALL) == 0 )
      _debug("NBIOT Init OK!");



/*
    if (  BG96_setGSM(OPERATOR_APN) == 0 ) 
      _debug("GSM Init OK!");
    
*/    

    // a delay... probably not needed
    delay(2000);  

    // we try to attach to network
    if (BG96_setOperator(OPERATOR_CODE) == 0 )
      network_attached = true;
    else
      network_attached = false;

}

void loop() {
  // put your main code here, to run repeatedly:
  int network_status;
  long int delta_tx, delta_attach;
  
  delta_tx = millis() - timer_millis_tx;
  delta_attach = millis() - timer_millis_attach;

  if (( delta_tx > timer_period_to_tx) && network_attached )
  {
     // Here we send Data
     //
     //  BG96_SocketOpen(char *ip, uint16_t port);
     //  BG96_TXData(char *data, int data_len);
     //  BG96_SocketClose();
     

     timer_millis_tx = millis();
  }

  //
  // We regularly check if we are still attached to network
  //
  if (( network_attached == false )|| ( delta_attach > timer_period_to_attach ) )
  {
    if (  BG96_isNetworkAttached(&network_status, false) == 0 )
    {
      if (( network_status == 1 ) || ( network_status == 5) )
      {
        network_attached = true;
        _debug("Attached!");
      
      }
      else
      {
        _debug("Not Attached!");
         
         // we try to reattach
         BG96_setOperator(OPERATOR_CODE);
         delay(5000);
      }
    }

    timer_millis_attach = millis();
 
  }

}
