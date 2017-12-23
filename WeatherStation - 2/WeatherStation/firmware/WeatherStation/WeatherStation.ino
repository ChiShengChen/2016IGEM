/// GENERICS
  // includes
  #include <SPI.h>
  #include <Wire.h>
  #include <Ethernet.h>
  #include <WebServer.h>
  #include <ERxPachube.h>
  #include "variables.h"
  
  // defines
  #define smillis() ((unsigned long)millis()) /// millis() rollover handling
  #define SAMPLE_TIME 5      // sample every 5 second
  #define UPLOAD_TIME 900000 // upload date to Pachube every 15 minutes
  
  // valiables
  unsigned long data_smillis    = 0;
  unsigned long pachube_smillis = 0;
  unsigned long reset_smillis   = 0;
  
  weather results; // structure containing the current weather conditions
  float temp_value;
  
  bool upload_done = false;
/// GENERICS


/// ADC
  #define NUM_READS   100 // generic ADC read constant
  
  float readADC(int sensorpin) {
    // read multiple values and sort them to take the mode
    int sortedValues[NUM_READS];
    
    for(char i=0; i<NUM_READS; i++) {
      delay(1); // wait a little
      
      int value = analogRead(sensorpin);
      
      char j;
      if(value<sortedValues[0] || i==0) {
        j=0; //insert at first position
      } else {
        for(j=1; j<i; j++) if(sortedValues[j-1]<=value && sortedValues[j]>=value) break;
      }
      
      for(char k=i;k>j;k--) sortedValues[k]=sortedValues[k-1];
      
      sortedValues[j]=value; //insert current reading
    }
    
    //return scaled mode of 10 values
    float returnval = 0;
    
    for(int i=NUM_READS/2-5; i<(NUM_READS/2+5); i++) returnval += sortedValues[i];
    
    return (returnval/10);
  }
/// ADC

/// SOFTWARE RESET
  #define RESET_PIN  7
  
  void initReset() {
    pinMode(RESET_PIN, OUTPUT);
    digitalWrite(RESET_PIN, LOW);
  }
  
  void reset() {
    delay(10);
    digitalWrite(RESET_PIN, HIGH);
  }
/// SOFTWARE RESET

/// ETHERNET
  #define PREFIX        ""
  #define PARAMETER_LEN 8
  #define VALUE_LEN     8
  #define WEB_PORT      80
  
  byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x65, 0x88 }; // MAC address
  IPAddress ip(192, 168, 178, 91);                     // IP address
  
  WebServer webserver(PREFIX, WEB_PORT);
/// ETHERNET

/// PACHUBE
  #define PACHUBE_API_KEY "X1Ld4vRM8eIfvFSAvaLSaHQrs7wE-DhBD_Cp-vDX91lbRoLQpQ6Ny04V3wCNl0lXnTnOG69P6QvUy0zC1NDVwHUfJ8bq2LK6ggzi47deADVGgn-ryLkkJlKBRWj_wRjp"
  #define PACHUBE_FEED_ID 46257
  
  ERxPachubeDataOut dataout(PACHUBE_API_KEY, PACHUBE_FEED_ID);
  
  void PrintDataStream(const ERxPachube& pachube);
  
  void initPachube() {
    dataout.addData(0, "temperature");
    dataout.addData(1, "pressure");
    dataout.addData(2, "relative humidity");
    dataout.addData(3, "dew point");
    dataout.addData(4, "humidex");
    dataout.addData(5, "wind speed");
    dataout.addData(6, "wind direction");
    dataout.addData(7, "rainfall");
    dataout.addData(8, "light");
  }
/// PACHUBE

