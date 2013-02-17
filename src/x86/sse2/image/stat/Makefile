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
# ident	"@(#)Makefile	9.15	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/stat

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/stat

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageExtremaLocations.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageExtremaLocations_Fp.c

# C specific source under src/stdc/generic
SRCS_STDC =

# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageMinimum.c              \
            mlib_s_ImageMaximum.c              \
            mlib_s_ImageMaximum_Fp.c           \
            mlib_s_ImageMinimum_Fp.c           \
            mlib_s_ImageXProj.c                \
            mlib_s_ImageYProj.c                \
            mlib_s_ImageXProj_Fp.c             \
            mlib_s_ImageYProj_Fp.c             \
            mlib_s_ImageMean.c                 \
            mlib_s_ImageMean_Fp.c              \
            mlib_s_ImageExtrema2.c             \
            mlib_s_ImageExtrema2_Fp.c          \
            mlib_s_ImageMoment2.c              \
            mlib_s_ImageMoment2_Fp.c           \
            mlib_s_ImageStdDev.c               \
            mlib_s_ImageStdDev_Fp.c            \
            mlib_s_ImageCrossCorrel.c          \
            mlib_s_ImageCrossCorrel_Fp.c       \
            mlib_s_ImageAutoCorrel.c           \
            mlib_s_ImageAutoCorrel_Fp.c        \
            mlib_s_ImageNormCrossCorrel.c      \
            mlib_s_ImageNormCrossCorrel_Fp.c

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

$(DIR_O)/mlib_m_ImageExtrema2$(OBJ_EXT) :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageMean$(OBJ_EXT)     :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h

$(DIR_O)/mlib_ImageAutoCorrel_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageCorrel.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageCrossCorrel_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageCorrel.h
$(DIR_O)/mlib_ImageExtrema2$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageExtrema2_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageExtremaLocations$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageExtremaLocations_Fp$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageMaximumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageMinimumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageMoment2_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageMoment2.h
$(DIR_O)/mlib_ImageStdDev_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageMoment2.h

$(DIR_O)/mlib_c_ImageAutoCorrel$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_c_ImageCorrel.h
$(DIR_O)/mlib_c_ImageCrossCorrel$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageCorrel.h
$(DIR_O)/mlib_c_ImageExtrema2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMaximum$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMinimum$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMoment2$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageMoment2.h
$(DIR_O)/mlib_c_ImageStdDev$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageMoment2.h

