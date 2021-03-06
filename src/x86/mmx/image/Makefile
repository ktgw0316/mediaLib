#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.2	07/11/05 SMI"
#

TOP_DIR = ../../../..
MODNAME = image
ALT_MOD = signal
LIBNAME = mlib_$(MODNAME)

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

include $(PROJECT_ROOT)/make/main.cfg
include $(CONFIG.$(HOST_ARCH_NAME).PROJECT)
include $(CONFIG.$(TARCH).OVERRIDES)
include $(CONFIG.$(TARCH).C)

VERSION_TARGET = mmx

SUB_DIRS = fourier arith blend color common copy format geom logic radio spatial stat

OBJS_ALT = mlib_SignalFFT_Frw \
           mlib_SignalFFT_Inv \
           mlib_SignalFFT_D64 \
           mlib_SignalFFT_D64Disp \
           mlib_SignalFFT_D64Disp_8 \
           mlib_SignalFFT_D64Disp_24 \
           mlib_SignalFFT_D64Disp_32 \
           mlib_SignalFFT_D64_1 \
           mlib_SignalFFT_D64_2 \
           mlib_SignalFFT_D64_3

OBJS_ALT_V = $(OBJS_ALT)

OBJS =  $(DIR_O)/*$(OBJ_EXT) $(DIR_SYS)/*$(OBJ_EXT) \
        $(OBJS_ALT:%=$(DIR_O)/../$(ALT_MOD)/%$(OBJ_EXT))

MAPFILE_DIR = $(TOP_DIR)/src/generic/stdc/$(MODNAME)

MAPFILES = $(MAPFILE_DIR)/mapfile $(MAPFILE_DIR)/../mapfile $(MAPFILE_DIR)/mapfile_common $(MAPFILE_DIR)/../signal/mapfile_common

include $(PROJECT_ROOT)/make/rules/directory.cfg
include $(CONFIG.$(TARCH).MAKE)

include $(PROJECT_ROOT)/make/rules/lib.cfg
