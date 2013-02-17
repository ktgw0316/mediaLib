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
# ident	"@(#)Makefile	9.19	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/spatial

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/spatial

UTILS_STDC = mlib_ImageClipping.c               \
             mlib_ImageConv2x2_f.c              \
             mlib_ImageConvMxN_ext.c            \
             mlib_ImageConv3x3_Bit.c            \
             mlib_ImageConv_8nw.c               \
             mlib_ImageConv_8ext.c              \
             mlib_ImageConv_16ext.c             \
             mlib_ImageConv_16nw.c              \
             mlib_ImageConv_u16nw.c             \
             mlib_ImageConv_u16ext.c            \
             mlib_ImageConv_32ext.c             \
             mlib_ImageConv_32nw.c              \
             mlib_ImageConv_D64ext.c            \
             mlib_ImageConv_D64nw.c             \
             mlib_ImageConv_F32ext.c            \
             mlib_ImageConv_F32nw.c             \
             mlib_ImageConvClearEdge_Bit.c      \
             mlib_ImageConvCopyEdge_Bit.c       \
             mlib_ImageDilate4.c                \
             mlib_ImageDilate8.c                \
             mlib_ImageErode4.c                 \
             mlib_ImageErode8.c                 \
             mlib_ImageDilate4_1nw.c            \
             mlib_ImageErode4_1nw.c             \
             mlib_ImageGradient3x3.c            \
             mlib_ImageGradientMxN.c            \
             mlib_ImageMaxFilter3x3_1nw.c       \
             mlib_ImageMaxFilter3x3.c           \
             mlib_ImageMaxFilter5x5.c           \
             mlib_ImageMaxFilter7x7.c           \
             mlib_ImageMinFilter3x3_1nw.c       \
             mlib_ImageMinFilter3x3.c           \
             mlib_ImageMinFilter5x5.c           \
             mlib_ImageMinFilter7x7.c           \
             mlib_ImageMedianFilterFunc.c       \
             mlib_ImageMedianFilterMxN.c        \
             mlib_ImageMedianFilterMxNFunc.c    \
             mlib_ImageMedianFilter_US.c        \
             mlib_ImageRankFilter5x5.c          \
             mlib_ImageRankFilter7x7.c          \
             mlib_ImageRankFilterMxN.c          \
             mlib_ImageRankFilterMxN_U16.c      \
             mlib_ImageRankFilterMxN_S16.c      \
             mlib_ImageRankFilter_US.c          \
             mlib_ImageSConv_32nw.c             \
             mlib_ImageSConv_D64nw.c            \
             mlib_ImageSConv_F32nw.c            \
             mlib_ImageSConv_32ext.c            \
             mlib_ImageSConv_D64ext.c           \
             mlib_ImageSConv_F32ext.c           \
             mlib_ImageSobel.c                  \
             mlib_ImageSqrtTable_U16.c          \
             mlib_ImageConvIndex_16_16ext.c     \
             mlib_ImageConvIndex_16_16nw.c      \
             mlib_ImageConvIndex_16_8ext.c      \
             mlib_ImageConvIndex_16_8nw.c       \
             mlib_ImageConvIndex_8_16ext.c      \
             mlib_ImageConvIndex_8_16nw.c       \
             mlib_ImageConvIndex_8_8ext.c       \
             mlib_ImageConvIndex_8_8nw.c        \
             mlib_ImageConvMxNIndex.c           \
             mlib_ImageConvKernelConvert.c      \
             mlib_ImageConvolveMxN.c            \
             mlib_ImageSConvKernelConvert.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageConv7x7_Fp.c           \
               mlib_ImageConvClearEdge_Fp.c     \
               mlib_ImageConvCopyEdge_Fp.c      \
               mlib_ImageConvMxN_Fp.c           \
               mlib_ImageConvolveMxN_Fp.c       \
               mlib_ImageDilate4_Fp.c           \
               mlib_ImageDilate8_Fp.c           \
               mlib_ImageErode4_Fp.c            \
               mlib_ImageErode8_Fp.c            \
               mlib_ImageGradient3x3_Fp.c       \
               mlib_ImageGradientMxN_Fp.c       \
               mlib_ImageMaxFilter3x3_Fp.c      \
               mlib_ImageMaxFilter5x5_Fp.c      \
               mlib_ImageMaxFilter7x7_Fp.c      \
               mlib_ImageMinFilter3x3_Fp.c      \
               mlib_ImageMinFilter5x5_Fp.c      \
               mlib_ImageMinFilter7x7_Fp.c      \
               mlib_ImageMedianFilter3x3_Fp.c   \
               mlib_ImageMedianFilter5x5_Fp.c   \
               mlib_ImageMedianFilter7x7_Fp.c   \
               mlib_ImageMedianFilterMxN_Fp.c   \
               mlib_ImageRankFilter3x3_Fp.c     \
               mlib_ImageRankFilter5x5_Fp.c     \
               mlib_ImageRankFilter7x7_Fp.c     \
               mlib_ImageRankFilterMxN_Fp.c     \
               mlib_ImageSobel_Fp.c

