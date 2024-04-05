/*
  Titre        : Projet Final 1 - Collecteur de donnees a Serveur Web
  Auteur       : Dominique KOUASSI.
  Date         : 29/03/2024.
  Description  : Collecte des donnees a partir d'un DHT22 utilisant un serveur Web pour afficher les donnees
                 Un serveur Web asynchrone permet a lútilisateur d'afficher les donnees collectees.
                 l'utilisateur doit d'abord connecter sont WIFI au ESP32 en utilisant les paramettres suivants:
                 SSID: ESP32_AP.
                 password: 123456789
                 L'utilsateur doit saisir dans un navigateur l'adresse: 192.168.1.1 car l'ESP32 possede une adresse
                 statique.

  Version      : 0.0.1
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "Weather.hpp"

#define LED 33                          // LED pin.
#define DHTPIN 27                       // DHT pin.

String  etat_led;                       // LED state.
AsyncWebServer server(80);              // AsyncWebServer object on port 80.
const char* ssid     = "ESP32_AP";
const char* password = "123456789";

// Set ESP32 Static IP address.
IPAddress local_IP(192, 168, 1, 8);
IPAddress gateway(192, 168, 1, 8);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
IPAddress dhcp_lease_start(192, 168, 1, 10);

Weather my_weather(DHTPIN);             // Data collection object


/**
 * @brief Initialize the SPIFFS
 * 
 */
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else {
    Serial.println("SPIFFS mounted successfully");
  }
}

/**
 * @brief Initialize the wifi in Access Point mode
 * 
 */
void init_wifi_ap() {

  Serial.print("Setting AP (Access Point) for the ESP32…");
  // Configures static IP address
  if(!WiFi.softAPConfig(local_IP, gateway, subnet, dhcp_lease_start)){
    Serial.println("Failed to configure WIFI AP");
  }
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password, 10);
  // Print the IP
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

}

// Replaces placeholder in the HTML prior to send to client
String processor(const String& var){

  if(var == "TEMPERATURE") {
    //return "27.6, 28.8, 21.7, 34.1, 29.0, 28.4, 45.6, 51.7, 39.0, 60.0, 28.6, 32.1";
    return my_weather.get_temperature_str();
  } else if(var == "DATETIME") {
    //return "'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'";
    return my_weather.get_datetime_str();
  } else if(var == "HUMIDITY") {
    //return "27.6, 28.8, 21.7, 34.1, 29.0, 28.4, 45.6, 51.7, 39.0, 60.0, 28.6, 32.1";
    return my_weather.get_humidity_str();
  } else if(var == "AVGTEMP") {
    //return "26";
    return my_weather.get_avg_temperature_str();
  } else if(var == "AVGHUM") {
    //return "45";
    return my_weather.get_avg_humidity_str();
  } else if(var == "ETAT_LED") {
    if(digitalRead(LED)) {
      etat_led = "Allumee";
      }
    else {
      etat_led = "Eteinte";
    }
      return etat_led;
  }

  return String();
}


String get_graph_json_str(){

  String date_time = my_weather.get_datetime_str();

  String temperature_data = my_weather.get_temperature_str();
  String humidity_data = my_weather.get_humidity_str();

  String avg_temperature = my_weather.get_avg_temperature_str();
  String avg_humidity = my_weather.get_avg_humidity_str();

  //"{\"labels\":[\"2021-08-01\", \"2021-08-02\", \"2021-08-03\", \"2021-08-04\", \"2021-08-05\", \"2021-08-06\", \"2021-08-07\", \"2021-08-08\", \"2021-08-09\"], \"temperature\":[0, 0, 84, 76, 0 , 0, 84, 76, 50], \"humidity\":[1, 1, 1, 1, 0, 1, 1, 1, 50]}";
  String graph_json_str = "{\"labels\":[" + date_time + "]," + "\"temperature\":[" + temperature_data + "]," + "\"humidity\":[" + humidity_data + "]," + "\"avg_temperature\":" + avg_temperature + "," + "\"avg_humidity\":" + avg_humidity + "}";

  return graph_json_str;
}

void setup() {

  Serial.begin(115200);           // Initialize Serial Comm

  init_wifi_ap();
  initSPIFFS();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/update_graph", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("GET received:");

    String json = my_weather.get_graph_json_str();
    //String json =  "{\"labels\":[\"2021-08-01\", \"2021-08-02\", \"2021-08-03\", \"2021-08-04\", \"2021-08-05\", \"2021-08-06\", \"2021-08-07\", \"2021-08-08\", \"2021-08-09\"], \"temperature\":[0, 0, 84, 76, 0 , 0, 84, 76, 50], \"humidity\":[1, 1, 1, 1, 0, 1, 1, 1, 50]}";

    request->send(200, "application/json", json);
    json = String();

  });

  server.begin();

  pinMode(LED, OUTPUT);           // initialize digital LED pin  as an output.

}

void loop() {

  // Blink the LED and collect data
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  delay(2000);

  my_weather.process_weather();
  

}