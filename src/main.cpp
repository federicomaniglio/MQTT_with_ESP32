#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

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
    client.publish("esp32/test", "Ciao dal esp32");
    delay(5000);
}
