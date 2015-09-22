//  This is an Arduino sketch for MediaTek LinkIt ONE to read data from various Grove sensors and transmit the sensor data to AzureIoTService hosted in the Microsoft Azure cloud.
//  The sketch also activates Grove actuators when requested by AzureIoTService.
//  Grove LED is connected on Digital Pin 2 (D2)
//  Grove Light sensor is connected on Analogue Pin 0 (A0)
//  Grove Temperature sensor is connected on Analogue Pin 1 (A1)
#include "sensors.h"

//  The setup function runs once when you press reset or power the board.
void setup() {
  Serial.begin(9600);
  Serial.println("Begin Setup");
  //  Initialize digital pin 13, the onboard LED, as an output for monitoring purpose.
  pinMode(13, OUTPUT);

  //  Set up the other sensors and actuators.
  led_setup();
  temperature_sensor_setup();
  light_sensor_setup();
  send_data_setup();
  Serial.println("End Setup");
}

//  The loop function runs over and over again forever.
void loop() {
  Serial.println("Begin Loop");
  delay(1000);  //  Wait for a second between iterations.

  //  Read sensor data and send to the cloud.  Activate actuators if requested by the cloud.
  float temperature_sensor_value = temperature_sensor_loop();
  int light_sensor_value = light_sensor_loop();
  send_data_loop(temperature_sensor_value, light_sensor_value);
  Serial.println("End Loop");
}

//  Moved the system include files here so there's less clutter at the top.
#include <b64.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>
#include <LDateTime.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LWiFiServer.h>
#include <LWiFiUdp.h>
#include <HttpClient.h>
#include <Wire.h>

