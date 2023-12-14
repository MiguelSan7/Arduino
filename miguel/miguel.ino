int ledrojo  = 10, ledamarillo = 9, ledverde = 8;

void setup()
{
  Serial.begin(9600);
  pinMode(ledrojo, OUTPUT);
  pinMode(ledamarillo, OUTPUT);
  pinMode(ledverde, OUTPUT);
}

void loop() {
    encenderled(ledrojo);
    apagarled(ledamarillo);
    apagarled(ledverde);
    if (digitalRead(ledrojo) == HIGH) {
        Serial.println("LED_Rojo=1");
    } 
    delay(2000);

    apagarled(ledrojo);
    if(digitalRead(ledrojo) == LOW){
      Serial.println("LED_Rojo=0");
    }
    delay(1000);
    encenderled(ledamarillo);
    if (digitalRead(ledamarillo) == HIGH) {
        Serial.println("LED_Amarillo=1");
    } 
    delay(2000);
    apagarled(ledamarillo);
    if(digitalRead(ledamarillo) == LOW){
      Serial.println("LED_Amarillo=0");
    }
    delay(1000);
    for (int i = 0; i <= 3; i++) {
        encenderled(ledverde);
        if (digitalRead(ledverde) == HIGH) {
            Serial.println("LED_Verde=1");
        } 
        delay(2000);
        apagarled(ledverde);
        
        if(digitalRead(ledverde) == LOW){
        Serial.println("LED_Verde=0");
        } 
        delay(1000); 
    }
}
void encenderled(int led)
{
	digitalWrite(led, HIGH);
}

void apagarled(int led)
{
  	digitalWrite(led, LOW);
}