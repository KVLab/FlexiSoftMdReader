# FlexiSoft Runtime Manual

## 1. Overview

FlexiSoft Runtime is a service application for monitoring selected states of SICK Flexi Soft safety controllers through RK512 communication.

The application is designed for machines where a safety controller may report a fault after a safety guard is opened and closed. This is typically caused by non-simultaneous switching of dual-channel magnetic safety switches.

FlexiSoft Runtime detects configured input errors, informs the operator, and allows sending a controlled reset command. If the physical safety state is correct, the reset can clear the fault. If the fault is still physically present, the error remains active and the operator is warned again.

FlexiSoft Runtime is not a safety controller and does not replace any safety function of the machine.

## 2. Application Files

Recommended directory:

```text
C:\FlexiSoftRuntime\
```

Typical files:

```text
FlexiSoftRuntime.exe      Runtime application
FlexiSoftConfig.exe       Future configuration tool
config.json               Main configuration
config.json.bak           Backup configuration
languages.json            UI localization file
version.json              Version information
flexi_runtime.log         Runtime log
install.log               Installation log
README.md                 Quick start documentation
MANUAL.md                 Service manual
```

## 3. Tray Icon

The application runs in the Windows tray area.

### Icon Colors

| Color             | Meaning                                       |
| ----------------- | --------------------------------------------- |
| Green             | Communication OK, no active configured error. |
| Grey              | No active communication.                      |
| Yellow / blinking | Opening transport or reconnecting.            |
| Red               | At least one configured channel is in error.  |

## 4. Tray Menu

The tray menu contains status information and basic controls.

### Channel Status Lines

Example:

```text
CH1: OK / ON
CH2: ERROR / OFF
CH3: disabled
CH4: disabled
```

These lines are informational only.

Meaning:

| Text     | Meaning                               |
| -------- | ------------------------------------- |
| OK       | Channel is not reporting an error.    |
| ERROR    | Channel is reporting an error.        |
| ON       | Configured ON bit is active.          |
| OFF      | Configured ON bit is inactive.        |
| disabled | Channel is disabled in configuration. |

### Send Command

Menu items:

```text
Send CH1 command
Send CH2 command
Send CH3 command
Send CH4 command
```

These items queue a reset command for the selected channel.

The command is not sent directly from the tray menu thread. It is queued and processed by the runtime communication thread.

### Reload Config

Reloads `config.json` and restarts the runtime communication.

### Reconnect

Stops the current transport and starts communication again.

### Language

Changes the active UI language.

Changing language must update:

* Tray menu
* Tray tooltip
* Open alerts
* About dialog

The language setting is stored in `config.json`.

### About

Opens the About dialog.

The About dialog shows:

```text
FlexiSoft Runtime
Version
Build date

KVLab - Vladimír Kopal
vladakopal@gmail.com

Open README
Open MANUAL
Open LOG
OK
```

## 5. Alerts

### 5.1 Active Input Error Alert

Shown when one or more configured channels report an error.

The alert lists all active configured errors.

Buttons:

| Button | Action                                         |
| ------ | ---------------------------------------------- |
| YES    | Send one common command for all listed errors. |
| NO     | Close alert without command.                   |

If several channels require the same command, only one command telegram is sent. If several channels require different bits in the same command block, the bits are merged into one command.

### 5.2 Repeat Fault Alert

Shown when an input error returns repeatedly after a command.

This usually means:

* Guard is not properly closed.
* One channel of a dual-channel safety switch is missing.
* Sensor alignment is incorrect.
* Wiring problem exists.
* The fault is real and must not be bypassed.

The repeat fault threshold is configured per channel.

### 5.3 Command Failed Alert

Shown when the command could not be completed.

The command is considered successful only after:

1. Command ON telegram is accepted.
2. Pulse time expires.
3. Command OFF telegram is accepted.

If any step fails, the command failed alert is shown.

Buttons:

| Button | Action                     |
| ------ | -------------------------- |
| YES    | Retry the command.         |
| NO     | Close alert without retry. |

## 6. Configuration File

The main configuration file is:

```text
config.json
```

The application expects this file next to `FlexiSoftRuntime.exe`.

## 7. transport

```json
"transport": "network"
```

Supported values:

| Value   | Meaning              |
| ------- | -------------------- |
| network | Use network section. |
| serial  | Use serial section.  |

`transport` selects the main transport configuration section.

For network transport, the concrete network mode is configured in:

```json
"network": {
  "mode": "tcp_client"
}
```

This allows adding other network modes in the future.

## 8. serial

```json
"serial": {
  "port": "COM5",
  "baud": 115200,
  "data_bits": 8,
  "parity": "N",
  "stop_bits": 1,
  "timeout_ms": 2500
}
```

### port

Windows COM port name.

Examples:

```text
COM1
COM4
COM5
```

### baud

Serial baud rate.

Current tested value:

```text
115200
```

### data_bits

Normally:

```text
8
```

### parity

Normally:

```text
N
```

### stop_bits

Normally:

```text
1
```

### timeout_ms

Serial read/write timeout.

If this value is too low, communication may fail randomly.

If this value is too high, error recovery may feel slow.

## 9. network

```json
"network": {
  "mode": "tcp_client",
  "host": "192.168.0.7",
  "port": 4001,
  "bind_host": "0.0.0.0",
  "bind_port": 0,
  "connect_timeout_ms": 3000,
  "timeout_ms": 1500
}
```

### mode

Currently supported:

```text
tcp_client
```

### host

IP address of the TCP-to-RS232 converter.

### port

TCP port of the converter.

Typical value:

```text
4001
```

### connect_timeout_ms

