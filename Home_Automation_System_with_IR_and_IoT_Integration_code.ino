/**********************************************************************************
 *  TITLE: Blynk control 1 Relay using ESP-01 (ESP-01/01s relay v4.0)
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/fhEU80xJi-g
 *  Related Blog : https://iotcircuithub.com/esp32-projects/
 *  by Tech StudyCell
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP8266 : https://github.com/esp8266/Arduino
 *
 *  Download the libraries 
 *  Blynk 1.2.0 Library:  https://github.com/blynkkk/blynk-library
 **********************************************************************************/

/* Fill-in your Template ID (only if using Blynk.Cloud) */

#define BLYNK_TEMPLATE_ID "TMPL3GHgApVKm"
#define BLYNK_TEMPLATE_NAME "esp01 Relay"
#define BLYNK_AUTH_TOKEN "WPVEY5jk-YzbTAzYx389H-Y2gUeYy-Tp"
// Your WiFi credentials.


// Set password to "" for open networks.
char ssid[] = "STUDENT_SECURED12";
char pass[] = "Rana@1234";

bool fetch_blynk_state = false;

#include <ESP8266WiFi.h>        
#include <BlynkSimpleEsp8266.h>
#include "IRremote.hpp"

const int irReceiverPin = 0;
#define RelayPin1 1 
#define RelayPin2 3 //GPIO-0

unsigned long lastSignalTime = 0;
unsigned long signalIgnoreInterval = 100;
//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2
// Relay State
bool toggleState_1 = LOW;
bool toggleState_2 = LOW; //remember the toggle state for relay

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}
BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
 
    Blynk.syncVirtual(VPIN_BUTTON_2);
  }
  else{
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  }
  
}


void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(irReceiverPin , ENABLE_LED_FEEDBACK);  

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);

  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);

  }
}

void loop()
{  
  if (IrReceiver.decode()) {

    unsigned long int decCode = IrReceiver.decodedIRData.command;
    Serial.println(decCode, HEX);

  unsigned long currentTime = millis();
    if (currentTime - lastSignalTime > signalIgnoreInterval) {
delay(300);
      if (decCode == 0x45) {
        digitalWrite(RelayPin1, !digitalRead(RelayPin1));
       
      } else if (decCode == 0x46) {
        digitalWrite(RelayPin2, !digitalRead(RelayPin2));
      }
      
      lastSignalTime = currentTime;
    }

    IrReceiver.resume();
  }
  Blynk.run();
  timer.run();
  
  if (!Blynk.connected()) {
    toggleState_1 = LOW;
    toggleState_2 = LOW;
    digitalWrite(RelayPin1, !toggleState_1);
    digitalWrite(RelayPin2, !toggleState_2);
  }
}
