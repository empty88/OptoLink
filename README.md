# Optolink

Dieses Projekt liest von einer Viessmann Vitodens 200-W mit Vitotronic 200 VScotHO1 (2019) einige Daten aus und sendet diese bei Bedarf an einen MQTT Broker. Die Betriebsart kann zwischen "Abschaltbetrieb", "Nur Warmwasser" und "Heizen+Warmwasser" umgeschaltet werden.
Fehlercodes werden in Fehlermeldungen übersetzt.
Die Datenpunkte werden automatisch im Webinterface mit dem aktuellen Wert angezeigt. Aktuelle Werte werden per AJAX nachgeladen.

[Projektdetails auf meinem Blog](https://blog.mt88.eu/2022/10/27/viessmann-heizungsdaten-2-0/)


## Einrichtung

 - mit WLAN "optolink_config" verbinden
 - 192.168.0.1 im Browser aufrufen
 - WLAN-Zugangsdaten eingeben
 - NTP-Server IP eingeben (optional)
 - MQTT-Broker IP eingeben (optional)
 - MQTT Topic Prefix anpassen (optional)
 - MQTT Client Id anpassen (optional)
 - Änderung des Protokolls (P300 / KW) in zwei Dateien ändern(viessmann.h, viessmann.cpp) (optional)

## Features

 - Kann beim Start des Moduls (z.B. nach Ersteinrichtung) keine Verbindung zum WLAN hergestellt werden, werden die WLAN-Einstellungen nach etwa 1,5 Minuten zurückgesetzt und somit der Konfigurationsmodus wieder aktiviert.

 - Die Einstellungen können auch nachträglich noch über das Webinterface verändert werden.

 - Die Einstellungen werden im Flash-Speicher persistent gespeichert.

 - Benachrichtigung bei Störung

 - Logeinträge im Webinterface mit Datum und Uhrzeit, da NTP Integration

 - Falls keine Daten über die Infrarotschnittstelle gelesen werden können, sendet das Modul optolink/error = 1 (bei Standard Topic Prefix) über MQTT, wartet eine Minute und versucht es erneut.
Durch dieses Verhalten wird auch sichergestellt, dass die Heizung nach einem Stromausfall wieder startet. Beim Start der Heizung dürfen keine Infrarotsignale gesendet werden, da die Heizung sonst in einer Start-Schleife hängen bleibt.

 - Das Modul lässt sich im Nachgang über WLAN(OTA) flashen.

 - Werte der Datenpunkte werden im Webinterface übersichtlich dargestellt (responsives Design) mit automatischer Aktualisierung (AJAX)

 - Umschalten der Betriebsart über das Webinterface möglich
 
 ![wide](https://user-images.githubusercontent.com/29315283/217073328-74bc63c6-5688-4ee7-8f82-0507d2c4505c.JPG)
![small](https://user-images.githubusercontent.com/29315283/217073387-57d89a8f-52c1-41c8-a976-6f2cc904aa9c.JPG)
