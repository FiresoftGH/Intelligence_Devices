char status;
char count;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  count == 0;
}

void loop()
{
  status = digitalRead(2);
  if (status == HIGH){
      if (count == 0) {
        digitalWrite(13,HIGH);
        count = 1; }
      else if (count == 1) {
        digitalWrite(13,LOW);
        digitalWrite(12,HIGH);
        count = 2; }
      else if (count == 2) {
        digitalWrite(12,LOW);
        digitalWrite(11,HIGH);
        count = 3; }
      else if (count == 3) {
        digitalWrite(11,LOW);
        count = 0;
      }
   delay(1000);
  }
}
             
                    
                    
            
