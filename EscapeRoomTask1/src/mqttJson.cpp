#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Grove_LED_Bar.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>

//Wifi credentials
const char* ssid = "Redmi";       //Your wifi name
const char* pass = "d7612b80bc9b";    //Your wifi password

//Mqtt broker adress
const char* mqtt_server = "earth.informatik.uni-freiburg.de";
const char* pubTopic = "ubilab/bomb";
const char* subTopic = "ubilab/defuse";

WiFiClient espClient;
PubSubClient client(espClient);


//run this in setup
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

//callback function
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Recieved message: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
}

void setup() {
  setupWifi();
  client.setServer(mqtt_server, 1883);
}

void sendMessage(char* topic, String message) {
  StaticJsonDocument<300> doc;
  doc["part"].set("Display");
  doc["message"].set(message);
}

void loop(void) {

}