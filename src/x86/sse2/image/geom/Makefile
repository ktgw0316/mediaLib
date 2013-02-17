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
# ident	"@(#)Makefile	9.25	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/geom
HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/geom

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageAffineIndex.c \
             mlib_ImageFlipMainDiag_Bit.c \
             mlib_ImageFlipY_Bit.c \
             mlib_ImageAffineEdge.c \
             mlib_ImageAffineTransform.c \
             mlib_ImageAffineTransformIndex.c \
             mlib_ImageAffineTable.c \
             mlib_ImageAffineTable_16ext.c \
             mlib_ImageAffineTable_16nw.c \
             mlib_ImageAffineTable_16_3nw.c \
             mlib_ImageAffineTable_u16ext.c \
             mlib_ImageAffineTable_u16nw.c \
             mlib_ImageAffineTable_u16_3nw.c \
             mlib_ImageAffineTable_32ext.c \
             mlib_ImageAffineTable_32nw.c \
             mlib_ImageAffineTable_32_3nw.c \
             mlib_ImageAffineTable_8ext.c \
             mlib_ImageAffineTable_8nw.c \
             mlib_ImageAffineTable_8_3nw.c \
             mlib_ImageAffineTable_D64ext.c \
             mlib_ImageAffineTable_D64nw.c \
             mlib_ImageAffineTable_D64_3nw.c \
             mlib_ImageAffineTable_F32ext.c \
             mlib_ImageAffineTable_F32nw.c \
             mlib_ImageAffineTable_F32_3nw.c \
             mlib_ImageAffineTable_F32nw_2.c \
             mlib_ImageAffine.c \
             mlib_ImageAffine_NN_Bit.c \
             mlib_ImageGridWarp.c     \
             mlib_ImageFilters.c \
             mlib_ImageGridWarpTable.c     \
             mlib_ImageInterpTable.c \
             mlib_ImagePolynomialWarpTools.c \
             mlib_ImagePolynomialWarp_0.c \
             mlib_ImagePolynomialWarp_1.c \
             mlib_ImagePolynomialWarp_BC_S32.c \
             mlib_ImagePolynomialWarp_BL_S32.c \
             mlib_ImagePolynomialWarpTable_f.c \
             mlib_ImagePolynomialWarpTable_it.c \
             mlib_ImagePolynomWarpTable_0.c\
             mlib_ImageRotate.c \
             mlib_ImageRotateIndex.c \
             mlib_ImageScanPoly.c \
             mlib_ImageZoomClipping.c \
             mlib_ImageZoomEdge.c \
             mlib_ImageZoomEdgeIndex.c \
             mlib_ImageZoomIn2XIndex_BC_Edge.c \
             mlib_ImageZoomIn2XIndex_BC2_Edge.c \
             mlib_ImageZoomIn2XIndex_BL_Edge.c \
             mlib_ImageZoomIn2XIndex_Edge.c \
             mlib_ImageZoomIn2X_Edge.c \
             mlib_ImageZoomIn2X_BL.c \
             mlib_ImageZoomOut2XEdge.c \
             mlib_ImageZoomOut2XIndex_Edge.c \
             mlib_ImageZoomTranslateToGray.c \
             mlib_ImageZoomTranslateTable.c \
             mlib_ImageZoomTransTable_16ext.c \
             mlib_ImageZoomTransTable_16nw.c \
             mlib_ImageZoomTransTable_u16ext.c \
             mlib_ImageZoomTransTable_u16nw.c \
             mlib_ImageZoomTransTable_32ext.c \
             mlib_ImageZoomTransTable_32nw.c \
             mlib_ImageZoomTransTable_8ext.c \
             mlib_ImageZoomTransTable_8nw.c \
             mlib_ImageZoomTransTable_D64ext.c \
             mlib_ImageZoomTransTable_D64nw.c \
             mlib_ImageZoomTransTable_F32ext.c \
             mlib_ImageZoomTransTable_F32nw.c \
             mlib_ImageZoom.c          \
             mlib_ImageZoomBlendEdge.c \
             mlib_ImageZoomBlend.c     \
             mlib_ImageZoomTranslateBlend.c \
             mlib_ImageZoomTransTableBlend.c \
             mlib_ImageZoomTransTableBlEdge.c \
             mlib_ImageZoomTransTableBl_8ext.c \
             mlib_ImageZoomTransTableBl_8nw.c \
             mlib_ImageSubsamBinToGray4x4.c \
             mlib_ImageSubsamBinToGray2x2.c \
             mlib_ImageSubsampleBinaryToGray.c \
             mlib_ImageFilteredSubsample_U8.c    \
             mlib_ImageFilteredSubsam_U8_f.c  \
             mlib_ImageFilteredSubsample_U16.c   \
             mlib_ImageFilteredSubsam_U16_f.c \
             mlib_ImageFilteredSubsample_S16.c   \
             mlib_ImageFilteredSubsam_S16_f.c \
             mlib_ImageFilteredSubsample_S32.c   \
             mlib_ImageFilteredSubsam_S32_f.c \
             mlib_ImageFilteredSubsample_F32.c   \
             mlib_ImageFilteredSubsam_F32_f.c \
             mlib_ImageFilteredSubsample_D64.c   \
             mlib_ImageFilteredSubsam_D64_f.c \

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageAffineTable_Fp.c \
               mlib_ImageAffineTransform_Fp.c \
               mlib_ImageAffine_Fp.c \
               mlib_ImageGridWarp_Fp.c \
               mlib_ImageGridWarpTable_Fp.c    \
               mlib_ImagePolynomialWarp_0_Fp.c \
               mlib_ImagePolynomialWarp_Call_Fp.c \
               mlib_ImagePolynomialWarp_BC_Fp.c \
               mlib_ImagePolynomialWarp_BL_Fp.c \
               mlib_ImagePolynomialWarp_NN_2_Fp.c \
               mlib_ImagePolynomialWarp_NN_3_Fp.c \
               mlib_ImagePolynomialWarp_NN_4_Fp.c \
               mlib_ImagePolynomialWarp_NN_5_Fp.c \
               mlib_ImagePolynomialWarp_NN_Fp.c \
               mlib_ImagePolynomialWarpTable_ft.c \
               mlib_ImagePolynomWarpTable_0_Fp.c \
               mlib_ImageRotate_Fp.c \
               mlib_ImageZoomIn2X_Edge_Fp.c \
               mlib_ImageZoomIn2X_Fp.c \
               mlib_ImageZoomTranslateTable_Fp.c \
               mlib_ImageZoom_Fp.c   \
               mlib_ImageSubsampleAverage_Fp.c

