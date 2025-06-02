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
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "xplr.h"

int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO26\n");

    // Inicializace ADC modulu
    adc_init();

    // Nastavi pin do vysokoimpedancniho stavu, vypne pull-up rezistory, aby neovlivnovaly mereni
    adc_gpio_init(26);
    // Vybere ADC vstup 0 (GPIO26)
    adc_select_input(0);

    while (1) {
        // 12-bit prevod, prepokladana max. hodnota == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);

        // Nacteni hodnoty z ADC
        uint16_t result = adc_read();

        // Vypis hodnoty v hexadecimalnim formatu a odpovidajiciho napeti ve voltech
        printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);

        // Zpozdeni mezi jednotlivymi merenimi
        sleep_ms(500);
    }
}