#include <Arduino.h>
#include <LiquidCrystal.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <iostream>
#include <sstream>

//const char *ssid = "Redmi";        //Your wifi name
//const char *pass = "d7612b80bc9b"; //Your wifi password
const char *ssid = "INNIRION";        //Your wifi name
const char *pass = "23726994"; //Your wifi password

//Mqtt broker adress
const char *mqtt_server = "earth.informatik.uni-freiburg.de";
const char *pubTopic = "ubilab/bomb";
const char *subTopic = "ubilab/defuse";

const int button = 4;
const int rs = 19, en = 23, d4 = 9, d5 = 10, d6 = 5, d7 = 18;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char message[50];
char recieved_msg[50];

Adafruit_7segment matrix = Adafruit_7segment();
int segment_status = 0; //7segment off in beginning (0); on (1); solved (2)
unsigned long startTime = 0;
unsigned long myTime;
unsigned long timeDiff = 0;
unsigned long timeDiff_old = 0;

//led driver setup
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40, Wire);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x4C, Wire);

int task5 = 0;
bool attachSuitcase = false;

#define cable1 26
#define cable2 32
#define cable3 33
#define cable4 27
#define cable5 25

int pullCable = 0;
int task5_solved = 0;

long timer_countdown = 1200;
int timeout = 0;

/*
int brightness[80] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                      18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,
                      6, 5, 4, 3, 2, 1};
int state = 0;
int dutyCycle = 0;
*/

void clear_outputs_slave();
void task_solved_blink();
void mealy_leds();
void update_7segment();

void setupWifi()
{
  delay(200);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("-");
  }

  Serial.print("\nConnected to ");
  Serial.println(ssid);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("\nConnecting to ");
    Serial.println(mqtt_server);
    if (client.connect("esp32_Slave"))
    {
      Serial.print("\nConnected to ");
      Serial.println(mqtt_server);
      Serial.println(pubTopic);
    }
    else
    {
      Serial.println("\nTrying");
      delay(5000);
    }
  }
}
/*
void blinkLed(int ledPin, int driver)
{
  while(1){ //use millis
  if (state >= 79) //prevent overflow
  {
    state = 0;
  }
  else
  {
    state++; //set next state in array
  }
  dutyCycle = 102 * brightness[state]; //define state for breathing effect
  Serial.println(dutyCycle);
  Serial.println(state);
  if(driver == 1){
    pwm1.setPWM(ledPin, 0, dutyCycle);
  }
  else if (driver == 2)
  {
    pwm2.setPWM(ledPin, 0, dutyCycle);
  }
   // delay(15);
  }
}
*/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("callback");
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char *part = doc["part"];
  const char *message1 = doc["message1"];
  const char *message2 = doc["message2"];
  Serial.println(part);
  Serial.println(message1);
  Serial.println(message2);
  if (strcmp("Display", part) == 0)
  {
    lcd.clear();
    delay(1); //test
    lcd.setCursor(0, 0);
    lcd.print(message1);
    lcd.setCursor(0, 1);
    lcd.print(message2);
    Serial.println("message on display");
  }
  else if (strcmp("7segment", part) == 0)
  {
    if (strcmp("on", message1) == 0)
    {
      segment_status = 1;
      startTime = millis();
    }
    else if (strcmp("stop", message1) == 0)
    {
      segment_status = 2;
    }
    Serial.println("7segment status: ");
    Serial.println(message1);
  }
  else if (strcmp("LED1", part) == 0)
  {
    std::stringstream ss(message1);
    ss << message1;
    unsigned int ledPin;
    ss >> ledPin;
    Serial.println("ledPin: ");
    Serial.println(ledPin);
    if (strcmp("On", message2) == 0)
    {
      pwm1.setPin(ledPin, 2000, false);
    }
    else if (strcmp("Off", message2) == 0)
    {
      pwm1.setPin(ledPin, 0, false);
    }
    /*
    else if (strcmp("Blink", message2) == 0)
    {
      blinkLed(ledPin, 1);
    }
    */
  }
  else if (strcmp("LED2", part) == 0)
  {
    std::stringstream ss(message1);
    ss << message1;
    unsigned int ledPin;
    ss >> ledPin;
    Serial.println("ledPin: ");
    Serial.println(ledPin);
    if (strcmp("On", message2) == 0)
    {
      pwm2.setPin(ledPin, 2000, false);
    }
    else if (strcmp("Off", message2) == 0)
    {
      pwm2.setPin(ledPin, 0, false);
    }
    /*
    else if (strcmp("Blink", message2) == 0)
    {
      blinkLed(ledPin, 2);
    }
    */
  }
  else if (strcmp("clear_outputs", part) == 0)
  {
    clear_outputs_slave();
  }
  else if (strcmp("success", part)==0)
  {
    task_solved_blink();
  }
  else if (strcmp("mealy_leds", part) == 0)
  {
    mealy_leds();
  }
  else if (strcmp("task5", part) == 0)
  {
    task5 = 1;
  }
  else if (strcmp("startSuitcase", part) == 0)
  {
    ESP.restart();
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);

  //cable pins setup
  pinMode(cable1, INPUT);
  pinMode(cable2, INPUT);
  pinMode(cable3, INPUT);
  pinMode(cable4, INPUT);
  pinMode(cable5, INPUT);

  //led driver setup
  pwm1.begin();
  pwm1.setPWMFreq(1000);
  pwm2.begin();
  pwm2.setPWMFreq(1000);
  Wire.setClock(400000);

  setupWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lcd.begin(16, 2);
  lcd.print("Lets play a");
  lcd.setCursor(0, 1);
  lcd.print("Game");
  delay(1000);
  lcd.clear();
  //setup 7 segment
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!client.connected())
  {
    reconnect();
  }
  client.subscribe(pubTopic);
  client.loop();

  if (segment_status == 1)
  {

    myTime = millis();
    timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
    if (timeDiff > timeDiff_old)
    { //only update if new
      update_7segment();
    }
  }
  else if (segment_status == 2)
  {
    update_7segment();
    for (int i = 0; i < 10; i++)
    {
      pwm1.setPin(i, 4000, false);
      delay(28);
    }
    for (int i = 0; i < 8; i++)
    {
      pwm2.setPin(i, 4000, false);
      delay(28);
    }
    matrix.clear();
    matrix.writeDisplay();
    for (int i = 0; i < 10; i++)
    {
      pwm1.setPin(i, 0, false);
      delay(28);
    }
    for (int i = 0; i < 8; i++)
    {
      pwm2.setPin(i, 0, false);
      delay(28);
    }
  }
  if(task5==1){
    if (!digitalRead(cable1))
    {
      pullCable = 1;
    }
    else if (!digitalRead(cable2))
    {
      pullCable = 2;
    }
    else if (!digitalRead(cable3))
    {
      pullCable = 3;
    }
    else if (!digitalRead(cable5))
    {
      pullCable = 5;
    }
    else if (!digitalRead(cable4))
    {
      pullCable = 4;
    }
    //Serial.println(pullCable);
    if (pullCable)
    { //at least one cable pulled out
      if (pullCable == 4)
      {
        delay(2000);
        lcd.clear();
        lcd.print("Concratulations!");
        lcd.setCursor(0, 1);
        lcd.print("You did it!");
        Serial.println("task 5 solved");
        task5_solved = 1; //exit loop and solved task5
        segment_status = 2;
        task5 = 0;
      }
      else
      {
        delay(2000);
        lcd.clear();
        lcd.print("Ohh no!");
        lcd.setCursor(0, 1);
        lcd.print("You are dead now!");
        Serial.println("BOOOOOOOOOOOOOOOOOOOOOM!"); //wrong cable --> game over
        task5_solved = 2;                           //exit loop, but not solved
        segment_status = 2;
        task5 = 0;
      }
    }

  }
  
}

