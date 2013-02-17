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
# ident	"@(#)Makefile	9.3	07/10/08 SMI"
#

TOP_DIR = ../../../../..
MODNAME = algebra

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  = mlib_MatrixAdd.c \
         mlib_MatrixAddS.c \
         mlib_MatrixAve.c \
         mlib_MatrixSub.c \
         mlib_MatrixSubS.c \
         mlib_MatrixScale.c \
         mlib_MatrixMul.c \
         mlib_MatrixMulS.c \
         mlib_MatrixMulShift.c \
         mlib_MatrixMulShiftC.c \
         mlib_MatrixMulSShift.c \
         mlib_MatrixMaximum.c   \
         mlib_MatrixMinimum.c \
         mlib_MatrixUnit.c

SRCS_C  = mlib_c_MatrixMul.c \
          mlib_c_MatrixMulC.c \
          mlib_c_MatrixMulShift.c \
          mlib_c_MatrixMulShiftC.c \
          mlib_c_MatrixTranspose.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
MLIB_UTILS_H=$(TOP_DIR)/src/generic/stdc/include/mlib_Utils.h
MATRIXMUL_H=./mlib_MatrixMul.h ./../include/mlib_AlgebraUtil.h $(MLIB_UTILS_H)

$(DIR_O)/mlib_MatrixMul$(OBJ_EXT)         : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixMulShift$(OBJ_EXT)    : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixMulShiftC$(OBJ_EXT)   : $(MATRIXMUL_H)
$(DIR_O)/mlib_c_MatrixMul$(OBJ_EXT)       : $(MATRIXMUL_H)
$(DIR_O)/mlib_c_MatrixMulC$(OBJ_EXT)      : $(MATRIXMUL_H)
$(DIR_O)/mlib_c_MatrixMulShift$(OBJ_EXT)  : $(MATRIXMUL_H)
$(DIR_O)/mlib_c_MatrixMulShiftC$(OBJ_EXT) : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixUnit$(OBJ_EXT)        : ./mlib_MatrixUnit.h
$(DIR_O)/mlib_MatrixMaximum$(OBJ_EXT)     : ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_MatrixMinimum$(OBJ_EXT)     : ./../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_c_MatrixTranspose$(OBJ_EXT) : ./mlib_MatrixTranspose.h
