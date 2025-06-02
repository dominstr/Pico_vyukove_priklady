# Hello ST7735
Tato ukázka demonstruje, jak používat knihovnu [displaylib_16bit_PICO](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/tree/main) pro práci s TFT displejem na vývojové desce Explorer Board. Komunikace s displejem probíhá přes SPI rozhraní.

## Postup
Tuto ukázku stačí naimportovat a spustit pro ověření funkčnosti displeje. Po nahrání programu se na malou chvíli zobrazí hláška `Hello World`.

## Použití displeje ve vlastním projektu
### Úprava CMakeLists.txt
Pro správnou funkci je třeba nastavit správný standard pro jazyk C++ pomocí `set(CMAKE_CXX_STANDARD 20)`.

Následně je třeba přidat několik zdrojových souborů pomocí:
```
add_executable(hello_st7735 hello_st7735.cpp
  ../shared/displaylib/src/displaylib_16/st7735.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_graphics.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Print.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Font.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_common.cpp
  ../shared/displaylib/src/displaylib_16/Bitmap_TEST_Data.cpp
)
```

Poté je třeba přidat příkaz `target_compile_options(hello_st7735 PUBLIC "-std=c++20")`, který zajistí, že program bude kompilován podle standardu C++20

Následně už stačí jen přidat potřebné knihovny pomocí:
```
target_include_directories(project_name PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../shared/displaylib/include
    ${CMAKE_CURRENT_LIST_DIR}/../shared
)

target_link_libraries(project_name
    pico_stdlib
    hardware_spi
)
```

### Úprava zdrojového souboru
Pro práci s knihovnou displeje je potřeba používat jazyk C++ (zdrojový soubor .cpp).

## Práce s displejem
Pin `TFT_BL_PIN` se nastaví jako výstup a bude sloužit k ovládání podsvícení displeje.

Inicializace displeje se provede pomocí funkce `TFTInitSPIType`, která nakonfiguruje SPI komunikaci.

### setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT)
Inicializuje všechny piny potřebné pro ovládání displeje.

### TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT)
Umožňuje nastavit rozměry displeje. Deska Explorer Board má displej s rozlišením 128 x 160 px.

### TFTInitPCBType(myTFT.TFT_ST7735R_Red)
Zvolí správný typ displeje. Deska Explorer Board používá typ TFT_ST7735R_Red.

### fillScreen(myTFT.C_BLACK)
Vyplní displej černou barvou.

### setTextColor(myTFT.C_GREEN, myTFT.C_BLACK)
Nastaví barvu textu na zelenou a pozadí textu bude černé.

### setCursor(5,5);
Nastaví kurzor pro vykreslení textu na dané souřadnice.

### setFont(font_default)
Nastaví font na výchozí.
