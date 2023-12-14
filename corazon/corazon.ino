const int sensorPin = A0; // Conecta el sensor piezoeléctrico al pin analógico A0

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
}

void loop() {
  // Lee el valor analógico del sensor piezoeléctrico
  int sensorValue = analogRead(sensorPin);

  // Imprime el valor leído en el monitor serial
  Serial.println(sensorValue);

  // Espera un breve periodo de tiempo antes de la siguiente lectura
  delay(3000);
}
