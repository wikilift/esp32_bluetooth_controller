#include "config_pins.h"
#include "constants/constants.hpp"
#include "read_inputs.h"

void setup_pins(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;

    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        
            ESP_ERROR_CHECK(gpio_isr_handler_add(static_cast<gpio_num_t>(button_pins[i]), button_isr_handler, (void *)button_pins[i]));
       // }
    }
    for (int i = 0; i < sizeof(hat_pins) / sizeof(hat_pins[0]); i++)
    {
        ESP_ERROR_CHECK(gpio_isr_handler_add(static_cast<gpio_num_t>(hat_pins[i]), button_isr_handler, (void *)hat_pins[i]));
    }

    //! POLLING
    // for (int i = 0; i < NUM_BUTTONS; i++)
    // {

    //     // esp_rom_gpio_pad_select_gpio(static_cast<gpio_num_t>(button_pins[i]));
    //     gpio_set_direction(static_cast<gpio_num_t>(button_pins[i]), GPIO_MODE_INPUT);
    //     gpio_set_pull_mode(static_cast<gpio_num_t>(button_pins[i]), GPIO_PULLUP_ONLY);
    // }

    // adc1_config_width(ADC_WIDTH_BIT_12);
    // for (int i = 0; i < NUM_AXES; i++)
    // {
    //     adc1_config_channel_atten(static_cast<adc1_channel_t>(axis_pins[i]), ADC_ATTEN_DB_11);
    // }
}