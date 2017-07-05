#define BLYNK_PRINT Serial //Para ver el depurador  - se puede quitar
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "baf15e71d8334ec2b00062dec2d329fe";//Contraseña Blynk
char ssid[] = "TURBONETT_ALSW";//Nombre de red
char pass[] = "2526-4897";//Contraseña

void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop(){
  Blynk.run();
}