/// WEATHER METERS
  #define ANEMOMETER 2  // digital
  #define RAIN       3  // digital
  #define REL_HUM    A0 // analog
  #define LIGHT      A1 // analog
  #define WIND_VANE  A2 // analog
  
  // constants
  const float WIND_CONST = 2.4;    // 1rot/sec = 2.4km/h
  const float RAIN_CONST = 0.2794; // each drop is 0.2794mm rain
  
  // variables
  volatile int windticks, raindrops;
  volatile unsigned long windtime, windlast;
  volatile unsigned long raintime, rainlast;
  volatile int visits_counter = 0;
  
  void initWeatherMeters(){
    pinMode(ANEMOMETER, INPUT);
    digitalWrite(ANEMOMETER, HIGH);
    attachInterrupt(0, wspeedIRQ, FALLING);
    
    pinMode(RAIN, INPUT);
    digitalWrite(RAIN, HIGH);
    attachInterrupt(1, rainIRQ, FALLING); //ISR associated with pin2, goes to countAnemometer function and is triggered when pin goes from HIGH to LOW
  }
/// WEATHER METERS

/// INTERRUPTS
void wspeedIRQ() {
  windtime = micros(); // grab current time
  
  if( (windtime - windlast) > 100 ) {
    windlast = windtime;
    
    windticks++; // each int is 1/2 rotation; 1rot/sec = 2.4km/h
  }
}

void rainIRQ() { // each int is 0.2794mm rain
  raintime = micros(); // grab current time
  
  if ( (raintime - rainlast) > 100 ) { // 100uS debounce
    raindrops++; // increment bucket counter
    rainlast = raintime; // set up for next event
  }
}
/// INTERRUPTS

////////////////////////////////////////////////////////////////////////////////////////////////////
void WeatherStation() {
  if( (smillis() - data_smillis) >= (SAMPLE_TIME*1000) ) { // in milliseconds
    data_smillis = smillis();
    
    if(upload_done == true) {
      upload_done = false;
      
      results.pressure      = 0;
      results.temperature   = 0;
      results.RH            = 0;
      results.dewpoint      = 0;
      results.humidex       = 0;
      results.windspeed     = 0;
      results.winddirection = 0;
      results.precipitation = 0;
      results.light         = 0;
    }
    
    cli();
      getTemperaturePressure(results);
      getRHDewpointHumidex(REL_HUM, results);
      
      // divide windticks by 2 as there are 2 triggers/rotation
      temp_value = ((float) (windticks>>1)/(float) SAMPLE_TIME) * WIND_CONST; // 2.4 km/h for one switch closure per second
      results.windspeed = max(results.windspeed, temp_value);
      windticks = 0;
      
      // rain caculation
      if ( visits_counter < (60/SAMPLE_TIME) ) { // meassure the rain fall every 1 minute (60seconds/polling frequency; e.g. 60/10 = 6)
        visits_counter++;
      } else {
        temp_value = RAIN_CONST * raindrops; // 0.2794 mm per contact closure; this step gives number of clicks per minute unit = mm per minute
        results.precipitation = max(results.precipitation, temp_value);
        raindrops = 0;
        visits_counter = 0;
      }
      
      temp_value = readADC(WIND_VANE)/4; // divide by 4 for 0...255 range
      results.winddirection = max(results.winddirection, (int) temp_value);
      
      results.light = (int) readADC(LIGHT); // Get light level reading
    sei();
  }
  
  /// Pachube
  if( (smillis() - pachube_smillis) >= UPLOAD_TIME ) { // every 15 minutes upload a new value
    pachube_smillis = smillis();
    
    dataout.updateData(0, results.temperature    );
    dataout.updateData(1, (int) results.pressure );
    dataout.updateData(2, results.RH             );
    dataout.updateData(3, results.dewpoint       );
    dataout.updateData(4, results.humidex        );
    dataout.updateData(5, results.windspeed      );
    dataout.updateData(6, results.winddirection  );
    dataout.updateData(7, results.precipitation  );
    dataout.updateData(8, results.light          );
    dataout.updatePachube();
    
    upload_done = true;
  } 
}
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  initReset();         // tart the reset mechanism
  initWebServer();     // start the web interface
  initBarometer();     // start the barometer
  initHygrometer();    // start the hygrometer
  initPachube();       // start Pachube service
  initWeatherMeters(); // start the weather meters
  
  data_smillis = pachube_smillis = reset_smillis = smillis();
}
////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() {
  webserver.processConnection(); // process web server requests
  WeatherStation();              // gather weather data
  
  if( (smillis() - reset_smillis) >= (86400000*2) ) reset(); // every 48 hours perform a reset
}
