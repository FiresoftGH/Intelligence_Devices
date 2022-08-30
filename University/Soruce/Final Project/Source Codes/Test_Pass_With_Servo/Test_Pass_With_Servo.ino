//Webserver Libaries

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>

//LCD Libaries

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Servo Libaries
#include <ESP32Servo.h>

Servo serv; //Servo 1 Variable

int unlock; //Servo status variable
int islock = 1; //LDR 1 for servo 1 status tracking
int islock2 = 1; //LDR 2 for servo 2 status tracking

// Keypad Libaries
#include <Keypad.h>

// Failed Attempts

//Website Pre-Setup code

// Setting network credentials


// Change ssid & password according to hotspot you chose
const char* ssid = "Parkchai-2.4G";
const char* password = "HB1525897";

// Web authentication username & password
const char* http_username = "user";
const char* http_password = "12345";

const char* base64Encoding = "dXNlcjoxMjM0NQ==";


// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outp25 = "locked";
String outp23 = "sound off";

// Assign output variables to GPIO pins
const int otp25 = 25;
const int otp23 = 23;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


// Google sheet data triggering
String seerver = "http://maker.ifttt.com";
String eventName = "ldr_data";
String IFTTT_Key = "dVQQRmdbnHXLfAlHPyAxD4hfT-TAKekV1Et64yJZ_6d";
String IFTTTUrl="https://maker.ifttt.com/trigger/ldr_data/with/key/dVQQRmdbnHXLfAlHPyAxD4hfT-TAKekV1Et64yJZ_6d";
String Sheets_GAS_ID = "AKfycbyO1BRkY0C4rTPYBok8UL3v-X9MoFrVuGUopYw3LtgvEep65HZD8yilI3iKezATgrQ";

// async with web server 
WiFiServer server(80);


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
//char lcdinputchar;

int passlen = passcode.length(); //Passcode length variable
int inputlen = input_passcode.length(); //Input length variable

int setuptoggle = 0; //Setup the password Toggle status variable
int usabletoggle = 0; //Ready to use status variable
int resettoggle = 0; // Reset the pasword variable
int failedcount = 0; // Number of failed attempts
int permlocktoggle = 0; // Permanent lock toggle

void setup(){
  Serial.begin(115200);
//  Servo Setup Codes
  serv.attach(otp25); //Servo 1 Pin
  lcd.begin(); //Initialize LCD
  lcd.backlight(); //Turn on LCD backlight
  pinMode(otp23,OUTPUT); //Buzzer Pin
  lcd.setCursor(0,0);
  lcd.print("Press 'C' to start");

  // Connect to Wi-Fi network with SSID and password & Print ESP Local IP Address
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.println("Connecting to WiFi");
  }
//  Serial.println("Connection set up successfully!");
//  Serial.println(WiFi.localIP());
  server.begin();
  
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
  if (unlock = 0){
    lcd.print("Unlocked");
    }
    else {
      lcd.print("Locked");
      }
   lcd.setCursor(0,1);
   lcd.print("Passcode:"); 
   lcd.setCursor(0,2);
   lcd.print("IP:");
   lcd.setCursor(3,2);
   lcd.print(WiFi.localIP());
}

void resetlcd(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Reset Initialized");
  lcd.setCursor(0,2);
  lcd.print("Old Passcode:");
  input_passcode = "";
}