# C specific source under src/stdc/generic
SRCS_STDC =  mlib_c_ImageConv_f.c               \
             mlib_c_ImageConvClearEdge.c        \
             mlib_c_ImageConvCopyEdge.c         \
             mlib_c_ImageGradientMxNFunc.c      \
             mlib_c_ImageRankFilter3x3.c        \
             mlib_c_ImageSConv_16nw.c           \
             mlib_c_ImageSConv_8nw.c            \
             mlib_c_ImageSConv_16ext.c          \
             mlib_c_ImageSConv_8ext.c           \
             mlib_c_ImageSConv_u16nw.c          \
             mlib_c_ImageSConv_u16ext.c         \
             mlib_c_ImageVisVersion.c

# MMX specific source under src/x86/mmx
SRCS_MMX =   mlib_m_ImageConv7x7.c              \
             mlib_m_ImageGradient3x3Func.c      \
             mlib_m_ImageConvVersion.c

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageConv2x2.c              \
            mlib_s_ImageConv3x3.c              \
            mlib_s_ImageConv4x4.c              \
            mlib_s_ImageConv5x5.c              \
            mlib_s_ImageConvMxN.c              \
            mlib_s_ImageConv2x2_8nw.c          \
            mlib_s_ImageConv2x2_16nw.c         \
            mlib_s_ImageConv2x2_u16nw.c        \
            mlib_s_ImageConv3x3_8nw.c          \
            mlib_s_ImageConv3x3_16nw.c         \
            mlib_s_ImageConv3x3_u16nw.c        \
            mlib_s_ImageConv4x4_8nw.c          \
            mlib_s_ImageConv4x4_16nw.c         \
            mlib_s_ImageConv4x4_u16nw.c        \
            mlib_s_ImageConv5x5_8nw.c          \
            mlib_s_ImageConv5x5_8nw_34.c       \
            mlib_s_ImageConv5x5_16nw.c         \
            mlib_s_ImageConv5x5_u16nw.c        \
            mlib_s_ImageConvMxN_8nw.c          \
            mlib_s_ImageConvMxN_16nw.c         \
            mlib_s_ImageConvMxN_u16nw.c        \
            mlib_s_ImageSConv3x3_8nw.c         \
            mlib_s_ImageSConv3x3_16nw.c        \
            mlib_s_ImageSConv3x3_u16nw.c       \
            mlib_s_ImageSConv5x5_8nw.c         \
            mlib_s_ImageSConv5x5_16nw.c        \
            mlib_s_ImageSConv5x5_u16nw.c       \
            mlib_s_ImageSConv7x7_8nw.c         \
            mlib_s_ImageSConv7x7_16nw.c        \
            mlib_s_ImageSConv7x7_u16nw.c       \
            mlib_s_ImageDilate4_32nw.c         \
            mlib_s_ImageDilate4_16nw.c         \
            mlib_s_ImageDilate4_u16nw.c        \
            mlib_s_ImageDilate4_8nw.c          \
            mlib_s_ImageErode4_32nw.c          \
            mlib_s_ImageErode4_16nw.c          \
            mlib_s_ImageErode4_u16nw.c         \
            mlib_s_ImageErode4_8nw.c           \
            mlib_s_ImageMaxFilter3x3_8nw.c     \
            mlib_s_ImageMaxFilter3x3_16nw.c    \
            mlib_s_ImageMaxFilter3x3_u16nw.c   \
            mlib_s_ImageMaxFilter3x3_32nw.c    \
            mlib_s_ImageMaxFilter5x5_8nw.c     \
            mlib_s_ImageMaxFilter5x5_16nw.c    \
            mlib_s_ImageMaxFilter5x5_u16nw.c   \
            mlib_s_ImageMaxFilter5x5_32nw.c    \
            mlib_s_ImageMaxFilter7x7_8nw.c     \
            mlib_s_ImageMaxFilter7x7_16nw.c    \
            mlib_s_ImageMaxFilter7x7_u16nw.c   \
            mlib_s_ImageMaxFilter7x7_32nw.c    \
            mlib_s_ImageMinFilter3x3_16nw.c    \
            mlib_s_ImageMinFilter3x3_u16nw.c   \
            mlib_s_ImageMinFilter3x3_32nw.c    \
            mlib_s_ImageMinFilter3x3_8nw.c     \
            mlib_s_ImageMinFilter5x5_16nw.c    \
            mlib_s_ImageMinFilter5x5_u16nw.c   \
            mlib_s_ImageMinFilter5x5_32nw.c    \
            mlib_s_ImageMinFilter5x5_8nw.c     \
            mlib_s_ImageMinFilter7x7_16nw.c    \
            mlib_s_ImageMinFilter7x7_u16nw.c   \
            mlib_s_ImageMinFilter7x7_32nw.c    \
            mlib_s_ImageMinFilter7x7_8nw.c     \
            mlib_s_ImageMedianFilterFunc.c     \
            mlib_s_ImageMedianFilter3x3.c      \
            mlib_s_ImageMedianFilter3x3Func.c  \
            mlib_s_ImageMedianFilter5x5.c      \
            mlib_s_ImageMedianFilter5x5Func.c  \
            mlib_s_ImageMedianFilter7x7.c      \
            mlib_s_ImageSConv3x3.c             \
            mlib_s_ImageSConv5x5.c             \
            mlib_s_ImageSConv7x7.c             \
            mlib_s_ImageSobel.c                \
            mlib_s_ImageConv2x2Index.c         \
            mlib_s_ImageConv3x3Index.c         \
            mlib_s_ImageConv4x4Index.c         \
            mlib_s_ImageConv5x5Index.c         \
            mlib_s_ImageConv7x7Index.c         \
            mlib_s_ImageConvMxNIndex.c         \
            mlib_s_ImageConvIndex3_8_8nw.c     \
            mlib_s_ImageConvIndex3_8_8ext.c    \
            mlib_s_ImageConvIndex3_8_16nw.c    \
            mlib_s_ImageConvIndex3_8_16ext.c   \
            mlib_s_ImageConvIndex4_8_8nw.c     \
            mlib_s_ImageConvIndex4_8_8ext.c    \
            mlib_s_ImageConvIndex4_8_16nw.c    \
            mlib_s_ImageConvIndex4_8_16ext.c   \
            mlib_s_ImageMaxFilter3x3_F32.c     \
            mlib_s_ImageMaxFilter3x3_D64.c     \
            mlib_s_ImageMaxFilter5x5_F32.c     \
            mlib_s_ImageMaxFilter5x5_D64.c     \
            mlib_s_ImageMaxFilter7x7_F32.c     \
            mlib_s_ImageMaxFilter7x7_D64.c     \
            mlib_s_ImageMinFilter3x3_F32.c     \
            mlib_s_ImageMinFilter3x3_D64.c     \
            mlib_s_ImageMinFilter5x5_F32.c     \
            mlib_s_ImageMinFilter5x5_D64.c     \
            mlib_s_ImageMinFilter7x7_F32.c     \
            mlib_s_ImageMinFilter7x7_D64.c     \
            mlib_s_ImageErode4_F32.c           \
            mlib_s_ImageErode4_D64.c           \
            mlib_s_ImageDilate4_F32.c          \
            mlib_s_ImageDilate4_D64.c          \
            mlib_s_ImageConv2x2_8ext.c         \
            mlib_s_ImageConv2x2_16ext.c        \
            mlib_s_ImageConv2x2_u16ext.c       \
            mlib_s_ImageConv3x3_8ext.c         \
            mlib_s_ImageConv3x3_16ext.c        \
            mlib_s_ImageConv3x3_u16ext.c       \
            mlib_s_ImageConv4x4_8ext.c         \
            mlib_s_ImageConv4x4_16ext.c        \
            mlib_s_ImageConv4x4_u16ext.c       \
            mlib_s_ImageConv5x5_8ext.c         \
            mlib_s_ImageConv5x5_16ext.c        \
            mlib_s_ImageConv5x5_u16ext.c       \
            mlib_s_ImageSConv3x3_8ext.c        \
            mlib_s_ImageSConv3x3_16ext.c       \
            mlib_s_ImageSConv3x3_u16ext.c      \
            mlib_s_ImageSConv5x5_8ext.c        \
            mlib_s_ImageSConv5x5_16ext.c       \
            mlib_s_ImageSConv5x5_u16ext.c      \
            mlib_s_ImageSConv7x7_8ext.c        \
            mlib_s_ImageSConv7x7_16ext.c       \
            mlib_s_ImageSConv7x7_u16ext.c      \
            mlib_s_ImageConv2x2_Fp.c           \
            mlib_s_ImageConv3x3_Fp.c           \
            mlib_s_ImageConv4x4_Fp.c           \
            mlib_s_ImageConv5x5_Fp.c           \
            mlib_s_ImageConv2x2_d64ext.c       \
            mlib_s_ImageConv2x2_d64nw.c        \
            mlib_s_ImageConv2x2_f32ext.c       \
            mlib_s_ImageConv2x2_f32nw.c        \
            mlib_s_ImageConv3x3_d64ext.c       \
            mlib_s_ImageConv3x3_d64nw.c        \
            mlib_s_ImageConv3x3_f32ext.c       \
            mlib_s_ImageConv3x3_f32nw.c        \
            mlib_s_ImageConv4x4_d64ext.c       \
            mlib_s_ImageConv4x4_d64nw.c        \
            mlib_s_ImageConv4x4_f32ext.c       \
            mlib_s_ImageConv4x4_f32nw.c        \
            mlib_s_ImageConv5x5_d64ext.c       \
            mlib_s_ImageConv5x5_d64nw.c        \
            mlib_s_ImageConv5x5_f32ext.c       \
            mlib_s_ImageConv5x5_f32nw.c        \
            mlib_s_ImageSConv3x3_Fp.c          \
            mlib_s_ImageSConv5x5_Fp.c          \
            mlib_s_ImageSConv7x7_Fp.c          \
            mlib_s_ImageSConv3x3_d64ext.c      \
            mlib_s_ImageSConv3x3_d64nw.c       \
            mlib_s_ImageSConv3x3_f32ext.c      \
            mlib_s_ImageSConv3x3_f32nw.c       \
            mlib_s_ImageSConv5x5_d64ext.c      \
            mlib_s_ImageSConv5x5_d64nw.c       \
            mlib_s_ImageSConv5x5_f32ext.c      \
            mlib_s_ImageSConv5x5_f32nw.c       \
            mlib_s_ImageSConv7x7_d64ext.c      \
            mlib_s_ImageSConv7x7_d64nw.c       \
            mlib_s_ImageSConv7x7_f32ext.c      \
            mlib_s_ImageSConv7x7_f32nw.c


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

