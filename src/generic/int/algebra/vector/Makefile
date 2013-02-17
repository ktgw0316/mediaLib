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
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.2	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = algebra

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/vector

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS = mlib_VectorMaximum.c 		\
		mlib_VectorMinimum.c	\
		mlib_VectorMulM.c		\
		mlib_VectorMulShift32.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  =	 mlib_i_VectorZero.c	\
		 mlib_i_VectorCopy.c	\
		 mlib_i_VectorAdd.c	\
		 mlib_i_VectorAddS.c	\
		 mlib_i_VectorMaximum.c \
		 mlib_i_VectorMinimum.c	\
		 mlib_i_VectorScale.c	\
		 mlib_i_VectorSet.c	\
		 mlib_i_VectorMag.c	\
		 mlib_i_VectorSub.c	\
		 mlib_i_VectorSubS.c	\
		 mlib_i_VectorSumAbs.c	\
		 mlib_i_VectorSumAbsDiff.c	\
		 mlib_i_VectorDotProd8.c	\
		 mlib_i_VectorDotProd16.c	\
		 mlib_i_VectorDistance.c	\
		 mlib_i_VectorNorm.c	\
		 mlib_i_VectorMul.c		\
		 mlib_i_VectorMulS.c		\
		 mlib_i_VectorMulM.c		\
		 mlib_i_VectorMulMShift.c	\
		 mlib_i_VectorMulSAdd.c		\
		 mlib_i_VectorMulSAddC.c	\
		 mlib_i_VectorMulShift.c	\
		 mlib_i_VectorMulSShift.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include

# define depends


$(DIR_O)/mlib_AlgebraUtil$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_VectorMulM$(OBJ_EXT)               :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h	\
						    $(HOME_TXT)/../../include/mlib_Utils.h	\
						    ./mlib_VectorMulM.h		\
						    $(HOME_TXT)/mlib_VectorMulM_proto.h
$(DIR_O)/mlib_i_VectorAng$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorMaximum$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_VectorMaximum.h
$(DIR_O)/mlib_i_VectorMinimum$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_VectorMaximum.h
$(DIR_O)/mlib_i_VectorMul$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorMulSShift$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorMulM$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h	\
						    $(HOME_TXT)/../../include/mlib_Utils.h	\
						    ./mlib_VectorMulM.h		\
						    $(HOME_TXT)/mlib_VectorMulM_proto.h
$(DIR_O)/mlib_i_VectorMulMShift$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h	\
						    $(HOME_TXT)/mlib_VectorMulM_proto.h
$(DIR_O)/mlib_i_VectorMulS$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorMulSAdd$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorScale$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O)/mlib_i_VectorSet$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_AlgebraUtil.h

