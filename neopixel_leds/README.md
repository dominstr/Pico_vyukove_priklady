# Neopixel leds
Deska Explorer Board obsahuje 4 NeoPixel RGB LED diody. Tyto diody jsou připojeny pouze na pin `1`. Každou z diod je možno individuálně adresovat.

Rozmístění diod na desce Explorer board:
Rozmístění||
---|---
0 | 1
2 | 3

## Doporučený postup
Naimportujte si tuto ukázku a prakticky odzkoušejte. Tato ukázka má 3 předchystané funkce pro světelné efekty. V nekonečné smyčce je možno tyto funkce zavolat a odzkoušet.

## Integrace do vlastního projektu
Do složky projektu nakopírujte soubor ws2812.pio z [této ukázky](https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812).

Upravte soubor CMakeLists.txt a zkonfigurujte CMake pomocí rozšíření Raspberry Pi Pico.
```
add_executable(nazev_projektu
        nazev_projektu.c
        ws2812.pio
        )

target_link_libraries(nazev_projektu
        hardware_pio
        hardware_clocks
        )
```

Spusťe terminál, přejděte do složky projektu a zadejte příkaz: `& "$env:PICO_SDK_PATH\tools\pioasm\build\pioasm.exe" -o c-sdk ws2812.pio ws2812.pio.h`

Ve složce projektu se vygeneruje soubor ws2812.pio.h.

Pokud se soubor nevygeneruje, zkontrolujte, zda je správně nastavena proměnná PICO_SDK_PATH: `echo $env:PICO_SDK_PATH`. Očekávaný výstup tohoto příkazu: `C:\Users\uzivatel\.pico-sdk\sdk\2.1.1`. Pokud je proměnná prázdná nebo nesprávná, nastavte ji: `$env:PICO_SDK_PATH = "C:\Users\uzivatel\.pico-sdk\sdk\2.1.1"`. Přejděte do adresáře tools/pioasm a zkontrolujte, jestli ve složce build existuje soubor `pioasm.exe`. Pokud složka build nebo soubor `pioasm.exe` neexistují, přejděte v terminálu do adresáře tools/pioasm a zadejte tyto příkazy:
```
mkdir build
cd build
cmake ..
cmake --build .
```
Spusťe znovu příkaz `"$env:PICO_SDK_PATH\tools\pioasm\build\pioasm.exe" -o c-sdk ws2812.pio ws2812.pio.h`.

Pokud je vše v pořádku, přidejte do programového souboru knihovny:
```
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
```