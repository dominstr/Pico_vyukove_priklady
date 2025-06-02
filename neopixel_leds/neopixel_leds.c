// Pro pouziti NeoPixel LED diod musi byt deska Explorer Board napajena USB-C kabelem, nestaci pouze microUSB kabel!!!

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "xplr.h"

#define IS_RGBW false
#define NUM_PIXELS 4
#define WS2812_PIN NEOPIXEL_LEDS_PIN
#define BRIGHTNESS 0x0A // rozsah jasu 0-255 (pro hodnoty vetsi nez 20 sviti neprijemne do oci!!!)

// Ocekava barvu v poradi GRB (Green, Red, Blue)
static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// Nastaveni jedne LED diody na zelenou barvu
void set_single_led(PIO pio, uint sm, uint led_index, uint len) {
    for (uint i = 0; i < len; ++i) {
        if (i == led_index) {
            put_pixel(pio, sm, urgb_u32(BRIGHTNESS, 0, 0)); // Zelena
        } else {
            put_pixel(pio, sm, 0); // Zhasnuto
        }
    }
}

// Efekt rotace LED diod
void pattern_rotate(PIO pio, uint sm, uint len, uint t) {
    // Definice poradi: 0-1-3-2
    uint led_order[] = {0, 1, 3, 2};

    // Vybereme aktivni LED podle poradi
    uint active_led = led_order[t % len];

    // Rozsviti pouze aktivni LED, ostatni LED zhasne
    for (uint i = 0; i < len; ++i) {
        if (i == active_led)
            put_pixel(pio, sm, urgb_u32(0, 0, BRIGHTNESS));
        else
            put_pixel(pio, sm, 0);
    }
}

// Efekt policejni majaky
void pattern_police_lights(PIO pio, uint sm, uint t) {
    for (int i = 0; i < NUM_PIXELS; ++i) {
        if (i == 0) {
            put_pixel(pio, sm, (t % 2 == 0) ? urgb_u32(0, 0, BRIGHTNESS) : 0); // Modra
        } else if (i == 1) {
            put_pixel(pio, sm, (t % 2 == 1) ? urgb_u32(0, BRIGHTNESS, 0) : 0); // Cervena
        } else {
            put_pixel(pio, sm, 0); // Ostatní LED zhasnuty
        }
    }
}

int main()
{
    stdio_init_all();

    // todo get free sm
    PIO pio;
    uint sm;
    uint offset;

    // Najde volne PIO jadro a volny State Machine a nahraje PIO program do pameti PIO jadra
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int t = 0;
    while (true) {
        set_single_led(pio, sm, 3, NUM_PIXELS); // Rozsviti 4. LED (index 3)

        // pattern_rotate(pio, sm, NUM_PIXELS, t); // Rozsviti LED diody v poradi 0-1-3-2

        // pattern_police_lights(pio, sm, t); // Rozsviti modrou a cervenou LED stridave

        t++;
        sleep_ms(500);
    }

    // Odebere PIO program z PIO a uvolni State Machine
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
