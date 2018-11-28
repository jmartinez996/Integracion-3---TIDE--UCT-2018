#include <SHT1x.h>
#include <NewPing.h> //libreria para sensor de proximidad
//Set pin de temperatura y humedad
#define dataPin  2
#define clockPin 3
//Set pin de Pir
#define PIRPin 4 // Pin de entrada
// PINES para motores
#define  izqA 5
#define  izqB 6
#define  derA 9
#define  derB 10
int velocidad = 255;          // velocidad de los motores (0-255)

//seteo de pines sensor proximidad
#define TRIGGER_PIN  12      // pin cambiado de 6 a 12
#define ECHO_PIN     11      // pin cambiado de 5 a 11
#define MAX_DISTANCE 200 //distancia maxima a identificar

//Iniciacion de Variables
char modulo;  //recibe la informacion por serial, para ser filtrada en los movimientos 
int temp;
int humedad;
int dist;
int piir;

//Se utiliza en modulo ver datos de App
String aData[4];
String dv = ",";
String data = "";

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

//funcion que controla movimientos en las distintas direcciones
void Movimiento1(char mov){
  if(mov=='a'){ //Ariba
    digitalWrite(derB, 0);     
    digitalWrite(izqB, 0); 
    digitalWrite(derA, velocidad);  
    digitalWrite(izqA, velocidad);
    delay(500);
    digitalWrite(derA, 0); 
    digitalWrite(izqA, 0);  
  }
  
  if(mov=='b'){ //Abajo  
    digitalWrite(derA, 0);    
    digitalWrite(izqA, 0);
    digitalWrite(derB, velocidad);  
    digitalWrite(izqB, velocidad);
    delay(500);
    digitalWrite(derB, 0); 
    digitalWrite(izqB, 0);      
  }
  
  if(mov=='c'){//Derecha
    digitalWrite(derB, 0);     
    digitalWrite(izqB, 0);
    digitalWrite(izqA, 0);
    digitalWrite(derA, velocidad);
    delay(500);
    digitalWrite(derA, 0);  
  }
  
  if(mov=='d'){//Izquierda
    digitalWrite(derB, 0);     
    digitalWrite(izqB, 0); 
    digitalWrite(derA, 0);  
    digitalWrite(izqA, velocidad);
    delay(500);
    digitalWrite(izqA, 0);    
  }
  
  if(mov=='e'){//se detiene
    digitalWrite(derB, 0);     
    digitalWrite(izqB, 0); 
    digitalWrite(derA, 0);  
    digitalWrite(izqA, 0);    
  }  

  //Falta definir movimientos en 45 grados
}

//funcion que controla movimiento por bloques
void Movimiento2(String bloque){
  //Genera split a cadena de intrucciones
  for(int i=0; i<sizeof(bloque)-1; i++){
    /**Serial.print(i, DEC);
    Serial.print(" = ");
    Serial.write(bloque[i]);
    Serial.println();**/
    if(bloque[i]!=','){
      Movimiento1(bloque[i]);
    }
  }
}


void setup()
{
   Serial.begin(57600);
   //Serial.println("iniciando");
    pinMode(PIRPin, INPUT); // Configuramos el pin como entrada

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
  
  while(Serial.available()>0){
    modulo = Serial.read();

    do{
      char estado = Serial.read();
      Movimiento1(estado);
    }while(modulo=='p' && modulo!='q');
    
    do{
      String estado = String(Serial.read());
      Movimiento2(estado);
    }while(modulo=='r' && modulo!='s');
    
    do{
      data = aData[0]+dv+aData[1]+dv+aData[2]+dv+aData[3];
      Serial.println(data);
    }while(modulo=='v' && modulo!='w');
  }
}
