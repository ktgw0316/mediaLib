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
# ident	"@(#)Makefile	9.18	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

ALT_MOD_AL = algebra
ALT_MOD = image

ALT_UTILS_MOD = $(ALT_MOD)
STDC_ALT_MOD = $(ALT_UTILS_MOD)

ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/blend
ALT_UTILS_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_UTILS_MOD)/common

HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/geom

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/color

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS_STDC =   mlib_VideoColorAVDBlend.c \
          mlib_VideoColorAVDBlend_Inp.c \
          mlib_VideoColorAVDBlend_RA.c \
          mlib_VideoColorAVDBlend_RA_Inp.c \
          mlib_VideoColorAVDResize_NN.c \
          mlib_VideoColorAVDResize.c

SRCS_STDC  = mlib_c_VideoColorAVDResize_BC.c \
          mlib_c_VideoColorTables.c

SRCS_SSE2 =	mlib_s_VideoColorABGR2JFIFYCC444.c	\
		mlib_s_VideoColorABGR2RGB.c				\
		mlib_s_VideoColorARGB2JFIFYCC444.c		\
		mlib_s_VideoColorARGB2RGB.c				\
		mlib_s_VideoColorBGR2JFIFYCC420.c		\
		mlib_s_VideoColorBGR2JFIFYCC422.c		\
		mlib_s_VideoColorBGR2JFIFYCC444.c		\
		mlib_s_VideoColorJFIFYCC2ABGR444.c		\
		mlib_s_VideoColorJFIFYCC2ARGB444.c		\
		mlib_s_VideoColorJFIFYCC2RGB444.c		\
		mlib_s_VideoColorRGB2ABGR.c				\
		mlib_s_VideoColorRGB2ARGB.c				\
          	mlib_s_VideoColorAVDABGR.c \
          	mlib_s_VideoColorAVDYUV411.c \
          	mlib_s_VideoColorAVDYUV420.c \
          	mlib_s_VideoColorAVDYUV422.c \
          	mlib_s_VideoColorAVDYUV444.c \
          	mlib_s_VideoColorAVDUYV422int.c \
          	mlib_s_VideoColorAVDResize_BL.c \
		mlib_s_VideoColorAVD422int.c	\
		mlib_s_VideoColorRGB2JFIFYCC420.c		\
		mlib_s_VideoColorRGB2JFIFYCC422.c		\
		mlib_s_VideoColorRGB2JFIFYCC444.c		\
		mlib_s_VideoColorYUV2ABGR411.c			\
		mlib_s_VideoColorYUV2ABGR420.c			\
		mlib_s_VideoColorYUV2ABGR422.c			\
		mlib_s_VideoColorYUV2ABGR444.c			\
		mlib_s_VideoColorYUV2ARGB411.c			\
		mlib_s_VideoColorYUV2ARGB420.c			\
		mlib_s_VideoColorYUV2ARGB422.c			\
		mlib_s_VideoColorYUV2ARGB444.c			\
		mlib_s_VideoColorYUV2RGB411.c			\
		mlib_s_VideoColorYUV2RGB420.c			\
		mlib_s_VideoColorYUV2RGB422.c			\
		mlib_s_VideoColorYUV2RGB444.c			\
		mlib_s_VideoColorABGR2JFIFYCC420.c      \
		mlib_s_VideoColorARGB2JFIFYCC420.c      \
		mlib_s_VideoColorJFIFYCC2RGB420.c       \
		mlib_s_VideoJFIFYCC2RGB422_NN.c         \
		mlib_s_VideoColorABGR2JFIFYCC422.c      \
		mlib_s_VideoColorARGB2JFIFYCC422.c      \
		mlib_s_VideoJFIFYCC2RGB420_NN.c		\
        	mlib_s_VideoColorJFIFYCC2RGB422.c	\
        	mlib_s_VideoDownSample420.c \
        	mlib_s_VideoDownSample422.c \
        	mlib_s_VideoUpSample420.c \
        	mlib_s_VideoUpSample422.c \
          	mlib_s_VideoDownSample420_S16.c \
          	mlib_s_VideoDownSample422_S16.c \
          	mlib_s_VideoUpSample420_S16.c \
          	mlib_s_VideoUpSample422_S16.c \
		mlib_s_VideoClrCMYK2JFIFYCCK444.c \
		mlib_s_VideoClrJFIFYCCK2CMYK444.c \
		mlib_s_VideoColorYUV2ABGR420_W.c  \
        	mlib_s_VideoColorYUV2ABGR420_X2.c \
        	mlib_s_VideoColorYUV2ABGR420_X3.c \
        	mlib_s_VideoColorYUV2ABGR420_WX2.c \
        	mlib_s_VideoColorYUV2ABGR420_WX3.c \
        	mlib_s_VideoColorSplit2.c \
        	mlib_s_VideoColorSplit2_S16.c \
        	mlib_s_VideoColorSplit3.c \
        	mlib_s_VideoColorSplit3_S16.c \
        	mlib_s_VideoColorSplit4.c \
        	mlib_s_VideoColorSplit4_S16.c \
        	mlib_s_VideoColorMerge2.c \
        	mlib_s_VideoColorMerge2_S16.c \
        	mlib_s_VideoColorMerge3.c \
        	mlib_s_VideoColorMerge3_S16.c \
        	mlib_s_VideoColorMerge4.c \
        	mlib_s_VideoColorMerge4_S16.c	\
        	mlib_s_VideoColorAVDBlendUtil.c	\
        	mlib_s_VideoColorAVDBlendUtil_RA.c	\
        	mlib_s_VideoJFIFYCC2RGB444_S16.c \
        	mlib_s_VideoBGR2JFIFYCC444_S16.c \
        	mlib_s_VideoRGB2JFIFYCC444_S16.c

