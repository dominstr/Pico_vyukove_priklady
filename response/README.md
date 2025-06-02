# Response - odezva programu
Tato ukázka má za cíl předvést odezvu programu a jak ji snížit, aby program správně reagoval na stisk tlačítka.

Tento příklad byl upraven pro použití na Raspberry Pi Pico z původního [zdroje](https://github.com/jdolinay/utb_frdm_vyuka/blob/master/programs/response/response.c) výuky na UTB.

## Odezva programu
Odezva programu znamená, jak rychle dokáže mikropočítač zareagovat na nějakou událost, jako je třeba stisknutí tlačítka.

## Zapojení
`LED_RED` - 20

`LED_GREEN` - 21

`LED_YELLOW` - 22

`BUTTON_DOWN` - 14

Diody se rozsvítí přivedením log. 1 na výstup pinu.

Tlačítko je zapojeno mezi GND a příslušný GPIO pin. Na vstupu tohoto pinu se používá vnitřní Pull-up rezistor, kdy při sepnutém tlačítku bude na vstupu pinu log. 0 a při rozepnutém tlačítku bude na vstupu log. 1.

Pokud je na vstupu hodnota napětí 0-0,8 V, je detekována log. 0.

Pokud je na vstupu hodnota napětí 2-3,3 V, je detekována log. 1.

![Pull-up a pull-down rezistory](/readme_images/pullup_pulldown.jpg)
(zdroj obrázku: https://i.ytimg.com/vi/aPVMKyZpaPA/maxresdefault.jpg)

## Zadání úlohy
Na začátku nebude svítit žádná dioda. Dokud bude tlačítko stisknuté poběží světelný had - postupně se budou rozsvicovat diody. Tzn. svítí pouze LED1, potom svítí pouze LED2, potom svítí pouze LED3 a potom bude opět svítit LED1. Doba svícení je určena hodnotou `BLINK_DELAY`. Po uvolnění tlačítka se efekt zastaví.

## Doporučený postup
Naimportujeme si tento příklad do VS Code. Vybereme rozšíření Raspberry Pi Pico, zvolíme Import Project. Kliknutím na tlačítko Change zvolíme cestu k této ukázce a stiskneme tlačítko Import.

Tato ukázka má již 5 předchystaných variant kódu. Variantu kódu vybereme změnou hodnoty makra **VERSION** v horní části kódu. Aktivní část kódu je v IDE běžným textem, neaktivní část je zešednutá.

Pro všechny varianty kódu jsou ve spodní části funkce, jako třeba čtení tlačítka.

Postupně projděte všechny varianty a prakticky odzkoušejte postupné vylepšování odezvy programu.

## Verze kódu
### Verze 1
Tento kód je napsán jako konečný automat:

`ST_OFF` - žádná LED nesvítí

`ST_EFFECT` - probíhá světelný efekt

Program nejdříve kontroluje, zda je tlačítko stisknuto. Pokud je stisknuto, přejde do stavu **ST_EFFECT**, jinak do stavu **ST_OFF**.

Dále se pomocí příkazu switch vykoná patřičná část kódu.

Pro stav **ST_EFFECT** rozsvítí první LED, čeká určitou dobu, rozsvítí druhou LED, čeká určitou dobu atd.

Pro stav **ST_OFF** zhasne všechny LED diody.

Na stisk tlačítka program reaguje rychle, protože se kontroluje pouze stav tlačítka.
Pro uvolnění tlačítka je odezva horší, protože stav tlačítka se testuje až, když je dokončen celý efekt (až uběhne čekací čas po 3. LED diodě).

### Verze 2
Oproti předchozí verzi jsme stav efekt rozdělili na stavy pro každou LED zvlášť.
Tlačítko tedy můžeme kontrolovat po dokončení efektu každé z diod.

Odezva se sice zlepší, už nemusíme čekat na doběhnutí celého efektu, ale pořád zůstane odezva o délce svitu jedné LED.

### Verze 3
V této verzi bude mít každá dioda 2 stavy. Příklad stavů pro LED1:

**ST_LED1_ON** - pro rozsvícení LED

**ST_LED1_OFF** - pro čekání na uplynutí doby svitu

 Nepoužíváme čekání (busy waiting), kdy je procesor vytížen tímto čekáním na určitý čas. Místo toho budeme zjišťovat, jestli uběhl potřebný čas pro přechod do dalšího stavu. Jedná se o tzv. dotazování (polling).

Díky tomu program nikde nečeká a můžeme kontrolovat tlačítko častěji. Dioda zhasne okamžitě po uvolnění tlačítka.

Budem potřebovat dvě proměnné typu **uint32_t** pro odměřování času:

`waitStart` - pro uložení aktuálního času na začátku efektu jednotlivých diod

`currentTime` - pro získání aktuální času v každém stavu **WAIT** jednotlivých diod

Hodnoty časů získáme následovně:
```
 waitStart = to_ms_since_boot(get_absolute_time())
 ```

 `get_absolute_time()` - vrací absolutní čas od zapnutí zařízení ve formátu `absolute_time_t`

 `to_ms_since_boot(...)` - převede absolutní čas z formátu `absolute_time_t` na počet ms

 Výsledek rozdílu mezi `current_time`a `waitStart` porovnáme s dobou svitu `BLINK_DELAY` a pokud uplynul stanovený čas, tak přejdeme do dalšího stavu.

### Verze 4
Program je rozdělen na dílčí úlohy - tasky:

`TaskButton()` - čtení tlačítka

`TaskEffect()` - efekt běžícího světla

 Odezva je stejná jako ve verzi 3, ale struktura programu je přehlednější a může se snadno rozšířit o další tasky:
 
 `TaskPicoLed()` - blikání on-board LED
 
  Každá úloha se ale musí provést v co nejkratším čase.

### Verze 5
Tato varianta slouží jako ukázka nepřehledného kódu.










## Společné pomocné funkce programu
### void init(void)
Zinicializuje potřebné GPIO pro externí LED diody a tlačítko.

### void LED_control(bool d1, bool d2, bool d3)
Nastaví hodnoty pro všechny 3 externí LED diody podle parametrů.

`true` - svítí

`false` - nesvítí

### int read_button(void)
Slouží ke čtení tlačítka s ošetřením zákmitu. Pokud bylo tlačítko stisknuto, vrátí `BUTTON_PRESSED` (0). Pokud nebylo stisknuto, vrátí `BUTTON_NOT_PRESSED` - (1).

### int pico_led_init(void)
Slouží pro inicializace on-board LED. Komatibilní s Pico a Pico W.

### void pico_set_led(bool led_on)
Nastaví on-board LED podle parametru.

`true` - svítí

`false` - nesvítí