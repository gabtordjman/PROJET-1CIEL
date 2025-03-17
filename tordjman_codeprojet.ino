// Wemos Weather - 2024-2026 
// Projet CIEL par Gabriel TORDJMAN BARRENQUI
// A téléverser sur l'ESP8266 (ou Wemos D1 Mini) pour démarrer le serveur WEB

#include <WiFiClient.h> //Pour la connexion a un réseau wifi défini ci-dessous
#include <ESP8266WebServer.h> //Bibliothèque pour le serveur web sur ESP8266
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h> //Bibliothèque pour la syncronisation de l'heure avec un serveur NTP
#include <DHT.h> // Ajout de la bibliothèque DHT
#include <Adafruit_Sensor.h>

#define ssid      "ATD3-TP"        // SSID
#define password  ""               // PASS

#define DHT_PIN 2 // Le PIN 2 correspond au D4 sur le Wemos
#define DHT_TYPE DHT11 // Type du capteur (DHT11)
// #define DHT_TYPE DHT22 // Type du capteur (DHT22)
DHT dht(DHT_PIN, DHT_TYPE);  // Création de l'objet DHT

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
ESP8266WebServer server(80);

String getPage() {
  timeClient.update(); // Mise à jour de l'heure à partir du serveur NTP
  String currentTime = timeClient.getFormattedTime(); // Récupère l'heure au format "HH:MM:SS"
  
  // Lecture de la température du capteur DHT11
  float temperature = dht.readTemperature();  // Lecture en Celsius
  
  if (isnan(temperature)) {
    temperature = -1;  // En cas d'échec de la lecture, afficher -1
  }
  
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>Serveur Meteo Wemos</title>";
  page += "<style> body { background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
  page += " #time { position: absolute; top: 10px; right: 10px; font-size: 20px; font-weight: bold; color: #0000FF; }</style>";
  page += "</head><body><h1>Wemos Weather</h1>";
  page += "<p>Bienvenue sur le serveur Wemos Weather, qui vous permet d'obtenir des informations météorologiques</p><br>";
  page += "<div id='time'>" + currentTime + "</div>";
  page += "<h3>La température actuelle est de : " + String(temperature) + " °C</h3>";

  page += "<br><br><p><a href='https://gabrieltordjman.fr'>- Gabriel TORDJMAN -</a></p>";
  page += "</body></html>";
  
  return page;
}

void handleRoot() {
  if (server.method() == HTTP_GET) {
    server.send(200, "text/html", getPage());
  }
  else if (server.method() == HTTP_POST) {
    server.send(200, "text/html", getPage());  
  }
}

void setup() {
  Serial.begin(115200);            // Initialise le moniteur série
  delay(10);
  
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connexion au Wi-Fi en cours ...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print(ssid);
  Serial.println("  OK");
  Serial.print("Votre adresse IP est : ");
  Serial.println(WiFi.localIP());
  Serial.println("Vous êtes maintenant connecté au Wi-Fi");
  
  dht.begin(); // Initialisation du capteur DHT
  
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP OK");
}

void loop() {
  server.handleClient();
  delay(1000);
}
