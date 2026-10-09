#include "telemetry.h"
#include <stdio.h>

int telemetry_format_csv(const sensor_data_t *d, char *out, size_t out_size)
{
    int n = snprintf(out, out_size, "%.2f,%.2f,%.2f,%.2f,%.2f",
                     d->temp_c, d->tds_ppm, d->ph, d->od_mgl, d->sst_mgl);
    if (n < 0 || (size_t)n >= out_size) return -1;
    return n;
}
