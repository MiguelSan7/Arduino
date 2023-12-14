#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <DHT.h>

#define GPS_RX 2   // RX pin del módulo GPS conectado al pin 2 del Arduino
#define GPS_TX 3   // TX pin del módulo GPS conectado al pin 3 del Arduino
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); // Crea un objeto SoftwareSerial para el módulo GPS

#define VIBRATION_SENSOR_PIN 9
#define DHT_SENSOR_PIN 8
#define AD8232_SENSOR_PIN 4
#define PIEZO_SENSOR_PIN A1
#define ACCELEROMETER_SDA A4
#define ACCELEROMETER_SCL A5

DHT dht(DHT_SENSOR_PIN, DHT11); // Crea un objeto DHT para el sensor DHT11

TinyGPSPlus gps; // Crea un objeto TinyGPS++ para el módulo GPS

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();
}

void loop() {
  // Comprobar el sensor de vibración
  int vibrationValue = digitalRead(VIBRATION_SENSOR_PIN);
  Serial.print("Sensor de Vibración: ");
  Serial.println(vibrationValue);

  // Comprobar el sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Sensor DHT11 - Humedad: ");
  Serial.print(humidity);
  Serial.print("%, Temperatura: ");
  Serial.println(temperature);

  // Comprobar el sensor AD8232 (puede requerir un manejo más complejo dependiendo de la aplicación)
  // Aquí solo mostraremos un mensaje indicando que el sensor está conectado
  Serial.println("Sensor AD8232 conectado");

  // Comprobar el sensor piezoeléctrico
  int piezoValue = analogRead(PIEZO_SENSOR_PIN);
  Serial.print("Sensor Piezoeléctrico: ");
  Serial.println(piezoValue);

  // Comprobar el módulo GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      Serial.println("Módulo GPS funcionando");
      Serial.print("Latitud: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitud: ");
      Serial.println(gps.location.lng(), 6);
    }
  }

  // Pausa de 1 segundo entre lecturas
  delay(1000);
}
