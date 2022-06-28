# SimpleHTTPUpdater

Library for automatic generated FIRMWARE_BUILD numbers and for retrieving firmware updates from HTTP servers.

# Installation (PlatformIO)

Add the following line to your `platformio.ini`:
```
lib_deps = https://github.com/sivar2311/SimpleHTTPUpdater
```

# Use

### Including the automatically generated `version.h`

```C++
#include "version.h"
```

### Define the download URLs for `firmware.bin` and `firmware.nfo`:
```C++
const char* firmware_download_url = "http://server/firmware.bin";
const char* firmware_info_url     = "http://server/firmware.nfo";
```

### Initialize the library in `setup()`:

```C++
SimpleHTTPUpdate.begin(firmware_download_url, firmware_info_url, FIRMWARE_BUILD);
```

### Call the library's handle function periodically in `loop()`:
```C++
SimpleHTTPUpdate.handle();
```
*Note: `FIRMWARE_BUILD`is defined in `version.h` and increases automatically on each firmware-build*

# Compile & Upload to your webserver
Compile your project and upload the new versions of `firmware.bin` and `firmware.h` to your webserver.

# Example
For details, see the example: [HTTPUpdate.ino](./examples/HTTPUpdate/HTTPUpdate.ino)
# How it works (PlatformIO)

The library maks use of PlatformIO scripting and creates and updates automatically the files `version.h` in your source folder and `firmware.nfo` in the project folder. 

The version number increases on each firmware build.

`SimpleHTTPUpdate.handle()` checks periodically if new firmware updates are available on the web server.
If a higher version is found, it will be downloaded and installed automatically.