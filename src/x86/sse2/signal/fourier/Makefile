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
# ident	"@(#)Makefile	9.7	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/fourier

UTILS_STDC   = mlib_SignalFFT_D64.c \
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

SRCS_STDC  = mlib_c_SignalFFTDisp.c \
	  mlib_c_SignalFFTMain.c \
	  mlib_c_SignalFFTW_4.c \
          mlib_c_SignalFFT_4.c

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_SignalFFTDisp.c \
	    mlib_s_SignalFFTMain.c \
	    mlib_s_SignalFFT_2Lay.c \
	    mlib_s_SignalFFT_First.c \
	    mlib_s_SignalFFT_Tbl.c \
	    mlib_s_SignalFFT_1.c \
	    mlib_s_SignalFFT_2.c \
	    mlib_s_SignalFFT_3.c \
	    mlib_s_SignalFFTW_1.c \
	    mlib_s_SignalFFTW_2.c \
	    mlib_s_SignalFFTW_3.c

# SSE2 header files under src/x86/sse2
HEADER_SSE2 = mlib_s_SignalFFTDisp.h \
	      mlib_s_SignalFFTSettings.h \
	      mlib_s_SignalFFTUtil.h \
	      mlib_s_SignalFFT_2_Macros.h \
	      mlib_s_SignalFFT_3_Macros.h \
	      mlib_s_SignalFFT_Tbl.h \
	      mlib_s_SignalFFT_x_Macros.h \
	      mlib_s_sse_int.h

#SRCS = $($(TARCH)_SRCS)
SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends

$(DIR_O_STDC)/mlib_SignalFFT_D64$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64Disp$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64Disp_24$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64Disp_32$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64Disp_8$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64_1$(OBJ_EXT)      :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64_2$(OBJ_EXT)      :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_D64_3$(OBJ_EXT)      :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_F32$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_F32Disp$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_S16Disp$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_Frw$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFT_Inv$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalFFTMain$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_c_SignalFFTDisp$(OBJ_EXT)      :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_s_SignalFFTDisp$(OBJ_EXT)      :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFTMain$(OBJ_EXT)      :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFTW_1$(OBJ_EXT)       :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFTW_2$(OBJ_EXT)       :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFTW_3$(OBJ_EXT)       :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_c_SignalFFTW_4$(OBJ_EXT)       :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_s_SignalFFT_1$(OBJ_EXT)        :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFT_2$(OBJ_EXT)        :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFT_3$(OBJ_EXT)        :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFT_2Lay$(OBJ_EXT)     :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFT_First$(OBJ_EXT)    :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_s_SignalFFT_Tbl$(OBJ_EXT)      :  ${HEADER_SSE2}
$(DIR_O_STDC)/mlib_c_SignalFFT_4$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalFFTSettings.h $(HOME_TXT)/./mlib_SignalFFTUtil.h
