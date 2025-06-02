#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "xplr.h"

#define LED_RED 20
#define LED_GREEN 21
#define LED_YELLOW 22
#define ADC_PIN 26

uint8_t read_adc(void)
{
    uint16_t raw_value = adc_read(); // Precti 12-bitovou hodnotu ADC (0-4095)
    uint8_t scaled_value = (raw_value >> 4); // Preved ADC hodnotu na 8-bitovou (0-255)
        
    return scaled_value;;
}

void set_led_pwm(uint gpio_pin, uint8_t level) {
    pwm_set_gpio_level(gpio_pin, level); // Nastavi PWM na urcitou hodnotu
}

// Nastaveni GPIO pro PWM
void init_pwm(void) {
    gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);      // PWM_A[2]
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_GREEN), 255);         // Nastavi periodu 256 cyklu (0-255 vcetne)
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_GREEN), true);   // Zapnuti PWM
}

// Nastaveni GPIO pro ADC
void init_adc(void) {
    // Inicializace ADC modulu
    adc_init();
    // Nastavi GPIO pro pouziti ADC, nastavi pin do stavu vysoke impedance bez pull-up a pull-down rezistoru
    adc_gpio_init(ADC_PIN);
    // Vybere vstupni kanal ADC 0 (GPIO26)
    adc_select_input(0);
}

int main()
{
    stdio_init_all();
    init_adc(); // Inicializace ADC
    init_pwm(); // Inicializace PWM

    while (true) {
        uint8_t pwm_value = read_adc();
        set_led_pwm(LED_GREEN, pwm_value); // Nastavi PWM na hodnotu ADC
        sleep_ms(20); // Pauza 20 ms
    }
}

