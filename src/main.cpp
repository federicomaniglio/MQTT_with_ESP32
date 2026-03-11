#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <cstdio>

#include "credentials.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_BROKER;
constexpr int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
    }

    espClient.setInsecure(); // per test TLS

    client.setServer(mqtt_server, mqtt_port);

    while (!client.connected()) {
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("MQTT connected");
        } else {
            delay(2000);
        }
    }
}

void loop() {
    client.loop();

    const int rssi = WiFi.RSSI();
    int qualityLevel = 0;

    if (rssi >= -50) {
        qualityLevel = 4;
    } else if (rssi >= -60) {
        qualityLevel = 3;
    } else if (rssi >= -70) {
        qualityLevel = 2;
    } else if (rssi >= -80) {
        qualityLevel = 1;
    } else {
        qualityLevel = 0;
    }

    const char* qualityLabel = "";

    switch (qualityLevel) {
        case 4:
            qualityLabel = "Eccellente";
            break;
        case 3:
            qualityLabel = "Buona";
            break;
        case 2:
            qualityLabel = "Discreta";
            break;
        case 1:
            qualityLabel = "Debole";
            break;
        default:
            qualityLabel = "Scarsa";
            break;
    }

    char payload[64];
    snprintf(payload, sizeof(payload), "%s: %d dBm", qualityLabel, rssi);

    client.publish("esp32/test", payload);
    delay(5000);
}