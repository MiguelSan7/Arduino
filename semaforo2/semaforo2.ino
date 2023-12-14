int pinRojo = 13;
int pinAmarillo = 11;
int pinVerde = 9;
int tiempo = 5000;
int parpadeo = 1000;

void setup() {
  pinMode(pinRojo, OUTPUT);
  pinMode(pinAmarillo, OUTPUT);
  pinMode(pinVerde, OUTPUT);

  Serial.begin(9600);
}

void loop() {
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinAmarillo, LOW);
    digitalWrite(pinVerde, HIGH);
    Serial.println("V1");
    delay(tiempo);
    digitalWrite(pinVerde, LOW);
    Serial.println("0");
    delay(parpadeo);
    digitalWrite(pinVerde, HIGH);
    Serial.println("V1");
    delay(parpadeo);
    digitalWrite(pinVerde, LOW);
    Serial.println("0");
    delay(parpadeo);
    digitalWrite(pinVerde, HIGH);
    Serial.println("V1");
    delay(parpadeo);

    digitalWrite(pinRojo, LOW);
    digitalWrite(pinAmarillo, HIGH);
    digitalWrite(pinVerde, LOW);
    Serial.println("A1");

    delay(tiempo);

    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinAmarillo, LOW);
    digitalWrite(pinVerde, LOW);
    Serial.println("R1");

    delay(tiempo);

}
