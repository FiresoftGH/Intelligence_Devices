#include <Servo.h>
int a = 0;
int b = 0;
int c = 180;
Servo servo;

void setup() {
  servo.attach(9);
  pinMode(2,INPUT);
  attachInterrupt(digitalPinToInterrupt(2),ButtonClicked,RISING);
  Serial.begin(9600);
}

void loop() {
  
}

void ButtonClicked() {
  if (a < 180){
    b = 0;
  }
  else if (a >= 180){
    b = 1;
  }
  switch(b){
    case 0:
      a = a+45;
      Serial.println(a);
      servo.write(a);
      break;
    case 1:
      c = c-45;
      Serial.println(c);
      servo.write(c);
      if (c <= 0){
        a = 0;
        c = 180;
      }
      break;
  }
}
