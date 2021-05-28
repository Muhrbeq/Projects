################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Drivers/Sensor/AS726X.c \
../Src/Drivers/Sensor/HDC1080.c \
../Src/Drivers/Sensor/MultiSensor.c \
../Src/Drivers/Sensor/Tmp112.c \
../Src/Drivers/Sensor/VEML7700.c 

OBJS += \
./Src/Drivers/Sensor/AS726X.o \
./Src/Drivers/Sensor/HDC1080.o \
./Src/Drivers/Sensor/MultiSensor.o \
./Src/Drivers/Sensor/Tmp112.o \
./Src/Drivers/Sensor/VEML7700.o 

C_DEPS += \
./Src/Drivers/Sensor/AS726X.d \
./Src/Drivers/Sensor/HDC1080.d \
./Src/Drivers/Sensor/MultiSensor.d \
./Src/Drivers/Sensor/Tmp112.d \
./Src/Drivers/Sensor/VEML7700.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Drivers/Sensor/AS726X.o: ../Src/Drivers/Sensor/AS726X.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/Sensor/AS726X.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/Sensor/HDC1080.o: ../Src/Drivers/Sensor/HDC1080.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/Sensor/HDC1080.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/Sensor/MultiSensor.o: ../Src/Drivers/Sensor/MultiSensor.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/Sensor/MultiSensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/Sensor/Tmp112.o: ../Src/Drivers/Sensor/Tmp112.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/Sensor/Tmp112.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Drivers/Sensor/VEML7700.o: ../Src/Drivers/Sensor/VEML7700.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Drivers/Sensor/VEML7700.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

