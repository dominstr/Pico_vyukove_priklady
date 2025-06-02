# Blink example
Tato ukázka má za cíl seznámit uživatele s ovládáním LED diody s využitím ukázkového příkladu přímo od Raspberry Pi.

## Založení nového projektu z ukázky
Klikneme na ikonku rozšíření **Raspberry Pi Pico Project** a vybereme možnost **New Project From Example**.

Z rozbalovací nabídky vybereme příklad **blink** a zvolíme správnou variantu desky Pico. Vybereme složku, kam se má náš projekt uložit a vytvoříme projekt kliknutím na **Create**.

## Vestavěná LED dioda
Pico má vestavěnou diodu `PICO_DEFAULT_LED_PIN` na pinu `20`.

Dioda se rozsvítí přivedením výstupní log. 1 na příslušný pin.

Ve verzi Pico W pin č. 20 slouží pro wifi modul. Nelze tedy ovládat vestavěnou diodu tímto pinem a musí se spínat přes wifi modul, jako je to v této ukázce.

CMake nakonfiguruje soubory podle typu desky. Pico W má definován pin `CYW43_WL_GPIO_LED_PIN`. Podle toho, jestli byl tento pin definován se vybere správná varianta kódu. VS Code označí neaktivní kód zešednutím. Tímto způsobem lze psát kód tak, aby byl univerzální mezi více Pico variantami

## Hlavní struktura programu
Ve smyčce `main` se nejdříve otestuje, zda se povedla inicializace GPIO pinu. Pokud se inicializace nepovede, program se zastaví.
Po úspěšné inicializaci program pokračuje do nekonečné smyčky `while`. Program uvnitř smyčky while se vykonává stále dokola.

## Funkce pro inicializaci GPIO
Ve funkci `pico_led_init(void)` se provede sekvence následujících příkazů:
```
gpio_init(PICO_DEFAULT_LED_PIN);
gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
return PICO_OK;
```
Nejprve se nastaví příslušný pin pro práci s GPIO.
Poté se pin nastaví jako výstup.
Na konec vrátí hodnotu `PICO_OK`, což je kód 0.

Pro variantu Pico W celou tuhle incializaci obstará wifi modul příkazem `return cyw43_arch_init();`

## Funkce pro blikání diodou
Uvnitř funkce `pico_set_led(led_on)` se dioda ovládá voláním funkce `gpio_put(PICO_DEFAULT_LED_PIN, led_on)`.

Pro Pico W se volá funkce `cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on)`.

Parametr `led_on` určuje, zda bude dioda rozsvícená (**true**) nebo zhasnutá (**false**).

## Hlavní program
V nekonečné smyčce `while` budeme rozsvicovat a zhasínat diodu voláním funkce `pico_set_led(led_on)` a mezi změnami stavů LED bude program čekat o stanovený počet ms pomocí funkce `sleep_ms(LED_DELAY_MS)`.

V hlavičce souboru můžeme definovat hodnotu makra `LED_DELAY_MS` pomocí `#define LED_DELAY_MS 250`. Při kompilaci programu se makro nahradí číselnou hodnotou, což v případě embedded zařízení šetří paměť.