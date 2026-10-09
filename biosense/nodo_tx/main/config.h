#pragma once
/* ---------- Pines de sensores (Heltec V3) ---------- */
#define PIN_ONE_WIRE   4   /* DS18B20 (con pull-up de 4.7k a 3.3V) */
#define PIN_TDS        2
#define PIN_PH         5   /* simulado con potenciometro */
#define PIN_OD         6   /* simulado con potenciometro */
#define PIN_TSS        7   /* simulado con potenciometro */

/* ---------- ADC ---------- */
#define ADC_SAMPLES         16
/* Voltaje (mV) que corresponde al 100 % en los sensores simulados.
   Con atenuacion de 12 dB el ADC del S3 satura cerca de 3100 mV.
   Si tu potenciometro llega a 3.3 V, igual se limitara a este valor. */
#define SIM_FULLSCALE_MV    3100.0f

/* Rangos de los sensores simulados */
#define PH_MAX              14.0f    /* pH 0..14 */
#define OD_MAX_MGL          20.0f    /* OD 0..20 mg/L */
#define SST_MAX_MGL         1000.0f  /* SST 0..1000 mg/L */

/* ---------- Temporizacion ---------- */
#define TX_PERIOD_MS        5000
#define TX_TIMEOUT_MS       3000

/* Valor enviado si el DS18B20 no responde (igual que la libreria Dallas) */
#define TEMP_ERROR_VALUE    (-127.0f)
