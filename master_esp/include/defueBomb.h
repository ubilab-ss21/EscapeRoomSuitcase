#include <Arduino.h>

#define wire1 25  // first wire
#define wire2 26  // second wire
#define wire3 32  // third wire
#define wire4 33  // fourth wire
#define wire5 27  // fifth wire

boolean defused = false;

// 25,26,32,33,27

void setupDefuse(){
    pinMode(wire1, INPUT);
    pinMode(wire2, INPUT);
    pinMode(wire3, INPUT);
    pinMode(wire4, INPUT);
    pinMode(wire5, INPUT);
}

void lastTask(){
  while(defused == false){
    int readW1 = analogRead(wire1);
    int readW2 = analogRead(wire2);
    int readW3 = analogRead(wire3);
    int readW4 = analogRead(wire4);
    int readW5 = analogRead(wire5);
    Serial.print("Current Voltage: ");
    Serial.println(readW1);
    Serial.println(readW2);
    Serial.println(readW3);
    Serial.println(readW4);
    Serial.println(readW5);
    if (readW3 == 0){
        
    }
    delay(500);
  }
}
