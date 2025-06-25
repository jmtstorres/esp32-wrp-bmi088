#include "bmi088.h"

#define I2C_PORT                I2C_NUM_0
#define I2C_SCL_IO              GPIO_NUM_22
#define I2C_SDA_IO              GPIO_NUM_21
#define I2C_FREQ_HZ             100000
#define BMI088_ADDR             0x18
#define BMI088_ACC_DATA_START   0x12

#define BMI088_ACC_X_LSB 0x12
#define BMI088_ACC_X_MSB 0x13
#define BMI088_ACC_Y_LSB 0x14
#define BMI088_ACC_Y_MSB 0x15
#define BMI088_ACC_Z_LSB 0x16
#define BMI088_ACC_Z_MSB 0x17

#define BMI088_REG_CHIP_ID      0x00

static const char *TAG = "BMI088";

static i2c_master_dev_handle_t i2c_dev;

esp_err_t bmi088_init(i2c_master_bus_handle_t i2c_bus) {
    ESP_ERROR_CHECK(i2c_master_dev_setup(i2c_bus, &i2c_dev, BMI088_ADDR));

    // Ler o CHIP_ID
    uint8_t chip_id[1] = {0};
    if (read_reg(i2c_dev, BMI088_REG_CHIP_ID, chip_id, 1) == ESP_OK) {
        ESP_LOGI(TAG, "CHIP_ID: 0x%02X", chip_id[0]);
    } else {
        ESP_LOGE(TAG, "Falha ao ler CHIP_ID");
    }

    write(i2c_dev, 0x7D, 0x04);  // ACC_PWR_CTRL: habilitar
    write(i2c_dev, 0x7C, 0x00);  // ACC_PWR_CONF: normal mode

    return ESP_OK;
}

esp_err_t bmi088_acc_data(bmi088_data data){
    uint8_t acc_data[6] = {0};
    if (read_reg(i2c_dev, BMI088_ACC_X_LSB, acc_data, 6) == ESP_OK) {
        data.acc_x = ((int16_t)((acc_data[1] << 8) | acc_data[0])) >> 3;
        data.acc_y = ((int16_t)((acc_data[3] << 8) | acc_data[2])) >> 3;
        data.acc_z = ((int16_t)((acc_data[5] << 8) | acc_data[4])) >> 1;
        ESP_LOGI(TAG, "ACC X: %d, Y: %d, Z: %d", data.acc_x, data.acc_y, data.acc_z);
    } else {
        ESP_LOGE(TAG, "Erro ao ler dados magnéticos");
    }
    return ESP_OK;
}