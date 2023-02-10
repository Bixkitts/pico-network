#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <math.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define PWM_PIN 15

#define A3 56818
#define D4 42566
#define E4 37921
#define F4 35793
#define G4 31888
#define A4 28409
#define B4 25310
#define C4 47777

uint16_t beat   = 500; //amount of milliseconds a beat consists of
                       //(adjusts the speed of the song!)
uint16_t stop   = 18; //amount of time between notes
uint16_t volume = 20000;
uint32_t slice;

uint64_t sTime = time_us_64(); //time when program is started

void setVolume(uint16_t vol)
{ 
    pwm_set_gpio_level(PWM_PIN, vol);

}
void playTone(float tone, uint32_t time)
{ 
    setVolume(volume);
    pwm_set_wrap(slice, tone);
    sleep_ms(time-stop);
    setVolume(0);
    sleep_ms(stop);
}

void playTune()
{
    playTone(D4, beat/2);
    playTone(D4, beat/2);
    playTone(D4, beat/2);
    playTone(E4, beat/2);
    playTone(F4, beat);
    playTone(F4, beat/2);
    sleep_ms(beat/2);
    playTone(B4, beat/2);
    playTone(B4, beat/2);
    playTone(B4, beat/2);
    playTone(A4, beat/2);
    playTone(G4, beat);
    playTone(G4, beat/2);
    sleep_ms(beat/2);

    playTone(C4, beat*2);
    playTone(A3, beat*2);
    playTone(E4, beat*2);
    playTone(C4, beat*2);
    //second verse

    playTone(D4, beat/2);
    playTone(D4, beat/2);
    playTone(D4, beat/2);
    playTone(E4, beat/2);
    playTone(F4, beat);
    playTone(F4, beat/2);
    sleep_ms(beat/2);
    playTone(B4, beat/2);
    playTone(B4, beat/2);
    playTone(B4, beat/2);
    playTone(A4, beat/2);
    playTone(G4, beat);
    playTone(G4, beat/2);
    sleep_ms(beat/2);

    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(A4, beat/2);
    playTone(B4, beat/2);
    playTone(A4, beat);
    playTone(G4, beat);
    playTone(F4, beat);
    playTone(E4, beat);
}

int main() {
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    slice = pwm_gpio_to_slice_num(PWM_PIN);

    pwm_config redConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&redConfig, 10);
    pwm_init(slice, &redConfig, true);

    pwm_clear_irq(slice);

    pwm_set_wrap(slice, 50000);
    pwm_set_gpio_level(PWM_PIN, volume);

    while(1)
    {
    playTune();
    }
}
