################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/common.c \
../src/cr_startup_lpc176x.c \
../src/dac.c \
../src/lpc1769_init.c \
../src/main.c \
../src/pwm.c \
../src/qei.c \
../src/rit.c \
../src/uart.c 

OBJS += \
./src/adc.o \
./src/common.o \
./src/cr_startup_lpc176x.o \
./src/dac.o \
./src/lpc1769_init.o \
./src/main.o \
./src/pwm.o \
./src/qei.o \
./src/rit.o \
./src/uart.o 

C_DEPS += \
./src/adc.d \
./src/common.d \
./src/cr_startup_lpc176x.d \
./src/dac.d \
./src/lpc1769_init.d \
./src/main.d \
./src/pwm.d \
./src/qei.d \
./src/rit.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\spica\Documents\LPCXpresso_5.2.2_2108\workspace\CMSISv2p00_LPC17xx\inc" -O3 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc176x.o: ../src/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\spica\Documents\LPCXpresso_5.2.2_2108\workspace\CMSISv2p00_LPC17xx\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


