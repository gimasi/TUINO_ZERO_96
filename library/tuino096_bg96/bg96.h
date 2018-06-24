#ifndef _GMX_BG96_H_
#define _GMX_BG96_H_

#include <Arduino.h>

#define BG96_OK                      0 
#define BG96_KO                      1


#define BG96_DEFAULT_TIMEOUT          2000
#define BG96_BOOT_TIMEOUT             6000
#define BG96_LONG_TIMEOUT            120000



#define BG96_NB1_BAND_B1                "1"
#define BG96_NB1_BAND_B2                "2"
#define BG96_NB1_BAND_B3                "4"
#define BG96_NB1_BAND_B4                "8"
#define BG96_NB1_BAND_B5                "10"
#define BG96_NB1_BAND_B8                "80"
#define BG96_NB1_BAND_B12               "800"
#define BG96_NB1_BAND_B13               "1000"
#define BG96_NB1_BAND_B18               "20000"
#define BG96_NB1_BAND_B19               "40000"
#define BG96_NB1_BAND_B20               "80000"
#define BG96_NB1_BAND_B26               "2000000"
#define BG96_NB1_BAND_B28               "8000000"
#define BG96_NB1_BAND_B39               "4000000000"
#define BG96_NB1_BAND_ALL               "A0E189F"


uint8_t BG96_init();
uint8_t BG96_getIMEI(char *data, uint8_t max_size);
uint8_t BG96_getIMSI(char *data, uint8_t max_size);

uint8_t BG96_setOperator(char *code);
uint8_t BG96_setGSM(char *apn);
uint8_t BG96_setNBIoT(char *apn, char *band);
uint8_t BG96_setCatM1(char *apn);

uint8_t BG96_attach();
uint8_t BG96_isNetworkAttached(int *attach_status, bool mode);

uint8_t BG96_TXData(char *data, int data_len);
uint8_t BG96_SocketClose();
uint8_t BG96_SocketOpen(char *ip, uint16_t port);


uint8_t BG96_signalQuality(int *rssi, int *ber );


#endif 
