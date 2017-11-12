################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/MMA8451Q.c \
../Sources/main.c \
../Sources/mqx_tasks.c 

OBJS += \
./Sources/Events.o \
./Sources/MMA8451Q.o \
./Sources/main.o \
./Sources/mqx_tasks.o 

C_DEPS += \
./Sources/Events.d \
./Sources/MMA8451Q.d \
./Sources/main.d \
./Sources/mqx_tasks.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Static_Code/System" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Static_Code/PDD" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Static_Code/IO_Map" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Sources" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Headers" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/Generated_Code" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/include" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/config" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/kernel" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/psp/cortex_m" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/psp/cortex_m/core/M0" -I"C:/Users/luka/workspace.kds/FRDM_KE06Z_app/MQXLITE/psp/cortex_m/compiler/cwgcc" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


