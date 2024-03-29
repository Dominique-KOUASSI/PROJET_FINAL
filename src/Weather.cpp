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

#include "Weather.hpp"
#include <stdio.h>
#include <vector>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DS3231.h>
#include <Wire.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#define DHTTYPE DHT22               // Type de capteur DHT utilisé (DHT11, DHT22, AM2302, etc.).
#define COLLECTION_DELAY 60000      // collecte data every minute     

//Weather::Weather(){};
Weather::Weather(int pin_for_dht22): dht22_pin(pin_for_dht22), dht(pin_for_dht22, DHTTYPE) {
    
    // Start the I2C interface
	Wire.begin();
    this->dht.begin();

    this->timer.startTimer(60000);

};

/**
 * @brief Get the temperature str to build the graph
 * 
 * @return String 
 */
String Weather::get_temperature_str() {
  String temperature_str;

  for (int i = 0; i < weather_table.size(); i++) {
    temperature_str += weather_table[i].temperature;
    if(i < weather_table.size() - 1){
      temperature_str += ',';
    }
  }

  return temperature_str;
}

/**
 * @brief Get the humidity str to build the graph
 * 
 * @return String 
 */
String Weather::get_humidity_str() {
  String humidity_str;

  for (int i = 0; i < weather_table.size(); i++) {
    humidity_str += weather_table[i].humidity;
    if (i < weather_table.size() - 1) {
      humidity_str += ',';
    }
  }

  return humidity_str;
}

/**
 * @brief Get the datetime str to build the graph
 * 
 * @return String 
 */
String Weather::get_datetime_str() {
  String datetime_str;

  for (int i = 0; i < weather_table.size(); i++) {
    datetime_str += (String)"'" + weather_table[i].date_time + "'";
    if (i < weather_table.size() - 1) {
      datetime_str += ',';
    }
  }

  return datetime_str;
}

String Weather::get_avg_temperature_str() {
  double avg(0);

  for(int i(0); i < weather_table.size(); ++i)
  {
    avg += weather_table[i].temperature;   // Add all temperature
  }

  avg /= weather_table.size();

  return (String)avg;
}

String Weather::get_avg_humidity_str() {
  double avg(0);

  for(int i(0); i < weather_table.size(); ++i)
  {
    avg += weather_table[i].humidity;   // Add all temperature
  }

  avg /= weather_table.size();

  return (String)avg;
}

/**
 * @brief Collect data every COLLECTION_DELAY, default is every minute
 * 
 */
void Weather::process_weather() {  

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    //Serial.println((String)"Record: " + humidity + " " + temperature);

    if (this->timer.isTimerReady()) {

        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temperature)) {
            Serial.println(F("Failed to read from DHT sensor!"));
        } else {
            // Fill structure.
            current_weather.temperature = temperature;
            current_weather.humidity = humidity;
            sprintf(current_weather.date_time, "20%02d-%02d-%02d %02d:%02d:%02d", my_rtc.getYear(), my_rtc.getMonth(century), my_rtc.getDate(), my_rtc.getHour(h12Flag, pmFlag), my_rtc.getMinute(), my_rtc.getSecond());

            // Save structure into vector.
            if (weather_table.size() < RECORD_MAX_SIZE) {
            weather_table.push_back(current_weather);
            } else {
            weather_table.erase(weather_table.begin()); // Remove the first element.
            weather_table.push_back(current_weather);   // Add.
            }
        }


        this->timer.startTimer(60000);
    }

}