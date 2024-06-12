#include "DHT.h"

// Define DHT11 GPIO
#define DHTPIN 4     // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
#define dht_relay 13

#define flame_sensor 5
#define led_flame_sensor 18

#define mq2_sensor 34
#define led_mq2 19

#define light_sensor 15
#define led_light_sensor  21
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 1000; // Interval in milliseconds for reading sensors

// Setup
void setup() {
  pinMode(flame_sensor, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(led_flame_sensor, OUTPUT);// initialize digital pin LED_BUILTIN as an output.

  pinMode(led_mq2, OUTPUT);
  analogWrite(led_mq2, LOW);

  pinMode(dht_relay, OUTPUT);

  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();

  attachInterrupt(flame_sensor, flamesensor, RISING);
}

// MAIN function
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    dht11_relay();
    light_sensor_function();
    flamesensor_off();
    print_sensor_MQ2();
    Serial.println("\n");
  }
}

// DHT11 FUNCTION
void read_dht11() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
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


// Control the relay based on dht11 temperature
void dht11_relay() {
  read_dht11(); // Read DHT11 sensor data
  float t = dht.readTemperature(); // Read temperature value
  if (isnan(t)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }
  
  // Control the relay based on temperature
  if (t >= 32.00) {
    digitalWrite(dht_relay, HIGH); // Turn on the relay
  } else {
    digitalWrite(dht_relay, LOW); // Turn off the relay
  }
}


// LIGHT SENSOR FUNCTION
void light_sensor_function() {
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100);
  Serial.print("Light output: ");
  Serial.println(light);
  if (light >= 300) {
    analogWrite(led_light_sensor, light);
  }
  else {
    analogWrite(led_light_sensor, LOW);
  }
}


// FLAME SENSOR FUNCTION
void flamesensor() {
  digitalWrite(led_flame_sensor, HIGH);
  Serial.println("** Fire detected!!! **");
}

void flamesensor_off() {
  digitalWrite(led_flame_sensor, LOW);
  Serial.println("No Fire detected");
}


// MQ2 SENSOR FUNCTION
// Read sensor
int readSensor_MQ2() {
  unsigned int sensorValue = analogRead(mq2_sensor);
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255);
  if (outputValue > 400) {
    analogWrite(led_mq2, outputValue);
  }
  else {
    analogWrite(led_mq2, LOW);
  }
  return outputValue;
}


// Print the value to terminal
void print_sensor_MQ2() {
  Serial.print("Analog MQ2 output: ");
  Serial.println(readSensor_MQ2());
}