UTIL_HEADERS = ../../sse/mlib_sse_utils.h ../../../mmx/mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_s_ImageConv2x2_8nw$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_16nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_8nw$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_16nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_8nw$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_16nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_8nw$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_8nw_34$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_16nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConvMxN_8nw$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConvMxN_16nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConvMxN_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_8ext$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_16ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_u16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_8ext$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_16ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_u16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_8ext$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_16ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_u16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_8ext$(OBJ_EXT)      : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_16ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_u16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_8nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_u16nw$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_8nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_u16nw$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_8nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_16nw$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_u16nw$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_8ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_u16ext$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_8ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_u16ext$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_8ext$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_16ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_u16ext$(OBJ_EXT)   : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h \
                                                  $(HOME_TXT)/mlib_ImageClipping.h \
                                                  $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                  $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageConv3x3$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h \
                                                  $(HOME_TXT)/mlib_ImageClipping.h \
                                                  $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                  $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageConv4x4$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h \
                                                  $(HOME_TXT)/mlib_ImageClipping.h \
                                                  $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                  $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageConv5x5$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h \
                                                  $(HOME_TXT)/mlib_ImageClipping.h \
                                                  $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                  $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageConvMxN$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h \
                                                  $(HOME_TXT)/mlib_ImageClipping.h \
                                                  $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                  $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageConv2x2_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageClipping.h \
	                                          $(HOME_TXT)/mlib_ImageConv.h \
                                              $(HOME_TXT)/mlib_ImageConvEdge.h \
                                              $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageConv3x3_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageClipping.h \
	                                          $(HOME_TXT)/mlib_ImageConv.h \
                                              $(HOME_TXT)/mlib_ImageConvEdge.h \
                                              $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageConv4x4_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageClipping.h \
	                                          $(HOME_TXT)/mlib_ImageConv.h \
                                              $(HOME_TXT)/mlib_ImageConvEdge.h \
                                              $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageConv5x5_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageClipping.h \
	                                          $(HOME_TXT)/mlib_ImageConv.h \
                                              $(HOME_TXT)/mlib_ImageConvEdge.h \
                                              $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageConv2x2_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv2x2_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv3x3_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv4x4_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageConv5x5_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_Fp$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageClipping.h \
	                                         $(HOME_TXT)/mlib_ImageConvEdge.h \
                                             $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                             $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageSConv5x5_Fp$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageClipping.h \
	                                         $(HOME_TXT)/mlib_ImageConvEdge.h \
                                             $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                             $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageSConv7x7_Fp$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageClipping.h \
	                                         $(HOME_TXT)/mlib_ImageConvEdge.h \
                                             $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                             $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_s_ImageSConv3x3_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv5x5_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_f32ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_f32nw$(OBJ_EXT)     : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_d64ext$(OBJ_EXT)    : $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv7x7_d64nw$(OBJ_EXT)     : $(UTIL_HEADERS)

