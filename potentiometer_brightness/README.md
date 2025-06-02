# Potentiometer brightness - ovládání jasu diody pomocí potenciometru
Tato ukázka popisuje krok za krokem, jak napsat program pro ovládání jasu diody pomocí potenciometru s využitím PWM.

## Zapojení
Zapojení diod:

LED_RED - `20`

LED_GREEN - `21`

LED_YELLOW - `22`

Potenciometr je zapojen na pin `26`.

## Zadání
Naprogramujte program tak, aby četl analogovou hodnotu z potenciometru a podle toho měnil jas diody 0-100 %. Periodu PWM nastavte na 256 cyklů (0-255). Program čleňte do funkcí.

## Postup
Zapojte diody a potenciometr dle zapojení výše a naimportujte ukázku. Prakticky odzkoušejte.

### Rozšíření programu
Upravte program tak, aby využil všechny 3 LED diody a postupně se rozsvicovaly včetně jasu jednotlivých diod.

Do inicializačních funkcí zahrňte i další dvě diody. Vytvořte další funkci `void update_leds(uint8_t)`, která bude přepočítávat analogovou hodnotu `value` v rozsahu 0-255 pro rozdělení mezi 3 diody.

`LED_GREEN` - rozsah `value` 0-84

`LED_YELLOW` - rozsah `value` 85-169

`LED_RED` - rozsah `value` 170-255

Funkce `update_leds(uint8_t)` poté bude nastavovat každou z diod na hodnotu 0-255.

## Funkce programu
### void init_pwm(void)
Slouží k inicializaci GPIO pro práci s PWM.

### void init_adc(void)
Inicializuje analogový vstup.

### uint8_t read_adc(void)
Přečte 12-bitovou hodnotu (0-4095) a potom ji převede na 8-bitovou (0-255).

### set_led_pwm(gpio_pin, level)
Nastaví pin `gpio_pin` na hodnotu PWM `level`.
