void setup() {
  Serial.setup(9600);
  pinMode(11, OUTPUT);
}

void loop() {
  int val = analogRead(A0);
  unsigned char dim = map(val,0,1023,0,255);
  analogWrite(11,dim);
}
