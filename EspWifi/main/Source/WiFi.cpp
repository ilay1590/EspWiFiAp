#include "wifi.h"

WiFi::IPtoMACMap WiFi::registered_devices;

WiFi::WiFi() 
{
    ESP_ERROR_CHECK(esp_netif_init());
}
