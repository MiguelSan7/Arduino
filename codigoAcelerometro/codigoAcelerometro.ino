#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  mpu.initialize();
  delay(1000);

  if (!mpu.testConnection()) {
    Serial.println("MPU-6050 connection failed. Please check your connections.");
    while (1);
  }

  Serial.println("MPU-6050 connection successful");
}

void loop() {
  int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;

  mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

  // Factores de escala
  const float factorAceleracion = 9.81 / 16384.0;  // 1 g = 9.81 m/s², 1 g = 16384 LSB
  const float factorGiroscopio = 1.0 / 131.0;      // 1 grado/segundo = 131 LSB

  // Lecturas convertidas a unidades más comprensibles
  float aceleracionX = ax_raw * factorAceleracion;
  float aceleracionY = ay_raw * factorAceleracion;
  float aceleracionZ = az_raw * factorAceleracion;

  float velocidadAngularX = gx_raw * factorGiroscopio;
  float velocidadAngularY = gy_raw * factorGiroscopio;
  float velocidadAngularZ = gz_raw * factorGiroscopio;

  // Imprime las lecturas convertidas
  Serial.println("Lecturas del MPU-6050:");
  Serial.print("Aceleración en X: "); Serial.print(aceleracionX); Serial.println(" m/s²");
  Serial.print("Aceleración en Y: "); Serial.print(aceleracionY); Serial.println(" m/s²");
  Serial.print("Aceleración en Z: "); Serial.print(aceleracionZ); Serial.println(" m/s²");

  Serial.println("--------");

  delay(1000);
}
