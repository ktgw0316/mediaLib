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

UTILS  = mlib_SignalWindowUtil.c            \
         mlib_SignalMulKaiser.c

SRCS_FP = mlib_SignalWindow_Fp.c            \
          mlib_SignalMulBlackman_Fp.c       \
          mlib_SignalMulHamming_Fp.c        \
          mlib_SignalMulHanning_Fp.c        \
          mlib_SignalMulKaiser_Fp.c         \
          mlib_SignalMulBartlett_Fp.c       \
          mlib_SignalMulBartlett1_Fp.c      \
          mlib_SignalMulRectangular_Fp.c    \
          mlib_SignalMulWindow_Fp.c

SRCS_C  = mlib_c_SignalMulBartlett.c        \
          mlib_c_SignalMulBlackman.c        \
          mlib_c_SignalMulHamming.c         \
          mlib_c_SignalMulHanning.c         \
          mlib_c_SignalMulRectangular.c     \
          mlib_c_SignalMulWindow.c          \
          mlib_c_SignalWindow.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends

$(DIR_O)/mlib_SignalMulBartlett1_Fp$(OBJ_EXT)   : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulBlackman_Fp$(OBJ_EXT)    : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulHamming_Fp$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulHanning_Fp$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulKaiser_Fp$(OBJ_EXT)      : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalWindow_Fp$(OBJ_EXT)         : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulKaiser$(OBJ_EXT)         : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_SignalWindowUtil$(OBJ_EXT)        : ./mlib_SignalWindowUtil.h

$(DIR_O)/mlib_c_SignalWindow$(OBJ_EXT)          : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_c_SignalMulBartlett$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_c_SignalMulBlackman$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                  ./mlib_SignalWindow.h
$(DIR_O)/mlib_c_SignalMulHamming$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                 ./mlib_SignalWindow.h
$(DIR_O)/mlib_c_SignalMulHanning$(OBJ_EXT)     : ./mlib_SignalWindowUtil.h \
                                                 ./mlib_SignalWindow.h
