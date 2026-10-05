cmake_minimum_required(VERSION 3.24)

project(stm32_firmware_platform C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(FIRMWARE_TARGET stm32_firmware)
set(CMSIS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS)
set(HAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver)
set(CMSIS_DEVICE_ROOT ${CMSIS_ROOT}/Device/ST/STM32F4xx)
set(CMSIS_CORE_INCLUDE ${CMSIS_ROOT}/Core/CMSIS_5/CMSIS/Core/Include)

set(FIRMWARE_SOURCES
    Core/Src/main.c
    Core/Src/syscalls.c
    App/app.c
    Device/bme280/bme280.c
    Platform/debug_uart.c
    Platform/i2c_bus.c
    ${CMSIS_DEVICE_ROOT}/Source/Templates/system_stm32f4xx.c
    ${CMSIS_DEVICE_ROOT}/Source/Templates/gcc/startup_stm32f446xx.s
    ${HAL_ROOT}/Src/stm32f4xx_hal.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_cortex.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_dma.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_dma_ex.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_flash.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_flash_ex.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_gpio.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_i2c.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_pwr.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_pwr_ex.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_rcc.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_rcc_ex.c
    ${HAL_ROOT}/Src/stm32f4xx_hal_uart.c
)

add_executable(${FIRMWARE_TARGET}.elf ${FIRMWARE_SOURCES})

target_include_directories(${FIRMWARE_TARGET}.elf PRIVATE
    Core/Inc
    App
    Device/bme280
    Platform
    ${HAL_ROOT}/Inc
    ${CMSIS_DEVICE_ROOT}/Include
    ${CMSIS_CORE_INCLUDE}
)

target_compile_definitions(${FIRMWARE_TARGET}.elf PRIVATE
    STM32F446xx
    USE_HAL_DRIVER
    HSE_VALUE=8000000U
)

target_compile_options(${FIRMWARE_TARGET}.elf PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4;-mthumb;-mfpu=fpv4-sp-d16;-mfloat-abi=hard;-ffunction-sections;-fdata-sections;-Wall;-Wextra>
    $<$<COMPILE_LANGUAGE:ASM>:-mcpu=cortex-m4;-mthumb;-mfpu=fpv4-sp-d16;-mfloat-abi=hard>
)

target_link_options(${FIRMWARE_TARGET}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    --specs=nano.specs
    --specs=nosys.specs
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F446RETX_FLASH.ld
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_TARGET}.map
    -Wl,--cref
    -nostartfiles
)

set_target_properties(${FIRMWARE_TARGET}.elf PROPERTIES SUFFIX "")

add_custom_command(TARGET ${FIRMWARE_TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${FIRMWARE_TARGET}.elf>
            ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${FIRMWARE_TARGET}.elf>
            ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_TARGET}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${FIRMWARE_TARGET}.elf>
    BYPRODUCTS
        ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_TARGET}.hex
        ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_TARGET}.bin
    COMMENT "Creating HEX and BIN firmware images"
)
