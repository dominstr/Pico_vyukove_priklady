# PWM setup
V této ukázce je představeno základní použití pulzně-šířkové modulace (PWM).

## PWM
Výstup mikropočítače je pouze digitální - low (0 V) nebo high (3,3 V). Díky tomu může LED dioda buď svítit na plno nebo nesvítit vůbec.

Pokud bysme chtěli rozvítit diodu na poloviční jas, nemůžeme nastavit konkrétní analogovou hodnotu na výstup (např. hodnotu 1,7 V).

Existuje možnost, jak si vyrobit falešný analogový signál pomocí pulzně-šířkové modulace (pulse-width modulation - PWM).

Pokud budeme výstup zapínat a vypínat, vytvoříme pulz. Změnou, jak rychle se výstup zapíná a vypíná, můžeme modulovat šířku pulzu, takže ovlivňujeme jakou část periody je signál v aktivní úrovni.

Každý GPIO pin Pica umožňuje použití PWM, ale Pico má jen 8 nezávislých PWM modulů nazývané *slices*. Každý *slice* má dva kanály, tedy má celkově 16 kanálů.

![Mapování PWM kanálu na GPIO piny RP2040](/readme_images/pwm_gpio.jpg)
(zdroj obrázku: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)

Jak je patrné z tabulky výše, každý pin s PWM má číslo a písmeno. Číslo udává, který PWM *slice* je připojen k pinu, písmeno udává, který z dvojice výstupů byl použit.

Musíme si dát pozor, abysme nepoužili dvakrát některý PWM kanál se shodným písmenem i číslem. Např. PWM_A[0] se nachází na GP0 i GP16.

## Zapojení
`20` - červená LED

`21` - zelená LED

`22` - žlutá LED

Diody se rozsvítí přivedením napětí na tyto piny.

## Popis programu
### gpio_set_function(pin_number, GPIO_FUNC_PWM)
Nastaví patřičný GPIO pin jako PWM výstup.

### pwm_gpio_to_slice_num(pin_number)
Zjistí, který *slice* je použit na příslušném pinu (možno vyčíst i z dokumentace).

### pwm_set_wrap(slice_num, max_duty)
Nastaví max. hodnotu čítače PWM pro konkrétní *slice*. Po dosažení této hodnoty čítač přeteče a začne čítat od 0, čímž se opakuje PWM cyklus.

3 -> 0-3

255 -> 0-255

Podle potřeby můžeme tedy měnit rozlišení. Maximální hodnota je 65535, tedy 16bitové rozlišení.

### pwm_set_chan_level(slice_num, pwm_chan, duty)
Nastaví hodnotu střídy (duty) pro daný kanál v rámci *slice*. Při `max_duty = 3` je perioda PWM cyklu rozdělena na 4 cykly. Hodnota `duty` udává, kolik je v periodě aktivních cyklů.

| Hodnota duty | Střída |
| ------------ |-------:|
| 0            | 0 %    |
| 1            | 25 %   |
| 2            | 50 %   |
| 3            | 75 %   |
| 4            | 100 %  |

Střída 100 % je pro hodnotu `max_duty + 1`.

![Průběhy signálů pro kanály A a B pro periodu o hodnotě 4 cyklů](/readme_images/pwm_graph.jpg)
(zdroj obrázku: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)


### pwm_set_enabled(slice_num, true)
Spustí PWM pro konkrétní *slice*.

## Ověření maximální hodnoty
Pro snadné ověření, že střída 100 % je opravdu až pro hodnotu `max_duty + 1` odkomentujeme nekonečnou smyčku:
```
while (true) {
    pwm_set_gpio_level(LED_YELLOW, 3);
    sleep_ms(1000);
    pwm_set_gpio_level(LED_YELLOW, 4);
    sleep_ms(1000);
}
```
Tím se bude jas diody měnit z hodnoty střídy `3` (75 %) na hodnotu `4` (100 %).
