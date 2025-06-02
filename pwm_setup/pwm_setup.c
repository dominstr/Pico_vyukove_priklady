#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_RED 20
#define LED_GREEN 21
#define LED_YELLOW 22

int main()
{
    stdio_init_all();

    // Nastaveni GPIO pro PWM
    gpio_set_function(LED_RED, GPIO_FUNC_PWM);      // PWM_A[2]
    gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);    // PWM_B[2]
    gpio_set_function(LED_YELLOW, GPIO_FUNC_PWM);   // PWM_A[3]

    // PWM pro LED_RED a LED_GREEN
    uint slice_red_green = pwm_gpio_to_slice_num(LED_RED);  // Zjisti na jakém slice je prislusny GPIO (PWM_A[2] a PWM_B[2] jsou na stejnem slice - 2)
    pwm_set_wrap(slice_red_green, 3);                       // Nastavi periodu 4 cyklu (0-3 vcetne)
    pwm_set_chan_level(slice_red_green, PWM_CHAN_A, 1);     // Nastavi kanal A na HIGH po dobu jednoho cyklu (25%)
    pwm_set_chan_level(slice_red_green, PWM_CHAN_B, 3);     // Nastavi kanal B na HIGH po dobu tri cyklu (75%)
    pwm_set_enabled(slice_red_green, true);                 // Zapnuti PWM

    // Dalsi zpusob zapisu pro LED_YELLOW
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_YELLOW), 3);         // Nastavi periodu 4 cyklu (0-3 vcetne)
    pwm_set_gpio_level(LED_YELLOW, 2);                          // Nastavi kanal A na HIGH po dobu dvou cyklu (50%)
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_YELLOW), true);   // Zapnuti PWM

    // Overeni maximalni stridy PWM
    // Jas LED_YELLOW strida 75 % (3 cykly) a strida 100 % (4 cykly)
    // while (true) {
    //     pwm_set_gpio_level(LED_YELLOW, 3);
    //     sleep_ms(1000);
    //     pwm_set_gpio_level(LED_YELLOW, 4);
    //     sleep_ms(1000);
    // }
}
