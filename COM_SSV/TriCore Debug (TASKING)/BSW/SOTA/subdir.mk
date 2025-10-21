################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/SOTA/Sota.c" 

COMPILED_SRCS += \
"BSW/SOTA/Sota.src" 

C_DEPS += \
"./BSW/SOTA/Sota.d" 

OBJS += \
"BSW/SOTA/Sota.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/SOTA/Sota.src":"../BSW/SOTA/Sota.c" "BSW/SOTA/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/SOTA/Sota.o":"BSW/SOTA/Sota.src" "BSW/SOTA/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-SOTA

clean-BSW-2f-SOTA:
	-$(RM) ./BSW/SOTA/Sota.d ./BSW/SOTA/Sota.o ./BSW/SOTA/Sota.src

.PHONY: clean-BSW-2f-SOTA

