#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "displaylib_16/st7735.hpp"
#include "example_http_client_util.h"
#include "hardware/spi.h"
#include "keys.h"  // WIFI_SSID, WIFI_PASSWORD, API_KEY
#include "lwip/altcp_tls.h"
#include "pico/async_context.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "xplr.h"

#define HOST "api.openweathermap.org"
#define NUM_CITIES 3

const char *CITIES[NUM_CITIES] = {"Zlin", "Uherske%20Hradiste", "Praha"}; // Pro API. Misto mezery pouzit zastupny znak %20
const char *DISPLAY_CITIES[NUM_CITIES] = {"Zlin", "Uherske Hradiste", "Praha"}; // Pro zobrazeni na displeji
int current_city_index = 0;

const uint DHT_PIN = DHT11_PIN;
const uint MAX_TIMINGS = 85;
char url_request[256];
ST7735_TFT myTFT;

typedef struct {
    int temp;
    int humidity;
} indoor_data_t;

typedef struct {
    char city[64];
    float temp;
    int humidity;
    float pressure;
    float wind_speed;
    int wind_deg;
    absolute_time_t last_updated;
} weather_data_t;

weather_data_t weather_data[NUM_CITIES];
indoor_data_t indoor_data = {-1, -1};

void read_from_dht(void);
bool wifi_connect(void);
void get_weather_data(int city_index);
void init(void);
void display_init(void);
void display_print(weather_data_t *weather_data);
void print_to_console(weather_data_t *weather_data);
int parse_weather_json(const char *json_response, char *city, float *temp, int *humidity, float *pressure, float *wind_speed, int *wind_deg);
err_t http_client_receive_json_fn(void *arg, __unused struct altcp_pcb *conn, struct pbuf *p, err_t err);
const char *wind_direction_to_string(int degrees);
void button_callback(uint gpio, uint32_t events);

int main() {
    init();

    if (!wifi_connect()) {
        return 1;
    }

    // Ziskani dat pro vsechna mesta
    for (int i = 0; i < NUM_CITIES; ++i) {
        strcpy(weather_data[i].city, CITIES[i]);
        // Inicializace dat na nemplatne hodnoty
        weather_data[i].temp = -100;
        weather_data[i].humidity = -1;
        weather_data[i].pressure = -1;
        weather_data[i].wind_speed = -1;
        weather_data[i].wind_deg = -1;
        weather_data[i].last_updated = nil_time;
    }

    while (true) {
        for (int i = 0; i < NUM_CITIES; ++i) {
            get_weather_data(i); // Ziskani dat pro vsechna mesta
        }
        sleep_ms(50); // Kratka pauza pro zpracovani dat
        read_from_dht(); // Ziskani dat z DHT senzoru

        print_to_console(&weather_data[current_city_index]); // Tisk dat do konzole
        display_print(&weather_data[current_city_index]); // Zobrazeni dat na displeji
        sleep_ms(60000); // Aktualizace kazdou minutu
    }

    cyw43_arch_deinit();
    return 0;
}

// Inicializacni funkce
void init(void) {
    stdio_init_all();
    gpio_init(DHT_PIN);
    gpio_init(BUTTON_RIGHT_PIN);
    gpio_set_dir(BUTTON_RIGHT_PIN, GPIO_IN);
    gpio_init(BUTTON_LEFT_PIN);
    gpio_set_dir(BUTTON_LEFT_PIN, GPIO_IN);

    // Nastaveni preruseni pro tlacitka
    gpio_set_irq_enabled_with_callback(BUTTON_RIGHT_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_LEFT_PIN, GPIO_IRQ_EDGE_RISE, true, &button_callback);

    display_init();
}

// Inicializace TFT displeje
void display_init(void) {
    gpio_init(TFT_BL_PIN);
    gpio_set_dir(TFT_BL_PIN, GPIO_OUT);
    gpio_put(TFT_BL_PIN, 1);

    bool bhardwareSPI = true;

    if (bhardwareSPI == true) {
        uint32_t TFT_SCLK_FREQ = 8000;
        myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
    } else {
        uint16_t SWSPICommDelay = 0;
        myTFT.TFTInitSPIType(SWSPICommDelay);
    }

    int8_t SDIN_TFT = SPI_MOSI_PIN;
    int8_t SCLK_TFT = SPI_SCK_PIN;
    int8_t DC_TFT = TFT_DC_PIN;
    int8_t CS_TFT = TFT_CS_PIN;
    int8_t RST_TFT = TFT_RES_PIN;
    myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);

    uint8_t OFFSET_COL = 0;
    uint8_t OFFSET_ROW = 0;
    uint16_t TFT_WIDTH = 128;
    uint16_t TFT_HEIGHT = 160;
    myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);

    myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red);

    myTFT.fillScreen(myTFT.C_BLACK);
    myTFT.setFont(font_default);
    myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
    myTFT.setCursor(20, 60);
    myTFT.print("Loading...");
}