# C specific source under src/stdc/generic
SRCS_STDC = mlib_c_ImageAffineIndex_BL.c \
             mlib_c_ImageAffineIndex_BC.c \
             mlib_c_ImageZoom_BC.c \
             mlib_c_ImageZoom_NN_f.c     \
             mlib_c_ImageZoomIn2XIndex.c \
             mlib_c_ImageZoomIn2XIndex_BC.c \
             mlib_c_ImageZoomIn2XIndex_BC2.c \
             mlib_c_ImageZoomIn2XIndex_BL.c \
             mlib_c_ImageZoomIndex.c \
             mlib_c_ImageZoomIndex_BC.c \
             mlib_c_ImageZoomIndex_BL.c \
             mlib_c_ImageZoomOut2XIndex.c \
             mlib_c_ImageFilteredSubsample_Fp.c \
             mlib_c_ImageSubsampleAverage.c

# MMX specific source under src/x86/mmx
SRCS_MMX =   mlib_m_ImageFilters.c \
	     mlib_m_ImageZoomTranslate_Fp.c \
             mlib_m_ImageFlipX.c        \
             mlib_m_ImageFlipX_Fp.c     \
             mlib_m_ImagePolynomialWarpTable.c    \
             mlib_m_ImagePolyWarpTable_Fp.c       \
             mlib_m_ImagePolynomialWarp_Call.c    \
             mlib_m_ImagePolynomialWarp_Fp.c      \
             mlib_m_ImagePolynomialWarp_NN.c      \
             mlib_m_ImagePolynomialWarp_NN_2.c    \
             mlib_m_ImagePolynomialWarp_NN_3.c    \
             mlib_m_ImagePolynomialWarp_NN_4.c    \
             mlib_m_ImagePolynomialWarp_NN_5.c    \
             mlib_m_ImageGetAffineFunc.c     \
             mlib_m_ImageAffineTable_8nw.c   \
             mlib_m_ImageAffineTable_s16nw.c \
             mlib_m_ImageAffineTable_u16nw.c \
             mlib_m_ImageAffineTable_1ch.c   \
             mlib_m_ImageAffineTable_2ch.c   \
             mlib_m_ImageAffineTable_3ch.c   \
             mlib_m_ImageAffineTable_4ch.c   \
	     mlib_m_ImageInterpTable.c

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageZoom_BL.c             \
       mlib_s_ImageZoom_BL_S16.c              \
       mlib_s_ImageZoom_BL_U16.c              \
       mlib_s_ImageZoom_BC.c                  \
       mlib_s_ImageZoom_BC_S16.c              \
       mlib_s_ImageZoom_BC_U16.c              \
       mlib_s_ImageZoomBlend_NN.c             \
       mlib_s_ImageZoomTranslate.c            \
       mlib_s_ImageZoomTranslateBlend.c       \
       mlib_s_ImageGetZoomFunc.c              \
       mlib_s_ImageZoomTransTable_8nw.c       \
       mlib_s_ImageSubsampleAverage_F32.c     \
       mlib_s_ImageSubsampleAverage_D64.c     \
       mlib_s_ImageSubsampleAverage_U8.c      \
       mlib_s_ImageSubsampleAverage_S16.c     \
       mlib_s_ImageSubsampleAverage_U16.c     \
       mlib_s_ImageSubsampleAverage_S32.c     \
       mlib_s_ImageZoomOut2X.c                \
       mlib_s_ImageZoomOut2X_Fp.c             \
       mlib_s_ImageZoomOut2X_NN.c             \
       mlib_s_ImageZoomOut2X_NN_S32.c         \
       mlib_s_ImageZoomOut2X_NN_Fp.c          \
       mlib_s_ImageZoomOut2X_BL.c             \
       mlib_s_ImageZoomOut2X_BL_S16.c         \
       mlib_s_ImageZoomOut2X_BL_U16.c         \
       mlib_s_ImageZoomOut2X_BL_S32.c         \
       mlib_s_ImageZoomOut2X_BL_F32.c         \
       mlib_s_ImageZoomOut2X_BL_D64.c         \
       mlib_s_ImageZoomOut2X_BC.c             \
       mlib_s_ImageZoomOut2X_BC_S16.c         \
       mlib_s_ImageZoomOut2X_BC_U16.c         \
       mlib_s_ImageZoomOut2X_BC_S32.c         \
       mlib_s_ImageZoomOut2X_BC_F32.c         \
       mlib_s_ImageZoomOut2X_BC_D64.c         \
       mlib_s_ImageZoomOut2X_BC2.c            \
       mlib_s_ImageZoomOut2X_BC2_S16.c        \
       mlib_s_ImageZoomOut2X_BC2_U16.c        \
       mlib_s_ImageZoomOut2X_BC2_S32.c        \
       mlib_s_ImageZoomOut2X_BC2_F32.c        \
       mlib_s_ImageZoomOut2X_BC2_D64.c        \
       mlib_s_ImageZoomIn2X.c                 \
       mlib_s_ImageZoomIn2X_NN.c              \
       mlib_s_ImageZoomIn2X_NN_S32.c          \
       mlib_s_ImageZoomIn2X_NN_D64_Fp.c       \
       mlib_s_ImageRotate90.c                 \
       mlib_s_ImageRotate90_Fp.c              \
       mlib_s_ImageRotate180.c                \
       mlib_s_ImageRotate180_Fp.c             \
       mlib_s_ImageRotate270.c                \
       mlib_s_ImageRotate270_Fp.c             \
       mlib_s_ImageFilteredSubsample.c        \
       mlib_s_ImageFilteredSubsample_U8.c     \
       mlib_s_ImageZoomIn2X_BL.c              \
       mlib_s_ImageZoomIn2X_BL_S32.c          \
       mlib_s_ImageZoomIn2X_BL_Fp.c           \
       mlib_s_ImagePolynomialWarp.c           \
       mlib_s_ImagePolynomialWarp_BC.c        \
       mlib_s_ImagePolynomialWarp_BL.c        \
       mlib_s_ImageFlipMainDiag.c             \
       mlib_s_ImageFlipAntiDiag.c             \
       mlib_s_ImageFlipMainDiag_Fp.c          \
       mlib_s_ImageFlipAntiDiag_Fp.c          \
       mlib_s_ImageFlipY.c                    \
       mlib_s_ImageFlipY_Fp.c                 \
       mlib_s_ImageZoomIn2X_BC_S32.c          \
       mlib_s_ImageZoomIn2X_BC_Fp.c           \
       mlib_s_ImageZoomIn2X_BC.c              \
       mlib_s_ImageZoomIn2X_BC2_S32.c         \
       mlib_s_ImageZoomIn2X_BC2_Fp.c          \
       mlib_s_ImageZoomIn2X_BC2.c             \
       mlib_s_ImageZoom_BL_S32.c              \
       mlib_s_ImageZoom_BL_Fp.c               \
       mlib_s_ImageZoom_BL_Fp_D64.c           \
       mlib_s_ImageZoom_NN.c                  \
       mlib_s_ImageZoom_BC_S32.c              \
       mlib_s_ImageZoom_BC_Fp.c               \
       mlib_s_ImageZoom_BC_Fp_D64.c           \
       mlib_s_ImageAffine_NN.c                \
       mlib_s_ImageAffine_NN_Fp.c             \
       mlib_s_ImageAffine_BL.c                \
       mlib_s_ImageAffine_BC.c                \
       mlib_s_ImageAffine_BL_S16.c            \
       mlib_s_ImageAffine_BC_S16.c            \
       mlib_s_ImageAffine_BL_U16.c            \
       mlib_s_ImageAffine_BC_U16.c            \
       mlib_s_ImageAffine_BL_S32.c            \
       mlib_s_ImageAffine_BL_F32.c            \
       mlib_s_ImageAffine_BL_D64.c            \
       mlib_s_ImageAffine_BC_S32.c            \
       mlib_s_ImageAffine_BC_F32.c            \
       mlib_s_ImageAffine_BC_D64.c

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
INCLUDES += -I$(HOME_TXT)/../radio
INCLUDES += -I$(ALT_DIR)
INCLUDES += -I$(ALT_DIR)/../../mmx
INCLUDES += -I../../sse
INCLUDES += -I$(HOME_TXT)/../../signal/fourier

