#pragma once

#include <HTTPClient.h>
#include <WString.h>

using SimpleHTTPUpdateError_t = const char*;

extern SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_OK;
extern SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_INFO_NOT_FOUND;
extern SimpleHTTPUpdateError_t SIMPLEHTTP_UPDATE_FIRMWARE_NOT_FOUND;

using THandlerFunction_Begin    = std::function<void(int)>;
using THandlerFunction_End      = std::function<void(void)>;
using THandlerFunction_Error    = std::function<void(SimpleHTTPUpdateError_t)>;
using THandlerFunction_Progress = std::function<void(size_t, size_t)>;

class SimpleHTTPUpdateClass {
public:
    void begin(String firmware_url, String firmware_info_url, int current_version);
    void handle();
    void setInterval(unsigned long interval);
    int  available();
    bool update();
    int  getError();
    void onBegin(THandlerFunction_Begin fn);
    void onEnd(THandlerFunction_End fn);
    void onError(THandlerFunction_Error fn);
    void onProgress(THandlerFunction_Progress fn);
    void setClient(WiFiClient* client);
protected:
    HTTPClient    http;
    String        firmware_url;
    String        firmware_info_url;
    int           update_interval = 300000;
    int           current_version = 0;
    unsigned long last_check      = 0;
    WiFiClient*    client         = nullptr;

    THandlerFunction_Begin begin_fn;
    THandlerFunction_End   end_fn;
    THandlerFunction_Error error_fn;
};

extern SimpleHTTPUpdateClass SimpleHTTPUpdate;