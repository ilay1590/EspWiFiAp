#ifndef WIFI_H
#define WIFI_H

#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#include <unordered_map>
#include <cstring> // For memcpy
#include <string>

class WiFi 
{
protected:
    #define MAC_SIZE 6
    using mac = uint8_t[MAC_SIZE];
    
    typedef struct {
        wifi_mode_t mode;
        wifi_config_t config;
    } wifi_request_t;

    WiFi();
    ~WiFi() = default;

    // virtual void wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    class IPtoMACMap {
    private:
        std::unordered_map<uint32_t, uint8_t[6]> ip_mac_map;

    public:
        // Overload operator[] to access elements
        uint8_t* operator[](uint32_t key) {
            return ip_mac_map[key];
        }

        // Const version of operator[]
        const uint8_t* operator[](uint32_t key) const {
            auto it = ip_mac_map.find(key);
            if (it != ip_mac_map.end()) {
                return it->second;
            }
            else {
                return nullptr;
            }
        }

        // Function to insert an IP-MAC pair into the map
        void insert(uint32_t ip, const uint8_t mac[6]) {
            std::memcpy(ip_mac_map[ip], mac, sizeof(uint8_t) * 6);
        }

        void clear()
        {
            ip_mac_map.clear();
        }
    };

    static IPtoMACMap registered_devices;

    // static void wifiRequestHandler(void* pvParameters) 
    // {
    //     while (true) 
    //     {
    //         wifi_request_t request;
    //         if (xQueueReceive(wifiRequestQueue, &request, portMAX_DELAY) == pdTRUE) {
    //             if (request.mode == WIFI_MODE_AP || request.mode == WIFI_MODE_APSTA) {
    //                 ESP_ERROR_CHECK(esp_wifi_set_mode(request.mode));
    //                 ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &request.config));
    //                 ESP_ERROR_CHECK(esp_wifi_start());
    //                 ESP_LOGI("WIFI_REQUEST_HANDLER", "SoftAP mode started");
    //             } else if (request.mode == WIFI_MODE_STA || request.mode == WIFI_MODE_APSTA) {
    //                 ESP_ERROR_CHECK(esp_wifi_set_mode(request.mode));
    //                 ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &request.config));
    //                 ESP_ERROR_CHECK(esp_wifi_start());
    //                 ESP_LOGI("WIFI_REQUEST_HANDLER", "Station mode started");
    //             }
    //         }
    //     }

    //     vTaskDelete(NULL);
    // }

};

#endif /* WIFI_H */
