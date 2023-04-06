################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/src/main.c \
../Core/src/perif.c \
../Core/src/startup.c 

OBJS += \
./Core/src/main.o \
./Core/src/perif.o \
./Core/src/startup.o 

C_DEPS += \
./Core/src/main.d \
./Core/src/perif.d \
./Core/src/startup.d 


# Each subdirectory must supply rules for building sources it contributes
Core/src/%.o Core/src/%.su: ../Core/src/%.c Core/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Freertos tests/NFC_and_Barcode_STM32/CMSIS/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/Core/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPL/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/NFC/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/LCD/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/USART/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPI/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-src

clean-Core-2f-src:
	-$(RM) ./Core/src/main.d ./Core/src/main.o ./Core/src/main.su ./Core/src/perif.d ./Core/src/perif.o ./Core/src/perif.su ./Core/src/startup.d ./Core/src/startup.o ./Core/src/startup.su

.PHONY: clean-Core-2f-src

