################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTC/src/rtc.c \
../RTC/src/rtc_time.c 

OBJS += \
./RTC/src/rtc.o \
./RTC/src/rtc_time.o 

C_DEPS += \
./RTC/src/rtc.d \
./RTC/src/rtc_time.d 


# Each subdirectory must supply rules for building sources it contributes
RTC/src/%.o RTC/src/%.su: ../RTC/src/%.c RTC/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Freertos tests/NFC_and_Barcode_STM32/CMSIS/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/Core/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPL/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/NFC/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/LCD/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/USART/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPI/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RTC-2f-src

clean-RTC-2f-src:
	-$(RM) ./RTC/src/rtc.d ./RTC/src/rtc.o ./RTC/src/rtc.su ./RTC/src/rtc_time.d ./RTC/src/rtc_time.o ./RTC/src/rtc_time.su

.PHONY: clean-RTC-2f-src

