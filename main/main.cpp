

#include "BleGamepad.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "daemon/battery_level.h"
#include "esp_mac.h"
#include "helpers/helpers.hpp"
#include "constants/constants.hpp"
#include "driver/gpio.h"
#include "daemon/config_pins.h"
#include "daemon/read_inputs.h"
BleGamepad bleGamepad("SNES Gamepad", "Espressif", 100);
// const uint8_t* newMACAddress = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF - 0x02};
BleGamepadConfiguration initConfig(BleGamepad &bleGamepad);

extern "C" void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    setup_pins();
    BleGamepadConfiguration bleGamepadConfig = initConfig(bleGamepad);
    bleGamepad.begin(&bleGamepadConfig);
    xTaskCreatePinnedToCore(&read_buttons, "button_task", 2048, &bleGamepad, 7, nullptr, 1);
    xTaskCreatePinnedToCore(read_axis, "axis_task", 6048, &bleGamepad, 8, nullptr, 1);
    xTaskCreatePinnedToCore(read_hat, "hat_task", 6048, &bleGamepad, 8, nullptr, 1);
    xTaskCreatePinnedToCore(sendBatteryState, "batteryDaemon", 6048, &bleGamepad, 2, nullptr, 1);
}

BleGamepadConfiguration initConfig(BleGamepad &bleGamepad)
{
    BleGamepadConfiguration bleGamepadConfig;

    bleGamepadConfig.setAutoReport(false);
    // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
    bleGamepadConfig.setVid(0xe502);
    bleGamepadConfig.setPid(0xabcd);
    bleGamepadConfig.setModelNumber(const_cast<char *>("1.0"));
    bleGamepadConfig.setSoftwareRevision(const_cast<char *>("Software Rev 1"));
    bleGamepadConfig.setSerialNumber(const_cast<char *>("9876543210"));
    bleGamepadConfig.setFirmwareRevision(const_cast<char *>("2.0"));
    bleGamepadConfig.setHardwareRevision(const_cast<char *>("1.7"));
    bleGamepadConfig.setButtonCount(NUM_BUTTONS);
    bleGamepadConfig.setIncludeStart(true);
    bleGamepadConfig.setIncludeSelect(true);
    bleGamepadConfig.setWhichAxes(true, true, true, true, false, false, false, false);
    bleGamepadConfig.setAxesMin(AXIS_MIN);
    bleGamepadConfig.setAxesMax(AXIS_MAX);

    // esp_base_mac_addr_set(newMACAddress);
    return bleGamepadConfig;
}
