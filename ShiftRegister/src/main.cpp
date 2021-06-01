#include <Arduino.h>

void  writeMuster();
void rightRotatebyOne();



int shiftPin = 19; //SHCP
int storePin = 21; //STCP
int dataPin = 23; //DS

int shiftInput = 5;
int serInput = 9;
int muster[8] = {0, 0, 0, 0, 0, 0, 0};

bool trigerr = true;

//globale Variable 
//dient als Index für die aktive LED
//im Lauflicht
int activeIndex = -1;

void IRAM_ATTR isr() {
  trigerr = true;
}

void setup() {
  //setzen der Pins als Ausgang    
  Serial.begin(9600);
  pinMode(storePin, OUTPUT);
  pinMode(serInput, INPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  attachInterrupt(shiftInput, isr, CHANGE);
  //setzen des Pins am digitalen Pin D8
  //auf LOW quasi Status "AUS"
  digitalWrite(storePin, LOW);
}
void loop () {
  if (trigerr == true)
  {
    Serial.println("Shift trigerred");
    writeMuster();
    trigerr = false;
  }
  //eine kleine Pause von 150ms.
  delay(150); 
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
  Serial.print(muster[0]);
  Serial.print(muster[1]);
  Serial.print(muster[2]);
  Serial.print(muster[3]);
  Serial.print(muster[4]);
  Serial.print(muster[5]);
  Serial.print(muster[6]);
  Serial.print(muster[7]);
  rightRotatebyOne();
}

void rightRotatebyOne()
{
  int i;
  for (i = 8-1; i > 0; i--)
     muster[i] = muster[i-1];
}

