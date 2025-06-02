# Time measure - měření času zápisu na GPIO
Tato ukázka slouží pro porovnání časů zápisů na GPIO piny pomocí různých způsobů zápisu.

## Doporučený postup
Naimportujte tuto ukázku, nahrajte na Pico a zkontrolujte naměřené hodnoty pomocí Serial Monitoru.

Naměřené hodnoty:
```
Zapis na GPIO pomoci funkce:
Pocet cyklu: 10000000, cas: 480051 us

Zapis na GPIO pomoci registru:
Pocet cyklu: 10000000, cas: 480044 us

Zapis na vestavenou LED na Pico W:
Pocet cyklu: 215, cas: 490401 us
```
Z naměřených hodnot vyplývá, že 10 milionů cyklů zápisů na GPIO externí LED diody trvá přibližně 0,48 s. Časový rozdíl mezi zápisem pomocí funkce nebo pomocí registrů je téměř zanedbatelný. To může být způsobeno např. dobrou optimalizací při překladu.

Zápis na vestavěnou LED pomocí funkce Wi-Fi modulu je ale znatelně pomalejší. Za stejnou dobu zvládne provést pouze 215 cyklů zápisů.

## Popis programu
### pico_led_init(void)
Funkce pro inicializaci vestavěné LED diody.

### pico_set_led(led_on)
Tato funkce slouží pro nastavení vestavěné LED diody pomocí funkce `gpio_put(PICO_DEFAULT_LED_PIN, led_on)` nebo v případě Pico W pomocí Wi-Fi modulu `cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on)`.

### external_led_init(void)
Inicializace externí LED diody. (Pro tvorbu této ukázky bylo použito Pico W, proto není možno přistupovat k vestavěné LED jinak, než přes funkci Wi-Fi modulu.)

### time_us_32()
Tato funkce vrací počet mikrosekund od zapnutí zařízení nebo posledního resetu. Díky tomu můžeme porovnat čas před zápisem na GPIO a čas po zápisu.

### write_gpio_using_func(uint pin, uint32_t cycles)
Tato funkce zapíše hodnotu `true` a následně zase `false` na příslušný pin podle přednastaveného počtu opakování.

### write_gpio_using_registers(uint pin, uint32_t cycles)
Pomocí této funkce se provede přímý zápis do GPIO registrů podle přednastaveného počtu opakování.

### write_built_in_led(uint32_t cycles)
Tato funkce zapíše hodnotu `true` a následně zase `false` na vestavěnou LED diodu podle přednastaveného počtu opakování.