//Librerias a utilizar
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>
#include <Phant.h>

const char mySSID[] = "ALSW";
const char myPSK[] = "21241324";

//Datos del servidor https://data.sparkfun.com/
const char phantServer[] = "data.sparkfun.com";
const char publicKey[] = "OGEQjwOb8ViOX9QpXmAV";
const char privateKey[] = "8b16W25EYwSaxGDJxlyB";

Phant phant(phantServer, publicKey, privateKey);

int Boton = 2;
int Sensor = A0;

void setup() {
  Serial.begin(9600);

  //Inicializa el modulo WiFi
  esp8266.begin();
  esp8266.setMode(ESP8266_MODE_STA);  //Modo estacion (cliente WiFi)
  if (esp8266.connect(mySSID, myPSK) <= 0) {
    //Si no se puede conectar emite un error y para el programa
    Serial.println(F("Error al conectarse a la red"));
    Serial.println(F("Presione reset para reintentar"));
    for (;;);
  }

  pinMode(Boton, INPUT);
}

void loop() {
  if (digitalRead(Boton) == 1) {
    //Si el boton es precionado envia un dato
    Serial.println("Empezando rutina");
    EnviarPhant();
  }
}

void EnviarPhant() {
  ESP8266Client client;

  //Intenta conectarse con el servidor
  if (client.connect(phantServer, 80) <= 0)
  {
    Serial.println(F("Error conectando con el Servidor"));
    return;
  }
  Serial.println(F("Conectando con el Servido"));

  //Obtiene la temperatura
  int Temperatura = ObtenerTemperatura(Sensor);

  //Agrega el dato a enviar
  phant.add(F("temp1"), Temperatura);

  //Envia a Phant
  Serial.println(F("Enviando a  Phant!"));
  client.print(phant.post());

  //Imprime la respuesta del sercidor
  while (client.available())
    Serial.write(client.read());


  //si el ciente esta vivo lo cierra
  if (client.connected())
    client.stop();
}

int ObtenerTemperatura(int Sensor) {
  float voltage, degreesC;
  voltage = analogRead(Sensor) * 0.004882814;
  degreesC = (voltage - 0.5) * 100.0;
  return degreesC;
}
