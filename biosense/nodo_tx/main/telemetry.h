#pragma once
#include <stddef.h>
#include "sensors.h"

/* Formato: "TEMP,TDS,PH,OD,SST" con 2 decimales. Devuelve longitud o <0 si no cabe. */
int telemetry_format_csv(const sensor_data_t *d, char *out, size_t out_size);
