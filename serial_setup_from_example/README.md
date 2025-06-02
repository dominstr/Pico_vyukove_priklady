# Výstup na sériovou linku
Tato ukázka má za cíl seznámit uživatele s nastavením sériové linky v ukázkovém příkladu přímo od Raspberry Pi a také nastavení pro vlastní projekt.
## Nastavení ukázkového projektu
Klikneme na ikonku rozšíření **Raspberry Pi Pico Project** a vybereme možnost **New Project From Example**.

Z rozbalovací nabídky vybereme příklad **hello_usb** pokud chceme pracovat se sériovou linkou přímo přes USB kabel. Vybereme správnou variantu desky Pico. Zvolíme složku, kam se má náš projekt uložit a vytvoříme projekt kliknutím na **Create**.

Soubor `CMakeLists.txt` již obsahuje potřebné řádky:
```
# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(nazev_projektu 0)
pico_enable_stdio_usb(nazev_projektu 1)
```

Pokud programujeme Pico přes Debug Probe, můžeme zvolit ukázkový příklad **hello_serial**, který tyto řádky nevyžaduje, případně můžeme v předchozím příkladě zaměnit UART `1` a USB `0`. Následně je třeba nakonfigurovat CMake pomocí rozšíření Raspberry Pi Pico pro VS Code, kde vybereme možnost *Configure CMake*.

## Nastavení při založení nového projektu
Při zakládání nového projektu, lze zvolit v sekci *Stdio support*, jakým způsobem chceme vypisovat na sériovou linku.

V případě, že je programované Pico připojeno přímo přes USB kabel, vybereme možnost *Console over USB*.

Pokud Pico ladíme přes Debug Probe, zvolíme možnost *Console over UART*.

![Stdio support](/readme_images/new_project.jpg)

Pro změnu typu komunikace v již založeném projektu, je třeba v souboru `CMakeLists.txt` upravit příslušné řádky. 

## Vypisování textu do konzole
Pokud jsme založili nový projekt, již máme vygenerován ukázkový program *Hello World!*

Aby výpis fungoval správně, musíme na začátku funkce main zavolat inicializační funkci `stdio_init_all()`.

Po inicializaci budeme v nekonečné smyčce volat funkci `printf("Hello, world!\n")`, která vypíše textový řetězec do konzole.

Výpis můžeme zpomalit zavoláním funkce `sleep_ms(1000)` s parametrem počtu ms.

Po nahrání programu do Pica, zvolíme záložku *SERIAL MONITOR* v dolní liště VS Code (Pokud není panel vidět, stiskneme **CTRL+J** a panel se objeví). Vybereme správný Port. Obvykle je to vyšší číslo (na obrázku COM12). Pokud se v rozbalovací nabídce zobrazuje jen port COM1 i po stisknutí Refresh tlačítka vedle rozbalovací nabídky, je potřeba překontrolovat zapojení, příp. *CMakeLists.txt* (popsáno výše).

Po výběru portu začneme monitorovat sériovou linku pomocí modrého tlačítka **Start Monitoring**. Na konzoli by se měl každých 1000 ms vypisovat nápis *Hello, world!* Napravo od modrého tlačítka **Start Monitoring** je tlačítko pro vymazání výstupu.
![Console output](/readme_images/console_output.jpg)
