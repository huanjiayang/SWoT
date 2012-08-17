###############################################################################
#
# MODULE:   Makefile
#
# DESCRIPTION: Makefie for the ZBPro Home Sensor Demo
# 
###############################################################################
# 
# This software is owned by Jennic and/or its supplier and is protected
# under applicable copyright laws. All rights are reserved. We grant You,
# and any third parties, a license to use this software solely and
# exclusively on Jennic products. You, and any third parties must reproduce
# the copyright and warranty notice and any other legend of ownership on each
# copy or partial copy of the software.
# 
# THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
# ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
# BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
# INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
# 
# Copyright Jennic Ltd 2009. All rights reserved
#
###############################################################################
# Subversion variables
# $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Application_Notes/JN-AN-1122-ZBPro-Home-Sensor-Demo/Tags/ZBPRO-HOME-SENSOR-DEMO_1v7_RC4/AN1122_ZBP_SensorNode_SED/Build/Makefile $
# $Revision: 29653 $
# $LastChangedBy: nxp29741 $
# $LastChangedDate: 2011-03-01 13:37:20 +0000 (Tue, 01 Mar 2011) $
# $Id: Makefile 29653 2011-03-01 13:37:20Z nxp29741 $ 
#
###############################################################################

# Application target name

TARGET = AN1122_ZBP_SensorNode_SED

###############################################################################
#User definable make parameters that may be overwritten from the command line

# Default target device is the JN5148

JENNIC_CHIP ?= JN5148

###############################################################################
# Default DK2 development kit target hardware

JENNIC_PCB ?= DEVKIT2

###############################################################################
# Select the network stack (e.g. MAC, ZBPro)

JENNIC_STACK ?= ZBPro

###############################################################################
# ZBPro Stack specific options
ZBPRO_DEVICE_TYPE = ZED

###############################################################################
# Debug options - define DEBUG to enable
# DEBUG ?=HW
#
# Define which UART to use for HW debug
# DEBUG_PORT ?= UART1

###############################################################################
# Define TRACE to use with DBG module
# By default TRACE and OVERLAYS have been turned off
# Set them to 1 to enable TRACE and overlays.
TRACE ?=1
OVERLAY_BUILD ?=0

ifeq ($(OVERLAY_BUILD),1)
CFLAGS += -DOVERLAYS_BUILD
ZBPRO_OVERLAYS = 1
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/OVLY/Include
APPLIBS += OVLY
endif

ifeq ($(TRACE), 1)
CFLAGS  += -DDBG_ENABLE
$(info Building trace version ...)

# Enable any debug output here:
CFLAGS  += -DTRACE_APP=1
CFLAGS  += -DTRACE_APP_BUTTON=0
CFLAGS  += -DTRACE_APP_LED=0
CFLAGS  += -DTRACE_APP_SAMPLE=1
CFLAGS  += -DTRACE_APP_SYSCON=1
CFLAGS  += -DTRACE_APP_PWRM=1
CFLAGS  += -DTRACE_OVERLAYS=0
endif

###############################################################################
# Path definitions

# Use if application directory contains multiple targets
SDK_BASE_DIR   	 	= $(abspath ../../../..)
APP_BASE           	= $(abspath ../..)
APP_BLD_DIR			= $(APP_BASE)/$(TARGET)/Build
APP_SRC_DIR 	   	= $(APP_BASE)/$(TARGET)/Source
APP_COMMON_SRC_DIR  = $(APP_BASE)/Common/Source

###############################################################################
# Application Source files

# Note: Path to source file is found using vpath below, so only .c filename is required
APPSRC  = os_gen.c
APPSRC += os_irq.S
APPSRC += os_irq_alignment.S
APPSRC += os_irq_buserror.S
APPSRC += os_irq_illegalinstruction.S
APPSRC += os_irq_stackoverflowexception.S
APPSRC += os_irq_unimplementedmodule.S
APPSRC += pdum_gen.c
APPSRC += zps_gen.c
APPSRC += app_timer_driver.c
APPSRC += app_start.c
APPSRC += app_sample_SED.c
APPSRC += app_syscon.c
APPSRC += app_buttons.c
APPSRC += app_led.c
APPSRC += app_sensor_node_SED.c
APPSRC += serial.c
APPSRC += serialq.c
APPSRC += uart.c

APP_ZPSCFG = ZBProHomeSensorDemo.zpscfg

###############################################################################
# Standard Application header search paths

INCFLAGS += -I$(APP_SRC_DIR)
INCFLAGS += -I$(APP_SRC_DIR)/..
INCFLAGS += -I$(APP_COMMON_SRC_DIR)

# Application specific include files
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/JennicLogo/Include
INCFLAGS += -I$(COMPONENTS_BASE_DIR)/Utilities/Include

###############################################################################
# Application libraries
# Specify additional Component libraries

#APPLIBS += 

###############################################################################

# You should not need to edit below this line

###############################################################################
###############################################################################
# Configure for the selected chip or chip family

include $(SDK_BASE_DIR)/Chip/Common/Build/config.mk
include $(SDK_BASE_DIR)/Platform/Common/Build/Config.mk
include $(SDK_BASE_DIR)/Stack/Common/Build/config.mk

###############################################################################

TEMP = $(APPSRC:.c=.o)
APPOBJS = $(TEMP:.S=.o)

###############################################################################
# Application dynamic dependencies

APPDEPS = $(APPOBJS:.o=.d)

###############################################################################
# Linker

# Add application libraries before chip specific libraries to linker so
# symbols are resolved correctly (i.e. ordering is significant for GCC)

LDLIBS := $(addsuffix _$(JENNIC_CHIP_FAMILY),$(APPLIBS)) $(LDLIBS)

