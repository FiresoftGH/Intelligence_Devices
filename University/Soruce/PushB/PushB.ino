char status; 
char schwoop;

void setup() 
{
  pinMode(13,OUTPUT);
  pinMode(2, INPUT);
  schwoop = 0;
}

void loop() 
{
 status = digitalRead(2);
  if (status == 0) {
    if (schwoop) {
      schwoop = 0;
    } else {
      schwoop = 1;
    }
  }
  if (schwoop) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  delay(100);
  }
