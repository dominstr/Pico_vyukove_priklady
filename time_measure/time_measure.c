#include <stdio.h>

#include "pico/stdlib.h"
#include "xplr.h"

#define LED_RED 20
#define CYCLES 10000000
#define CYCLES_PICO_W 215

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Inicializace built-in LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

// Ovladani built-in LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// Inicializace externi LED
void external_led_init(void) {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
}

// Zapis na GPIO pomoci funkce
void write_gpio_using_func(uint pin, uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        gpio_put(pin, true);
        gpio_put(pin, false);
    }
}

// Zapis na GPIO pomoci registru
void write_gpio_using_registers(uint pin, uint32_t cycles) {
    uint32_t mask = 1ul << pin;
    for (uint32_t i = 0; i < cycles; i++) {
        sio_hw->gpio_set = mask;
        sio_hw->gpio_clr = mask;
    }
}

// Zapis na vestavenou LED na Pico W
void write_built_in_led(uint32_t cycles) {
    for (uint32_t i = 0; i < cycles; i++) {
        pico_set_led(true);
        pico_set_led(false);
    }
}

int main() {
    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    external_led_init();

    uint32_t start_time;
    uint32_t end_time;
    uint32_t elapsed_time;

    sleep_ms(1000);

    while (true) {
        printf("Zapis na GPIO pomoci funkce:\n");
        start_time = time_us_32();
        write_gpio_using_func(LED_RED, CYCLES);
        end_time = time_us_32();
        elapsed_time = end_time - start_time;
        printf("Pocet cyklu: %d, cas: %d us\n\n", CYCLES, elapsed_time);
        sleep_ms(1000);

        printf("Zapis na GPIO pomoci registru:\n");
        start_time = time_us_32();
        write_gpio_using_registers(LED_RED, CYCLES);
        end_time = time_us_32();
        elapsed_time = end_time - start_time;
        printf("Pocet cyklu: %d, cas: %d us\n\n", CYCLES, elapsed_time);
        sleep_ms(1000);

#if defined(CYW43_WL_GPIO_LED_PIN)
        printf("Zapis na vestavenou LED na Pico W:\n");
        start_time = time_us_32();
        write_built_in_led(CYCLES_PICO_W);
        end_time = time_us_32();
        elapsed_time = end_time - start_time;
        printf("Pocet cyklu: %d, cas: %d us\n\n", CYCLES_PICO_W, elapsed_time);
        sleep_ms(1000);
#endif

        sleep_ms(4000);
        printf("---------------------------------------\n\n");
    }
}
