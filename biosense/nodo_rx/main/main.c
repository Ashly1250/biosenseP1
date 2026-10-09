#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "biosense_radio.h"
#include "sx1262.h"
#include "config.h"
#include "sd_logger.h"

static const char *TAG = "RX";

/* Una trama valida tiene 5 campos => 4 comas */
static bool trama_valida(const char *s)
{
    int comas = 0;
    for (; *s; s++) if (*s == ',') comas++;
    return comas == 4;
}

static void rx_task(void *arg)
{
    uint8_t buf[BIOSENSE_LORA_MAX_PAYLOAD + 1];

    ESP_ERROR_CHECK(sx1262_start_rx());
    ESP_LOGI(TAG, "Esperando lecturas de sensores...");

    while (1) {
        size_t len = 0;
        int16_t rssi = 0;
        float snr = 0;
        esp_err_t e = sx1262_receive(buf, sizeof(buf) - 1, &len, &rssi, &snr, RX_POLL_TIMEOUT_MS);

        if (e == ESP_ERR_TIMEOUT) continue;
        if (e == ESP_ERR_INVALID_CRC) {
            ESP_LOGW(TAG, "Paquete con CRC/cabecera invalida, descartado");
            continue;
        }
        if (e != ESP_OK) {
            ESP_LOGE(TAG, "Error de recepcion: %s", esp_err_to_name(e));
            sx1262_start_rx();
            continue;
        }

        buf[len] = '\0';
        char *trama = (char *)buf;
        while (len > 0 && (trama[len - 1] == '\r' || trama[len - 1] == '\n')) trama[--len] = '\0';

        ESP_LOGI(TAG, "---- NUEVA LECTURA ----");
        ESP_LOGI(TAG, "Trama CSV: %s", trama);
        ESP_LOGI(TAG, "RSSI: %d dBm | SNR: %.2f dB", rssi, snr);

        if (!trama_valida(trama)) {
            ESP_LOGW(TAG, "Trama con formato inesperado, no se guarda");
            continue;
        }

        uint32_t t_ms = (uint32_t)(esp_timer_get_time() / 1000);
        if (sd_logger_append(t_ms, trama, rssi, snr) == ESP_OK) {
            ESP_LOGI(TAG, "-> Guardado en %s", SD_FILE_PATH);
        } else {
            ESP_LOGW(TAG, "No se pudo guardar en la SD");
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "BIOSENSE - Nodo receptor LoRa + SD");

    ESP_ERROR_CHECK(biosense_radio_init());

    if (sd_logger_init() != ESP_OK) {
        ESP_LOGW(TAG, "La recepcion funcionara, pero no se registraran datos en la SD");
    }

    xTaskCreate(rx_task, "rx_task", 6144, NULL, 5, NULL);
}
