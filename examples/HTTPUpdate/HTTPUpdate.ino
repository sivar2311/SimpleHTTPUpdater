#include <Arduino.h>
#include <WiFi.h>

#include <SimpleHTTPUpdate.h>
#include "version.h"

const char* wifi_ssid = "your_wifi_ssid";
const char* wifi_pass = "your_wifi_pass";

const char* firmware_download_url = "http://server/firmware.bin";
const char* firmware_info_url     = "http://server/firmware.nfo";

void setupWiFi() {
    WiFi.begin(wifi_ssid, wifi_pass);
    while (WiFi.status() != WL_CONNECTED) yield();
    Serial.println("WiFi: Connected");
}

void setupSimpleHTTPUpdate() {
    SimpleHTTPUpdate.onBegin([](int version) { Serial.printf("Update: Updating to version %d\r\n", version); });
    SimpleHTTPUpdate.onProgress([](size_t progress, size_t total) { Serial.printf("Update: %d / %d\r\n", progress, total);  });
    SimpleHTTPUpdate.onEnd([]() { Serial.println("Update finished. Rebooting..."); ESP.restart(); });
    SimpleHTTPUpdate.onError([](SimpleHTTPUpdateError_t err) { Serial.printf("Update error: %s\r\n", err); });
    SimpleHTTPUpdate.setInterval(10);
    SimpleHTTPUpdate.begin(firmware_download_url, firmware_info_url, FIRMWARE_BUILD);
}

void setup() {
    Serial.begin(115200);
    Serial.printf("Current built: %d\r\n", FIRMWARE_BUILD);

    setupWiFi();
    setupSimpleHTTPUpdate();
}

void loop() {
    SimpleHTTPUpdate.handle();
}