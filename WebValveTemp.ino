//Fixes the pins on the NodeMCU
#ifndef Pins_Arduino_h
#define Pins_Arduino_h
#include "../generic/common.h"
static const uint8_t SDA = 4;
static const uint8_t SCL = 5;
static const uint8_t LED_BUILTIN = 16;
static const uint8_t BUILTIN_LED = 16;
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
#endif Pins_Arduino_h 
//Fixes the pins on the NodeMCU^ 

//Library's to include
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WebServer.h>
//Library's to include ^


// For Temperature Sensor
#define myPeriodic 15 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS D9 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float prevTemp = 0;
const char* thingSpeak = "api.thingspeak.com";
String tempApiKey ="YOUR_API_KEY";
String phApiKey ="YOUR_API_KEY";
int sent = 0;
// For Temperature Sensor ^


//defining values for valve control
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const int tempSendInterval = 300000;
const int oneSecondInterval = 1000;
const int intValves = 4;
int valveOpenTime[intValves]; // in minutes
int valveCloseTime[intValves]; // in minutes
int valvePin[intValves];
int isOpen[intValves];     // this gives us a way to keep track of the current state of a valve - assume that if it is not open it is closed - closed will equal false or 0
int remainingOpenTime[intValves];   //  this gives us a place to track how long a valve has been open
int remainingCloseTime[intValves];  //  this gives us a place to track how long a valve has been close
//defining values for valve control ^

//defining Wifi values
const char* ssid     = "WiFi SSID";
const char* password = "WiFi Password";
ESP8266WebServer server(80);
IPAddress ip(172,16,10,100);  //Node static IP
IPAddress gateway(172,16,10,1);
IPAddress subnet(255,255,255,0);
//defining Wifi values ^

// Functions 



void handleRoot() {
  
}




