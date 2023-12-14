#include <Wire.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RX_PIN 16  // Pin RX2 del ESP32 conectado al TX del módulo GPS
#define TX_PIN 17  // Pin TX2 del ESP32 conectado al RX del módulo GPS

HardwareSerial gpsSerial(2);  // Utiliza el puerto serial 2 (RX2, TX2) para la comunicación con el módulo GPS
TinyGPSPlus gps;  // Crea un objeto TinyGPS++ para manejar los datos GPS

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);  // Inicia la comunicación serial con el módulo GPS

  Serial.println("Verificando el funcionamiento del módulo GPS...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c); 
  Serial.print(c); // Alimenta cada carácter al objeto TinyGPS++
  }

  if (gps.location.isUpdated()) {
    // Si la posición GPS ha sido actualizada
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    // Imprime las coordenadas en el Monitor Serie
    Serial.print("Latitud: ");
    Serial.print(latitude, 6);
    Serial.print(" | Longitud: ");
    Serial.println(longitude, 6);

    // Ahora puedes utilizar las variables 'latitude' y 'longitude' para tu aplicación
    // Por ejemplo, puedes construir una solicitud para la API de Google Maps aquí
  }

  if (gps.charsProcessed() < 10) {
    // Si no se ha procesado suficientes caracteres, es posible que no se haya recibido una sentencia completa
    delay(100);
  }
}
