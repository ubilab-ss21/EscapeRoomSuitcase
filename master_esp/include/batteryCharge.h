#include <Arduino.h>
#include "arduinoFFT.h"
#include <Grove_LED_Bar.h>
#include "helpButton.h"

#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 4096 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
#define MAX 10

int interval=3000;
// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long myTime;

float getStdDev();
float getMean();

float counter = 1;
float rawDataArray[MAX] = {};
float dataArray[MAX] = {};

float MeanValue = 0.0;
bool charge = false;
Grove_LED_Bar bar4(19, 23, 0);  // Clock pin, Data pin, Orientation
hw_timer_t * timer = NULL;
arduinoFFT FFT = arduinoFFT();
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

void setupBatteryCharge() {
  bar4.begin();
  Serial.begin(9600); //Baud rate for the Serial Monitor
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}



void batteryCharge()
{
    static bool batterySucces = false;
    while(batterySucces == false)
    {
      
      if (buttonPressed == true) {
        sendHelp(0,hintNumber++);
        delay(500);
        buttonPressed = false;
      }
      /*Sample SAMPLES times*/
      for(int i=0; i<SAMPLES; i++)
      {
          microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
          vReal[i] = analogRead(37); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
          vImag[i] = 0; //Makes imaginary term 0 always

          /*remaining wait time between samples if necessary*/
          while(micros() < (microSeconds + samplingPeriod))
          {
            //do nothing
          }
      }
  
      /*Perform FFT on samples*/
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

      /*Find peak frequency and print peak*/
      double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      
      myTime = millis();
      //Serial.println(peak);
      Serial.println(peak);
      if ((peak > 600.0) && (peak < 1000))
      {
        if (counter >= 20){
          batterySucces = true;
        } else {
          counter++;
        }
        Serial.println(counter/2);
        bar4.setLevel(counter/2);
        previousMillis = myTime;
        delay(500);
      } else if ((unsigned long)(myTime - previousMillis) >= interval){
        if (counter == 1){
          bar4.toggleLed(counter);
        }
        if (counter > 1){
          counter --;
          bar4.setLevel(counter/2);
        }
        Serial.println("negative");
        Serial.println(counter/2);
        delay(300);
      }
    }
    for (int i = 0; i < 10; i++)
    {
          bar4.setLevel(0);
          delay(300);
          bar4.setLevel(10);
          delay(300);
    }
    hintNumber = 0;

}

