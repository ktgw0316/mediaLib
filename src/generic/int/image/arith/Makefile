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
# ident	"@(#)Makefile	9.3	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/arith

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =		mlib_ImageBlendColor.c \
                mlib_ImageBlend_Inp.c \
                mlib_ImageConstAdd_Inp.c \
                mlib_ImageConstDiv.c \
                mlib_ImageConstDivShift.c \
                mlib_ImageConstMul.c \
                mlib_ImageConstMulShift.c \
                mlib_ImageDivAlpha.c \
                mlib_ImageDivConstShift.c \
                mlib_ImageDivShift.c \
                mlib_ImageMulShift_Inp.c \
                mlib_ImageScalarBlend_Inp.c \
                mlib_ImageScale2.c \
                mlib_ImageSqrShift_Inp.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP =	mlib_ImageBlendColor_Fp.c \
		mlib_ImageBlendColor_Fp_Inp.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  =	mlib_i_ImageArith_Div_Tab_U8.c \
                mlib_i_ImageBlend.c \
                mlib_i_ImageBlendColor_f.c \
                mlib_i_ImageBlendColor_Inp.c \
                mlib_i_ImageBlendRGBA2ARGB.c \
                mlib_i_ImageBlendRGBA2BGRA.c \
                mlib_i_ImageConstAdd.c \
                mlib_i_ImageConstSub.c \
                mlib_i_ImageDivAlpha_f.c \
                mlib_i_ImageDivAlpha_Inp.c \
                mlib_i_ImageDivShiftU8.c \
                mlib_i_ImageMulAlpha.c \
                mlib_i_ImageMulAlpha_Inp.c \
                mlib_i_ImageMulShift.c \
                mlib_i_ImageScalarBlend.c \
                mlib_i_ImageScalarBlendFunc.c \
                mlib_i_ImageScale.c \
                mlib_i_ImageSqrShift.c \
                mlib_i_ImageSqrShift_f.c


include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./ \
		-I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(HOME_TXT)/../blend \
		-I$(HOME_TXT)/../copy \
		-I$(HOME_TXT)/../format \
		-I$(HOME_TXT)/../geom \
		-I$(HOME_TXT)/../spatial

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h


$(DIR_O)/mlib_ImageScale$(OBJ_EXT)              :  ./mlib_ImageScale.h
$(DIR_O)/mlib_ImageAbs$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAbs.h
$(DIR_O)/mlib_ImageBlendMulti$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../../include/mlib_Utils.h
$(DIR_O)/mlib_ImageBlendColor$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageBlendColor_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_ImageBlendColor_Fp_Inp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_ImageConstDiv$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageConstDivShift$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageConstMulShift$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O)/mlib_ImageDivAlpha$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageDivAlpha_f.h $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageDivShift$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageExpTbl$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageExpTbl.h
$(DIR_O)/mlib_ImageExp_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageExpTbl.h
$(DIR_O)/mlib_ImageLogTbl$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageLogTbl.h
$(DIR_O)/mlib_ImageLog_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageLogTbl.h
$(DIR_O)/mlib_ImageScale2$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O)/mlib_ImageScaleCheck$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O)/mlib_i_ImageBlendColor_f$(OBJ_EXT)     : ./mlib_i_ImageBlendColor.h
$(DIR_O)/mlib_i_ImageBlendColor_Inp$(OBJ_EXT)   : ./mlib_i_ImageBlendColor.h
$(DIR_O)/mlib_i_ImageSqrShift$(OBJ_EXT)         : ./mlib_i_ImageSqrShift.h
$(DIR_O)/mlib_i_ImageSqrShift_f$(OBJ_EXT)       : ./mlib_i_ImageSqrShift.h
$(DIR_O)/mlib_ImageConstDivShift$(OBJ_EXT)      : ./mlib_i_ImageConstDiv.h ./mlib_i_ImageArith_Div_Tab_U8.h
$(DIR_O)/mlib_i_ImageScale$(OBJ_EXT)            : ./mlib_ImageScale.h $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O)/mlib_i_ImageArith_Div_Tab_U8$(OBJ_EXT) : ./mlib_i_ImageArith_Div_Tab_U8.h
$(DIR_O)/mlib_ImageDivConstShift$(OBJ_EXT)      : ./mlib_i_ImageDivConstShift.h
$(DIR_O)/mlib_ImageConstMulShift$(OBJ_EXT)      : ./mlib_i_ImageConstMulShift.h $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O)/mlib_i_ImageScalarBlendFunc$(OBJ_EXT)  : ./mlib_i_ImageScalarBlendFunc.h $(HOME_TXT)/mlib_c_ImageScalarBlendFunc.h
$(DIR_O)/mlib_i_ImageDivAlpha_Inp$(OBJ_EXT)     : ./mlib_i_ImageArith_Div_Tab_U8.h
$(DIR_O)/mlib_i_ImageDivAlpha_f$(OBJ_EXT)       : ./mlib_i_ImageArith_Div_Tab_U8.h
$(DIR_O)/mlib_i_ImageDivShiftU8$(OBJ_EXT)       : ./mlib_i_ImageArith_Div_Tab_U8.h
$(DIR_O)/mlib_ImageConstDivShift$(OBJ_EXT)      : ./mlib_i_ImageConstDiv.h ./mlib_i_ImageArith_Div_Tab_U8.h
