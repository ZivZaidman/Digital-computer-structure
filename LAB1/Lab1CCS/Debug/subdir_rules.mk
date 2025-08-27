################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
api.obj: ../api.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=16 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/Lab1CCS" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/source" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/header" --advice:power=all --define=__MSP430FG4616__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="api.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

bsp.obj: ../bsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=16 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/Lab1CCS" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/source" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/header" --advice:power=all --define=__MSP430FG4616__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bsp.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

halGPIO.obj: ../halGPIO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=16 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/Lab1CCS" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/source" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/header" --advice:power=all --define=__MSP430FG4616__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="halGPIO.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=16 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/Lab1CCS" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/source" --include_path="C:/Users/zivza/Ziv/English/Comp2/projects/LAB1/MSP430G2xx3/interruptDrivenFSMAbstractionLayers/cCode/header" --advice:power=all --define=__MSP430FG4616__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


