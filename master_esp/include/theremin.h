#include <Arduino.h>
#include <PubSubClient.h>
#include <string.h>

String char2string(char* charArr);
void thereminTask();

//task teremin
float duration_us, distance_cm;
int sequence_solution[5] = {1, 3, 5, 2, 4};
int sequence_current[5] = {0, 0, 0, 0, 0};
int level_previous = 0;
int count_theremin = 0;
int winningTheremin = 0; //task theremin not finished (0) or finished (1)

int check_solution();
void update_sequence(int tone_new);

void thereminTask(int trigPin, int echoPin, int buzzer){
  sendMessage(pubTopic, "Display", "1 3 5 2 4", "");
  delay(2000);
  while(!winningTheremin){

    // Generate 10-microsecond pulse to TRIG pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure duration of pulse from ECHO pin
    duration_us = pulseIn(echoPin, HIGH);

    // Calculate the distance
    distance_cm = 0.017 * duration_us;

    int distance_int = static_cast<int>(distance_cm);
    if(distance_int>24){
      distance_int = 0;
    }
    int level_current = map(distance_int, 0, 20, 0, 6);
    bar.setLevel(level_current);
    bar2.setLevel(level_current);
    bar3.setLevel(level_current);
    if(level_current == level_previous){
        count_theremin++;
    }else{
        count_theremin = 0;
    }
    level_previous = level_current;
    if(count_theremin >= 10){
        //distance hold long enough
        update_sequence(level_current);
        if (check_solution())
        {
            winningTheremin = 1;
        }
        count_theremin = 0;
        level_previous = 0;
    }
    delay(150);
  }
}

int check_solution()
{
    for (int i = 0; i < 5;i++)
    {
      if (sequence_solution[i] != sequence_current[i])
      {
        return 0; //solution wrong
      }
    }
    return 1; //task solved solution correct
}

void update_sequence(int tone_new){
    for (int i = 0; i < 4;i++){
        sequence_current[i] = sequence_current[i + 1];
    }
    sequence_current[4] = tone_new;
    String sequenceStr = "now: ";
    for(int i=0; i<5;i++){
      sequenceStr = sequenceStr + " ";
      sequenceStr = sequenceStr + String(sequence_current[i]);
    }
    sendMessage(pubTopic, "Display", "1 3 5 2 4", sequenceStr);
    for(int i = 0; i<6; i++){
      bar.setLevel(0);
      bar2.setLevel(0);
      bar3.setLevel(0);
      delay(300);
      bar.setLevel(tone_new);
      bar2.setLevel(tone_new);
      bar3.setLevel(tone_new);
      delay(300);
    }
}