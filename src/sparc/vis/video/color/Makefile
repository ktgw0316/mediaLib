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
MODNAME = video

ALT_MOD_AL = algebra
ALT_MOD = image
ALT_UTILS_MOD = $(ALT_MOD)
STDC_ALT_MOD = $(ALT_UTILS_MOD)

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/color

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

ALT_DIR = $(TOP_DIR)/src/sparc/vis/$(ALT_MOD)/blend
ALT_UTILS_DIR = $(TOP_DIR)/src/sparc/vis/$(ALT_UTILS_MOD)/geom
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/geom

SRCS_V  = mlib_v_VideoColorABGR2JFIFYCC420.c \
          mlib_v_VideoColorABGR2JFIFYCC422.c \
          mlib_v_VideoColorABGR2JFIFYCC444.c \
          mlib_v_VideoColorABGR2RGB.c \
          mlib_v_VideoColorARGB2JFIFYCC420.c \
          mlib_v_VideoColorARGB2JFIFYCC422.c \
          mlib_v_VideoColorARGB2JFIFYCC444.c \
          mlib_v_VideoColorARGB2RGB.c \
          mlib_v_VideoColorAVD422int.c \
          mlib_v_VideoColorAVDABGR.c \
          mlib_v_VideoColorAVDBlendUtil.c \
          mlib_v_VideoColorAVDBlendUtil_RA.c \
          mlib_v_VideoColorAVDResize_BC.c \
          mlib_v_VideoColorAVDResize_BL.c \
          mlib_v_VideoColorAVDUYV422int.c \
          mlib_v_VideoColorAVDYUV411.c \
          mlib_v_VideoColorAVDYUV420.c \
          mlib_v_VideoColorAVDYUV422.c \
          mlib_v_VideoColorAVDYUV444.c \
          mlib_v_VideoClrCMYK2JFIFYCCK444.c \
          mlib_v_VideoColorJFIFYCC2ABGR444.c \
          mlib_v_VideoColorJFIFYCC2ARGB444.c \
          mlib_v_VideoColorJFIFYCC2RGB420.c \
          mlib_v_VideoJFIFYCC2RGB420_NN.c \
          mlib_v_VideoColorJFIFYCC2RGB422.c \
          mlib_v_VideoJFIFYCC2RGB422_NN.c \
          mlib_v_VideoColorJFIFYCC2RGB444.c \
          mlib_v_VideoJFIFYCC2RGB444_S16.c \
          mlib_v_VideoClrJFIFYCCK2CMYK444.c \
          mlib_v_VideoColorMerge2.c \
          mlib_v_VideoColorMerge2_S16.c \
          mlib_v_VideoColorMerge3.c \
          mlib_v_VideoColorMerge3_S16.c \
          mlib_v_VideoColorMerge4.c \
          mlib_v_VideoColorMerge4_S16.c \
          mlib_v_VideoColorRGB2ABGR.c \
          mlib_v_VideoColorRGB2ARGB.c \
          mlib_v_VideoColorRGB2JFIFYCC420.c \
          mlib_v_VideoColorRGB2JFIFYCC422.c \
          mlib_v_VideoColorRGB2JFIFYCC444.c \
          mlib_v_VideoRGB2JFIFYCC444_S16.c \
          mlib_v_VideoColorSplit2.c \
          mlib_v_VideoColorSplit2_S16.c \
          mlib_v_VideoColorSplit3.c \
          mlib_v_VideoColorSplit3_S16.c \
          mlib_v_VideoColorSplit4.c \
          mlib_v_VideoColorSplit4_S16.c \
          mlib_v_VideoColorYUV2ABGR411.c \
          mlib_v_VideoColorYUV2ABGR420.c \
          mlib_v_VideoColorYUV2ABGR420_W.c \
          mlib_v_VideoColorYUV2ABGR420_WX2.c \
          mlib_v_VideoColorYUV2ABGR420_WX3.c \
          mlib_v_VideoColorYUV2ABGR420_X2.c \
          mlib_v_VideoColorYUV2ABGR420_X3.c \
          mlib_v_VideoColorYUV2ABGR422.c \
          mlib_v_VideoColorYUV2ABGR444.c \
          mlib_v_VideoColorYUV2ARGB411.c \
          mlib_v_VideoColorYUV2ARGB420.c \
          mlib_v_VideoColorYUV2ARGB422.c \
          mlib_v_VideoColorYUV2ARGB444.c \
          mlib_v_VideoColorYUV2RGB411.c \
          mlib_v_VideoColorYUV2RGB420.c \
          mlib_v_VideoColorYUV2RGB422.c \
          mlib_v_VideoColorYUV2RGB444.c \
          mlib_v_VideoDownSample420.c \
          mlib_v_VideoDownSample420_S16.c \
          mlib_v_VideoDownSample422.c \
          mlib_v_VideoDownSample422_S16.c \
          mlib_v_VideoUpSample420.c \
          mlib_v_VideoUpSample420_S16.c \
          mlib_v_VideoUpSample422.c \
          mlib_v_VideoUpSample422_S16.c\
	  mlib_v_VideoColorBGR2JFIFYCC420.c\
	  mlib_v_VideoColorBGR2JFIFYCC422.c\
	  mlib_v_VideoColorBGR2JFIFYCC444.c\
	  mlib_v_VideoBGR2JFIFYCC444_S16.c

SRCS_STDC_ALT = mlib_ImageFilters.c
UTILS_ALT_V   = mlib_v_ImageFilters.c
SRCS_ALT_V    = mlib_v_ImageBlendTable.c


include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(TOP_DIR)/src/sparc/vis/$(ALT_UTILS_MOD)/blend \
            -I$(HOME_TXT) -I$(HOME_ALT_TXT)\
            -I$(HOME_ALT_TXT)/../copy -I$(ALT_UTILS_DIR)

# define depends

$(DIR_O)/../$(ALT_UTILS_MOD)/mlib_v_ImageFilters$(OBJ_EXT) : $(ALT_UTILS_DIR)/mlib_v_ImageFilters.h
$(DIR_O)/../$(STDC_ALT_MOD)/mlib_ImageFilters$(OBJ_EXT)    : $(HOME_ALT_TXT)/mlib_ImageFilters.h

$(DIR_O)/mlib_VideoColorAVDBlend$(OBJ_EXT)          : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA$(OBJ_EXT)       : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_Inp$(OBJ_EXT)      : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA_Inp$(OBJ_EXT)   : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h

$(DIR_O)/mlib_v_VideoColorAVDBlendUtil$(OBJ_EXT)    : $(ALT_DIR)/mlib_v_ImageBlendTable.h $(HOME_ALT_TXT)/../copy/mlib_ImageCopy.h \
                                                      $(HOME_TXT)/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_v_VideoColorAVDBlendUtil_RA$(OBJ_EXT) : $(ALT_DIR)/mlib_v_ImageBlendTable.h $(HOME_ALT_TXT)/../copy/mlib_ImageCopy.h \
                                                      $(HOME_TXT)/mlib_VideoColorAVDBlend_proto.h

$(DIR_O)/mlib_VideoColorAVDResize_NN$(OBJ_EXT)      : ${HOME_TXT}/mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_VideoColorAVDResize$(OBJ_EXT)         : ${HOME_TXT}/mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_v_VideoColorAVDResize_BC$(OBJ_EXT)    : ${HOME_TXT}/mlib_VideoColorAVDResize.h ${ALT_UTILS_DIR}/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_VideoColorAVDResize_BL$(OBJ_EXT)    : ${HOME_TXT}/mlib_VideoColorAVDResize.h
