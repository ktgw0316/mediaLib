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
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/filter

# common source under src/stdc/generic
UTILS_STDC  =  \

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic
SRCS_STDC  =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2  = mlib_s_SignalEmphasize.c      \
             mlib_s_SignalEmphasize_Fp.c   \
             mlib_s_SignalVisVersion.c     \
             mlib_s_SignalLMSFilter.c      \
             mlib_s_SignalLMSFilter_Fp.c   \
             mlib_s_SignalIIR_Biquad.c     \
             mlib_s_SignalIIR_Biquad_Fp.c  \
             mlib_s_SignalFIR.c            \
             mlib_s_SignalFIR_f.c          \
             mlib_s_SignalNLMSFilter.c     \
             mlib_s_SignalNLMSFilter_Fp.c  \
             mlib_s_SignalIIR_P4.c         \
             mlib_s_SignalIIR_P4_Fp.c

# MMX specific source under src/x86/mmx
SRCS_MMX  =

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse -I../../sse -I../../../mmx/mmx

# define depends
__DEP_DIR_B = $(HOME_TXT)/./../../include

$(DIR_O_STDC)/mlib_SignalFIR$(OBJ_EXT)           :  $(HOME_TXT)/./mlib_SignalFIR.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_SignalFIR_f$(OBJ_EXT)         :  $(HOME_TXT)/./mlib_SignalFIR.h
$(DIR_O_STDC)/mlib_SignalIIR_P4$(OBJ_EXT)        :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_SignalLMSFilter$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalFIR.h $(HOME_TXT)/./mlib_SignalLMS.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_SignalNLMSFilter$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalFIR.h $(HOME_TXT)/./mlib_SignalLMS.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_SignalLMSFilter_Fp$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalFIR.h $(HOME_TXT)/./mlib_SignalLMS.h
$(DIR_O_STDC)/mlib_SignalNLMSFilter_Fp$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalFIR.h $(HOME_TXT)/./mlib_SignalLMS.h
$(DIR_O_STDC)/mlib_c_SignalVisVersion$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalVisVersion.h
