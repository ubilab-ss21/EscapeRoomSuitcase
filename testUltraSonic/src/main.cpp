#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <Tone32.h>

#define trigPin 12       // TRIG of ultrasonic
#define echoPin 14       // ECHO of ultrasonic
#define buzzer 10        //buzzer
#define buzzer_channel 0 //buzzer channel

float duration_us, distance_cm;

void select_tone(float dist);

void setup()
{
  pinMode(trigPin, OUTPUT); // Config trigger pin to output mode
  pinMode(echoPin, INPUT);  // Config echo pin to input mode
  //pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  // Generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distance_cm = 0.017 * duration_us;

  Serial.print("distance =");
  Serial.println(distance_cm);

  select_tone(distance_cm);
}

void select_tone(float dist)
{
  //noTone(buzzer, buzzer_channel);
  if (dist < 5)
  {
    tone(buzzer, NOTE_A4, 100, buzzer_channel);
  }
  else if (dist >= 5 && dist < 10)
  {
    tone(buzzer, NOTE_B4, 100, buzzer_channel);
  }
  else if (dist >= 10 && dist < 15)
  {
    tone(buzzer, NOTE_C4, 100, buzzer_channel);
  }
  else if (dist >= 15 && dist < 20)
  {
    tone(buzzer, NOTE_D4, 100, buzzer_channel);
  }
  else if (dist >= 20 && dist < 25)
  {
    tone(buzzer, NOTE_E4, 100, buzzer_channel);
  }
  else if (dist >= 25 && dist < 30)
  {
    tone(buzzer, NOTE_F4, 100, buzzer_channel);
  }
  else if (dist >= 30 && dist < 35)
  {
    tone(buzzer, NOTE_G4, 100, buzzer_channel);
  }
  else if (dist >= 35 && dist < 40)
  {
    tone(buzzer, NOTE_A5, 100, buzzer_channel);
  }
}
