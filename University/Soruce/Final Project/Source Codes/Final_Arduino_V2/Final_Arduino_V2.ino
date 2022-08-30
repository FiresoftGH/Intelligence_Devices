//LCD Libaries

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Servo Libaries
#include <Servo.h>

Servo serv; //Servo 1 Variable

//int unlock; //Servo status variable
int islock = 1; //LDR 1 for servo 1 status tracking
//int islock2 = 1; //LDR 2 for servo 2 status tracking

// Keypad Libaries
#include <Keypad.h>

// Keypad Pre-Setup Code

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// connect the pins from right to left
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String passcode; // Password Variable
String input_passcode; //Password Input
String lcdinput;
char lcdinputchar;

int passlen = passcode.length(); //Passcode length variable
int inputlen = input_passcode.length(); //Input length variable

int setuptoggle = 0; //Setup the password Toggle status variable
int usabletoggle = 0; //Ready to use status variable
int resettoggle = 0; // Reset the pasword variable
int failedcount = 0; // Number of failed attempts
int permlocktoggle = 0; // Permanent lock toggle

void setup(){
  Serial.begin(115200);
  //Servo Setup Codes
  serv.attach(10); //Servo 1 Pin
  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on LCD backlight
  pinMode(12,OUTPUT); //Buzzer Pin
  lcd.setCursor(0,0);
  lcd.print("Press 'C' to start");
  serv.write(180);
}
//lcd.print("AAAAAAAAAAAAAAAAAA" Function (Temporary) (Keypad)

void permanentlock() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wait 5 Minutes to");
  lcd.setCursor(0,1);
  lcd.print("unlock your drawer");
  lcd.setCursor(0,2);
  delay(3000);
  failedcount = 0;
  lcd.clear();
}

void setuplcd(){
  lcd.setCursor(0,1);
  lcd.print("Setup Initialized");
  lcd.setCursor(0,2);
  lcd.print("Passcode:");
}

void usablelcd(){
  lcd.setCursor(0,0);
  lcd.print("Status:");
  lcd.setCursor(7,0);
  if (islock = 0){
    lcd.print("Unlocked");
    }
    else {
      lcd.print("Locked");
      }
   lcd.setCursor(0,1);
   lcd.print("Passcode:"); 
}

void resetlcd(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Reset Initialized");
  lcd.setCursor(0,2);
  lcd.print("Old Passcode:");
  input_passcode = "";
}

void loop(){
  //Servo Main codes
  if (islock == 0){
    int val = analogRead(A0); //Sensitivity
    
    if (val < 800 ){ //Sensitivity
      delay(2000);
      serv.write(180);
      islock = 1;
    }
  }
 
// Keypad Main codes

  char key = keypad.getKey();
    if (key){
      Serial.println(key);
      if(key == '*' && permlocktoggle == 0){
        input_passcode = ""; // clear input passcode
        lcd.clear();
        lcdinput = "";
        lcdinputchar = ' ';
        if (setuptoggle == 1 && permlocktoggle == 0){
          setuplcd();
        }
        else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && permlocktoggle == 0){
          usablelcd();
        }
        else if (resettoggle == 1 && permlocktoggle == 0){
          resetlcd();
        }
      }
       else if (key == 'B' && passcode != "" && permlocktoggle == 0){
        resettoggle = 1;
        resetlcd();
        input_passcode = "";
       }
       else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && permlocktoggle == 0){
          usablelcd();
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
          lcd.setCursor(9,1);
          lcd.print(lcdinput);
        }
        if(key == '#'){
          if (input_passcode == passcode){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Passcode is correct");
            input_passcode = "";
            islock = 1;
            serv.write(0);
            failedcount == 0;
            delay(1000);
            lcd.clear();
            lcdinput = "";
            lcdinputchar = ' ';
            usablelcd();
          }
          else {
            lcd.setCursor(0,2);
            failedcount += 1;
            lcd.print("Wrong Passcode");
            lcd.setCursor(0,3);
            lcd.print("Failed count:");
            lcd.print(failedcount);
            input_passcode = ""; // clear input password
            lcd.setCursor(9,1);
            delay(1000);
            lcd.clear();
            usablelcd();
            lcdinput = "";
            lcdinputchar = ' ';
              }
        }
       }
       else if (key == 'C' && permlocktoggle == 0){
        setuptoggle = 1;
        lcd.clear();
        setuplcd();
        input_passcode = "";
        }
       else if (resettoggle == 1 && permlocktoggle == 0){
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
          lcd.setCursor(13,2);
          lcd.print(lcdinput);
        }
        if (key == '#'){
          if (input_passcode == passcode){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Correct Password");
            lcd.setCursor(0,2);
            lcd.print("Setup Initialized");
            setuptoggle = 1;
            resettoggle = 0;
            input_passcode = "";
            delay(1000);
            lcd.clear();
            lcdinput = "";
            lcdinputchar = ' ';
            lcd.setCursor(0,1);
            lcd.print("Setup Initialized");
            lcd.setCursor(0,2);
            lcd.print("Passcode:");
          }
          else {
            lcd.setCursor(0,3);
            lcd.print("Wrong Password");
            input_passcode = "";
            lcd.setCursor(13,2);
            delay(1000);
            lcd.clear();
            resetlcd();
            lcdinput = "";
            lcdinputchar = ' ';
          }
        }
       }
       else if (setuptoggle == 1){
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
          lcd.setCursor(9,2);
          lcd.print(lcdinput);
        }
        if (key == '#' && input_passcode.length() == 6){
          passcode = input_passcode;
          setuptoggle = 0;
          resettoggle = 0;
          usabletoggle = 1;
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Passcode setup done!");
          lcd.setCursor(0,2);
          lcd.print(passcode);
          input_passcode = "";
//          delay(1000);
          lcd.clear();
          usablelcd();
          lcdinput = "";
          lcdinputchar = ' ';
        }
        else if (key == '#' && input_passcode.length() != 6){
          input_passcode = "";
          lcd.setCursor(0,3);
          lcd.print("6 Digits Only");
          lcd.setCursor(9,2);
          lcdinput = "";
          lcdinputchar = ' ';
          delay(1000);
          lcd.clear();
        }
       }
      }
      else if (failedcount == 5){
        permanentlock();
      }
      else if (input_passcode.length() == 0){
        lcdinput = "*";
      }
      else if (input_passcode.length() == 1){
        lcdinput = "**";
      }
      else if (input_passcode.length() == 2){
        lcdinput = "***";
      }
      else if (input_passcode.length() == 3){
        lcdinput = "****";
      }
      else if (input_passcode.length() == 4){
        lcdinput = "*****";
      }
      else if (input_passcode.length() == 5){
        lcdinput = "******";
      }
}