void handleStatus() {
   
   
   String Status = "<html><head>";
   Status +=  "<title>Aquaponics Status Page</title>";
   Status +=  "<style>";
   Status +=  "body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
   Status +=  "</style>";
   Status += "</head>";
   Status +=  "<body>";
   Status +=  "<h1>Please Enjoy the Statistics Below</h1>";
   
   Status +=  "<p> The Back Grow Bed has ";
  if (remainingCloseTime[1] == 0) 
  {
   Status += remainingOpenTime[1]/60;
   Status +=  " Minutes Left in Draining </p>";
  }
  else 
  {
    Status += remainingCloseTime[1]/60;
    Status +=  " Minutes Left in Filling </p>";
  }
   
    Status +=  "<p>&nbsp;&nbsp;&nbsp; The Back Grow Bed Fill Time is set to ";
    Status += valveCloseTime[1]/60;
    Status +=  " Minutes  </p>";

    Status +=  "<p>&nbsp;&nbsp;&nbsp; The Back Grow Bed Drain Time is set to ";
    Status += valveOpenTime[1]/60;
    Status +=  " Minutes  </p>";
  
  
   Status +=  "<p> The Left Grow Bed has ";
  if (remainingCloseTime[2] == 0) 
  {
   Status += remainingOpenTime[2]/60;
   Status +=  " Minutes Left Draining </p>";
  }
  else 
  {
    Status += remainingCloseTime[2]/60;
    Status +=  " Minutes Left Filling </p>";
  }
   
    Status +=  "<p>&nbsp;&nbsp;&nbsp; The Left Grow Bed Fill Time is set to ";
    Status += valveCloseTime[2]/60;
    Status +=  " Minutes  </p>";

    Status +=  "<p>&nbsp;&nbsp;&nbsp; The Left Grow Bed Drain Time is set to ";
    Status += valveOpenTime[2]/60;
    Status +=  " Minutes  </p>";

    
     Status +=  "<p> The Right Grow Bed has ";
  if (remainingCloseTime[3] == 0) 
  {
   Status += remainingOpenTime[3]/60;
   Status +=  " Minutes Left in Draining </p>";
  }
  else 
  {
    Status += remainingCloseTime[3]/60;
    Status +=  " Minutes Left in Filling </p>";
  }
   
   Status +=  "<p>&nbsp;&nbsp;&nbsp; The Right Grow Bed Fill Time is set to ";
   Status += valveCloseTime[3]/60;
   Status +=  " Minutes  </p>";
   Status +=  "<p>&nbsp;&nbsp;&nbsp; The Right Grow Bed Drain Time is set to ";
   Status += valveOpenTime[3]/60;
   Status +=  " Minutes  </p>";
   Status += "<iframe width=""450"" height=""260"" style=""border: 1px solid #cccccc;"" src=""https://thingspeak.com/channels/176777/charts/1?bgcolor=%23ffffff&color=%23d62020&days=7&dynamic=true&max=100&min=50&title=Aquaponics+Temperature&type=line""></iframe>"; 
   Status += "<iframe width=""450"" height=""260"" style=""border: 1px solid #cccccc;"" src=""https://thingspeak.com/channels/181035/charts/1?bgcolor=%23ffffff&color=%23d62020&days=3&dynamic=true&type=line""></iframe>";
  

  Status +=  "<h1>Set the Flood and Drain timers below</h1>";
   Status +=    "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/BGBF' method='POST'>";
   Status +=    "Back_Grow_Bed_Fill_Time: <input type='text' name='BGBF'><br>";
   Status +=    "<input type='submit' value='Submit'>";  
   Status +=    "</form>";   
   Status +=    "<p></p>";
   Status +=   "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/BGBD' method='POST'>";
   Status +=    "Back_Grow_Bed_Drain_Time: <input type='text' name='BGBD'><br>";
   Status +=    "<input type='submit' value='Submit'>";  
   Status +=    "</form>";   
   Status +=    "<p></p>";
   Status +=    "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/LGBF' method='POST'>";
   Status +=    "Left_Grow_Bed_Fill_Time: <input type='text' name='LGBF'><br>";
   Status +=    "<input type='submit' value='Submit'>";
   Status +=    "</form>";     
   Status +=    "<p></p>";
   Status +=    "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/LGBD' method='POST'>";
   Status +=    "Left_Grow_Bed_Drain_Time: <input type='text' name='LGBD'><br>";
   Status +=    "<input type='submit' value='Submit'>"; 
   Status +=    "</form>";     
   Status +=    "<p></p>";
   Status +=    "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/RGBF' method='POST'>";
   Status +=    "Right_Grow_Bed_Fill_Time: <input type='text' name='RGBF'><br>";
   Status +=    "<input type='submit' value='Submit'>";  
   Status +=    "</form>";    
   Status +=    "<p></p>";
   Status +=    "<form action='http://MICROCONTROLLER_ADDRESS_OR_HOSTNAME/RGBD' method='POST'>";
   Status +=    "Right_Grow_Bed_Drain_Time: <input type='text' name='RGBD'><br>";
   Status +=    "<input type='submit' value='Submit'>";      
   Status +=    "</form>";
   Status +=  "</body>";





 
   
   Status +=  "</form>";
   Status += "</body>";
   Status +=  "</html>";
  
    server.send(200, "text/html", Status);
    Serial.print(Status);
}


void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleBGBF() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "BGBF") {
         String NewTime1 = server.arg(i);
         valveCloseTime[1] =  NewTime1.toInt();
         valveCloseTime[1] = valveCloseTime[1] * 60;
        // Serial.print("Back Grow Bed Will Now Fill For ");
        // Serial.print(NewTime1);
        // Serial.println(" Minutes");
        
         server.send(200, "text/plain", "The New Time is set to " += NewTime1);
      }
    }
  }
}

