################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/STM/Stm.c" 

COMPILED_SRCS += \
"BSW/STM/Stm.src" 

C_DEPS += \
"./BSW/STM/Stm.d" 

OBJS += \
"BSW/STM/Stm.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/STM/Stm.src":"../BSW/STM/Stm.c" "BSW/STM/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/STM/Stm.o":"BSW/STM/Stm.src" "BSW/STM/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-STM

clean-BSW-2f-STM:
	-$(RM) ./BSW/STM/Stm.d ./BSW/STM/Stm.o ./BSW/STM/Stm.src

.PHONY: clean-BSW-2f-STM

