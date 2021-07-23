#include <Arduino.h>
#include <stdio.h>
#include <Grove_LED_Bar.h>
//#include <Tone32.h>
#include "batteryCharge.h"
#include "forceSensing.h"
#include "theremin.h"
#include "mealy.h"

#define trigPin 5        // TRIG of ultrasonic pin 5
#define echoPin 18       // ECHO of ultrasonic pin 18
#define buzzer 2         //buzzer pin 2
#define buzzer_channel 0 //buzzer channel
#define helpButton 4      // Pinr für Helpbutton

void clear_outputs_master();

Grove_LED_Bar bar_m(9, 10, false);
Grove_LED_Bar bar2_m(33, 32, false);
Grove_LED_Bar bar3_m(27, 14, false);
Grove_LED_Bar bar4_m(19, 23, false);
int shiftPin_m = 15; //SHCP
int storePin_m = 13; //STCP
int dataPin_m = 12; //SER


void IRAM_ATTR isr2()
{
 buttonPressed = true;
}


void setup(void)
{
  pinMode(helpButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(helpButton), isr2, HIGH);
  setupBatteryCharge();
  setupMealy();

  pinMode(trigPin, OUTPUT); // Config trigger pin to output mode
  pinMode(echoPin, INPUT);  // Config echo pin to input mode
  pinMode(buzzer, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(void)
{
  int start_master = 0; //wait for suitcase to be opened (measure with ultrasonic)
  int slave_restart = 0; //send restart message to slave only once
  while(!start_master){ //wait for suitcase to be opened (measure with ultrasonic)
    if (!client.connected())
    {
      reconnect();
    }
        // Generate 10-microsecond pulse to TRIG pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure duration of pulse from ECHO pin
    float duration_us_start = pulseIn(echoPin, HIGH);

    // Calculate the distance
    float distance_cm_start = 0.017 * duration_us_start;
    
    Serial.println(distance_cm_start);
    if(distance_cm_start > 15.0){
      start_master = 1;
    }
    if(!slave_restart){
      sendMessage(pubTopic,"startSuitcase","","");
      slave_restart=1;
    }
    delay(1000);
  }
  if (!client.connected())
  {
    reconnect();
  }
  //start 7segment
  if(!slave_restart){
    sendMessage(pubTopic,"startSuitcase","","");
  }
  delay(15000);
  slave_restart = 0; //enable restart slave again
  send7segment(pubTopic, "on"); 
  sendMessage(pubTopic, "clear_outputs", "", "");
  clear_outputs_master();
  delay(2000);

  //start Battery charge
  batteryCharge();
  clear_outputs_master();
  sendMessage(pubTopic, "clear_outputs", "", "");
  sendMessage(pubTopic, "Display", "Battery charged", "");
  sendLED(pubTopic, "0", "On");
  sendLED(pubTopic, "1", "On");
  sendLED(pubTopic, "2", "On");
  sendLED(pubTopic, "3", "On");
  sendLED(pubTopic, "4", "On");
  delay(500);
  Serial.println("Battery done");

  //start Forcesensing
  taskForceSensing();
  Serial.println("Force Done");
  clear_outputs_master();
  sendMessage(pubTopic, "clear_outputs", "", "");
  sendMessage(pubTopic, "success", "", "");
  sendLED(pubTopic, "0", "On");
  sendLED(pubTopic, "1", "On");
  sendLED(pubTopic, "3", "On");
  sendLED(pubTopic, "4", "On");
  delay(2000);

  //start Theremin
  thereminTask(trigPin, echoPin, buzzer);  
  Serial.println("theremin Done");
  clear_outputs_master();
  sendMessage(pubTopic, "clear_outputs", "", "");
  sendMessage(pubTopic, "success", "", "");
  sendLED(pubTopic, "0", "On");
  sendLED(pubTopic, "1", "On");
  sendLED(pubTopic, "3", "On");
  delay(2000);

  // Begin Mealy
  sendMessage(pubTopic, "mealy_leds", "", "");
  mealyAutomat();
  sendMessage(pubTopic, "clear_outputs", "", "");
  clear_outputs_master();
  sendMessage(pubTopic, "success", "", "");
  sendLED(pubTopic, "3", "On");
  delay(2000);
  Serial.println("mealy Done");

  // Begin task 5
  sendMessage(pubTopic,"task5","","");
  while(1){
    Serial.println("wait for task5");
    delay(1000);
  }
  client.loop();
}

void clear_outputs_master()
{
  bar_m.setLevel(0);
  bar2_m.setLevel(0);
  bar3_m.setLevel(0);
  bar4_m.setLevel(0);
 // tone(buzzer, 0, 1, 0); //turn off buzzer
  for (int i = 0; i < 8;i++){
    digitalWrite(shiftPin_m, LOW);
    digitalWrite(dataPin_m, LOW);
    digitalWrite(shiftPin_m, HIGH);
  }
    //schreiben der Daten
  digitalWrite(storePin_m, HIGH);
  delay(20); //eine kleine Pause
  //zurücksetzen 
  digitalWrite(storePin_m, LOW);
}