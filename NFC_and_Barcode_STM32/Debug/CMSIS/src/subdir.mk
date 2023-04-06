################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/src/system_stm32f10x.c 

OBJS += \
./CMSIS/src/system_stm32f10x.o 

C_DEPS += \
./CMSIS/src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/src/%.o CMSIS/src/%.su: ../CMSIS/src/%.c CMSIS/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Freertos tests/NFC_and_Barcode_STM32/CMSIS/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/Core/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPL/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/NFC/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/LCD/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/USART/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPI/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CMSIS-2f-src

clean-CMSIS-2f-src:
	-$(RM) ./CMSIS/src/system_stm32f10x.d ./CMSIS/src/system_stm32f10x.o ./CMSIS/src/system_stm32f10x.su

.PHONY: clean-CMSIS-2f-src

