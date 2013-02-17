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
MODNAME = video

ALT_MOD_AL = algebra
ALT_MOD = image

ALT_UTILS_MOD = $(ALT_MOD)
STDC_ALT_MOD = $(ALT_UTILS_MOD)

ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/blend
ALT_UTILS_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_UTILS_MOD)/common

HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/geom

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS =   mlib_VideoColorAVDBlend.c \
          mlib_VideoColorAVDBlend_Inp.c \
          mlib_VideoColorAVDBlend_RA.c \
          mlib_VideoColorAVDBlend_RA_Inp.c \
          mlib_VideoColorAVDResize_NN.c \
          mlib_VideoColorAVDResize.c

SRCS_C  = mlib_c_VideoColorAVD422int.c \
          mlib_c_VideoColorAVDABGR.c \
          mlib_c_VideoColorAVDBlendUtil.c \
          mlib_c_VideoColorAVDBlendUtil_RA.c \
          mlib_c_VideoColorAVDResize_BC.c \
          mlib_c_VideoColorAVDResize_BL.c \
          mlib_c_VideoColorAVDUYV422int.c \
          mlib_c_VideoColorAVDYUV411.c \
          mlib_c_VideoColorAVDYUV420.c \
          mlib_c_VideoColorAVDYUV422.c \
          mlib_c_VideoColorAVDYUV444.c \
          mlib_c_VideoColorABGR2JFIFYCC420.c \
          mlib_c_VideoColorABGR2JFIFYCC422.c \
          mlib_c_VideoColorABGR2JFIFYCC444.c \
          mlib_c_VideoColorABGR2RGB.c \
          mlib_c_VideoColorARGB2JFIFYCC420.c \
          mlib_c_VideoColorARGB2JFIFYCC422.c \
          mlib_c_VideoColorARGB2JFIFYCC444.c \
          mlib_c_VideoColorARGB2RGB.c \
          mlib_c_VideoClrCMYK2JFIFYCCK444.c \
          mlib_c_VideoColorJFIFYCC2ABGR444.c \
          mlib_c_VideoColorJFIFYCC2ARGB444.c \
          mlib_c_VideoColorJFIFYCC2RGB420.c \
          mlib_c_VideoJFIFYCC2RGB420_NN.c \
          mlib_c_VideoColorJFIFYCC2RGB422.c \
          mlib_c_VideoJFIFYCC2RGB422_NN.c \
          mlib_c_VideoColorJFIFYCC2RGB444.c \
          mlib_c_VideoJFIFYCC2RGB444_S16.c \
          mlib_c_VideoClrJFIFYCCK2CMYK444.c \
          mlib_c_VideoColorMerge2.c \
          mlib_c_VideoColorMerge2_S16.c \
          mlib_c_VideoColorMerge3.c \
          mlib_c_VideoColorMerge3_S16.c \
          mlib_c_VideoColorMerge4.c \
          mlib_c_VideoColorMerge4_S16.c \
          mlib_c_VideoColorRGB2ABGR.c \
          mlib_c_VideoColorRGB2ARGB.c \
          mlib_c_VideoColorBGR2JFIFYCC420.c \
          mlib_c_VideoColorBGR2JFIFYCC422.c \
          mlib_c_VideoColorBGR2JFIFYCC444.c \
          mlib_c_VideoBGR2JFIFYCC444_S16.c \
          mlib_c_VideoColorRGB2JFIFYCC420.c \
          mlib_c_VideoColorRGB2JFIFYCC422.c \
          mlib_c_VideoColorRGB2JFIFYCC444.c \
          mlib_c_VideoRGB2JFIFYCC444_S16.c \
          mlib_c_VideoColorSplit2.c \
          mlib_c_VideoColorSplit2_S16.c \
          mlib_c_VideoColorSplit3.c \
          mlib_c_VideoColorSplit3_S16.c \
          mlib_c_VideoColorSplit4.c \
          mlib_c_VideoColorSplit4_S16.c \
          mlib_c_VideoColorTables.c \
          mlib_c_VideoColorYUV2ABGR411.c \
          mlib_c_VideoColorYUV2ABGR420.c \
          mlib_c_VideoColorYUV2ABGR420_W.c \
          mlib_c_VideoColorYUV2ABGR420_WX2.c \
          mlib_c_VideoColorYUV2ABGR420_WX3.c \
          mlib_c_VideoColorYUV2ABGR420_X2.c \
          mlib_c_VideoColorYUV2ABGR420_X3.c \
          mlib_c_VideoColorYUV2ABGR422.c \
          mlib_c_VideoColorYUV2ABGR444.c \
          mlib_c_VideoColorYUV2ARGB411.c \
          mlib_c_VideoColorYUV2ARGB420.c \
          mlib_c_VideoColorYUV2ARGB422.c \
          mlib_c_VideoColorYUV2ARGB444.c \
          mlib_c_VideoColorYUV2RGB411.c \
          mlib_c_VideoColorYUV2RGB420.c \
          mlib_c_VideoColorYUV2RGB422.c \
          mlib_c_VideoColorYUV2RGB444.c \
          mlib_c_VideoDownSample420.c \
          mlib_c_VideoDownSample420_S16.c \
          mlib_c_VideoDownSample422.c \
          mlib_c_VideoDownSample422_S16.c \
          mlib_c_VideoUpSample420.c \
          mlib_c_VideoUpSample420_S16.c \
          mlib_c_VideoUpSample422.c \
          mlib_c_VideoUpSample422_S16.c


