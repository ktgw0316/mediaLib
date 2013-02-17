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
# ident	"@(#)Makefile	9.13	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/radio

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/radio

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageHistogram.c                \
             mlib_ImageLookUp2.c                  \
             mlib_ImageLookUpMask_64.c            \
             mlib_ImageLookUp_64.c                \
             mlib_ImageLookUp_Bit.c               \
             mlib_ImageLookUpMask_f.c             \
             mlib_ImageThresh_Inp.c               \
             mlib_ImageLookUp_Inp.c               \
             mlib_ImageColorTrue2IndexInit.c      \
             mlib_ImageColorTrue2Index.c          \
             mlib_ImageColorErrorDiffusionMxN.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageReplaceColor_Fp.c        \
               mlib_ImageThresh1_F32.c            \
               mlib_ImageThresh1_D64.c            \
               mlib_ImageThresh_Fp_Inp.c

# C specific source under src/stdc/generic
SRCS_STDC = mlib_c_ImageColorErrDiff3x3.c         \
            mlib_c_ImageThresh1_U8.c              \
            mlib_c_ImageLookUpMask.c              \
            mlib_c_ImageColorErrDiffMxN_Bit.c

# MMX specific source under src/x86/mmx

SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageThresh1.c              \
            mlib_s_ImageThresh1BFunc.c         \
            mlib_s_ImageThresh2.c              \
            mlib_s_ImageThresh3.c              \
            mlib_s_ImageThresh4.c              \
            mlib_s_ImageThresh5.c              \
            mlib_s_ImageClrOrdDither8x8.c      \
            mlib_s_ImageClrOrdDitherMxN.c      \
            mlib_s_ImageClrOrdDitherMxN_Bit.c  \
            mlib_s_ImageReplaceColor.c         \
            mlib_s_ImageLookUp.c               \
            mlib_s_ImageLookUp_8.c             \
            mlib_s_ImageLookUp_16.c            \
            mlib_s_ImageLookUp_32.c            \
            mlib_s_ImageHistogram2.c           \
            mlib_s_ImageHistogram2_f.c         \
            mlib_s_ImageThresh1_Fp.c           \
            mlib_s_ImageThresh2_Fp.c           \
            mlib_s_ImageThresh3_Fp.c           \
            mlib_s_ImageThresh4_Fp.c           \
            mlib_s_ImageThresh5_Fp.c

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

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$$(DIR_O)/mlib_ImageColorErrorDiffusionMxN$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageColorErrorDiffusionMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_ImageReplaceColor_Fp$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageReplaceColor.h
$(DIR_O)/mlib_ImageColorTrue2Index$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageColormap.h  \
                                                            $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageColorTrue2IndexInit$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_ImageHistogram2_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O)/mlib_ImageLookUpMask_64$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUpMask_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUp_64$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageLookUp_Bit$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageThresh1_D64$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_F32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageColorErrDiffMxN_Bit$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageClrOrdDither8x8$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageClrOrdDitherMxN$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_s_ImageClrOrdDitherMxN_Bit$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageReplaceColor$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageReplaceColor.h
$(DIR_O)/mlib_c_ImageHistogram2$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O)/mlib_c_ImageLookUp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h \
                                                            $(HOME_TXT)/mlib_c_ImageLookUp.h
$(DIR_O)/mlib_c_ImageLookUpMask$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_c_ImageLookUp_f$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h \
                                                            $(HOME_TXT)/mlib_c_ImageLookUp.h
$(DIR_O)/mlib_c_ImageThresh1$(OBJ_EXT)              :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_S16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_S32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_U16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_U8$(OBJ_EXT)           :  $(HOME_TXT)/mlib_c_ImageThresh1.h

$(DIR_O)/mlib_m_ImageHistogram2$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageLookUp$(OBJ_EXT)       :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageLookUp_16$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageLookUp_32$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageLookUp_8$(OBJ_EXT)     :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageThresh4$(OBJ_EXT)      :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageThresh5$(OBJ_EXT)      :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_s_ImageClrOrdDither8x8$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_s_ImageClrOrdDitherMxN$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_s_ImageClrOrdDitherMxN_Bit$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_s_ImageReplaceColor$(OBJ_EXT)         :  ./mlib_s_ImageReplaceColor.h
$(DIR_O)/mlib_s_ImageThresh1$(OBJ_EXT)              :  ./mlib_s_ImageThreshFunc.h
$(DIR_O)/mlib_s_ImageThresh1BFunc$(OBJ_EXT)         :  ./mlib_s_ImageThreshFunc.h
