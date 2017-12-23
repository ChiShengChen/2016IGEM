//parametro de la red Wi-Fi
#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__
char passphrase[] = "pass_wifi"; 
char ssid[] = "name_wifi";
#endif

#include <OneWire.h>
#include "WiFly.h"

int sensorPin = A0;
int sensorValue = 0;

int DS18S20_Pin = 3;
OneWire ds(DS18S20_Pin);
 
Client client( "your_website", 80 );
 
int looped = 1;
 
void setup()
{
  Serial.begin( 9600 ); 
  WiFly.begin();
 
  if( !WiFly.join( ssid, passphrase ) ) 
  {
    Serial.println( "Association failed." );
 
    while( 1 ) 
    {
      // Hang on failure.
    }
  }
}
 
void loop()
{ 
  
  if( client.connect() ) 
  {
    sensorValue = analogRead(sensorPin);
    float temp = getTemp(); 
    Serial.println( temp );
    Serial.println(sensorValue);
 
    client.print( "GET /add.php?");
    client.print("temp1=");
    client.print( temp );
    client.print("&&");
    client.print("moi1=");
    client.print( sensorValue );
    client.println( " HTTP/1.1");
    client.println( "Host: your_website" );
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
  }
  delay( 5000 );
}

float getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}
