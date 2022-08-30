char status;
char count;

void setup() {
  pinMode(2,INPUT);
  attachInterrupt(digitalPinToInterrupt(2),button_event,RISING);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  count == 0;
}

void loop(){
  digitalWrite(4,HIGH);
  delay(1000);
  digitalWrite(4,LOW);
  delay(1000);
}

void button_event(){
status = digitalRead(2);
  if (status == HIGH){
      if (count == 0) {
        digitalWrite(5,HIGH);
        count = 1; }
      else if (count == 1) {
        digitalWrite(5,LOW);
        count = 0; }
  }
}
