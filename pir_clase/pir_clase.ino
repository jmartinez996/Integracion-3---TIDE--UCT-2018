#define LEDPin 13 // Pin de diodo LED incorporado
#define PIRPin 4 // Pin de entrada

class spir{
  private:
    int value;
  public: 
    int GetValue(){
      value = digitalRead(PIRPin); //sonar.ping_cm() envía un ping y devuelve distancia en cm, sino hay eco de ping dentro del limite de distancia establecido entonces devuelve 0
      return value;
    }
};

void setup() {
  Serial.begin(9600);
  pinMode(PIRPin, INPUT); // Configuramos el pin como entrada
}

void loop() {

  spir sp;
  /*
  if (sp.GetValue() == HIGH)
    { // Si es un valor alto:
      Serial.println("Hay");
    }
  else
    { // Si es un valor bajo:
      Serial.println("No Hay");
    }*/
  Serial.println(sp.GetValue());
  delay(100);
}

