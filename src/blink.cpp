#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"

#define LED_PIN 15
#define BUTTON_PIN 10

volatile bool ledToggled = 0;
int64_t toggleLED(alarm_id_t id, void *user_data);

int main() {
    stdio_init_all();
    volatile bool ledOn = 0;
    volatile bool buttonReady = 1;
    uint64_t timestamp = 
            to_ms_since_boot(get_absolute_time());

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    while (true) {
        if(!gpio_get(BUTTON_PIN) && buttonReady)
        {
            sleep_ms(2);
            if(!gpio_get(BUTTON_PIN))
                ledOn = !ledOn;
            buttonReady = 0;
        }
        if(gpio_get(BUTTON_PIN))
            buttonReady = 1;
        if(ledOn && !ledToggled)
        {
            add_alarm_in_ms(100, toggleLED, NULL, false);
            ledToggled = 1;
        }
    }
}

int64_t toggleLED(alarm_id_t id, void *user_data)
{
    bool LED = gpio_get(LED_PIN);
    gpio_put(LED_PIN, LED == 0);
    ledToggled = 0; 
    return 0;
}
