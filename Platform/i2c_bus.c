#include "i2c_bus.h"

#include "main.h"

#define I2C_BUS_TIMEOUT_MS 100U

I2C_BusStatus i2c_bus_read_register(uint16_t device_address,
                                    uint8_t register_address,
                                    uint8_t *value)
{
    if ((value == 0) ||
        (HAL_I2C_Mem_Read(&hi2c1,
                          device_address,
                          register_address,
                          I2C_MEMADD_SIZE_8BIT,
                          value,
                          1U,
                          I2C_BUS_TIMEOUT_MS) != HAL_OK))
    {
        return I2C_BUS_ERROR;
    }

    return I2C_BUS_OK;
}