void clear_outputs_slave()
{
  for (int i = 0; i < 16; i++) //turn off all leds at led driver 1 (red leds)
  {
    pwm1.setPin(i, 0, false);
  }
  for (int i = 0; i < 16; i++)
  {
    pwm2.setPin(i, 0, false); //turn off all leds at led driver 2 (green leds)
  }
  lcd.clear(); //turn off display
}

void task_solved_blink(){
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
  for (int i = 0; i < 10; i++)
  {
    pwm1.setPin(i, 4000, false);
    delay(100);
  }
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
  for (int i = 0; i < 8; i++)
  {
    pwm2.setPin(i, 4000, false);
    delay(100);
  }
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
  for (int j = 0; j < 5;j++){
    myTime = millis();
    timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
    update_7segment();
    for (int i = 0; i < 10; i++)
    {
      pwm1.setPin(i, 0, false);
    }
    for (int i = 0; i < 8; i++)
    {
      pwm2.setPin(i, 0, false);
    }
    delay(400);
    myTime = millis();
    timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
    update_7segment();
    for (int i = 0; i < 10; i++)
    {
      pwm1.setPin(i, 4000, false);
    }
    for (int i = 0; i < 8; i++)
    {
      pwm2.setPin(i, 4000, false);
    }
    delay(400);
  }
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
  for (int i = 0; i < 10; i++)
  {
    pwm1.setPin(i, 0, false);
    delay(100);
  }
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
  for (int i = 0; i < 8; i++)
  {
    pwm2.setPin(i, 0, false);
    delay(100);
  }
  myTime = millis();
  timeDiff = (myTime - startTime) / 1000; //time difference rounded in seconds
  update_7segment();
}

void mealy_leds(){ 
  pwm2.setPin(0, 4000, false);
  pwm2.setPin(1, 4000, false);
  pwm2.setPin(2, 0, false);
  pwm2.setPin(3, 4000, false);
  pwm2.setPin(4, 0, false);
  pwm2.setPin(5, 4000, false);
  pwm2.setPin(6, 4000, false);
  pwm2.setPin(7, 0, false);
}

void update_7segment(){
  timeDiff_old = timeDiff;
  int timeDiff_down = timer_countdown - timeDiff;
  if(timeDiff_down <= 0 && timeout == 0){
    delay(2000);
    lcd.clear();
    lcd.print("You are");
    lcd.setCursor(0, 1);
    lcd.print("dead now!");
    Serial.println("BOOOOOOOOOOOOOOOOOOOOOM!"); //wrong cable --> game over
    task5_solved = 2;                           //exit loop, but not solved
    segment_status = 2;
    task5 = 0;
    timeout = 1;
  }
  if(!timeout){
    int digit1 = timeDiff_down / 600;
    int digit2 = timeDiff_down / 60;
    digit2 = digit2 % 10;
    int secondsTemp = timeDiff_down % 60;
    int digit3 = secondsTemp / 10;
    int digit4 = secondsTemp % 10;
    matrix.writeDigitNum(0, digit1, false);
    matrix.writeDigitNum(1, digit2, false);
    matrix.writeDigitNum(3, digit3, false);
    matrix.writeDigitNum(4, digit4, false);
    matrix.drawColon(true);
    matrix.writeDisplay();
  }else{
    matrix.writeDigitNum(0, 0, false);
    matrix.writeDigitNum(1, 0, false);
    matrix.writeDigitNum(3, 0, false);
    matrix.writeDigitNum(4, 0, false);
    matrix.drawColon(true);
    matrix.writeDisplay();
  }
}