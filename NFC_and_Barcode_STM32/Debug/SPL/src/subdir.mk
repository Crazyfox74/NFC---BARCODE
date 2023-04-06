################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPL/src/misc.c \
../SPL/src/stm32f10x_adc.c \
../SPL/src/stm32f10x_bkp.c \
../SPL/src/stm32f10x_can.c \
../SPL/src/stm32f10x_cec.c \
../SPL/src/stm32f10x_crc.c \
../SPL/src/stm32f10x_dac.c \
../SPL/src/stm32f10x_dbgmcu.c \
../SPL/src/stm32f10x_dma.c \
../SPL/src/stm32f10x_exti.c \
../SPL/src/stm32f10x_flash.c \
../SPL/src/stm32f10x_fsmc.c \
../SPL/src/stm32f10x_gpio.c \
../SPL/src/stm32f10x_i2c.c \
../SPL/src/stm32f10x_iwdg.c \
../SPL/src/stm32f10x_pwr.c \
../SPL/src/stm32f10x_rcc.c \
../SPL/src/stm32f10x_rtc.c \
../SPL/src/stm32f10x_sdio.c \
../SPL/src/stm32f10x_spi.c \
../SPL/src/stm32f10x_tim.c \
../SPL/src/stm32f10x_usart.c \
../SPL/src/stm32f10x_wwdg.c 

OBJS += \
./SPL/src/misc.o \
./SPL/src/stm32f10x_adc.o \
./SPL/src/stm32f10x_bkp.o \
./SPL/src/stm32f10x_can.o \
./SPL/src/stm32f10x_cec.o \
./SPL/src/stm32f10x_crc.o \
./SPL/src/stm32f10x_dac.o \
./SPL/src/stm32f10x_dbgmcu.o \
./SPL/src/stm32f10x_dma.o \
./SPL/src/stm32f10x_exti.o \
./SPL/src/stm32f10x_flash.o \
./SPL/src/stm32f10x_fsmc.o \
./SPL/src/stm32f10x_gpio.o \
./SPL/src/stm32f10x_i2c.o \
./SPL/src/stm32f10x_iwdg.o \
./SPL/src/stm32f10x_pwr.o \
./SPL/src/stm32f10x_rcc.o \
./SPL/src/stm32f10x_rtc.o \
./SPL/src/stm32f10x_sdio.o \
./SPL/src/stm32f10x_spi.o \
./SPL/src/stm32f10x_tim.o \
./SPL/src/stm32f10x_usart.o \
./SPL/src/stm32f10x_wwdg.o 

C_DEPS += \
./SPL/src/misc.d \
./SPL/src/stm32f10x_adc.d \
./SPL/src/stm32f10x_bkp.d \
./SPL/src/stm32f10x_can.d \
./SPL/src/stm32f10x_cec.d \
./SPL/src/stm32f10x_crc.d \
./SPL/src/stm32f10x_dac.d \
./SPL/src/stm32f10x_dbgmcu.d \
./SPL/src/stm32f10x_dma.d \
./SPL/src/stm32f10x_exti.d \
./SPL/src/stm32f10x_flash.d \
./SPL/src/stm32f10x_fsmc.d \
./SPL/src/stm32f10x_gpio.d \
./SPL/src/stm32f10x_i2c.d \
./SPL/src/stm32f10x_iwdg.d \
./SPL/src/stm32f10x_pwr.d \
./SPL/src/stm32f10x_rcc.d \
./SPL/src/stm32f10x_rtc.d \
./SPL/src/stm32f10x_sdio.d \
./SPL/src/stm32f10x_spi.d \
./SPL/src/stm32f10x_tim.d \
./SPL/src/stm32f10x_usart.d \
./SPL/src/stm32f10x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
SPL/src/%.o SPL/src/%.su: ../SPL/src/%.c SPL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Freertos tests/NFC_and_Barcode_STM32/CMSIS/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/Core/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPL/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/NFC/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/LCD/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/USART/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/SPI/inc" -I"D:/Freertos tests/NFC_and_Barcode_STM32/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-SPL-2f-src

clean-SPL-2f-src:
	-$(RM) ./SPL/src/misc.d ./SPL/src/misc.o ./SPL/src/misc.su ./SPL/src/stm32f10x_adc.d ./SPL/src/stm32f10x_adc.o ./SPL/src/stm32f10x_adc.su ./SPL/src/stm32f10x_bkp.d ./SPL/src/stm32f10x_bkp.o ./SPL/src/stm32f10x_bkp.su ./SPL/src/stm32f10x_can.d ./SPL/src/stm32f10x_can.o ./SPL/src/stm32f10x_can.su ./SPL/src/stm32f10x_cec.d ./SPL/src/stm32f10x_cec.o ./SPL/src/stm32f10x_cec.su ./SPL/src/stm32f10x_crc.d ./SPL/src/stm32f10x_crc.o ./SPL/src/stm32f10x_crc.su ./SPL/src/stm32f10x_dac.d ./SPL/src/stm32f10x_dac.o ./SPL/src/stm32f10x_dac.su ./SPL/src/stm32f10x_dbgmcu.d ./SPL/src/stm32f10x_dbgmcu.o ./SPL/src/stm32f10x_dbgmcu.su ./SPL/src/stm32f10x_dma.d ./SPL/src/stm32f10x_dma.o ./SPL/src/stm32f10x_dma.su ./SPL/src/stm32f10x_exti.d ./SPL/src/stm32f10x_exti.o ./SPL/src/stm32f10x_exti.su ./SPL/src/stm32f10x_flash.d ./SPL/src/stm32f10x_flash.o ./SPL/src/stm32f10x_flash.su ./SPL/src/stm32f10x_fsmc.d ./SPL/src/stm32f10x_fsmc.o ./SPL/src/stm32f10x_fsmc.su ./SPL/src/stm32f10x_gpio.d ./SPL/src/stm32f10x_gpio.o ./SPL/src/stm32f10x_gpio.su ./SPL/src/stm32f10x_i2c.d ./SPL/src/stm32f10x_i2c.o ./SPL/src/stm32f10x_i2c.su ./SPL/src/stm32f10x_iwdg.d ./SPL/src/stm32f10x_iwdg.o ./SPL/src/stm32f10x_iwdg.su ./SPL/src/stm32f10x_pwr.d ./SPL/src/stm32f10x_pwr.o ./SPL/src/stm32f10x_pwr.su ./SPL/src/stm32f10x_rcc.d ./SPL/src/stm32f10x_rcc.o ./SPL/src/stm32f10x_rcc.su ./SPL/src/stm32f10x_rtc.d ./SPL/src/stm32f10x_rtc.o ./SPL/src/stm32f10x_rtc.su ./SPL/src/stm32f10x_sdio.d ./SPL/src/stm32f10x_sdio.o ./SPL/src/stm32f10x_sdio.su ./SPL/src/stm32f10x_spi.d ./SPL/src/stm32f10x_spi.o ./SPL/src/stm32f10x_spi.su ./SPL/src/stm32f10x_tim.d ./SPL/src/stm32f10x_tim.o ./SPL/src/stm32f10x_tim.su ./SPL/src/stm32f10x_usart.d ./SPL/src/stm32f10x_usart.o ./SPL/src/stm32f10x_usart.su ./SPL/src/stm32f10x_wwdg.d ./SPL/src/stm32f10x_wwdg.o ./SPL/src/stm32f10x_wwdg.su

.PHONY: clean-SPL-2f-src

