#include "DHT.h"


// Define DHT11 GPIO
#define DHTPIN 4     // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors

#define flame_sensor 5
#define led_flame_sensor 18

#define mq2_sensor 34
#define led_mq2 19

#define light_sensor 15
#define led_light_sensor  21
DHT dht(DHTPIN, DHTTYPE);


// Setup
void setup() 
{
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(led_flame_sensor, OUTPUT);// initialize digital pin LED_BUILTIN as an output.

  pinMode(led_mq2, OUTPUT);
  analogWrite(led_mq2, LOW);
  

  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();

  attachInterrupt(flame_sensor, flamesensor, RISING);
  // attachInterrupt(mq2_sensor, ISR_MQ2, RISING);
}


// MAIN function
void loop()
{
  dht11();
  light_sensor_function();
  flamesensor_off();
  print_sensor_MQ2();
  delay(1000); // add a delay to only read and print every 1 second
}


// DHT11 FUNCTION
void dht11() 
{
  // Wait a few seconds between measurements.
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  
  // Compute heat index in Celsius (Fahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("  Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}


// LIGHT SENSOR FUNCTION
void light_sensor_function() 
{
  int raw_light = analogRead(light_sensor); // read the raw value from light_sensor pin (A3)
  int light = map(raw_light, 0, 1023, 0, 100); // map the value from 0, 1023 to 0, 100
 
  Serial.print("Light level: "); 
  Serial.println(light); // print the light value in Serial Monitor

  if(light >= 300)
  {
    analogWrite(led_light_sensor, light);
  }
  else
  {
    analogWrite(led_light_sensor, LOW);
  }

}


// FLAME SENSOR FUNCTION
void flamesensor()
{
  digitalWrite(led_flame_sensor, HIGH); // Led ON
  Serial.println("** Fire detected!!! **");
}

void flamesensor_off()
{
  digitalWrite(led_flame_sensor, LOW); // Led OFF
  Serial.println("No Fire detected");
  //delay(100);
}


// MQ2 SENSOR FUNCTION
// Read sensor
int readSensor_MQ2() 
{
  unsigned int sensorValue = analogRead(mq2_sensor);  // Read the analog value from sensor
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data

  if (outputValue > 400)
  {
    analogWrite(led_mq2, outputValue); // generate PWM signal
  }
  else
  {
    analogWrite(led_mq2, LOW);
  }
  return outputValue;             // Return analog moisture value

}

// Print the value to terminal
void print_sensor_MQ2()
{
  Serial.print("Analog MQ2 output: ");

  Serial.println(readSensor_MQ2());

  Serial.print("\n"); 
  //delay(500);
  
}




