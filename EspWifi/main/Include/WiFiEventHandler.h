// #ifndef WIFI_EVENT_HANDLER_H
// #define WIFI_EVENT_HANDLER_H 

// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_mac.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "esp_netif_net_stack.h"
// #include "esp_netif.h"
// #include "nvs_flash.h"
// #include "lwip/inet.h"
// #include "lwip/netdb.h"
// #include "lwip/sockets.h"
// #include "lwip/lwip_napt.h"
// #include "lwip/err.h"
// #include "lwip/sys.h"
// #include "WiFi.h"


// class WiFiEventHandler
// {
// public:
//     WiFiEventHandler() = default;
//     WiFiEventHandler(WiFiEventHandler &&) = default;
//     WiFiEventHandler(const WiFiEventHandler &) = default;
//     WiFiEventHandler &operator=(WiFiEventHandler &&) = default;
//     WiFiEventHandler &operator=(const WiFiEventHandler &) = default;
//     ~WiFiEventHandler() = default;

//     static void registerEvent(WiFi* wifi_instance, esp_event_base_t event_base, int32_t event_id) 
//     {

//     }

//     static void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                                int32_t event_id, void *event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
//         wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
//         ESP_LOGI("TAG_AP", "Station "MACSTR" joined, AID=%d", MAC2STR(event->mac), event->aid);
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
//         wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *) event_data;
//         ESP_LOGI("TAG_AP", "Station "MACSTR" left, AID=%d", MAC2STR(event->mac), event->aid);
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//         ESP_LOGI("TAG_STA", "Station started");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
//         ESP_LOGI("TAG_STA", "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
//     }
// }

// private:
// };



// #endif // !WIFI_EVENT_HANDLER_H