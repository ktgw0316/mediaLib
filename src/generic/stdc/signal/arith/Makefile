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

UTILS  = mlib_SignalAutoCorrel_Fp.c \
         mlib_SignalCrossCorrel_Fp.c \
         mlib_SignalMul_Inp.c \
         mlib_SignalMulS_Fp.c \
         mlib_SignalMul_Fp.c \
         mlib_SignalMulSAdd_Fp.c \
         mlib_SignalConv.c

SRCS_C  = mlib_c_SignalAutoCorrel.c \
          mlib_c_SignalCrossCorrel.c \
          mlib_c_SignalMul.c \
          mlib_c_SignalMulS.c \
          mlib_c_SignalMulSAdd.c \
          mlib_c_SignalMulSShift.c \
          mlib_c_SignalMulSShiftAdd.c \
          mlib_c_SignalMulShift.c \
          mlib_c_SignalConv_f.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../fourier

# define depends
__DEP_DIR_C = ./../../include

$(DIR_O)/mlib_SignalAutoCorrel_Fp$(OBJ_EXT)  :  ./mlib_SignalCorrel.h
$(DIR_O)/mlib_SignalConv$(OBJ_EXT)           :  ./mlib_SignalConv.h ./../fourier/mlib_SignalFFTSettings.h \
                                                ./../fourier/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O)/mlib_SignalCrossCorrel_Fp$(OBJ_EXT) :  ./mlib_SignalCorrel.h
$(DIR_O)/mlib_c_SignalAutoCorrel$(OBJ_EXT)   :  ./mlib_SignalCorrel.h
$(DIR_O)/mlib_c_SignalConv_f$(OBJ_EXT)       :  ./mlib_SignalConv.h ./../fourier/mlib_SignalFFTSettings.h \
                                                ./../fourier/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O)/mlib_c_SignalCrossCorrel$(OBJ_EXT)  :  ./mlib_SignalCorrel.h
$(DIR_O)/mlib_c_SignalMulSAdd$(OBJ_EXT)      :  $(__DEP_DIR_C)/mlib_Utils.h
