char butt_state;
char num;
void setup() {
  pinMode(2,INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  randomSeed(analogRead(0));
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
}
char getNum() {
  char number;
  number = random(0,9);
  return number;
}
void loop() {
  butt_state = digitalRead(2);
  if (butt_state == HIGH); {
    num = getNum();
      if (num == 0) {
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
        digitalWrite(8,HIGH);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        delay(1000);
        }
      else if (num == 1) {
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
        digitalWrite(9,HIGH);
        digitalWrite(10,LOW);
        delay(1000);
        }
      else if (num == 2) {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,HIGH);
        digitalWrite(8,HIGH);
        digitalWrite(9,LOW);
        digitalWrite(10,HIGH);
        delay(1000);
        }
      else if (num == 3) {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,HIGH);
        digitalWrite(8,LOW);
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);
        delay(1000);
        }
      else if (num == 4) {
       digitalWrite(4,LOW);
       digitalWrite(5,HIGH);
       digitalWrite(6,HIGH);
       digitalWrite(7,HIGH);
       digitalWrite(8,LOW);
       digitalWrite(9,HIGH);
       digitalWrite(10,LOW);
       delay(1000);
       }
      else if (num == 5) {
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
       digitalWrite(6,LOW);
       digitalWrite(7,HIGH);
       digitalWrite(8,LOW);
       digitalWrite(9,HIGH);
       digitalWrite(10,HIGH);
       delay(1000);
       }
      else if (num == 6) {
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
       digitalWrite(6,LOW);
       digitalWrite(7,HIGH);
       digitalWrite(8,HIGH);
       digitalWrite(9,HIGH);
       digitalWrite(10,HIGH);
       delay(1000);
       }
      else if (num == 7) {
       digitalWrite(4,HIGH);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       digitalWrite(7,LOW);
       digitalWrite(8,LOW);
       digitalWrite(9,HIGH);
       digitalWrite(10,LOW);
       delay(1000);
       }
      else if (num == 8) {
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
       digitalWrite(6,HIGH);
       digitalWrite(7,HIGH);
       digitalWrite(8,HIGH);
       digitalWrite(9,HIGH);
       digitalWrite(10,HIGH);
       delay(1000);
       }
      else if (num == 9) {
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
       digitalWrite(6,HIGH);
       digitalWrite(7,HIGH);
       digitalWrite(8,LOW);
       digitalWrite(9,HIGH);
       digitalWrite(10,HIGH);
       delay(1000);
       }
  }
}
