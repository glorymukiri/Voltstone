#include <Arduino.h>
#include "GSM.h"
#include "mqtt.h"

// #define RX_PIN 17 // 1 // 13//17//13//9//5
// #define TX_PIN 16 // 0 // 12//16//12//8//4
#define RX_PIN 17
#define TX_PIN 16

#define GSM_PIN ""
String IMEI;
// MQTT setup details
const char *broker = "mqtt.thingsboard.cloud"; //   mqtt.flespi.io  demo.thingsboard.io

UART Serial1(TX_PIN, RX_PIN, 0, 0);

//#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(Serial1);
#endif

TinyGsmClient client(modem);

void modemSetup()
{
    // Set GSM module baud rate
    Serial1.begin(9600);
    // delay(6000);

    digitalWrite(1, LOW);
    delay(2000);
    digitalWrite(1, HIGH);
    delay(1000);
}

void modemPowerOff(){
    modem.poweroff();   
}

void resetPico() {
  void (*resetFunc)(void) = 0;
  resetFunc();  // Calls the reset function pointer, triggering a reset
}

void checkTCPConn()
{
    if (client.connected())
    {
        Serial.println(F("TCP connection is okay"));
        mqttReconnect();
    }
    else
    {
        Serial.println(F("TCP disconnected"));
        Serial.println(F("Trying to reconnect TCP..."));
        if (client.connect(broker, 1883))
        {
            Serial.println(F("TCP reconnected"));
        }
        else
        {
            Serial.println(F("Unable to connect TCP"));
            checkGPRSConn();
        }
    }
}

void checkGPRSConn()
{
    if (modem.isGprsConnected())
    {
        Serial.println(F("GPRS is connected"));
        checkTCPConn();
    }
    else
    {
        Serial.println(F("GPRS disconnected"));
        Serial.println(F("Trying to reconnect GPRS..."));
        if (!modem.gprsConnect(""))
        {
            Serial.println(F("Failed to reconnect GPRS"));
            Serial.println(F("Resetting device..."));
            modem.poweroff();
            delay(5000);
            /************addition************/
            modemSetup();
            modem.init();
            // resetFunc();
            delay(5000);
            return;
            /************addition************/
        }
        else
        {
            Serial.println(F("GPRS reconnected successfully"));
        }
    }
}

int8_t regNetStatus()
{
    int8_t netsimstatus = modem.getRegistrationStatus();
    return netsimstatus;
}

void readsimNetworkCode()
{
    switch (regNetStatus())
    {
    case NOT_REGISTERED_NOT_SEARCHING:
        Serial.println(F("Not registered and not searching for network"));
        break;
    case REGISTERED_TO_HOME:
        Serial.println(F("Registered to home network"));
        break;
    case NOT_REGISTERED_BUT_SEARCHING:
        Serial.println(F("Not registered but searching for network"));
        break;
    case REGISTRATION_DENIED:
        Serial.println(F("Registration denied"));
        break;
    case UNKNOWN_EG_OUT_OF_RANGE:
        Serial.println(F("Unknown state"));
        break;
    case REGISTERED_ROAMING:
        Serial.println(F("Registered to national/international network"));
        break;
    default:
        Serial.println(F("Reading connection state..."));
    }
}

void modemRestart()
{
    // To skip it, call init() instead of restart()
    Serial.println(F("Restarting modem..."));
    modem.restart();
    // modem.init();
}

void modemInfo()
{
    String modemInfo = modem.getModemInfo();
    Serial.print(F("Modem Info: "));
    Serial.println(modemInfo);
    IMEI = modem.getIMEI();
    Serial.print(F("IMEI: "));
    Serial.println(IMEI);
}

void checkPin()
{
    // Unlock your SIM card with a PIN if needed
    if (GSM_PIN && modem.getSimStatus() != 3)
    {
        modem.simUnlock(GSM_PIN);
    }
}

void checkNetworkReg()
{
    // wait for 10 minutes for network to connect
    if (!modem.waitForNetwork(180000L, true))
    {
        Serial.println(F("Net registration failed"));
        readsimNetworkCode();
        delay(10000);
        return; // finishes the method setup so that the code below it is not executed; in short it terminates the execution of a function
    }
    Serial.println(F("Net registration success"));
    if (modem.isNetworkConnected())
    {
        Serial.println(F("SIM registered to network"));
    }
}

void connectGPRS()
{
    // GPRS connection parameters are usually set after network registration
    Serial.println(F("Connecting to mobile net... "));
    if (!modem.gprsConnect("sfctelematics"))
    //if (!modem.gprsConnect("onomondo"))
    {
        Serial.println(F("Connect fail"));
        delay(10000);
        return;
    }
    Serial.println(F("Connect success"));

    if (modem.isGprsConnected())
    {
        Serial.println(F("GPRS connected"));
    }
}

// PubSubClient mqtt1;

void brokerSetup()
{
    mqtt.setClient(client);
    mqtt.setServer(broker, 1883);

    // client is now configured for use
    if (mqttConnect())
    {
        Serial.println(F("MQTT connection successful"));
    }
    else
    {
        Serial.println(F("MQTT connection unsuccessful"));
        readStateReturnCode();
    }
}

void powerTest()
{
    delay(5000);
    modem.poweroff();
    delay(5000);
    /************addition************/
    modemSetup();
    modem.init();
    // resetFunc();
    delay(5000);
}
