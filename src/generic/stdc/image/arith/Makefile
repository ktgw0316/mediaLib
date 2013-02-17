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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS   = mlib_ImageAbs.c \
          mlib_ImageBlendColor.c \
          mlib_ImageConstDiv.c \
          mlib_ImageConstDivShift.c \
          mlib_ImageConstMul.c \
          mlib_ImageConstMulShift.c \
          mlib_ImageDivAlpha.c \
          mlib_ImageDivConstShift.c \
          mlib_ImageDivShift.c \
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
          mlib_ImageAve_Inp.c \
          mlib_ImageAdd_Inp.c \
          mlib_ImageAbs_Inp.c

SRCS_FP = mlib_ImageAbs_Fp.c \
          mlib_ImageAbs_Fp_Inp.c \
          mlib_ImageAdd_Fp.c \
          mlib_ImageAdd_Fp_Inp.c \
          mlib_ImageAve_Fp.c \
          mlib_ImageAve_Fp_Inp.c \
          mlib_ImageBlend_Fp.c \
          mlib_ImageBlend_Fp_Inp.c \
          mlib_ImageBlendMulti.c \
          mlib_ImageBlendColor_Fp.c \
          mlib_ImageBlendColor_Fp_Inp.c \
          mlib_ImageConstAdd_Fp.c \
          mlib_ImageConstAdd_Fp_Inp.c \
          mlib_ImageConstDiv_Fp.c \
          mlib_ImageConstMul_Fp.c \
          mlib_ImageConstSub_Fp.c \
          mlib_ImageConstSub_Fp_Inp.c \
          mlib_ImageDivAlpha_Fp.c \
          mlib_ImageDivAlpha_Fp_Inp.c \
          mlib_ImageDiv_Fp.c \
          mlib_ImageExp_Fp.c \
          mlib_ImageInvert_Fp.c \
          mlib_ImageInvert_Fp_Inp.c \
          mlib_ImageLog_Fp.c \
          mlib_ImageMax_Fp.c \
          mlib_ImageMax_Fp_Inp.c \
          mlib_ImageMin_Fp.c \
          mlib_ImageMin_Fp_Inp.c \
          mlib_ImageMulAlpha_Fp.c \
          mlib_ImageMulAlpha_Fp_Inp.c \
          mlib_ImageMul_Fp.c \
          mlib_ImageMul_Fp_Inp.c \
          mlib_ImageScalarBlend_Fp.c \
          mlib_ImageScalarBlend_Fp_Inp.c \
          mlib_ImageSqr_Fp.c \
          mlib_ImageSqr_Fp_Inp.c \
          mlib_ImageSub_Fp.c \
          mlib_ImageSub_Fp_Inp.c \
          mlib_ImageScale_Fp_Inp.c

SRCS_C  = mlib_c_ImageAbs.c \
          mlib_c_ImageAdd.c \
          mlib_c_ImageSqrShift.c \
          mlib_c_ImageAve.c \
          mlib_c_ImageBlend.c \
          mlib_c_ImageBlendRGBA2ARGB.c \
          mlib_c_ImageBlendRGBA2BGRA.c \
          mlib_c_ImageBlendColor_f.c \
          mlib_c_ImageBlendColor_Inp.c \
          mlib_c_ImageConstAdd.c \
          mlib_c_ImageConstSub.c \
          mlib_c_ImageDivAlpha_f.c \
          mlib_c_ImageDivAlpha_Inp.c \
          mlib_c_ImageDivShiftU8.c \
          mlib_c_ImageMax.c \
          mlib_c_ImageMin.c \
          mlib_c_ImageMulAlpha.c \
          mlib_c_ImageMulAlpha_Inp.c \
          mlib_c_ImageMulShift.c \
          mlib_c_ImageScalarBlend.c \
          mlib_c_ImageScalarBlendFunc.c \
          mlib_c_ImageScale.c \
          mlib_c_ImageScale2Func.c \
          mlib_c_ImageScale_Fp.c \
          mlib_c_ImageSqrShift_f.c \
          mlib_c_ImageSub.c \
          mlib_c_ImageSub_Inp.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../include -I./../blend -I./../spatial -I./../../include

