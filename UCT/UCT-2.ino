#include <SHT1x.h>
#include <NewPing.h> //libreria para sensor de proximidad
#include <Servo.h> //libreria motores servo 360
//Set pin de temperatura y humedad
#define dataPin  2
#define clockPin 3
//Set pin de Pir
#define PIRPin 4 // Pin de entrada

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

int pin1 = 5; //motor servo izquierdo
int pin2 = 6; //motor servo derecho
int stop_position = 95; //manda el servo la orden de parar
int velocity = 30; //velocidad motor

//Se utiliza en modulo ver datos de App
String aData[4];
String dv = ",";
String data = "";

//Constructor sensor proximidad y temmperatura y humedad
SHT1x sht1x(dataPin, clockPin);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//Crea objetos servo
Servo servoLeft;
Servo servoRigth;

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
void MoveForward(){
    servoLeft.write(stop_position + velocity);     
    servoRigth.write(stop_position - velocity);         
    delay(7000); 
   
    servoLeft.write(95); 
    servoRigth.write(95); 
    delay(3000);
  }
  
void MoveBack(){
    servoLeft.write(stop_position - velocity);     
    servoRigth.write(stop_position + velocity);         
    delay(7000); 
   
    servoLeft.write(95); 
    servoRigth.write(95); 
    delay(3000);
}   

void ShutDownMotor(){
   servoLeft.write(95); 
   servoRigth.write(95);
   delay(200);
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
      //Movimiento1(bloque[i]);
    }
  }
}


void setup()
{
  Serial.begin(57600);
   //Serial.println("iniciando");
  pinMode(PIRPin, INPUT); // Configuramos el pin como entrada

    //pinMode para control motores
  servoLeft.attach(pin1);
  servoRigth.attach(pin2);

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
  
  char salir;
  while(Serial.available()>0){
    modulo = Serial.read();
    while(modulo=='p' || modulo=='r' || modulo=='v'){ 
      do{
        //Serial.println("Estoy en p1");
        salir = Serial.read();
        //Comienza movimiento motores
        if(modulo=='p'){
          ShutDownMotor();
          MoveForward();
          MoveBack();
        }
      }while(modulo=='p' && salir!='r' && salir!='v');
  
      if(salir=='r' || salir=='v'){
        modulo = salir;
      }
  
      do{
        //Serial.println("Estoy en p2");
        salir = Serial.read();
        if(modulo=='r'){
          ShutDownMotor();
        }
      }while(modulo=='r' && salir!='p' && salir!='v');
  
      if(salir=='p' || salir=='v'){
        modulo = salir;
      }

      do{
        //Serial.println("Estoy en sensores");
        salir = Serial.read();
        if(modulo=='v'){
          ShutDownMotor();
          data = aData[0]+dv+aData[1]+dv+aData[2]+dv+aData[3];
          Serial.println(data);
        }
      }while(modulo=='v' && salir!='p' && salir!='r');
  
      if(salir=='p' || salir=='r'){
        modulo = salir;
      }
    }
  }
}