// Zobrazeni dat na displeji
void display_print(weather_data_t *weather_data) {
    char temp_str[20];
    char humidity_str[20];
    char pressure_str[20];
    char wind_speed_str[20];
    char wind_dir_str[20];
    char indoor_temp_str[20];
    char indoor_humidity_str[20];

    myTFT.fillScreen(myTFT.C_BLACK);
    myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
    myTFT.setCursor(0, 0);
    myTFT.print(DISPLAY_CITIES[current_city_index]);

    snprintf(temp_str, sizeof(temp_str), "Temp: %.0f C", weather_data->temp);
    snprintf(humidity_str, sizeof(humidity_str), "Hum: %d %%", weather_data->humidity);
    snprintf(pressure_str, sizeof(pressure_str), "Press: %.0f hPa", weather_data->pressure);
    snprintf(wind_speed_str, sizeof(wind_speed_str), "Speed: %.0f m/s", weather_data->wind_speed);
    const char *wind_direction = wind_direction_to_string(weather_data->wind_deg);
    snprintf(wind_dir_str, sizeof(wind_dir_str), "Direction: %s", wind_direction);

    myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
    myTFT.setCursor(0, 30);
    myTFT.print(temp_str);

    myTFT.setCursor(0, 40);
    myTFT.print(humidity_str);

    myTFT.setCursor(0, 50);
    myTFT.print(pressure_str);

    myTFT.setCursor(0, 70);
    myTFT.print("Wind:");

    myTFT.setCursor(0, 80);
    myTFT.print(wind_speed_str);

    myTFT.setCursor(0, 90);
    myTFT.print(wind_dir_str);

    myTFT.setTextColor(myTFT.C_ORANGE, myTFT.C_BLACK);
    myTFT.setCursor(0, 120);
    myTFT.print("Indoor:");

    if (indoor_data.humidity != -1) {
        snprintf(indoor_temp_str, sizeof(temp_str), "Temp: %d C", indoor_data.temp);
        snprintf(indoor_humidity_str, sizeof(humidity_str), "Hum: %d %%", indoor_data.humidity);

        myTFT.setCursor(0, 130);
        myTFT.print(indoor_temp_str);

        myTFT.setCursor(0, 140);
        myTFT.print(indoor_humidity_str);
    } else {
        myTFT.setCursor(0, 130);
        myTFT.print("Temp: N/A");

        myTFT.setCursor(0, 140);
        myTFT.print("Hum: N/A");
    }
}

// Vypis dat do konzole
void print_to_console(weather_data_t *weather_data) {
    printf("Mesto: %s\n", weather_data->city);
    printf("Teplota: %.2f °C\n", weather_data->temp);
    printf("Vlhkost vzduchu: %d%%\n", weather_data->humidity);
    printf("Tlak: %.2f hPa\n", weather_data->pressure);
    printf("Rychlost vetru: %.2f m/s\n", weather_data->wind_speed);
    printf("Smer vetru: %d° (%s)\n", weather_data->wind_deg, wind_direction_to_string(weather_data->wind_deg));
    
    if (indoor_data.humidity != -1) {
        printf("Vnitrni teplota: %d °C\n", indoor_data.temp);
        printf("Vnitrni vlhkost: %d%%\n", indoor_data.humidity);
    } else {
        printf("Vnitrni vlhkost: N/A\n");
    }

    printf("\n");
}

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

// Pripojeni k WiFi
bool wifi_connect() {
    if (cyw43_arch_init()) {
        printf("Selhala inicializace WiFi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Pripojeni k WiFi selhalo\n");
        return false;
    }
    return true;
}

