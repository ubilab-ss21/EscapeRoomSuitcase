#include <Arduino.h>
#include "mqtt.h"


void helpLoad(int hintNumber) {
    switch(hintNumber) {
        case 0: sendMessage(pubTopic, "Display", "Clap your", "Hands");
        break;
        case 1: sendMessage(pubTopic, "Display", "Load the battery", "clap your Hands!");
        break;
    }
}

void helpForce(int hintNumber) {
    switch(hintNumber) {
        case 0: sendLED(pubTopic, "5", "On");
        sendLED(pubTopic, "9", "On");
        break;
        case 1: sendMessage(pubTopic, "Display", "Press the Buttons", "");
        break;
        case 2: sendMessage(pubTopic, "Display", "five LEDs should", "be on");     
        break;   
    }
}

void helpTheremin(int hintNumber) {
    switch(hintNumber) {
        case 0: sendLED(pubTopic, "8", "On");
        break;
        case 1: sendMessage(pubTopic, "Display", "find the", "eyes");
        break;
        case 2: sendMessage(pubTopic, "Display", "numbers on the", "display = distance"); 
        break;
    }
}

void helpMealy(int hintNumber) {
    switch(hintNumber) {
        case 0: sendLED(pubTopic, "6", "On");
        sendLED(pubTopic, "7", "On");
        break;
        case 1: sendMessage(pubTopic, "Display", "try the switches", "");
        break;
        case 2: sendMessage(pubTopic, "Display", "copy the lights", ""); 
        break;        
    }
}

void sendHelp(int taskNumber, int hintNumber){
    Serial.println("interrupt triggered");
    if (hintNumber > 2 ) {
        sendMessage(pubTopic, "Display", "No more Help", "try harder");
    } else {
        switch(taskNumber) {
            case 0: helpLoad(hintNumber);
            break;
            case 1: helpForce(hintNumber);
            break;
            case 2: helpTheremin(hintNumber);
            break;
            case 3: helpMealy(hintNumber);
            break;
        }
    } 
    hintNumber++;
    Serial.printf("hintnumber: %d \n",hintNumber);
    
}


