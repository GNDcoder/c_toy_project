#include "app.h"

#include <stdint.h>

#include "bme280.h"
#include "debug_uart.h"

static uint8_t chip_id;
static BME280_Status status;

void app_init(void)
{
    debug_uart_write("\r\nSTM32F446RE firmware started\r\n");
    debug_uart_write("[BME280] Initializing...\r\n");

    status = bme280_read_chip_id(&chip_id);
    if (status == BME280_OK)
    {
        debug_uart_printf("[BME280] Chip ID: 0x%02X\r\n", chip_id);
        if (chip_id == BME280_CHIP_ID_VALUE)
        {
            debug_uart_write("[BME280] Device detected\r\n[I2C] Communication OK\r\n");
        }
        else
        {
            debug_uart_write("[BME280] Unexpected chip ID\r\n");
        }
    }
    else
    {
        debug_uart_write("[BME280] Not found at address 0x76 or 0x77\r\n");
    }
}

void app_process(void)
{
    /* Sensor measurements and storage will be added in later steps. */
}
