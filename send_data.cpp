//  Based on https://github.com/MediaTek-Labs/WeatherStation1.0/blob/master/WeatherStation_1.0.ino

//  Update these 2 lines with your group ID and device ID.
#define GROUP "1"
#define DEVICE "1"

//  This is the REST service for transmitting the sensor values.
#define SITE_URL "AzureIoTService.cloudapp.net"
#define SITE_PORT 80

//  This is a long-lasting TCP socket connection to receive actuation commands from the cloud.
#define SERVER_IP "AzureIoTService.cloudapp.net"
#define SERVER_PORT 2012
/*
Make sure you have a file named password.h containing the lines:
#define WIFI_SSID "yourwifissid"
#define WIFI_KEY "yourwifipassword"
*/
#include <Arduino.h>
#include "password.h"
#include "send_data.h"
#include "led.h"
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <Wire.h>
#include <math.h>
#include <LDateTime.h>
#include <HttpClient.h>
#include <LFlash.h>

// ================================================
// Define
#undef _DBG_
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define per 5 // time interval in seconds to upload sensor data to MCS
#define per1 50 // time interval in seconds to initiate TCP heart beat
#define Drv LFlash

// ================================================
// Global variable

//  For networking.
char ssid[] = WIFI_SSID;
char key[] = WIFI_KEY;  
char ip[] = SERVER_IP;  
int portnum = SERVER_PORT;
LWiFiClient c;
LWiFiClient c2;
HttpClient http(c2);
char connection_info[21]="                    ";

//  This is the Hello message ("HELLO 1 2"), used by the LinkIt ONE to specify the group ID and device ID upon startup.
//  The Hello message is also sent periodically by the LinkIt ONE to keep the TCP connection alive with the server.
String hello;

//  For real-time clock.
unsigned int rtc;
unsigned int lrtc;
unsigned int lrtc2;
unsigned int rtc2;
unsigned int rtc1;
unsigned int lrtc1;
int val = 0;
int state = 0;
int i = 0;

void fatal_error(const char* str)
{
  Serial.println(str);  
  while(1)
    delay(100);
}

// ==============================================================
// TCP Socket server connection functions

void connectTCP(){
  //  Connect to the TCP server to establish a channel for the cloud to send actuation commands to the LinkIt ONE.
  c.stop();
  Serial.println("Connecting to TCP socket for receiving actuation commands...");
  Serial.print("IP: "); Serial.println(ip);  
  Serial.print("Port: "); Serial.println(portnum);  
  while (0 == c.connect(ip, portnum))
  {
    Serial.println("Re-connecting to TCP socket...");    
    delay(1000);
  }  
  //  Tell the server our group ID and device ID.
  Serial.println("Connected to TCP socket for receiving actuation commands");
  hello = "HELLO " + String(GROUP) + " " + String(DEVICE);
  Serial.print(">>> "); Serial.println(hello);
  c.println(hello);
  Serial.println("Waiting for TCP response...");
} //connectTCP

void heartBeat(){
  //  The Hello message is sent periodically by the LinkIt ONE to keep the TCP connection alive with the server.
  Serial.println("Sending TCP heartBeat...");
  Serial.print(">>> "); Serial.println(hello);
  c.println(hello);    
} //heartBeat