void handleBGBD() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "BGBD") {
         String NewTime2 = server.arg(i);
         valveOpenTime[1] =  NewTime2.toInt();
         valveOpenTime[1] = valveOpenTime[1] * 60;
       //  Serial.print("Back Grow Bed Will Now Fill For ");
       //  Serial.print(NewTime2);
       //  Serial.println(" Minutes");
         NewTime2 = "";   
         server.send(200, "text/plain", "Accepted, Click Back to Return to Status Page");     
      }
    }
  }
}

void handleLGBF() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "LGBF") {
         String NewTime3 = server.arg(i);
         valveCloseTime[2] =  NewTime3.toInt();
         valveCloseTime[2] = valveCloseTime[2] * 60;
         Serial.print("Back Grow Bed Will Now Fill For ");
         Serial.print(NewTime3);
         Serial.println(" Minutes");
         NewTime3 = "";
         server.send(200, "text/plain", "Accepted, Click Back to Return to Status Page");
      }
    }
  }
}

void handleLGBD() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "LGBD") {
         String NewTime4 = server.arg(i);
         valveOpenTime[2] =  NewTime4.toInt();
         valveOpenTime[2] = valveOpenTime[2] * 60;
         Serial.print("Back Grow Bed Will Now Fill For ");
         Serial.print(NewTime4);
         Serial.println(" Minutes");
         NewTime4 = "";
         server.send(200, "text/plain", "Accepted, Click Back to Return to Status Page");
      }
    }
  }
}

void handleRGBF() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "RGBF") {
         String NewTime5 = server.arg(i);
         valveCloseTime[3] =  NewTime5.toInt();
         valveCloseTime[3] = valveCloseTime[3] * 60;
         Serial.print("Back Grow Bed Will Now Fill For ");
         Serial.print(NewTime5);
         Serial.println(" Minutes");
         NewTime5 = "";
         server.send(200, "text/plain", "Accepted, Click Back to Return to Status Page");
      }
    }
  }
}

void handleRGBD() {
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "RGBD") {
         String NewTime6 = server.arg(i);
         valveOpenTime[3] =  NewTime6.toInt();
         valveOpenTime[3] = valveOpenTime[3] * 60;
         Serial.print("Back Grow Bed Will Now Fill For ");
         Serial.print(NewTime6);
         Serial.println(" Minutes");
         NewTime6 = "";
         server.send(200, "text/plain", "Accepted, Click Back to Return to Status Page");
      }
    }
  }
}



void handleReset() {
  ESP.reset();
}

void closeValve(int valve) {
  digitalWrite(valvePin[valve], HIGH);  
  isOpen[valve] = false;
  remainingCloseTime[valve] = valveCloseTime[valve]; 
}
 
 
void openValve(int valve) {
  digitalWrite(valvePin[valve], LOW); 
  isOpen[valve] = true;
  remainingOpenTime[valve] = valveOpenTime[valve];
}

void sendTeperatureTS(float temp){  
   WiFiClient client;
   if (client.connect(thingSpeak, 80)) { 
       
   String postStr = tempApiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + tempApiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(750);
    }
   sent++;
 client.stop();
}

// Functions ^