$(DIR_O)/mlib_s_ImageErode4_16nw$(OBJ_EXT)      : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageErode4_32nw$(OBJ_EXT)      : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageErode4_8nw$(OBJ_EXT)       : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageErode4_u16nw$(OBJ_EXT)     : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageDilate4_16nw$(OBJ_EXT)     : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageDilate4_32nw$(OBJ_EXT)     : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageDilate4_8nw$(OBJ_EXT)      : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageDilate4_u16nw$(OBJ_EXT)    : $(UTIL_HEADERS) ./mlib_s_ImageDilateErode4.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_16nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_32nw$(OBJ_EXT)   :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_8nw$(OBJ_EXT)    :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_u16nw$(OBJ_EXT)  :  $(UTIL_HEADERS) ./mlib_s_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_s_ImageMedianFilter3x3$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageMedianFilter3x3Func$(OBJ_EXT) :  $(UTIL_HEADERS) ./mlib_s_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageMedianFilter5x5$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageMedianFilter5x5Func$(OBJ_EXT) :  $(UTIL_HEADERS) ./mlib_s_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageMedianFilter7x7$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageMedianFilterFunc$(OBJ_EXT)    :  $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_s_ImageSobel$(OBJ_EXT)               :  $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageGradient3x3Func$(OBJ_EXT)     :  $(UTIL_HEADERS)
$(DIR_O)/mlib_s_ImageSConv3x3$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageSConv5x5$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_s_ImageSConv7x7$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h

