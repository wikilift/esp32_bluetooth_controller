#include "helpers.hpp"
#include "nvs_flash.h"
#include "esp_mac.h"

uint8_t *Helpers::get_mac_address()
{

    uint8_t mac[6];
    esp_err_t ret = esp_read_mac(mac, ESP_MAC_EFUSE_CUSTOM);

    if (ret == ESP_OK)
    {
        return mac;
    }
    else
    {
        return nullptr;
    }
}