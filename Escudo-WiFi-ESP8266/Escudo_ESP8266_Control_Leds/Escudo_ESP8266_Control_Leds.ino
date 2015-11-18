#include <SoftwareSerial.h>

#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>

const char ssid[] = "ALSW";
const char password[] = "42314212";
ESP8266Server server = ESP8266Server(80);
ESP8266Client cliente;

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
  char c;

  cliente = server.available(500);
  if (cliente) {
    Serial.println(F("Cliente conectado"));
    while (cliente.connected()) {
      if (cliente.available()) {
        c = cliente.read();
        Serial.write(c);
        procesarCaracteres(c);
      }
    }
  }
}

void procesarCaracteres(char c) {
  static int estado = 0;
  static boolean lineaEnBlanco = false;

  switch (estado) {
    case 0:
      if (c == '/')
        estado = 1;
      break;
    case 1:
      if (c == ' ')
        estado = 2;
      //else guardar caracter
      break;
    case 2:
      if (c == '\n' && lineaEnBlanco) {
        Serial.println(F("Enviando pagina..."));
        enviarPagina();
        delay(100);
        cliente.stop();
        Serial.println(F("Terminado"));

        estado = 0;
        lineaEnBlanco = false;
      }

      if (c == '\n')
        lineaEnBlanco = true;
      else if (c != '\r')
        lineaEnBlanco = false;
      break;

  }
}

void enviarPagina() {
  const char cabeceraHTTP[] = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: close\r\n\r\n";
  const char cabeceraHTML[] = "<!DOCTYPE html>\n"
                              "<html>\n"
                              "  Temperatura: ";
  const char terminadorHTML[] = "&degC \n"
                                " </html>\n";

  cliente.write(cabeceraHTTP, strlen(cabeceraHTTP));
  cliente.write(cabeceraHTML, strlen(cabeceraHTML));
  cliente.print(123);
  cliente.write(terminadorHTML, strlen(terminadorHTML));
}
