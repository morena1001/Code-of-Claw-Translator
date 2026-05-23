################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/arm_cfft_f32.c \
../Core/Src/arm_cfft_radix8_f32.c \
../Core/Src/arm_cmplx_mag_f32.c \
../Core/Src/arm_common_tables.c \
../Core/Src/arm_const_structs.c \
../Core/Src/arm_rfft_fast_f32.c \
../Core/Src/arm_rfft_fast_init_f32.c \
../Core/Src/arm_sin_f32.c \
../Core/Src/main.c \
../Core/Src/main2.c \
../Core/Src/main3.c \
../Core/Src/stm32f3xx_hal_msp.c \
../Core/Src/stm32f3xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f3xx.c 

S_UPPER_SRCS += \
../Core/Src/arm_bitreversal2.S 

OBJS += \
./Core/Src/arm_bitreversal2.o \
./Core/Src/arm_cfft_f32.o \
./Core/Src/arm_cfft_radix8_f32.o \
./Core/Src/arm_cmplx_mag_f32.o \
./Core/Src/arm_common_tables.o \
./Core/Src/arm_const_structs.o \
./Core/Src/arm_rfft_fast_f32.o \
./Core/Src/arm_rfft_fast_init_f32.o \
./Core/Src/arm_sin_f32.o \
./Core/Src/main.o \
./Core/Src/main2.o \
./Core/Src/main3.o \
./Core/Src/stm32f3xx_hal_msp.o \
./Core/Src/stm32f3xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f3xx.o 

S_UPPER_DEPS += \
./Core/Src/arm_bitreversal2.d 

C_DEPS += \
./Core/Src/arm_cfft_f32.d \
./Core/Src/arm_cfft_radix8_f32.d \
./Core/Src/arm_cmplx_mag_f32.d \
./Core/Src/arm_common_tables.d \
./Core/Src/arm_const_structs.d \
./Core/Src/arm_rfft_fast_f32.d \
./Core/Src/arm_rfft_fast_init_f32.d \
./Core/Src/arm_sin_f32.d \
./Core/Src/main.d \
./Core/Src/main2.d \
./Core/Src/main3.d \
./Core/Src/stm32f3xx_hal_msp.d \
./Core/Src/stm32f3xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.S Core/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@" "$<"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_CM4 -D__FPU_PRESENT=1U -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/arm_bitreversal2.d ./Core/Src/arm_bitreversal2.o ./Core/Src/arm_cfft_f32.cyclo ./Core/Src/arm_cfft_f32.d ./Core/Src/arm_cfft_f32.o ./Core/Src/arm_cfft_f32.su ./Core/Src/arm_cfft_radix8_f32.cyclo ./Core/Src/arm_cfft_radix8_f32.d ./Core/Src/arm_cfft_radix8_f32.o ./Core/Src/arm_cfft_radix8_f32.su ./Core/Src/arm_cmplx_mag_f32.cyclo ./Core/Src/arm_cmplx_mag_f32.d ./Core/Src/arm_cmplx_mag_f32.o ./Core/Src/arm_cmplx_mag_f32.su ./Core/Src/arm_common_tables.cyclo ./Core/Src/arm_common_tables.d ./Core/Src/arm_common_tables.o ./Core/Src/arm_common_tables.su ./Core/Src/arm_const_structs.cyclo ./Core/Src/arm_const_structs.d ./Core/Src/arm_const_structs.o ./Core/Src/arm_const_structs.su ./Core/Src/arm_rfft_fast_f32.cyclo ./Core/Src/arm_rfft_fast_f32.d ./Core/Src/arm_rfft_fast_f32.o ./Core/Src/arm_rfft_fast_f32.su ./Core/Src/arm_rfft_fast_init_f32.cyclo ./Core/Src/arm_rfft_fast_init_f32.d ./Core/Src/arm_rfft_fast_init_f32.o ./Core/Src/arm_rfft_fast_init_f32.su ./Core/Src/arm_sin_f32.cyclo ./Core/Src/arm_sin_f32.d ./Core/Src/arm_sin_f32.o ./Core/Src/arm_sin_f32.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main2.cyclo ./Core/Src/main2.d ./Core/Src/main2.o ./Core/Src/main2.su ./Core/Src/main3.cyclo ./Core/Src/main3.d ./Core/Src/main3.o ./Core/Src/main3.su ./Core/Src/stm32f3xx_hal_msp.cyclo ./Core/Src/stm32f3xx_hal_msp.d ./Core/Src/stm32f3xx_hal_msp.o ./Core/Src/stm32f3xx_hal_msp.su ./Core/Src/stm32f3xx_it.cyclo ./Core/Src/stm32f3xx_it.d ./Core/Src/stm32f3xx_it.o ./Core/Src/stm32f3xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f3xx.cyclo ./Core/Src/system_stm32f3xx.d ./Core/Src/system_stm32f3xx.o ./Core/Src/system_stm32f3xx.su

.PHONY: clean-Core-2f-Src