###############################################################################
# Dependency rules

.PHONY: all clean
# Path to directories containing application source 
vpath % $(APP_SRC_DIR):$(APP_COMMON_SRC_DIR)


all: $(TARGET)_$(JENNIC_CHIP)$(BIN_SUFFIX).bin

-include $(APPDEPS)
%.d:
	rm -f $*.o

$(APP_SRC_DIR)/os_gen.c $(APP_SRC_DIR)/os_gen.h $(APP_SRC_DIR)/os_irq.S $(APP_SRC_DIR)/os_irq_alignment.S $(APP_SRC_DIR)/os_irq_buserror.S $(APP_SRC_DIR)/os_irq_illegalinstruction.S $(APP_SRC_DIR)/os_irq_stackoverflowexception.S $(APP_SRC_DIR)/os_irq_unimplementedmodule.S: $(APP_SRC_DIR)/App_$(TARGET)_$(JENNIC_CHIP_FAMILY).oscfgdiag $(OSCONFIG) 
	$(info Configuring the OS ...)
	$(OSCONFIG) -f "$(shell cygpath -w "$<")" -o "$(shell cygpath -w "$(APP_SRC_DIR)")"
	@echo

$(APP_SRC_DIR)/pdum_gen.c $(APP_SRC_DIR)/pdum_gen.h: $(APP_COMMON_SRC_DIR)/$(APP_ZPSCFG) $(PDUMCONFIG)
	$(info Configuring the PDUM ...)
	$(PDUMCONFIG) -z $(TARGET) -f "$(shell cygpath -w "$<")" -o "$(shell cygpath -w "$(APP_SRC_DIR)")"
	@echo

$(APP_SRC_DIR)/zps_gen.c $(APP_SRC_DIR)/zps_gen.h: $(APP_COMMON_SRC_DIR)/$(APP_ZPSCFG) $(ZPSCONFIG)
	$(info Configuring the Zigbee Protocol Stack ...)
	$(ZPSCONFIG) -n $(TARGET) -t $(JENNIC_CHIP) -l "$(shell cygpath -w $(COMPONENTS_BASE_DIR)/Library/libZPSNWK_$(JENNIC_CHIP_FAMILY).a)" -a "$(shell cygpath -w $(COMPONENTS_BASE_DIR)/Library/libZPSAPL_$(JENNIC_CHIP_FAMILY).a)" -f "$(shell cygpath -w "$<")" -o "$(shell cygpath -w "$(APP_SRC_DIR)")"
	@echo

%.o: %.S
	$(info Assembling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(CFLAGS) $(INCFLAGS) $< -MD -MF $*.d -MP
	@echo

%.o: %.c 
	$(info Compiling $< ...)
	$(CC) -c -o $(subst Source,Build,$@) $(CFLAGS) $(INCFLAGS) $< -MD -MF $*.d -MP
	@echo

$(TARGET)_$(JENNIC_CHIP).elf: $(APPOBJS) $(addsuffix _$(JENNIC_CHIP_FAMILY).a,$(addprefix $(COMPONENTS_BASE_DIR)/Library/lib,$(APPLIBS))) 
	$(info Linking $@ ...)
	$(CC) -Wl,--unique -Wl,-r -Wl,-u_AppColdStart -Wl,-u_AppWarmStart $(LDFLAGS) -TAppBuild_$(JENNIC_CHIP)_Blank.ld -o temp.elf -Wl,--start-group $(APPOBJS) $(addprefix -l,$(LDLIBS)) -Wl,--end-group 
	$(TOOL_BASE_DIR)/OVLYUtils/bin/OVLY_Relocation temp.elf $(OVERLAYS)
	$(info Final Link $@ ...)
	$(CC) -Wl,--gc-sections -Wl,-u_AppColdStart -Wl,-u_AppWarmStart $(LDFLAGS) -TAppBuild_$(JENNIC_CHIP).ld $(addsuffix .ld,$(addprefix -TApp_Overlay_,$(OVERLAYS))) -TAppBuild_$(JENNIC_CHIP)_End.ld -o $@ temp.elf -Wl,-Map,$(TARGET)_$(JENNIC_CHIP)$(BIN_SUFFIX).map 
	$(info Generating checksums $@ ...)
	$(TOOL_BASE_DIR)/OVLYUtils/bin/OVLY_Checksum $@ $(addprefix .,$(OVERLAYS))
	rm -f temp.elf
	$(TOOL_BASE_DIR)/OVLYUtils/bin/image_size.sh $(TARGET)_$(JENNIC_CHIP)$(BIN_SUFFIX).map
	@echo

$(TARGET)_$(JENNIC_CHIP)$(BIN_SUFFIX).bin: $(TARGET)_$(JENNIC_CHIP)$(BIN_SUFFIX).elf 
	$(info Generating binary ...)
	$(OBJCOPY) -j .flashheader -j .oad -j .mac -j .heap_location -j .rtc_clt  -j .rodata -j .data -j .text -j .overlay_location $(addprefix -j .,$(OVERLAYS)) -j .bss -j .heap -j .stack -S -O binary $< $@
	
###############################################################################

clean:
	rm -f $(APPOBJS) $(APPDEPS) $(TARGET)_$(JENNIC_CHIP)*.bin $(TARGET)_$(JENNIC_CHIP)*.elf $(TARGET)_$(JENNIC_CHIP)*.map
	rm -f $(APP_SRC_DIR)/os_gen.c $(APP_SRC_DIR)/os_gen.h $(APP_SRC_DIR)/os_irq*.S $(APP_SRC_DIR)/pdum_gen.* $(APP_SRC_DIR)/zps_gen*.*

###############################################################################
