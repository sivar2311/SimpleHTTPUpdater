#include "SimpleHTTPUpdate.h"

#include <Update.h>

SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_OK                 = "OK";
SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_INFO_NOT_FOUND     = "Firmware info not found";
SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_FIRMWARE_NOT_FOUND = "Firmware not found";

void SimpleHTTPUpdateClass::setClient(const WiFiClient& client) {
    this->client = client;
}

/**
 * @brief Check if a firmware-info file is located at the given URL and return its value
 * The firmware-info file must contain a signle line with the provided firmware version as an integer
 *
 * @param url URL to the firmware-info file
 * @return available firmware version (provided by the firmware-info file) or 0 if no firmware-info file was found
 */
int SimpleHTTPUpdateClass::available() {
    int version = 0;
    if (!firmware_info_url.length()) return version;

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(client, firmware_info_url);
    int http_code = http.GET();
    if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
        version = http.getString().toInt();
    } else {
        if (error_fn) error_fn(SIMPLEHTTP_UPDATE_INFO_NOT_FOUND);
    }
    http.end();
    return version;
}

/**
 * @brief Update firmware from provided URL
 *
 * @param url download URL pointing to the firmware file
 * @return true download and flass was successful
 * @return false  download or flash was not successful -> use getError()
 */
bool SimpleHTTPUpdateClass::update() {
    if (!firmware_url.length()) return false;

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(client, firmware_url);

    int http_code = http.GET();
    if (http_code != HTTP_CODE_OK && http_code != HTTP_CODE_MOVED_PERMANENTLY) {
        if (error_fn) error_fn(SIMPLEHTTP_UPDATE_FIRMWARE_NOT_FOUND);
        return false;
    }

    int totalLength = http.getSize();

    if (!Update.begin(totalLength)) return false;

    WiFiClient* stream  = http.getStreamPtr();
    size_t      written = Update.writeStream(*stream);

    bool success = Update.end();
    if (end_fn) end_fn();

    return success;
}

void SimpleHTTPUpdateClass::setInterval(unsigned long interval) {
    this->update_interval = interval * 1000;
}

int SimpleHTTPUpdateClass::getError() {
    return Update.getError();
}

void SimpleHTTPUpdateClass::onProgress(UpdateClass::THandlerFunction_Progress fn) { Update.onProgress(fn); };
void SimpleHTTPUpdateClass::onBegin(THandlerFunction_Begin fn) { begin_fn = fn; }
void SimpleHTTPUpdateClass::onEnd(THandlerFunction_End fn) { end_fn = fn; }
void SimpleHTTPUpdateClass::onError(THandlerFunction_Error fn) { error_fn = fn; }

void SimpleHTTPUpdateClass::begin(String firmware_url, String firmware_info_url, int current_version) {
    this->firmware_url      = firmware_url;
    this->firmware_info_url = firmware_info_url;
    this->current_version   = current_version;
}

void SimpleHTTPUpdateClass::handle() {
    if (!current_version) return;
    if (!firmware_url.length()) return;
    if (!firmware_info_url.length()) return;

    unsigned long current_millis = millis();
    if (last_check && current_millis - last_check < update_interval) return;
    last_check = current_millis;

    int version = available();
    if (version > current_version) {
        if (begin_fn) begin_fn(version);
        update();
    }
}

SimpleHTTPUpdateClass SimpleHTTPUpdate;