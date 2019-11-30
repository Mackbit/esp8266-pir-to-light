#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const int ledPin = 2; // This code uses the built-in led
int blink_counter = 0;
int bounce_off = 0;
int lampstate = 0;
int timeout_lamp = 200;
int timeout_lamp_count = 0;
const int motionPin = 5;

// WiFi
// Make sure to update this for your own WiFi network!
const char* ssid = "The LAN before time";
const char* wifi_password = "*****";

// Initialise the WiFi
WiFiClient wifiClient;

void setup() {
  pinMode(motionPin, INPUT);
  pinMode(ledPin, OUTPUT);
  // Switch the on-board LED off to start with
  digitalWrite(ledPin, HIGH);

  // Begin Serial on 115200
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("ESP8266 IP address: ");
  Serial.println(WiFi.localIP());  
  send_lamp_off();
}

// the loop function runs over and over again forever
void loop() {
  if (bounce_off==0){
    long state = digitalRead(motionPin);
    if(state == HIGH) {
      //Serial.println("High..");
      timeout_lamp_count = 0;
      bounce_off = 10;
      if(lampstate == 0) {
        send_lamp_on();
        lampstate = 1;
      }
      blink_double();
    }
    else{
      //Serial.println("Low..");
      if(lampstate == 1) {
        timeout_lamp_count += 1;
        if(timeout_lamp_count > timeout_lamp == 1) {
          send_lamp_off();
          lampstate = 0;
          timeout_lamp_count = 0;
          }
       }
    }
  }
  else{
    bounce_off -= 1;
  }  
  delay(100);
}

void blink_double(){
  digitalWrite(ledPin, LOW);
  delay(10);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(10);
  digitalWrite(ledPin, HIGH);
}

void send_lamp_on(){
  Serial.print("Turning lamp on ...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://192.168.0.3/relay/0?turn=on");
    int httpCode = http.GET();
    http.end();   //Close connection 
  } 
}

void send_lamp_off(){
  Serial.print("Turning lamp off ...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://192.168.0.3/relay/0?turn=off");
    int httpCode = http.GET();
    http.end();   //Close connection
  } 
}