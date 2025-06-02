# Blink - Traffic lights
Ukázka rozšiřuje ukázku Blink o další diody.

Deska Explorer Board obsahuje adresovatelné Neopixel diody, pro které nelze použít tento způsob programování.
Pro tuto ukázku je potřeba mít externí diody na nepájivém poli.

## Zapojení LED diod
V ukázce jsou LED diody jsou připojeny na následující GPIO:
- `LED_RED` - `20`
- `LED_GREEN` - `21`
- `LED_YELLOW` - `22`

Diody se rozsvítí přivedením výstupní log. 1 na příslušný GPIO.

## Zadání úlohy Traffic lights
Naprogramujte Semafor pomocí LED diod - červená, žlutá, zelená následovně:
1. Signál "**Stůj**" - červená svítí 5 s
2. Signál "**Připravit k jízdě**" - červená a současně žlutá svítí 1 s
3. Signál "**Volno**" - zelená svítí 5 s
4. Signál "**Pozor**" - žlutá svítí 1 s

## Doporučený postup
Vytvořte funkci `traffic_lights_init(void)`, která zinicializuje GPIO pro LED a nastaví je jako výstup.

Dále vytvořte funkci `set_traffic_lights(bool green_on, bool yellow_on, bool red_on)`, která přijme parametry typu `bool` pro všechny 3 diody najednou a nastaví každou LED pomocí `gpio_put()`.

## Možné rozšíření
Pomocí vestavěné diody signalizujte semafor na přechodu pro chodce:
1. Pouze když je signál "**Stůj**" na hlavním semaforu, tak na přechodu svítí signál "**Volno**" - svítí vestavěná dioda.
2. Pokud není signál "**Stůj**" na hlavním semaforu, tak na přechodu svítí signál "**Stůj**" - zhasnutá vestavěná dioda.

Prodlevu na průjezd aut neuvažujte.