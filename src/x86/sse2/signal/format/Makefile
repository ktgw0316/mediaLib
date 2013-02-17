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
# ident	"@(#)Makefile	9.10	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/x86/sse2/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/format

UTILS_STDC   = mlib_SignalSplit.c \
          mlib_SignalMerge.c \
          mlib_SignalUpSampleFIR.c \
          mlib_SignalReSampleFIR.c

SRCS_STDC_FP = mlib_SignalUpSample_Fp.c \
          mlib_SignalQuant_Fp.c \
          mlib_SignalMerge_Fp.c \
          mlib_SignalSplit_Fp.c

SRCS_STDC  = mlib_c_SignalQuant.c


# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_SignalConvertShift.c \
	    mlib_s_SignalConvertShift_S16.c \
	    mlib_s_SignalConvertShift_S32.c \
	    mlib_s_SignalConvertShift_S8.c  \
	    mlib_s_SignalConvertShift_U8.c  \
	    mlib_s_SignalConvertShift_Fp.c  \
        mlib_s_SignalDownSample.c       \
        mlib_s_SignalDownSample_Fp.c    \
        mlib_s_SignalLimit.c            \
        mlib_s_SignalLimit_Fp.c         \
        mlib_s_SignalUpSampleFIR_f.c    \
        mlib_s_SignalUpSample.c

SRCS = $(SRCS_SSE2)

SRCS_ALT = mlib_s_VectorZero.c	\
	   mlib_s_VectorCopy.c	\
	   mlib_s_VectorMerge.c \
           mlib_s_VectorSplit.c


HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/include \
            -I$(HOME_TXT)/../filter -I$(HOME_INCLUDE)/../../sse \
	    -I$(HOME_TXT)/../../../../x86/sse2/sse \
	    -I$(HOME_TXT)/../../../../x86/mmx/mmx

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

# define depends
__DEP_DIR_B = $(HOME_TXT)/./../../include

$(DIR_O_STDC)/mlib_s_SignalConvertShift_Fp$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_s_SignalConvertShift$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_s_SignalConvertShift_S16$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_s_SignalConvertShift_S32$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_s_SignalConvertShift_S8$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O_STDC)/mlib_s_SignalConvertShift_U8$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h

$(DIR_O_STDC)/mlib_SignalReSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/./mlib_SignalReSampleFIRFunc.h
$(DIR_O_STDC)/mlib_SignalUpSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/./../filter/mlib_SignalFIR.h $(HOME_TXT)/./mlib_SignalUpSampleFIRFunc.h
$(DIR_O_STDC)/mlib_c_SignalUpSampleFIR_f$(OBJ_EXT)   :  $(HOME_TXT)/./mlib_SignalUpSampleFIRFunc.h $(__DEP_DIR_B)/mlib_Utils.h
