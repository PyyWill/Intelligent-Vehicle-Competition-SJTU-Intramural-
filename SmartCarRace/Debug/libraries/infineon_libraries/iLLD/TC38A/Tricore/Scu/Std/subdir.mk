################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.c \
../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.c \
../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.c \
../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.c \
../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.c 

COMPILED_SRCS += \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.src \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.src \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.src \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.src \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.src 

C_DEPS += \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.d \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.d \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.d \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.d \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.d 

OBJS += \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.o \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.o \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.o \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.o \
./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/%.src: ../libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/%.c libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fE:/GitRepository/Intelligent-Vehicle-Competition-SJTU-Intramural-/SmartCarRace/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=2 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/libraries\/infineon_libraries\/iLLD\/TC38A\/Tricore\/Scu\/Std\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/%.o: ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/%.src libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Scu-2f-Std

clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Scu-2f-Std:
	-$(RM) ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuCcu.src ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuEru.src ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuLbist.src ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuRcu.src ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/IfxScuWdt.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Scu-2f-Std