Timeout for TCP connection attempt.

### timeout_ms

Timeout for network communication.

## 10. rk512

```json
"rk512": {
  "device_local": "0x4F",
  "device_reply": "0x4D",
  "token_hex": "0F 0F 46 4C 58 54 30 31"
}
```

### device_local

Local RK512 device identifier.

### device_reply

Expected reply device identifier.

### token_hex

Token telegram data used before write commands.

## 11. poll

```json
"poll": {
  "period_ms": 1000,
  "read_block": "0x76",
  "read_size": 54
}
```

### period_ms

Polling interval.

### read_block

Data block read from Flexi Soft.

### read_size

Number of bytes read from the block.

## 12. inputs

Each input channel describes how to read one logical safety input state.

Example:

```json
{
  "enabled": true,
  "name": "CH1",
  "status_byte": 0,
  "on_bit": 0,
  "ok_bit": 1,
  "alert_text_en": "CH1 reports an error",
  "alert_text_cz": "CH1 hlásí chybu"
}
```

### enabled

Enables or disables the channel.

### name

Display name of the channel.

### status_byte

Index of the byte in the fetched process image.

### on_bit

Bit showing whether the configured input is active.

### ok_bit

Bit showing whether the configured input is OK.

If the channel is enabled and `ok_bit` is false, the channel is treated as error.

### alert_text_xx

Localized channel error text.

Examples:

```json
"alert_text_en": "CH1 reports an error",
"alert_text_cz": "CH1 hlásí chybu"
```

The suffix `_xx` matches the active language.

If the text for the active language is missing, English is used.

## 13. output

```json
"output": {
  "block": "0x42",
  "byte": 0,
  "bit": 0,
  "pulse_ms": 1000
}
```

### block

RK512 block used for command output.

### byte

Byte index in the command telegram.

### bit

Bit to set during command ON.

### pulse_ms

How long the bit remains active before command OFF is sent.

## 14. repeat_fault

```json
"repeat_fault": {
  "count": 3,
  "window_ms": 30000,
  "text_en": "Check proper closing of the left door guard",
  "text_cz": "Zkontroluj správné dovření krytu u levých dveří"
}
```

### count

Number of repeated faults required to show repeat fault alert.

### window_ms

Time window for repeat fault counting.

### text_xx

Localized repeat fault text.

## 15. ui

```json
"ui": {
  "tray_tooltip_en": "FlexiSoft Runtime",
  "tray_tooltip_cz": "FlexiSoft Runtime"
}
```

Tooltip text shown over the tray icon.

## 16. logging

```json
"logging": {
  "enabled": true,
  "file": "flexi_runtime.log",
  "newest_first": true,
  "max_bytes": 65536
}
```

### enabled

Enables or disables runtime logging.

### file

Log file path.

If only filename is used, the log is stored next to the executable.

### newest_first

If true, newest log entries are placed at the top.

### max_bytes

Maximum log file size.

When the log reaches this size, it is rotated/truncated according to runtime logic.

Runtime logs are always written in English.

## 17. debug

```json
"debug": {
  "force_command_fail": false
}
```

### force_command_fail

Test option for command failed alert.

Normal value:

```json
false
```

Use only for testing command failed behavior.

## 18. Localization

The active language is stored in `config.json`:

```json
"language": "en"
```

UI translations are loaded from:

```text
languages.json
```

If `languages.json` is missing, the application must still run in English and may generate a new `languages.json` containing English default texts.

User-configurable machine texts are stored directly in `config.json` using suffixes:

```text
alert_text_en
alert_text_cz
text_en
text_cz
```

## 19. version.json

`version.json` contains application version information.

Example:

```json
{
  "product": "FlexiSoft Runtime",
  "version": "1.0-beta0.1",
  "description_en": "...",
  "description_cz": "...",
  "author": "KVLab - Vladimír Kopal",
  "email": "vladakopal@gmail.com",
  "build_date": "2026-06-15"
}
```

The About dialog uses this file.

## 20. Troubleshooting

### Tray icon stays grey

Possible causes:

* Wrong COM port.
* TCP converter not reachable.
* Wrong IP address.
* Flexi Soft not powered.
* Cable disconnected.

Check:

* `transport`
* `serial.port`
* `network.host`
* `network.port`
* physical wiring

### Tray icon blinks grey/yellow

The application is trying to reconnect.

This is normal during:

* TCP converter restart
* Ethernet unplug/replug
* Flexi Soft restart
* RS232 cable reconnect

### Tray icon is red

At least one configured input channel is reporting an error.

Open tray menu or check the alert.

### Command failed

Possible causes:

* Communication lost during command.
* TCP converter disconnected.
* Flexi Soft did not accept telegram.
* OFF telegram failed.

Try:

1. Check communication.
2. Use retry.
3. Reconnect.
4. Restart TCP converter if needed.

### Alert keeps returning

The physical fault is probably still present.

Check:

* Safety guard position.
* Magnetic safety switch alignment.
* Both safety channels.
* Wiring.
* Flexi Soft diagnostics.

### No alert appears

Check:

* Channel `enabled`
* Correct `status_byte`
* Correct `ok_bit`
* Correct `read_block`
* Correct `read_size`

### Wrong language

Check:

```json
"language": "en"
```

and `languages.json`.

If translation is missing, the application should fall back to English.

## 21. Safety Notes

FlexiSoft Runtime is not a safety device.

It must not be used to bypass safety functions.

If a repeat fault appears, the physical guard, sensor, wiring, and Flexi Soft safety logic must be checked.

The application only sends configured reset commands. It does not verify that the machine is safe to operate.

## 22. Support

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
