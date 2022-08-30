const char IN3 = 2;
const char IN4 = 3;
int count = 0;
int value;
const int button = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(button,INPUT);
  Serial.begin(9600);
} 

void loop() {
  // put your main code here, to run repeatedly:
  int butt_state;
  butt_state = digitalRead(button);
  value = analogRead(A0)/4;
  if (butt_state == HIGH){
    count+=1;
    Serial.println(butt_state);
    delay(100);
  }
  if (count == 0) {
    // clockwise
    Serial.println(butt_state);
    analogWrite(IN3, value);
    digitalWrite(IN4, 0);
   // delay(3000);
  }
  else if (count == 1) {
    // counterclockwise
    Serial.println(butt_state);
    digitalWrite(IN3, 0);
    analogWrite(IN4, value);
   // delay(3000);
  }
  else {
    count = 0;
  }
 
}
