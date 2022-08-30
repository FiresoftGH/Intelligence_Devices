#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p transmitter(7,8);

int i=0;

void setup (){
delay (150);
Serial .begin (9600) ;
SPI.begin();
SPI.setBitOrder (MSBFIRST) ;
transmitter.channel (85) ; //
transmitter.TXaddress("PSWRD"); //
transmitter.RXaddress("PSWRD"); //
transmitter. init ();

}

void loop()
{

String data = "Arduino Wireless Data : "+String (i);

transmitter .txPL (data) ;
transmitter .send (FAST) ;
delay (2000);

i++;

}
