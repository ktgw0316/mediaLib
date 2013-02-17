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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/arith

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_V  = mlib_v_ImageAbs.c \
          mlib_v_ImageAdd.c \
          mlib_v_ImageAddFunc.c \
          mlib_v_ImageSqrShift.c \
          mlib_v_ImageAve.c \
          mlib_v_ImageAveFunc.c \
          mlib_v_ImageBlend.c \
          mlib_v_ImageBlendFunc.c \
          mlib_v_ImageBlendSAFunc.c \
          mlib_v_ImageBlendRGBA2ARGB.c \
          mlib_v_ImageBlendRGBA2BGRA.c \
          mlib_v_ImageBlendColor_U8.c  \
          mlib_v_ImageBlendColor_S16.c \
          mlib_v_ImageBlendColor_U16.c \
          mlib_v_ImageBlendColor_Inp.c \
          mlib_v_ImageConstAdd.c \
          mlib_v_ImageConstAddFunc.c \
          mlib_v_ImageConstSub.c \
          mlib_v_ImageConstSubFunc.c \
          mlib_v_ImageDivAlphaFunc.c \
          mlib_v_ImageDivAlphaFunc_Inp.c \
          mlib_v_ImageDivAlphaTabl.c \
          mlib_v_ImageDivAlpha_Inp.c \
          mlib_v_ImageDivShiftU8.c \
          mlib_v_ImageMax.c \
          mlib_v_ImageMaxFunc.c \
          mlib_v_ImageMin.c \
          mlib_v_ImageMinFunc.c \
          mlib_v_ImageMulAlpha.c \
          mlib_v_ImageMulAlphaFunc.c \
          mlib_v_ImageMulAlphaFunc_Inp.c \
          mlib_v_ImageMulAlpha_Inp.c \
          mlib_v_ImageMulAlpha_U8.c  \
          mlib_v_ImageMulAlpha_S16.c \
          mlib_v_ImageMulAlpha_U16.c \
          mlib_v_ImageMulShift.c \
          mlib_v_ImageMulShiftFunc.c \
          mlib_v_ImageMulShiftFuncS16.c \
          mlib_v_ImageScalarBlend.c \
          mlib_v_ImageScalarBlendFunc.c \
          mlib_v_ImageScale.c \
          mlib_v_ImageScale2Func.c \
          mlib_v_ImageScaleExtFunc.c \
          mlib_v_ImageScaleFunc.c \
          mlib_v_ImageScaleFunc_U16.c \
          mlib_v_ImageScaleIntFunc.c \
          mlib_v_ImageScale_Fp.c \
          mlib_v_ImageSqrShift_f.c \
          mlib_v_ImageSub.c \
          mlib_v_ImageSubFunc.c \
          mlib_v_ImageSub_Inp.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../geom -I./../format \
             -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_TXT)/../spatial \
             -I$(HOME_TXT)/../copy

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h


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


