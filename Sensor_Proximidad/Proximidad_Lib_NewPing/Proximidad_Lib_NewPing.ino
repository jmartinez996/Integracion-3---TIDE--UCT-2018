#include <NewPing.h>
 
#define TRIGGER_PIN  6
#define ECHO_PIN     5
#define MAX_DISTANCE 200

//Constructor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
 
void setup() {
  Serial.begin(9600); //unidades de señal por segundo
}
 
void loop() {
  delay(1000);
  Serial.print("Distancia: ");
  Serial.print(sonar.ping_cm()); //sonar.ping_cm() envía un ping y devuelve distancia en cm, sino hay eco de ping dentro del limite de distancia establecido entonces devuelve 0
  Serial.println("cm");
}
