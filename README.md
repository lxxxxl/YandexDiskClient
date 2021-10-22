# Yandex Disk Client
Yandex Disk REST API client for ESP8266

## Installation

1. Install [ArduinoJson library](https://github.com/bblanchon/ArduinoJson)

2. Download actual library version and unpack it to `Arduino/libraries` directory. There should be this directory structure:

   `Arduino/libraries/YandexDIskClient/src`

## Usage

##### Create directory

```c++
YandexDiskClient disk(AUTH_TOKEN);
if (!disk.mkdir("/dir")){
  Serial.println("mkdir() failed");
  return;
 }
```

##### Upload file

```c++
YandexDiskClient disk(AUTH_TOKEN);
uint8_t buf[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x00};
uint16_t len = 8;
if (!disk.upload("/dir/test.txt", buf, len)){
  Serial.println("upload() failed");
  return;
}
```

##### Download file

```c++
YandexDiskClient disk(AUTH_TOKEN);
uint8_t buf[32];
uint16_t len = 32;
if (!disk.download("/dir/test.txt", (uint8_t*)&buf2, &len)){
  Serial.println("download() failed");
  return;
}
Serial.println((char*)&buf);
```

##### Remove directory

```c++
YandexDiskClient disk(AUTH_TOKEN);
if (!disk.remove("/dir")){
  Serial.println("remove() failed");
  return;
}
```

Please check [example](https://github.com/lxxxxl/YandexDiskClient/blob/main/examples/APITest/APITest.ino) for more info.

### TODO
1. Add support for other API methods

2. Add actual Yandex certificate fingerprint

