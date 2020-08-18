################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
G8RTOS/%.obj: ../G8RTOS/%.s $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs910/ccs/ccs_base/arm/include" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/G8RTOS" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/inc" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/src" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/DriverLib" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/board" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink/include" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink/source" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/SL_Common" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/spi_cc3100" --include_path="C:/ti/ccs910/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers" --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="G8RTOS/$(basename $(<F)).d_raw" --obj_directory="G8RTOS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

G8RTOS/%.obj: ../G8RTOS/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs910/ccs/ccs_base/arm/include" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/G8RTOS" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/inc" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/src" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/BoardSupportPackage/DriverLib" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/board" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/cc3100_usage" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink/include" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/simplelink/source" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/SL_Common" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers/CC3100SupportPackage/spi_cc3100" --include_path="C:/ti/ccs910/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Danny/Documents/CCS Workspaces/Side Projects/Yerpers" --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="G8RTOS/$(basename $(<F)).d_raw" --obj_directory="G8RTOS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


