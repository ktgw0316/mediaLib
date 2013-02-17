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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.3	07/10/09 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image
ALT_MOD = signal

ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/fourier

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  = mlib_ImageDFT.c

SRCS_ALT = mlib_SignalFFT_Frw.c \
           mlib_SignalFFT_Inv.c \
           mlib_SignalFFT_D64.c \
           mlib_SignalFFT_D64Disp.c \
           mlib_SignalFFT_D64Disp_8.c \
           mlib_SignalFFT_D64Disp_24.c \
           mlib_SignalFFT_D64Disp_32.c \
           mlib_SignalFFT_D64_1.c \
           mlib_SignalFFT_D64_2.c \
           mlib_SignalFFT_D64_3.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

INCLUDES += -I$(ALT_DIR) -I./../include

# define depends

$(OBJS) : ./../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageDFT$(OBJ_EXT) :  ./../include/mlib_ImageDivTables.h \
                                    ./mlib_ImageDFT.h \
                                    $(ALT_DIR)/mlib_SignalFFTSettings.h \
                                    $(ALT_DIR)/mlib_SignalFFTUtil.h

$(ALT_DIR_O)/mlib_SignalFFT_D64$(OBJ_EXT)        :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64Disp$(OBJ_EXT)    :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64Disp_24$(OBJ_EXT) :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64Disp_32$(OBJ_EXT) :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64Disp_8$(OBJ_EXT)  :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64_1$(OBJ_EXT)      :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64_2$(OBJ_EXT)      :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_D64_3$(OBJ_EXT)      :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_Frw$(OBJ_EXT)        :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h
$(ALT_DIR_O)/mlib_SignalFFT_Inv$(OBJ_EXT)        :  $(ALT_DIR)/mlib_SignalFFTSettings.h $(ALT_DIR)/mlib_SignalFFTUtil.h

