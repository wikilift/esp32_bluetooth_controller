#include "read_inputs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "BleGamepad.h"
#include "esp_log.h"

#include "constants/constants.hpp"
#include "driver/gpio.h"

static QueueHandle_t button_event_queue;

typedef struct
{
    ps3_button_t button;
    bool state;
} button_event_t;

void IRAM_ATTR button_isr_handler(void *arg)
{
    ps3_button_t button = static_cast<ps3_button_t>(reinterpret_cast<uintptr_t>(arg));
    button_event_t evt;
    evt.button = button;
    evt.state = (gpio_get_level(static_cast<gpio_num_t>(button)) == 0);
    xQueueSendFromISR(button_event_queue, &evt, NULL);
}

void read_buttons(void *pVp)
{
    if (button_event_queue == nullptr)
    {
        button_event_queue = xQueueCreate(10, sizeof(button_event_t));
    }
    BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
    if (bleGamepad == nullptr)
    {
        ESP_LOGE("ERROR", "Fatal error on read buttons");
        vTaskDelete(nullptr);
        return;
    }

    button_event_t evt;
    for (;;)
    {
        if (xQueueReceive(button_event_queue, &evt, portMAX_DELAY))
        {
            if (bleGamepad->isConnected())
            {
                if (evt.state)
                {
                    bleGamepad->press(evt.button + 1);
                }
                else
                {
                    bleGamepad->release(evt.button + 1);
                }
                bleGamepad->sendReport();
            }
        }
    }
}

void read_axis(void *pVp)
{
    if (button_event_queue == nullptr)
    {
        button_event_queue = xQueueCreate(10, sizeof(button_event_t));
    }
    BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
    if (bleGamepad == nullptr)
    {
        ESP_LOGE("ERROR", "Fatal error on read axes");
        vTaskDelete(nullptr);
        return;
    }
    for (;;)
    {
        if (bleGamepad->isConnected())
        {
            int16_t lx = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[0]));
            int16_t ly = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[1]));
            int16_t rx = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[2]));
            int16_t ry = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[3]));

            bleGamepad->setLeftThumb(lx, ly);
            bleGamepad->setRightThumb(rx, ry);
            bleGamepad->sendReport();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(nullptr);
}

void read_hat(void *pVp)
{
    if (button_event_queue == nullptr)
    {
        button_event_queue = xQueueCreate(10, sizeof(button_event_t));
    }
    BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
    if (bleGamepad == nullptr)
    {
        ESP_LOGE("ERROR", "Fatal error on read hat");
        vTaskDelete(nullptr);
        return;
    }

    for (;;)
    {
        if (bleGamepad->isConnected())
        {
            signed char hatValue = HAT_CENTERED;

            if (gpio_get_level(static_cast<gpio_num_t>(HAT_UP_PIN)) == 0)
                hatValue = HAT_UP;
            if (gpio_get_level(static_cast<gpio_num_t>(HAT_DOWN_PIN)) == 0)
                hatValue = HAT_DOWN;
            if (gpio_get_level(static_cast<gpio_num_t>(HAT_LEFT_PIN)) == 0)
                hatValue = HAT_LEFT;
            if (gpio_get_level(static_cast<gpio_num_t>(HAT_RIGHT_PIN)) == 0)
                hatValue = HAT_RIGHT;

            bleGamepad->setHat1(hatValue);
            bleGamepad->sendReport();
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    vTaskDelete(nullptr);
}

//! POLLING
// void read_buttons(void *pVp)
// {
//     BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
//     if (bleGamepad == nullptr)
//     {
//         ESP_LOGE("ERROR", "Fatall error on read buttons");
//         vTaskDelete(nullptr);
//         return;
//     }
//     for (;;)
//     {
//         if (bleGamepad->isConnected())
//         {
//             for (int i = 0; i < NUM_BUTTONS; i++)
//             {
//                 if (gpio_get_level(static_cast<gpio_num_t>(button_pins[i])) == 0)
//                 {
//                     bleGamepad->press(i + 1);
//                 }
//                 else
//                 {
//                     bleGamepad->release(i + 1);
//                 }
//             }
//             bleGamepad->sendReport();
//         }
//         vTaskDelay(pdMS_TO_TICKS(2));
//     }
//     vTaskDelete(nullptr);
// }

// void read_axis(void *pVp)
// {
//     BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
//     if (bleGamepad == nullptr)
//     {
//         ESP_LOGE("ERROR", "Fatall error on read axes");
//         vTaskDelete(nullptr);
//         return;
//     }
//     for (;;)
//     {
//         if (bleGamepad->isConnected())
//         {
//             int16_t lx = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[0]));
//             int16_t ly = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[1]));
//             int16_t rx = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[2]));
//             int16_t ry = adc1_get_raw(static_cast<adc1_channel_t>(axis_pins[3]));

//             bleGamepad->setLeftThumb(lx, ly);
//             bleGamepad->setRightThumb(rx, ry);
//             bleGamepad->sendReport();
//         }
//         vTaskDelay(pdMS_TO_TICKS(2));
//     }
//     vTaskDelete(nullptr);
// }

// void read_hat(void *pVp)
// {
//     BleGamepad *bleGamepad = static_cast<BleGamepad *>(pVp);
//     if (bleGamepad == nullptr)
//     {
//         ESP_LOGE("ERROR", "Fatal error on read hat");
//         vTaskDelete(nullptr);
//         return;
//     }

//     for (;;)
//     {
//         if (bleGamepad->isConnected()) {
//             signed char hatValue = HAT_CENTERED;

//             if (gpio_get_level(static_cast<gpio_num_t>(HAT_UP)) == 0)
//                 hatValue = HAT_UP;
//             if (gpio_get_level(static_cast<gpio_num_t>(HAT_DOWN)) == 0)
//                 hatValue = HAT_DOWN;
//             if (gpio_get_level(static_cast<gpio_num_t>(HAT_LEFT)) == 0)
//                 hatValue = HAT_LEFT;
//             if (gpio_get_level(static_cast<gpio_num_t>(HAT_RIGHT)) == 0)
//                 hatValue = HAT_RIGHT;

//             bleGamepad->setHat1(hatValue);
//             bleGamepad->sendReport();
//         }
//         vTaskDelay(pdMS_TO_TICKS(2));
//     }
//     vTaskDelete(nullptr);
// }