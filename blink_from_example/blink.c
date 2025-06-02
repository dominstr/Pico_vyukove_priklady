/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Modified by Dominik Strnad in 2025 for educational purposes.
 * Adapted to work with the Explorer Board development platform.
 */

#include "pico/stdlib.h"

// Pico W pouziva GPIO na WIFI cipu pro vestavenou LED diodu,
// pri kompilaci pro Pico W bude definovan CYW43_WL_GPIO_LED_PIN
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
#endif

// Inicializace vestavene LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Pico, ktere pouziva GPIO pro vestavenou LED diodu, ma definovany PICO_DEFAULT_LED_PIN,
    // takze je mozno pouzit beznou funkcionalitu GPIO pro zapnuti a vypnuti LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Pro Pico W je treba inicializovat ovladac atd.
    return cyw43_arch_init();
#endif
}

// Zapnuti nebo vypnuti vestavene LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Nastaveni GPIO na zapnuto nebo vypnuto
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Zapnuti nebo vypnuti GPIO pres ovladac wifi cipu
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main() {
    // Inicializace vestavene LED a kontrola, zda probehla uspesne
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    // Nekonecna smycka programu
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
