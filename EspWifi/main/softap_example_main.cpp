/* UDP MultiCast Send/Receive Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "esp_mac.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"


#include "WiFiAp.h"

static const char *TAG = "multicast";

int CreateUdpSocket(int port)
{
    struct sockaddr_in saddr = {};
    int sock = -1;
    int err_code = 0;
    int reuse = 1;
    int opt = 1;
    uint8_t ttl = 1;

    ESP_LOGI(TAG, "Create socket");

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket. Error %d", errno);
        return -1;
    }

    ESP_LOGI(TAG, "Reuse address");

    err_code = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (err_code < 0) {
        ESP_LOGE(TAG, "setsockopt SO_REUSEADDR err");
        goto err;
    }

    ESP_LOGI(TAG, "Packet info");
    // Enable IP_PKTINFO
    err_code = setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));
    if (err_code < 0) {
        ESP_LOGE(TAG, "setsockopt IP_PKTINFO err");
        goto err;
    }

    ESP_LOGI(TAG, "Bind");

    // Bind the socket to any address
    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    err_code = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (err_code < 0) {
        ESP_LOGE(TAG, "Failed to bind socket. Error %d", errno);
        goto err;
    }

    ESP_LOGI(TAG, "TTL");

    // Assign multicast TTL (set separately from normal interface TTL)
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(uint8_t));
    if (err_code < 0) {
        ESP_LOGE(TAG, "Failed to set IP_MULTICAST_TTL. Error %d", errno);
        goto err;
    }

    // All set, socket is configured for sending and receiving
    return sock;

err:
    close(sock);
    return -1;
}

void JoinMulticast(int sock, const char* multicast_address)
{
    struct ip_mreq imreq = { 0 };
    struct in_addr iaddr = { 0 };
    int err = 0;

    esp_netif_ip_info_t ip_info = { 0 };
    err = esp_netif_get_ip_info(WiFiAp::Instance().GetApNetIf(), &ip_info);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get IP address info. Error 0x%x", err);
        goto err;
    }

    inet_addr_from_ip4addr(&iaddr, &ip_info.ip);
    err = inet_aton(multicast_address, &imreq.imr_multiaddr.s_addr);
    if (err != 1) {
        ESP_LOGE(TAG, "Configured IPV4 multicast address '%s' is invalid.", multicast_address);
        // Errors in the return value have to be negative
        err = -1;
        goto err;
    }
    ESP_LOGI(TAG, "Configured IPV4 Multicast address %s", inet_ntoa(imreq.imr_multiaddr.s_addr));
    if (!IP_MULTICAST(ntohl(imreq.imr_multiaddr.s_addr))) {
        ESP_LOGW(TAG, "Configured IPV4 multicast address '%s' is not a valid multicast address. This will probably not work.", multicast_address);
    }

    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
                        sizeof(struct in_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Failed to set IP_MULTICAST_IF. Error %d", errno);
        goto err;
    }

    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                         &imreq, sizeof(struct ip_mreq));
    if (err < 0) {
        ESP_LOGE(TAG, "Failed to set IP_ADD_MEMBERSHIP. Error %d", errno);
        goto err;
    }

 err:
}

void print_millis(const char *message) {
    // Get the current time in microseconds
    int64_t time_us = esp_timer_get_time();
    // Convert to milliseconds
    int64_t time_ms = time_us / 1000;
    printf("%lld %s\n", time_ms, message);
}

void Receive(int sock)
{
    while (1)
    {
        // ESP_LOGI(TAG, "Receive");
        // Incoming datagram received
        char recvbuf[1500];

        struct iovec iov = {
            .iov_base = recvbuf,
            .iov_len = sizeof(recvbuf)
        };

        // Prepare to receive packets
        char cmbuf[CMSG_SPACE(sizeof(struct in_pktinfo))];
        struct sockaddr_in peeraddr;
        struct msghdr mh = {
            .msg_name = &peeraddr,
            .msg_namelen = sizeof(peeraddr),
            .msg_iov = &iov,
            .msg_iovlen = 1,
            .msg_control = cmbuf,
            .msg_controllen = sizeof(cmbuf),
        };

        print_millis("Waiting to receive");

        int len = recvmsg(sock, &mh, 0);

        for (struct cmsghdr *cmsg = CMSG_FIRSTHDR(&mh); cmsg != NULL; cmsg = CMSG_NXTHDR(&mh, cmsg)) {
            if (cmsg->cmsg_level != IPPROTO_IP || cmsg->cmsg_type != IP_PKTINFO) {
                continue;
            }
            struct in_pktinfo *pi = (in_pktinfo *)CMSG_DATA(cmsg);
            // printf("Destination IP: %s\n", inet_ntoa(pi->ipi_addr.s_addr));
            // printf("Interface IP: %s\n", inet_ntoa(pi->ipi_addr));

            print_millis("received bytes");

            recvbuf[len] = 0; // Null-terminate whatever we received and treat like a string...
            // ESP_LOGI(TAG, "%s", recvbuf);
        }
    }
}

void ReceiveThread(void* parameters)
{
    int sock = *((int*)(parameters));
    Receive(sock);
}

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    WiFiAp::Instance().start();

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");

    char multicast_ip_first[] = "232.10.11.12";
    char multicast_ip_second[] = "225.5.3.10";
    int port = 33334;

    int sock = CreateUdpSocket(port);
    JoinMulticast(sock, multicast_ip_second);
    
    // xTaskCreate(&SendThread, "mcast_task", 4096, &sendParameters, 5, NULL);
    xTaskCreate(&ReceiveThread, "mcast_task", 4096 * 8, &sock, 5, NULL);
}
