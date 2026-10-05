#include "bme280.h"

#include "i2c_bus.h"

#define BME280_ADDRESS_PRIMARY   (0x76U << 1U)
#define BME280_ADDRESS_SECONDARY (0x77U << 1U)

BME280_Status bme280_read_chip_id(uint8_t *chip_id)
{
    if (chip_id == 0)
    {
        return BME280_ERROR;
    }

    if (i2c_bus_read_register(BME280_ADDRESS_PRIMARY,
                              BME280_REGISTER_CHIP_ID,
                              chip_id) == I2C_BUS_OK)
    {
        return BME280_OK;
    }

    if (i2c_bus_read_register(BME280_ADDRESS_SECONDARY,
                              BME280_REGISTER_CHIP_ID,
                              chip_id) == I2C_BUS_OK)
    {
        return BME280_OK;
    }

    return BME280_ERROR;
}
