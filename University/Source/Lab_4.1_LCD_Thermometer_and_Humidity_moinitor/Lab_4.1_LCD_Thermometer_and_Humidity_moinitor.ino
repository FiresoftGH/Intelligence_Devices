#include <LiquidCrystal.h>
#include <DHT.h>

const int rs = 12, en = 11, d4 = 5 , d5 = 4,d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

DHT dht;

void setup() {
  lcd.begin(16,2);
  lcd.display();
  dht.setup(7);

}

void loop() {
  float hum = dht.getHumidity();
  float temp = dht.getTemperature();

  lcd.setCursor(0,0);
  lcd.print(hum);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.println("C     ");

  delay(5000);

}
