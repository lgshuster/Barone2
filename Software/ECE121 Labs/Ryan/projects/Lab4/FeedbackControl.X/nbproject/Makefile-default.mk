#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED="C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/BOARD.c" "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/DCMotorDrive.c" "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FreeRunningTimer.c" "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/RotaryEncoder.c" "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FeedbackControl.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/2119251165/BOARD.o ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/2119251165/BOARD.o.d ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o.d ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o.d ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o.d ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/2119251165/BOARD.o ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o

# Source Files
SOURCEFILES=C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/BOARD.c C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/DCMotorDrive.c C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FreeRunningTimer.c C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/RotaryEncoder.c C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FeedbackControl.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX340F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/2119251165/BOARD.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/BOARD.c  .generated_files/49a2c9395e8fe0d7f08ebbab4bbd2860ce332309.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/BOARD.o.d" -o ${OBJECTDIR}/_ext/2119251165/BOARD.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/BOARD.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/DCMotorDrive.c  .generated_files/ffa718dd476c317c4cc334fb4491c956897a531b.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/DCMotorDrive.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/FreeRunningTimer.c  .generated_files/4622e529188d9ff9942c119e3f74d66debe4c3b.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FreeRunningTimer.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/RotaryEncoder.c  .generated_files/99badc96271660cf70720a21973113780039bcb9.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/RotaryEncoder.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/FeedbackControl.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/FeedbackControl.c  .generated_files/47a109f42ef528a716d1f72669719c3087ab393b.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FeedbackControl.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/2119251165/BOARD.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/BOARD.c  .generated_files/70e7efb49e817dce4220f9ac10cbd16e8e0abc70.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/BOARD.o.d" -o ${OBJECTDIR}/_ext/2119251165/BOARD.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/BOARD.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/DCMotorDrive.c  .generated_files/b1f5bb21bf58f1ec209f9c600b7d0c17a2600d13.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/2119251165/DCMotorDrive.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/DCMotorDrive.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/FreeRunningTimer.c  .generated_files/7b28695b45f3cd22e0f1875906527999bca7d504.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/2119251165/FreeRunningTimer.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FreeRunningTimer.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/RotaryEncoder.c  .generated_files/55ecd32c51c489d016491d24e35f902880646af9.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/2119251165/RotaryEncoder.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/RotaryEncoder.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2119251165/FeedbackControl.o: C\:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/FeedbackControl.c  .generated_files/1672839c97a280216543caa50229afc087bdcb8.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/2119251165" 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/2119251165/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/2119251165/FeedbackControl.o "C:/Users/Ryan/Documents/UCSC/Senior/ECE 121/cwong744/src/FeedbackControl.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/Protocol.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Users\Ryan\Documents\UCSC\Senior\ECE 121\cwong744\src\Protocol.X.a"      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Users/Ryan/Documents/UCSC/Senior/ECE\ 121/cwong744/src/Protocol.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Users\Ryan\Documents\UCSC\Senior\ECE 121\cwong744\src\Protocol.X.a"      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/FeedbackControl.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
