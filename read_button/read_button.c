#include <stdio.h>
#include "pico/stdlib.h"
#include "xplr.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

int pico_led_init(void) {
    #if defined(PICO_DEFAULT_LED_PIN)
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
        return PICO_OK;
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        return cyw43_arch_init();
    #endif
}

void pico_set_led(bool led_on) {
    #if defined(PICO_DEFAULT_LED_PIN)
        gpio_put(PICO_DEFAULT_LED_PIN, led_on);
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}

int main()
{   
    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    gpio_init(BUTTON_DOWN_PIN);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);

    while (true) {
        if (gpio_get(BUTTON_DOWN_PIN) == 1) {
            pico_set_led(false);
        } else {
            pico_set_led(true);
        }
    }
}
