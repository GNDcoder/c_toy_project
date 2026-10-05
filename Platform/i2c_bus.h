#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <stdint.h>

typedef enum
{
    I2C_BUS_OK = 0,
    I2C_BUS_ERROR
} I2C_BusStatus;

I2C_BusStatus i2c_bus_read_register(uint16_t device_address,
                                    uint8_t register_address,
                                    uint8_t *value);

#endif