SRCS_STDC_ALT = mlib_ImageFilters.c
UTILS_ALT = mlib_ImageDivTables.c
SRCS_ALT = mlib_c_ImageBlendTable.c

#SRCS = $($(TARCH)_SRCS)
SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../../image/blend \
            -I$(HOME_TXT)/./../../image/include \
	    -I$(HOME_TXT)/./../../image/geom -I$(HOME_INCLUDE)/../../sse

# define depends


$(DIR_O_STDC)/mlib_VideoColorAVDBlend$(OBJ_EXT)                 :  $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h
$(DIR_O_STDC)/mlib_VideoColorAVDBlend_Inp$(OBJ_EXT)             :  $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h
$(DIR_O_STDC)/mlib_VideoColorAVDBlend_RA$(OBJ_EXT)              :  $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h
$(DIR_O_STDC)/mlib_VideoColorAVDBlend_RA_Inp$(OBJ_EXT)          :  $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h
$(DIR_O_STDC)/mlib_VideoColorAVDResize$(OBJ_EXT)                :  $(HOME_TXT)/./mlib_VideoColorAVDResize.h
$(DIR_O_STDC)/mlib_VideoColorAVDResize_NN$(OBJ_EXT)             :  $(HOME_TXT)/./mlib_VideoColorAVDResize.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDBlendUtil$(OBJ_EXT)           :  $(HOME_TXT)/./../../image/include/mlib_ImageDivTables.h $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h \
                                                              $(HOME_TXT)/./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDBlendUtil_RA$(OBJ_EXT)        :  $(HOME_TXT)/./../../image/include/mlib_ImageDivTables.h $(HOME_TXT)/./mlib_VideoColorAVDBlend_proto.h \
                                                              $(HOME_TXT)/./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDResize_BC$(OBJ_EXT)           :  $(HOME_TXT)/./../../image/include/mlib_ImageDivTables.h $(HOME_TXT)/./../../image/geom/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/./mlib_VideoColorAVDResize.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDResize_BL$(OBJ_EXT)           :  $(HOME_TXT)/./../../image/include/mlib_ImageDivTables.h $(HOME_TXT)/./../../image/geom/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/./mlib_VideoColorAVDResize.h \
                                                              $(HOME_TXT)/./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDYUV411$(OBJ_EXT)              :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDYUV420$(OBJ_EXT)              :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDYUV422$(OBJ_EXT)              :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorAVDYUV444$(OBJ_EXT)              :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorTables$(OBJ_EXT)                 :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorYUV2ABGR420_W$(OBJ_EXT)          :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorYUV2ABGR420_WX2$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h
$(DIR_O_STDC)/mlib_c_VideoColorYUV2ABGR420_WX3$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_c_VideoColorYUV.h

