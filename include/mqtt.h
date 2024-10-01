// use this if a connection attempt fails to get more info about the failure
extern const char* trackerData;

enum clientstate_Code
{
    MQTT_CONNECTION_TIMEOUT = -4,
    MQTT_CONNECTION_LOST,
    MQTT_CONNECT_FAILED,
    MQTT_DISCONNECTED,
    MQTT_CONNECTED = 0,
    MQTT_CONNECT_BAD_PROTOCOL,
    MQTT_CONNECT_BAD_CLIENT_ID,
    MQTT_CONNECT_UNAVAILABLE,
    MQTT_CONNECT_BAD_CREDENTIALS,
    MQTT_CONNECT_UNAUTHORIZED
};

#include <PubSubClient.h>

extern PubSubClient mqtt;

bool mqttConnect();
bool connectionStatus();
bool checkServConn();
int clientCurrentState();
void readStateReturnCode();
void mqttReconnect();
void publish_msg();