//Google sheets Trigger
void sendData(void) {
  
  String url = seerver + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + "Opened";  

//  Serial.println(url);

  //Start to send data to IFTTT

  HTTPClient http;

//  Serial.print("[HTTP] begin...\n");

  http.begin(url); //HTTP

//  Serial.print("[HTTP] GET...\n");

  // start connection and send HTTP header

  int httpCode = http.GET();

  // httpCode will be negative on error

  if(httpCode > 0) {

    // HTTP header has been send and Server response header has been handled

//    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server

    if(httpCode == HTTP_CODE_OK) {

      String payload = http.getString();

//      Serial.println(payload);
    }
  } else {

//    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop(){
  //Servo Main codes
//  if (unlock == 1){
//    serv.write(0);
//    delay(2000);
//    unlock = 0;
//    islock = 0;
//    islock2 = 0;
//  }
  if (islock == 0){
    int val = analogRead(34); //Sensitivity
    
    if (val < 2500){ //Sensitivity
      delay(2000);
      serv.write(180);
      islock = 1;
      unlock = 0;
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
        //lcdinputchar = ' ';
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
            serv.write(0);
//            unlock = 1;
            failedcount == 0;
            delay(2000);
            islock = 0;
            lcd.clear();
            lcdinput = "";
            //lcdinputchar = ' ';
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
//            //lcdinputchar = ' ';
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
//            //lcdinputchar = ' ';
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
//            //lcdinputchar = ' ';
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
//          //lcdinputchar = ' ';
        }
        else if (key == '#' && input_passcode.length() != 6){
          input_passcode = "";
          lcd.setCursor(0,3);
          lcd.print("6 Digits Only");
          lcd.setCursor(9,2);
          lcdinput = "";
//          //lcdinputchar = ' ';
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
      // Put this entire junction below after other functions in this loop
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
//    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
//        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // check base64 encode for authentication
            // Finding the right credentials
            if (header.indexOf(base64Encoding)>=0)
            {
            
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // turns the GPIOs on and off
              if (header.indexOf("GET /25/on") >= 0) {
//                Serial.println("SERV 25 on");
                outp25 = "unlock";
                serv.write(180);
                sendData();
              } else if (header.indexOf("GET /25/off") >= 0) {
//                Serial.println("SERV 25 off");
                outp25 = "locked";
                serv.write(0);
              } else if (header.indexOf("GET /23/on") >= 0) {
//                Serial.println("SERV 23 sound on");
                outp23 = "sound on";
                digitalWrite(otp23, HIGH);
              } else if (header.indexOf("GET /23/off") >= 0) {
//                Serial.println("SERV 23 sound off");
                outp23 = "sound off";
                digitalWrite(otp23, LOW);
              }
              
              
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><title>mastuff</title>");
              client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family:  Arial; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println("h3 {font-size: 1.8rem;}");
              client.println("p {font-size: 1.6rem;}");
              client.println(".stat-label {vertical-align:middle; padding-bottom: 18px;");
              client.println("body {margin: 0;}");
              //client.println(".topnav { overflow: hidden; background-color: #2f4468; height: 100px; color: white; font-size: 1.7rem; font-weight:500}");
              //client.println(".content { padding: 20px; }");
              client.println(".button {display: block;width: 80px; background-color: #0793f7; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #0793f7;}</style></head>");
              
              
              // Web Page Heading
              client.println("<body><h3 style=\"font-size:60px; letter-spacing: 6px; font-variant: small-caps; color:#2f4468\">mastuff</h3>");
              // Display current state, and ON/OFF buttons for GPIO 25  
              client.println("<p style=\"color:#4e1b7a\" ><b> State " + outp25 + "</b></p>");
              // If the outp25 is off, it displays the ON button       
              if (outp25=="locked") {
                client.println("<p><a href=\"/25/on\"><button class=\"button\">LOCKED</button></a></p>");
              } else {
                client.println("<p><a href=\"/25/off\"><button class=\"button button2\">UNLOCK</button></a></p>");
              } 

              // Display current state, and ON/OFF buttons for GPIO 23  
              client.println("<p style=\"color:#4e1b7a\" ><b>" + outp23 + "</b></p>");
              // If the outp23 is off, it displays the ON button
              if (outp23=="sound off") {
                client.println("<p><a href=\"/23/on\"><button class=\"button\">Sound Off</button></a></p>");
              } else {
                client.println("<p><a href=\"/23/off\"><button class=\"button button2\">Sound On</button></a></p>");
              } 
                
             
              client.println("</body></html>");
              
              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            }
            else{
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"Secure\"");
              client.println("Content-Type: text/html");
              client.println();
              client.println("<html>Authentication failed</html>");
            }
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    delay(1000);
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
//    Serial.println("Client disconnected.");
//    Serial.println("");
  }
}
