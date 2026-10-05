set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(NOT DEFINED ENV{STM32_ARM_GCC_ROOT})
    message(FATAL_ERROR "Set STM32_ARM_GCC_ROOT to the Arm GNU Toolchain install directory.")
endif()

file(TO_CMAKE_PATH "$ENV{STM32_ARM_GCC_ROOT}" STM32_ARM_GCC_ROOT)
set(ARM_GNU_BIN "${STM32_ARM_GCC_ROOT}/bin")

set(CMAKE_C_COMPILER "${ARM_GNU_BIN}/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER "${ARM_GNU_BIN}/arm-none-eabi-gcc.exe")
set(CMAKE_AR "${ARM_GNU_BIN}/arm-none-eabi-ar.exe")
set(CMAKE_RANLIB "${ARM_GNU_BIN}/arm-none-eabi-ranlib.exe")
set(CMAKE_OBJCOPY "${ARM_GNU_BIN}/arm-none-eabi-objcopy.exe")
set(CMAKE_SIZE "${ARM_GNU_BIN}/arm-none-eabi-size.exe")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
