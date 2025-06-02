#include <stdio.h>
#include "pico/stdlib.h"
#include "xplr.h"

// Rele je sepnuto privedenim log. 1 na pin RELAY_PIN (28)
// Zapojeni Normally Open (NO) - pri sepnutem rele je spinany obvod sepnuty
// Zapojeni Normally Closed (NC) - pri sepnutem rele je spinany obvod rozpojeny

int main()
{
    stdio_init_all();
    // Inicializace GPIO pro rele
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    gpio_put(RELAY_PIN, 0);

    while (true) {
        // Nastavení vystupu na log. 1
        gpio_put(RELAY_PIN, 1);
        sleep_ms(5000);
        // Nastavení vystupu na log. 0
        gpio_put(RELAY_PIN, 0);
        sleep_ms(5000);
    }
}
