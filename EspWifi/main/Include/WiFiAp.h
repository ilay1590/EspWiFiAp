#ifndef WIFI_STA_H
#define WIFI_STA_H

#include "WiFi.h"
#include <functional>
#include "Singleton.h"
#include <string.h>

class WiFiAp : public WiFi, public Singleton<WiFiAp>
{
    friend class Singleton<WiFiAp>;
public:
    ~WiFiAp() = default;

    static esp_netif_t* start();

    uint8_t* GetName()
    {
        return _ssid_name;
    }

    uint8_t* GetPassword()
    {
        return _password;
    }

    uint8_t& GetMaxConnection()
    {
        return _max_connection;        
    }

    esp_netif_t* GetApNetIf()
    {
        return _apNetif;        
    }

    void SetApNetIf(esp_netif_t* apNetif)
    {
        _apNetif = apNetif;
    }

    static void wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

private:
    uint8_t _ssid_name[32];
    uint8_t _password[64];
    uint8_t _max_connection;
    esp_netif_t* _apNetif;
    
    inline WiFiAp() : WiFi() 
    {
    }
};

#endif /* WIFI_STA_H */ 