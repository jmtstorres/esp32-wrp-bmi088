#ifndef BMI088_H
#define BMI088_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wrp_i2c.h"

typedef struct {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
} bmi088_data;

esp_err_t bmi088_init(i2c_master_bus_handle_t i2c_bus);
esp_err_t bmi088_acc_data(bmi088_data data);

#endif