$(DIR_O)/mlib_v_ImageAdd$(OBJ_EXT)              :  ./mlib_v_ImageAddFunc.h
$(DIR_O)/mlib_v_ImageAddFunc$(OBJ_EXT)          :  ./mlib_v_ImageAddFunc.h
$(DIR_O)/mlib_v_ImageAve$(OBJ_EXT)              :  ./mlib_v_ImageAveFunc.h
$(DIR_O)/mlib_v_ImageAveFunc$(OBJ_EXT)          :  ./mlib_v_ImageAveFunc.h
$(DIR_O)/mlib_v_ImageBlend$(OBJ_EXT)            :  ./mlib_v_ImageBlendFunc.h
$(DIR_O)/mlib_v_ImageBlendFunc$(OBJ_EXT)        :  ./mlib_v_ImageBlendFunc.h
$(DIR_O)/mlib_v_ImageBlendRGBA2ARGB$(OBJ_EXT)   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   ./../format/mlib_v_ImageChannelCopy.h
$(DIR_O)/mlib_v_ImageBlendRGBA2BGRA$(OBJ_EXT)   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   ./../format/mlib_v_ImageChannelCopy.h
$(DIR_O)/mlib_v_ImageBlendSAFunc$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   ./mlib_v_ImageBlendFunc.h
$(DIR_O)/mlib_v_ImageBlendColor_Inp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_v_ImageBlendColor_S16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlendColor_U16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlendColor_U8$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendColor.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageConstAdd$(OBJ_EXT)         :  ./mlib_v_ImageConstAddFunc.h
$(DIR_O)/mlib_v_ImageConstAddFunc$(OBJ_EXT)     :  ./mlib_v_ImageConstAddFunc.h
$(DIR_O)/mlib_v_ImageConstSub$(OBJ_EXT)         :  ./mlib_v_ImageConstSubFunc.h
$(DIR_O)/mlib_v_ImageConstSubFunc$(OBJ_EXT)     :  ./mlib_v_ImageConstSubFunc.h
$(DIR_O)/mlib_v_ImageDivAlphaFunc$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_v_ImageDivAlpha.h
$(DIR_O)/mlib_v_ImageDivAlphaFunc_Inp$(OBJ_EXT) :  ./mlib_v_ImageDivAlpha.h
$(DIR_O)/mlib_v_ImageDivAlphaTabl$(OBJ_EXT)     :  ./mlib_v_ImageDivAlpha.h
$(DIR_O)/mlib_v_ImageDivAlpha_Inp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageDivAlpha_f.h \
                                                   ./mlib_v_ImageDivAlpha.h
$(DIR_O)/mlib_v_ImageDivShiftU8$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O)/mlib_v_ImageMax$(OBJ_EXT)              :  ./mlib_v_ImageMaxFunc.h
$(DIR_O)/mlib_v_ImageMaxFunc$(OBJ_EXT)          :  ./mlib_v_ImageMaxFunc.h
$(DIR_O)/mlib_v_ImageMin$(OBJ_EXT)              :  ./mlib_v_ImageMinFunc.h
$(DIR_O)/mlib_v_ImageMinFunc$(OBJ_EXT)          :  ./mlib_v_ImageMinFunc.h
$(DIR_O)/mlib_v_ImageMulAlpha$(OBJ_EXT)         :  ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlphaFunc$(OBJ_EXT)     :  ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlphaFunc_Inp$(OBJ_EXT) :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlpha_Inp$(OBJ_EXT)     :  ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlpha_S16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlpha_U16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulAlpha_U8$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_v_ImageMulAlpha.h
$(DIR_O)/mlib_v_ImageMulShift$(OBJ_EXT)         :  ./mlib_v_ImageMulShift.h
$(DIR_O)/mlib_v_ImageMulShiftFunc$(OBJ_EXT)     :  ./mlib_v_ImageMulShift.h
$(DIR_O)/mlib_v_ImageMulShiftFuncS16$(OBJ_EXT)  :  ./mlib_v_ImageMulShift.h
$(DIR_O)/mlib_v_ImageScalarBlend$(OBJ_EXT)      :  ./mlib_v_ImageScalarBlendFunc.h
$(DIR_O)/mlib_v_ImageScalarBlendFunc$(OBJ_EXT)  :  ./mlib_v_ImageScalarBlendFunc.h
$(DIR_O)/mlib_v_ImageScale$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageScale2.h ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageScale2Func$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageScale2.h ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageScaleExtFunc$(OBJ_EXT)     :  ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageScaleFunc$(OBJ_EXT)        :  ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageScaleFunc_U16$(OBJ_EXT)    :  ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageScaleIntFunc$(OBJ_EXT)     :  ./mlib_v_ImageScale.h
$(DIR_O)/mlib_v_ImageSqrShift$(OBJ_EXT)         :  ./mlib_v_ImageSqrShift_f.h
$(DIR_O)/mlib_v_ImageSqrShift_f$(OBJ_EXT)       :  ./mlib_v_ImageSqrShift_f.h
$(DIR_O)/mlib_v_ImageSub$(OBJ_EXT)              :  ./mlib_v_ImageSubFunc.h
$(DIR_O)/mlib_v_ImageSubFunc$(OBJ_EXT)          :  ./mlib_v_ImageSubFunc.h
