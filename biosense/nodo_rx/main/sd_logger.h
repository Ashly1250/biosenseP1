#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

esp_err_t sd_logger_init(void);     /* alimenta Vext, monta la SD y crea el CSV con encabezado */
bool      sd_logger_ready(void);

/* Agrega una linea: tiempo_ms,<trama>,rssi,snr */
esp_err_t sd_logger_append(uint32_t tiempo_ms, const char *trama,
                           int rssi_dbm, float snr_db);
