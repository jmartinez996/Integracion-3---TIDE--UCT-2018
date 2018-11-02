#include <SHT1x.h>
#include <NewPing.h> //libreria para sensor de proximidad

#define dataPin  2
#define clockPin 3
#define LEDPin 13 // Pin de diodo LED incorporado
#define PIRPin 4 // Pin de entrada


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

class spir{
  private:
    int value;
  public: 
    int GetValue(){
      value = digitalRead(PIRPin); //leemos el pin
      return value;
    }
};

void setup()
{
   Serial.begin(57600);
   //Serial.println("iniciando");
    pinMode(PIRPin, INPUT); // Configuramos el pin como entrada
}

void loop()
{
  int temp;
  int dist;
  shtx TH;
  //Muestra los valores de Sensores
  //Serial.print("Temperatura: ");
  temp = (TH.GetTemperatura());
  //Serial.print("Humedad: ");
  //Serial.print(TH.GetHumedad());
  //Serial.println("%");

  sProximity Px; //instacia clase sProximity
  //Muestra valores sensor de proximidad en cm
  
  dist = (Px.GetDistance());

  Serial.print("temp: ");
  Serial.println(temp);
  //Serial.print("|");
  Serial.print("distancia: ");
  Serial.println(dist);
  //Serial.print(dist);

  
  //Serial.print(",");  
  //Serial.println("cm");
  
  spir sp;
  //muestra valores del sensor de presencia
  //Serial.print("Presencia: ");
  //Serial.print(sp.GetValue());//imprimimos el 1 si detecta movimiento y 0 si no
  //Serial.print(",");
  //int temp = (TH.GetTemperatura(),1);
  //int dist = Px.GetDistance();
  //sprintf(buffer,"%d",dist);
  //Serial.println(buffer);

  delay(1000);
  
}
