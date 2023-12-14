int pines[] = {9, 11 , 13};
int tiempo = 5000;
String mensaje[] = {"V1","A1","R1"};
void setup() {
  for(int i = 0;i < 2;i++){ pinMode(pines[i], OUTPUT); }
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 3; i++){
    digitalWrite(pines[i-1], LOW);
    if(i == 0){
      digitalWrite(pines[0], HIGH);
        Serial.println(mensaje[0]);
        delay(tiempo);
      for(int u = 0; u<3; u++){
        digitalWrite(pines[0], LOW);
        Serial.println("0");
        delay(tiempo/5);
        digitalWrite(pines[0], HIGH);
        Serial.println(mensaje[0]);
        delay(tiempo/5);
      }
    }else{
        digitalWrite(pines[i], HIGH);
        Serial.println(mensaje[i]);
        delay(tiempo);
      }
    digitalWrite(pines[2], LOW);
  }
}