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

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS   = mlib_SignalSplit.c \
          mlib_SignalMerge.c \
          mlib_SignalUpSampleFIR.c \
          mlib_SignalReSampleFIR.c

SRCS_FP = mlib_SignalUpSample_Fp.c \
          mlib_SignalConvertShift_Fp.c \
          mlib_SignalQuant_Fp.c \
          mlib_SignalLimit_Fp.c \
          mlib_SignalMerge_Fp.c \
          mlib_SignalSplit_Fp.c

SRCS_C  = mlib_c_SignalConvertShift.c \
          mlib_c_SignalDownSample.c \
          mlib_c_SignalDownSample_Fp.c \
          mlib_c_SignalLimit.c \
          mlib_c_SignalQuant.c \
          mlib_c_SignalUpSample.c \
          mlib_c_SignalUpSampleFIR_f.c \
          mlib_c_SignalConvertShift_Fp.c

SRCS_ALT_C = mlib_c_VectorZero.c \
             mlib_c_VectorMerge.c \
             mlib_c_VectorSplit.c \
             mlib_c_VectorCopy.c

SRCS = $(SRCS_C)
SRCS_ALT = $(SRCS_ALT_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/include \
            -I../filter

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

# define depends
__DEP_DIR_B = ./../../include

$(ALT_DIR_O)/mlib_c_VectorZero$(OBJ_EXT) : $(ALT_DIR)/../include/mlib_AlgebraUtil.h
$(ALT_DIR_O)/mlib_c_VectorSet$(OBJ_EXT)  : $(ALT_DIR)/../include/mlib_AlgebraUtil.h

$(DIR_O)/mlib_SignalConvertShift_Fp$(OBJ_EXT)   :  ./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalReSampleFIR$(OBJ_EXT)       :  ./mlib_SignalReSampleFIRFunc.h
$(DIR_O)/mlib_SignalUpSampleFIR$(OBJ_EXT)       :  ./../filter/mlib_SignalFIR.h ./mlib_SignalUpSampleFIRFunc.h
$(DIR_O)/mlib_c_SignalConvertShift$(OBJ_EXT)    :  ./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_c_SignalConvertShift_Fp$(OBJ_EXT) :  ./mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_c_SignalUpSampleFIR_f$(OBJ_EXT)   :  ./mlib_SignalUpSampleFIRFunc.h $(__DEP_DIR_B)/mlib_Utils.h
