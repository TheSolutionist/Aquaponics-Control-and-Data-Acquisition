/*
 # This sample code is used to test the pH meter Pro V1.0.
 # Editor : YouYou
 # Ver    : 1.0
 # Product: analog pH meter Pro
 # SKU    : SEN0169
*/

const char* thingSpeak = "api.thingspeak.com";
String phApiKey ="YOUR_API_KEY";
int sent = 0;
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#define SensorPin 0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0           //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 300000
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
#define CS_PIN 15 // Use 10 when compiling for Arduino
int adcRead(int channel) {
  if ((channel > 7) || (channel < 0)) {
    return -1;
  }

  digitalWrite(CS_PIN, LOW);

  SPI.transfer(1);
  uint8_t r1 = SPI.transfer((channel + 8) << 4);
  uint8_t r2 = SPI.transfer(0);

  digitalWrite(CS_PIN, HIGH);

  return ((r1 & 3) << 8) + r2;
}


//defining Wifi values
const char* ssid     = "WiFi SSID";
const char* password = "WiFi Password";
IPAddress ip(192,168,1,105);  //Node static IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
//defining Wifi values ^

void sendPH(float PH) {   
  WiFiClient client;
   if (client.connect(thingSpeak, 80)) { 
       
   String postStr = phApiKey;
   postStr += "&field1=";
   postStr += String(PH);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + phApiKey + "\n");
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



void setup(void){

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

  
  pinMode(LED,OUTPUT);  
  Serial.begin(115200);  
  Serial.println("pH meter experiment!");    //Test the serial monitor

  pinMode(CS_PIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setFrequency(1000); // 1 MHz -- remove line when running on Arduino UNO
  digitalWrite(CS_PIN, HIGH);




}
void loop(void)
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=adcRead(0);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
  Serial.println(pHValue,2);
        //digitalWrite(LED,digitalRead(LED)^1);
        sendPH(pHValue);
        printTime=millis();
        
  }
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
