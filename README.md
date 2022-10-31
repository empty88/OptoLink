Optolink
--------

Dieses Projekt liest von einer Viessmann Vitodens 200-W mit Vitotronic 200 VScotHO1 (2019) einige Daten aus und sendet diese an einen MQTT Broker.
Fehlercodes werden in Fehlermeldungen übersetzt.

[Projektdetails auf meinem Blog](https://blog.mt88.eu/2022/10/27/viessmann-heizungsdaten-2-0/)


Einrichtung
-----------
`main.cpp`
WIFI_SSID und WIFI_PW anpassen

`mqtt.h`
Bei Bedarf topic_prefix anpassen

`mqqt.cpp`
IP des MQTT-Brokers anpasssen