#
# dependencies
#

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

# SSE2

$(DIR_O)/mlib_s_ImageFlipY_Fp$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_s_ImageZoom_BL$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageFilters.h \
                                                 $(HOME_TXT)/mlib_ImageZoom.h $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                 ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BL_S16$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BL_U16$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BC$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BC_S16$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BC_U16$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoomTranslate$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageZoom.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoomBlend_NN$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoomTranslateBlend$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageZoom.h \
                                                      $(HOME_TXT)/mlib_ImageFilters.h \
                                                      $(ALT_DIR)/mlib_m_ImageFilters.h
$(DIR_O)/mlib_s_ImageZoomTransTable_8nw$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                      $(HOME_TXT)/mlib_ImageZoom.h \
                                                      $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_s_ImageGetZoomFunc$(OBJ_EXT) :          $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                      $(HOME_TXT)/mlib_ImageZoomTranslateTable.h

$(DIR_O)/mlib_s_ImageSubsampleAverage_D64$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageSubsampleAverage_F32$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageSubsampleAverage_S32$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageSubsampleAverage_S16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageSubsampleAverage_U16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageSubsampleAverage_U8$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2_D64$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2_F32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2_S32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2_S16$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC2_U16$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC_S16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BC_U16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h ./mlib_s_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL_S16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_BL_U16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h ./mlib_s_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_s_ImageZoomOut2X_NN$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_NN_D64_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_NN_S32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_NN$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BL$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BL_S32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BL_Fp$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImagePolynomialWarp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h ./mlib_s_ImageFilters.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_s_ImagePolynomialWarp_BC$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_s_ImagePolynomialWarp_BL$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC_Fp$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC_S32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC2_Fp$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC2_S32$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoomIn2X_BC2$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomIn2X.h ./mlib_s_ImageZoomIn2X.h
$(DIR_O)/mlib_s_ImageZoom_BL_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BL_Fp_D64$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BL_S32$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h ./mlib_s_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BC_S32$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_s_ImageZoom_BC_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                            ./mlib_s_ImageZoom_BC.h
$(DIR_O)/mlib_s_ImageZoom_BC_Fp_D64$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                            ./mlib_s_ImageZoom_BC.h

