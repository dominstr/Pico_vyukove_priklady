# Hello ADC from example
Tato ukázka popisuje jak naimportovat ukázkový projekt hello_adc od Raspberry Pi pro jednoduché čtení analogových hodnot.

## Zapojení
Analogové hodnoty budeme číst na pinu `26`.

### Potenciometr
Potenciometr má 3 vývody. Prostřední zapojíme na pin `26`. Ostatní piny zapojíme na GND a 3V3. Tím vznikne napěťový dělič.

### Fotorezistor
Pokud chceme použít fotorezistor, musíme si vytvořit napěťový dělič pomocí rezistoru 10 kΩ. Jeden vývod fotorezistoru připojíme na pin 3V3. Rezistor připojíme na GND a výstup napěťového děliče připojíme na pin `26`.

## Postup
Zapojíme potenciometr nebo fotorezistor dle zapojení výše.

Ve VS code vybereme rozšíření Raspberry Pi Pico a zvolíme možnost New Project From Example. Vybereme projekt hello_adc, zvolím správnou variantu desky. Zvolíme umístění projektu tlačítkem Change. Projekt vytvoříme kliknutím na tlačítko Create.

Do souboru CMakeLists.txt přidáme následující řádky:
```
pico_enable_stdio_uart(hello_adc 0)
pico_enable_stdio_usb(hello_adc 1)
```

V rozšíření Raspberry Pi Pico vybereme možnost Configure CMake. Program nahrajeme tlačítkem Run.

Otevřeme Serial Monitor a vybereme správný port. Po stisknutí tlačítka Start Monitoring program začne vypisovat naměřené hodnoty.