#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <NewPingESP8266.h>

#define DHTPIN D7       // DHT-22 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 22 (AM2302)
#define TRIGGER_PIN_1  D0
#define ECHO_PIN_1     D1
#define TRIGGER_PIN_2  D2
#define ECHO_PIN_2     D3
#define MAX_DISTANCE 400

const char* ssid = "OPTIXFiber - REHMAN";     // Your WiFi network name
const char* password = "Rehman@123"; // Your WiFi network password

WiFiServer server(80);

NewPingESP8266 sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPingESP8266 sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

float hum;    
float temp;   
float duration1; 
float duration2; 
float distance1; 
float distance2; 
float soundsp;  
float soundcm;  
int iterations = 5;

DHT dht(DHTPIN, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<html><head><title>ESP8266 Data</title></head><body>");
            client.println("<h1>ESP8266 Data</h1>");
            client.println("<p>Ultrasonic 1: " + String(distance1) + " cm</p>");
            client.println("<p>Ultrasonic 2: " + String(distance2) + " cm</p>");
            client.println("<p>Temperature: " + String(temp) + " C</p>");
            client.println("<p>Humidity: " + String(hum) + " %</p>");
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }

  delay(1000);  

  hum = dht.readHumidity();  
  temp = dht.readTemperature();  
    
  soundsp = 331.4 + (0.606 * temp) + (0.0124 * hum);  
  soundcm = soundsp / 10000;
  
  duration1 = sonar1.ping();
  delay(1000);
  duration2 = sonar2.ping();
  
  distance1 = (duration1 / 2) * soundcm;
  distance2 = (duration2 / 2) * soundcm;
}