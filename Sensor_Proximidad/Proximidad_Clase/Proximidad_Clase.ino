#include <NewPing.h>

#define TRIGGER_PIN  6
#define ECHO_PIN     5
#define MAX_DISTANCE 200

//Constructor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

class sProximity{
  private:
    int Distance;
  public:
    int GetDistance(){
      Distance = sonar.ping_cm(); //sonar.ping_cm() envía un ping y devuelve distancia en cm, sino hay eco de ping dentro del limite de distancia establecido entonces devuelve 0
      return Distance;
      // Falta incorporar algun tipo de accion cuando distancia>200cm para que el robot continue en movimiento
    }
};

void setup() {
  Serial.begin(9600); //unidades de señal por segundo
}

void loop() {
  sProximity Px; //intacia clase sProximity
  //Muestra Valores sensor de proximidad en cm
  Serial.print("Distancia: ");
  Serial.print(Px.GetDistance());  
  Serial.println("cm");
  delay(1000);
}
