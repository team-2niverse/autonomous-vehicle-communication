################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/ULTRASONIC/Ultrasonic.c" 

COMPILED_SRCS += \
"BSW/ULTRASONIC/Ultrasonic.src" 

C_DEPS += \
"./BSW/ULTRASONIC/Ultrasonic.d" 

OBJS += \
"BSW/ULTRASONIC/Ultrasonic.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/ULTRASONIC/Ultrasonic.src":"../BSW/ULTRASONIC/Ultrasonic.c" "BSW/ULTRASONIC/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/ULTRASONIC/Ultrasonic.o":"BSW/ULTRASONIC/Ultrasonic.src" "BSW/ULTRASONIC/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-ULTRASONIC

clean-BSW-2f-ULTRASONIC:
	-$(RM) ./BSW/ULTRASONIC/Ultrasonic.d ./BSW/ULTRASONIC/Ultrasonic.o ./BSW/ULTRASONIC/Ultrasonic.src

.PHONY: clean-BSW-2f-ULTRASONIC

