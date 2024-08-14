#include <Arduino.h>
#include <ArduinoJson.h>
#include "mqtt.h"
#include "GSM.h"
#include "ultrasonic.h"

//MQTT publish details
const char *trackerData = "Reading";

PubSubClient mqtt;


bool mqttConnect()
{
    Serial.println(F("Connecting to Flespi account"));  //flespi account

    // Connect the client //clientID, username, password  //GsmClientName, EDIsxSu3lnnzigHLRIsiQlLex3Sg4lTzMpP9890jkcGtie2XMk5qtEuRvaXqjENV
    bool status = mqtt.connect ("xyz", "", "");
    return status;
}
bool connectionStatus()
{
    bool status = mqtt.loop();
    return status;
}

bool checkServConn()
{
    bool status = mqtt.connected();
    return status;
}

int clientCurrentState()
{
    int returnCode = mqtt.state();
    return returnCode;
}

void readStateReturnCode()
{
    switch (clientCurrentState())
    {
    case MQTT_CONNECTION_TIMEOUT:
        Serial.println(F("Server didn't respond within keepalive time"));
        mqttReconnect();
        break;
    case MQTT_CONNECTION_LOST:
        Serial.println(F("Broken network connection"));
        mqttReconnect();
        break;
    case MQTT_CONNECT_FAILED:
        Serial.println(F("Network connection failed"));
        checkTCPConn();
        break;
    case MQTT_DISCONNECTED:
        Serial.println(F("Client disconnected cleanly."));
        mqttReconnect();
        break;
    case MQTT_CONNECTED:
        Serial.println(F("Successful connection"));
        break;
    case MQTT_CONNECT_BAD_PROTOCOL:
        Serial.println(F("Server doesn't support requested MQTT version"));
        break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
        Serial.println(F("Server rejected client identifier"));
        break;
    case MQTT_CONNECT_UNAVAILABLE:
        Serial.println(F("Server was unable to accept connection"));
        break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
        Serial.println(F("Username/password were rejected"));
        break;
    case MQTT_CONNECT_UNAUTHORIZED:
        Serial.println(F("Client was not authorised to connect"));
        break;
    default:
        Serial.println(F("Reading connection state..."));
    }
}

void mqttReconnect()
{
    Serial.println(F("Reconnecting mqtt..."));
    if (mqttConnect())
    {
        return;
    }
    else
    {
        Serial.println(F("Unable to connect to mqtt"));
    }
}


void publish_msg()
{
  // NB:You cannot set QOS of published message. It is fixed at 0
  char myn[128];

  StaticJsonDocument<128> doc;

  doc["ident"] = IMEI;
  doc["Height"] = 465; //distance; // lat_info1;
  doc["Batteryvoltage"] = Vin;
  // doc["Lng"] = long_info1;
  // doc["T"] = time_sat1;

  serializeJson(doc, myn);

  size_t n = serializeJson(doc, myn);

  if (checkServConn())
  {
    mqtt.publish(trackerData, myn, n);
    Serial.println(F("published"));
  }
  else
  {
    readStateReturnCode();
    Serial.println(F("Not published"));
  }
}