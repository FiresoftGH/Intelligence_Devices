#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p receiver(7,8) ;//CSN,CE
String message = "";

void setup(){
delay (150);
Serial.begin (9600);
SPI.begin();
SPI.setBitOrder (MSBFIRST) ;
receiver.channel (95); //
receiver.TXaddress("PSWRD"); //
receiver.RXaddress("PSWRD"); //
receiver.init();

//Between 0-127
//PASSWORD 5 char max
//Same as above
}
void loop() {
  if (receiver.available()) {
  receiver.read();
  receiver. rxPL (message) ;
  Serial.println (message) ;
  message = "Bruh";
  }
}
