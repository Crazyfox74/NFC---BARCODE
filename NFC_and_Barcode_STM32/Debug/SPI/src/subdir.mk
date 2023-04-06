################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI/src/flashUtils.c \
../SPI/src/spi.c \
../SPI/src/spiFlash.c 

OBJS += \
./SPI/src/flashUtils.o \
./SPI/src/spi.o \
./SPI/src/spiFlash.o 

C_DEPS += \
./SPI/src/flashUtils.d \
./SPI/src/spi.d \
./SPI/src/spiFlash.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/src/%.o SPI/src/%.su: ../SPI/src/%.c SPI/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Freertos tests/NFC_and_Barcode_STM32/CMSIS/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/Core/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPL/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/NFC/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/LCD/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/USART/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPI/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-SPI-2f-src

clean-SPI-2f-src:
	-$(RM) ./SPI/src/flashUtils.d ./SPI/src/flashUtils.o ./SPI/src/flashUtils.su ./SPI/src/spi.d ./SPI/src/spi.o ./SPI/src/spi.su ./SPI/src/spiFlash.d ./SPI/src/spiFlash.o ./SPI/src/spiFlash.su

.PHONY: clean-SPI-2f-src

