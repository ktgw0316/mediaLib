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
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
STDC_ALT_MOD = signal

ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/fourier
HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/fourier
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/fourier

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageDFT.c

# common alternative source under src/stdc/generic/
SRCS_STDC_ALT = mlib_SignalFFT_Frw.c \
                mlib_SignalFFT_Inv.c \
                mlib_SignalFFT_D64.c \
                mlib_SignalFFT_D64Disp.c \
                mlib_SignalFFT_D64Disp_8.c \
                mlib_SignalFFT_D64Disp_24.c \
                mlib_SignalFFT_D64Disp_32.c \
                mlib_SignalFFT_D64_1.c \
                mlib_SignalFFT_D64_2.c \
                mlib_SignalFFT_D64_3.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic
SRCS_STDC =

# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 =


# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

include $(TOP_DIR)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)
INCLUDES += -I$(HOME_TXT)/../include
INCLUDES += -I$(HOME_TXT)/../blend
INCLUDES += -I$(HOME_TXT)/../copy
INCLUDES += -I$(HOME_TXT)/../spatial
INCLUDES += -I$(ALT_DIR)
INCLUDES += -I$(ALT_DIR)/../../mmx
INCLUDES += -I../../sse
INCLUDES += -I$(HOME_ALT_TXT)


#
# dependencies
#

$(OBJS) $(OBJS_STDC_ALT) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageDFT$(OBJ_EXT) :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                    $(HOME_TXT)/mlib_ImageDFT.h \
                                    $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h \
                                    $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h

$(DIR_O_STDC_ALT)/mlib_SignalFFT_Frw$(OBJ_EXT) : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_Inv$(OBJ_EXT) : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_D64$(OBJ_EXT) : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_D64Disp$(OBJ_EXT) : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_D64_1$(OBJ_EXT)   : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_D64_2$(OBJ_EXT)   : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
$(DIR_O_STDC_ALT)/mlib_SignalFFT_D64_3$(OBJ_EXT)   : $(HOME_ALT_TXT)/mlib_SignalFFTUtil.h $(HOME_ALT_TXT)/mlib_SignalFFTSettings.h
