################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.c" 

COMPILED_SRCS += \
"BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.src" 

C_DEPS += \
"./BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.d" 

OBJS += \
"BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.o" 


# Each subdirectory must supply rules for building sources it contributes
"BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.src":"../BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.c" "BSW/GTM_ATOM_PWM/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/COM_SSV/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.o":"BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.src" "BSW/GTM_ATOM_PWM/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-BSW-2f-GTM_ATOM_PWM

clean-BSW-2f-GTM_ATOM_PWM:
	-$(RM) ./BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.d ./BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.o ./BSW/GTM_ATOM_PWM/Gtm_Atom_Pwm.src

.PHONY: clean-BSW-2f-GTM_ATOM_PWM

