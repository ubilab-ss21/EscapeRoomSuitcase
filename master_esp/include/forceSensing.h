#include <Arduino.h>
#include <Grove_LED_Bar.h>

Grove_LED_Bar bar(9, 10, false);
Grove_LED_Bar bar2(33, 32, false);
Grove_LED_Bar bar3(27, 14, false);
//Grove_LED_Bar bar4 (19, 23, false);

//Force Pins
int fsrAnalogPin = 34; // FSR is connected to analog 0
int fsrAnalogPin2 = 35;
//int fsrAnalogPin3 = 38;
int fsrReading; // the analog reading from the FSR resistor divider
int fsrReading2;
bool level;
int leveld = 0;

void taskForceSensing()
{
  bar.setLevel(0);
  bar2.setLevel(0);
  bar3.setLevel(0);
  bar4.setLevel(0);
  while (level == false)
  {
    if (buttonPressed == true) {
      sendHelp(1,hintNumber++);
      delay(500);
      buttonPressed = false;
    }
    delay(50);
    fsrReading = analogRead(fsrAnalogPin);
    fsrReading = map(fsrReading, 500, 4095, 0, 10);
    bar.setLevel(fsrReading);
    fsrReading2 = analogRead(fsrAnalogPin2);
    fsrReading2 = map(fsrReading2, 500, 4095, 0, 10);
    bar2.setLevel(fsrReading2);
    bar3.setLevel(fsrReading2);
    if (fsrReading2 == 5 and fsrReading == 5 and leveld < 15)
    {
      //sendMessage(pubTopic, "Level", "");
      Serial.println("-----------LEVELD");
      Serial.println(leveld);
      leveld++;
      delay(200);
    }
    if (leveld == 15) {

      //sendMessage(pubTopic, "Task1 Done");
      level = true;
    }
  }
  hintNumber = 0;
}
