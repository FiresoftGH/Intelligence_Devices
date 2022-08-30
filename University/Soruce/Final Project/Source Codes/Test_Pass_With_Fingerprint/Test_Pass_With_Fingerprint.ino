//LCD Libararies

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Servo Libaries
#include <ESP32Servo.h>

Servo serv; //Servo 1 Variable
Servo serv2; //Servo 2 Variable
int unlock; //Servo status variable
int islock = 1; //LDR 1 for servo 1 status tracking
int islock2 = 1; //LDR 2 for servo 2 status tracking

// Keypad Libaries
#include <Keypad.h>

//Fingerprint
#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

// Failed Attempts
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
byte rowPins[ROWS] = {16, 4, 0, 2};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
byte colPins[COLS] = {19, 18, 5, 17}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String passcode; // Password Variable
String input_passcode; //Password Input
String lcdinput;
char lcdinputchar;


int setuptoggle = 0; //Setup the password Toggle status variable
int usabletoggle = 0; //Ready to use status variable
int resettoggle = 0; // Reset the pasword variable
int failedcount = 0; // Number of failed attempts
int templocktoggle = 0; // Permanent lock toggle


//Fingerprint Initialization code and Variables

uint8_t id; //Actual id input variable
int fingerscanstate = 0; //state variable
int fingersetup = 0; //setup variable
int fingerready = 0; //ready to use variable
String input_id; //id input variable
int input_idcount = input_id.toInt() == 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup(){
  Serial.begin(115200);
  //Servo Setup Codes
  serv.attach(25); //Servo 1 Pin
  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on LCD backlight
  pinMode(23,OUTPUT); //Buzzer Pin
  lcd.setCursor(0,0);
  lcd.print("Press 'C' to start");
  serv.write(180);
  serv2.write(180);
}

//Fingerprint main codes

void serialEvent(){
  String amogus = Serial.readString();
  amogus.replace("\n","");
  if (amogus == "amogus"){
    fingerscanstate=1;
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintID() {
  if (fingerscanstate==0){
    uint8_t p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No finger detected");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
  

  // OK success!
    p = finger.image2Tz();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    
    }
  
    // OK converted!
    p = finger.fingerSearch();
    if (p == FINGERPRINT_OK) {
      Serial.println("Found a print match!");
      Serial.println("servo goes brrr!");
      serv.write(180);
      //serv2.write(180);
      Serial.println("spin to 90");
      Serial.println("unlocked");
      delay(2000);
      serv.write(0);
      islock = 0;
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_NOTFOUND) {
      Serial.println("Did not find a match");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  
    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
  
    return finger.fingerID;
  }
}
  
  // returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  if(fingerscanstate==0){
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
  
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
  
    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return -1;
  }
}
uint8_t getFingerprintEnroll() {
  if (fingerscanstate==1){
    int p = -1;
    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
      }
    }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    fingerscanstate=0;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
  }
}



//LCD/Keypad Functions

void temporarylock() {
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

void fingersetuplcd(){
  lcd.setCursor(0,1);
  lcd.print("Place your finger on");
  lcd.setCursor(0,2);
  lcd.print("The scanner");
}

void fingerlcdenroll(){
  lcd.setCursor(0,0);
  lcd.print("Fingerprint id:");
  lcd.setCursor(0,1);
  lcd.print(id);
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
  if (unlock = 0){
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
  if (unlock == 1){
    serv.write(0);
    serv2.write(0);
    delay(2000);
    unlock = 0;
    islock = 0;
    islock2 = 0;
  }
  if (islock == 0){
    int val = analogRead(34); //Sensitivity
    int val2 = analogRead(35);
    if (val>750 && val2>750){ //Sensitivity
      delay(2000);
      serv.write(180);
      islock = 1;
      unlock = 0;
    }
  }
  char key = keypad.getKey();
  if (key){
    if(key == '*' && templocktoggle == 0){
      input_passcode = ""; // clear input passcode
      lcd.clear();
      lcdinput = "";
      lcdinputchar = ' ';
      if (setuptoggle == 1 && templocktoggle == 0){
        setuplcd();
      }
      else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && templocktoggle == 0){
        usablelcd();
      }
      else if (resettoggle == 1 && templocktoggle == 0){
        resetlcd();
      }
    }
     else if (key == 'B' && passcode != "" && templocktoggle == 0){
      resettoggle = 1;
      resetlcd();
      input_passcode = "";
     }
     else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && templocktoggle == 0){
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
          unlock = 1;
          failedcount == 0;
          delay(1000);
          lcd.clear();
          lcdinput = "";
          lcdinputchar = ' ';
          usablelcd();
          //Website
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
     else if (key == 'A' && templocktoggle == 0 && fingerready == 1){
      setuptoggle = 1;
      lcd.clear();
      setuplcd();
      input_passcode = "";
     }
     else if (key == 'C' && templocktoggle == 0){
      fingersetup = 1;
      lcd.clear();
      fingersetuplcd();
      fingerscanstate = 1;
     }
     else if (fingerscanstate == 1 && fingerready == 0){
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D'){
        input_id += key;
        lcd.clear();
        fingersetuplcd();
      }
      if (key == '#'){
        if (input_idcount == 0 || input_idcount > 127){
          lcd.setCursor(0,3);
          lcd.print("Invalid ID");
        }
        else {
          id = input_idcount;
          input_id = "";
          fingerready = 1;
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Your fingerprint ID:");
          lcd.print(id);
          lcd.setCursor(0,2);
          lcd.print("Press 'A' to continue");
          lcd.setCursor(0,3);
          lcd.print("'D' to enroll more");
          while (!  getFingerprintEnroll() );
          if (key == 'D'){
            lcd.clear();
            fingersetuplcd();
          }
          else {
            
          }
        }
      }
     }
     else if (resettoggle == 1 && templocktoggle == 0){
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
        delay(1000);
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
      temporarylock();
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
