#include <SHT1x.h>
#include <NewPing.h> //libreria para sensor de proximidad

#define dataPin  2
#define clockPin 3

//seteo de pines sensor proximidad
#define TRIGGER_PIN  6
#define ECHO_PIN     5
#define MAX_DISTANCE 200 //distancia maxima a identificar

SHT1x sht1x(dataPin, clockPin);
//Constructor sensor proximidad
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

class shtx{
  private:
    float Temperatura;
    float Humedad;
  public:
    float GetTemperatura(){
        Temperatura = sht1x.readTemperatureC();
        return Temperatura;
      }
    float GetHumedad(){
        Humedad = sht1x.readHumidity();
        return Humedad;
      }
  };

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

void setup()
{
   Serial.begin(9600);
   Serial.println("iniciando");
}

void loop()
{
  shtx TH;
  //Muestra los valores de Sensores
  Serial.print("Temperatura: ");
  Serial.print(TH.GetTemperatura(),1);
  Serial.print(" C / ");
  Serial.print("Humedad: ");
  Serial.print(TH.GetHumedad());
  Serial.println("%");

  sProximity Px; //instacia clase sProximity
  //Muestra valores sensor de proximidad en cm
  Serial.print("Distancia: ");
  Serial.print(Px.GetDistance());  
  Serial.println("cm");
  
  delay(1000);
  
}
