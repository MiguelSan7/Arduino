#define BUZZER_PIN 2
#define BUZZER_CHANNEL 0

bool activarBuzzer = false;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  // Inicializar el módulo LEDC para el buzzer
  ledcSetup(BUZZER_CHANNEL, 1000, 8);  // Canal, frecuencia, resolución
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();  // Eliminar espacios en blanco al principio y al final

    if (comando == "encender") {
      activarBuzzer = true;
      Serial.println("Buzzer encendido");
    } else if (comando == "apagar") {
      activarBuzzer = false;
      Serial.println("Buzzer apagado");
    }
  }

  if (activarBuzzer) {
    encenderBuzzer(1000);
  } else {
    apagarBuzzer();
  }

  delay(100);
}

void encenderBuzzer(int frecuencia) {
  ledcWriteTone(BUZZER_CHANNEL, frecuencia);
  delay(200);
  ledcWriteTone(BUZZER_CHANNEL, 0);  // Apagar el tono
  delay(200);
}

void apagarBuzzer() {
  ledcWriteTone(BUZZER_CHANNEL, 0);  // Apagar el tono
}
