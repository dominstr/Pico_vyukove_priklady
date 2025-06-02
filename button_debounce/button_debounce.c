#include <stdio.h>

#include "pico/stdlib.h"
#include "xplr.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define BUTTON_PRESSED 0
#define BUTTON_NOT_PRESSED 1

// Inicializace vestavene LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

// Ovladani vestavene LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// Inicializace tlacitka
void button_init() {
    gpio_init(BUTTON_DOWN_PIN);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_DOWN_PIN);
}

// Cteni stavu tlacitka s debounce 20 ms
int read_button() {
    int button_state = BUTTON_NOT_PRESSED;

    if (gpio_get(BUTTON_DOWN_PIN) == 0) {
        sleep_ms(20);
        if (gpio_get(BUTTON_DOWN_PIN) == 0) {
            button_state = BUTTON_PRESSED;
        }
    }

    return button_state;
}

int main() {
    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    button_init();

    bool led_on = false;
    int state;
    int previous_state = BUTTON_NOT_PRESSED;

    pico_set_led(led_on);

    while (true) {
        state = read_button();

        if (state == BUTTON_PRESSED && previous_state == BUTTON_NOT_PRESSED) {
            led_on = !led_on;
            pico_set_led(led_on);
        }

        previous_state = state;
    }
}