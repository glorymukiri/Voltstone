#include <Arduino.h>
#include "ultrasonic.h"
#include <ArduinoJson.h>
#include "mqtt.h"
#include "GSM.h"

#define RX 5
#define TX 4

#define echo 5
#define trig 4

// defines variables
long dur;
int dist;

UART u_sensor(TX, RX, 0, 0);


unsigned int distance;
float Vin;
float R3 = 10000.0;
float R2 = 1912.0;


void pinSetup()
{
    pinMode(trig, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echo, INPUT);  // Sets the echoPin as an INPUT
}

/*void lp300k_echoMode()
{
    // Clears the trigPin condition
    digitalWrite(trig, LOW); //
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    dur = pulseIn(echo, HIGH);
    // Calculating the distance
    dist = dur * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println(" cm"); // working  code for aj-sr04m
    // delay(1000);
}*/

void ultras_baudrate()
{
    u_sensor.begin(9600);
}

void trig_read()
{
    u_sensor.write(0x01);
     delay(500);
    if (u_sensor.available())
    {
        //delay(100);
        lp47k_serialMode();
    }
}

void zero_ohm_read()
{
    u_sensor.write(0x01); // 0x01 byte starts distance calculation
    delay(10);
    if (u_sensor.available())
    {
        Serial.println(u_sensor.readString());
    }
}

void lp47k_serialMode()
{
  //AJ-SRO4M
    byte startByte, h_data, l_data, sum = 0;
    byte buf[3];

    startByte = (byte)u_sensor.read();
    if (startByte == 255)
    {
        u_sensor.readBytes(buf, 3);
        h_data = buf[0];
        l_data = buf[1];
        sum = buf[2];
        distance = (h_data << 8) + l_data;
        if (((h_data + l_data) & 0xFF) != sum)
        {
            Serial.println("Invalid result");
        }
        else
        {
            if (distance == 8355)
            {
                Serial.print("Sensor placed in wrong position");
                distance = 0;
            }
            else if (distance <= 200)
            {
                Serial.print("Sensor can't read below this level");
                distance = 200;
            }
            else
            {
                Serial.print("Distance [mm]:");
                Serial.println(distance);
            }
        }
    }
    else
        return;
}



float read_voltage()
{
    uint16_t result = adc_read();
    float voltage = result * 3.3f / (1 << 12); // 12-bit ADC
    Vin = voltage * (R3 + R2) / R2;
    return Vin;
}
