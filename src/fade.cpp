// Fade an LED between low and high brightness. An interrupt handler updates
// the PWM slice's output level each time the counter wraps.

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define RED_PIN 11
#define GREEN_PIN 12
#define BLUE_PIN 13

#define RED_SW 7
#define GREEN_SW 8
#define BLUE_SW 9

uint r_current = 255; 
uint g_current = 255; 
uint b_current = 255; 

uint r_grav = 255;
uint g_grav = 255;
uint b_grav = 255;

static const uint8_t DEBOUNCE = 50;
static bool pressed = false;
static int32_t alarm_id = 0;

void setColor(uint r, uint g, uint b)
{ 
    pwm_set_gpio_level(RED_PIN, r*r);
    pwm_set_gpio_level(GREEN_PIN, g*g);
    pwm_set_gpio_level(BLUE_PIN, b*b);
}
void setGrav(uint r, uint g, uint b)
{
    r_grav = r;
    g_grav = g;
    b_grav = b;
}

int64_t enable_button(alarm_id_t alarm_id, void *user_data) {
    pressed = false;
    return 0;
}

void gpio_callback(uint gpio, uint32_t events) {
    if (pressed) {
        cancel_alarm(alarm_id);
    } 
    else if(gpio == RED_SW)
    {
        pressed = true;
        setGrav(0, 255, 255);
    }
    else if(gpio == GREEN_SW)
    {
        pressed = true;
        setGrav(255, 0, 255);
    }
    else if(gpio == BLUE_SW)
    {
        pressed = true;
        setGrav(255, 255, 0);
    }
    alarm_id = add_alarm_in_ms(DEBOUNCE, enable_button, NULL, false);
}

void on_pwm_wrap()
{
    pwm_clear_irq(pwm_gpio_to_slice_num(RED_PIN));
    setColor(r_current, g_current, b_current);
    r_current += r_current < r_grav;
    r_current -= r_current > r_grav;
    g_current += g_current < g_grav;
    g_current -= g_current > g_grav;
    b_current += b_current < b_grav;
    b_current -= b_current > b_grav;
}

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning pwm/led_fade example requires a board with a regular LED
#else
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(RED_PIN, GPIO_FUNC_PWM);
    gpio_set_function(GREEN_PIN, GPIO_FUNC_PWM);
    gpio_set_function(BLUE_PIN, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    uint redSlice = pwm_gpio_to_slice_num(RED_PIN);
    uint greenSlice = pwm_gpio_to_slice_num(GREEN_PIN);
    uint blueSlice = pwm_gpio_to_slice_num(BLUE_PIN);
    //pwm_set_wrap(redSlice, 65465);
    //pwm_set_wrap(greenSlice, 65465);
    //pwm_set_wrap(blueSlice, 65465);

    pwm_config redConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&redConfig, 6.f);
    pwm_init(redSlice, &redConfig, true);

    pwm_config greenConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&greenConfig, 6.f);
    pwm_init(greenSlice, &greenConfig, true);

    pwm_config blueConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&blueConfig, 6.f);
    pwm_init(blueSlice, &blueConfig, true);

    gpio_init(RED_SW);
    gpio_pull_up(RED_SW);
    gpio_set_irq_enabled_with_callback(RED_SW, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback);

    gpio_init(GREEN_SW);
    gpio_pull_up(GREEN_SW);
    gpio_set_irq_enabled(GREEN_SW, GPIO_IRQ_LEVEL_LOW, true);

    gpio_init(BLUE_SW);
    gpio_pull_up(BLUE_SW);
    gpio_set_irq_enabled(BLUE_SW, GPIO_IRQ_LEVEL_LOW, true);
    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    pwm_clear_irq(redSlice);
    pwm_set_irq_enabled(redSlice, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);


    while (1)
        tight_loop_contents();
#endif
}
