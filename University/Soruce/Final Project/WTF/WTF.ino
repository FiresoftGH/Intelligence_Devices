void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  delay(300);
}

//void serialEvent(){
//  String command = Serial.readString();
//  command.replace("\n","");
//  Serial.println(command);
//  if (command == "on"){
//    digitalWrite(13,HIGH);
//  }
//  else if (command == "off"){
//    digitalWrite(13,LOW);
//  }
//}
