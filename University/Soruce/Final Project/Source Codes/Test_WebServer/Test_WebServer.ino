#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Setting network credentials

const char* ssid = "Pattarapark";
const char* password = "Most40233";


/*
const char* ssid = "Putt-iPad";
const char* password = "puttipong785";
*/

const char* http_username = "putt";
const char* http_password = "785";


const char* input_parameter1 = "output";
const char* input_parameter2 = "state";

const char* base64Encoding = "cHV0dC5hNzg1Ojc4NQ==";
// Creating a AsyncWebServer object 
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


void setup(){
  // Serial port for debugging purposes
  //Serial.begin(115200);
/*
pinMode(P35,OUTPUT);
digitalWrite(P32, LOW);
pinMode(25, OUTPUT);
digitalWrite(25, LOW);
pinMode(27, OUTPUT);
digitalWrite(27, LOW);
pinMode(13, OUTPUT);
digitalWrite(13, LOW);
*/
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.println("Connecting to WiFi");
  }

  // Print ESP Local IP Address
  //Serial.println("Connection set up successfully!");
  //Serial.println(WiFi.localIP());

  // Route for root / web page
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
  
  // Start server
  server.begin();
}

void loop() {

}
