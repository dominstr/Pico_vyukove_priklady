#include <stdio.h>

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "task.h"
#include "queue.h"
#include "xplr.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Globalni promenna pro frontu, ktera bude prenaset stav tlacitka mezi tasky
QueueHandle_t button_queue;

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
void button_init(void) {
    gpio_init(BUTTON_DOWN_PIN);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);
}

// Task pro LED
void led_task(void *pvParameters) {
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    bool led_state;
    uint32_t button_state;

    while (1) {
        // Cekani na zpravu z fronty a ovladani LED podle prijateho stavu
        if (xQueueReceive(button_queue, &led_state, portMAX_DELAY) == pdPASS) {
            pico_set_led(led_state);
        }
    }
}

// Task pro tlacitko
void button_task(void *pvParameters) {
    button_init();

    bool last_state = true;
    bool current_state;
    
    while (1) {
        // Zjisteni stavu tlacitka
        if (gpio_get(BUTTON_DOWN_PIN) == 0) {
            current_state = true;
        } else {
            current_state = false;
        }

        // Kontrola zmeny stavu tlacitka
        if (current_state != last_state) {
            last_state = current_state;
            // Odeslani stavu tlacitka do fronty
            xQueueSend(button_queue, &current_state, 0);
        }

        // Debounce tlacitka 50 ms
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

int main() {
    stdio_init_all();

    // Vytvoreni fronty pro tlacitko
    button_queue = xQueueCreate(1, sizeof(bool));

    // Vytvoreni tasku pro LED
    xTaskCreate(led_task, "LED_Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    // Vytvoreni tasku pro tlacitko
    xTaskCreate(button_task, "Button_Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // Spusteni planovace
    vTaskStartScheduler();

    // Tento kod by se nikdy nemel vykonat
    while (1) {};
}
