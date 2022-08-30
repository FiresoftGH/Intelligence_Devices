#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p wireless(7,8);
String message = "";
int buttstat = 0;
int count=0;

void setup (){
  char pwd[] = "WORD";
  delay(150);
  Serial.begin (9600) ;
  SPI.begin();
  SPI.setBitOrder(MSBFIRST) ;
  wireless.channel (85); //0-127
  wireless.TXaddress(pwd); //Password for Transmitter
  wireless.RXaddress(pwd); //Password for Reciever
  wireless.init();
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
}

void loop(){
  if (digitalRead(3)== LOW){
    if (count = 0){
      digitalWrite(2,HIGH);
      count++;
    }
    else if (count = 1){
      digitalWrite(2,LOW);
      count++;
    }
    else if (count = 2){
      count = 0;
    }
  }
    String data = "Clicked";
    wireless.txPL(data);
    wireless.send(FAST);
//    delay(1000);
  }
//  else if(wireless.available()) {
//    wireless.read();
//    wireless.rxPL(message);
//    Serial.println(message);
//    digitalWrite(2,HIGH);
//    message = "";
//  }
