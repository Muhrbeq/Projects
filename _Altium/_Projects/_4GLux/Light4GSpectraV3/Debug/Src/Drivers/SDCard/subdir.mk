################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Drivers/SDCard/SDCard.c \
../Src/Drivers/SDCard/bsp_driver_sd.c \
../Src/Drivers/SDCard/fatfs.c \
../Src/Drivers/SDCard/sd_diskio.c \
../Src/Drivers/SDCard/sdmmc.c 

OBJS += \
./Src/Drivers/SDCard/SDCard.o \
./Src/Drivers/SDCard/bsp_driver_sd.o \
./Src/Drivers/SDCard/fatfs.o \
./Src/Drivers/SDCard/sd_diskio.o \
./Src/Drivers/SDCard/sdmmc.o 

C_DEPS += \
./Src/Drivers/SDCard/SDCard.d \
./Src/Drivers/SDCard/bsp_driver_sd.d \
./Src/Drivers/SDCard/fatfs.d \
./Src/Drivers/SDCard/sd_diskio.d \
./Src/Drivers/SDCard/sdmmc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Drivers/SDCard/SDCard.o: ../Src/Drivers/SDCard/SDCard.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/SDCard/SDCard.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/SDCard/bsp_driver_sd.o: ../Src/Drivers/SDCard/bsp_driver_sd.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/SDCard/bsp_driver_sd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/SDCard/fatfs.o: ../Src/Drivers/SDCard/fatfs.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/SDCard/fatfs.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/SDCard/sd_diskio.o: ../Src/Drivers/SDCard/sd_diskio.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/SDCard/sd_diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/SDCard/sdmmc.o: ../Src/Drivers/SDCard/sdmmc.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/SDCard/sdmmc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

