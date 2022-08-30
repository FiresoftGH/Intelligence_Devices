#include <LiquidCrystal.h>
float voltage = 0;
float current = 0;
float resist = 0;
const int rs = 12, en = 11, d4 = 5 , d5 = 4,d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {

Serial.begin(9600);
lcd.begin(16,2);
lcd.display();
pinMode(A0,INPUT);
}

void loop() {
  float a = analogRead(A0);
  float voltage = (a/1024)*5;
  float current = (5 - voltage)/10000;
  float resist = voltage/current;

  lcd.setCursor(0,0);
  lcd.println("Resistance:      ");
  lcd.setCursor(0,1);
  lcd.print(resist/1000);
  lcd.println(" KOhms      ");
  delay(1000);

}
