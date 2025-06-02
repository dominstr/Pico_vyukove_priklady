#include <stdio.h>
#include "pico/stdlib.h"
#include "xplr.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define LED_RED 20
#define LED_GREEN 21
#define LED_YELLOW 22

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

int traffic_lights_init(void) {
    gpio_init(LED_GREEN);
    gpio_init(LED_YELLOW);
    gpio_init(LED_RED);

    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);

    return PICO_OK;
}

void set_traffic_lights(bool green_on, bool yellow_on, bool red_on) {
    gpio_put(LED_GREEN, green_on);
    gpio_put(LED_YELLOW, yellow_on);
    gpio_put(LED_RED, red_on);
    pico_set_led(red_on && !yellow_on);
}

int main()
{
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    traffic_lights_init();

    while (true) {
        set_traffic_lights(false, false, true);
        sleep_ms(5000);
        set_traffic_lights(false, true, true);
        sleep_ms(1000);
        set_traffic_lights(true, false, false);
        sleep_ms(5000);
        set_traffic_lights(false, true, false);
        sleep_ms(1000);
    }
}
