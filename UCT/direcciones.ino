int Mizq1 = 10; // pin digital 10
int Mder1 = 9; // pin digital 10
int Mizq2 = 6; // pin digital 10
int Mder2 = 5; // pin digital 10
class mov{  
  public:
    void up(){
      digitalWrite(Mizq1, HIGH); // asigna el valor HIGH al pin 
      digitalWrite(Mder1, HIGH); // asigna el valor HIGH al pin
      delay(500);
      digitalWrite(Mizq1, LOW); // asigna el valor HIGH al pin 
      digitalWrite(Mder1, LOW); // asigna el valor HIGH al pin
      delay(500);
    }
    void down(){
      digitalWrite(Mizq2, HIGH); // asigna el valor HIGH al pin 
      digitalWrite(Mder2, HIGH); // asigna el valor HIGH al pin
      delay(500);
      digitalWrite(Mizq2, LOW); // asigna el valor HIGH al pin 
      digitalWrite(Mder2, LOW); // asigna el valor HIGH al pin
      delay(500);
    }
    void Mizq(){
      digitalWrite(Mizq1, HIGH); // asigna el valor HIGH al pin 
      digitalWrite(Mder2, HIGH); // asigna el valor HIGH al pin
      delay(500);
      digitalWrite(Mizq1, LOW); // asigna el valor HIGH al pin 
      digitalWrite(Mder2, LOW); // asigna el valor HIGH al pin
      delay(500);
    }
    void Mder(){
      digitalWrite(Mizq2, HIGH); // asigna el valor HIGH al pin 
      digitalWrite(Mder1, HIGH); // asigna el valor HIGH al pin
      delay(500);
      digitalWrite(Mizq2, LOW); // asigna el valor HIGH al pin 
      digitalWrite(Mder1, LOW); // asigna el valor HIGH al pin
      delay(500);
    }
    
};

int direccion;
void setup() {
  Serial.begin(57600);
  pinMode(Mizq1, OUTPUT);
  pinMode(Mder1, OUTPUT);
  pinMode(Mizq2, OUTPUT);
  pinMode(Mder2, OUTPUT);
}
mov r1;
void loop() {

  if(Serial.available()>0){
     direccion = 0;
     direccion = Serial.read();
  
    if(direccion =='       ↑'){ //Arriba
         r1.up();
         delay(500);
      }
    if(direccion =='       ↓'){ //Abajo  
       r1.down();
       delay(500);
    }
    if(direccion =='       →'){//Derecha
        r1.Mder();
        delay(500);
    }
    if(direccion =='       ←'){//Izquierda
        r1.Mizq();
        delay(500);      
    } 
  } 
  
}
