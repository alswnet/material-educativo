//Librerias empleadas
#include <SoftwareSerial.h> //Requerida internamete por el escudo WiFi
#include <SparkFunESP8266WiFi.h>

void setup() {

  //Inicializa el puerto Serial para comunicacion con la PC
  Serial.begin(9600);

  //Comprueba que este presente el esp8266
  while (esp8266.begin() != true)
  {
    Serial.print("Error connecting to ESP8266.");
    delay(1000);
  }

  // Pone en pin 5 en salida, este pin tiene el led Azul
  esp8266.pinMode(5, OUTPUT);
}

void loop() {
  esp8266.digitalWrite(5, HIGH);
  delay(500);
  esp8266.digitalWrite(5, LOW);
  delay(500);
}
