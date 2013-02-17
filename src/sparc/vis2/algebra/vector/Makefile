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

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/vector
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/vector

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_VectorAdd8.c \
         mlib_v_VectorAdd16.c \
         mlib_v_VectorAdd32.c \
         mlib_v_VectorAve.c \
         mlib_v_VectorConjRevC.c \
         mlib_v_VectorConjSymExtC.c \
         mlib_v_VectorSub8.c \
         mlib_v_VectorSub16.c \
         mlib_v_VectorSub32.c \
         mlib_v_VectorDotProd8C.c \
         mlib_v_VectorDotProd16C.c \
         mlib_v_VectorConvert1.c \
         mlib_v_VectorConvert2.c \
         mlib_v_VectorConvert3.c \
         mlib_v_VectorMag.c \
         mlib_v_VectorMulS.c \
         mlib_v_VectorMul.c \
         mlib_v_VectorMulSAdd.c \
         mlib_v_VectorMulSAddC.c \
         mlib_v_VectorMulShift.c \
         mlib_v_VectorMulSShift.c \
         mlib_v_VectorScale.c \
         mlib_v_VectorMerge.c \
         mlib_v_VectorSplit.c \
         mlib_v_VectorSumAbs32.c \
         mlib_v_VectorMaximum.c  \
         mlib_v_VectorMinimum.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include

# change compiler options
$(DIR_O)/mlib_v_VectorDistance_U8$(OBJ_EXT) := CFLAGS += -xprefetch=no
$(DIR_O)/mlib_v_VectorCopy$(OBJ_EXT) := CFLAGS += -xprefetch=no
$(DIR_O)/mlib_v_VectorZero$(OBJ_EXT) := CFLAGS += -xprefetch=no
$(DIR_O)/mlib_v_VectorSet$(OBJ_EXT)  := CFLAGS += -xprefetch=no
$(DIR_O)/mlib_v_VectorReverseByteOrder$(OBJ_EXT)  := CFLAGS += -xprefetch=no

# define depends

$(DIR_O)/mlib_AlgebraUtil$(OBJ_EXT) : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_VectorMulM$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_AlgebraUtil.h \
                                      $(HOME_TXT)/mlib_VectorMulM.h $(HOME_TXT)/mlib_VectorMulM_proto.h \
                                      $(TOP_DIR)/src/generic/stdc/include/mlib_Utils.h
$(DIR_O)/mlib_v_VectorAng$(OBJ_EXT)              : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorDotProd16$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorDotProd16C$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorDotProd8C$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMulM$(OBJ_EXT)             : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMulMShift$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMulS$(OBJ_EXT)             : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMulSAdd$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMulSAddC$(OBJ_EXT)         : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorNorm$(OBJ_EXT)             : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorSumAbs8$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorReverseByteOrder$(OBJ_EXT) : $(HOME_TXT)/mlib_VectorReverseByteOrder.h
$(DIR_O)/mlib_v_VectorMaximum$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_v_VectorMinimum$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_AlgebraUtil.h

$(DIR_O)/mlib_v_VectorMaximum$(OBJ_EXT)     :  ./mlib_v_VectorMaximum.h
$(DIR_O)/mlib_v_VectorMinimum$(OBJ_EXT)     :  ./mlib_v_VectorMaximum.h

$(DIR_O)/mlib_v_VectorConjSymExtC$(OBJ_EXT) : $(HOME_TXT)/mlib_VectorConjSymExt.h

