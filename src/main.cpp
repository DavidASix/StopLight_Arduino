#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SoftwareSerial.h"


const int SENSOR_PIN = 4; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library
float tempCelsius;    // temperature in Celsius

SoftwareSerial btSerial(6,5); // TX RX
const int btState = 7;  // 0 = waiting to pair, 1 = pairing. 1 can be wrong if pairing failed

const int r = 12;
const int y = 11;
const int g = 10;
int count = 0;
int led = 0;

void setup() {
  btSerial.begin(9600); // 38400 for AT commands, 9600 for BT connections
  Serial.begin(9600);
  tempSensor.begin();    // initialize the sensor
  pinMode(btState, INPUT);
  pinMode(r, OUTPUT);
  pinMode(y, OUTPUT);
  pinMode(g, OUTPUT);
  
}

void loop() {
  if (btSerial.available()) {
    char txt = btSerial.read();
    switch (txt) {
      case 'r': led = r; break;
      case 'y': led = y; break;
      case 'g': led = g; break;
      case 't': 
        tempSensor.requestTemperatures();
        tempCelsius = tempSensor.getTempCByIndex(0);
        btSerial.println(tempCelsius);
        break;
      case 's': 
        btSerial.println(String(digitalRead(r)) + String(digitalRead(y)) + String(digitalRead(g)));
        break;
      default: led = 99; break;
    }
    digitalWrite(led, abs(digitalRead(led) - 1));
  }
}