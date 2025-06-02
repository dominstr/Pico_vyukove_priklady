/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Modified by Dominik Strnad in 2025 for educational purposes.
 * Adapted to work with the Explorer Board development platform.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "xplr.h"

#define TONE_FREQUENCY 1000 // Hz

// Funkce pro prehrani tonu
void play_tone(uint slice_num, uint channel, uint frequency) {
    // Vypocet delice hodinoveho signalu
    float clkdiv = 125000.0f / frequency;
    pwm_set_clkdiv(slice_num, clkdiv);              // Nastaveni delice
    pwm_set_wrap(slice_num, 1000);                  // Nastaveni wrap hodnoty pro PWM
    pwm_set_chan_level(slice_num, channel, 500);    // Nastaveni stridy 50 % (500 z 1000)
    pwm_set_enabled(slice_num, true);               // Aktivace PWM pro generovani tonu
}

// Funkce pro zastaveni tonu
void stop_tone(uint slice_num) {
    pwm_set_enabled(slice_num, false);  // Deaktivace PWM signalu
}

int main()
{
    stdio_init_all();

    // Inicializce tlacitka
    gpio_init(BUTTON_DOWN_PIN);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);

    // Nastaveni pinu bzucaku pro PWM vystup
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    // Ziskani slice cisla pro pin bzucaku
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    while (true) {
        if (gpio_get(BUTTON_DOWN_PIN) == 0) {
            play_tone(slice_num, PWM_CHAN_B, TONE_FREQUENCY);
        } else {
            stop_tone(slice_num);
        }
    }

    return 0;
}