// Ziskani aktualniho pocasi z API
void get_weather_data(int city_index) {
    absolute_time_t now = get_absolute_time();

    // Pokud jsou data starsi nez 60 sekund, aktualizujeme
    if (is_nil_time(weather_data[city_index].last_updated) || absolute_time_diff_us(now, weather_data[city_index].last_updated) > 60 * 1000 * 1000) {
        snprintf(url_request, sizeof(url_request),
                 "/data/2.5/weather?q=%s&appid=%s&units=metric",
                 CITIES[city_index], API_KEY);

        EXAMPLE_HTTP_REQUEST_T req = {0};
        req.hostname = HOST;
        req.url = url_request;
        req.headers_fn = NULL;
        req.recv_fn = http_client_receive_json_fn;

        req.callback_arg = &weather_data[city_index];

        int result = http_client_request_sync(cyw43_arch_async_context(), &req);

        if (result != 0) {
            printf("Pozadavek HTTP se nezdaril (%d), znovu pripojuji WiFi...\n", result);
        } else {
            weather_data[city_index].last_updated = now;
        }
    } else {
        printf("Data z cache pro mesto: %s\n", CITIES[city_index]);
    }
}

// Cteni dat z DHT senzoru
void read_from_dht() {
    int data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;
    int attempts = 0;
    const int max_attempts = 5; // Maximum pokusu pro nacteni dat

    while (attempts < max_attempts && (indoor_data.humidity == -1 || indoor_data.temp == -1)) {
        attempts++;
        j = 0;
        memset(data, 0, sizeof(data)); // Reset dat pro kazdy pokus

        gpio_set_dir(DHT_PIN, GPIO_OUT);
        gpio_put(DHT_PIN, 0);
        sleep_ms(20);
        gpio_set_dir(DHT_PIN, GPIO_IN);

        for (uint i = 0; i < MAX_TIMINGS; i++) {
            uint count = 0;
            while (gpio_get(DHT_PIN) == last) {
                count++;
                sleep_us(2);
                if (count == 255) break;
            }
            last = gpio_get(DHT_PIN);
            if (count == 255) break;

            if ((i >= 4) && (i % 2 == 0)) {
                data[j / 8] <<= 1;
                if (count > 16) data[j / 8] |= 1;
                j++;
            }
        }

        if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
            float humidity = (float)(data[0]);
            float temp_celsius = (float)(data[2]);

            if (humidity < 0 || humidity > 100) {
                indoor_data.humidity = -1;
                indoor_data.temp = -1;
                printf("Neplatna data z DHT, opakuji...\n");
            } else {
                indoor_data.humidity = (int)humidity;
                indoor_data.temp = (int)temp_celsius;
                printf("DHT data uspesne nactena.\n");
            }
        } else {
            printf("Chybna data, opakuji...\n");
            indoor_data.temp = -1;
            indoor_data.humidity = -1;
        }
        sleep_ms(50); // Kratka pauza pred dalsim pokusem
    }

    if (indoor_data.humidity == -1 || indoor_data.temp == -1) {
        printf("Nepovedlo se nacist data z DHT.\n");
    }
}

// Prevod smeru vetru ze stupnu na text
const char *wind_direction_to_string(int degrees) {
    if (degrees < 0 || degrees > 360) {
        return "N/A";
    }

    if (degrees <= 22.5 || degrees > 337.5) return "N";
    if (degrees <= 67.5) return "NE";
    if (degrees <= 112.5) return "E";
    if (degrees <= 157.5) return "SE";
    if (degrees <= 202.5) return "S";
    if (degrees <= 247.5) return "SW";
    if (degrees <= 292.5) return "W";
    return "NW";
}

// Prepinani mezi mesty pomoci tlacitek
void button_callback(uint gpio, uint32_t events) {
    static absolute_time_t last_pressed_time = nil_time; // Cas posledniho stisku tlacitka
    absolute_time_t now = get_absolute_time();
    const uint debounce_delay_ms = 200;

    if ((gpio == BUTTON_RIGHT_PIN || gpio == BUTTON_LEFT_PIN) && (events & GPIO_IRQ_EDGE_RISE)) {
        // Debounce pro obe tlacitka
        if (is_nil_time(last_pressed_time) || absolute_time_diff_us(last_pressed_time, now) > debounce_delay_ms * 1000) {
            if (gpio == BUTTON_RIGHT_PIN) {
                current_city_index = (current_city_index + 1) % NUM_CITIES;
            } else {
                current_city_index = (current_city_index - 1 + NUM_CITIES) % NUM_CITIES; // Zajisti, ze index bude vzdy kladny
            }
            printf("Prepnuti mesta: %s\n", DISPLAY_CITIES[current_city_index]);
            weather_data[current_city_index].last_updated = nil_time; // Nulovani last_updated pro nove mesto
            display_print(&weather_data[current_city_index]); // Zobrazeni dat z cache
            last_pressed_time = now; // Aktualizujeme cas posledniho stisku tlacitka
        }
    }
}