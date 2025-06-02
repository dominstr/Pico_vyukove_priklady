# HTTP request
Ukázkový příklad pro získání dat o počasí z OpenWeather API pomocí názvu města.

OpenWeather nabízí zdarma API pro aktuální počasí. Umožňuje až 60 volání API za minutu nebo 1 000 000 volání za měsíc. Kromě aktuálního počasí nabízí zdarma i 5 denní předpověď po 3 hodinách.

Tato ukázka používá vzorovou ukázku pro [http klienta](https://github.com/raspberrypi/pico-examples/tree/master/pico_w/wifi/http_client) od Raspberry Pi a [parser pro JSON](https://github.com/DaveGamble/cJSON).

## Postup
Naimportujte tuto ukázku.

Zaregistrujte se na stránce OpenWeather a vygenerujte si nový API klíč.
Upravte soubor keys.h podle vaší Wi-Fi a API klíče:
```
#define WIFI_SSID "wifi_ssid_here"
#define WIFI_PASSWORD "wifi_password_here"
#define API_KEY "your_api_key_here"
```

V hlavním souboru http_request.c můžete změnit město změnou: `#define CITY "Zlin"`. Mezery v názvu města nahraďte zástupným znakem `%20`.

## Popis funkcí
### cyw43_arch_init()
Zinicializuje Wi-Fi modul.

### cyw43_arch_enable_sta_mode()
Přepne Wi-Fi modul do režimu station, což znamená, že se zařízení připojí k Wi-Fi síti.

### cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)
Pokusí se připojit k Wi-Fi síti dle specifikovaných parametrů.

### snprintf(url_request, sizeof(url_request), "/data/2.5/weather?q=%s&appid=%s&units=metric", CITY, API_KEY)
Vytvoří řetězec URL se zadanými parametry.

### http_client_receive_json_fn(void *arg, __unused struct altcp_pcb *conn, struct pbuf *p, err_t err)
Slouží ke zpracování odpovědí HTTP klienta.

### http_client_request_sync(cyw43_arch_async_context(), &req)
Odeslání HTTP požadavku a synchronní čekání na odpověď.

### parse_weather_json(const char *json_response, char *city, float *temp, int *humidity, float *pressure, float *wind_speed, int *wind_deg)
Slouží k parsování JSON odpovědi, kterou server vrátí jako odpověď na HTTP požadavek.

### cyw43_arch_deinit()
Vypne Wi-Fi modul a ukončí připojení k síti.