#
# MMX
#

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_m_ImageAffineTable_1ch$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_2ch$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_3ch$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_4ch$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_8nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_s16nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffineTable_u16nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffine_BC$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageFilters.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffine_BC2$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageFilters.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffine_BL$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageFilters.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffine_NN$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageAffine_NN_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageFilteredSubsample_U8$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageFilters$(OBJ_EXT)                :  $(ALT_DIR)/mlib_m_ImageFilters.h
$(DIR_O)/mlib_m_ImageFlipAntiDiag$(OBJ_EXT)           :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageFlipMainDiag$(OBJ_EXT)           :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageGetAffineFunc$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_m_ImageInterpTable$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarpTable$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolyWarpTable_Fp$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                         $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_BC$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_BL$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_Call$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_Fp$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_NN$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_NN_2$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImagePolynomialWarp_NN.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_NN_3$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImagePolynomialWarp_NN.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_NN_4$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImagePolynomialWarp_NN.h
$(DIR_O)/mlib_m_ImagePolynomialWarp_NN_5$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                         $(ALT_DIR)/mlib_m_ImagePolynomialWarp_NN.h
$(DIR_O)/mlib_m_ImageRotate180$(OBJ_EXT)              :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageRotate270$(OBJ_EXT)              :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageRotate90$(OBJ_EXT)               :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoomTranslate$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h
$(DIR_O)/mlib_m_ImageZoomTranslate_Fp$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_m_ImageZoom_BC$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BC_S16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BC_S16_f$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BC_U16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BC_U16_f$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BC_f$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL_S16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL_S16_f$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL_U16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL_U16_f$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_BL_f$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/mlib_m_ImageFilters.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_NN$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageZoom_NN_f$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                         $(HOME_TXT)/mlib_ImageZoom.h \
                                                         $(ALT_DIR)/../../mmx/mlib_mmx_utils.h

