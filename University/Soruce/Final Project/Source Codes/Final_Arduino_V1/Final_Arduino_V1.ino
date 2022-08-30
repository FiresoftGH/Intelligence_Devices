//LCD Libaries

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Servo Libaries
#include <Servo.h>

Servo serv; //Servo 1 Variable

int unlock = 1; //Servo status variable
int islock = 1; //LDR 1 for servo 1 status tracking
int islock2 = 1; //LDR 2 for servo 2 status tracking

//Fingerprint Libarary

#include <Adafruit_Fingerprint.h>

uint8_t id; //ID For fingerprint

SoftwareSerial mySerial(12, 13);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Keypad Pre-Setup Code

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
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
int fingerset = 0; //Fingerprint Setup Variable
int fingerusable = 0; //Fingerprint state Variable


void setup() {
  Serial.begin(115200);
  //Servo Setup Codes
  serv.attach(10); //Servo 1 Pin
  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on LCD backlight
  pinMode(11, OUTPUT); //Buzzer Pin
  lcd.setCursor(0, 0);
  lcd.print("Press 'C' to start");
  serv.write(180);

  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  ////Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  //Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); //Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); //Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); //Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); //Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); //Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); //Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); //Serial.println(finger.baud_rate);
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//lcd.print("AAAAAAAAAAAAAAAAAA" Function (Temporary) (Keypad)

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); //Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  //Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); //Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  //Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); //Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.print("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.print("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.print("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.print("Error writing to flash");
    return p;
  } else {
    lcd.print("Unknown error");
    return p;
  }

  return true;
}

void permanentlock() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wait 5 Minutes to");
  lcd.setCursor(0, 1);
  lcd.print("unlock your drawer");
  lcd.setCursor(0, 2);
  delay(3000);
  failedcount = 0;
  lcd.clear();
}

void setuplcd() {
  lcd.setCursor(0, 1);
  lcd.print("Setup Initialized");
  lcd.setCursor(0, 2);
  lcd.print("Passcode:");
}

void usablelcd() {
  lcd.setCursor(0, 0);
  lcd.print("Status:");
  lcd.setCursor(7, 0);
  if (unlock = 0) {
    lcd.print("Unlocked");
  }
  else {
    lcd.print("Locked");
  }
  lcd.setCursor(0, 1);
  lcd.print("Passcode:");
  if (fingerusable = 0){
    lcd.setCursor(0, 2);
    lcd.print("FP Scanner");
    lcd.setCursor(0, 3);
    lcd.print("Press A to begin");
  }
  else {
    lcd.setCursor(0, 2);
    lcd.print("Place your finger");
    lcd.setCursor(0, 3);
    lcd.print("To unlock");
  }
  lcd.setCursor(3, 2);
}

void resetlcd() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Reset Initialized");
  lcd.setCursor(0, 2);
  lcd.print("Old Passcode:");
  input_passcode = "";
}

void fingersetattemptlcd(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Enter your passcode");
  lcd.setCursor(0,2);
  lcd.print("Passcode:");
}

void fingersetlcd(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Place your finger on");
  lcd.setCursor(0,2);
  lcd.print("On the scanner.");
  lcd.setCursor(0,3);
  lcd.print("Your ID:");
}

