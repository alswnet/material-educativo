//Librerias empleadas
#include <SoftwareSerial.h> //Requerida internamete por el escudo WiFi
#include <SparkFunESP8266WiFi.h>

//Configuracion de la red inalambrica
const char ssid[] = "ALSW";
const char password[] = "42314212";

//Objetos de clase para el servidor (arduino) y el cliente (navegador)
ESP8266Server server = ESP8266Server(80);
ESP8266Client cliente;

//Cadena usada para almacenar la ruta de la URL
String rutaUrl;

//Estado del LED conectado
bool estadoLed = false;

//Pin de conexion del LED
const int pinLed = 12;

void setup() {
  IPAddress direccionIP;

  //Inicializa el pin del LED
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, estadoLed);

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
  char c;

  //Espera un cliente el proximo medio segundo
  cliente = server.available(500);

  //Verifica si el cliente se conecto
  if (cliente) {
    Serial.println(F("Cliente conectado"));
    //El cliente se procesa mientras haya conexion activa
    while (cliente.connected()) {
      procesarCliente();
    }
  }
}

void procesarCliente() {
  static int estado = 0;
  static boolean lineaEnBlanco = false;
  char c;

  //Determina si hay caracteres disponibles viniendo del cliente
  if (cliente.available()) {
    //La siguiente accion a realizar depende del estado actual
    switch (estado) {
      case 0:
        //En el estado inicial, se espera la secuencia GET y solo
        //salta al siguiente estado si se encontro
        if (cliente.find("GET "))
          estado = 1;
        break;
      case 1:
        //Una vez encontrada la secuencia GET, se lee la URL solicitada,
        //la cual siempre termina con un espacio
        rutaUrl = cliente.readStringUntil(' ');
        Serial.print(F("Pagina solicitada: "));
        Serial.println(rutaUrl);

        //Dependiendo de la URL solicitada, se enciende o apaga el LED
        //(Nota: Si no se especifica ninguna ruta valida, no se hace
        //nada con el LED)
        if (rutaUrl == "/encender") {
          Serial.println(F("Encendiendo Led"));
          estadoLed = true;
          digitalWrite(pinLed, estadoLed);
        }
        if (rutaUrl == "/apagar") {
          Serial.println(F("Apagando Led"));
          estadoLed = false;
          digitalWrite(pinLed, estadoLed);
        }

        //Despues de la solicitud siempre se pasa al siguiente estado
        estado = 2;
        break;
      case 2:
        //En este estado se esperan 2 fines de linea consecutivos
        c = cliente.read();
        if (c == '\n' && lineaEnBlanco) {
          //Si ocrurren, el navegador ya envio la solicitud, asi que se
          //contesta enviando la pagina
          Serial.println(F("Enviando pagina..."));
          enviarPagina();
          delay(100);
          cliente.stop();
          Serial.println(F("Terminado"));

          //Una vez terminado se reinicia la secuencia del automata
          estado = 0;
          lineaEnBlanco = false;
        }

        if (c == '\n')
          //Si se recibio un fin de linea, se marca la bandera por si
          //viene un segundo
          lineaEnBlanco = true;
        else if (c != '\r')
          //Para todos los demas caracteres que no son fin de linea
          //(excepto retorno de carro) se desmarca la bandera
          lineaEnBlanco = false;
        break;
    }
  }
}

void enviarPagina() {
  //Cadenas de texto con los protocolos de comunicacion HTTP y HTML
  const char cabeceraHTTP[] = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: close\r\n\r\n";
  const char cabeceraHTML[] = "<!DOCTYPE html>\n"
                              "<html>\n"
                              "  Estado del LED: ";
  const char terminadorHTML_0[] = "Apagado \n"
                                  "  <br> <a href=\"/encender\"> Encender </a>\n"
                                  "</html>\n";
  const char terminadorHTML_1[] = "Encendido \n"
                                  "  <br> <a href=\"/apagar\"> Apagar </a>\n"
                                  "</html>\n";

  //El inicio de la pagina siempre es el mismo
  cliente.write(cabeceraHTTP, strlen(cabeceraHTTP));
  cliente.write(cabeceraHTML, strlen(cabeceraHTML));

  //El final de la pagina depende del estado del LED
  if (!estadoLed)
    cliente.write(terminadorHTML_0, strlen(terminadorHTML_0));
  else
    cliente.write(terminadorHTML_1, strlen(terminadorHTML_1));
}
