#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// connect the pins from right to left to pin 2, 3, 4, 5,6,7,8,9
byte rowPins[ROWS]      = {16, 0, 4, 2}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte colPins[COLS] = {19, 18, 5, 17}; // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins

String password; // change your password here
String input_password;
int passlen = password.length(); //The length of your password
int intpasslen = input_password.length();
int setuptoggle = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(115200);

}

void loop(){
  char key = keypad.getKey();
    if (key){
      Serial.println(key);
    }
}

//void loop(){
//  char key = keypad.getKey();
//  if (key){
//    Serial.println(passlen);
//    Serial.println(key);
//    if(key == '*'){
//      input_password = ""; // clear input password
//    }
//    else if (setuptoggle != 0){
//      passset();
//      if (intpasslen == 0){
//        Serial.println("Setup initialized");
//      }
//    }
//    else if (key == 'A'){
//      setuptoggle = 1;
//    }
//    else if (passlen == 6){
//      input_password += key;
//      if (key == '#'){
//        if (input_password == password){
//          Serial.println("The drawer is opened");
//        }
//        else {
//          Serial.println("Wrong Password");
//          input_password == "";
//        }
//      }
//    }
//  }
//}

//void passset(){
//  char key = keypad.getKey();
//  if (passlen < 6){ //Length of your password: passlen < X, where X is the password length.
//    input_password += key;
//    if (key == '#'){
//      input_password == password;
//      setuptoggle = 0;
//    }
//  }
//  else if (passlen == 6){
//    input_password += key;
//    if (key == '#'){
//      if (input_password == password){
//        password = "";
//        input_password = "";
//        }
//    }
//  }
//}

//  if (key){
//    Serial.println(key);
//    if(key == '*') {
//      input_password = ""; // clear input password
//    } else if(key == '#') {
//      if(password == input_password) {
//        Serial.println("password is correct");
//        // DO YOUR WORK HERE
//      } else {
//        Serial.println("password is incorrect, try again");
//      }
//      input_password = ""; // clear input password
//    } else {
//      input_password += key; // append new character to input password string
//    }
//  }
//}
