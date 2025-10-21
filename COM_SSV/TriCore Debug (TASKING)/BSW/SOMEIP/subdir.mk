################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/SOMEIP/Someip.c" 

COMPILED_SRCS += \
"BSW/SOMEIP/Someip.src" 

C_DEPS += \
"./BSW/SOMEIP/Someip.d" 

OBJS += \
"BSW/SOMEIP/Someip.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/SOMEIP/Someip.src":"../BSW/SOMEIP/Someip.c" "BSW/SOMEIP/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/SOMEIP/Someip.o":"BSW/SOMEIP/Someip.src" "BSW/SOMEIP/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-SOMEIP

clean-BSW-2f-SOMEIP:
	-$(RM) ./BSW/SOMEIP/Someip.d ./BSW/SOMEIP/Someip.o ./BSW/SOMEIP/Someip.src

.PHONY: clean-BSW-2f-SOMEIP

