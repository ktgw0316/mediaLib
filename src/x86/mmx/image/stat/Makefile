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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/stat

UTILS_STDC   = mlib_ImageExtremaLocations.c

SRCS_STDC_FP = mlib_ImageAutoCorrel_Fp.c          \
               mlib_ImageCrossCorrel_Fp.c         \
               mlib_ImageNormCrossCorrel_Fp.c     \
               mlib_ImageExtrema2_Fp.c            \
               mlib_ImageExtremaLocations_Fp.c    \
               mlib_ImageMaximum_Fp.c             \
               mlib_ImageMean_Fp.c                \
               mlib_ImageMinimum_Fp.c             \
               mlib_ImageMoment2_Fp.c             \
               mlib_ImageStdDev_Fp.c              \
               mlib_ImageXProj_Fp.c               \
               mlib_ImageYProj_Fp.c

SRCS_STDC    = mlib_c_ImageAutoCorrel.c           \
               mlib_c_ImageCrossCorrel.c          \
               mlib_c_ImageNormCrossCorrel.c      \
               mlib_c_ImageMoment2.c              \
               mlib_c_ImageStdDev.c               \
               mlib_c_ImageXProj.c                \
               mlib_c_ImageYProj.c

win32_SRCS  =  mlib_m_ImageMean.c                 \
               mlib_m_ImageExtrema2.c             \
               mlib_m_ImageMinimum.c              \
               mlib_m_ImageMaximum.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_INCLUDE)/../../mmx

#
# define depends
#

$(OBJS) $(OBJS_STDC) : $(MLIB_HOME)/src/generic/stdc/image/include/mlib_ImageCheck.h

$(ARCH_DIR_O)/mlib_m_ImageExtrema2$(OBJ_EXT) :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageMaximum$(OBJ_EXT)  :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageMean$(OBJ_EXT)     :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageMinimum$(OBJ_EXT)  :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h

$(DIR_O_STDC)/mlib_ImageAutoCorrel_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageCorrel.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O_STDC)/mlib_ImageCrossCorrel_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageCorrel.h
$(DIR_O_STDC)/mlib_ImageExtrema2$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_ImageExtrema2_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O_STDC)/mlib_ImageExtremaLocations$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O_STDC)/mlib_ImageExtremaLocations_Fp$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O_STDC)/mlib_ImageMaximumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_ImageMinimumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_ImageMoment2_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageMoment2.h
$(DIR_O_STDC)/mlib_ImageStdDev_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageMoment2.h

$(DIR_O_STDC)/mlib_c_ImageAutoCorrel$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_c_ImageCorrel.h
$(DIR_O_STDC)/mlib_c_ImageCrossCorrel$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageCorrel.h
$(DIR_O_STDC)/mlib_c_ImageExtrema2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_c_ImageMaximum$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_c_ImageMinimum$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O_STDC)/mlib_c_ImageMoment2$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageMoment2.h
$(DIR_O_STDC)/mlib_c_ImageStdDev$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_c_ImageMoment2.h
