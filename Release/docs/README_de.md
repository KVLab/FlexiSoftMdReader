# FlexiSoft Runtime — Schnellstart

**LOKALISIERUNGSTEST: README_de.md**

FlexiSoft Runtime ist eine eigenständige Service-Anwendung zur Überwachung von SICK Flexi Soft Sicherheitssteuerungen, zur kontrollierten Wiederherstellung ausgewählter Fehlerzustände und zur Benachrichtigung des Bedieners über wiederholte Fehler an Sicherheitseingängen.

## Zweck

Die Anwendung ist für Maschinen gedacht, bei denen Flexi Soft Eingangfehler meldet, die durch nicht gleichzeitiges Schalten zweikanaliger Sicherheitssensoren entstehen.

Typischer Ablauf:

1. Eine Schutztür wird geöffnet und geschlossen.
2. Beide Sicherheitskanäle schalten nicht exakt gleichzeitig.
3. Flexi Soft meldet einen Gleichzeitigkeitsfehler am Eingang.
4. FlexiSoft Runtime erkennt den Fehler.
5. Der Bediener kann einen kontrollierten Reset-Befehl bestätigen.
6. Wenn nur ein Zeitversatz die Ursache war, kehrt der Eingang zu OK zurück.
7. Wenn der physische Fehler weiterhin vorhanden ist, bleibt der Fehler aktiv und der Bediener wird erneut gewarnt.

Die Anwendung ersetzt keine Sicherheitsfunktion von Flexi Soft. Sie überwacht nur, informiert den Bediener und sendet konfigurierte Reset-Befehle.

## Hauptfunktionen

* RK512-Kommunikation mit SICK Flexi Soft.
* Serieller Transport.
* TCP-Client-Transport über TCP-to-RS232-Konverter.
* Überwachung von bis zu vier konfigurierten Kanälen.
* Eine gemeinsame Meldung für mehrere aktive Fehler.
* Kontrollierter Reset-Befehl für betroffene Kanäle.
* Erkennung wiederholter Fehler.
* Meldung bei fehlgeschlagenem Befehl mit Wiederholoption.
* Statusanzeige im Tray.
* Runtime-Log.
* JSON-Konfiguration.
* Kompatibilität von Windows XP SP3 32-bit bis Windows 11.

## Installation

Empfohlener Ordner:

```text
C:\FlexiSoftRuntime\
```

Empfohlener Inhalt:

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

## Erster Start

1. Kopieren Sie `FlexiSoftRuntime.exe` und die Konfigurationsdateien in den Arbeitsordner.
2. Prüfen Sie die Kommunikationseinstellungen in `config.json`.
3. Starten Sie `FlexiSoftRuntime.exe`.
4. Das Tray-Symbol erscheint im Windows-Infobereich.
5. Warten Sie, bis das Symbol grün wird.

## Farben des Tray-Symbols

| Farbe | Bedeutung |
| --- | --- |
| Grün | Kommunikation OK und kein konfigurierter Kanal ist im Fehler. |
| Grau | Kommunikation nicht aktiv oder Transport getrennt. |
| Gelb / blinkend | Verbindung oder Reconnect läuft. |
| Rot | Kommunikation OK, aber mindestens ein Kanal ist im Fehler. |

## Meldungen

* **Input Error Alert** — aktiver Eingangsfehler.
* **Repeat Fault Alert** — derselbe Fehler kehrt nach einem Befehl zurück.
* **Command Failed Alert** — der Reset-Befehl wurde nicht korrekt abgeschlossen.

## Dokumentation

Detaillierte Servicedokumentation:

```text
docs\MANUAL_de.md
```

Wenn keine lokalisierte Dokumentation vorhanden ist, öffnet die Anwendung den Fallback:

```text
docs\README.md
docs\MANUAL.md
```

## Autor

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
