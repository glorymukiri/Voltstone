#include <Arduino.h>
// #include <ArduinoJson.h>
#include "GSM.h"
#include "mqtt.h"
#include "ultrasonic.h"

#define RST_PIN 3
#define SDA_PIN 5
#define ADC_PIN 26

// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_DEBUG Serial

#define TINY_GSM_USE_GPRS true

uint32_t startMillis;
unsigned long duration = 300000UL;

void setup()
{
  Serial.println(F("Wait..."));

  adc_init();
  adc_gpio_init(ADC_PIN);

  ultras_baudrate();

  // pinSetup();

  modemSetup();

  modemRestart();

  modemInfo();

  checkPin();

  checkNetworkReg();

  connectGPRS();

  brokerSetup();
  startMillis = millis();
  // Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
void loop()
{

  connectionStatus();

  //trig_read();
  read_voltage();

  // lp300k_echoMode();
  //Serial.println("Voltage: ");
  //Serial.print(read_voltage());
  // printf("Voltage %f",read_voltage());

  if (millis() - startMillis >= duration)
  {
    publish_msg();
    startMillis = millis();
  }

  
}
