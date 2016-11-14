//Codigo de ejemplo para el hola mundo ESP8266 Thing


void setup() {//Funcion que se ejecuta 1 ves para configurar 
  pinMode(LED_BUILTIN, OUTPUT); //Led GPIO 5 
}

// cliclo sin fin 
void loop() {
  digitalWrite(LED_BUILTIN, LOW);//Enciende el led   
  delay(1000);//Espera 1 segundo
  digitalWrite(LED_BUILTIN, HIGH);//Apaga el led
  delay(1000);//Espera 1 segundo
}
