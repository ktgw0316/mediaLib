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
# ident	"@(#)Makefile	9.3	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image
STDC_ALT_MOD = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/fourier
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/fourier

UTILS_STDC = mlib_ImageDFT.c

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

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_ALT_TXT)

#
# define depends
#

$(OBJS) $(OBJS_STDC_ALT) $(OBJS_STDC) : $(MLIB_HOME)/src/generic/stdc/image/include/mlib_ImageCheck.h

$(DIR_O_STDC)/mlib_ImageDFT$(OBJ_EXT) :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
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



