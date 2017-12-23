const int analogInPin = A1;
int sensorValue = 0;
float sensorVoltage = 0.0;

void setup()
{
Serial.begin(9600);
}

void loop()
{
sensorValue = analogRead(analogInPin);
sensorVoltage = sensorValue * .004882814; // Convert from 0...1024 to 0...5v
Serial.print("Sensor Value: ");
Serial.println(sensorValue);
Serial.print("\t");
Serial.print("Sensor Voltage: ");
Serial.println(sensorVoltage);
delay(100);
}
