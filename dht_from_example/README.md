# DHT example
Tato ukázka popisuje, jak naimportovat ukázkový příklad od Raspberry Pi pro senzor vlhkosti a teploty DHT11 a upravit jej pro správnou funkci na desce Explorer Board.

## Založení nového projektu z ukázky
Klikneme na ikonku rozšíření **Raspberry Pi Pico Project** a vybereme možnost **New Project From Example**.

Z rozbalovací nabídky vybereme příklad **dht** a zvolíme správnou variantu desky Pico. Vybereme složku, kam se má náš projekt uložit a vytvoříme projekt kliknutím na **Create**.

## Nastavení projektu
Pro práci se sériovou linkou je třeba přidat tyto řádky do souboru CMakeLists.txt:
```
pico_enable_stdio_uart(dht 0)
pico_enable_stdio_usb(dht 1)
```
Pokud chceme používat i náš hlavičkový soubor s definicemi čísel pinů, přidáme ještě:
```
target_include_directories(dht PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)
```
Přidáme náš hlavičkový soubor do .c souboru projektu příkazem `#include "xplr.h"`.

V ukázkovém projektu změníme pin DHT senzoru z výchozího č. 15 na námi definovaný pin DHT11_PIN, což je pin č. 0.

`const uint DHT_PIN = DHT11_PIN;`

Pro správné čtení dat z DHT senzoru je třeba uvnitř funkce `read_from_dht(dht_reading *result)` změnit `sleep_us(1)` na hodnotu `sleep_us(2)`.

Správnou funkci programu otestujeme a na sériové lince by měl být výpis ve tvaru:
```
Humidity = 54.0%, Temperature = 21.0C (69.8F)
```
Pokud nastane nějaká chyba (třeba není správná hodnota `sleep_us()`), tak dostaneme výpis ve tvaru:
```
Bad data
Humidity = 0.0%, Temperature = 0.0C (32.0F)
```

## Funkce pro čtení hodnot z DHT senzoru
Předchystaná funkce `read_from_dht(dht_reading *result)` se za nás postará o čtení dat ze senzoru.

Pokud nedokáže přečíst data ze senzoru, vypíše text `Bad data` na sériový výstup.

## Hlavní programu
```
while (1) {
    dht_reading reading;
    read_from_dht(&reading);
    float fahrenheit = (reading.temp_celsius * 9 / 5) + 32;
    printf("Humidity = %.1f %%, Temperature = %.1f °C (%.1f °F)\n",
            reading.humidity, reading.temp_celsius, fahrenheit);

    sleep_ms(2000);
}
```

V nekonečné smyčce se do struktury `reading` ukládají přečtená data ze senzoru. Jedná se o hodnoty `humidity` (vlhkost v %) a `temp_celsius` (teplota ve °C). V ukázce je i dodatečný převod na °F.

Následně se data vypíší na sériový výstup. Zástupné znaky `%.1f` naformátují číslo s jedním desetinným místem. Vlhkost je udávána v `%`. Pro zobrazení znaku `%` je třeba použít dva znaky `%%` za sebou, musíme tedy použít formát `%.1f%%`.

Výpis můžeme zpřehlednit přidáním mezer. Pokud sériový monitor podporuje kódování UTF-8, můžeme použít i znak `°`. 

```
printf("Humidity = %.1f %%, Temperature = %.1f °C (%.1f °F)\n",
               reading.humidity, reading.temp_celsius, fahrenheit);
```

Na sériový výstup dostaneme tedy přehlednější výpis ve tvaru:
```
Humidity = 54.0 %, Temperature = 21.0 °C (69.8 °F)
```