const int sensorPin = A1; 
int sensorValue = 0; 
float sensorVoltage = 0; 
float windSpeed = 0;
 
float voltageConversionConstant = .004882814; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 1000; 

float voltageMin = .4; 
float windSpeedMin = 0; 
 
float voltageMax = 2.0; 
float windSpeedMax = 32; 
 
 
 
void setup() 
{              
  Serial.begin(9600);  
 
} 
void loop() 
{
sensorValue = analogRead(sensorPin); 
 
sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage
 

if (sensorVoltage &lt;= voltageMin){
 windSpeed = 0; 
}
else {
  windSpeed = (sensorVoltage - voltageMin)*windSpeedMax/(voltageMax - voltageMin); 
}
 

  Serial.print("Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("\t"); 
  Serial.print("Wind speed: ");
  Serial.println(windSpeed); 
 
 delay(sensorDelay);
}
