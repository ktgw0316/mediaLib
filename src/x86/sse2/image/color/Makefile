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
# ident	"@(#)Makefile	9.8	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/color

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/color

# common source under src/stdc/generic
UTILS_STDC =

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageColorHSL2RGB_Fp.c \
               mlib_ImageColorHSV2RGB_Fp.c \
               mlib_ImageColorRGB2HSL_Fp.c \
               mlib_ImageColorRGB2HSV_Fp.c \
               mlib_ImageColorRGB2Mono_Fp.c

# C specific source under src/stdc/generic

SRCS_STDC = mlib_c_ImageColorTables.c

# MMX specific source under src/x86/mmx
SRCS_MMX =


# SSE2 specific source under src/x86/sse2

SRCS_SSE2 = 	mlib_s_ImageColorConvert_Fp.c	\
                mlib_s_ImageColorRGB2Mono.c	\
		mlib_s_ImageColorConvert.c	\
		mlib_s_ImageColorConvert1_f.c	\
		mlib_s_ImageColorConvert2_f.c	\
		mlib_s_ImageColorRGB2XYZ.c	\
		mlib_s_ImageColorRGB2YCC.c	\
		mlib_s_ImageColorXYZ2RGB.c	\
		mlib_s_ImageColorYCC2RGB.c	\
		mlib_s_ImageColorRGB2HSL.c	\
		mlib_s_ImageColorRGB2HSV.c	\
		mlib_s_ImageColorHSL2RGB.c	\
		mlib_s_ImageColorHSV2RGB.c

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


$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_s_ImageColorConvert$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_s_ImageColorRGB2HSV$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_s_ImageColorRGB2HSL$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_s_ImageColorHSV2RGB$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_s_ImageColorHSL2RGB$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorTables$(OBJ_EXT)    : $(HOME_TXT)/mlib_c_ImageColorTables.h
