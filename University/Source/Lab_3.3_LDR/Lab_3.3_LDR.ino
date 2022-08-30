char ldrToggle = 0;
char blinkToggle = 0;
char ldrBlink = 0;
void setup() {
  Serial.begin(9600); //Begin the Serial comunication bandwidth
  pinMode(13,OUTPUT); 
  Serial.println("--Control LED Mode--"); //Setup for the User Interface
  Serial.println(">> Select LED Mode (on,off,ldr,blink,ldrblink):");

}
//The loop below has the logic of controlling the complex task such as the LDR mode.
void loop() {
  if (ldrToggle == 1){
    int a = analogRead(A0);
    if (a < 750) { //Midpoint of the resistance to make the led light-sensitive
      digitalWrite(13,LOW);
    }
    else {
      digitalWrite(13,HIGH);
    }
  }
  else if (blinkToggle == 1) {
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500); }
  else if (ldrBlink == 1) {
    int b = analogRead(A0);
    if (b < 750) {
      digitalWrite(13,LOW);
    }
    else {
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
      delay(500); }
    }
  }

void serialEvent(){
  String command = Serial.readString();
  command.replace("\n","");
  Serial.println(command);
  if (command == "on"){
    digitalWrite(13,HIGH);
    Serial.println("LED is on");
    ldrToggle = 0;
    blinkToggle = 0;
    ldrBlink = 0;
  }
  else if (command == "off") {
    digitalWrite(13,LOW);
    Serial.println("LED is off");
    ldrToggle = 0;
    blinkToggle = 0;
    ldrBlink = 0;
  }
  else if (command =="ldr"){
    ldrToggle = 1;
    blinkToggle = 0;
    ldrBlink = 0;
    Serial.println("LDR is controlling the LED");
  }
  else if (command == "blink"){
    ldrToggle = 0;
    blinkToggle = 1;
    ldrBlink = 0;
    Serial.println("LED is blinking");
  }
  else if (command == "ldrblink"){
    ldrToggle = 0;
    blinkToggle = 0;
    ldrBlink = 1;
    Serial.println("LDR is controlling the LED (Blink mode)");
  }
  else {
    Serial.println("Wrong command");
  }
}
