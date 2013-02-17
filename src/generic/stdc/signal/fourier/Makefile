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
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS   = mlib_SignalFFT_D64.c \
          mlib_SignalFFT_Inv.c \
          mlib_SignalFFT_Frw.c \
          mlib_SignalFFT_D64Disp.c    \
          mlib_SignalFFT_D64Disp_8.c  \
          mlib_SignalFFT_D64Disp_24.c \
          mlib_SignalFFT_D64Disp_32.c \
          mlib_SignalFFT_D64_1.c \
          mlib_SignalFFT_D64_2.c \
          mlib_SignalFFT_D64_3.c \
          mlib_SignalFFT_F32Disp.c \
          mlib_SignalFFT_S16Disp.c \
          mlib_SignalFFT_F32.c \
          mlib_SignalFFTMain.c

SRCS_C  = mlib_c_SignalFFTDisp.c \
          mlib_c_SignalFFTMain.c \
          mlib_c_SignalFFTW_1.c \
          mlib_c_SignalFFTW_2.c \
          mlib_c_SignalFFTW_3.c \
          mlib_c_SignalFFTW_4.c \
          mlib_c_SignalFFT_1.c \
          mlib_c_SignalFFT_2.c \
          mlib_c_SignalFFT_3.c \
          mlib_c_SignalFFT_4.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends

$(DIR_O)/mlib_SignalFFT_D64$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp$(OBJ_EXT)    :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_24$(OBJ_EXT) :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_32$(OBJ_EXT) :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_8$(OBJ_EXT)  :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_1$(OBJ_EXT)      :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_2$(OBJ_EXT)      :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_3$(OBJ_EXT)      :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_F32$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_F32Disp$(OBJ_EXT)    :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_S16Disp$(OBJ_EXT)    :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_Frw$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_Inv$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFTMain$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTDisp$(OBJ_EXT)      :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTMain$(OBJ_EXT)      :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTW_1$(OBJ_EXT)       :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTW_2$(OBJ_EXT)       :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTW_3$(OBJ_EXT)       :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFTW_4$(OBJ_EXT)       :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFT_1$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFT_2$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFT_3$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalFFT_4$(OBJ_EXT)        :  ./mlib_SignalFFTSettings.h ./mlib_SignalFFTUtil.h
