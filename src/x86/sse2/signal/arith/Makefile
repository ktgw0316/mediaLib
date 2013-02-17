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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/arith

UTILS_STDC  = mlib_SignalAutoCorrel_Fp.c \
         mlib_SignalCrossCorrel_Fp.c \
         mlib_SignalMul_Inp.c \
         mlib_SignalMulS_Fp.c \
         mlib_SignalMul_Fp.c \
         mlib_SignalMulSAdd_Fp.c \
         mlib_SignalConv.c

SRCS_STDC  = \
          mlib_c_SignalConv_f.c

SRCS_MMX =

SRCS_SSE2 = \
		  mlib_s_SignalMul.c \
		  mlib_s_SignalMulS.c \
          mlib_s_SignalMulSAdd.c \
          mlib_s_SignalMulSShift.c \
          mlib_s_SignalMulShift.c \
          mlib_s_SignalMulSShiftAdd.c \
          mlib_s_SignalAutoCorrel.c \
          mlib_s_SignalCrossCorrel.c \

SRCS_ALT = $(SRCS_MMX)

SRCS = $(SRCS_SSE2)

#SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../fourier -I$(HOME_INCLUDE)/../../sse

# define depends
__DEP_DIR_C = $(HOME_TXT)/./../../include

$(DIR_O_STDC)/mlib_SignalAutoCorrel_Fp$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalCorrel.h
$(DIR_O_STDC)/mlib_SignalConv$(OBJ_EXT)           :  $(HOME_TXT)/./mlib_SignalConv.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O_STDC)/mlib_SignalCrossCorrel_Fp$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalCorrel.h
$(DIR_O_STDC)/mlib_c_SignalAutoCorrel$(OBJ_EXT)   :  $(HOME_TXT)/./mlib_SignalCorrel.h
$(DIR_O_STDC)/mlib_c_SignalConv_f$(OBJ_EXT)       :  $(HOME_TXT)/./mlib_SignalConv.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O_STDC)/mlib_c_SignalCrossCorrel$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalCorrel.h
$(DIR_O_STDC)/mlib_c_SignalMulSAdd$(OBJ_EXT)      :  $(__DEP_DIR_C)/mlib_Utils.h
