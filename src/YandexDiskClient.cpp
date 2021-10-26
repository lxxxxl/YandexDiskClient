#include "YandexDiskClient.h"

YandexDiskClient::YandexDiskClient(String auth_token)
{
    token = auth_token;
    client.setInsecure(); // do not check HTTPS certificate
}

bool YandexDiskClient::upload(String filename, uint8_t *buf, uint16_t len)
{
    String req = baseUrl + "resources/upload" + "?path=" + filename;

    uint16_t result = request("GET", req);
    if (result != HTTP_OK){
        Serial.printf("upload failed (%d): %s\n", result, req.c_str());
        return false;
    }

    String response = client.readString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    const char *href = doc["href"];

    if (href){
        result = request(doc["method"], href, buf, len);
        if ((result != HTTP_CREATED) && (result != HTTP_ACCEPTED)){
            Serial.printf("upload failed (%d): %s\n", result, href);
            return false;
        }
        return true;
    }
    return false;
}

bool YandexDiskClient::download(String filename, uint8_t *buf, uint16_t *len)
{
    String req = baseUrl + "resources/download" + "?path=" + filename;
    uint16_t result = request("GET", req);
    if (result != HTTP_OK){
        Serial.printf("upload failed (%d): %s\n", result, req.c_str());
        return false;
    }
    String response = client.readString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    const char *href = doc["href"];

    if (href){
        result = request(doc["method"], doc["href"]);
        if (result != HTTP_OK){
            Serial.printf("download failed (%d): %s\n", result, href);
            return false;
        }

        uint16_t _len;
        _len = client.readBytes(buf, *len);
        *len = _len;
        return true;
    }
    return false;
}

bool YandexDiskClient::mkdir(String path)
{
    String req = baseUrl + "resources" + "?path=" + path;
    uint16_t result = request("PUT", req);
    if (result != HTTP_CREATED)
        return false;
    return true;
}

bool YandexDiskClient::remove(String path)
{
    String req = baseUrl + "resources" + "?path=" + path;
    uint16_t result = request("DELETE", req);
    if ((result != HTTP_NO_CONTENT) && (result != HTTP_ACCEPTED))
        return false;
    return true;
}


void YandexDiskClient::parseUrl(String url, String *host, uint16_t *port, String* path)
{
    int index = url.indexOf(':');
    String protocol = url.substring(0, index);
    *port = (protocol == "https" ? 443 : 80);

    url.remove(0, (index + 3)); // remove protocol part
    index = url.indexOf('/');
    *host = url.substring(0, index);
    url.remove(0, index); // remove host part

    // check if there is port in url
    index = host->indexOf(':');
    if (index != -1){
        *port = host->substring(index+1).toInt();
        host->remove(index);
    }

    *path = url;
}

uint16_t YandexDiskClient::request(String method, String url, uint8_t* data, uint16_t len)
{
    String _host;
    uint16_t _port;
    String _path;

    parseUrl(url, &_host, &_port, &_path);

    if (!client.connect(_host, _port)) {
        Serial.println("connection failed");
        return -1;
    }

    // send HTTP headers
    client.print(method + " " + _path + " HTTP/1.1\r\n");
    client.print("Host: " + _host + "\r\n");
    client.print("Authorization: OAuth " + token + "\r\n");
    client.print("User-Agent: client\r\n");
    if (len != 0){
         client.print("Content-Length: " + String(len) + "\r\n");
    }
    client.print("Connection: close\r\n\r\n");
    if (data != NULL){
        for (uint16_t i = 0; i < len; i++){
            client.print(data[i]);
        }
    }

    // check response code
	String line = client.readStringUntil('\n');

    int index = line.indexOf(' ');
	uint16_t code = line.substring(index+1, index+4).toInt();

    // read out all headers
    while (client.connected()) {
        line = client.readStringUntil('\n');
        // process HTTP redirect
        if ((code == HTTP_MOVED_PERMANENTLY) || (code == HTTP_MOVED_TEMPORARILY)){
            index = line.indexOf("Location: ");
            if (index != -1){
                line = line.substring(strlen("Location: "), line.length()-1);
                return request(method, line, data, len);
            }
        }
        if (line == "\r") {
            break;
        }
    }
    return code;
}

void YandexDiskClient::disconnect()
{
    if (!client.connected())
        return;
    while(client.available() > 0)
        client.read();
    client.stop();
}
