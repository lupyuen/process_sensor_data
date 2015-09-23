# process_sensor_data
This is an Arduino sketch for MediaTek LinkIt ONE to read data from various Grove sensors and transmit the sensor data to AzureIoTService hosted in the Microsoft Azure cloud. 

The sketch also activates Grove actuators when requested by AzureIoTService. When it runs, you should see this in the Serial Monitor:

```
Begin Setup
Connecting to WiFi access point...
WiFi Connected
LED is set to LOW
Connecting to TCP socket for receiving actuation commands...
IP: AzureIoTService.cloudapp.net
Port: 2012
Connected to TCP socket for receiving actuation commands
>>> HELLO 1 1
Waiting for TCP response...
End Setup
Begin Loop
temperature = 25.20
lightlevel = 554
Welcome to Azure IoT Service! Your group ID is 1 and device ID is 6
Your group ID and device ID have been updated. Your group ID is 1 and device ID is 1.
End Loop
Begin Loop
temperature = 25.37
lightlevel = 554
End Loop
Begin Loop
temperature = 25.37
lightlevel = 548
End Loop
Begin Loop
temperature = 25.37
lightlevel = 547
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.37&LightLevel=547 HTTP/1.1
Waiting for HTTP response...
End Loop
Begin Loop
temperature = 25.37
lightlevel = 547
End Loop
Begin Loop
temperature = 25.37
lightlevel = 548
End Loop
Begin Loop
temperature = 25.37
lightlevel = 547
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.37&LightLevel=547 HTTP/1.1
Waiting for HTTP response...
HTTP/1.1 200 OK
Cache-Control: private
Content-Type: text/html; charset=utf-8
Expires: Wed, 23 Sep 2015 01:20:20 GMT
Server: Microsoft-IIS/8.5
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Wed, 23 Sep 2015 01:21:20 GMT
Content-Length: 4
Connection: close

'OK'
Sensor data sent to cloud
End Loop
Begin Loop
temperature = 25.37
lightlevel = 553
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.37&LightLevel=553 HTTP/1.1
Waiting for HTTP response...
HTTP/1.1 200 OK
Cache-Control: private
Content-Type: text/html; charset=utf-8
Expires: Wed, 23 Sep 2015 01:20:23 GMT
Server: Microsoft-IIS/8.5
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Wed, 23 Sep 2015 01:21:23 GMT
Content-Length: 4
Connection: close

'OK'
Sensor data sent to cloud
End Loop
Begin Loop
temperature = 25.37
lightlevel = 548
End Loop
Begin Loop
temperature = 25.37
lightlevel = 548
End Loop
Begin Loop
temperature = 25.20
lightlevel = 547
End Loop
Begin Loop
temperature = 25.37
lightlevel = 554
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.37&LightLevel=554 HTTP/1.1
Waiting for HTTP response...
Waiting for HTTP response...
Waiting for HTTP response...
HTTP/1.1 200 OK
Cache-Control: private
Content-Type: text/html; charset=utf-8
Expires: Wed, 23 Sep 2015 01:20:31 GMT
Server: Microsoft-IIS/8.5
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Wed, 23 Sep 2015 01:21:31 GMT
Content-Length: 4
Connection: close

'OK'
Sensor data sent to cloud
End Loop
Begin Loop
temperature = 25.20
lightlevel = 551
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.20&LightLevel=551 HTTP/1.1
Waiting for HTTP response...
HTTP/1.1 200 OK
Cache-Control: private
Content-Type: text/html; charset=utf-8
Expires: Wed, 23 Sep 2015 01:20:35 GMT
Server: Microsoft-IIS/8.5
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Wed, 23 Sep 2015 01:21:35 GMT
Content-Length: 4
Connection: close

'OK'
Sensor data sent to cloud
End Loop
Begin Loop
temperature = 25.20
lightlevel = 554
Sending sensor data to cloud...
Connecting to AzureIoTService.cloudapp.net port 80...
Sending POST request to cloud REST server...
>>> POST /RecordSensorData.aspx?Group=1&Device=1&Temperature=25.20&LightLevel=554 HTTP/1.1
Waiting for HTTP response...
HTTP/1.1 200 OK
Cache-Control: private
Content-Type: text/html; charset=utf-8
Expires: Wed, 23 Sep 2015 01:20:39 GMT
Server: Microsoft-IIS/8.5
X-AspNet-Version: 4.0.30319
X-Powered-By: ASP.NET
Date: Wed, 23 Sep 2015 01:21:39 GMT
Content-Length: 4
Connection: close

'OK'
Sensor data sent to cloud
```