void loop() {
  //Servo Main codes
  if (islock == 0) {
    int val = analogRead(A0); //Sensitivity
    //    int val2 = analogRead(35);

    if (val < 800) { //Sensitivity
      delay(2000);
      serv.write(180);
      islock = 1;
      unlock = 0;
    }
  }

  // Keypad Main codes

  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    Serial.println(analogRead(A0));
    if (key == '*' && permlocktoggle == 0) {
      input_passcode = ""; // clear input passcode
      lcd.clear();
      lcdinput = "";
      lcdinputchar = ' ';
      if (setuptoggle == 1 && permlocktoggle == 0) {
        setuplcd();
      }
      else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && permlocktoggle == 0) {
        usablelcd();
      }
      else if (resettoggle == 1 && permlocktoggle == 0) {
        resetlcd();
      }
    }
    else if (key == 'B' && passcode != "" && permlocktoggle == 0) {
      resettoggle = 1;
      resetlcd();
      input_passcode = "";
    }
    else if (key == 'A' && passcode != "" && permlocktoggle) {
      fingerset = 1;
      fingersetattemptlcd();
      input_passcode = "";
    }
    else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && permlocktoggle == 0) {
      usablelcd();
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
        input_passcode += key;
        lcd.setCursor(9, 1);
        lcd.print(lcdinput);
      }
      if (key == '#') {
        if (input_passcode == passcode) {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Passcode is correct");
          input_passcode = "";
          serv.write(0);
          //            unlock = 0;
          islock = 0;
          failedcount == 0;
          delay(1000);
          lcd.clear();
          lcdinput = "";
          lcdinputchar = ' ';
          usablelcd();
        }
        else {
          lcd.setCursor(0, 2);
          failedcount += 1;
          lcd.print("Wrong Passcode");
          lcd.setCursor(0, 3);
          lcd.print("Failed count:");
          lcd.print(failedcount);
          input_passcode = ""; // clear input password
          lcd.setCursor(9, 1);
          delay(1000);
          lcd.clear();
          usablelcd();
          lcdinput = "";
          lcdinputchar = ' ';
        }
      }
    }
    else if (key == 'C' && permlocktoggle == 0) {
      setuptoggle = 1;
      lcd.clear();
      setuplcd();
      input_passcode = "";
    }
    else if (fingerset == 1 && permlocktoggle == 0){
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
        input_passcode += key;
        lcd.setCursor(9,1);
        lcd.print(input_passcode);
      }
      if (key == '#') {
        if (input_passcode == passcode) {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Correct Password");
          lcd.setCursor(0, 2);
          lcd.print("Setup Initialized");
          setuptoggle = 1;
          resettoggle = 0;
          input_passcode = "";
          delay(1000);
          fingerset = 2;
          lcd.clear();
          lcdinput = "";
          lcdinputchar = ' ';
          fingersetlcd();
        }
        else {
          lcd.setCursor(0, 3);
          lcd.print("Wrong Password");
          input_passcode = "";
          lcd.setCursor(9, 2);
          delay(1000);
          lcd.clear();
          fingersetattemptlcd();
          lcdinput = "";
          lcdinputchar = ' ';
        }
      }
    }
    else if (fingerset = 2 && permlocktoggle == 0){
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
        input_passcode += key;
        lcd.setCursor(8, 3);
        lcd.print(lcdinput);
      }
      if (key == '#') {
        byte convert = input_passcode.toInt();
        id = convert;
        getFingerprintEnroll();
        }
    }
    else if (resettoggle == 1 && permlocktoggle == 0) {
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
        input_passcode += key;
        lcd.setCursor(13, 2);
        lcd.print(lcdinput);
      }
      if (key == '#') {
        if (input_passcode == passcode) {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Correct Password");
          lcd.setCursor(0, 2);
          lcd.print("Setup Initialized");
          setuptoggle = 1;
          resettoggle = 0;
          input_passcode = "";
          delay(1000);
          lcd.clear();
          lcdinput = "";
          lcdinputchar = ' ';
          lcd.setCursor(0, 1);
          lcd.print("Setup Initialized");
          lcd.setCursor(0, 2);
          lcd.print("Passcode:");
        }
        else {
          lcd.setCursor(0, 3);
          lcd.print("Wrong Password");
          input_passcode = "";
          lcd.setCursor(13, 2);
          delay(1000);
          lcd.clear();
          resetlcd();
          lcdinput = "";
          lcdinputchar = ' ';
        }
      }
    }
    else if (setuptoggle == 1) {
      if (key != '#' && key != 'A' && key != 'B' && key != 'C' && key != 'D') {
        input_passcode += key;
        lcd.setCursor(9, 2);
        lcd.print(lcdinput);
      }
      if (key == '#' && input_passcode.length() == 6) {
        passcode = input_passcode;
        setuptoggle = 0;
        resettoggle = 0;
        usabletoggle = 1;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Passcode setup done!");
        lcd.setCursor(0, 2);
        lcd.print(passcode);
        input_passcode = "";
        delay(1000);
        lcd.clear();
        usablelcd();
        lcdinput = "";
        lcdinputchar = ' ';
      }
      else if (key == '#' && input_passcode.length() != 6) {
        input_passcode = "";
        lcd.setCursor(0, 3);
        lcd.print("6 Digits Only");
        lcd.setCursor(9, 2);
        lcdinput = "";
        lcdinputchar = ' ';
        delay(1000);
        lcd.clear();
      }
    }
  }
  else if (failedcount == 5) {
    permanentlock();
  }
  else if (input_passcode.length() == 0) {
    lcdinput = "*";
  }
  else if (input_passcode.length() == 1) {
    lcdinput = "**";
  }
  else if (input_passcode.length() == 2) {
    lcdinput = "***";
  }
  else if (input_passcode.length() == 3) {
    lcdinput = "****";
  }
  else if (input_passcode.length() == 4) {
    lcdinput = "*****";
  }
  else if (input_passcode.length() == 5) {
    lcdinput = "******";
  }

}
