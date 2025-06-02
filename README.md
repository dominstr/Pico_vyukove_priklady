# Výukové příklady pro Rasbperry Pi Pico
Ukázkové příklady jsou psány pro použití na rozšiřující desce Joy-IT Explorer Board. [Oficiální dokumentace](https://joy-it.net/en/products/RB-P-XPLR)

Pro relé a RGB LED diody je třeba desku Explorer Board napájet přes USB-C kabel.

## Ukázkové příklady
Ukázky v tomto repozitáří stačí jednoduše naimportovat do VS Code a prakticky odzkoušet.

Ukázky s názvem končícím na `_from_example` jsou zamýšleny také jako návody pro import již existujících oficiálních ukázek od Raspberry Pi z [oficiálního repozitáře](https://github.com/raspberrypi/pico-examples). Tyto ukázky vyžadují ještě dodatečnou úpravu jako nastavení sériové linky nebo změnu čísel pinů pro použití na desce Explorer Board. V případě potřeby je možno ukázky `_from_example` i přímo naimportovat a spustit.

### Základní ukázky
Program|Popis
---|---
[blink_from_example](blink_from_example/) | Ukázka práce s vestavěnou LED diodou.
[read_button](read_button/) | Základní čtení tlačítka.
[button_debounce](button_debounce/) | Ošetření zákmitu tlačítka.
[serial_setup_from_example](serial_setup_from_example/) | Nastavení sériové linky.

### Senzory
Program|Popis
---|---
[dht_from_example](dht_from_example/) | Senzor teploty a vlhkosti DHT11.
[hello_adc_from_example](hello_adc_from_example/) | Čtení analogové hodnoty z potenciometru.

### Výstupní zařízení
Program|Popis
---|---
[relay](relay/) | Ovládání relé.
[buzzer](buzzer/) | Bzučák.

### Řízení LED diod
Program|Popis
---|---
[blink_traffic_lights](blink_traffic_lights/) | Jednoduchý semafor s externími diodami.
[traffic_lights_state_machine](traffic_lights_state_machine/) | Semafor pomocí konečného automatu.
[neopixel_leds](neopixel_leds/) | Ovládání NeoPixel LED diod pomocí PIO.
[pwm_setup](pwm_setup/) | Nastavení jasu diod pomocí PWM.
[potentiometer_brightness](potentiometer_brightness/) | Nastavení jasu diody pomocí potenciometru.

### Pokročilé ukázky
Program|Popis
---|---
[response](response/) | Odezva programu.
[freertos_setup](freertos_setup/) | Nastavení FreeRTOS a základní tasky.
[hello_st7735](hello_st7735/) | Hello World na TFT displeji pro driver ST7735.
[http_request](http_request/) | Jednoduchý HTTP request pro získání aktuálního počasí z OpenWeather API.
[time_measure](time_measure/) | Měření času zápisů na GPIO přes funkce, registry a Pico W funkci.
[weather_station](weather_station/) | Meteostanice - aktuální počasí z API + DHT11.
[timer_blink](timer_blink/) | Blikání LED pomocí přerušení a časovače

## Složka shared
V této složce se nachází hlavičkový soubor xplr.h, který obsahuje definice čísel pinů pro snadnější práci.

Aby bylo možno používat hlavičkový soubor xplr.h v novém projektu, je třeba přidat cestu ke složce `shared` přidáním `${CMAKE_SOURCE_DIR}/../shared` do souboru `CMakeLists.txt`:
```
# Add the standard include files to the build
target_include_directories(blink PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)
```
Dále jsou zde knihovny pro TFT displej, parsování JSON, FreeRTOS.

## Rozmístění periferií na desce Explorer Board
Pin na Explorer Board|Pin na RP Pico
---|---
DHT11|GP0
NeoPixel LED diody|GP1
I2C - SDA|4
I2C - SCL|5
SERVO 1|7
SERVO 2|8
SERVO 3|9
SERVO 4|20
TFT - podsvícení|2
TFT - DC|3
TFT - RES|6
TFT - CS|17
SPI - SCK|18
SPI - MISO|16
SPI - MOSI|19
UART - TX|12
UART - RX|13
CROCO 21|21
CROCO 22|22
CROCO 26|26
Bzučák|27
Relé|28