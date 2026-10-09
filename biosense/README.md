# BIOSENSE - ESP-IDF (Heltec WiFi LoRa 32 V3)

```
biosense/
├── components/              <- compartido por TX y RX
│   ├── sx1262/              driver SPI del radio SX1262 (LoRa)
│   └── biosense_common/     parametros LoRa + pines del radio (iguales en ambos nodos)
├── nodo_tx/                 proyecto ESP-IDF del transmisor
│   └── main/  main.c  config.h  sensors.c/.h  ds18b20.c/.h  telemetry.c/.h
└── nodo_rx/                 proyecto ESP-IDF del receptor
    └── main/  main.c  config.h  sd_logger.c/.h
```

## Compilar y cargar
Requiere ESP-IDF 5.3 o superior.

```bash
cd nodo_tx
idf.py set-target esp32s3
idf.py -p COMx flash monitor      # Linux: /dev/ttyUSB0

cd ../nodo_rx
idf.py set-target esp32s3
idf.py -p COMy flash monitor
```
Cada carpeta es un proyecto independiente: se compilan por separado.

## Pines
| Funcion | GPIO |
|---|---|
| DS18B20 (1-Wire, pull-up 4.7k) | 4 |
| TDS | 7 |
| pH (pot) | 5 |
| OD (pot) | 6 |
| SST (pot) | 2 |
| SD CS / SCK / MOSI / MISO | 26 / 47 / 48 / 45 |
| Vext (alimentacion SD) | 36 (LOW = ON) |

Los pines se cambian en `nodo_tx/main/config.h` y `nodo_rx/main/config.h`.
Los parametros LoRa (frecuencia, SF, etc.) se cambian UNA vez en
`components/biosense_common/include/biosense_radio.h`.

## Formato
Trama LoRa: `TEMP,TDS,PH,OD,SST`
Archivo SD: `TIEMPO_MS,TEMP_C,TDS_PPM,PH,OD_MGL,SST_MGL,RSSI_DBM,SNR_DB`

## Notas
- GPIO45 es un pin de strapping del ESP32-S3 y GPIO26 esta cerca de los pines de flash.
  Si el receptor no arranca con la SD conectada, desconecta MISO (45) al resetear o muevelo a otro GPIO libre.
- SD en FAT32. Si algun dia falla el .csv, cambia `SD_FILE_PATH` a `.txt` en `nodo_rx/main/config.h`.
