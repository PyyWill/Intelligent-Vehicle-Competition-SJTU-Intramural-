################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/encoder.c \
../code/imagprocess.c \
../code/led.c \
../code/motor.c \
../code/servo.c 

COMPILED_SRCS += \
./code/encoder.src \
./code/imagprocess.src \
./code/led.src \
./code/motor.src \
./code/servo.src 

C_DEPS += \
./code/encoder.d \
./code/imagprocess.d \
./code/led.d \
./code/motor.d \
./code/servo.d 

OBJS += \
./code/encoder.o \
./code/imagprocess.o \
./code/led.o \
./code/motor.o \
./code/servo.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fE:/GitRepository/Intelligent-Vehicle-Competition-SJTU-Intramural-/SmartCarRace/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=2 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/encoder.d ./code/encoder.o ./code/encoder.src ./code/imagprocess.d ./code/imagprocess.o ./code/imagprocess.src ./code/led.d ./code/led.o ./code/led.src ./code/motor.d ./code/motor.o ./code/motor.src ./code/servo.d ./code/servo.o ./code/servo.src

.PHONY: clean-code

