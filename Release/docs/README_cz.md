# FlexiSoft Runtime — rychlý start

**TEST LOKALIZACE: README_cz.md**

FlexiSoft Runtime je samostatná servisní aplikace pro sledování bezpečnostních kontrolérů SICK Flexi Soft, automatizované zotavení vybraných chybových stavů a upozornění obsluhy na opakované chyby bezpečnostních vstupů.

## Účel

Aplikace je určená pro stroje, kde Flexi Soft hlásí chyby bezpečnostních vstupů způsobené nesoučasným sepnutím dvoukanálových bezpečnostních snímačů.

Typický průběh:

1. Ochranný kryt se otevře a zavře.
2. Oba bezpečnostní kanály nepřepnou přesně současně.
3. Flexi Soft zahlásí chybu souslednosti vstupu.
4. FlexiSoft Runtime chybu detekuje.
5. Obsluha může potvrdit řízený resetovací command.
6. Pokud šlo jen o časový rozdíl, vstup se vrátí do OK.
7. Pokud fyzická chyba stále trvá, chyba zůstane aktivní a obsluha je upozorněna znovu.

Aplikace nenahrazuje bezpečnostní funkci Flexi Soft. Pouze sleduje stav, informuje obsluhu a posílá nakonfigurované resetovací příkazy.

## Hlavní funkce

* RK512 komunikace se SICK Flexi Soft.
* Sériový transport.
* TCP client transport přes TCP-to-RS232 převodník.
* Sledování až čtyř nakonfigurovaných kanálů.
* Jeden společný alert pro více aktivních chyb.
* Řízený resetovací command pro zasažené kanály.
* Detekce opakované chyby.
* Alert při selhání commandu s možností opakování.
* Stavová ikona v tray.
* Runtime logování.
* Konfigurace přes JSON.
* Kompatibilita Windows XP SP3 32-bit až Windows 11.

## Instalace

Doporučená složka:

```text
C:\FlexiSoftRuntime\
```

Doporučený obsah:

```text
FlexiSoftRuntime.exe
config.json
languages.json
version.json
runtime_state.json
flexi_runtime.log
docs\README.md
docs\README_cz.md
docs\README_uk.md
docs\MANUAL.md
docs\MANUAL_cz.md
docs\MANUAL_uk.md
fonts\...
```

## První spuštění

1. Zkopíruj `FlexiSoftRuntime.exe` a konfigurační soubory do pracovní složky.
2. Zkontroluj komunikaci v `config.json`.
3. Spusť `FlexiSoftRuntime.exe`.
4. V oznamovací oblasti Windows se objeví tray ikona.
5. Počkej, až ikona přejde do zeleného stavu.

## Barvy tray ikony

| Barva | Význam |
| --- | --- |
| Zelená | Komunikace OK a žádný nakonfigurovaný kanál není v chybě. |
| Šedá | Komunikace není aktivní nebo je transport odpojený. |
| Žlutá / blikání | Probíhá připojování nebo reconnect. |
| Červená | Komunikace OK, ale alespoň jeden kanál je v chybě. |

## Alerty

* **Input Error Alert** — zobrazí se při aktivní chybě vstupu.
* **Repeat Fault Alert** — zobrazí se, když se stejná chyba opakovaně vrací po commandu.
* **Command Failed Alert** — zobrazí se, když resetovací command neproběhl korektně.

## Dokumentace

Detailní servisní dokumentace je v souboru:

```text
docs\MANUAL_cz.md
```

Pokud lokalizovaná dokumentace pro aktivní jazyk neexistuje, aplikace otevře fallback:

```text
docs\README.md
docs\MANUAL.md
```

## Autor

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
