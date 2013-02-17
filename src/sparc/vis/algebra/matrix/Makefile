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
# ident	"@(#)Makefile	9.3	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = algebra

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/matrix

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_V  = mlib_v_MatrixMul.c \
          mlib_v_MatrixMul_U8.c \
          mlib_v_MatrixMul_U8C.c \
          mlib_v_MatrixMul_S8.c \
          mlib_v_MatrixMul_S8C.c \
          mlib_v_MatrixMulC.c \
          mlib_v_MatrixMulShift.c \
          mlib_v_MatrixMulShiftC.c \
          mlib_v_MatrixTranspose.c \
          mlib_v_MatrixTranspose_Inp.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/../include -I$(HOME_TXT)

# define depends
MLIB_UTILS_H=$(TOP_DIR)/src/generic/stdc/include/mlib_Utils.h
MATRIXMUL_H=$(HOME_TXT)/mlib_MatrixMul.h $(HOME_TXT)/../include/mlib_AlgebraUtil.h $(MLIB_UTILS_H)

$(DIR_O)/mlib_MatrixMul$(OBJ_EXT)         : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixMulShift$(OBJ_EXT)    : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixMulShiftC$(OBJ_EXT)   : $(MATRIXMUL_H)
$(DIR_O)/mlib_MatrixUnit$(OBJ_EXT)        : $(HOME_TXT)/mlib_MatrixUnit.h

$(DIR_O)/mlib_v_MatrixMul$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_AlgebraUtil.h $(HOME_TXT)/mlib_MatrixTranspose.h
$(DIR_O)/mlib_v_MatrixMulC$(OBJ_EXT)      : $(HOME_TXT)/../include/mlib_AlgebraUtil.h $(HOME_TXT)/mlib_MatrixTranspose.h
$(DIR_O)/mlib_v_MatrixMulShift$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_AlgebraUtil.h $(HOME_TXT)/mlib_MatrixTranspose.h
$(DIR_O)/mlib_v_MatrixMulShiftC$(OBJ_EXT) : $(HOME_TXT)/../include/mlib_AlgebraUtil.h $(HOME_TXT)/mlib_MatrixTranspose.h

$(DIR_O)/mlib_v_MatrixTranspose$(OBJ_EXT) : $(HOME_TXT)/mlib_MatrixTranspose.h
$(DIR_O)/mlib_v_MatrixTranspose_Inp$(OBJ_EXT) : $(HOME_TXT)/mlib_MatrixTranspose.h