#
# STDC
#

$(DIR_O)/mlib_ImageAffine$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineEdge$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineIndex$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineTable$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/mlib_ImageAffineTable.h \
                                                              $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16_3nw$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16ext$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32_3nw$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32ext$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8_3nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64_3nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64ext$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32_3nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32ext$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32nw_2$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16_3nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16ext$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffine_BC_D64$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_F32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_S32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_D64$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_F32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_S32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_NN$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_NN_Bit$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageFilteredSubsample_D64$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_D64_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_F32$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_F32_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_S16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_S16_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_S32$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_S32_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_U16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_U16_f$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_U8$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_U8_f$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilters$(OBJ_EXT)                  :  $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageFlipAntiDiag$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipAntiDiag_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Bit$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipX$(OBJ_EXT)                    :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipX_Fp$(OBJ_EXT)                 :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageFlipY_Bit$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageFlipY.h $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipY_f$(OBJ_EXT)                  :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_ImageGridWarp$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageGridWarpTable$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageGridWarp.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageGridWarpTable_Fp$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageGridWarp_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageInterpTable$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomialWarpTable$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_f$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_ft$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTable_f.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_it$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTable_f.h
$(DIR_O)/mlib_ImagePolynomWarpTable_0$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomWarpTable_0_Fp$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTools$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTools.h
$(DIR_O)/mlib_ImagePolynomialWarp_0$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_0_Fp$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_1$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_Fp$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_S32$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_Fp$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_S32$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Call_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5_Fp$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_Fp$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImageRotate180$(OBJ_EXT)                :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageRotate180_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageRotate270$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate270_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate90$(OBJ_EXT)                 :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate90_Fp$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageScanPoly$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageSubsampleAverage_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageSubsampleAverage_Fp.h
$(DIR_O)/mlib_ImageSubsamBinToGray4x4$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h \
                                                              $(HOME_TXT)/../radio/mlib_c_ImageLookUp.h
