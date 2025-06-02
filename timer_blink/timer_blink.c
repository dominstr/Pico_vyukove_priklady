/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Modified by Dominik Strnad in 2025 for educational purposes.
 * Adapted to work with the Explorer Board development platform.
 */

#include <stdio.h>

#include "hardware/irq.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Alarm 0
#define ALARM_NUM 0
#define ALARM_IRQ timer_hardware_alarm_get_irq_num(timer_hw, ALARM_NUM)

// Priznak spusteni alarmu
static volatile bool alarm_fired;

static volatile bool led_on = false;
#define LED_RED 20

// Inicializace LED diody
void init(void) {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
}

// Obsluha preruseni alarmu
static void alarm_irq(void) {
    // Vynulovani registru preruseni
    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);

    // Alarm byl spusten
    alarm_fired = true;
    printf("Alarm IRQ fired\n");

    // Nastaveni LED diody
    led_on = !led_on;
    gpio_put(LED_RED, led_on);
}

// Nastaveni alarmu na urcity cas
static void alarm_in_us(uint32_t delay_us) {
    // Povoleni preruseni pro alarm (casovac ma 4 preruseni alarmu)
    hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
    // Nastaveni obsluhy preruseni
    irq_set_exclusive_handler(ALARM_IRQ, alarm_irq);
    // Povoleni preruseni alarmu
    irq_set_enabled(ALARM_IRQ, true);
    // Povoleni preruseni v bloku a na procesoru

    // Alarm je pouze 32 bitovy
    uint64_t target = timer_hw->timerawl + delay_us;

    // Zapise dolnich 32 bitu daneho casu do alarmu a tim ho aktivuje
    timer_hw->alarm[ALARM_NUM] = (uint32_t)target;
}

int main() {
    stdio_init_all();
    init();

    while (1) {
        alarm_fired = false;
        alarm_in_us(1000000 * 2);  // Nastaveni alarmu na 2 s
        while (!alarm_fired);      // Ceka na spusteni alarmu
    }
}
