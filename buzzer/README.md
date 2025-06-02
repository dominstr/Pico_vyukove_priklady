# Buzzer - bzučák
Ukázka ovládání pasivního bzučáku ovládaného pomocí pulzně-šířkové modulace (PWM). Při stisku tlačítka BUTTON_DOWN se rozezní bzučák.

## Doporučený postup
Naimportujte tuto ukázku a prakticky odzkoušejte.

## Popis programu
Na začátku se inicializuje vstupní pin tlačítka a výstupní pin bzučáku, který je nataven jako PWM výstup. Pokud je tlačítko stisknuto, aktivuje se generování signálu o frekvenci 1000 Hz pomocí PWM signálu se střídou 50 %. Jakmile uživatel tlačítko pustí, generování tónu se zastaví.

## Popis funkcí
### gpio_set_function(pin, GPIO_FUNC_PWM)
Nastaví daný pin jako PWM výstup. To umožní generovat na tomto pinu periodický signál.

### pwm_gpio_to_slice_num(pin)
Vrátí číslo tzv. slice (části PWM modulu), který ovládá daný GPIO pin.

### pwm_set_clkdiv(slice_num, clkdiv)
Nastaví dělič hodinového signálu pro daný PWM slice. Tím se upravuje frekvence signálu.

### pwm_set_wrap(slice_num, value)
Nastaví maximální hodnotu čítače (tzv. wrap value). Určuje délku PWM periody.

### pwm_set_chan_level(slice_num, channel, value)
Nastaví střídu signálu.

### pwm_set_enabled(slice_num, true/false)
Zapne nebo vypne PWM výstup na daném slice.