#ifndef YANDEXDISK_H_
#define YANDEXDISK_H_

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class YandexDiskClient
{
    public:
        YandexDiskClient(String auth_token);
        bool upload(String filename, uint8_t *buf, uint16_t len);
        bool download(String filename, uint8_t *buf, uint16_t *len);
        bool mkdir(String path);
        bool remove(String path);

    private:
        uint16_t request(String method, String url, uint8_t* data = NULL, uint16_t len = 0);
        void parseUrl(String url, String *host, uint16_t *port, String* path);
        void disconnect();

        WiFiClientSecure client;
        String token;
        uint16_t code;

        const String baseUrl = "https://cloud-api.yandex.net/v1/disk/";  // URL for API requests

        enum HttpResponse{
            HTTP_OK = 200,
            HTTP_CREATED = 201,
            HTTP_ACCEPTED = 202,
            HTTP_NO_CONTENT = 204,
            HTTP_MOVED_PERMANENTLY = 301,
            HTTP_MOVED_TEMPORARILY = 302
        };
};

#endif /* YANDEXDISK_H_ */
