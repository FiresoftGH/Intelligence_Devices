#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p wireless(7, 8);

char red = 5;
char button = 3;
char Status = 0;
String receive = "";

void setup() {
  char pwd[] = "WORD";
  delay(150);
  pinMode(red, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  attachInterrupt(digitalPinToInterrupt(button), interrupt_task, FALLING);
  wireless.channel(69);
  wireless.TXaddress(pwd);
  wireless.RXaddress(pwd);
  wireless.init();
}

void loop() {

  if (wireless.available()) {
    wireless.read();
    wireless.rxPL(receive);
    if (receive == "wirelesssend") {
      Serial.println("Message Received");
      Status = ! Status;
      digitalWrite(red, Status);
    }
    receive = "";
  }
  delay(200);
}

void interrupt_task() {
    String transmit = "wirelessreceive";
    wireless.txPL(transmit);
    wireless.send(FAST);
    Serial.println("Message Sent");
}