$(DIR_O)/mlib_s_ImageConvIndex3_8_8nw$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex3_8_8ext$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex3_8_16nw$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex3_8_16ext$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex4_8_8nw$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex4_8_8ext$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex4_8_16nw$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvIndex4_8_16ext$(OBJ_EXT)     :  $(UTIL_HEADERS) ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMaxFilter3x3_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMaxFilter5x5_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMaxFilter7x7_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageMinFilter3x3_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageMinFilter5x5_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_F32$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageMinFilter7x7_D64$(OBJ_EXT)    : $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_s_ImageErode4_D64$(OBJ_EXT)          :  $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_s_ImageErode4_F32$(OBJ_EXT)          :  $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_s_ImageDilate4_D64$(OBJ_EXT)         :  $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_s_ImageDilate4_F32$(OBJ_EXT)         :  $(UTIL_HEADERS) $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_s_ImageConv2x2Index$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConv3x3Index$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConv4x4Index$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConv5x5Index$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConv7x7Index$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h
$(DIR_O)/mlib_s_ImageConvMxNIndex$(OBJ_EXT)     : ./mlib_s_ImageConvIndex.h

$(DIR_O)/mlib_m_ImageDilate4_16nw$(OBJ_EXT)        :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageDilate4_32nw$(OBJ_EXT)        :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageDilate4_8nw$(OBJ_EXT)         :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageDilate4_u16nw$(OBJ_EXT)       :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageErode4_16nw$(OBJ_EXT)         :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageErode4_32nw$(OBJ_EXT)         :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageErode4_8nw$(OBJ_EXT)          :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageErode4_u16nw$(OBJ_EXT)        :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageGradient3x3Func$(OBJ_EXT)     :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter3x3_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter3x3_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter3x3_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter3x3_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter5x5_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter5x5_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter5x5_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter5x5_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter7x7_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter7x7_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter7x7_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMaxFilter7x7_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilter3x3$(OBJ_EXT)     :  $(ALT_DIR)/mlib_m_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilter3x3Func$(OBJ_EXT) :  $(ALT_DIR)/mlib_m_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilter5x5$(OBJ_EXT)     :  $(ALT_DIR)/mlib_m_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilter5x5Func$(OBJ_EXT) :  $(ALT_DIR)/mlib_m_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilter7x7$(OBJ_EXT)     :  $(ALT_DIR)/mlib_m_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMedianFilterFunc$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMedianFilter.h $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter3x3_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter3x3_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter3x3_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter3x3_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter5x5_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter5x5_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter5x5_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter5x5_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter7x7_16nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter7x7_32nw$(OBJ_EXT)   :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter7x7_8nw$(OBJ_EXT)    :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMinFilter7x7_u16nw$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageSobel$(OBJ_EXT)               :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h


