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
# ident	"@(#)Makefile	9.8	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/window

UTILS_STDC  = mlib_SignalWindowUtil.c

SRCS_STDC_FP = mlib_SignalWindow_Fp.c       \
          mlib_SignalMulBlackman_Fp.c       \
          mlib_SignalMulHamming_Fp.c        \
          mlib_SignalMulHanning_Fp.c        \
          mlib_SignalMulKaiser_Fp.c         \
          mlib_SignalMulBartlett_Fp.c       \
          mlib_SignalMulBartlett1_Fp.c      \
          mlib_SignalMulRectangular_Fp.c    \
          mlib_SignalMulWindow_Fp.c

SRCS_STDC  = mlib_c_SignalMulRectangular.c

SRCS_MMX =

SRCS_SSE2 = \
          mlib_s_SignalWindow.c             \
          mlib_s_SignalMulBartlett.c        \
          mlib_s_SignalMulWindow.c          \
          mlib_s_SignalMulHamming.c         \
          mlib_s_SignalMulBlackman.c        \
          mlib_s_SignalMulHanning.c         \
          mlib_s_SignalWinTab.c             \
          mlib_s_SignalMulKaiser.c

SRCS_ALT = $(SRCS_MMX)

SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends

$(DIR_O_STDC)/mlib_SignalMulBartlett1_Fp$(OBJ_EXT)   : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalMulBlackman_Fp$(OBJ_EXT)    : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalMulHamming_Fp$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalMulHanning_Fp$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalMulKaiser_Fp$(OBJ_EXT)      : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalWindow_Fp$(OBJ_EXT)         : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h
$(DIR_O_STDC)/mlib_SignalWindowUtil$(OBJ_EXT)        : $(HOME_TXT)/./mlib_SignalWindowUtil.h

$(DIR_O_STDC)/mlib_s_SignalWindow$(OBJ_EXT)          : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
	                                                  $(HOME_TXT)/./mlib_SignalWindow.h \
													  ./mlib_s_SignalWinTab.h
$(DIR_O_STDC)/mlib_s_SignalMulBartlett$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h \
													  ./mlib_s_SignalWinTab.h
$(DIR_O_STDC)/mlib_s_SignalMulBlackman$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h\
													  ./mlib_s_SignalWinTab.h
$(DIR_O_STDC)/mlib_s_SignalMulHamming$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                 $(HOME_TXT)/./mlib_SignalWindow.h \
													  ./mlib_s_SignalWinTab.h
$(DIR_O_STDC)/mlib_s_SignalMulHanning$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                 $(HOME_TXT)/./mlib_SignalWindow.h \
													  ./mlib_s_SignalWinTab.h
$(DIR_O_STDC)/mlib_s_SignalMulKaiser$(OBJ_EXT)       : $(HOME_TXT)/./mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/./mlib_SignalWindow.h \
													  ./mlib_s_SignalWinTab.h
$(DIR_O)/mlib_s_SignalWinTab$(OBJ_EXT)     :     ./mlib_s_SignalWinTab.h
