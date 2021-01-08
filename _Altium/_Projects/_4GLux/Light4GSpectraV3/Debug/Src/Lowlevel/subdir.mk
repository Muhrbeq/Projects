################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Lowlevel/LowLevel.c \
../Src/Lowlevel/adc.c \
../Src/Lowlevel/crc.c \
../Src/Lowlevel/gpio.c \
../Src/Lowlevel/i2c.c \
../Src/Lowlevel/iwdg.c \
../Src/Lowlevel/lptim.c \
../Src/Lowlevel/rtc.c \
../Src/Lowlevel/stm32l4xx_hal_msp.c \
../Src/Lowlevel/stm32l4xx_it.c \
../Src/Lowlevel/system_stm32l4xx.c \
../Src/Lowlevel/usart.c 

OBJS += \
./Src/Lowlevel/LowLevel.o \
./Src/Lowlevel/adc.o \
./Src/Lowlevel/crc.o \
./Src/Lowlevel/gpio.o \
./Src/Lowlevel/i2c.o \
./Src/Lowlevel/iwdg.o \
./Src/Lowlevel/lptim.o \
./Src/Lowlevel/rtc.o \
./Src/Lowlevel/stm32l4xx_hal_msp.o \
./Src/Lowlevel/stm32l4xx_it.o \
./Src/Lowlevel/system_stm32l4xx.o \
./Src/Lowlevel/usart.o 

C_DEPS += \
./Src/Lowlevel/LowLevel.d \
./Src/Lowlevel/adc.d \
./Src/Lowlevel/crc.d \
./Src/Lowlevel/gpio.d \
./Src/Lowlevel/i2c.d \
./Src/Lowlevel/iwdg.d \
./Src/Lowlevel/lptim.d \
./Src/Lowlevel/rtc.d \
./Src/Lowlevel/stm32l4xx_hal_msp.d \
./Src/Lowlevel/stm32l4xx_it.d \
./Src/Lowlevel/system_stm32l4xx.d \
./Src/Lowlevel/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Lowlevel/LowLevel.o: ../Src/Lowlevel/LowLevel.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/LowLevel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/adc.o: ../Src/Lowlevel/adc.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/crc.o: ../Src/Lowlevel/crc.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/crc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/gpio.o: ../Src/Lowlevel/gpio.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/i2c.o: ../Src/Lowlevel/i2c.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/i2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/iwdg.o: ../Src/Lowlevel/iwdg.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/iwdg.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/lptim.o: ../Src/Lowlevel/lptim.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/lptim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/rtc.o: ../Src/Lowlevel/rtc.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/rtc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/stm32l4xx_hal_msp.o: ../Src/Lowlevel/stm32l4xx_hal_msp.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/stm32l4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/stm32l4xx_it.o: ../Src/Lowlevel/stm32l4xx_it.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/system_stm32l4xx.o: ../Src/Lowlevel/system_stm32l4xx.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Lowlevel/usart.o: ../Src/Lowlevel/usart.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Lowlevel/usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

