#include "DHT.h"
#include <NewPingESP8266.h>

#define DHTPIN D7       // DHT-22 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 22 (AM2302)
#define TRIGGER_PIN_1  D0
#define ECHO_PIN_1     D1
#define TRIGGER_PIN_2  D2
#define ECHO_PIN_2     D3
#define MAX_DISTANCE 400

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
}

void loop() {
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
  
  Serial.print("Ultrasonic 1: ");
  if (distance1 >= 400 || distance1 <= 2) {
    Serial.print("Out of range");
  } else {
    Serial.print(distance1);
    Serial.print(" cm ");
  }
  
  Serial.print("       ");
  Serial.print("Ultrasonic 2: ");
  if (distance2 >= 400 || distance2 <= 2) {
    Serial.print("Out of range");
  } else {
    Serial.print(distance2);
    Serial.print(" cm");
  }
  
  Serial.print("       ");
  Serial.print(" Temperature: ");
  Serial.print(temp);
  Serial.print(" C");
  Serial.print("     ");
  Serial.print(" Humidity: ");
  Serial.print(hum);
  Serial.print(" %");
  Serial.println(" ");
}
