# Čtení stavu tlačítka
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


## Čtení tlačítka
Vytvoříme program, kdy bude při stisknutém tlačítku svítit vestavěná LED dioda.

Nejdříve nastavíme pin tlačítka jako GPIO a nastavíme ho jako vstup.:
```
gpio_init(BUTTON_DOWN_PIN);
gpio_set_dir(BUTTON_DOWN_PIN, GPIO_IN);
```
Uvnitř nekonečné smyčky budeme zjišťovat stav tlačítka.

Pokud je tlačítko rozepnuté (log. 1 na vstupu), diodu nastavíme na **false**.

Pokud je tlačítko sepnuté (log. 0 na vstupu), nastavíme diodu na **true**.
```
if (gpio_get(BUTTON_DOWN_PIN) == 1) {
    pico_set_led(false);
} else {
    pico_set_led(true);
}
sleep_ms(100);
```
