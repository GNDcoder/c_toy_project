#include "debug_uart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

#define DEBUG_UART_BUFFER_SIZE 128U

void debug_uart_write(const char *text)
{
    if (text != 0)
    {
        (void)HAL_UART_Transmit(&huart2,
                                (uint8_t *)text,
                                (uint16_t)strlen(text),
                                HAL_MAX_DELAY);
    }
}

void debug_uart_printf(const char *format, ...)
{
    char buffer[DEBUG_UART_BUFFER_SIZE];
    va_list arguments;

    va_start(arguments, format);
    (void)vsnprintf(buffer, sizeof(buffer), format, arguments);
    va_end(arguments);

    debug_uart_write(buffer);
}
