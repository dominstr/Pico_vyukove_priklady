# Button debounce
V této ukázce je znázorněno, jak číst tlačítka a ošetřit zákmity (debounce).

## Zapojení tlačítek
Tlačítka jsou zapojena mezi GND a příslušné GPIO piny:

`BUTTON_LEFT` - 15

`BUTTON_UP` - 10

`BUTTON_RIGHT` - 11

`BUTTON_DOWN` - 14

Na vstupu pinů se používá vnitřní Pull-up rezistor, kdy při sepnutém tlačítku bude na vstupu pinu log. 0 a při rozepnutém tlačítku bude na vstupu log. 1.

Pokud je na vstupu hodnota napětí 0-0,8 V, je detekována log. 0.

Pokud je na vstupu hodnota napětí 2-3,3 V, je detekována log. 1.

![Pull-up a pull-down rezistory](/readme_images/pullup_pulldown.jpg)
(zdroj obrázku: https://i.ytimg.com/vi/aPVMKyZpaPA/maxresdefault.jpg)


## Čtení tlačítka s ošetřením zákmitu
Vytvoříme program, kdy s každým stiskem tlačítka změníme stav diody.

Vytvoříme funkce pro inicializaci a ovládání vestavěné LED diody. Pro Pico W nezapomeneme naimportovat hlavičkový soubor pro práci s LED (musí se upravit i CMakeLists.txt).
```
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// inicializace on-board LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

// ovladani on-board LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}
```

Nadefinujeme si makra pro stavy tlačítka, vytvoříme funkce pro inicializaci a čtení tlačítka s ošetřením zákmitu.

```
#define BUTTON_PRESSED 0
#define BUTTON_NOT_PRESSED 1

// inicializace tlacitka
void button_init() {
    gpio_init(BUTTON_DOWN_PIN);
    gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_DOWN_PIN);
}

// cteni stavu tlacitka s debounce 50 ms
int read_button() {
    int button_state = BUTTON_NOT_PRESSED;

    if (gpio_get(BUTTON_DOWN_PIN) == 0) {
        sleep_ms(50);
        if (gpio_get(BUTTON_DOWN_PIN) == 0) {
            button_state = BUTTON_PRESSED;
        }
    }

    return button_state;
}
```
Ve funkci read_button() zjišťujeme, jestli je tlačítko sepnuté (log. 0 na vstupu), počkáme 50 ms a znovu ověříme stav tlačítka. Pokud i po této době je tlačítko zachyceno jako sepnuté, vrátíme stav `BUTTON_PRESSED`, jinak vrací stav `BUTTON_NOT_PRESSED`.

Ve smyčce main zinicializujeme GPIO:
```
stdio_init_all();
int rc = pico_led_init();
hard_assert(rc == PICO_OK);
button_init();
```
Vytvoříme si proměnnou `led_on` typu **bool**, která bude sloužit k ukládání stavu LED. Nastavíme ji ze začátku na false. Dále je potřeba proměnné typu int state a previous_state, do kterých budeme ukládat stavy tlačítek. Musíme zavolat i funkci `pico_set_led(led_on)`, aby byla nastavená výchozí hodnota LED:
```
bool led_on = false;
int state;
int previous_state = BUTTON_NOT_PRESSED;

pico_set_led(led_on);
```
V nekonečné smyčce budeme zjišťovat stav tlačítka. Pokud je stav tlačítka BUTTON_PRESSED a zároveň byl předchozí stav BUTTON_NOT_PRESSED (detekujeme náběžnou hranu), tak znegujeme hodnotu led_on a podle ní nastavíme hodnotu LED:
```
state = read_button();

if (state == BUTTON_PRESSED && previous_state == BUTTON_NOT_PRESSED) {
    led_on = !led_on;
    pico_set_led(led_on);
}

previous_state = state;
```
Kdyby se nekontroloval předchozí stav, tlačítko by mohlo být zaregistrováno vícekrát, i když zdánlivě klikneme jen jednou. Můžeme si to ověřit jednoduchým výpisem do konzole:
```
if (state == BUTTON_PRESSED) {
    printf("Tlacitko zmacknuto\n");
}
```