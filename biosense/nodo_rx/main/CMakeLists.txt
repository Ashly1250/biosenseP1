idf_component_register(SRCS "main.c" "sd_logger.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver esp_timer fatfs sdmmc vfs biosense_common sx1262)
