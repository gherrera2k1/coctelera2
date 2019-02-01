//El siguiente sketch considera los trabajos de Sidlauskas (Sidlaukas, 2016) y diymachines.co.uk//
#include "AccelStepper.h"
const int  buttonPin = 2;   int buttonState = 0;         
int lastButtonState = 0;     int greenPin = 10;
int redPin = 9;int pos=0;
String serialBuffer = "";
String actions[30];
int counter = 0; int lastIndex = 0;
int cup = false;
AccelStepper stepper(1, 3, 5);
AccelStepper stepper2(1, 6, 11);
void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  stepper.setMaxSpeed(1100);
  stepper2.setMaxSpeed(1100);
  homeXAxis();   Serial.begin(9600);
  stepper2.setCurrentPosition(0);
}
void homeXAxis() {
  if (cup == false) {
  setColor(255, 0);
  Serial.print("Buscando el vaso...");
  buttonState = digitalRead(buttonPin); 
  while (buttonState == LOW) {
    stepper.moveTo(-100);
    stepper.setSpeed(-500);
    stepper.runSpeed();
    buttonState = digitalRead(buttonPin);
  }
  stepper.moveTo(stepper.currentPosition() + 50);
  while (stepper.distanceToGo() != 0) {
    stepper.setSpeed(150*1);
    stepper.runSpeed();
  }
  buttonState = digitalRead(buttonPin);
  while (buttonState == LOW) {
    stepper.moveTo(-100);
    stepper.setSpeed(150*-1);
    stepper.runSpeed();
    buttonState = digitalRead(buttonPin);  }
 stepper.setCurrentPosition(0);
 setColor(200, 100); 
 delay(500); } }
void setColor(int red, int green)  {
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green); 
} void loop() {
  while(Serial.available() > 0) {
    char ch = Serial.read();
    serialBuffer += ch;
    if (ch == '\n') {
      for(int i=0; i<serialBuffer.length(); i++) {
        if(serialBuffer.substring(i, i+1) == ",") {
          actions[counter] = serialBuffer.substring(lastIndex, i);
          lastIndex = i + 1;
          counter++;         }
        if(i == serialBuffer.length() - 1) {
          actions[counter] = serialBuffer.substring(lastIndex, i);
        }   }
      for(int z=0; z<30; z++) {
        if(actions[z] != "0") {
          parseInput(actions[z]);        }  }
      for(int y=0; y<30; y++) {
        actions[y] = "0";      }      
      serialBuffer = "";
      counter = 0;       lastIndex = 0;      
    }  }}
void parseInput(String input) {
  input.trim();
  byte command = input.charAt(0);
  switch(command) {
    case 'H':      homeXAxis();       break;
    case 'X':      moveXTo(input);       break;
    case 'F':       pour(input);       break;
  }}
void moveXTo(String input) {
  int pos = input.substring(1).toInt();
    Serial.print("X goes to: ");
     if(pos < 0 && pos >= -4600) {
    stepper.setAcceleration(200);
    stepper.moveTo(-pos);
      if(pos < stepper.currentPosition()) {
        stepper.setSpeed(100);
        Serial.println(stepper.distanceToGo());
      } else {         stepper.setSpeed(-100);
        Serial.println(stepper.distanceToGo());      }
    while(stepper.distanceToGo() != 0) {
      setColor(0, 255);       stepper.run();
    }  } else {
    Serial.println("La posición debe estar entre -4600 and 0");
  }}
  void pour(String input) {
  int count = 0;  int times = 0; 
  int holdDuration = 0;   int waitDuration = 0;
    for(int z=0; z<input.length(); z++) {
    byte parameter = input.substring(z, z+1).charAt(0);
    switch(parameter) {
      case 'F':
        times = getParameterValue(input, z);   break;
      case 'H':
        holdDuration = getParameterValue(input, z);  break;
      case 'W':
        waitDuration = getParameterValue(input, z);
        break;    }  }
  if(holdDuration > 0 && waitDuration > 0) {
    for(int i=0; i<times; i++) {
       setColor(125, 1255);
       stepper2.setAcceleration(150);
       stepper2.moveTo(3200);
       stepper2.setSpeed(400);
       while(stepper2.distanceToGo() != 0) {
       stepper2.run(); 
       Serial.println(stepper2.distanceToGo());     }
    delay (4000);
       stepper2.setAcceleration(-150);
       stepper2.moveTo(0);
       stepper2.setSpeed(-400);
       while(stepper2.distanceToGo() != 0) {
       stepper2.run(); 
       Serial.println(stepper2.distanceToGo());
    }         if(times - 1 > count) {
        delay(waitDuration);
      } else {         delay(2000);      }
      count++;
    }  } else {
    Serial.println("El tiempo de duración no puede ser igual o menor que 0");   }}
int getParameterValue(String input, int z) {
  for(int y=z+1; y<input.length(); y++) {
    if(input.substring(y, y+1) == " ") {
      return input.substring(z+1, y).toInt();
      break;    }
    if(y == input.length() - 1) {
      return input.substring(z+1, y+1).toInt();
    }  }}

