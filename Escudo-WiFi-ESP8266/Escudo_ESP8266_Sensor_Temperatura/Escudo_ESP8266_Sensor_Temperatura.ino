#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>

const int pinSensor = 0;

const char ssid[] = "ALSW";
const char password[] = "42314212";
ESP8266Server server = ESP8266Server(80);

void setup() {
  Serial.begin(115200);
  IPAddress direccionIP;

  esp8266.begin();
  esp8266.setMode(ESP8266_MODE_STA);
  if (esp8266.connect(ssid, password) <= 0) {
    Serial.println(F("Error al conectarse a la red"));
    Serial.println(F("Presione reset para reintentar"));
    for (;;);
  }

  direccionIP = esp8266.localIP();
  Serial.print(F("Servidor listo, visite la direccion "));
  Serial.println(direccionIP);

  server.begin();
}

void loop() {
  boolean lineaEnBlanco = true;
  char c;

  ESP8266Client client = server.available(500);
  if (client) {
    Serial.println(F("Cliente conectado"));
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        if (c == '\n' && lineaEnBlanco) {
          Serial.println(F("Enviando pagina..."));
          delay(100);
          enviarPagina(client);
          delay(100);
          client.stop();
          Serial.println(F("Terminado"));
        }
        if (c == '\n')
          lineaEnBlanco = true;
        else if (c != '\r')
          lineaEnBlanco = false;
      }
    }
  }
}

void enviarPagina(ESP8266Client &client) {
  client.print(F("HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Connection: close\r\n\r\n"
                 "<!DOCTYPE html>\n"
                 "<html>\n"
                 "  <body>\n"
                 "  Temperatura: "));
  client.print(leerTemperatura());
  client.print(F("&degC \n"
                 "  <meta http-equiv=\"refresh\" content=\"5\">"
                 "  </body>\n"
                 "</html>\n"));
}

float leerTemperatura() {
  float voltaje, temperatura;
  voltaje = analogRead(pinSensor) * 0.004882814;
  temperatura = (voltaje - 0.5) * 100.0;
  return temperatura;
}

