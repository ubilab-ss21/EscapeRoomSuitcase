#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Grove_LED_Bar.h>


Grove_LED_Bar bar( 9, 10, false);
Grove_LED_Bar bar2 (33, 32, false);
Grove_LED_Bar bar3 (27, 14, false);
Grove_LED_Bar bar4 (19, 23, false);

//Force Pins 
int fsrAnalogPin = 34; // FSR is connected to analog 0
int fsrAnalogPin2 = 35; 
int fsrAnalogPin3 = 38;
int fsrReading;      // the analog reading from the FSR resistor divider
int fsrReading2;
int fsrReading3;
bool level;


const char* ssid = "Redmi";       //Your wifi name
const char* pass = "d7612b80bc9b";    //Your wifi password

//Mqtt broker adress
const char* mqtt_server = "earth.informatik.uni-freiburg.de";
const char* pubTopic = "ubilab/bomb";
const char* subTopic = "ubilab/defuse";

WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char message[50];
char recieved_msg[50];

//void lastTask(){
//  pinMode(wire1, INPUT);
//  pinMode(wire2, INPUT);
//  pinMode(wire3, INPUT);
//  pinMode(led1, OUTPUT);
//  pinMode(led2, OUTPUT);
//  pinMode(led3, OUTPUT);
//  digitalWrite(led1, HIGH);
//  digitalWrite(led2, HIGH);
//  digitalWrite(led3, HIGH);
//  while(1){
//    u = analogRead(wire1);
//    fsrReading = analogRead(wire2);
//    fsrReading2 = analogRead(wire3);
//    Serial.print("Current Voltage: ");
//    Serial.println(u);
//    Serial.println(fsrReading);
//    Serial.println(fsrReading2);
//    if(fsrReading2 < 2000){digitalWrite(led3, LOW);}
//    if(fsrReading < 2000){digitalWrite(led2,LOW);}
//    if(u < 2000){digitalWrite(led1, LOW);}
//    delay(500);
//  }
//}
//

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void taskFOrceSensing() {

  while(level == false){
    fsrReading = analogRead(fsrAnalogPin);
    fsrReading = map(fsrReading, 0, 4095, 0, 10);
    bar.setLevel(fsrReading);
    fsrReading2 = analogRead(fsrAnalogPin2);
    fsrReading2 = map(fsrReading2, 0, 4095, 0, 10);
    bar2.setLevel(fsrReading2);
    fsrReading3 = analogRead(fsrAnalogPin3);
    fsrReading3 = map(fsrReading3, 0, 4095, 0, 10);
    bar4.setLevel(fsrReading3);
    Serial.println("Analog reading = ");
    Serial.print(fsrReading);
    Serial.print(" ");
    Serial.print(fsrReading2);
    Serial.print(" ");
    Serial.print(fsrReading2);
    if (fsrReading2 == 5 and fsrReading == 5) {
      Serial.print("LEVEL");
      bar.toggleLed(5);
      bar2.toggleLed(5);
      bar3.toggleLed(5);
      delay(2000);
    }
    delay(500);
  }
}

void setupWifi(){
  delay(1000);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }

  Serial.print("\nConnected to ");
  Serial.println(ssid);
}

void reconnect(){
  while(!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.println(mqtt_server);
    if(client.connect("esp32_Michael")){
      Serial.print("\nConnected to ");
      Serial.println(mqtt_server);
      client.subscribe(subTopic);
    } else {
      Serial.println("\nTrying");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Recieved message: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  // lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  // lcd.print((char*) payload);
  delay(2000);
  //if(strcmp("LEDon", (char*) payload)) {
  //  digitalWrite(14, HIGH);
  //  Serial.println((char*) payload);
  //}
  //if(strcmp("LEDoff", (char*) payload)){
  //  digitalWrite(14, LOW);
  //  Serial.print((char*) payload);
  //}
  Serial.println();

}


void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    // set up the LCD's number of columns and rows:
  // lcd.begin(16, 2);
  // Print a message to the LCD.
  // lcd.print("hello, world!");
}

void loop(void) {
//  lastTask();
  if (!client.connected()){
    reconnect();
  }
  client.loop();
//
//  currentTime = millis();
//  if(currentTime - lastTime > 10000) {
//    count++;
//    snprintf(message, 75, "Count: %ld", count);
//    Serial.print("Sending Messsage: ");
//    Serial.println(message);
    Serial.println("sending disaply");
    client.publish(subTopic, "Display");
    delay(5000);
//    lastTime = millis();
// }
  //while(1){
  //  // put your main code here, to run repeatedly:
  //  //taskFOrceSensing();
  //}
}