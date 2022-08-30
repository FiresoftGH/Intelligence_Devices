#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p wireless(7,8);
String message = "";
int count=0;
int i=0;

void setup (){
  delay (150);
  Serial.begin (9600) ;
  SPI.begin();
  SPI.setBitOrder(MSBFIRST) ;
  wireless.channel (85); //0-127
  wireless.TXaddress("WORD"); //
  wireless.RXaddress("WORD"); //
  wireless.init();
}

void loop(){
  String data = "Arduino Wireless Data : "+String (i);
  wireless.txPL(data) ;
  wireless.send(FAST) ;
  Serial.println (data) ;
  delay(2000);
  i++;
  //if(wireless.available()) {
  //  wireless.read();
  //  wireless.rxPL (message) ;
  //  Serial.println (message) ;
  //  message = "";
  //}
}
