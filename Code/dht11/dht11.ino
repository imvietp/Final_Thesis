#include "DHT.h"
#define DHTPIN 4     // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
#define dht_relay_cooler 13
#define dht_relay_warmer 12
#define dht_relay_humid 14

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();
  pinMode(dht_relay_cooler, OUTPUT);
  pinMode(dht_relay_warmer, OUTPUT);
  pinMode(dht_relay_humid, OUTPUT);
}


void loop() {
  dht11_function(); // Read DHT11 sensor data
  float t = dht.readTemperature(); // Read temperature value
  float h = dht.readHumidity();
  // Control the relay based on temperature
  if (t > 33.75) 
  {
    // If temperature is higher than 33.6 degrees Celsius, turn on the cooler relay and turn off the warmer relay
    digitalWrite(dht_relay_cooler, HIGH);
    digitalWrite(dht_relay_warmer, LOW); // Ensure the warmer relay is off
  } 
  else if (t <= 33.75)
  {
    // If temperature is less than or equal to 33.6 degrees Celsius, turn on the warmer relay and turn off the cooler relay
    digitalWrite(dht_relay_cooler, LOW); // Ensure the cooler relay is off
    digitalWrite(dht_relay_warmer, HIGH);
  }

  // Control the relay based on HUMIDITY
  if (h < 70)
  {
    digitalWrite(dht_relay_humid, HIGH);
  }
  else
  {
    digitalWrite(dht_relay_humid, LOW); 
  }
}


void dht11_function(){
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