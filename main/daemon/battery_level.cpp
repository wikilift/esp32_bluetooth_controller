#include "battery_level.h"


uint8_t batteryLevel = 100;

 void sendBatteryState(void *test)
{
  BleGamepad *batteryInstance = reinterpret_cast<BleGamepad *>(test);
  if (batteryInstance == nullptr)
  {
    ESP_LOGE("BatteryLevel", "batteryInstance es nullptr");
    vTaskDelete(nullptr);
    return;
  }

  static uint8_t lastBattery = 0;
  for (;;)
  {
    if (batteryInstance->isConnected())
    {
      if (batteryLevel != lastBattery)
      {
        batteryInstance->setBatteryLevel(batteryLevel);
        lastBattery = batteryLevel;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}