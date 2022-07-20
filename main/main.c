#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "app_adc.h"
#include "esp_adc_cal.h"

static esp_adc_cal_characteristics_t adc1_chars;
uint32_t adc_cal[3];
esp_err_t Vol[3];
uint32_t adc_vol = 0;
uint32_t dac_val = 0;


void app_main(void)
{

    adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    while(1)
    {
       
        Vol[0] = esp_adc_cal_get_voltage(ADC1_CHANNEL_3, &adc1_chars, &adc_cal[0]);
        Vol[1] = esp_adc_cal_get_voltage(ADC1_CHANNEL_6, &adc1_chars, &adc_cal[1]);
        Vol[2] = esp_adc_cal_get_voltage(ADC1_CHANNEL_7, &adc1_chars, &adc_cal[2]);

        printf("Vol_ADC1: %d, Vol_ADC2: %d, Vol_ADC3: %d \n", adc_cal[0], adc_cal[1], adc_cal[2]);


        dac_val = adc1_get_raw(ADC1_CHANNEL_7);
        dac_output_enable(DAC_CHANNEL_1);
        dac_output_voltage(DAC_CHANNEL_1, (dac_val >> 4));
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}