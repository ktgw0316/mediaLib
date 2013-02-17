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
# ident	"@(#)Makefile	9.24	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/arith
HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/arith

# common source under src/stdc/generic
UTILS_STDC =	mlib_ImageAbs.c \
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
		mlib_ImageSqrShift_Inp.c \
		mlib_ImageConstAdd_Inp.c \
		mlib_ImageBlend_Inp.c \
		mlib_ImageBlendMulti.c \
		mlib_ImageAve_Inp.c \
		mlib_ImageAdd_Inp.c \
		mlib_ImageAbs_Inp.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =	mlib_ImageBlendColor_Fp.c \
		mlib_ImageBlendColor_Fp_Inp.c \
		mlib_ImageDivAlpha_Fp_Inp.c \
		mlib_ImageExp_Fp.c \
		mlib_ImageLog_Fp.c \
		mlib_ImageMulAlpha_Fp_Inp.c

# C specific source under src/stdc/generic
SRCS_STDC =	mlib_c_ImageDivAlpha_f.c \
		mlib_c_ImageDivAlpha_Inp.c \
		mlib_c_ImageDivShiftU8.c \
		mlib_c_ImageScale2Func.c \
		mlib_c_ImageSub_Inp.c

# MMX specific source under src/x86/mmx
SRCS_MMX =	mlib_mmx_utils.c

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 =	mlib_s_ImageBlend.c \
		mlib_s_ImageAbs.c \
		mlib_s_ImageAdd.c \
		mlib_s_ImageSub.c \
		mlib_s_ImageMax.c \
		mlib_s_ImageMin.c \
		mlib_s_ImageAve.c \
		mlib_s_ImageConstAdd.c \
		mlib_s_ImageConstSub.c \
		mlib_s_ImageBlendRGBA2ARGB.c \
		mlib_s_ImageBlendRGBA2BGRA.c \
		mlib_s_ImageMulAlpha.c \
		mlib_s_ImageMulAlpha_U8.c \
		mlib_s_ImageMulAlpha_S16.c \
		mlib_s_ImageMulAlpha_U16.c \
		mlib_s_ImageMulAlpha_Inp.c \
		mlib_s_ImageMulAlpha_Inp_U8.c \
		mlib_s_ImageMulAlpha_Inp_S16.c \
		mlib_s_ImageMulAlpha_Inp_U16.c \
		mlib_s_ImageSqrShift.c \
		mlib_s_ImageSqrShift_f.c \
		mlib_s_ImageMulShift.c	\
		mlib_s_ImageMulShift_f.c \
		mlib_s_ImageBlendColor.c \
		mlib_s_ImageBlendColor_U8.c \
		mlib_s_ImageBlendColor_U16.c \
		mlib_s_ImageBlendColor_S16.c \
		mlib_s_ImageBlendColor_S32.c \
		mlib_s_ImageBlendColor_Inp.c \
		mlib_s_ImageBlendColor_Inp_U8.c	\
		mlib_s_ImageBlendColor_Inp_U16.c \
		mlib_s_ImageBlendColor_Inp_S16.c \
		mlib_s_ImageBlendColor_Inp_S32.c \
		mlib_s_ImageAbs_Fp.c \
		mlib_s_ImageSqr_Fp.c \
		mlib_s_ImageAve_Fp.c \
		mlib_s_ImageMax_Fp.c \
		mlib_s_ImageMin_Fp.c \
		mlib_s_ImageAdd_Fp.c \
		mlib_s_ImageSub_Fp.c \
		mlib_s_ImageMul_Fp.c \
		mlib_s_ImageDiv_Fp.c \
		mlib_s_ImageConstAdd_Fp.c \
		mlib_s_ImageConstSub_Fp.c \
		mlib_s_ImageConstMul_Fp.c \
		mlib_s_ImageConstDiv_Fp.c \
		mlib_s_ImageInvert_Fp.c \
		mlib_s_ImageBlend_Fp.c \
		mlib_s_ImageBlend_Fp_MA.c \
		mlib_s_ImageBlend_Fp_SA_F32.c \
		mlib_s_ImageBlend_Fp_SA_D64.c \
		mlib_s_ImageScalarBlend_Fp.c \
		mlib_s_ImageScalarBlend_Fp_F32.c \
		mlib_s_ImageScalarBlend_Fp_D64.c \
		mlib_s_ImageScalarBlend.c \
		mlib_s_ImageScalarBlend_u8.c  \
		mlib_s_ImageScalarBlend_s16.c \
		mlib_s_ImageScalarBlend_u16.c \
		mlib_s_ImageScalarBlend_s32.c \
		mlib_s_ImageScale_Fp.c \
		mlib_s_ImageScale_Fp_D64.c \
		mlib_s_ImageScale_Fp_F32.c \
		mlib_s_ImageScale_Fp_S32.c \
		mlib_s_ImageScale_Fp_S16.c \
		mlib_s_ImageScale_Fp_U16.c \
		mlib_s_ImageScale_Fp_U8.c  \
		mlib_s_ImageScale.c  \
		mlib_s_ImageScale_U8.c  \
		mlib_s_ImageScale_S16.c \
		mlib_s_ImageScale_U16.c \
		mlib_s_ImageScale_S32.c \
		mlib_s_ImageMulAlpha_Fp.c  \
		mlib_s_ImageDivAlpha_Fp.c

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

