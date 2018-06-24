# TUINO096 BG96 LIBRARY
Here a first version of a basic library to handle the BG96. Attach to the network and send data via UDP.<br/>


All function returns 0 or 1 in case of error.
```c
#define BG96_OK                        0 
#define BG96_KO                        1

```

## Initialization Functions
```c
uint8_t BG96_init();
```
Initialization of the library, powers up the BG96.

```c
uint8_t BG96_getIMEI(char *data, uint8_t max_size);
```
Returns the IMEI of the module in the *data char buffer, you need to specify the size of the char buffer you are passing.

```c
uint8_t BG96_getIMSI(char *data, uint8_t max_size);
```
Returns the IMSI of the SIM in the *data char buffer, you need to specify the size of the char buffer you are passing. If no SIM is inserted this function will return error.


```c
uint8_t BG96_setGSM(char *apn);
```
Configures for GSM/2G connection. The APN is the only parameter.<br/>

```c
uint8_t BG96_setCatM1(char *apn);
```
Configures for LTE-CATM1 connection. The APN is the only parameter.<br/>

```c
uint8_t BG96_setNBIoT(char *apn);
```
Configures for LTE-NB1 connection. The APN is one of the parameters, the other is the channel, use the following values:<br/>

* BG96_NB1_BAND_B1                
* BG96_NB1_BAND_B2                
* BG96_NB1_BAND_B3                
* BG96_NB1_BAND_B4                
* BG96_NB1_BAND_B5                
* BG96_NB1_BAND_B8                
* BG96_NB1_BAND_B12               
* BG96_NB1_BAND_B13               
* BG96_NB1_BAND_B18               
* BG96_NB1_BAND_B19               
* BG96_NB1_BAND_B20               
* BG96_NB1_BAND_B26               
* BG96_NB1_BAND_B28               
* BG96_NB1_BAND_B39               
* BG96_NB1_BAND_ALL               


## Attaching to network

```c
uint8_t BG96_attach(void);
uint8_t BG96_setOperator(char *code);
```

You can use one of these two functions to attach.<br/>
<b>gmxBC5_attach</b> sends an AT+CGATT=1 to the module, while <b>gmxBC95_setOperator</b> forces the operator code for the attach AT+COPS=1,2,"code". It's up to you to decide which one to use. <br/>
<br/>

```c
uint8_t gBG96_isNetworkAttached(int *attach_status);
```
This function simply check's if we are attached returning in the attach_status variable this possible values:

* 0     Not registered, MT is not currently searching an operator to register to
* 1     Registered, home network
* 2     Not registered, but MT is currently trying to attach or searching an operator to register to
* 3     Registration denied
* 4     Unknown (e.g. out of E-UTRAN coverage)
* 5     Registered, roaming
* 6     Registered for “SMS only”, home network (not applicable)
* 7     Registered for “SMS only”, roaming (not applicable)
* 8     Attached for emergency bearer services only
* 9     Registered for “CSFB not preferred”, home network (not applicable)
* 10    Registered for “CSFB not preferred”, roaming (not applicable)


## Sending  Data

```c
uint8_t BG96_SocketOpen(char *ip, uint16_t port);
```
Opens UDP socket that can be then used for transmission. 

```c
uint8_t BG96_SocketClose();
```
Closes the specified socket.

```c
uint8_t BG96_TXData(char *data, int data_len);
```
the data buffer must contain a binary sequence of bytes, and data_len is the lenght of the bytes to tranmsit.


## Utility Functions

```c
uint8_t BG96_signalQuality(int *rssi, int *ber );
```
Returns ths radio signal level, here the extract from BC95 manual<br/>
<b>rssi</b>  Integer type<br/>
* 0 -113dBm or less
* 1 -111dBm
* 2...30 -109dBm... -53dBm
* 31 -51dBm or greater
* 99 Not known or not detectable
<br/>

<b>ber</b> Integer type; channel bit error rate (in percent)<br/>
* 0...7 As RXQUAL values (please refer to 3GPP specifications)
* 99 Not known or not detectable

<hr>

<br/>
<br/>
Please feel free to send suggestions or correct mistakes that (probably) we have made...  info@gimasi.ch

