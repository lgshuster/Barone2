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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=Lab5Application.c ../../../src/DCMotorDrive.c ../../../src/FeedbackControl.c ../../../src/FreeRunningTimer.c ../../../src/RotaryEncoder.c ../../../src/BOARD.c ../../../src/NonVolatileMemory.c ../../../src/ADCFilter.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Lab5Application.o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Lab5Application.o.d ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d ${OBJECTDIR}/_ext/1386528437/BOARD.o.d ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d ${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Lab5Application.o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o

# Source Files
SOURCEFILES=Lab5Application.c ../../../src/DCMotorDrive.c ../../../src/FeedbackControl.c ../../../src/FreeRunningTimer.c ../../../src/RotaryEncoder.c ../../../src/BOARD.c ../../../src/NonVolatileMemory.c ../../../src/ADCFilter.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/Lab5Application.o: Lab5Application.c  .generated_files/f8d372511082fe480f8e62ea31295cb79cf771dd.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Lab5Application.o.d 
	@${RM} ${OBJECTDIR}/Lab5Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/Lab5Application.o.d" -o ${OBJECTDIR}/Lab5Application.o Lab5Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o: ../../../src/DCMotorDrive.c  .generated_files/53f5fa777b138d7923e6bf2e4cc63b4a24fe318.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ../../../src/DCMotorDrive.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FeedbackControl.o: ../../../src/FeedbackControl.c  .generated_files/ea645482d3b5a245de87e2967594897cab6d3308.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ../../../src/FeedbackControl.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/95341d722e6008037c302c9973231e335be725b2.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/ba22808285f035c6088c7b056836c1cd4f0bd50f.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/3f87a4e2fec5cbc15d078f5cef29c2dc826fff28.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o: ../../../src/NonVolatileMemory.c  .generated_files/ca958f02728d0c1322cda9bd4b9e86a82a450d99.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d" -o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ../../../src/NonVolatileMemory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/ADCFilter.o: ../../../src/ADCFilter.c  .generated_files/fe3d731a1de81c7fcee4beeea3fcfdd49892c52.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d" -o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o ../../../src/ADCFilter.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/Lab5Application.o: Lab5Application.c  .generated_files/ee33fd387cff85101ed2a4ce326bc113f2691ef1.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Lab5Application.o.d 
	@${RM} ${OBJECTDIR}/Lab5Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/Lab5Application.o.d" -o ${OBJECTDIR}/Lab5Application.o Lab5Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o: ../../../src/DCMotorDrive.c  .generated_files/5ebf295e5d592c4cccc857194ac3d2ad017ffcb2.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ../../../src/DCMotorDrive.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FeedbackControl.o: ../../../src/FeedbackControl.c  .generated_files/967837cb9f395f872bca7f9c82581ae755224f2b.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ../../../src/FeedbackControl.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/c636e01c6587b72624d4b5183becf3e779d026a6.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/6218c9d27cba1268dcd16db779d5b19d6f41c5cf.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/998798e766acb32080a5995d07a1aad43c4746f5.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o: ../../../src/NonVolatileMemory.c  .generated_files/b648a778f37407b80a8b0e32a49aa5d9caa07464.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d" -o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ../../../src/NonVolatileMemory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/ADCFilter.o: ../../../src/ADCFilter.c  .generated_files/d3feabbbb2ec2019bb42df71d7ee460ea8a11bc9.flag .generated_files/1c47ea2ca314a49ac59bd2d3f26b17fb8c64a71d.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d" -o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o ../../../src/ADCFilter.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../src/Protocol.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\..\src\Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../src/Protocol.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\..\src\Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Lab5Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