include $(TOP_DIR)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)
INCLUDES += -I$(HOME_TXT)/../include
INCLUDES += -I$(HOME_TXT)/../blend
INCLUDES += -I$(HOME_TXT)/../copy
INCLUDES += -I$(HOME_TXT)/../spatial
INCLUDES += -I$(ALT_DIR)
INCLUDES += -I$(ALT_DIR)/../../mmx
INCLUDES += -I../../sse

#
# dependencies
#

$(OBJS) $(OBJS_STDC) : \
        $(HOME_TXT)/../include/mlib_ImageCheck.h

#
# define depends
#

UTIL_HEADERS = ../../sse/mlib_sse_utils.h ../../../mmx/mmx/mlib_mmx_utils.h

$(DIR_O)/mlib_mmx_utils$(OBJ_EXT) : \
	$(ALT_DIR)/../../mmx/mlib_mmx_utils.h

$(DIR_O)/mlib_ImageAbs$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageAbs.h

$(DIR_O)/mlib_ImageBlendMulti$(OBJ_EXT) : \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h \
	$(HOME_TXT)/../../include/mlib_Utils.h

$(DIR_O)/mlib_ImageBlendColor_Fp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageBlendColor.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h

$(DIR_O)/mlib_ImageBlendColor_Fp_Inp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageBlendColor.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h

$(DIR_O)/mlib_ImageConstDiv$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageConstDiv.h

$(DIR_O)/mlib_ImageConstDivShift$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageConstDiv.h

$(DIR_O)/mlib_ImageConstMulShift$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageScale2.h

$(DIR_O)/mlib_ImageDivAlpha$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageDivAlpha_f.h \
	$(HOME_TXT)/../spatial/mlib_ImageVisVersion.h

$(DIR_O)/mlib_ImageDivShift$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageConstDiv.h

$(DIR_O)/mlib_ImageExpTbl$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageExpTbl.h

$(DIR_O)/mlib_ImageExp_Fp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageExpTbl.h

$(DIR_O)/mlib_ImageLogTbl$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageLogTbl.h

$(DIR_O)/mlib_ImageLog_Fp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageLogTbl.h
$(DIR_O)/mlib_ImageScale2$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageScale2.h

$(DIR_O)/mlib_ImageScaleCheck$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageScale2.h

$(DIR_O)/mlib_c_ImageAbs$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageAbs.h

