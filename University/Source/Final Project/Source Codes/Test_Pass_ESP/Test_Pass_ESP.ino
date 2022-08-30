#include <Keypad.h>
// Failed Attempts
//Keypad Pre-Setup Code

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// connect the pins from right to left
byte rowPins[ROWS] = {16, 4, 0, 2};// GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte colPins[COLS] = {19, 18, 5, 17};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String passcode; // Password Variable
String input_passcode; //Password Input

int setuptoggle = 0; //Setup the password Toggle status variable
int usabletoggle = 0; //Ready to use status variable
int resettoggle = 0; // Reset the pasword variable
int failedcount = 0; // Number of failed attempts
int permlocktoggle = 0; // Permanent lock toggle

void setup(){
  Serial.begin(115200);
}

//Permanent Lock Dummy Function (Temporary) (Keypad)

void permanentlock() {
  Serial.println("Login to your website to unlock your drawer and stop the buzzer");
  Serial.println("You have 5 minutes to go login");
  Serial.println("The timer starts now");
  delay(3000); //
  Serial.println("Your drawer is permanently locked");
  permlocktoggle = 1;
  failedcount = 0;
}

void loop(){
  char key = keypad.getKey();
    if (key){
      Serial.println(key);
      if(key == '*' && permlocktoggle == 0){
        input_passcode = ""; // clear input password
        Serial.println("Input cleared");
        }
       else if (key == 'B' && passcode != "" && permlocktoggle == 0){
        resettoggle = 1;
        Serial.println("Reset Initialized");
        Serial.println("Enter your password here:");
        input_passcode = "";
       }
       else if (usabletoggle == 1 && setuptoggle == 0 && resettoggle == 0 && permlocktoggle == 0){
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
          Serial.println("Input Password: ");
          Serial.println(input_passcode);
        }
        if(key == '#'){
          if (input_passcode == passcode){
            Serial.println("Password is correct");
            input_passcode = "";
          }
          else {
            failedcount += 1;
            Serial.println("Password is incorrect, try again");
            Serial.println("Failed Attempts");
            Serial.println(failedcount);
            input_passcode = ""; // clear input password
              }
        }
       }
       else if (key == 'D' && permlocktoggle == 0){
        setuptoggle = 1;
        Serial.println("Setup Initialized");
        Serial.println("Enter your password here:");
        input_passcode = "";
        }
       else if (resettoggle == 1){
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
        }
        if (key == '#'){
          if (input_passcode == passcode){
            Serial.println("Correct Password");
            Serial.println("Enter your new password here");
            setuptoggle = 1;
            resettoggle = 0;
            input_passcode = "";
          }
          else {
            Serial.println("Wrong Password");
            input_passcode = "";
          }
        }
       }
       else if (setuptoggle == 1){
        if (key != '#' && key != 'A'&& key != 'B' && key != 'C' && key != 'D'){
          input_passcode += key;
        }
        if (key == '#'){
          passcode = input_passcode;
          setuptoggle = 0;
          resettoggle = 0;
          usabletoggle = 1;
          Serial.println("Your password is:");
          Serial.println(passcode);
          input_passcode = "";
        }
       }
      }
      else if (failedcount == 5){
        permanentlock();
      }
}
