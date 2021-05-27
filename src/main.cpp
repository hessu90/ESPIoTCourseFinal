
//Ladataan kirjastot
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Määritetään Serveri ja sen portti (Oletus http portti on 80)
AsyncWebServer server(80);

// Määritetään langattoman verkon tunnukset
const char *ssid = "";
const char *password = "";

// Määritetään kiinteä IP-osoite
IPAddress local_IP(192, 168, 8, 254);
IPAddress gateway(192, 168, 8, 45);
IPAddress subnet(255, 255, 255, 0);

const int potPin = 34; // <= Signaalipinni
const int pwm = 26;    // <= digitaalinen ulostulo pinni
int output_value;      // <= Analogisen signaalin vahvuus muuttuja

// Not found eli 404 errori sivu
void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

// Haetaan kosteus anturi piiriltä funktio
String getMoisture()
{

    
    digitalWrite(pwm, HIGH);                            // Kirjoitaan 1 power output pinniin
    delay(1000);                                        // Odotetaan sekunti = 1000 millisekuntia
    output_value = analogRead(potPin);                  // Luetaan Analogi signaali anturilta
    output_value = map(output_value, 4095, 0, 0, 100);  // Mapataan analogi value 0-100 välille

    // Serial = terminaaliin tulostettavat rivit/teksti
    Serial.print("Moisture : ");
    Serial.print(output_value);
    Serial.println("%");

    delay(1000);
    // Kirjoitetaan 0 power pinniin
    digitalWrite(pwm, LOW);
    return String(output_value);
}

void setup()
{

    Serial.begin(9600);                                  // Aloitetaan Serialmonitor
    pinMode(pwm, OUTPUT);                               // Määritetään PinMode power pinnille
    if (!WiFi.config(local_IP, gateway, subnet))       // Määritetään Wifille asetukset
    {
        Serial.println("STA Failed to configure");
    }
    WiFi.begin(ssid, password);                         // Aloitetaan Wifi yhteys
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true))                // Aloitetaan tiedostojärjestelmä
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Määritellään HTTP_GET reitit
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false);
    });


    server.on("/moisture", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/plain", getMoisture().c_str());
    });


    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/main.js", "text/plain");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop()
{
}