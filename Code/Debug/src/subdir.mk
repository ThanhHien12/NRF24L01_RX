################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/STDIO_UART.c \
../src/mainRX.c \
../src/myLCD.c \
../src/nrf24l01.c \
../src/spi.c 

OBJS += \
./src/STDIO_UART.o \
./src/mainRX.o \
./src/myLCD.o \
./src/nrf24l01.o \
./src/spi.o 

C_DEPS += \
./src/STDIO_UART.d \
./src/mainRX.d \
./src/myLCD.d \
./src/nrf24l01.d \
./src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Thanh Hien\Documents\GitHub\Avr_Nrf\Code\firmware_RX\inc" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=7372800UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


