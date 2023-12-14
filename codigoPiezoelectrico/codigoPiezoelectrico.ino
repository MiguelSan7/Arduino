const int sensorPin = 32; // Pin analógico donde está conectado el sensor
const int umbralActividad = 1700; // Umbral para detectar actividad, ajusta según sea necesario

void setup() {
  Serial.begin(115200);
}

void loop() { 
  int sensorValue = analogRead(sensorPin);

  Serial.print("Valor leído: ");
  Serial.println(sensorValue);

  // Comprobar si el valor leído supera el umbral
  if (sensorValue > umbralActividad) {
    Serial.println("¡Actividad detectada en el sensor!");
    // Aquí puedes agregar lógica adicional, como enviar una notificación o activar una alarma.
  }

  delay(1000); // Puedes ajustar la frecuencia de muestreo según tus necesidades
}
