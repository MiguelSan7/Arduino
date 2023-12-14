const int sensorPin = A0; // Conecta el sensor piezoeléctrico al pin analógico A0

int threshold = 50; // Umbral para detectar cambios en la presión
int previousValue = 0; // Valor previo del sensor

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
}

void loop() {
  // Lee el valor analógico del sensor piezoeléctrico
  int sensorValue = analogRead(sensorPin);

  // Calcula la diferencia entre el valor actual y el valor previo
  int difference = abs(sensorValue - previousValue);

  // Si la diferencia supera el umbral, se detecta un cambio en la presión
  if (difference > threshold) {
    Serial.print(sensorValue);
    Serial.println("   ¡Se detectó un cambio en la presión! Respiración detectada.");
  }

  // Actualiza el valor previo con el valor actual para la próxima comparación
  previousValue = sensorValue;

  // Espera un breve periodo de tiempo antes de la siguiente lectura
  delay(100);
}