$(DIR_O)/mlib_ImageClipping$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2_f$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Bit$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv5x5$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv5x5_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv7x7$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConvClearEdge_Bit$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvClearEdge_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Bit$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvIndex_16_16ext$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_16nw$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8ext$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8nw$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvKernelConvert$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConvMxNIndex$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageClipping.h
$(DIR_O)/mlib_ImageConvMxN_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN_ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_16ext$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_16nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_32ext$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_32nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_8ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_8nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_u16ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_u16nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConvolveMxN$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageDilate4$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_1nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_32nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_D64$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_F32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageErode4$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_1nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_32nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_D64$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_F32$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_Fp$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageGradient3x3$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradient3x3_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageGradientFunc.h
$(DIR_O)/mlib_ImageGradientMxN$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradientMxN_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageMaxFilter3x3$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_1nw$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3Func$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5Func$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterFunc$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxNFunc$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter_US$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_1nw$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageRankFilter3x3_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageRankFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageMedianFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN_S16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_U16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilter_US$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageSConv3x3$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv3x3_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConvKernelConvert$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageSConv_32ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_32nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSobel$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageSobel.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageSobel_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageSqrtTable_U16$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageSqrtTable.h

#
# C
#
$(DIR_O)/mlib_c_ImageConv2x2Index$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConv3x3Index$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConv4x4Index$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConv5x5Index$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConv7x7Index$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConvClearEdge$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_c_ImageConvCopyEdge$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_c_ImageConvMxNIndex$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_c_ImageConv_f$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_c_ImageDilate4_16nw$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageDilate4_8nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageDilate4_u16nw$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageErode4_16nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageErode4_8nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageErode4_u16nw$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageDilate4.h $(HOME_TXT)/mlib_ImageErode4.h \
                                                     $(HOME_TXT)/mlib_c_ImageDilateErode4.h
$(DIR_O)/mlib_c_ImageGradient3x3Func$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageGradientFunc.h
$(DIR_O)/mlib_c_ImageGradientMxNFunc$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageGradientFunc.h
$(DIR_O)/mlib_c_ImageMaxFilter3x3_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMaxFilter3x3_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMaxFilter3x3_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMaxFilter5x5_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMaxFilter5x5_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMaxFilter5x5_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMaxFilter7x7_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageMaxFilter7x7_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageMaxFilter7x7_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageMedianFilterFunc$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_c_ImageMinFilter3x3_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMinFilter3x3_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMinFilter3x3_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter3x3.h
$(DIR_O)/mlib_c_ImageMinFilter5x5_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMinFilter5x5_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMinFilter5x5_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter5x5.h
$(DIR_O)/mlib_c_ImageMinFilter7x7_16nw$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageMinFilter7x7_8nw$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageMinFilter7x7_u16nw$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageMaxFilter.h $(HOME_TXT)/mlib_ImageMinFilter.h \
                                                     $(HOME_TXT)/mlib_c_ImageMaxMinFilter7x7.h
$(DIR_O)/mlib_c_ImageRankFilter3x3$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageRankFilter.h
$(DIR_O)/mlib_c_ImageSConv_16ext$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSConv_16nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSConv_8ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSConv_8nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSConv_u16ext$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSConv_u16nw$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_c_ImageSobel$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageSobel.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_c_ImageVisVersion$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageVisVersion.h