void setup() {

  Serial.begin(115200);  // Start Serial Monitor 

 // Start and Connect to Wifi
  WiFi.config(ip, gateway, subnet);
  delay(10);
  Serial.println("Connecting to "+String(ssid)); 
  WiFi.begin(ssid, password);   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("+");
  }
  Serial.println("");
  Serial.println("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());  
 // Start and Connect to Wifi ^

// Define and Start Web Server 
  server.on("/", handleRoot);            // root web page
  server.on("/BGBF", handleBGBF); 
  server.on("/BGBD", handleBGBD);
  server.on("/LGBF", handleLGBF);
  server.on("/LGBD", handleLGBD);
  server.on("/RGBF", handleRGBF);
  server.on("/RGBD", handleRGBD);
  server.on("/Status", handleStatus);    
  server.onNotFound(handleNotFound); 
  server.on("/Reset", handleReset);
  server.begin(); 
  Serial.flush();
// Define and Start Web Server ^

// Setup Valves
int valves;  // counter for loops

for (valves = 1 ; valves <= intValves; valves ++){
    pinMode(valvePin[valves] , OUTPUT); 
    }
valvePin[1] = D1;    //Back
valveOpenTime[1] = 20*60 ;  // Open time is the time that tank will be draining  in seconds
valveCloseTime[1] = 10*60 ;   // Fill time is the time that tank will be filling or overflowing
pinMode(D1, OUTPUT);
  
valvePin[2] = D2;    // Left
valveOpenTime[2] = 20*60 ;  // Open time is the time that tank will be draining
valveCloseTime[2] = 10*60 ;   // Fill time is the time that tank will be filling or overflowing
pinMode(D2, OUTPUT);
  
valvePin[3] = D3;   //Right
valveOpenTime[3] = 20*60 ;  // Open time is the time that tank will be draining
valveCloseTime[3] = 10*60 ;   // Fill time is the time that tank will be filling or overflowing
pinMode(D3, OUTPUT);
  
// Set Valve 1
closeValve(1);
remainingCloseTime[1] = 1*60;  // In Seconds
 
// Set Valve 2
openValve(2);
remainingOpenTime[2] = 1*60 ;
  
// Set Valve 3
openValve(3);
remainingOpenTime[3] = 1*60;  


pinMode(ONE_WIRE_BUS, INPUT);
}

void loop() {
server.handleClient();

/*
Serial.print("remaining Open Time for bed 1 is :  ");
Serial.println(remainingOpenTime[1]);
Serial.print("remaining Close Time for bed 1 is : ");
Serial.println(remainingCloseTime[1]);
Serial.print("Valve Close time for bed 1 is : ");
Serial.println(valveCloseTime[1]);
Serial.print("Valve Open time for bed 1 is : ");
Serial.println(valveOpenTime[1]);
Serial.println("");

Serial.print("remaining Open Time for bed 2 is :  ");
Serial.println(remainingOpenTime[2]);
Serial.print("remaining Close Time for bed 2 is : ");
Serial.println(remainingCloseTime[2]);
Serial.print("Valve Close time for bed 2 is : ");
Serial.println(valveCloseTime[2]);
Serial.print("Valve Open time for bed 2 is : ");
Serial.println(valveOpenTime[2]);
Serial.println("");

Serial.print("remaining Open Time for bed 3 is :  ");
Serial.println(remainingOpenTime[3]);
Serial.print("remaining Close Time for bed 3 is : ");
Serial.println(remainingCloseTime[3]);
Serial.print("Valve Close time for bed 3 is : ");
Serial.println(valveCloseTime[3]);
Serial.print("Valve Open time for bed 3 is : ");
Serial.println(valveOpenTime[3]);
Serial.print("system Time is : ");
Serial.println(millis());
Serial.println("-------------------------------------------");*/

//Valve Control
unsigned long currentMillis1 = millis();
 if (currentMillis1 - previousMillis1 >= oneSecondInterval) {   
   for (int valve = 1; valve <= intValves ; valve++) {
    if(isOpen[valve] == true)
    {
      remainingOpenTime[valve] -- ;
    }
    else
    {
      remainingCloseTime[valve] -- ;
    }
    if (isOpen[valve] == true && remainingOpenTime[valve] == 0)
    {
      closeValve(valve) ;
    }
    else if (isOpen[valve] == false && remainingCloseTime[valve] == 0)
    {
      openValve(valve) ; 
     }
     previousMillis1 = currentMillis1;
    }
  }
// Valve Control ^


// Temperature Sensor
unsigned long currentMillis2 = millis();
 if (currentMillis2 - previousMillis2 >= tempSendInterval) { 
  float temp;
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempFByIndex(0);
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);
  sendTeperatureTS(temp);
  previousMillis2 = currentMillis2;  
  // Temperature Sensor ^
 }

}
