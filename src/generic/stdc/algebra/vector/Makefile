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
MODNAME = algebra

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  = mlib_AlgebraUtil.c      \
         mlib_VectorMulM.c       \
         mlib_VectorMaximum.c    \
	 mlib_VectorMinimum.c    \
         mlib_VectorMulShift32.c


SRCS_C  = mlib_c_VectorAdd.c              \
          mlib_c_VectorAddS.c             \
          mlib_c_VectorAng.c              \
          mlib_c_VectorAve.c              \
          mlib_c_VectorConj.c             \
          mlib_c_VectorConjRev.c          \
          mlib_c_VectorConvert1.c         \
          mlib_c_VectorConvert2.c         \
          mlib_c_VectorConvert3.c         \
          mlib_c_VectorCopy.c             \
          mlib_c_VectorDistance.c         \
          mlib_c_VectorDotProd16.c        \
          mlib_c_VectorDotProd32.c        \
          mlib_c_VectorDotProd8.c         \
          mlib_c_VectorMag.c              \
          mlib_c_VectorMerge.c            \
          mlib_c_VectorMul.c              \
          mlib_c_VectorMulM.c             \
          mlib_c_VectorMulMShift.c        \
          mlib_c_VectorMulS.c             \
          mlib_c_VectorMulSAdd.c          \
          mlib_c_VectorMulSAddC.c         \
          mlib_c_VectorMulShift.c         \
          mlib_c_VectorMulSShift.c        \
          mlib_c_VectorNorm.c             \
          mlib_c_VectorScale.c            \
          mlib_c_VectorSet.c              \
          mlib_c_VectorSplit.c            \
          mlib_c_VectorSub.c              \
          mlib_c_VectorSubS.c             \
          mlib_c_VectorSumAbs.c           \
          mlib_c_VectorSumAbsDiff.c       \
          mlib_c_VectorZero.c             \
          mlib_c_VectorReverseByteOrder.c \
          mlib_c_VectorMaximum.c          \
          mlib_c_VectorMinimum.c


SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
$(DIR_O)/mlib_AlgebraUtil$(OBJ_EXT)              :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_VectorMulM$(OBJ_EXT)               :  ./../include/mlib_AlgebraUtil.h ./../../include/mlib_Utils.h ./mlib_VectorMulM.h  ./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_c_VectorAng$(OBJ_EXT)              :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorMaximum$(OBJ_EXT)          :  ./mlib_c_VectorMaximum.h
$(DIR_O)/mlib_c_VectorMinimum$(OBJ_EXT)          :  ./mlib_c_VectorMaximum.h
$(DIR_O)/mlib_c_VectorMul$(OBJ_EXT)              :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorMulM$(OBJ_EXT)             :  ./../include/mlib_AlgebraUtil.h ./../../include/mlib_Utils.h ./mlib_VectorMulM.h ./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_c_VectorMulMShift$(OBJ_EXT)        :  ./../include/mlib_AlgebraUtil.h ./mlib_VectorMulM_proto.h
$(DIR_O)/mlib_c_VectorMulS$(OBJ_EXT)             :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorMulSAdd$(OBJ_EXT)          :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorReverseByteOrder$(OBJ_EXT) :  ./mlib_VectorReverseByteOrder.h
$(DIR_O)/mlib_c_VectorScale$(OBJ_EXT)            :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorSet$(OBJ_EXT)              :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorSumAbs$(OBJ_EXT)           :  ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_VectorConjRev$(OBJ_EXT)          :  ./mlib_VectorConjSymExt.h

