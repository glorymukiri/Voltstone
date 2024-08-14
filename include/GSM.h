enum simnetwork_State
{
    NOT_REGISTERED_NOT_SEARCHING,
    REGISTERED_TO_HOME,
    NOT_REGISTERED_BUT_SEARCHING,
    REGISTRATION_DENIED,
    UNKNOWN_EG_OUT_OF_RANGE,
    REGISTERED_ROAMING
};

#define TINY_GSM_MODEM_SIM800
extern String IMEI;

#include <TinyGsmClient.h>

void modemSetup();
void checkGPRSConn();
void checkTCPConn();
void modemRestart();
void modemInfo();
void checkPin();
void checkNetworkReg();
void brokerSetup();
void connectGPRS();
