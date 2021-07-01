#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Grove_LED_Bar.h>


Grove_LED_Bar bar( 9, 10, true);
int fsrAnalogPin = 2; // FSR is connected to analog 0
int fsrAnalogPin2 = 4; 
int fsrReading;      // the analog reading from the FSR resistor divider
int fsrReading2;

const char* ssid = "WLAN";       //Your wifi name
const char* pass = "wifiPW";    //Your wifi password

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

void setupWifi(){
  delay(100);
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
  if(strcmp("LEDon", (char*) payload)) {
    digitalWrite(14, HIGH);
    Serial.println((char*) payload);
  }
  if(strcmp("LEDoff", (char*) payload)){
    digitalWrite(14, LOW);
    Serial.print((char*) payload);
  }
  Serial.println();

}

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //setupWifi();
  //client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop(void) {
 static float i = 0;
 i++;
 if(i > 10){
   i = 0;
 }
 Serial.println(i);
 bar.setLevel(i);
 delay(500);

//  if (!client.connected()){
//    reconnect();
//  }
//  client.loop();
//
//  currentTime = millis();
//  if(currentTime - lastTime > 10000) {
//    count++;
//    snprintf(message, 75, "Count: %ld", count);
//    Serial.print("Sending Messsage: ");
//    Serial.println(message);
//    client.publish(pubTopic, message);
//    lastTime = millis();
// }
  //while(1){
  //  // put your main code here, to run repeatedly:
  //  fsrReading = analogRead(fsrAnalogPin);
  //  fsrReading = map(fsrReading, 0, 4095, 0, 10);
  //  fsrReading2 = analogRead(fsrAnalogPin2);
  //  fsrReading2 = map(fsrReading2, 0, 4095, 0, 10);
  //  Serial.print("Analog reading = ");
  //  Serial.println(fsrReading);
  //  Serial.print(" ");
  //  Serial.println(fsrReading2);
  //  if (fsrReading2 == fsrReading and not fsrReading2 == 0) {
  //    Serial.print("LEVEL");
  //    delay(10000);
  //    return;
  //  }
  //  delay(200);
  //}
}