//  Based on https://github.com/MediaTek-Labs/WeatherStation1.0/blob/master/WeatherStation_1.0.ino

//  Update these 2 lines with your group ID and device ID.
#define GROUP "1"
#define DEVICE "1"

//  This is the REST service for transmitting the sensor values.
#define SITE_URL "AzureIoTService.cloudapp.net"
#define SITE_PORT 80

//  This is a long-lasting TCP socket connection to receive actuation commands from the cloud.
#define SERVER_IP "AzureIoTService.cloudapp.net"
#define SERVER_PORT "2012"

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

const unsigned char OSS = 0;  // Oversampling Setting
// Calibration values
int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;
unsigned int rtc;
unsigned int lrtc;
unsigned int lrtc2;
unsigned int rtc2;
unsigned int rtc1;
unsigned int lrtc1;
char ssid[] = WIFI_SSID;
char key[] = WIFI_KEY;  
char deviceid[] = "mydeviceid";  
char devicekey[] = "mydevicekey";  
LWiFiClient c;
LWiFiClient c2;
char connection_info[21]="                    ";
char port[] = SERVER_PORT;
char ip[] = SERVER_IP;  
int portnum;
int val = 0;
String tcpdata;
String tcpcmd_led_on;
String tcpcmd_led_off;
String upload_led;
HttpClient http(c2);
int state = 0;
int i = 0;


void fatal_error(const char* str)
{
  Serial.println(str);  
  while(1)
    delay(100);
}

// ==============================================================
// Call MCS API to get TCP Socket connection information

void getconnectInfo(){
  //calling RESTful API to get TCP socket connection
  //  TODO: Remove hardcoded group and device.
  String url = "GET /?Group=" + String(GROUP) + "&Device=" + String(DEVICE) + " HTTP/1.1";
  Serial.println(url);
  c2.println(url);  
  c2.print("Host: ");
  c2.print(SITE_URL); c2.print(":"); c2.println(SITE_PORT);  
  c2.println("Connection: close");
  c2.println();
  
  delay(500);

  int errorcount = 0;
  while (!c2.available())
  {
    Serial.println("waiting HTTP response: ");
    Serial.println(errorcount);
    errorcount += 1;
    if (errorcount > 10) {
      c2.stop();
      return;
    }
    delay(100);
  }
  int err = http.skipResponseHeaders();

  int bodyLen = http.contentLength();
  Serial.print("Content length is: ");
  Serial.println(bodyLen);
  Serial.println();
  char c;
  int ipcount = 0;
  int count = 0;
  int separater = 0;
  while (c2)
  {
    int v = c2.read();
    if (v != -1)
    {
      c = v;
      Serial.print(c);
      //  TODO
      //connection_info[ipcount]=c; ////
      if(c==',')
      separater=ipcount;
      ipcount++;    
    }
    else
    {
      Serial.println("no more content, disconnect");
      c2.stop();

    }
    
  }
  Serial.print("The connection info: ");
  Serial.println(connection_info);

  Serial.println("The TCP Socket connection instructions:");
  Serial.print("IP: ");
  Serial.println(ip);
  Serial.print("Port: ");
  Serial.println(port);
  portnum = atoi (port);

} //getconnectInfo

// ==============================================================
// TCP Socket server connection functions

void connectTCP(){
  //establish TCP connection with TCP Server with designate IP and Port
  c.stop();
  Serial.println("Connecting to TCP");
  Serial.print("IP: "); Serial.println(ip);  
  Serial.print("Port: "); Serial.println(portnum);  
  while (0 == c.connect(ip, portnum))
  {
    Serial.println("Re-Connecting to TCP");    
    delay(1000);
  }  
  //  Tell the server our group ID and device ID.
  Serial.println("send TCP connect");
  String hello = "HELLO " + String(GROUP) + " " + String(DEVICE);
  c.println(hello);
  c.println();
  Serial.println("waiting TCP response:");
  Serial.println("Connected..");  
} //connectTCP

void heartBeat(){
  Serial.println("send TCP heartBeat");
  c.println(tcpdata);
  c.println();
    
} //heartBeat

// ==============================================================
// MCS API call functions

void senddata(float temperature_sensor_value, int light_sensor_value) {
  Serial.println("Begin of senddata");
  Serial.println("1");
  char buffer1[5];
  sprintf(buffer1, "%.2f", temperature_sensor_value);
  Serial.println("2");

  LWiFiClient c2;
  unsigned long t1 = millis();
  Serial.print("Connecting to "); Serial.print(SITE_URL); Serial.print(":"); Serial.println(SITE_PORT);
  while (0 == c2.connect(SITE_URL, SITE_PORT))  
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }

  delay(500);
  Serial.println("send POST request");
  String url = "POST /RecordSensorData.aspx?Group=" + String(GROUP) + "&Device=" + String(DEVICE) + 
    "&Temperature=" + String(buffer1) + "&LightLevel=" + String(light_sensor_value) + " HTTP/1.1";
  Serial.println(url);  
  c2.println(url);  
  c2.print("Host: ");
  c2.print(SITE_URL); c2.print(":"); c2.println(SITE_PORT);  
  c2.print("Content-Length: ");
  c2.println(0); 
  //c2.println(thisLength);
  c2.println("Content-Type: text/csv");
  c2.println("Connection: close");
  c2.println();
  //c2.println(data);

  String dataget = "";
  Serial.println("waiting HTTP response:");

  int errorcount = 0;
  while (!c2.available())
  {
    Serial.println("waiting HTTP response:");
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
      Serial.println("no more content, disconnect");
      c2.stop();
    }
  }
  Serial.println("\r\nData uploaded");  
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
  Serial.println("Connecting to AP");
  Serial.println(ssid);  
  Serial.println(key);  
  while (0 == LWiFi.connect(ssid, LWiFiLoginInfo(WIFI_AUTH, key))){
    Serial.print(".");  
    delay(1000);
  }
  Serial.println("AP Connected");  
  delay(100);
  Serial.print("LED is set to LOW");
  LDateTime.getRtc(&lrtc);
  LDateTime.getRtc(&lrtc2);
  while (!c2.connect(SITE_URL, SITE_PORT)){  
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }
  delay(100);
  tcpdata = String(deviceid) + "," + String(devicekey) + ",0";
  tcpcmd_led_on = "led on";
  tcpcmd_led_off = "led off";
  getconnectInfo();
  connectTCP();
}


void send_data_loop(float temperature_sensor_value, int light_sensor_value) {
  if (LWiFi.status() == LWIFI_STATUS_DISCONNECTED) {
    Serial.println("Disconnected from AP");
    while (0 == LWiFi.connect(ssid, LWiFiLoginInfo(WIFI_AUTH, key))){
      Serial.println("Connecting to AP");
      delay(1000);
    }

  }
  LDateTime.getRtc(&rtc);
  if ((rtc - lrtc) >= per) {
    Serial.println("Senddata");
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
          Serial.println("Switch LED ON ");  
          led_on();  
          tcpcmd="";
        }else if(tcpcmd.indexOf("led off") >= 0) {  
          Serial.println("Switch LED OFF");  
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

