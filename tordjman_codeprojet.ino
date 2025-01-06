// Projet 1CIEL - Wemos Weather 
// Réalisé par Gabriel TORDJMAN - 2024-2025

#include <WiFiClient.h> //Pour la connexion a un réseau wifi défini ci-dessous
#include <ESP8266WebServer.h> //Bibliothèque pour le serveur web sur ESP8266
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h> //Bibliothèque pour la syncronisation de l'heure avec un serveur NTP

#define ssid      "ATD3-TP"        // SSID
#define password  ""               // PASS
#define DHT_PIN 2 // Le PIN GPIO2 correspond au "D4" sur le Wemos
#define DHT_TYPE DHT11
// #define DHT_TYPE DHT22 // Celui-ci est en commentaire car j'utiliserai un DHT11, fourni par l'enseignant

// Création objet WIFI UDP puis du client NTP
WiFiUDP ntpUDP;

// Mise à jour toutes les 60 secondes
// Serveur NTP : europe.pool.ntp.org
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// Création des objets / create Objects

ESP8266WebServer server(80);

String getPage() { // CODE HTML DE LA PAGE D'ACCUEIL
  timeClient.update(); // Mise à jour de l'heure à partir du serveur NTP
  String currentTime = timeClient.getFormattedTime(); // Récupère l'heure au format "HH:MM:SS"
  
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>Serveur Meteo Wemos</title>";
  page += "<style> body { background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
  page += " #time { position: absolute; top: 10px; right: 10px; font-size: 20px; font-weight: bold; color: #0000FF; }</style>";
  page += "</head><body><h1>Wemos Weather</h1>";

  // Affiche l'heure en haut à droite
  page += "<div id='time'>" + currentTime + "</div>";

  // Le pin DATA sur le capteur DHT11 pour le Wemos est le n°D4. Celui-ci va permettre de communiquer avec le capteur

  page += "<h3>La temperature actuelle est de :</h3>";
  page += "<br><br><p><a href='http://gabrieltordjman.fr'>- Gabriel TORDJMAN -</a></p>";
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
  
  // Attente de la connexion au réseau WiFi 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Affiche dans le moniteur série le SSID et l'adresse IP attribuée
  Serial.println("");
  Serial.print(ssid);
  Serial.println("  OK");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.println("Vous êtes maintenant connecté");
   
  // Branche la fonction qui gère la première page 
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Serveur HTTP OK");
}

void loop() {
  server.handleClient();
  delay(1000);
}
