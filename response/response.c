// Ukazka odezvy programu
//
// Ocekavane chovani:
// Pri stisku tlacitka se spusti efekt, kdy blikaji postupne LED,
// Po uvolneni tlacitka LED zhasnou. Tedy blikani trva po dobu stisku tlacitka.
//
// Ukazano je postupne vylepsovani odezvy programu - aby program rychleji reagoval na
// uvolneni tlacitka. Verzi programu zvolte nize v #define	VERSION
//
// Posledni zmena: 8.9.2021

// (Upraveno pro Raspberry Pi Pico) - puvodni zdroj: https://github.com/jdolinay/utb_frdm_vyuka/blob/master/programs/response/response.c

///////////////////////////////////////////////

#include <stdio.h>

#include "pico/stdlib.h"
#include "xplr.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Vyber verze kodu.
// 1. stav blikani a nesviti; kontrola tlacitka vzdy az po bliknuti vsech LED.
// 2. kontrola tlacitka po bliknuti kazde LED.
// 3. nepouziva se delay - polling casu.
// 4. rozdeleni kodu na ulohy - tasky.
// 5. spagetovy kod - nevhodne reseni!
#define VERSION 1

// Spolecne definice
#define BLINK_DELAY 1000
#define BUTTON_PRESSED 0
#define BUTTON_NOT_PRESSED 1

#define LED_RED 20
#define LED_GREEN 21
#define LED_YELLOW 22

// Prototypy funkci
void init(void);
int pico_led_init(void);
void pico_set_led(bool led_on);
int read_button(void);
void LED_control(bool d1, bool d2, bool d3);

///////////////////////////////////////////////////////////////////////////////////////
// Kod programu v nekolika verzich.
// Aktivni verze se vybere pomoci #define VERSION nahore!!!
#if VERSION == 1
// Verze 1
// Odezva je spatna, protoze stav tlacitka se testuje az po bliknuti vsech LED.
// Pri uvolneni tlacitka program zareaguje az na konci celeho efektu, kdyz zhasne
// treti LED a prvni uz se nerozsviti.

// Stavy programu
#define ST_EFFECT 1
#define ST_OFF 2

int state = ST_OFF;

int main() {
    // inicializace ovladace pinu a delay
    init();

    while (true) {
        if (read_button() == BUTTON_PRESSED) {
            state = ST_EFFECT;
        } else {
            state = ST_OFF;
        }

        switch (state) {
            case ST_OFF:
                LED_control(false, false, false);
                break;

            case ST_EFFECT:
                LED_control(true, false, false);
                sleep_ms(BLINK_DELAY);
                LED_control(false, true, false);
                sleep_ms(BLINK_DELAY);
                LED_control(false, false, true);
                sleep_ms(BLINK_DELAY);
                break;
        }  // switch
    }  // while

    /* Never leave main */
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

#elif VERSION == 2
// Verze 2
// Odezva je vylepsena tim, ze kontrola tlacitka probiha po bliknuti kazde LED.
// Po uvolneni tlacitka pak LED sice "dosviti" svou dobu svitu,
// ale dalsi LED uz se nerozsviti.

// Stavy programu
#define ST_LED1_ON 1
#define ST_LED2_ON 2
#define ST_LED3_ON 3
#define ST_OFF 4

int state = ST_OFF;

int main(void) {
    // inicializace ovladace pinu a delay
    init();

    while (true) {
        if (read_button() == BUTTON_PRESSED) {
            // Jen pokud je stisknuto tlacitko a soucasny stav je vypnuto,
            // prejdeme na stav rozsviceni prvni LED, jinak uz nektera LED
            // sviti a stavy se meni ve switch.
            if (state == ST_OFF) {
                state = ST_LED1_ON;
            }
        } else {
            state = ST_OFF;
        }

        switch (state) {
            case ST_OFF:
                LED_control(false, false, false);
                break;

            case ST_LED1_ON:
                // Bliknout LED1 a prejit na stav dalsi LED2
                LED_control(true, false, false);
                sleep_ms(BLINK_DELAY);
                state = ST_LED2_ON;
                break;

            case ST_LED2_ON:
                LED_control(false, true, false);
                sleep_ms(BLINK_DELAY);
                state = ST_LED3_ON;
                break;

            case ST_LED3_ON:
                LED_control(false, false, true);
                sleep_ms(BLINK_DELAY);
                state = ST_OFF;
                break;
        }  // switch
    }  // while

    /* Never leave main */
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

#elif VERSION == 3
// Verze 3
// Odezva je vylepsena tim, ze se nepouziva cekani (busy waiting) ale zjistuje se
// zda uz ubehl potrebny cas - jde o tzv. polling - dotazovani.
// Diky tomu LED zhasne okamzite po uvolneni tlacitka.

// Stavy programu
#define ST_LED1_ON 1
#define ST_LED2_ON 2
#define ST_LED3_ON 3
#define ST_OFF 4
#define ST_LED1_WAIT 5
#define ST_LED2_WAIT 6
#define ST_LED3_WAIT 7

int state = ST_OFF;

int main(void) {
    // inicializace ovladace pinu a delay
    init();

    uint32_t waitStart;    // cas, kdy se rozvitila LED
    uint32_t currentTime;  // aktualni cas, pomocna promenna

    while (true) {
        if (read_button() == BUTTON_PRESSED) {
            // Jen pokud je stisknuto tlacitko a soucasny stav je vypnuto,
            // prejdeme na stav rozsviceni prvni LED, jinak uz nektera LED
            // sviti a stavy se meni ve switch.
            if (state == ST_OFF) {
                state = ST_LED1_ON;
            }
        } else {
            state = ST_OFF;
        }

        switch (state) {
            case ST_OFF:
                LED_control(false, false, false);
                break;

            case ST_LED1_ON:
                // Rozsvitit LED, ulozit aktualni cas a prejit do stavu cekani na
                // uplynuti casu svitu teto LED.
                LED_control(true, false, false);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED1_WAIT;
                break;

            case ST_LED1_WAIT:
                // Kontrola jestli uz ubehlo dost casu abychom rozsvitili dalsi LED
                // a pokud ano, prechod na dalsi stav
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED2_ON;
                }
                break;

            case ST_LED2_ON:
                LED_control(false, true, false);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED2_WAIT;
                break;

            case ST_LED2_WAIT:
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED3_ON;
                }
                break;

            case ST_LED3_ON:
                LED_control(false, false, true);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED3_WAIT;
                break;

            case ST_LED3_WAIT:
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED1_ON;
                }
                break;
        }  // switch
    }  // while

    /* Never leave main */
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

