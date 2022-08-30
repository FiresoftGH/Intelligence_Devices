bool A,B,C,D,E;
void setup() {
  //pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  Serial.begin(9600);
}
byte i = 0;
void loop() {
  A = bitRead(i,2); B = bitRead(i,1); C = bitRead(i,0);
  digitalWrite(3,C); digitalWrite(4,B); digitalWrite(5,A); D = digitalRead(6); E = digitalRead(7);
  delay(1000);
  Serial.print("|"); Serial.print(A); Serial.print(","); Serial.print(B);
  Serial.print(","); Serial.print(C); 
  Serial.print(","); Serial.print(D);
  Serial.print(","); Serial.print(E);
  Serial.print("|"); Serial.println("");
  if (i >= 7) {
    i = 0;
  }
  else {
    i++;
  }
}
