################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Utilities/Error.c \
../Src/Utilities/Jsmn.c \
../Src/Utilities/TimeFunc.c \
../Src/Utilities/utils.c 

OBJS += \
./Src/Utilities/Error.o \
./Src/Utilities/Jsmn.o \
./Src/Utilities/TimeFunc.o \
./Src/Utilities/utils.o 

C_DEPS += \
./Src/Utilities/Error.d \
./Src/Utilities/Jsmn.d \
./Src/Utilities/TimeFunc.d \
./Src/Utilities/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Utilities/Error.o: ../Src/Utilities/Error.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Utilities/Error.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Utilities/Jsmn.o: ../Src/Utilities/Jsmn.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Utilities/Jsmn.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Utilities/TimeFunc.o: ../Src/Utilities/TimeFunc.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Utilities/TimeFunc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/Utilities/utils.o: ../Src/Utilities/utils.c
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_LL_DRIVER '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Include -I../Inc/Drivers -I../Inc/Lowlevel -I../Inc/Network -I../Inc/Utilities -I../Inc/Drivers/Sensor -I../Inc/Drivers/SDCard -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Utilities/utils.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
