//WebServer Libaries

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//Keypad Passcode Libaries

#include <CountDown.h>
#include <Keypad.h>

//Web Server Setting network credentials

const char* ssid = "Parkchai-2.4G"; // Your Wifi Name
const char* password = "HB1525897"; // Your Wifi Password


/*
const char* ssid = "Putt-iPad";
const char* password = "puttipong785";
*/

const char* http_username = "putt";
const char* http_password = "785";


const char* input_parameter1 = "output";
const char* input_parameter2 = "state";

const char* base64Encoding = "cHV0dC5hNzg1Ojc4NQ==";

// Creating a AsyncWebServer object (Web Server)
AsyncWebServer server(80);


   const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <title>mastuff</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="icon" href="data:,">
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      h3 {font-size: 1.8rem; color: white;}
      p {  font-size: 1.2rem;}
      .stat-label{
      vertical-align:middle;
      padding-bottom: 15px;
      }
      body {  margin: 0;}
      .switch {position: relative; display: inline-block; width: 120px; height: 68px; margin-bottom: 20px;}
      .switch input {display: none;}
      .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 68px;   opacity: 0.8;   cursor: pointer;}
      .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
      input:checked+.slider {background-color: #1b78e2}
      input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
      .topnav { overflow: hidden; background-color: #2f4468; height: 100px; color: white; font-size: 1.7rem; font-weight:500}
      .content { padding: 20px; }
      .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
      .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(500px, 1fr)); }
      .reading { font-size: 2.6rem; }
      .card-switch {color: #50a2ff; }
      .card.accelo { color: #6712e6; height: auto; width: 500px; margin-left: auto; margin-right: auto;}
      .card.togglepmnt { color: #6712e6; height: auto; width: 500px; margin-left: auto; margin-right: auto;}
      .icon-pointer{ cursor: pointer;}
   
    </style>
  </head>
  <body>
    <div class="topnav">
      <h3 style="font-size:40px; letter-spacing: 4px; font-variant: small-caps;">mastuff <span style="text-align:right;"> <i class="fas fa-user-slash icon-pointer" style="font-size:14px;" onclick="logoutButton()"></i></span></h3>
    </div>
    <p>&emsp;</p>
    <div class="content">
      <div class="cards">
        <div class="card accelo">
          <h4 style="color:#260670;font-size:35px;">Locker latest activates</h4><div><p class="reading">TBA</p></div>
        </div>
        <div class="card togglepmnt">
          <h4>PERMANENT LOCK MODE</h4><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="packet"> <span id="rt1"></span></p>
        </div>
  <script>
  
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

  </script>
  </body>
  </html>
  )rawliteral";
  



String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}


const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";

//Keypad Pre-Setup Code

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// connect the pins from right to left
byte rowPins[ROWS] = {16, 4, 0, 2}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
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
  // Connect to Wi-Fi (Web Server)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
}

// Print ESP Local IP Address (Webserver)
  Serial.println("Connection set up successfully!");
  Serial.println(WiFi.localIP());

  // Route for root / web page (Webserver)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html);  // HTML style setup thing
  });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html);  // DONT FORGET TO ADD IN processor
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });
  
  // Start server (Webserver)
  server.begin();
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
       else if (key == 'A' && permlocktoggle == 0){
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
