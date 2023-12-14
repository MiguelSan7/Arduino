#define BUZZER_PIN 2  // Reemplaza con el número del pin al que está conectado el buzzer

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Encender el buzzer
  encenderBuzzer();
  delay(250);  // Mantener el buzzer encendido durante 2 segundos

  // Apagar el buzzer
  apagarBuzzer();
  delay(100);  // Esperar 1 segundo
}

void encenderBuzzer() {
  tone(BUZZER_PIN, 1000);  // Frecuencia de 1000 Hz, ajusta según tus necesidades
  Serial.println("Buzzer encendido");
}

void apagarBuzzer() {
  noTone(BUZZER_PIN);
  Serial.println("Buzzer apagado");
}
