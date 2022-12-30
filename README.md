# Optolink

Dieses Projekt liest von einer Viessmann Vitodens 200-W mit Vitotronic 200 VScotHO1 (2019) einige Daten aus und sendet diese an einen MQTT Broker.
Fehlercodes werden in Fehlermeldungen übersetzt.

[Projektdetails auf meinem Blog](https://blog.mt88.eu/2022/10/27/viessmann-heizungsdaten-2-0/)


## Einrichtung

 - mit WLAN "optolink_config" verbinden
 - 192.168.0.1 im Browser aufrufen
 - WLAN-Zugangsdaten eingeben
 - NTP-Server IP eingeben (optional)
 - MQTT-Broker IP eingeben
 - MQTT Topic Prefix anpassen (optional)
 - MQTT Client Id anpassen (optional)

## Features

 - Kann beim Start des Moduls (z.B. nach Ersteinrichtung) keine Verbindung zum WLAN hergestellt werden, werden die WLAN-Einstellungen nach etwa 1,5 Minuten zurückgesetzt und somit der Konfigurationsmodus wieder aktiviert.

 - Die Einstellungen können auch nachträglich noch über das Webinterface verändert werden.

 - Die Einstellungen werden im Flash-Speicher persistent gespeichert.

 - Benachrichtigung bei Störung

 - Logeinträge im Webinterface mit Datum und Uhrzeit, da NTP Integration

 - Falls keine Daten über die Infrarotschnittstelle gelesen werden können, sendet das Modul optolink/error = 1 (bei Standard Topic Prefix) über MQTT, wartet eine Minute und versucht es erneut.
Durch dieses Verhalten wird auch sichergestellt, dass die Heizung nach einem Stromausfall wieder startet. Beim Start der Heizung dürfen keine Infrarotsignale gesendet werden, da die Heizung sonst in einer Start-Schleife hängen bleibt.

 - Das Modul lässt sich im Nachgang über WLAN(OTA) flashen.