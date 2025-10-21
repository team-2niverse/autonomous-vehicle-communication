################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/UDS/Diagnostic.c" \
"../BSW/UDS/Uds.c" 

COMPILED_SRCS += \
"BSW/UDS/Diagnostic.src" \
"BSW/UDS/Uds.src" 

C_DEPS += \
"./BSW/UDS/Diagnostic.d" \
"./BSW/UDS/Uds.d" 

OBJS += \
"BSW/UDS/Diagnostic.o" \
"BSW/UDS/Uds.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/UDS/Diagnostic.src":"../BSW/UDS/Diagnostic.c" "BSW/UDS/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/UDS/Diagnostic.o":"BSW/UDS/Diagnostic.src" "BSW/UDS/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"BSW/UDS/Uds.src":"../BSW/UDS/Uds.c" "BSW/UDS/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/UDS/Uds.o":"BSW/UDS/Uds.src" "BSW/UDS/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-UDS

clean-BSW-2f-UDS:
	-$(RM) ./BSW/UDS/Diagnostic.d ./BSW/UDS/Diagnostic.o ./BSW/UDS/Diagnostic.src ./BSW/UDS/Uds.d ./BSW/UDS/Uds.o ./BSW/UDS/Uds.src

.PHONY: clean-BSW-2f-UDS

