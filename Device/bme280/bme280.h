#ifndef BME280_H
#define BME280_H

#include <stdint.h>

typedef enum
{
    BME280_OK = 0,
    BME280_ERROR
} BME280_Status;

#define BME280_REGISTER_CHIP_ID 0xD0U
#define BME280_CHIP_ID_VALUE    0x60U

BME280_Status bme280_read_chip_id(uint8_t *chip_id);

#endif
