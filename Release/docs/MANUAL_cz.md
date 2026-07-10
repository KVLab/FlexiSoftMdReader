# FlexiSoft Runtime — servisní manuál

**TEST LOKALIZACE: MANUAL_cz.md**

## 1. Přehled

FlexiSoft Runtime je servisní aplikace pro sledování vybraných stavů bezpečnostních kontrolérů SICK Flexi Soft přes RK512 komunikaci.

Aplikace je určená pro stroje, kde bezpečnostní kontrolér po otevření a zavření ochranného krytu může nahlásit chybu. Typicky jde o nesoučasné sepnutí dvoukanálového magnetického bezpečnostního spínače.

FlexiSoft Runtime detekuje nakonfigurované chyby vstupů, informuje obsluhu a umožňuje poslat řízený resetovací command. Pokud je fyzický bezpečnostní stav správný, reset může chybu odstranit. Pokud chyba stále fyzicky trvá, zůstane aktivní a obsluha je znovu upozorněna.

FlexiSoft Runtime není bezpečnostní kontrolér a nenahrazuje žádnou bezpečnostní funkci stroje.

## 2. Soubory aplikace

Doporučená složka:

```text
C:\FlexiSoftRuntime\
```

Typická struktura po přesunu dokumentace:

```text
FlexiSoftRuntime.exe
config.json
languages.json
version.json
runtime_state.json
flexi_runtime.log
docs\README.md
docs\README_cz.md
docs\MANUAL.md
docs\MANUAL_cz.md
fonts\...
```

## 3. Tray ikona

Aplikace běží v oznamovací oblasti Windows.

| Barva | Význam |
| --- | --- |
| Zelená | Komunikace OK, žádná aktivní nakonfigurovaná chyba. |
| Šedá | Komunikace není aktivní. |
| Žlutá / blikání | Otevírání transportu nebo reconnect. |
| Červená | Alespoň jeden nakonfigurovaný kanál je v chybě. |

## 4. Tray menu

Tray menu obsahuje stav kanálů a základní ovládání.

Příklad:

```text
CH1: OK / ON
CH2: ERROR / OFF
CH3: disabled
CH4: disabled
```

Položky `Send CHx command` zařadí resetovací command pro vybraný kanál. Command se neposílá přímo z UI vlákna, ale zpracuje ho runtime komunikační vlákno.

`Reload config` znovu načte konfiguraci a restartuje komunikaci.

`Reconnect` zastaví aktuální transport a znovu spustí komunikaci.

`Language` přepíná aktivní jazyk UI.

`About` otevře dialog s verzí, odkazy na README/MANUAL/LOG a informace o autorovi.

## 5. Alerty

### 5.1 Active Input Error Alert

Zobrazí se, když jeden nebo více nakonfigurovaných kanálů hlásí chybu.

| Tlačítko | Akce |
| --- | --- |
| YES | Pošle jeden společný command pro všechny uvedené chyby. |
| NO | Zavře alert bez commandu. |

### 5.2 Repeat Fault Alert

Zobrazí se, když se chyba vstupu opakovaně vrací po commandu.

Typické příčiny:

* kryt není správně zavřený,
* chybí jeden kanál dvoukanálového snímače,
* snímač je špatně seřízený,
* problém v kabeláži,
* chyba je reálná a nesmí se obcházet.

### 5.3 Command Failed Alert

Zobrazí se, když command neproběhl správně.

Command je úspěšný pouze tehdy, když:

1. command ON telegram je přijat,
2. uplyne pulse time,
3. command OFF telegram je přijat.

## 6. Konfigurace

Hlavní konfigurace je zatím:

```text
config.json
```

Později se plánuje přesun JSON souborů do `conf/`.

## 7. transport

```json
"transport": "network"
```

| Hodnota | Význam |
| --- | --- |
| network | Použít sekci network. |
| serial | Použít sekci serial. |

## 8. serial

Sériová komunikace používá COM port, baudrate, paritu, stop bity a timeout. Otestovaná hodnota baudrate je `115200`.

## 9. network

Síťový režim `tcp_client` používá IP adresu a port TCP-to-RS232 převodníku.

Typický port:

```text
4001
```

## 10. rk512

Sekce `rk512` definuje lokální device id, očekávané reply id a token telegram.

## 11. poll

Sekce `poll` určuje periodu čtení, čtený blok a počet bajtů.

## 12. inputs

Každý input popisuje jeden logický bezpečnostní vstup.

Důležité položky:

* `enabled`
* `name`
* `status_byte`
* `on_bit`
* `ok_bit`
* `alert_text_xx`

Pokud je kanál povolený a `ok_bit` je false, kanál je brán jako chyba.

## 13. output

`output` popisuje command bit, který se má nastavit při resetovacím pulsu.

## 14. repeat_fault

`repeat_fault` určuje počet opakování, časové okno a text opakované chyby.

## 15. ui

Sekce `ui` obsahuje text tooltipu tray ikony.

## 16. logging

Runtime log je vždy anglicky bez ohledu na jazyk UI.

## 17. debug

`force_command_fail` slouží jen pro testování Command Failed Alertu.

## 18. Lokalizace

UI texty se načítají z `languages.json`.

Strojní texty jsou v `config.json` přes suffixy:

```text
alert_text_en
alert_text_cz
text_en
text_cz
```

Dokumentace se otevírá ze složky `docs/`:

```text
docs\README_<lang>.md -> docs\README.md
docs\MANUAL_<lang>.md -> docs\MANUAL.md
```

Ukrajinština používá suffix `uk`. `ua` se nepodporuje.

## 19. version.json

`version.json` obsahuje název produktu, verzi, popis, autora, email a datum buildu.

## 20. Troubleshooting

### Tray ikona zůstává šedá

Zkontroluj COM port, TCP převodník, IP adresu, napájení Flexi Soft a kabeláž.

### Tray ikona bliká šedě/žlutě

Aplikace se pokouší znovu připojit.

### Tray ikona je červená

Alespoň jeden nakonfigurovaný kanál hlásí chybu.

### Command failed

Zkontroluj komunikaci, použij retry, reconnect nebo restart TCP převodníku.

### Alert se vrací

Fyzická chyba nejspíš stále trvá. Zkontroluj kryt, snímače, oba bezpečnostní kanály, kabeláž a diagnostiku Flexi Soft.

## 21. Bezpečnostní poznámky

FlexiSoft Runtime není bezpečnostní zařízení.

Nesmí se používat k obcházení bezpečnostních funkcí.

Aplikace pouze posílá nakonfigurované resetovací commandy. Neověřuje, že je stroj bezpečný k provozu.

## 22. Podpora

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
