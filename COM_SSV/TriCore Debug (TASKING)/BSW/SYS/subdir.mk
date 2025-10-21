################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/SYS/Sys.c" 

COMPILED_SRCS += \
"BSW/SYS/Sys.src" 

C_DEPS += \
"./BSW/SYS/Sys.d" 

OBJS += \
"BSW/SYS/Sys.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/SYS/Sys.src":"../BSW/SYS/Sys.c" "BSW/SYS/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/SYS/Sys.o":"BSW/SYS/Sys.src" "BSW/SYS/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-SYS

clean-BSW-2f-SYS:
	-$(RM) ./BSW/SYS/Sys.d ./BSW/SYS/Sys.o ./BSW/SYS/Sys.src

.PHONY: clean-BSW-2f-SYS

