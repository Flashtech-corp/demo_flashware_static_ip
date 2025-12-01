################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/list.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/queue.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/tasks.c \
../ra/aws/FreeRTOS/FreeRTOS/Source/timers.c 

CREF += \
demo_flashware_static_ip.cref 

C_DEPS += \
./ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/list.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/queue.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/tasks.d \
./ra/aws/FreeRTOS/FreeRTOS/Source/timers.d 

OBJS += \
./ra/aws/FreeRTOS/FreeRTOS/Source/event_groups.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/list.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/queue.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/stream_buffer.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/tasks.o \
./ra/aws/FreeRTOS/FreeRTOS/Source/timers.o 

MAP += \
demo_flashware_static_ip.map 


# Each subdirectory must supply rules for building sources it contributes
ra/aws/FreeRTOS/FreeRTOS/Source/%.o: ../ra/aws/FreeRTOS/FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -w -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra_gen" -I"." -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra_cfg\\fsp_cfg\\bsp" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra_cfg\\fsp_cfg" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra_cfg\\aws" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\src" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\fsp\\inc" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\fsp\\inc\\api" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\fsp\\inc\\instances" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\fsp\\src\\rm_freertos_port" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\aws\\FreeRTOS\\FreeRTOS\\Source\\include" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\fsp\\src\\rm_freertos_plus_tcp" -I"D:\\Users\\ferdi.kurnaz\\e2_studio\\workspace\\demo_flashware_static_ip\\ra\\aws\\FreeRTOS\\FreeRTOS-Plus\\Source\\FreeRTOS-Plus-TCP\\source\\include" -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

