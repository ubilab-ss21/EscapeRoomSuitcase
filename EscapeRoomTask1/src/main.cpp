#include <Arduino.h>
int fsrAnalogPin = 2; // FSR is connected to analog 0
int fsrAnalogPin2 = 4; 
int fsrReading;      // the analog reading from the FSR resistor divider
int fsrReading2;

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop(void) {
  while(1){
    // put your main code here, to run repeatedly:
    fsrReading = analogRead(fsrAnalogPin);
    fsrReading = map(fsrReading, 0, 4095, 0, 10);
    fsrReading2 = analogRead(fsrAnalogPin2);
    fsrReading2 = map(fsrReading2, 0, 4095, 0, 10);
    Serial.print("Analog reading = ");
    Serial.println(fsrReading);
    Serial.print(" ");
    Serial.println(fsrReading2);
    if (fsrReading2 == fsrReading and not fsrReading2 == 0) {
      Serial.print("LEVEL");
      delay(10000);
      return;
    }
    delay(200);
  }
}