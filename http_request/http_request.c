#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "example_http_client_util.h"
#include "lwip/altcp_tls.h"
#include "pico/async_context.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "keys.h" // WIFI_SSID, WIFI_PASSWORD, API_KEY

#define HOST "api.openweathermap.org"
#define CITY "Zlin" // misto mezery pouzit zastupny znak %20 (Uherske%20Hradiste)

char url_request[256];

// struktura pro ukladani dat o pocasi
typedef struct {
    char city[64];
    float temp;
    int humidity;
    float pressure;
    float wind_speed;
    int wind_deg;
} weather_data_t;

// Funkce pro parsovani dat z JSON
int parse_weather_json(const char *json_response, char *city, float *temp, int *humidity, float *pressure, float *wind_speed, int *wind_deg) {
    cJSON *json = cJSON_Parse(json_response);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Chyba JSON: %s\n", error_ptr);
        }
        return 0;
    }

    const cJSON *city_item = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (cJSON_IsString(city_item) && (city_item->valuestring != NULL)) {
        strncpy(city, city_item->valuestring, 63);
        city[63] = '\0';
    }

    const cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
    if (main != NULL) {
        const cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(main, "temp");
        const cJSON *humidity_item = cJSON_GetObjectItemCaseSensitive(main, "humidity");
        const cJSON *pressure_item = cJSON_GetObjectItemCaseSensitive(main, "pressure");

        if (cJSON_IsNumber(temp_item)) *temp = temp_item->valuedouble;
        if (cJSON_IsNumber(humidity_item)) *humidity = humidity_item->valueint;
        if (cJSON_IsNumber(pressure_item)) *pressure = pressure_item->valuedouble;
    }

    const cJSON *wind = cJSON_GetObjectItemCaseSensitive(json, "wind");
    if (wind) {
        const cJSON *speed_item = cJSON_GetObjectItemCaseSensitive(wind, "speed");
        const cJSON *deg_item = cJSON_GetObjectItemCaseSensitive(wind, "deg");

        if (cJSON_IsNumber(speed_item)) *wind_speed = speed_item->valuedouble;
        if (cJSON_IsNumber(deg_item)) *wind_deg = deg_item->valueint;
    }

    cJSON_Delete(json);
    return 1;
}

// Funkce pro ziskani JSON
err_t http_client_receive_json_fn(void *arg, __unused struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    if (err != ERR_OK) {
        printf("\nContent error: %d\n", err);
        return err;
    }

    weather_data_t *weather_data = (weather_data_t *)arg;
    memset(weather_data, 0, sizeof(weather_data_t));

    u16_t offset = 0;
    char response_buffer[2048];
    int idx = 0;

    while (offset < p->tot_len && idx < sizeof(response_buffer) - 1) {
        response_buffer[idx++] = (char)pbuf_get_at(p, offset++);
    }
    response_buffer[idx] = '\0';
    // printf("Received JSON: %s\n", response_buffer);

    int parse_result = parse_weather_json(response_buffer, weather_data->city, &weather_data->temp,
                                          &weather_data->humidity, &weather_data->pressure,
                                          &weather_data->wind_speed, &weather_data->wind_deg);
    if (!parse_result) {
        printf("Chyba pri parsovani JSON\n");
    }

    return ERR_OK;
}

int main() {
    stdio_init_all();

    // Inicializace Wi-Fi modulu
    if (cyw43_arch_init()) {
        printf("Selhala inicializace WiFi\n");
        return 1;
    }

    // Prepnuti do rezimu station (klient)
    cyw43_arch_enable_sta_mode();

    // Pripojeni k Wi-Fi siti
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Pripojeni k WiFi selhalo\n");
        return 1;
    }

    // Vytvoreni URL pro HTTP pozadavek
    snprintf(url_request, sizeof(url_request),
             "/data/2.5/weather?q=%s&appid=%s&units=metric",
             CITY, API_KEY);

    sleep_ms(2000);

    // Nastaveni hostname a URL pro HTTP pozadavek
    EXAMPLE_HTTP_REQUEST_T req = {0};
    req.hostname = HOST;
    req.url = url_request;
    req.headers_fn = NULL;
    req.recv_fn = http_client_receive_json_fn;

    weather_data_t weather_data;
    req.callback_arg = &weather_data;

    // Odeslani HTTP pozadavku a synchronni cekani na odpoved
    int result = http_client_request_sync(cyw43_arch_async_context(), &req);

    // Pokud doslo k chybe pri pozadavku, znovu se pripoji k Wi-Fi
    if (result != 0) {
        printf("Pozadavek HTTP se nezdaril (%d), znovu pripojuji WiFi...\n", result);
        cyw43_arch_deinit();
        return 1;
    }

    // Vypis vysledku ziskanych z API
    printf("Mesto: %s\n", weather_data.city);
    printf("Teplota: %.2f °C\n", weather_data.temp);
    printf("Vlhkost vzduchu: %d%%\n", weather_data.humidity);
    printf("Tlak: %.2f hPa\n", weather_data.pressure);
    printf("Rychlost vetru: %.2f m/s\n", weather_data.wind_speed);
    printf("Smer vetru: %d°\n", weather_data.wind_deg);
    printf("\n");

    // Deaktivace Wi-Fi modulu
    cyw43_arch_deinit();
    return 0;
}