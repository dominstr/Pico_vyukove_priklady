# Weather Station - Metostanice
Tato ukázka kombinuje různé prvky použité v předchozích ukázkách, jako je práce s displejem, obsluha tlačítky, senzor DHT11 a získání dat o počasí z OpenWeather API.

Výsledkem je jednoduchá meteostanice se třemi přednastavenými městy, o kterých získává data aktuálního počasí z OpenWeather API a mezi nimiž je možno přepínat tlačítky `BUTTON_LEFT` a `BUTTON_RIGHT`.

## Doporučený postup
Naimportujte tuto ukázku.

Zaregistrujte se na stránce OpenWeather a vygenerujte si nový API klíč. Upravte soubor keys.h podle vaší wifi a API klíče:
```
#define WIFI_SSID "wifi_ssid_here"
#define WIFI_PASSWORD "wifi_password_here"
#define API_KEY "your_api_key_here"
```

Prakticky odzkoušejte.

![Praha](/readme_images/weather_station_zlin.jpeg)
![Praha](/readme_images/weather_station_uh.jpeg)
![Praha](/readme_images/weather_station_praha.jpeg)

## Kombinování C a C++
V této ukázce se používají původní soubory pro komunikaci s Wi-Fi od Raspberry Pi a které jsou napsány v jazyce C. Současně se ale používá knihovna pro TFT displej, která vyžaduje jazyk C++. Aby bylo možno kombinovat tyto dva jazyky v této ukázce, bylo potřeba připsat do souboru `example_http_client_util.h` následující řádky:
```
#ifdef __cplusplus
extern "C" {
#endif
```

A na konec tohoto souboru ještě:
```
#ifdef __cplusplus
}
#endif
```