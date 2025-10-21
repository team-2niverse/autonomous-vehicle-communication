################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/CAN/Can.c" 

COMPILED_SRCS += \
"BSW/CAN/Can.src" 

C_DEPS += \
"./BSW/CAN/Can.d" 

OBJS += \
"BSW/CAN/Can.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/CAN/Can.src":"../BSW/CAN/Can.c" "BSW/CAN/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/CAN/Can.o":"BSW/CAN/Can.src" "BSW/CAN/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-CAN

clean-BSW-2f-CAN:
	-$(RM) ./BSW/CAN/Can.d ./BSW/CAN/Can.o ./BSW/CAN/Can.src

.PHONY: clean-BSW-2f-CAN