void senddata(float temperature_sensor_value, int light_sensor_value) {
  //  Send the sensor values to the cloud.
  //  TODO: Send a dictionary of <key, value> strings instead.
  Serial.println("Sending sensor data to cloud...");
  char buffer1[5];
  sprintf(buffer1, "%.2f", temperature_sensor_value);

  //  Open a connection to the HTTP port (usually port 80).
  LWiFiClient c2;
  unsigned long t1 = millis();
  Serial.print("Connecting to "); Serial.print(SITE_URL); Serial.print(" port "); Serial.print(SITE_PORT); Serial.println("...");
  while (0 == c2.connect(SITE_URL, SITE_PORT))  
  {
    Serial.println("Reconnecting to cloud REST server...");
    delay(1000);
  }
  delay(500);

  //  Send a HTTP POST command to transmit the sensor values.
  //  TODO: May need to do URL encoding of the sensor values if they contain special characters.
  Serial.println("Sending POST request to cloud REST server...");
  String url = "POST /RecordSensorData.aspx?Group=" + String(GROUP) + "&Device=" + String(DEVICE) + 
    "&Temperature=" + String(buffer1) + "&LightLevel=" + String(light_sensor_value) + " HTTP/1.1";
  Serial.print(">>> "); Serial.println(url);  
  c2.println(url);  
  //  Send the other HTTP request headers, according to the HTTP specifications.
  c2.print("Host: ");
  c2.print(SITE_URL); c2.print(":"); c2.println(SITE_PORT);  
  c2.print("Content-Length: ");
  c2.println(0); 
  c2.println("Content-Type: text/csv");
  c2.println("Connection: close");
  c2.println();

  String dataget = "";
  Serial.println("Waiting for HTTP response...");

  int errorcount = 0;
  while (!c2.available())
  {
    Serial.println("Waiting for HTTP response...");
    errorcount += 1;
    if (errorcount > 10) {
      c2.stop();
      return;
    }
    delay(100);
  }
  while (c2)
  {
    int v = c2.read();
    if (v != -1)
    {
      Serial.print(char(v));

      delay(1);
    }
    else
    {
      Serial.println("No more content, disconnecting...");
      c2.stop();
    }
  }
  Serial.println("\r\nSensor data sent to cloud");  
  delay(300);
  unsigned long t2 = millis();
}

// ==============================================================
// Main program
void send_data_setup() {
  Wire.begin();
  LTask.begin();
  LWiFi.begin();
  //while(!Serial) delay(1000); //mark this demo as standalone
  Serial.println("Connecting to WiFi access point...");
  Serial.println(ssid);  
  Serial.println(key);  
  while (0 == LWiFi.connect(ssid, LWiFiLoginInfo(WIFI_AUTH, key))){
    Serial.print(".");  
    delay(1000);
  }
  Serial.println("WiFi Connected");  
  delay(100);
  Serial.println("LED is set to LOW");
  LDateTime.getRtc(&lrtc);
  LDateTime.getRtc(&lrtc2);
  while (!c2.connect(SITE_URL, SITE_PORT)){  
    Serial.println("Reconnecting to cloud REST server...");
    delay(1000);
  }
  delay(100);
  connectTCP();
}

void send_data_loop(float temperature_sensor_value, int light_sensor_value) {
  if (LWiFi.status() == LWIFI_STATUS_DISCONNECTED) {
    Serial.println("Disconnected from WiFi");
    while (0 == LWiFi.connect(ssid, LWiFiLoginInfo(WIFI_AUTH, key))){
      Serial.println("Connecting to WiFi...");
      delay(1000);
    }

  }
  LDateTime.getRtc(&rtc);
  if ((rtc - lrtc) >= per) {
     senddata(temperature_sensor_value, light_sensor_value);
    lrtc = rtc;
  }
  //Check for TCP socket command from MCS Server 
  String tcpcmd="";
  while (c.available())
   {
      int v = c.read();
      if (v != -1)
      {
        Serial.print((char)v);
        tcpcmd += (char)v;
        if (tcpcmd.indexOf("led on") >= 0) {
          Serial.print("Received actuator command: "); Serial.println(tcpcmd);  
          Serial.println("Switching LED ON...");  
          led_on();  
          tcpcmd="";
        }else if(tcpcmd.indexOf("led off") >= 0) {  
          Serial.print("Received actuator command: "); Serial.println(tcpcmd);  
          Serial.println("Switching LED OFF...");  
          led_off();  
          tcpcmd="";
        }
      }
   }
  //Check for hearbeat interval 
  LDateTime.getRtc(&rtc2);
  if ((rtc2 - lrtc2) >= per1) {
    heartBeat();
    lrtc2 = rtc2;
  }  
}

