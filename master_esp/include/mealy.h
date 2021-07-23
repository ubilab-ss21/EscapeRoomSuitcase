#include <Arduino.h>

void writeMuster();
void rightRotatebyOne();
bool checkMuster();
void mealyAutomat();
static bool succes = false;

int shiftPin = 15; //SHCP
int storePin = 13; //STCP
int dataPin = 12; //SER

int shiftInput = 26;
int serInput = 25;
int muster[8] = {0, 0, 0, 0, 0, 0, 0,0};
bool musterLoesung[8] = {1,1,0,1,0,1,1,0};
bool trigger = true;

//globale Variable 
//dient als Index für die aktive LED
//im Lauflicht
int activeIndex = -1;

void setupMealy() {
  Serial.begin(9600);
  //setzen der Pins als Ausgang    
  pinMode(storePin, OUTPUT);
  pinMode(serInput, INPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(shiftInput, INPUT);
  //attachInterrupt(shiftInput, isr, CHANGE);
  //setzen des Pins am digitalen Pin D8
  //auf LOW quasi Status "AUS"
  digitalWrite(storePin, LOW);
}

void mealyAutomat()
{
  static bool previousShiftInput = true;
  while (succes == false){

    if (buttonPressed == true) {
      sendHelp(3,hintNumber++);
      delay(500);
      buttonPressed = false;
    }

    if (digitalRead(shiftInput) != previousShiftInput){
      
      previousShiftInput = digitalRead(shiftInput);
      trigger = true;      
    }

    if (trigger == true)
    {
      Serial.println("Shift triggered");
      writeMuster();
      trigger = false;
    }
    //eine kleine Pause von 150ms.
    delay(100); 
  }
  hintNumber = 0;
}

void writeMuster() {
  //den aktiven Index um 1 erhöhen
  ++activeIndex;
  //Das "Ausgangsmuster" wo alle LEDs 
  //deaktivert sind.
  //Wenn der aktive Index größer als die
  //maximale Anzahl der LEDs ist, dann...
  if (activeIndex > 7) {
    activeIndex = 0; //den activen Index auf 0 setzen.
    //Dieses sorgt dafür dass, das Lauflicht wieder von vorne beginnt.
  }
  //den Wert im Array manipulieren
  //es wird an der Stelle "x" im Array eine 1 gesetzt
  muster[0] = digitalRead(serInput);;
  Serial.println(muster[0]);
  //Von 0 bis 8 mache...
  for (int i = 0; i < 8; i++) {
    digitalWrite(shiftPin, LOW);
    digitalWrite(dataPin, muster[i]);
    digitalWrite(shiftPin, HIGH);
  }
  //schreiben der Daten
  digitalWrite(storePin, HIGH);
  delay(20); //eine kleine Pause
  //zurücksetzen 
  digitalWrite(storePin, LOW);
  succes = checkMuster();
    if (succes == true){
    delay(1000);
    for ( int a = 0; a < 4; a++){
      delay(100);
      for (int i = 0; i < 8; i++) {
        if(muster[i]==1){
          muster[i]=0;
        } else {
          muster[i] = musterLoesung[i];
        }
        digitalWrite(shiftPin, LOW);
        digitalWrite(dataPin, muster[i]);
        digitalWrite(shiftPin, HIGH);
      }
      //schreiben der Daten
      digitalWrite(storePin, HIGH);
      //zurücksetzen 
      digitalWrite(storePin, LOW);
    }
  }
  rightRotatebyOne();
}

bool checkMuster()
{
  for(int i = 0; i < 8; i++) {
    if( muster[i] == musterLoesung[i] )
      continue;
    else {
      printf("Unterschied an Position %d\n",i);
      return false;
    }
  }
  Serial.println("right");
  return true;
}

void rightRotatebyOne()
{
  int i;
  for (i = 8-1; i > 0; i--)
     muster[i] = muster[i-1];
}

