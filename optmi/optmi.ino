int pines[] = {9, 11 , 13};
int tiempo = 5000;
String mensaje[] = {"V1","A1","R1"};
void setup() {
  for(int i = 0;i < 2;i++){ pinMode(pines[i], OUTPUT); }
  Serial.begin(9600);
}
void loop() {
  for(int i = 0; i < 3; i++){
    Prender(pines[i],mensaje[i],tiempo);
    if(i == 0){
      for(int u = 0; u<3; u++){
        digitalWrite(pines[0], LOW);
        Serial.println("0");
        delay(tiempo/5);
        Prender(pines[0],mensaje[0],tiempo/5);
      }
    }
    digitalWrite(pines[i], LOW);
  }
}
void Prender(int pin, String msg, int temp){
  digitalWrite(pin, HIGH);
        Serial.println(msg);
        delay(temp);
}