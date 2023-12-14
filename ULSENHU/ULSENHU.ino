#include <DHT.h>

int trigPin = 8;
int echoPin = 5;

#define DHTPIN 2   // El pin de datos del sensor DHT11 se conecta al pin 2
#define DHTTYPE DHT11  // Tipo de sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  dht.begin();  // Inicializar el sensor DHT11
}

void loop() {
  // Lectura del sensor de ultrasonido
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration / 58;

  Serial.print("US:");
  Serial.print("01:");
  Serial.println(distance);
  delay(1000);

  // Lectura del sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica si la lectura del DHT11 fue exitosa
  if (!isnan(humidity) && !isnan(temperature)) {
    Serial.print("TE:");
    Serial.print("01:");
    Serial.println(temperature);
    delay(1000);
    Serial.print("HU:");
    Serial.print("01:");
    Serial.println(humidity);
  } else {
    Serial.print(" | Error al leer el sensor DHT11");
  }

  delay(1000);
}