#elif VERSION == 4
// Verze 4
// Program je vylepsen rozdelenim na jednotlive ulohy - tasky.
// Z pohledu odezvy je chovani stejne jako u verze 3, ale struktura programu je
// prehlednejsi a snadneji by se rozsiroval o dalsi cinnosti.

// Stavy programu
#define ST_LED1_ON 1
#define ST_LED2_ON 2
#define ST_LED3_ON 3
#define ST_OFF 4
#define ST_LED1_WAIT 5
#define ST_LED2_WAIT 6
#define ST_LED3_WAIT 7

// globalni promenna pro stav tlacitka SW1
bool button_down_pressed;
// Promenna state je nove lokalni uvnitr tasku (funkce) pro blikani

// Prototypy funkci
void TaskButton(void);
void TaskEffect(void);
void TaskPicoLed(void);

int main(void) {
    // inicializace ovladace pinu a delay
    init();

    // inicializace on-board LED
    pico_led_init();

    while (true) {
        TaskButton();
        TaskEffect();
        TaskPicoLed();
    }  // while

    /* Never leave main */
    return 0;
}

// Uloha, ktera se stara o obsluhu tlacitek
void TaskButton(void) {
    if (read_button() == BUTTON_PRESSED) {
        button_down_pressed = true;
    } else {
        button_down_pressed = false;
    }
}

// Uloha, ktera se stara o blikani LED
void TaskEffect(void) {
    // Stav totoho tasku.
    // Promenna je static, aby si uchovala hodnotu mezi volanimi teto funkce,
    // tj. aby nezanikla na konci funkce
    static int state = ST_LED1_ON;

    static uint32_t waitStart;  // cas, kdy se rozvitila LED, musi byt static!
    uint32_t currentTime;       // aktualni cas, pomocna promenna

    // Uloha efekt LED se provadi jen pri stisknutem tlacitku
    if (button_down_pressed) {
        switch (state) {
            case ST_LED1_ON:
                // Rozsvitit LED, ulozit aktualni cas a prejit do stavu cekani na
                // uplynuti casu svitu teto LED.
                LED_control(true, false, false);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED1_WAIT;
                break;

            case ST_LED1_WAIT:
                // Kontrola jestli uz ubehlo dost casu abychom rozsvitili dalsi LED
                // a pokud ano, prechod na dalsi stav
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED2_ON;
                }
                break;

            case ST_LED2_ON:
                LED_control(false, true, false);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED2_WAIT;
                break;

            case ST_LED2_WAIT:
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED3_ON;
                }
                break;

            case ST_LED3_ON:
                LED_control(false, false, true);
                waitStart = to_ms_since_boot(get_absolute_time());
                state = ST_LED3_WAIT;
                break;

            case ST_LED3_WAIT:
                currentTime = to_ms_since_boot(get_absolute_time());
                if (currentTime - waitStart >= BLINK_DELAY) {
                    state = ST_LED1_ON;
                }
                break;
        }  // switch
    } else {
        // zhasnout LED pokud neni stisknuto tlacitko
        LED_control(false, false, false);
        state = ST_LED1_ON;  // reset stavu tasku
    }
}  // TaskEffect

