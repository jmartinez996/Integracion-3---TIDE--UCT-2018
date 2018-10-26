#include <SHT1x.h>

#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);

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

  delay(1000);
  
}
