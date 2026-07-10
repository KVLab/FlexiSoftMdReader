# FlexiSoft Runtime — Servicehandbuch

**LOKALISIERUNGSTEST: MANUAL_de.md**

## 1. Überblick

FlexiSoft Runtime ist eine Service-Anwendung zur Überwachung ausgewählter Zustände von SICK Flexi Soft Sicherheitssteuerungen über RK512-Kommunikation.

Die Anwendung ist für Maschinen gedacht, bei denen eine Sicherheitssteuerung nach dem Öffnen und Schließen einer Schutztür einen Fehler melden kann. Typisch ist ein nicht gleichzeitiges Schalten eines zweikanaligen magnetischen Sicherheitsschalters.

FlexiSoft Runtime erkennt konfigurierte Eingangsfehler, informiert den Bediener und ermöglicht das Senden eines kontrollierten Reset-Befehls. Wenn der physische Sicherheitszustand korrekt ist, kann der Reset den Fehler löschen. Wenn der Fehler weiterhin physisch vorhanden ist, bleibt er aktiv und der Bediener wird erneut gewarnt.

FlexiSoft Runtime ist keine Sicherheitssteuerung und ersetzt keine Sicherheitsfunktion der Maschine.

## 2. Anwendungsdateien

Empfohlener Ordner:

```text
C:\FlexiSoftRuntime\
```

Typische Struktur:

```text
FlexiSoftRuntime.exe
config.json
languages.json
version.json
runtime_state.json
flexi_runtime.log
docs\README.md
docs\README_de.md
docs\MANUAL.md
docs\MANUAL_de.md
fonts\...
```

## 3. Tray-Symbol

Die Anwendung läuft im Windows-Infobereich.

| Farbe | Bedeutung |
| --- | --- |
| Grün | Kommunikation OK, kein aktiver konfigurierter Fehler. |
| Grau | Keine aktive Kommunikation. |
| Gelb / blinkend | Transport wird geöffnet oder Reconnect läuft. |
| Rot | Mindestens ein konfigurierter Kanal ist im Fehler. |

## 4. Tray-Menü

Das Menü zeigt den Kanalstatus und Grundfunktionen.

`Send CHx command` stellt einen Reset-Befehl für den gewählten Kanal in die Warteschlange. Der Befehl wird vom Runtime-Kommunikationsthread verarbeitet.

`Reload config` lädt die Konfiguration neu und startet die Kommunikation neu.

`Reconnect` stoppt den aktuellen Transport und startet die Kommunikation erneut.

`Language` ändert die aktive UI-Sprache.

`About` öffnet den About-Dialog.

## 5. Meldungen

### 5.1 Active Input Error Alert

Wird angezeigt, wenn ein oder mehrere konfigurierte Kanäle einen Fehler melden.

| Taste | Aktion |
| --- | --- |
| YES | Einen gemeinsamen Befehl für alle gelisteten Fehler senden. |
| NO | Meldung ohne Befehl schließen. |

### 5.2 Repeat Fault Alert

Wird angezeigt, wenn ein Eingangsfehler nach einem Befehl wiederholt zurückkommt.

Mögliche Ursachen:

* Schutztür nicht richtig geschlossen,
* ein Kanal eines zweikanaligen Sensors fehlt,
* Sensor falsch ausgerichtet,
* Verdrahtungsproblem,
* echter Fehler, der nicht umgangen werden darf.

### 5.3 Command Failed Alert

Wird angezeigt, wenn der Befehl nicht korrekt abgeschlossen wurde.

Der Befehl ist erst erfolgreich nach akzeptiertem ON telegram, abgelaufener pulse time und akzeptiertem OFF telegram.

## 6. Konfiguration

Die Hauptkonfiguration ist derzeit:

```text
config.json
```

Der Umzug der JSON-Dateien nach `conf/` ist als nächster Schritt geplant.

## 7. transport

`transport` wählt die Sektion `network` oder `serial`.

## 8. serial

Der serielle Transport verwendet COM port, baudrate, parity, stop bits und timeout. Getesteter Wert: `115200`.

## 9. network

Der Modus `tcp_client` verwendet IP-Adresse und Port des TCP-to-RS232-Konverters.

## 10. rk512

Die Sektion `rk512` definiert device id, reply id und token telegram.

## 11. poll

Die Sektion `poll` definiert Leseperiode, Datenblock und Datengröße.

## 12. inputs

Jeder input beschreibt einen logischen Sicherheitseingang. Wenn der Kanal aktiviert ist und `ok_bit` false ist, gilt der Kanal als Fehler.

## 13. output

`output` beschreibt das command bit für den reset pulse.

## 14. repeat_fault

`repeat_fault` definiert Wiederholanzahl, Zeitfenster und Text des wiederholten Fehlers.

## 15. ui

Die Sektion `ui` enthält den Tray-Tooltip.

## 16. logging

Runtime-Logs werden immer auf Englisch geschrieben.

## 17. debug

`force_command_fail` dient nur zum Testen des Command Failed Alert.

## 18. Lokalisierung

UI-Texte werden aus `languages.json` geladen.

Dokumentation wird aus `docs/` geöffnet:

```text
docs\README_<lang>.md -> docs\README.md
docs\MANUAL_<lang>.md -> docs\MANUAL.md
```

Ukrainisch verwendet den Suffix `uk`. `ua` wird nicht unterstützt.

## 19. version.json

`version.json` enthält Produkt, Version, Beschreibung, Autor, E-Mail und Build-Datum.

## 20. Fehlerbehebung

Wenn das Tray-Symbol grau bleibt, Port, TCP-Konverter, IP-Adresse, Flexi-Soft-Versorgung und Verkabelung prüfen.

Wenn das Symbol rot ist, meldet mindestens ein konfigurierter Kanal einen Fehler.

Wenn die Meldung wiederkehrt, ist der physische Fehler wahrscheinlich noch vorhanden.

## 21. Sicherheitshinweise

FlexiSoft Runtime ist kein Sicherheitsgerät und darf nicht zum Umgehen von Sicherheitsfunktionen verwendet werden.

## 22. Support

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
