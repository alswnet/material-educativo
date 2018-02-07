#include <ESP8266WiFi.h>

const char* ssid = "TURBONETT_ALSW";
const char* password = "2526-4897";

WiFiServer server(80);
int val;

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  val = 1;
  pinMode(5, OUTPUT);
  digitalWrite(5, val);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/encender") != -1)
    val = 0;
  else if (req.indexOf("/apagada") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
   // client.stop();
   // return;
  }

  // Set GPIO2 according to the request
  digitalWrite(5, val);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<h1>Led ";
  s += (val) ? "Apagada" : "Encendida";
  s += "</h1>\r\n";
  s += "<a href=\"";
  s += (val) ? "encender" : "apagada";
  s += "\"> Cambiar</a> </html> \n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

