# Relay
Jednoduchá ukázka ovládání relé.

## Zapojení
Relé vyžaduje napájení desky USB-C kabelem. Ke svorkám relé je možno připojit spínaný obvod až 30 V. Pro ukázku není spínaný obvod vyžadován, relé má svou informační diodu, která svítí, když je relé sepnuto.

Spínaný obvod je možno zapojit jako Normally Open (NO) nebo Normally Closed (NC).
- NO - při sepnutém relé je spínaný obvod sepnutý
- NC - při sepnutém relé je spínaný obvod rozpojený

## Postup
Naimportujte ukázku relay a prakticky odzkoušejte.

Relé se sepne přivedením log. 1 na výstup daného pinu (pro desku Explorer Board pin č. 28) pomocí funkce  ```gpio_put(pin, 1)```.

Přivedením log. 0 se relé rozepne.