$(DIR_O)/mlib_c_ImageBlend$(OBJ_EXT) : \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h \
	$(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h

$(DIR_O)/mlib_c_ImageBlendColor_Inp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageBlendColor.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h \
	$(HOME_TXT)/../spatial/mlib_ImageVisVersion.h

$(DIR_O)/mlib_c_ImageDivAlpha_Inp$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageDivAlpha_f.h \
	$(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h

$(DIR_O)/mlib_c_ImageDivAlpha_f$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageDivAlpha_f.h \
	$(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h

$(DIR_O)/mlib_c_ImageDivShiftU8$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageConstDiv.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h

$(DIR_O)/mlib_c_ImageScalarBlend$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_c_ImageScalarBlendFunc.h

$(DIR_O)/mlib_c_ImageScalarBlendFunc$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_c_ImageScalarBlendFunc.h

$(DIR_O)/mlib_c_ImageScale$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageScale2.h

$(DIR_O)/mlib_c_ImageScale2Func$(OBJ_EXT) : \
	$(HOME_TXT)/mlib_ImageScale2.h

$(DIR_O)/mlib_s_ImageBlend$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageArith3.h

$(DIR_O)/mlib_s_ImageConstAdd$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageConstSub$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageAdd$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageSub$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageMax$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageMin$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageAve$(OBJ_EXT) : \
	$(UTIL_HEADERS) ./mlib_s_ImageConstArith.h

$(DIR_O)/mlib_s_ImageAbs$(OBJ_EXT) : \
	$(UTIL_HEADERS)

$(DIR_O)/mlib_s_ImageBlendRGBA2ARGB$(OBJ_EXT) : \
	$(HOME_TXT)/../copy/mlib_ImageCopy.h

$(DIR_O)/mlib_s_ImageBlendRGBA2BGRA$(OBJ_EXT) : \
	$(HOME_TXT)/../copy/mlib_ImageCopy.h

$(DIR_O)/mlib_s_ImageMulAlpha$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h \
	$(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h

$(DIR_O)/mlib_s_ImageMulAlpha_U8$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha.h

$(DIR_O)/mlib_s_ImageMulAlpha_U16$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha.h

$(DIR_O)/mlib_s_ImageMulAlpha_S16$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha.h

$(DIR_O)/mlib_s_ImageMulAlpha_Inp$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha_Inp.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h \
	$(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h

$(DIR_O)/mlib_s_ImageMulAlpha_Inp_U8$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha_Inp.h

$(DIR_O)/mlib_s_ImageMulAlpha_Inp_U16$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha_Inp.h

$(DIR_O)/mlib_s_ImageMulAlpha_Inp_S16$(OBJ_EXT) : \
	./mlib_s_ImageMulAlpha_Inp.h

$(DIR_O)/mlib_s_ImageSqrShift$(OBJ_EXT) : \
	./mlib_s_ImageSqrShift.h

$(DIR_O)/mlib_s_ImageSqrShift_f$(OBJ_EXT) : \
	./mlib_s_ImageSqrShift.h

$(DIR_O)/mlib_s_ImageMulShift$(OBJ_EXT) : \
	./mlib_s_ImageSqrShift.h

$(DIR_O)/mlib_s_ImageMulShift_f$(OBJ_EXT) : \
	./mlib_s_ImageSqrShift.h \
	$(HOME_TXT)/../include/mlib_ImageDivTables.h

$(DIR_O)/mlib_s_ImageBlendColor$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor.h

$(DIR_O)/mlib_s_ImageBlendColor_U8$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor.h

$(DIR_O)/mlib_s_ImageBlendColor_U16$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor.h

$(DIR_O)/mlib_s_ImageBlendColor_S16$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor.h

$(DIR_O)/mlib_s_ImageBlendColor_S32$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor.h

$(DIR_O)/mlib_s_ImageBlendColor_Inp$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor_Inp.h

$(DIR_O)/mlib_s_ImageBlendColor_Inp_U8$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor_Inp.h

$(DIR_O)/mlib_s_ImageBlendColor_Inp_U16$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor_Inp.h

$(DIR_O)/mlib_s_ImageBlendColor_Inp_S16$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor_Inp.h

$(DIR_O)/mlib_s_ImageBlendColor_Inp_S32$(OBJ_EXT) : \
	./mlib_s_ImageBlendColor_Inp.h

$(DIR_O)/mlib_s_ImageAdd_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageSub_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageMul_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageDiv_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageMax_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageMin_Fp$(OBJ_EXT) : \
	./mlib_s_ImageArith_Fp.h

$(DIR_O)/mlib_s_ImageConstAdd_Fp$(OBJ_EXT) : \
	./mlib_s_ImageConstArith_Fp.h

$(DIR_O)/mlib_s_ImageConstSub_Fp$(OBJ_EXT) : \
	./mlib_s_ImageConstArith_Fp.h

$(DIR_O)/mlib_s_ImageConstMul_Fp$(OBJ_EXT) : \
	./mlib_s_ImageConstArith_Fp.h

$(DIR_O)/mlib_s_ImageConstDiv_Fp$(OBJ_EXT) : \
	./mlib_s_ImageConstArith_Fp.h

$(DIR_O)/mlib_s_ImageBlend_Fp$(OBJ_EXT) : \
	./mlib_s_ImageBlend_Fp.h

$(DIR_O)/mlib_s_ImageBlend_Fp_MA$(OBJ_EXT) : \
	./mlib_s_ImageBlend_Fp.h

$(DIR_O)/mlib_s_ImageBlend_Fp_SA_F32$(OBJ_EXT) : \
	./mlib_s_ImageBlend_Fp.h

$(DIR_O)/mlib_s_ImageBlend_Fp_SA_D64$(OBJ_EXT) : \
	./mlib_s_ImageBlend_Fp.h

$(DIR_O)/mlib_s_ImageScalarBlend_Fp$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend_Fp.h

$(DIR_O)/mlib_s_ImageScalarBlend_Fp_D64$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend_Fp.h

$(DIR_O)/mlib_s_ImageScalarBlend_Fp_D64$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend_Fp.h

$(DIR_O)/mlib_s_ImageScalarBlend$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend.h

$(DIR_O)/mlib_s_ImageScalarBlend_u8$(OBJ_EXT) :  \
	./mlib_s_ImageScalarBlend.h

$(DIR_O)/mlib_s_ImageScalarBlend_s16$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend.h

$(DIR_O)/mlib_s_ImageScalarBlend_u16$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend.h

$(DIR_O)/mlib_s_ImageScalarBlend_s32$(OBJ_EXT) : \
	./mlib_s_ImageScalarBlend.h

$(DIR_O)/mlib_s_ImageScale_Fp$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_D64$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_F32$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_S32$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_S16$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_U16$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale_Fp_U8$(OBJ_EXT) : \
	./mlib_s_ImageScale_Fp.h

$(DIR_O)/mlib_s_ImageScale$(OBJ_EXT) : \
	./mlib_s_ImageScale.h

$(DIR_O)/mlib_s_ImageScale_S32$(OBJ_EXT) : \
	./mlib_s_ImageScale.h

$(DIR_O)/mlib_s_ImageScale_S16$(OBJ_EXT) : \
	./mlib_s_ImageScale.h

$(DIR_O)/mlib_s_ImageScale_U16$(OBJ_EXT) : \
	./mlib_s_ImageScale.h

$(DIR_O)/mlib_s_ImageScale_U8$(OBJ_EXT) : \
	./mlib_s_ImageScale.h
