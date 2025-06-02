# Timer blink - blikání pomocí přerušení
Tento příklad demonstruje použití hardwarového časovače (timeru) a alarmu pro periodické blikání LED diody. Program využívá přerušní, které se spustí vždy po uplynutí nastaveného času.

## Doporučený postup
Naimportujte ukázku a odzkoušejte.

## Popis programu
### init()
Tato funkce slouží k inicializaci LED diody na pinu LED_RED (20).

### alarm_in_us(1000000 * 2)
Nastaví alarm na daný počet mikrosekund. Poté aktivuje přerušení v časovači. Přiřadí oblužnou funkci přerušení.

### alarm_irq(void)
Obsluha přerušení - spustí se, když uplyne nastavený čas. Vynuluje příznak přerušení, přepne stav LED, nastaví `alarm_fired` na `true`.

### while (!alarm_fired)
Díky této smyčce program čeká, dokud se nezmění příznak `alarm_fired` na `true`.