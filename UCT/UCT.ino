#include <SHT1x.h>
#include <NewPing.h> //libreria para sensor de proximidad
//Set pin de temperatura y humedad
#define dataPin  2
#define clockPin 3
//define pines para Led (Provisorios) //COMO SON PINES PROVISORIOS SACAR CAGANDO NOMAS 
#define LEDPin1 13
#define LEDPin2 12
#define LEDPin3 11
#define LEDPin4 10        // CAMBIAR ESTE PIN
//Set pin de Pir
#define PIRPin 4 // Pin de entrada
// PINES para motores
#define  izqA 5
#define  izqB 6
#define  derA 9
#define  derB 10
int velocidad = 255;          // velocidad de los motores (0-255)

//seteo de pines sensor proximidad
#define TRIGGER_PIN  6      // CAMBIAR ESTE PIN
#define ECHO_PIN     5      // CAMBIAR ESTE PIN
#define MAX_DISTANCE 200 //distancia maxima a identificar

//Iniciacion de Variables
int estado;
int temp;
int humedad;
int dist;
int piir;
String aData[4];


//Constructor sensor proximidad y temmperatura y humedad
SHT1x sht1x(dataPin, clockPin);
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
    pinMode(LEDPin1 , OUTPUT);
    pinMode(LEDPin2 , OUTPUT);
    pinMode(LEDPin3 , OUTPUT);
    pinMode(LEDPin4 , OUTPUT);

    //pinMode para control motores
    pinMode(izqA, OUTPUT);
    pinMode(izqB, OUTPUT);
    pinMode(derA, OUTPUT);
    pinMode(derB, OUTPUT);

}

void loop()
{
  shtx TH;
  temp = (TH.GetTemperatura());
  humedad =(TH.GetHumedad());
  aData[0] = String(temp);
  aData[1] = String(humedad);

  sProximity Px; //instacia clase sProximity
  //Muestra valores sensor de proximidad en cm
  dist = (Px.GetDistance());
  aData[2] = String(dist);

  spir sp;
  piir=(sp.GetValue());//imprimimos el 1 si detecta movimiento y 0 si no
  aData[3] = String(piir);

  String dv = ",";
  String data = aData[0]+dv+aData[1]+dv+aData[2]+dv+aData[3];
  Serial.println(data);
  
  /*
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println("%");
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.println("cm");
  Serial.print("Movimiento: ");
  Serial.println(piir);
  */
  Serial.println(2);
  if(Serial.available()>0){
     estado = 0;
     estado = Serial.read();
  
    if(estado =='1'){ //Ariba
         digitalWrite(LEDPin1 , HIGH);
         delay(1000);
         digitalWrite(LEDPin1 , LOW);
         //new
         analogWrite(derB, 0);     
         analogWrite(izqB, 0); 
         analogWrite(derA, velocidad);  
         analogWrite(izqA, velocidad);  
        
      }
    if(estado =='2'){ //Abajo  
        digitalWrite(LEDPin2 , HIGH);
        delay(1000);
        digitalWrite(LEDPin2 , LOW);
        //new
        analogWrite(derA, 0);    
        analogWrite(izqA, 0);
        analogWrite(derB, velocidad);  
        analogWrite(izqB, velocidad);  
      
    }
    if(estado =='3'){//Derecha
        digitalWrite(LEDPin3 , HIGH);
        delay(1000);
        digitalWrite(LEDPin3 , LOW);
        //new

        analogWrite(derB, 0);     
        analogWrite(izqB, 0);
        analogWrite(izqA, 0);
        analogWrite(derA, velocidad);  
    }
    if(estado =='4'){//Izquierda
        digitalWrite(LEDPin4 , HIGH);
        delay(1000);
        digitalWrite(LEDPin4 , LOW);
        //new
        analogWrite(derB, 0);     
        analogWrite(izqB, 0); 
        analogWrite(derA, 0);  
        analogWrite(izqA, velocidad);    
    } 
  } 
  
}
