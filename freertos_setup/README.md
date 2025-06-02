# FreeRTOS setup
Nastavení FreeRTOS na Raspberry Pi Pico a ukázka 2 tasků pro obsluhu LED diody a čtení tlačítka.

## Postup nastavení FreeRTOS v novém projektu
Založte nový C/C++ projekt, vyberte správnou verzi desky a zvolte umístění projektu.

Tento repozitář již obsahuje potřebné soubory pro FreeRTOS ve složce `shared/FreeRTOS-Kernel`. Tyto soubory je možno naklonovat z oficiálního repozitáře: [https://github.com/FreeRTOS/FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel).

Ve složce `shared/freeRTOS` se potom nachází další konfigurační soubory, aby je bylo možno snadno použít ve více projektech.

### Úprava CmakeLists.txt
Za řádek `include(pico_sdk_import.cmake)` přidejte:
```
set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/../shared/FreeRTOS-Kernel)
include(${CMAKE_SOURCE_DIR}/../shared/freertos/FreeRTOS_Kernel_import.cmake)
```
Upravte tuto část:
```
target_include_directories(freertos_setup PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
        ${CMAKE_CURRENT_LIST_DIR}/../shared/freeRTOS
        ${FREERTOS_KERNEL_PATH}/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/include
)

target_link_libraries(freertos_setup 
        FreeRTOS-Kernel-Heap4
        )
```
V případě umístění souborů FreeRTOS mimo složku shared v tomto repozitáři je třeba upravit skutečnou cestu.

Pro Pico W přidejte ještě:
```
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(freertos_setup pico_cyw43_arch_none)
endif()
```

## FreeRTOS
### Tasky
- **`led_task`** - Tento task ovládá LED diodu. Čeká na zprávy ve frontě `button_queue` a podle přijatého stavu rozsvítí nebo zhadne LED.
- **`button_task`** - Tento task slouží ke čtení tlačítka. Pokud zaregistruje změnu stavu tlačítka, odešle jej do fronty `button_queue`

### Funkce FreeRTOS
-   **`xTaskCreate(TaskFunction_t pvTaskCode, const char *pcName, configSTACK_DEPTH_TYPE usStackDepth, void *pvParameters, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask)`**
    -   Vytvoří nový task.
        -   `pvTaskCode`: Ukazatel na funkci tasku (např. `led_task`).
        -   `pcName`: Název tasku.
        -   `usStackDepth`: Velikost zásobníku tasku. Pro jednoduché tasky použijte `configMINIMAL_STACK_SIZE`.
        -   `pvParameters`: Ukazatel na parametry tasku (může být `NULL`).
        -   `uxPriority`: Priorita tasku (vyšší číslo = vyšší priorita).
        -   `pxCreatedTask`: Ukazatel na proměnnou typu `TaskHandle_t`, kam se uloží handle vytvořeného tasku (může být `NULL`).
-   **`vTaskDelay(const TickType_t xTicksToDelay)`**
    -   Pozastaví task na určitou dobu.
        *   `xTicksToDelay`: Doba pozastavení v ticích. Pro převod ms na ticky se používá funkce `pdMS_TO_TICKS()`.
-   **`xQueueCreate(UBaseType_t uxQueueLength, UBaseType_t uxItemSize)`**
    -   Vytvoří novou frontu.
        -   `uxQueueLength`: Maximální počet položek, které může fronta obsahovat.
        -   `uxItemSize`: Velikost jedné položky ve frontě (v bytech).
-   **`xQueueSend(QueueHandle_t xQueue, const void *pvItemToQueue, TickType_t xTicksToWait)`**
    -   Odešle data do fronty.
        -   `xQueue`: Handle fronty, do které se odesílají data.
        -   `pvItemToQueue`: Ukazatel na data, která se odesílají.
        -   `xTicksToWait`: Maximální doba (v ticích), po kterou se má čekat na uvolnění místa ve frontě. Použijte `0` pro neblokující odeslání.
-   **`xQueueReceive(QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait)`**
    -   Přijme data z fronty.
        -   `xQueue`: Handle fronty, ze které se přijímají data.
        -   `pvBuffer`: Ukazatel na buffer, kam se uloží přijatá data.
        -   `xTicksToWait`: Maximální doba (v ticích), po kterou se má čekat na data ve frontě. Použijte `portMAX_DELAY` pro nekonečné čekání.

-   **`vTaskStartScheduler()`**
    -   Spustí FreeRTOS plánovač.

### Princip funkce FreeRTOS v ukázce
1. Funkce `main` inicializuje FreeRTOS, vytvoří frontu `button_queue`, tasky `led_task` a `button_task`.
2. `button_task` čte stav tlačítka odesílá jej do fronty `button_queue`.
3. `led_task` čeká na data ve frontě `button_queue` a podle přijatého stavu ovládá LED diodu.
4. `vTaskStartScheduler()` spustí plánovač, který se stará o přepínání mezi tasky. Plánovač běží do nekonečna, proto se kód za `vTaskStartScheduler` nikdy nevykoná.