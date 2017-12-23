#include <math.h>

const float ADC_CONST           = 4980.00/1023.00;
const float ZERO_OFFSET         = 958.00;
const float SLOPE               = 30.68;
const float TEMP_COMP_FIXED_VAL = 1.0546;
const float TEMP_COMP_COEFF     = 0.00216;
const float ERROR_CORRECTION    = 17.20; // compensates for the lower humidity meassured by the sensor - experimentaly determined

void initHygrometer() {
  analogReference(DEFAULT);
}

void getRHDewpointHumidex(int sensorpin, weather &values) {
  values.RH = ((((readADC(sensorpin)*ADC_CONST) - ZERO_OFFSET)/SLOPE)/(TEMP_COMP_FIXED_VAL-(TEMP_COMP_COEFF*values.temperature))) + ERROR_CORRECTION; // True RH = (Sensor RH) / (1.0546 - 0.00216T)  where "T" is degrees celsius
  
  float dewpoint = log(values.RH/100) + (17.62 * values.temperature) / (243.12 + values.temperature);
  values.dewpoint = (243.12 * dewpoint / (17.62 - dewpoint));
  values.humidex  = (values.temperature+0.5555*(6.11*exp(5417.7530*((1/273.16)-(1/(273.16+values.dewpoint))))-10)); // apparent temp in C
}
