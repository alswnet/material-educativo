//Librerias empleadas
#include <SoftwareSerial.h> //Requerida internamete por el escudo WiFi
#include <SparkFunESP8266WiFi.h>

//Pin usado por el sensor de temperatura
const int pinSensor = 0;

//Configuracion de la red inalambrica
const char ssid[] = "ALSW";
const char password[] = "42314212";

//Objetos de clase para el servidor (arduino) y el cliente (navegador)
ESP8266Server server = ESP8266Server(80);
ESP8266Client cliente;

void setup() {
  IPAddress direccionIP;

  //Inicializa el puerto serie para comunicarse con la PC
  Serial.begin(115200);

  //Inicializa el modulo WiFi
  esp8266.begin();
  esp8266.setMode(ESP8266_MODE_STA);  //Modo estacion (cliente WiFi)
  if (esp8266.connect(ssid, password) <= 0) {
    //Si no se puede conectar emite un error y para el programa
    Serial.println(F("Error al conectarse a la red"));
    Serial.println(F("Presione reset para reintentar"));
    for (;;);
  }

  //Comienza a servir la pagina web
  server.begin();

  //Se lee la direccion IP y se indica al usuario a donde ingresar
  direccionIP = esp8266.localIP();
  Serial.print(F("Servidor listo, visite la direccion "));
  Serial.println(direccionIP);
}

void loop() {
  boolean esperarInicio = true;
  boolean lineaEnBlanco = true;
  char c;

  //Espera un cliente el proximo medio segundo
  cliente = server.available(500);

  //Verifica si el cliente se conecto
  if (cliente) {
    Serial.println(F("Cliente conectado"));
    //El siguiente codigo se repite mientras haya conexion activa
    while (cliente.connected()) {
      //Determina si hay caracteres disponibles viniendo del cliente
      if (cliente.available()) {
        //Si los hay, espera inicialmente a que aparezca el caracter
        //dos puntos (:) solo una vez
        if (esperarInicio) {
          if (cliente.find(":"))
            esperarInicio = false;
        }
        else {
          //Una vez recibidos los dos puntos, se leen los caracteres de
          //manera normal
          c = cliente.read();
          //Se envia una copia de la informacion recibida a la terminal
          Serial.write(c);
        }

        //A continuacion se esperan 2 fines de linea consecutivos
        if (c == '\n' && lineaEnBlanco) {
          //Si ocrurren, el navegador ya envio la solicitud, asi que se
          //contesta enviando la pagina
          Serial.println(F("Enviando pagina..."));
          enviarPagina(cliente);
          delay(100);     //Espera brevemente antes de cerrar la conexion
          cliente.stop(); //Cierra la conexion
          Serial.println(F("Terminado"));
        }
        if (c == '\n')
          //Si se recibio un fin de linea, se marca la bandera por si
          //viene un segundo
          lineaEnBlanco = true;
        else if (c != '\r')
          //Para todos los demas caracteres que no son fin de linea
          //(excepto retorno de carro) se desmarca la bandera
          lineaEnBlanco = false;
      }
    }
  }
}

void enviarPagina(ESP8266Client & cliente) {
  //Cadenas de texto con los protocolos de comunicacion HTTP y HTML
  const char cabeceraHTTP[] = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: close\r\n\r\n";
  const char cabeceraHTML[] = "<!DOCTYPE html>\n"
                              "<html>\n"
                              "  <body>\n"
                              "    Temperatura: ";
  const char terminadorHTML[] = "&degC \n"
                                "    <meta http-equiv=\"refresh\" "
                                "content=\"5\">\n"
                                "  </body>\n"
                                "</html>\n";

  //Se procede a enviar la pagina con la temperatura
  cliente.write(cabeceraHTTP, strlen(cabeceraHTTP));
  cliente.write(cabeceraHTML, strlen(cabeceraHTML));
  cliente.print(leerTemperatura());
  cliente.write(terminadorHTML, strlen(terminadorHTML));
}

float leerTemperatura() {
  float voltaje, temperatura;

  //Se lee el voltaje del ADC y se calcula la temperatura para retornarla
  voltaje = analogRead(pinSensor) * 0.004882814;
  temperatura = (voltaje - 0.5) * 100.0;
  return temperatura;
}
