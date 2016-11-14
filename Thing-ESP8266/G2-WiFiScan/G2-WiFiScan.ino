//Codigo de Ejemplo para buscar redes
#include "ESP8266WiFi.h"//Librerias integrada de wifi 

void setup() {
  Serial.begin(9600);//Levanta la comunicacion serial con la PC

  //Pone el ESP8266 es modo repetidor y se desconecta anteriores de otros AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Configuracion Lista");
}

void loop() {
  Serial.println("Inciando scan");

  //  WiFi.scanNetworks() devolera el numero de desdes encontradas
  int n = WiFi.scanNetworks();
  Serial.println("scan listo");
  if (n == 0)
    Serial.println("No se encontro redes");
  else
  {
    Serial.print(n);
    Serial.println(" redes encontrada");
    for (int i = 0; i < n; ++i)
    {
      //Imprime el nombre de la red, intensidas y si tiene contraseña de cada red
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "Seguridad");
      delay(10);
    }
  }
  Serial.println("");

  // espera 5 segundos antes de empezar de nuevo 
  delay(5000);
}
