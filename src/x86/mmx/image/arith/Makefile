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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/arith

UTILS_STDC = mlib_ImageAbs.c \
             mlib_ImageBlendColor.c \
             mlib_ImageConstDiv.c \
             mlib_ImageConstDivShift.c \
             mlib_ImageConstMul.c \
             mlib_ImageConstMulShift.c \
             mlib_ImageDivConstShift.c \
             mlib_ImageDivShift.c \
             mlib_ImageDivAlpha.c \
             mlib_ImageExp.c \
             mlib_ImageExpTbl.c \
             mlib_ImageInvert.c \
             mlib_ImageLog.c \
             mlib_ImageLogTbl.c \
             mlib_ImageScale2.c \
             mlib_ImageScaleCheck.c \
             mlib_ImageMax_Inp.c \
             mlib_ImageInvert_Inp.c \
             mlib_ImageMin_Inp.c \
             mlib_ImageConstSub_Inp.c \
             mlib_ImageMulShift_Inp.c \
             mlib_ImageScalarBlend_Inp.c \
             mlib_ImageScale_Inp.c \
             mlib_ImageSqrShift_Inp.c \
             mlib_ImageConstAdd_Inp.c \
             mlib_ImageBlend_Inp.c \
             mlib_ImageBlendMulti.c \
             mlib_ImageAve_Inp.c \
             mlib_ImageAdd_Inp.c \
             mlib_ImageAbs_Inp.c

SRCS_STDC_FP = mlib_ImageAbs_Fp.c \
               mlib_ImageAbs_Fp_Inp.c \
               mlib_ImageAdd_Fp_Inp.c \
               mlib_ImageAve_Fp_Inp.c \
               mlib_ImageBlend_Fp_Inp.c \
               mlib_ImageBlendColor_Fp.c \
               mlib_ImageBlendColor_Fp_Inp.c \
               mlib_ImageConstAdd_Fp_Inp.c \
               mlib_ImageConstDiv_Fp.c \
               mlib_ImageConstMul_Fp.c \
               mlib_ImageConstSub_Fp_Inp.c \
               mlib_ImageDivAlpha_Fp.c \
               mlib_ImageDivAlpha_Fp_Inp.c \
               mlib_ImageDiv_Fp.c \
               mlib_ImageExp_Fp.c \
               mlib_ImageInvert_Fp.c \
               mlib_ImageInvert_Fp_Inp.c \
               mlib_ImageLog_Fp.c \
               mlib_ImageMax_Fp_Inp.c \
               mlib_ImageMin_Fp_Inp.c \
               mlib_ImageMulAlpha_Fp.c \
               mlib_ImageMulAlpha_Fp_Inp.c \
               mlib_ImageMul_Fp.c \
               mlib_ImageMul_Fp_Inp.c \
               mlib_ImageScalarBlend_Fp.c \
               mlib_ImageScalarBlend_Fp_Inp.c \
               mlib_ImageSqr_Fp.c \
               mlib_ImageSqr_Fp_Inp.c \
               mlib_ImageSub_Fp_Inp.c \
               mlib_ImageScale_Fp_Inp.c

win32_SRCS_STDC = \
            mlib_ImageBlend_Fp.c \
            mlib_c_ImageAve.c \
            mlib_ImageAve_Fp.c \
            mlib_ImageConstAdd_Fp.c \
            mlib_ImageConstSub_Fp.c \
            mlib_ImageAdd_Fp.c \
            mlib_ImageSub_Fp.c \
            mlib_ImageMax_Fp.c \
            mlib_ImageMin_Fp.c \
            mlib_c_ImageBlendRGBA2ARGB.c \
            mlib_c_ImageBlendRGBA2BGRA.c

SRCS_STDC = mlib_c_ImageBlendColor_Inp.c \
            mlib_c_ImageBlendColor_f.c \
            mlib_c_ImageDivAlpha_f.c \
            mlib_c_ImageDivAlpha_Inp.c \
            mlib_c_ImageDivShiftU8.c \
            mlib_c_ImageMulAlpha.c \
            mlib_c_ImageMulAlpha_Inp.c \
            mlib_c_ImageMulShift.c \
            mlib_c_ImageScalarBlend.c \
            mlib_c_ImageScalarBlendFunc.c \
            mlib_c_ImageScale.c \
            mlib_c_ImageScale2Func.c \
            mlib_c_ImageScale_Fp.c \
            mlib_c_ImageSqrShift_f.c \
            mlib_c_ImageSub_Inp.c \
            mlib_c_ImageSqrShift.c \
            $($(TARCH)_SRCS_STDC)

