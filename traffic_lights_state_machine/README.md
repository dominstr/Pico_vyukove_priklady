# Traffic lights - state machine
Ukázka ovládání semaforu pomocí jednoduchého konečného automatu.

Deska Explorer Board obsahuje adresovatelné Neopixel diody, pro které nelze použít tento způsob programování.
Pro tuto ukázku je potřeba mít externí diody na nepájivém poli.

## Zapojení LED diod
V ukázce jsou LED diody jsou připojeny na následující GPIO:
- `LED_RED` - `20`
- `LED_GREEN` - `21`
- `LED_YELLOW` - `22`

Diody se rozsvítí přivedením výstupní log. 1 na příslušný GPIO.

## Zadání úlohy
Naprogramujte Semafor pomocí konečného automatu.
1. Signál "**Stůj!**" - červená svítí 7 s
2. Signál "**Připravit k jízdě!**" - červená a současně žlutá svítí 2 s
3. Signál "**Stůj!**" - zelená svítí 7 s
4. Signál "**Pozor!**" - žlutá svítí 2 s

## Doporučený postup
Vytvořte funkci `traffic_lights_init(void)`, která zinicializuje GPIO pro LED a nastaví je jako výstup.

Dále vytvořte funkci `set_traffic_lights(bool green_on, bool yellow_on, bool red_on)`, která přijme parametry typu `bool` pro všechny 3 diody najednou a nastaví každou LED pomocí `gpio_put()`.

Definujte 4 stavy pro konečný automat:
```
typedef enum {
    STATE_RED,
    STATE_RED_AND_YELLOW,
    STATE_GREEN,
    STATE_YELLOW
} traffic_light_state_t;
```
V hlavní smyčce nastavte počáteční stav konečného automatu.

V nekonečné smyčce budeme pomocí switch-case přepínat mezi stavy. Aktuální stav konečného automatu si můžeme vypisovat i na sériovou linku.
```
switch (state) {
    case STATE_RED:
        printf("STATE_RED\n");
        set_traffic_lights(false, false, true);
        sleep_ms(7000);
        state = STATE_RED_AND_YELLOW;
        break;
    
    case STATE_RED_AND_YELLOW:
        printf("STATE_RED_AND_YELLOW\n");
        set_traffic_lights(false, true, true);
        sleep_ms(2000);
        state = STATE_GREEN;
        break;
    
    case STATE_GREEN:
        printf("STATE_GREEN\n");
        set_traffic_lights(true, false, false);
        sleep_ms(7000);
        state = STATE_YELLOW;
        break;
    
    case STATE_YELLOW:
        printf("STATE_YELLOW\n");
        set_traffic_lights(false, true, false);
        sleep_ms(2000);
        state = STATE_RED;
        break;
}
```
