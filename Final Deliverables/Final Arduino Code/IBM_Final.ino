
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <DFRobot_DHT20.h>
#define ldr 35
#define MAX 4095
#define MIN 0
#define MIN 2575
#define MAX 4095

DFRobot_DHT20 dht20;
// Replace with your network credentials
const char* ssid = "Predator";
const char* password = "allthebest";
const char* PARAM_INPUT_1 = "state";
const int output = 32;
const int buttonPin = 4;
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  float t = dht20.getTemperature();
  Serial.println(t);
  return String(t);
}

String readDHTHumidity() {
  float h = dht20.getHumidity() * 100;
  Serial.println(h);
  return String(h);
}


String outputState() {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

String readLDR() {
  int SensorValue = analogRead(ldr);
  int h;
  if (SensorValue > 3500 && SensorValue < 4095) {
    h = 10;
  }
  else if (SensorValue > 3000 && SensorValue < 3500) {
    h = 25;
  }
  else if (SensorValue > 2500 && SensorValue < 3000) {
    h = 35;
  }
  else if (SensorValue > 2000 && SensorValue < 2500) {
    h = 45;
  }
  else if (SensorValue > 2000 && SensorValue < 2500) {
    h = 55;
  }
  else if (SensorValue > 1500 && SensorValue < 2000) {
    h = 65;
  }
  else if (SensorValue > 1000 && SensorValue < 1500) {
    h = 75;
  }
  else if (SensorValue > 500 && SensorValue < 1000) {
    h = 85;
  }
  else
  {
    h = 95;
  }
  Serial.println(h);
  return String(h);
}

String readMoisture() {
  int SensorValue = analogRead(34);
  Serial.println(SensorValue);
 int temp = (SensorValue - MIN) / (MAX - MIN);
  int h = abs(100 - (temp * 100));
  Serial.println(h);
  return String(h);
}
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
        body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Smart Farmer's Dashboard</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
    <p>
    <i class="fas fa-solid fa-sun" style="color:#fce803;"></i> 
    <span class="dht-labels">Luminous Intensity</span>
    <span id="Luminous Intensity">%LDR%</span>
    <sup class="units">&percnt;</sup>
  </p>
      <p>
    <i class="fas fa-tank-water" style="color:#964B00;"></i> 
    <span class="dht-labels">Soil Moisture</span>
    <span id="Soil Moisture">35</span>
    <sup class="units">&percnt;</sup>
  </p>
  %BUTTONPLACEHOLDER%
  <script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "On";
      }
      else { 
        inputChecked = false;
        outputStateM = "Off";
      }
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;
</script>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Luminous Intensity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ldr", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Soil Moisture").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/moisture", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") {
    return readDHTHumidity();
  }
  else if (var == "LDR") {
    return readLDR();
  }
    else if (var == "MOISTURE") {
    return readMoisture();
  }
  else if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    String outputStateValue = outputState();
    buttons += "<h4>Motor Pump - Status:<span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  while (dht20.begin()) {
    Serial.println("Initialize sensor failed");
    delay(1000);
  }
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readLDR().c_str());
  });
  server.on("/moisture", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readMoisture().c_str());
  });
  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });
  // Start server
  server.begin();
}
void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(output, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
