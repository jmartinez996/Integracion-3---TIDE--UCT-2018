
const int LEDPin= 13; // Pin de diodo LED incorporado
const int PIRPin= 2; // Pin de entrada
void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT); // Configuramos el pin como salida
  pinMode(PIRPin, INPUT); // Configuramos el pin como entrada
}

void loop() {
  int value= digitalRead(PIRPin); // Leemos el pin
  if (value == HIGH)
  { // Si es un valor alto:
    digitalWrite(LEDPin, HIGH); // Encendemos el LED
  } else
  { // Si es un valor bajo:
    digitalWrite(LEDPin, LOW); // Apagamos el LED
  }
  Serial.println(value);
  delay(100);
}
