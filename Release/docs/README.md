# FlexiSoft Runtime

FlexiSoft Runtime is a standalone service application for monitoring SICK Flexi Soft safety controllers, automatic recovery of selected fault states, and operator notification about repeated safety input errors.

## Purpose

The application is intended for machines where Flexi Soft reports safety input faults caused by non-simultaneous switching of dual-channel safety sensors.

Typical use case:

1. A safety guard is opened and closed.
2. Both safety channels do not switch at exactly the same time.
3. Flexi Soft reports an input simultaneity fault.
4. FlexiSoft Runtime detects the fault.
5. The operator can confirm a controlled reset command.
6. If the fault was only caused by timing difference, the machine safety input returns to OK.
7. If the real safety fault is still present, the fault remains active and the operator is warned again.

The application does not replace the safety function of Flexi Soft. It only provides monitoring, operator notification, and controlled reset of configured inputs.

## Main Features

* RK512 communication with SICK Flexi Soft.
* Serial transport.
* TCP client transport through TCP-to-RS232 converter.
* Monitoring of up to four configured channels.
* One combined alert for multiple active faults.
* Controlled reset command for all affected channels.
* Repeat fault detection.
* Command failed alert with retry option.
* Tray icon status indication.
* Runtime logging.
* Configurable JSON setup.
* Windows XP SP3 32-bit to Windows 11 compatibility.

## Installation

The recommended installation directory is:

```text
C:\FlexiSoftRuntime\
```

Recommended directory content:

```text
FlexiSoftRuntime.exe
FlexiSoftConfig.exe
config.json
config.json.bak
languages.json
version.json
flexi_runtime.log
install.log
README.md
README_cz.md
MANUAL.md
MANUAL_cz.md
```

The application expects `config.json` in the same directory as the executable.

## First Start

1. Copy `FlexiSoftRuntime.exe` and `config.json` into `C:\FlexiSoftRuntime\`.
2. Check communication settings in `config.json`.
3. Start `FlexiSoftRuntime.exe`.
4. The tray icon appears in the Windows notification area.
5. Wait for the tray icon to turn green.

## Tray Icon Colors

| Color             | Meaning                                                                     |
| ----------------- | --------------------------------------------------------------------------- |
| Green             | Communication is OK and no configured channel is in error.                  |
| Grey              | Communication is not active or transport is disconnected.                   |
| Yellow / blinking | Reconnect or communication opening is in progress.                          |
| Red               | Communication is OK, but at least one configured input channel is in error. |

## Tray Menu

Right-click or left-click the tray icon to open the tray menu.

Typical menu items:

```text
FlexiSoft Runtime
CH1: OK / ON
CH2: ERROR / OFF
CH3: disabled
CH4: disabled

Send CH1 command
Send CH2 command
Send CH3 command
Send CH4 command

Reload config
Reconnect
Language
About
Exit
```

Channel lines are informative only.

Command menu items queue a reset command for the selected channel.

## Alerts

### Input Error Alert

Shown when one or more configured channels report an error.

The alert contains all affected channels.

Buttons:

* `YES` sends one common command for all listed errors.
* `NO` closes the alert without sending a command.

### Repeat Fault Alert

Shown when the same channel repeatedly returns to error after a reset command.

This usually means that the guard is not properly closed or one safety channel is still missing.

### Command Failed Alert

Shown when the reset command could not be completed.

Buttons:

* `YES` retries the command.
* `NO` closes the alert without retry.

## Logging

The runtime log file is normally:

```text
flexi_runtime.log
```

Logging is configured in `config.json`.

Logs are always written in English, regardless of selected UI language.

## Documentation

Detailed service documentation is available in:

```text
MANUAL.md
```

Localized documentation may be provided as:

```text
README_cz.md
MANUAL_cz.md
README_de.md
MANUAL_de.md
README_fr.md
MANUAL_fr.md
README_ua.md
MANUAL_ua.md
```

If localized documentation for the selected language is not available, the application falls back to English documentation.

## Author

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