#
# define depends
#

$(OBJS) : ./../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAbs$(OBJ_EXT)                :  ./mlib_ImageAbs.h
$(DIR_O)/mlib_ImageBlendMulti$(OBJ_EXT)         :  ./../include/mlib_ImageDivTables.h ./../../include/mlib_Utils.h
$(DIR_O)/mlib_ImageBlendColor$(OBJ_EXT)         :  ./mlib_ImageBlendColor.h \
                                                   ./../include/mlib_ImageDivTables.h \
                                                   ./../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageBlendColor_Fp$(OBJ_EXT)      :  ./mlib_ImageBlendColor.h \
                                                   ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_ImageBlendColor_Fp_Inp$(OBJ_EXT)  :  ./mlib_ImageBlendColor.h \
                                                   ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_ImageConstDiv$(OBJ_EXT)           :  ./mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageConstDivShift$(OBJ_EXT)      :  ./mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageConstMulShift$(OBJ_EXT)      :  ./mlib_ImageScale2.h
$(DIR_O)/mlib_ImageDivAlpha$(OBJ_EXT)           :  ./mlib_ImageDivAlpha_f.h \
                                                   ./../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageDivShift$(OBJ_EXT)           :  ./mlib_ImageConstDiv.h
$(DIR_O)/mlib_ImageExpTbl$(OBJ_EXT)             :  ./mlib_ImageExpTbl.h
$(DIR_O)/mlib_ImageExp_Fp$(OBJ_EXT)             :  ./mlib_ImageExpTbl.h
$(DIR_O)/mlib_ImageLogTbl$(OBJ_EXT)             :  ./mlib_ImageLogTbl.h
$(DIR_O)/mlib_ImageLog_Fp$(OBJ_EXT)             :  ./mlib_ImageLogTbl.h
$(DIR_O)/mlib_ImageScale2$(OBJ_EXT)             :  ./mlib_ImageScale2.h
$(DIR_O)/mlib_ImageScaleCheck$(OBJ_EXT)         :  ./mlib_ImageScale2.h
$(DIR_O)/mlib_c_ImageAbs$(OBJ_EXT)              :  ./mlib_ImageAbs.h
$(DIR_O)/mlib_c_ImageBlend$(OBJ_EXT)            :  ./../include/mlib_ImageDivTables.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlendColor_Inp$(OBJ_EXT)   :  ./mlib_ImageBlendColor.h \
                                                   ./../include/mlib_ImageDivTables.h \
                                                   ./../spatial/mlib_ImageVisVersion.h
$(DIR_O)/mlib_c_ImageBlendColor_f$(OBJ_EXT)     :  ./mlib_ImageBlendColor.h \
                                                   ./../include/mlib_ImageDivTables.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageDivAlpha_Inp$(OBJ_EXT)     :  ./mlib_ImageDivAlpha_f.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageDivAlpha_f$(OBJ_EXT)       :  ./mlib_ImageDivAlpha_f.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageDivShiftU8$(OBJ_EXT)       :  ./mlib_ImageConstDiv.h \
                                                   ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageMulAlpha$(OBJ_EXT)         :  ./../include/mlib_ImageDivTables.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageMulAlpha_Inp$(OBJ_EXT)     :  ./../include/mlib_ImageDivTables.h \
                                                   ./../blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageMulShift$(OBJ_EXT)         :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageScalarBlend$(OBJ_EXT)      :  ./mlib_c_ImageScalarBlendFunc.h
$(DIR_O)/mlib_c_ImageScalarBlendFunc$(OBJ_EXT)  :  ./mlib_c_ImageScalarBlendFunc.h
$(DIR_O)/mlib_c_ImageScale$(OBJ_EXT)            :  ./mlib_ImageScale2.h
$(DIR_O)/mlib_c_ImageScale2Func$(OBJ_EXT)       :  ./mlib_ImageScale2.h
$(DIR_O)/mlib_c_ImageSqrShift$(OBJ_EXT)         :  ./mlib_c_ImageSqrShift.h
$(DIR_O)/mlib_c_ImageSqrShift_f$(OBJ_EXT)       :  ./mlib_c_ImageSqrShift.h