win32_SRCS = mlib_mmx_utils.c  \
             mlib_m_ImageAdd.c \
             mlib_m_ImageSub.c \
             mlib_m_ImageAbs.c \
             mlib_m_ImageConstAdd.c \
             mlib_m_ImageConstSub.c \
             mlib_m_ImageMax.c \
             mlib_m_ImageMin.c \
             mlib_m_ImageBlend.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_TXT)/../spatial -I$(HOME_TXT)/../blend -I$(HOME_INCLUDE)/../../mmx

#
# define depends
#

$(OBJS) $(OBJS_STDC) : $(MLIB_HOME)/src/generic/stdc/image/include/mlib_ImageCheck.h

$(ARCH_DIR_O)/mlib_m_ImageAbs$(OBJ_EXT)      :  $(HOME_INCLUDE)/../../../../generic/stdc/image/arith/mlib_ImageAbs.h
$(ARCH_DIR_O)/mlib_m_ImageAdd$(OBJ_EXT)      :  $(HOME_INCLUDE)/mlib_m_ImageArith.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageBlend$(OBJ_EXT)    :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstAdd$(OBJ_EXT) :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstSub$(OBJ_EXT) :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageMax$(OBJ_EXT)      :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageMin$(OBJ_EXT)      :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageSub$(OBJ_EXT)      :  $(HOME_INCLUDE)/mlib_m_ImageArith.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_mmx_utils$(OBJ_EXT)       :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h

$(DIR_O_STDC)/mlib_ImageAbs$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAbs.h
$(DIR_O_STDC)/mlib_ImageBlendMulti$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../../include/mlib_Utils.h
$(DIR_O_STDC)/mlib_ImageBlendColor$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O_STDC)/mlib_ImageBlendColor_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_ImageBlendColor_Fp_Inp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_ImageConstDiv$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O_STDC)/mlib_ImageConstDivShift$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O_STDC)/mlib_ImageConstMulShift$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O_STDC)/mlib_ImageDivAlpha$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageDivAlpha_f.h \
                                                       $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O_STDC)/mlib_ImageDivShift$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConstDiv.h
$(DIR_O_STDC)/mlib_ImageExpTbl$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageExpTbl.h
$(DIR_O_STDC)/mlib_ImageExp_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageExpTbl.h
$(DIR_O_STDC)/mlib_ImageLogTbl$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageLogTbl.h
$(DIR_O_STDC)/mlib_ImageLog_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageLogTbl.h
$(DIR_O_STDC)/mlib_ImageScale2$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O_STDC)/mlib_ImageScaleCheck$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O_STDC)/mlib_c_ImageAbs$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAbs.h
$(DIR_O_STDC)/mlib_c_ImageBlend$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageBlendColor_Inp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../spatial/mlib_ImageVisVersion.h
$(DIR_O_STDC)/mlib_c_ImageBlendColor_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendColor.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageDivAlpha_Inp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageDivAlpha_f.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageDivAlpha_f$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageDivAlpha_f.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageDivShiftU8$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageConstDiv.h \
                                                       $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_ImageMulAlpha$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageMulAlpha_Inp$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                       $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_ImageMulShift$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_ImageScalarBlend$(OBJ_EXT)      :  $(HOME_TXT)/mlib_c_ImageScalarBlendFunc.h
$(DIR_O_STDC)/mlib_c_ImageScalarBlendFunc$(OBJ_EXT)  :  $(HOME_TXT)/mlib_c_ImageScalarBlendFunc.h
$(DIR_O_STDC)/mlib_c_ImageScale$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O_STDC)/mlib_c_ImageScale2Func$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageScale2.h
$(DIR_O_STDC)/mlib_c_ImageSqrShift$(OBJ_EXT)         :  $(HOME_TXT)/mlib_c_ImageSqrShift.h
$(DIR_O_STDC)/mlib_c_ImageSqrShift_f$(OBJ_EXT)       :  $(HOME_TXT)/mlib_c_ImageSqrShift.h