$(DIR_O)/mlib_ImageSubsamBinToGray2x2$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h
$(DIR_O)/mlib_ImageSubsampleBinaryToGray$(OBJ_EXT)    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h \
                                                              $(HOME_TXT)/../radio/mlib_c_ImageLookUp.h
$(DIR_O)/mlib_ImageZoomBlendEdge$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomClipping$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdge$(OBJ_EXT)                 :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdgeIndex$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC2_Edge$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC_Edge$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BL_Edge$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_Edge$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_S32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_S32$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_S32$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge_Fp$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_D64_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_S32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomOut2X$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XEdge$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XIndex_Edge$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoom2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_D64$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_F32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_S32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomTranslateTable$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTable_16ext$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_16nw$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_32ext$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_32nw$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8ext$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8nw$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_D64ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_D64nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_F32ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_F32nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTranslateTable_Fp$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTable_u16ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_u16nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTransTableBlend_f.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTransTableBlend_f.h
$(DIR_O)/mlib_ImageZoomTransTableBlend$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTableBlEdge$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTranslateToGray$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomTranslate_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoom_BC_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h
$(DIR_O)/mlib_ImageZoom_BC_Fp_D64$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h
$(DIR_O)/mlib_ImageZoom_BL_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_Fp_D64$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_S32$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_Fp$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoom_NN$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h

#
# C
#
$(DIR_O)/mlib_c_ImageAffineIndex_BC$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffineIndex_BL$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BC$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BC_S16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BC_U16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BL$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BL_S16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_BL_U16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageAffine_NN$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_c_ImageFilteredSubsample$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h
$(DIR_O)/mlib_c_ImageFilteredSubsample_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h
$(DIR_O)/mlib_c_ImageFlipMainDiag_f$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_c_ImageGetAffineFunc$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_c_ImageGetZoomFunc$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_c_ImagePolynomialWarp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_c_ImagePolynomialWarp_BC$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_c_ImagePolynomialWarp_BL$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_c_ImagePolynomialWarp_Call$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_c_ImageSubsampleAverage$(OBJ_EXT)       :  $(HOME_TXT)/mlib_c_ImageSubsampleAverage.h
$(DIR_O)/mlib_c_ImageSubsampleAverage_S16$(OBJ_EXT)   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_c_ImageSubsampleAverage_U16$(OBJ_EXT)   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_c_ImageSubsampleAverage_U8$(OBJ_EXT)    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_c_ImageZoomBlend_BC$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BC.h $(HOME_TXT)/../../include/mlib_Utils.h
$(DIR_O)/mlib_c_ImageZoomBlend_BL$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BL.h
$(DIR_O)/mlib_c_ImageZoomBlend_NN$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_c_ImageZoomIn2X$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_c_ImageZoomIn2XIndex$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_c_ImageZoomIn2XIndex_BC$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_c_ImageZoomIn2XIndex_BC2$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_c_ImageZoomIn2XIndex_BL$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_c_ImageZoomIn2X_BC$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_c_ImageZoomIn2X_BC2$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_c_ImageZoomIn2X_NN$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_c_ImageZoomIndex$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_c_ImageZoomIndex_BC$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoomIndex_BC.h
$(DIR_O)/mlib_c_ImageZoomIndex_BL$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoomIndex_BL.h
$(DIR_O)/mlib_c_ImageZoomOut2XIndex$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoom2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC2_S16$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC2_U16$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC_S16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BC_U16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BL$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BL_S16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_c_ImageZoomOut2X_BL_U16$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_c_ImageZoomOut2X_NN$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_c_ImageZoomTranslate$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_c_ImageZoomTranslateBlend$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomTranslateBlend$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_c_ImageZoom_BC$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BC.h
$(DIR_O)/mlib_c_ImageZoom_BC_S16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BC.h
$(DIR_O)/mlib_c_ImageZoom_BC_S32$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_c_ImageZoom_BC_U16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BC.h
$(DIR_O)/mlib_c_ImageZoom_BL$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BL.h
$(DIR_O)/mlib_c_ImageZoom_BL_S16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BL.h
$(DIR_O)/mlib_c_ImageZoom_BL_U16$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/../blend/mlib_c_ImageBlendTable.h \
                                                              $(HOME_TXT)/mlib_c_ImageZoom_BL.h
$(DIR_O)/mlib_c_ImageZoom_NN_f$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
