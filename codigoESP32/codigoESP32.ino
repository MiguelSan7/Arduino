#include <Wire.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>
#include <MPU6050.h>

//Pines
//Pines GPS
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

//Pines Temperatura y Humedad
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Pin Vibracion
#define VIBRATION_PIN 5

//Pin Piezoelectrico
const int piezoSensorPin = 32;
const int umbralActividad = 1700;

//Acelerometro
MPU6050 mpu;

//Pin Buzzer
#define BUZZER_PIN 2
void MQTTsubCallback(char *data, uint16_t len) {
  // Convertir el dato recibido a un entero
  int value = atoi(data);

  // Si el valor es 1, encender el buzzer
  if (value == 1) {
    encenderBuzzer(1000);
  }
  // Si el valor es 0, apagar el buzzer
  else if (value == 0) {
    apagarBuzzer();
  }
}

//Conexiones Wifi y Adafruit
#define WLAN_SSID       "pocox5"
#define WLAN_PASS       "34567890" 
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "Kiilver"
#define AIO_KEY         "aio_mjOU46kWDd8tSrJvwTSLkVxxhg3g" 
#define AIO_GROUP       "correa-inteligente"    

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperaturaValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "temperatura");
Adafruit_MQTT_Publish humedadValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "humedad");
Adafruit_MQTT_Publish vibracionValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "vibracion");
Adafruit_MQTT_Publish latitudValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "latitud");
Adafruit_MQTT_Publish longitudValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "longitud");
Adafruit_MQTT_Publish aceleracionXValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "aceleracionX");
Adafruit_MQTT_Publish aceleracionYValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "aceleracionY");
Adafruit_MQTT_Publish aceleracionZValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "aceleracionZ");
Adafruit_MQTT_Publish actividadPiezoValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "piezoelectrico");
Adafruit_MQTT_Publish buzzerValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "buzzer");
Adafruit_MQTT_Subscribe buzzerSubscribe = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/" AIO_GROUP "buzzer");

bool connectedToWiFi = false;

void setup() {
  mpu.reset();
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(115200);
  delay(1000);
  Serial.print("Conectando a ");
  Serial.println(WLAN_SSID);
  if (!connectedToWiFi) {
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi conectado");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
    connectedToWiFi = true;
  }
  //Iniciar GPS y Acelerometro
  gpsSerial.begin(9600); 
  mpu.initialize();
  delay(1000);

  if (!mpu.testConnection()) {
    Serial.println("MPU-6050 connection failed. Please check your connections.");
  }
  else{
      Serial.println("MPU-6050 connection successful");
  }

  ledcSetup(0, 5000, 8);  // Canal 0, frecuencia 5000 Hz, resolución de 8 bits
  ledcAttachPin(BUZZER_PIN, 0);
  buzzerSubscribe.setCallback(MQTTsubCallback);
  mqtt.subscribe(&buzzerSubscribe);

}

void loop() {
  MQTT_connect();
  mqtt.processPackets(10000);
  //Temperatura y humedad
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  if (!isnan(temperatura) && !isnan(humedad)) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C, Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");

    temperaturaValue.publish(temperatura);
    humedadValue.publish(humedad);
  } else {
    Serial.println("Error al leer el sensor DHT11");
  }

  //GPS
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);  // Alimenta cada carácter al objeto TinyGPS++
  }

  if (gps.location.isUpdated()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    Serial.print("Latitud: ");
    Serial.print(latitude, 6);
    Serial.print(" | Longitud: ");
    Serial.println(longitude, 6);

    latitudValue.publish(latitude);
    longitudValue.publish(longitude);
  }
  else {
    Serial.println("Error al leer GPS");
  }
  if (gps.charsProcessed() < 10) {
    delay(100);
  }

  //Acelerometro
  int16_t ax_raw, ay_raw, az_raw;
  mpu.getAcceleration(&ax_raw, &ay_raw, &az_raw);
    // Factores de escala
    const float factorAceleracion = 9.81 / 16384.0;  // 1 g = 9.81 m/s², 1 g = 16384 LSB

    // Lecturas convertidas a unidades más comprensibles
    float aceleracionX = ax_raw * factorAceleracion;
    float aceleracionY = ay_raw * factorAceleracion;
    float aceleracionZ = az_raw * factorAceleracion;

    // Verificar si las lecturas son válidas antes de publicar
    if ((ax_raw == 0) && (ay_raw == 0) && (az_raw == 0)) {
      Serial.println("Lecturas del MPU-6050:");
      Serial.print("Aceleración en X: "); Serial.print(aceleracionX); Serial.println(" m/s²");
      Serial.print("Aceleración en Y: "); Serial.print(aceleracionY); Serial.println(" m/s²");
      Serial.print("Aceleración en Z: "); Serial.print(aceleracionZ); Serial.println(" m/s²");

      // Publicar las lecturas
      aceleracionXValue.publish(aceleracionX);
      aceleracionYValue.publish(aceleracionY);
      aceleracionZValue.publish(aceleracionZ);
    } else {
      Serial.println("Error: Lecturas no válidas del MPU-6050");
      }

  //Vibracion
  //int valorVibracion = digitalRead(VIBRATION_PIN);
  //vibracionValue.publish(valorVibracion);

  //Piezoelec
  int valorPiezo = analogRead(piezoSensorPin);
  // Verificar si el valor leído en el sensor piezoeléctrico es válido
  if (!isnan(valorPiezo)) {
    // Comprobar si el valor leído supera el umbral en el sensor piezoeléctrico
    if (valorPiezo > umbralActividad) {
      Serial.println("¡Actividad detectada en el sensor piezoeléctrico!");
        actividadPiezoValue.publish(valorPiezo);
    }
  } else {
    Serial.println("Error: Lectura no válida en el sensor piezoeléctrico");
  }


  delay(5000);
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Conectando a MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Reintentando conexión MQTT en 5 segundos...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("¡MQTT conectado!");
}
void encenderBuzzer(int frecuencia) {
  int buzzerEstado = 1;
  while( buzzerEstado == 1){
    tone(BUZZER_PIN, frecuencia);
  delay(200);
  noTone(BUZZER_PIN);
  delay(200);
  // Comprobar si se ha recibido un nuevo dato de Adafruit IO
    if (mqtt.readSubscription(100)) {
      // Si se ha recibido, actualizar el valor de buzzerEstado
      buzzerEstado = atoi((char *)buzzerSubscribe.lastread);
    }
  }
  
}
void apagarBuzzer() {
  noTone(BUZZER_PIN);
}
