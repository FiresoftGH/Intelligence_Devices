bool A,B,C,D;
void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  Serial.begin(9600);
}
byte i = 0;
void loop() {
  A = bitRead(i,3); B = bitRead(i,2); C = bitRead(i,1);
  digitalWrite(3,C); digitalWrite(4,B); digitalWrite(5,A);
  delay(1000);
  Serial.print("|"); Serial.print(A); Serial.print(","); Serial.print(B);
  Serial.print(","); Serial.print(C); 
  Serial.print(","); Serial.print(D); 
  Serial.print("|"); Serial.println("");
  if (i > 7) {
    i = 0;
  }
  else {
    i++;
  }
}
