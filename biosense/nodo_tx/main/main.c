#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "biosense_radio.h"
#include "sx1262.h"
#include "config.h"
#include "sensors.h"
#include "telemetry.h"

static const char *TAG = "TX";

static void tx_task(void *arg)
{
    TickType_t last = xTaskGetTickCount();
    char frame[BIOSENSE_LORA_MAX_PAYLOAD];

    while (1) {
        sensor_data_t d;
        sensors_read(&d);

        int n = telemetry_format_csv(&d, frame, sizeof(frame));
        if (n > 0) {
            ESP_LOGI(TAG, "Trama: %s", frame);
            esp_err_t e = sx1262_send((const uint8_t *)frame, (size_t)n, TX_TIMEOUT_MS);
            if (e == ESP_OK) ESP_LOGI(TAG, "Datos enviados");
            else             ESP_LOGE(TAG, "Error de envio: %s", esp_err_to_name(e));
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(TX_PERIOD_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "BIOSENSE - Nodo transmisor");
    ESP_ERROR_CHECK(sensors_init());
    ESP_ERROR_CHECK(biosense_radio_init());
    xTaskCreate(tx_task, "tx_task", 4096, NULL, 5, NULL);
}
