################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/TOF/Tof.c" 

COMPILED_SRCS += \
"BSW/TOF/Tof.src" 

C_DEPS += \
"./BSW/TOF/Tof.d" 

OBJS += \
"BSW/TOF/Tof.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/TOF/Tof.src":"../BSW/TOF/Tof.c" "BSW/TOF/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/TOF/Tof.o":"BSW/TOF/Tof.src" "BSW/TOF/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-TOF

clean-BSW-2f-TOF:
	-$(RM) ./BSW/TOF/Tof.d ./BSW/TOF/Tof.o ./BSW/TOF/Tof.src

.PHONY: clean-BSW-2f-TOF

