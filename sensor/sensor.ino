#include <SHT1x.h>

#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);

class shtx{
  private:
    float Temperatura;
    float Humedad;
  public:
    void setTemperatura(){
        Temperatura = sht1x.readTemperatureC();
      }
    void setHumedad(){
        Humedad = sht1x.readHumidity();
      }
    float GetTemperatura(){
        return Temperatura;
      }
    float GetHumedad(){
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
  TH.setTemperatura();
  TH.setHumedad();
  //Muestra los valores de Sensores
  Serial.print("Temperatura: ");
  Serial.print(TH.GetTemperatura(),1);
  Serial.print(" C / ");
  Serial.print("Humedad: ");
  Serial.print(TH.GetHumedad());
  Serial.println("%");

  delay(1000);
  
}
