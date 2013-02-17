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
# ident	"@(#)Makefile	9.18	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = algebra

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/vector

# common source under src/stdc/generic
UTILS_STDC = \
	mlib_AlgebraUtil.c      \
	mlib_VectorMulM.c


# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic

SRCS_STDC = \
	mlib_c_VectorAng.c              \
	mlib_c_VectorMag.c              \
	mlib_c_VectorDotProd32.c

# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2

SRCS_SSE2 = \
	mlib_s_VectorCopy.c             \
	mlib_s_VectorZero.c             \
	mlib_s_VectorSet.c              \
	mlib_s_VectorAdd.c		\
	mlib_s_VectorAddS.c		\
	mlib_s_VectorSub.c              \
	mlib_s_VectorSubS.c		\
	mlib_s_VectorMul.c              \
	mlib_s_VectorMulS.c		\
	mlib_s_VectorMulShift.c         \
	mlib_s_VectorMulSShift.c	\
	mlib_s_VectorMulM.c             \
	mlib_s_VectorMulMShift.c	\
	mlib_s_VectorMulSAdd.c          \
	mlib_s_VectorMulSAddC.c         \
	mlib_s_VectorScale.c		\
	mlib_s_VectorSumAbs.c           \
	mlib_s_VectorSumAbsDiff.c	\
	mlib_s_VectorMerge.c            \
	mlib_s_VectorSplit.c		\
	mlib_s_VectorConvert1.c         \
	mlib_s_VectorConvert2.c         \
	mlib_s_VectorConvert3.c		\
	mlib_s_VectorReverseByteOrder.c \
	mlib_s_VectorConj.c             \
	mlib_s_VectorConjRev.c          \
	mlib_s_VectorConjSymExt.c	\
	mlib_s_VectorMaximum.c          \
	mlib_s_VectorMinimum.c		\
	mlib_s_VectorMaximum_Fp.c    	\
	mlib_s_VectorMinimum_Fp.c	\
	mlib_s_VectorAve.c		\
	mlib_s_VectorDistance.c         \
	mlib_s_VectorNorm.c		\
	mlib_s_VectorDotProd8.c		\
	mlib_s_VectorDotProd16.c

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

include $(TOP_DIR)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)
INCLUDES += -I$(HOME_TXT)/../include
INCLUDES += -I$(HOME_TXT)/../vector
INCLUDES += -I../../sse
INCLUDES += -I../../../mmx/mmx

#
# dependencies
#

$(DIR_O)/mlib_s_VectorMulM$(OBJ_EXT)             :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h $(HOME_TXT)/./../../include/mlib_Utils.h $(HOME_TXT)/./mlib_VectorMulM.h $(HOME_TXT)/./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_s_VectorMulMShift$(OBJ_EXT)        :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h $(HOME_TXT)/./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_s_VectorScale$(OBJ_EXT)            :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_s_VectorMulSAdd$(OBJ_EXT)          :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_s_VectorSumAbs$(OBJ_EXT)           :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_s_VectorReverseByteOrder$(OBJ_EXT) :  $(HOME_TXT)/./mlib_VectorReverseByteOrder.h
$(DIR_O)/mlib_s_VectorConjRev$(OBJ_EXT)          :  $(HOME_TXT)/./mlib_VectorConjSymExt.h

$(DIR_O)/mlib_AlgebraUtil$(OBJ_EXT)              :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_VectorMulM$(OBJ_EXT)               :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h $(HOME_TXT)/./../../include/mlib_Utils.h $(HOME_TXT)/./mlib_VectorMulM.h  $(HOME_TXT)/./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_c_VectorAng$(OBJ_EXT)              :  $(HOME_TXT)/./../include/mlib_AlgebraUtil.h
