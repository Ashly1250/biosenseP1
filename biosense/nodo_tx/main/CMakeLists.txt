idf_component_register(SRCS "main.c" "ds18b20.c" "sensors.c" "telemetry.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver esp_adc esp_timer esp_rom biosense_common sx1262)