///////////////////////////////////
// Doba svitu/zhasnuti on-board LED
#define PICO_LED_ON_DELAY 200
#define PICO_LED_OFF_DELAY 700

// Ukazka dalsiho tasku - blika pri stisknutem tlacitku RGB LED
void TaskPicoLed() {
    static enum {
        ST_LED_ON,
        ST_ON_WAIT,
        ST_LED_OFF,
        ST_OFF_WAIT

    } stav = ST_LED_ON;

    static uint32_t startTime;

    // uloha se provadi jen pri stisknutem tlacitku
    if (button_down_pressed) {
        switch (stav) {
            case ST_LED_ON:
                pico_set_led(true);
                startTime = to_ms_since_boot(get_absolute_time());
                stav = ST_ON_WAIT;
                break;

            case ST_ON_WAIT:
                if (to_ms_since_boot(get_absolute_time()) - startTime >= PICO_LED_ON_DELAY) {
                    stav = ST_LED_OFF;
                }
                break;

            case ST_LED_OFF:
                pico_set_led(false);
                startTime = to_ms_since_boot(get_absolute_time());
                stav = ST_OFF_WAIT;
                break;

            case ST_OFF_WAIT:
                if (to_ms_since_boot(get_absolute_time()) - startTime >= PICO_LED_OFF_DELAY) {
                    stav = ST_LED_ON;
                }
                break;
        }  // switch
    } else {
        pico_set_led(false);  // zhasni LED
        stav = ST_LED_ON;     // resetuj stav LED
    }
}

//////////////////////////////////////////////////////////////////////////////////
#endif /* VERSION == 4*/

/////////////////////////////////////////////////////////
// Pomocne funkce spolecne pro vsechny verze

// inicializace tlacitka a externich LED diod
void init(void) {
    gpio_init(LED_GREEN);
    gpio_init(LED_YELLOW);
    gpio_init(LED_RED);
    gpio_init(BUTTON_DOWN_PIN);

    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);

    gpio_put(LED_GREEN, false);
    gpio_put(LED_YELLOW, false);
    gpio_put(LED_RED, false);
}

void LED_control(bool d1, bool d2, bool d3) {
    gpio_put(LED_GREEN, d1);
    gpio_put(LED_YELLOW, d2);
    gpio_put(LED_RED, d3);
}

/*
 Cte stav tlacitka BUTTON_DOWN s osetrenim zakmitu.
 Vraci BUTTON_NOT_PRESSED pokud tlacitko neni stisknuto,
 BUTTON_PRESSED pokud je stisknuto.
*/
int read_button(void) {
    int button_state = BUTTON_NOT_PRESSED;

    if (gpio_get(BUTTON_DOWN_PIN) == BUTTON_PRESSED) {
        sleep_ms(50);

        if (gpio_get(BUTTON_DOWN_PIN) == BUTTON_PRESSED) {
            button_state = BUTTON_PRESSED;
        }
    } else {
        button_state = BUTTON_NOT_PRESSED;
    }

    return button_state;
}

// Inicializace on-board LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

// Ovladani on-board LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Spaghetti version of the code
#if VERSION == 5
// Zlepseni odezvy pridanim kontroly tlacitka do stavu pro efekt.
// Odezva se sice zlepsi, ale struktura programu je neprehledna.

// Stavy programu
#define ST_EFFECT 1
#define ST_OFF 2

int state = ST_OFF;

int main(void) {
    // inicializace ovladace pinu a delay
    init();

    while (true) {
        if (read_button() == BUTTON_PRESSED)
            state = ST_EFFECT;
        else
            state = ST_OFF;

        switch (state) {
            case ST_OFF:
                LED_control(false, false, false);
                break;

            case ST_EFFECT:
                LED_control(true, false, false);
                sleep_ms(BLINK_DELAY);
                // Pokud neni stisknuto tlacitko prerus sekvenci
                if (read_button() != BUTTON_PRESSED)
                    break;
                LED_control(false, true, false);
                sleep_ms(BLINK_DELAY);
                // Pokud neni stisknuto tlacitko prerus sekvenci
                if (read_button() != BUTTON_PRESSED)
                    break;
                LED_control(false, false, true);
                sleep_ms(BLINK_DELAY);
                break;
        }  // switch

    }  // while

    /* Never leave main */
    return 0;
}

#endif