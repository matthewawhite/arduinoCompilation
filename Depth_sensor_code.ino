#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

MS5803 sensor(ADDRESS_HIGH);

//Create variables to store results
float temperature_c, pressure_abs;


void setup() {
    Serial.begin(57600);
    //Retrieve calibration constants for conversion math.
    sensor.reset();
    sensor.begin();  
}

void loop() {
  
  // To measure to higher degrees of precision use the following sensor settings:
  // ADC_256 
  // ADC_512 
  // ADC_1024
  // ADC_2048
  // ADC_4096
    
  // Read temperature from the sensor in deg C. This operation takes about 
  temperature_c = sensor.getTemperature(CELSIUS, ADC_512);
  
  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);
  
  // Report values via UART
  Serial.print("#TPD=");
  Serial.print(temperature_c);
  Serial.print("|");
  //Serial.print("Pressure abs (mbar)= ");
  Serial.print(pressure_abs);
  Serial.print("|");
  //Serial.print("Depth (cm)= ");
  Serial.print(getDepth());
  Serial.println("|");
  
  delay(1000);

  }

  float getDepth()
  {
    float atm = 1013.25;  //atmospheric pressure
    float t = sensor.getTemperature(CELSIUS, ADC_512);
    float rho = 1000*(1 - (t+288.9414)/(508929.2*(t+68.12963))*pow((t-3.9863),2)); //freshwater density
    return 100.0*( (100.0*(sensor.getPressure(ADC_4096)-atm)) / (rho*9.81) );
  }
