################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/lwip-freertos-integration-master/arch/sys_arch.c" 

COMPILED_SRCS += \
"Libraries/lwip-freertos-integration-master/arch/sys_arch.src" 

C_DEPS += \
"./Libraries/lwip-freertos-integration-master/arch/sys_arch.d" 

OBJS += \
"Libraries/lwip-freertos-integration-master/arch/sys_arch.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/lwip-freertos-integration-master/arch/sys_arch.src":"../Libraries/lwip-freertos-integration-master/arch/sys_arch.c" "Libraries/lwip-freertos-integration-master/arch/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/lwip-freertos-integration-master/arch/sys_arch.o":"Libraries/lwip-freertos-integration-master/arch/sys_arch.src" "Libraries/lwip-freertos-integration-master/arch/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-lwip-2d-freertos-2d-integration-2d-master-2f-arch

clean-Libraries-2f-lwip-2d-freertos-2d-integration-2d-master-2f-arch:
	-$(RM) ./Libraries/lwip-freertos-integration-master/arch/sys_arch.d ./Libraries/lwip-freertos-integration-master/arch/sys_arch.o ./Libraries/lwip-freertos-integration-master/arch/sys_arch.src

.PHONY: clean-Libraries-2f-lwip-2d-freertos-2d-integration-2d-master-2f-arch