SRCS_STDC_ALT = mlib_ImageFilters.c
UTILS_ALT = mlib_ImageDivTables.c
SRCS_ALT = mlib_c_ImageBlendTable.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../../image/blend \
            -I./../../image/include \
	    -I./../../image/geom

# define depends


$(DIR_O)/mlib_VideoColorAVDBlend$(OBJ_EXT)                 :  ./mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_Inp$(OBJ_EXT)             :  ./mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA$(OBJ_EXT)              :  ./mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA_Inp$(OBJ_EXT)          :  ./mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDResize$(OBJ_EXT)                :  ./mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_VideoColorAVDResize_NN$(OBJ_EXT)             :  ./mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_c_VideoColorABGR2JFIFYCC420$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorABGR2JFIFYCC422$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorABGR2JFIFYCC444$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorARGB2JFIFYCC420$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorARGB2JFIFYCC422$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorARGB2JFIFYCC444$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorAVDBlendUtil$(OBJ_EXT)           :  ./../../image/include/mlib_ImageDivTables.h ./mlib_VideoColorAVDBlend_proto.h \
                                                              ./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_VideoColorAVDBlendUtil_RA$(OBJ_EXT)        :  ./../../image/include/mlib_ImageDivTables.h ./mlib_VideoColorAVDBlend_proto.h \
                                                              ./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_VideoColorAVDResize_BC$(OBJ_EXT)           :  ./../../image/include/mlib_ImageDivTables.h ./../../image/geom/mlib_ImageFilters.h \
                                                              ./mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_c_VideoColorAVDResize_BL$(OBJ_EXT)           :  ./../../image/include/mlib_ImageDivTables.h ./../../image/geom/mlib_ImageFilters.h \
                                                              ./mlib_VideoColorAVDResize.h \
                                                              ./../../image/blend/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_VideoColorAVDYUV411$(OBJ_EXT)              :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorAVDYUV420$(OBJ_EXT)              :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorAVDYUV422$(OBJ_EXT)              :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorAVDYUV444$(OBJ_EXT)              :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoClrCMYK2JFIFYCCK444$(OBJ_EXT)       :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorJFIFYCC2ABGR444$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorJFIFYCC2ARGB444$(OBJ_EXT)        :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoJFIFYCC2RGB420_NN$(OBJ_EXT) :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorJFIFYCC2RGB422$(OBJ_EXT)         :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoJFIFYCC2RGB422_NN$(OBJ_EXT) :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorJFIFYCC2RGB444$(OBJ_EXT)         :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoClrJFIFYCCK2CMYK444$(OBJ_EXT)       :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorRGB2JFIFYCC420$(OBJ_EXT)         :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorRGB2JFIFYCC422$(OBJ_EXT)         :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorRGB2JFIFYCC444$(OBJ_EXT)         :  ./mlib_c_VideoColorJFIFYCC.h
$(DIR_O)/mlib_c_VideoColorTables$(OBJ_EXT)                 :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR411$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420_W$(OBJ_EXT)          :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420_WX2$(OBJ_EXT)        :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420_WX3$(OBJ_EXT)        :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420_X2$(OBJ_EXT)         :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR420_X3$(OBJ_EXT)         :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR422$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ABGR444$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ARGB411$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ARGB420$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ARGB422$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2ARGB444$(OBJ_EXT)            :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2RGB411$(OBJ_EXT)             :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2RGB420$(OBJ_EXT)             :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2RGB422$(OBJ_EXT)             :  ./mlib_c_VideoColorYUV.h
$(DIR_O)/mlib_c_VideoColorYUV2RGB444$(OBJ_EXT)             :  ./mlib_c_VideoColorYUV.h
