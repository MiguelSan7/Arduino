#include <Wire.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>
#include <MPU6050.h>

#define RX_PIN 16  // Pin RX2 del ESP32 conectado al TX del módulo GPS
#define TX_PIN 17  // Pin TX2 del ESP32 conectado al RX del módulo GPS

HardwareSerial gpsSerial(2);  // Utiliza el puerto serial 2 (RX2, TX2) para la comunicación con el módulo GPS
TinyGPSPlus gps;  // Crea un objeto TinyGPS++ para manejar los datos GPS

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define VIBRATION_PIN 5

MPU6050 mpu;

#define WLAN_SSID       "Totalplay-21A0"
#define WLAN_PASS       "SGCKAIZENISO9001" 
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "Kiilver"
#define AIO_KEY         "aio_kCbi91Aoq0YmCC6yC83Xvqy1NdBL" 
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

bool connectedToWiFi = false;

void setup() {
  mpu.reset();
  Wire.begin();
  Wire.setClock(100000);  // Establece la velocidad de I2C a 100 kHz (ajusta según sea necesario)
  Serial.begin(115200);
  delay(10);

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

  gpsSerial.begin(9600);  // Inicia la comunicación serial con el módulo GPS
  mpu.initialize();
  delay(1000);

  if (!mpu.testConnection()) {
    Serial.println("MPU-6050 connection failed. Please check your connections.");
    while (1);
  }

  Serial.println("MPU-6050 connection successful");
}

void loop() {
  MQTT_connect();

  float temperatura = dht.readTemperature();  // Lee la temperatura en grados Celsius
  float humedad = dht.readHumidity();  // Lee la humedad relativa

  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);  // Alimenta cada carácter al objeto TinyGPS++
  }

  int16_t ax_raw, ay_raw, az_raw;
  mpu.getAcceleration(&ax_raw, &ay_raw, &az_raw);

  // Factores de escala
  const float factorAceleracion = 9.81 / 16384.0;  // 1 g = 9.81 m/s², 1 g = 16384 LSB

  // Lecturas convertidas a unidades más comprensibles
  float aceleracionX = ax_raw * factorAceleracion;
  float aceleracionY = ay_raw * factorAceleracion;
  float aceleracionZ = az_raw * factorAceleracion;

  // Imprime las lecturas convertidas
  Serial.println("Lecturas del MPU-6050:");
  Serial.print("Aceleración en X: "); Serial.print(aceleracionX); Serial.println(" m/s²");
  Serial.print("Aceleración en Y: "); Serial.print(aceleracionY); Serial.println(" m/s²");
  Serial.print("Aceleración en Z: "); Serial.print(aceleracionZ); Serial.println(" m/s²");

  // Publica las lecturas del MPU-6050 en Adafruit IO
  aceleracionXValue.publish(aceleracionX);
  aceleracionYValue.publish(aceleracionY);
  aceleracionZValue.publish(aceleracionZ);

  if (gps.location.isUpdated()) {
    // Si la posición GPS ha sido actualizada
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    // Imprime las coordenadas en el Monitor Serie
    Serial.print("Latitud: ");
    Serial.print(latitude, 6);
    Serial.print(" | Longitud: ");
    Serial.println(longitude, 6);

    // Publica las coordenadas en Adafruit IO
    latitudValue.publish(latitude);
    longitudValue.publish(longitude);
  }

  if (gps.charsProcessed() < 10) {
    // Si no se ha procesado suficientes caracteres, es posible que no se haya recibido una sentencia completa
    delay(100);
  }

  // Resto del código para publicar temperatura, humedad y vibración en Adafruit IO
  if (!isnan(temperatura) && !isnan(humedad)) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C, Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");

    // Publica los datos en Adafruit IO
    temperaturaValue.publish(temperatura);
    humedadValue.publish(humedad);
  } else {
    Serial.println("Error al leer el sensor DHT11");
  }

  // Lee el valor del sensor de vibración
  int valorVibracion = digitalRead(VIBRATION_PIN);

  // Publica el valor del sensor de vibración en Adafruit IO
  vibracionValue.publish(valorVibracion);

  delay(10000);
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
