#include <SHT1x.h>

#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);

void setup()
{
   Serial.begin(9600);
   Serial.println("iniciando");
}

void loop()
{
  float temperatura;
  float humedad;
  //Lee los sensores
  temperatura = sht1x.readTemperatureC();
  humedad = sht1x.readHumidity();

  //Muestra los valores de Sensores
  Serial.print("Temperatura: ");
  Serial.print(temperatura,1);
  Serial.print(" C / ");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println("%");

  delay(1000);
}
