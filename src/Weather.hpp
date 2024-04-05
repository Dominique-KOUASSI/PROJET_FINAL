/*
  Titre        : Projet Final 1 - Collecteur de donnees
  Auteur       : Dominique KOUASSI.
  Date         : 29/03/2024.
  Description  : Collecte des donnees a partir d'un DHT22.
                 Les donnees sont enregistrees dans un tableau dynamique dont la taille peut etre modifiee dans la constante
                 RECORD_MAX_SIZE.
                 Si le nombre d'enregistrement atteint RECORD_MAX_SIZE, le premier enregistrement est supprime du tableau 
                 dynamique avant d'ajouter un nouvel element en sorte que le tableau conserve toujours RECORD_MAX_SIZE
                 nombre d'elements.

  Version      : 0.0.1
*/

#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DS3231.h>
#include <vector>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "Timer.h"

#define RECORD_MAX_SIZE 10  // Max data record allowed.

struct weather_type {
  char date_time[20];       // Date and time in mysql format 2024-03-28 17:35:00.
  float temperature;        // Temperature read from DHT22.
  float humidity;           // Humidity data.
};
typedef struct weather_type weather_type_t;   // Define a new type to hold weather data.

class Weather
{
    private:
        int dht22_pin;

        DHT dht;
        DS3231 my_rtc;
        bool century = false;
        bool h12Flag;
        bool pmFlag;

        float humidity;
        float temperature;

        std::vector<weather_type_t> weather_table;    // Empty vector.
        weather_type_t current_weather;

        String graph_json_str;

        Timer timer;
        void build_graph_json_str();


    public:
        //Weather();
        Weather(int pin_for_dht22);

        String get_temperature_str();
        String get_humidity_str();
        String get_datetime_str();
        String get_avg_temperature_str();
        String get_avg_humidity_str();
        String get_graph_json_str();

        void process_weather();

};

#endif // WEATHER_HPP