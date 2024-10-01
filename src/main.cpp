
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
uint32_t startProgram;
unsigned long duration = 900000UL;
unsigned long resetDuration = 3600000UL;

// void (*resetFunc)(void) = 0;

void setup()
{

  Serial.println(F("Wait..."));

  adc_init();
  adc_gpio_init(ADC_PIN);

  // pinSetup();

  ultras_baudrate();

  modemSetup();

  modemRestart();

  modemInfo();

  checkPin();

  checkNetworkReg();

  connectGPRS();

  brokerSetup();

  startMillis = millis();
  startProgram = millis();
  //  counter since program started to start publishing data to mqtt

  // Serial.println("Setup Done");

  // Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{

  if (!connectionStatus())
  {
    readStateReturnCode();
  }

  // powerTest();
  pinSetup();

  trig_read();

  // read_voltage();

  // lp47k_serialMode();

  // lp300k_echoMode();
  // Serial.println("Voltage: ");
  // Serial.print(read_voltage());
  // printf("Voltage %f",read_voltage());

  if (millis() - startMillis >= duration)
  {
    publish_msg();
    startMillis = millis();
  }

  if (millis() - startProgram >= resetDuration)
  {
    Serial.println("Resetting device");
    modemPowerOff();
    delay(3000);
    // resetFunc();
    NVIC_SystemReset();
  }
}
