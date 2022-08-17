const char IN1 = 8;
const char IN2 = 9;
const char IN3 = 10;
const char IN4 = 11;

char dly = 4;

void stepper(char a,char b,char c,char d) {
  digitalWrite(IN1,a);
  digitalWrite(IN2,b);
  digitalWrite(IN3,c);
  digitalWrite(IN4,d);
}

void clockwise(int cnt) {
  for (int i = 0; i < cnt; i++) {
    stepper(1,0,0,0); delay(dly);
    stepper(1,1,0,0); delay(dly);
    stepper(0,1,0,0); delay(dly);
    stepper(0,1,1,0); delay(dly);
    stepper(0,0,1,0); delay(dly);
    stepper(0,0,1,1); delay(dly);
    stepper(0,0,0,1); delay(dly);
    stepper(1,0,0,1); delay(dly);
  }
}

void counterClockwise(int cnt) {
  for (int i = 0; i < cnt; i++) {
    stepper(0,0,0,1); delay(dly);
    stepper(0,0,1,1); delay(dly);
    stepper(0,0,1,0); delay(dly);
    stepper(0,1,1,0); delay(dly);
    stepper(0,1,0,0); delay(dly);
    stepper(1,1,0,0); delay(dly);
    stepper(1,0,0,0); delay(dly);
    stepper(1,0,0,1); delay(dly);
  }
}

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  clockwise(128); // 128 = 90 64 = 45 256 = 180 512 = 360
}

void loop() {
  
}
