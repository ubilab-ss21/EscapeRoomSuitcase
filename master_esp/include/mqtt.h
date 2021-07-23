#include <Arduino.h>
#include <stdio.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//const char *ssid = "unicorn";        //Your wifi name
//const char *pass = "abcdefgh"; //Your wifi password
//const char *ssid = "Redmi";        //Your wifi name
//const char *pass = "d7612b80bc9b"; //Your wifi password
//const char *ssid = "INNIRION";        //Your wifi name
//const char *pass = "23726994"; //Your wifi password
const char *ssid = "o2-WLAN36";
const char *pass = "QwP3He)$FEP&";

//Mqtt broker adress
const char *mqtt_server = "earth.informatik.uni-freiburg.de";
const char *pubTopic = "ubilab/bomb";
const char *subTopic = "ubilab/defuse";

bool buttonPressed = false;
int currentTask = 0;
int hintNumber = 0;

// Wifi
WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char message[50];
char recieved_msg[50];

void setupWifi()
{
  delay(1000);
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
    if (client.connect("esp32_Michael"))
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

void reconnectIf() {
  if (!client.connected())
  {
    reconnect();
  }
}

void sendLED(const char *topic, String ledPin, String mode) {
  reconnectIf();
  StaticJsonDocument<300> doc;
  doc["part"].set("LED1");
  doc["message1"].set(ledPin);
  doc["message2"].set(mode);
  Serial.println("Sending Message: ");
  Serial.println(ledPin);
  Serial.println(mode);

  char JSONmessageBuffer[100];
  serializeJson(doc, JSONmessageBuffer);
  client.publish(topic, JSONmessageBuffer);
}

void sendMessage(const char *topic,String part, String message1, String message2)
{
  reconnectIf();
  StaticJsonDocument<300> doc;
  doc["part"].set(part);
  doc["message1"].set(message1);
  doc["message2"].set(message2);
  Serial.println("Sending MEssage: ");
  Serial.print(message1);
  Serial.print(message2);

  char JSONmessageBuffer[100];
  serializeJson(doc, JSONmessageBuffer);
  client.publish(topic, JSONmessageBuffer);
}

void send7segment(const char *topic, String message1)
{
  reconnectIf();
  StaticJsonDocument<300> doc;
  doc["part"].set("7segment");
  doc["message1"].set(message1);
  Serial.println("Sending 7segment: ");
  Serial.print(message1);
  char JSONmessageBuffer[100];
  serializeJson(doc, JSONmessageBuffer);
  client.publish(topic, JSONmessageBuffer);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("callback");
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char *part = doc["part"];
  const char *message = doc["message"];
  Serial.println(part);
  Serial.println(message);
  Serial.print("Recieved message: ");
  Serial.println(topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  //lcd.clear();
  //lcd.setCursor(0, 1);
  //// print the number of seconds since reset:
  //lcd.print(message);
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
