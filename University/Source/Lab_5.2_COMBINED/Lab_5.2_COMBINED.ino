#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p wireless(7,8);
String message = "";

int i=0;

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
}

void loop(){
//  String data = "Arduino Wireless Data : "+String (i);
//  wireless.txPL(data) ;
//  wireless.send(FAST) ;
//  Serial.println(data) ;
//  delay(2000);
//  i++;
  if(wireless.available()) {
    wireless.read();
    wireless.rxPL (message) ;
    Serial.println (message) ;
    message = "";
  }
}
