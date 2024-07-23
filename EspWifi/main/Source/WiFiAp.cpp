#include "WiFiAp.h"

esp_netif_t* WiFiAp::start()
{
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t* apNetif = esp_netif_create_default_wifi_ap();
    
    WiFiAp::Instance().SetApNetIf(apNetif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "YOUR_AP_SSID",
            .password = "YOUR_AP_PASSWORD",
            .ssid_len = strlen("YOUR_AP_SSID"),
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4
        },
    };

    wifi_request_t request = {
        .mode = WIFI_MODE_AP,
        .config = ap_config,
    };

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(request.mode));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &request.config));
    ESP_ERROR_CHECK(esp_wifi_start());

    return apNetif;
}

void WiFiAp::wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData) 
{
    if (eventBase == WIFI_EVENT) {
        if(eventId == WIFI_EVENT_AP_START)
        {
            ESP_LOGI("WIFI_EVENT", "AP start");
        }
        else if(eventId == WIFI_EVENT_AP_STOP)
        {
            ESP_LOGI("WIFI_EVENT", "AP stop");
        }
        else if(eventId == WIFI_EVENT_AP_STACONNECTED)
        {
            ESP_LOGI("WIFI_EVENT", "Station connected");
        }
        else if(eventId == WIFI_EVENT_AP_STADISCONNECTED)
        {
            ESP_LOGI("WIFI_EVENT", "Station disconnected");
        }
    }
    else if(eventBase == IP_EVENT)
    {
        ESP_LOGI("IP_EVENT", "IP event accured, code %d:", static_cast<int>(eventId));

        if(eventId == IP_EVENT_AP_STAIPASSIGNED)
        {
            ip_event_ap_staipassigned_t* event = static_cast<ip_event_ap_staipassigned_t*>(eventData);
            unsigned int ip = esp_netif_htonl(event->ip.addr);
            ESP_LOGI("IP_EVENT", "Got ip and registering device: %08X", ip);
            WiFi::registered_devices.insert(ip, event->mac); 
        }
    }
}

// void WiFiAp::wifiRequestHandler(void* pvParameters) 
// {
//     while (true) 
//     {
//         wifi_request_t request;
//         if (xQueueReceive(wifiApRequestQueue, &request, portMAX_DELAY) == pdTRUE) 
//         {
//             if (request.mode == WIFI_MODE_AP || request.mode == WIFI_MODE_APSTA) 
//             {
//                 ESP_ERROR_CHECK(esp_wifi_set_mode(request.mode));
//                 ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &request.config));
//                 ESP_ERROR_CHECK(esp_wifi_start());
//                 ESP_LOGI("WIFI_REQUEST_HANDLER", "SoftAP mode started");
//             }
//         }
        
//         vTaskDelete(NULL);
//     }
// }
