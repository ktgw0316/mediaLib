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
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/siggen

# C specific source under src/stdc/generic
SRCS_STDC  =

# common source under src/stdc/generic
UTILS_STDC =

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_SignalSineWave.c      \
            mlib_s_SignalSineWave_Fp.c   \
            mlib_s_SignalWhiteNoise.c    \
            mlib_s_SignalWhiteNoise_Fp.c \
            mlib_s_SignalGaussNoise.c    \
            mlib_s_SignalGaussNoise_Fp.c

# MMX specific source under src/x86/mmx
SRCS_MMX  =

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I../../sse -I../../../mmx/mmx

# define depends
$(DIR_O_STDC)/mlib_SignalWhiteNoise$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalSiggen.h
$(DIR_O_STDC)/mlib_SignalWhiteNoise_Fp$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalSiggen.h
$(DIR_O_STDC)/mlib_SignalGaussNoise_Fp$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalSiggen.h
$(DIR_O_STDC)/mlib_c_SignalGaussNoise$(OBJ_EXT)   :  $(HOME_TXT)/./mlib_SignalSiggen.h
$(DIR_O_STDC)/mlib_SignalSineWave_Fp$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalSiggen.h
$(DIR_O_STDC)/mlib_c_SignalSineWave$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalSiggen.h
