//  http://www.seeedstudio.com/wiki/Grove_-_LED_Socket_Kit
#include <Arduino.h>  ////
#include "led.h"  ////
 
/*************************   2012 Seeedstudio   **************************
* File Name          : GroveLEDDemoCode.ino
* Author             : Seeedteam
* Version            : V1.1
* Date               : 18/2/2012
* Description        : Demo code for Grove - LED
*************************************************************************/

#define LED 2 //connect LED to digital pin2
void led_setup() {  ////                
  // initialize the digital pin2 as an output.
  pinMode(LED, OUTPUT);     
}
 
void led_loop() {  ////
  digitalWrite(LED, HIGH);   // set the LED on
  delay(500);               // for 500ms
  digitalWrite(LED, LOW);   // set the LED off
  delay(500);
}

////  New functions

void led_on() {
  //  Switch on the LED.
  digitalWrite(LED, HIGH);   //  Set the LED on.  
}

void led_off() {
  //  Switch off the LED.
  digitalWrite(LED, LOW);   //  Set the LED off.
}

int led_status() {
  //  Return the status of the LED.
  return digitalRead(LED);
}

