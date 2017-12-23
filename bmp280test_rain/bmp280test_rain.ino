#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define rainpin 3

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10
int RR = 0;
float rainmeter = 0;
Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
void blink()
{
  rainmeter += 0.2;
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  pinMode( rainpin , INPUT_PULLUP);
 
  attachInterrupt(digitalPinToInterrupt(rainpin), blink, RISING);
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}

void loop() {
    
     RR=digitalRead(rainpin);
  delay(500);
   Serial.print("RR:");
   Serial.println(RR);
  Serial.print(rainmeter);
  Serial.println("mm");
  
    Serial.print(F("Temperature = "));
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    
    Serial.print(F("Pressure = "));
    Serial.print(bme.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");
    Serial.println();
    delay(2000);

}
