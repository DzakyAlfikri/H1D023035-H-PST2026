#include <servo.h>

Servo drsServo:

const int buttonPin = 3;
const int ledPin = 11;
const int servoPin = 9;

bool drsActive = false;

void setup(){
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  drsServo.attach(servoPin);

  drsServo.write(0);
  digitalWrite(ledPin, LOW);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
    
    drsServo.write(90);
    digitalWrite(ledPin, HIGH);

    Serial.println("DRS: ACTIVE");

    delay(500);
  }

  else{
    drsServo.write(0);
    digitalWrite(ledPin, LOW);

    Serial.println("DRS: CLOSED");

    delay(500);
  }
}