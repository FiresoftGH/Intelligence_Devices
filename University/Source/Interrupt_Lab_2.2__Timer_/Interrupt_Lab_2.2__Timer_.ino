#include <TimerOne.h>
char stat;
char count;
int ledState = LOW;
volatile unsigned long blinkCount = 0; //volatile for shared value (could be refreshed)
void setup() {
  Timer1.initialize(1000000); //Set timer
  attachInterrupt(digitalPinToInterrupt(2),button_event,RISING);
  Timer1.attachInterrupt(blinkLED); //FUNCTION FOR THE THING
  pinMode(2,INPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  count == 0;
}
void blinkLED(void)
{
  if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;  // increase when LED turns on
  } else {
    ledState = LOW;
  }
  digitalWrite(13, ledState);
}

void loop() {
  
}

void button_event(){
stat = digitalRead(2);
  if (stat == HIGH){
      if (count == 0) {
        digitalWrite(12,HIGH);
        count = 1; }
      else if (count == 1) {
        digitalWrite(12,LOW);
        count = 0; }
  }
}
