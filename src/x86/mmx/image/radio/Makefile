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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/radio

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

SRCS_STDC_FP = mlib_ImageReplaceColor_Fp.c        \
               mlib_ImageThresh1_Fp.c             \
               mlib_ImageThresh1_F32.c            \
               mlib_ImageThresh1_D64.c            \
               mlib_ImageThresh2_Fp.c             \
               mlib_ImageThresh3_Fp.c             \
               mlib_ImageThresh4_Fp.c             \
               mlib_ImageThresh5_Fp.c             \
               mlib_ImageThresh_Fp_Inp.c

SRCS_STDC  = mlib_c_ImageColorErrDiff3x3.c            \
             mlib_c_ImageClrOrdDither8x8.c            \
             mlib_c_ImageReplaceColor.c               \
             mlib_c_ImageLookUpMask.c                 \
             mlib_c_ImageThresh1.c                    \
             mlib_c_ImageThresh1_U8.c                 \
             mlib_c_ImageThresh1_S16.c                \
             mlib_c_ImageThresh1_U16.c                \
             mlib_c_ImageThresh1_S32.c                \
             mlib_c_ImageThresh2.c                    \
             mlib_c_ImageThresh3.c                    \
             mlib_c_ImageClrOrdDitherMxN.c            \
             mlib_c_ImageColorErrDiffMxN_Bit.c        \
             mlib_c_ImageClrOrdDitherMxN_Bit.c

win32_SRCS =   mlib_m_ImageHistogram2.c             \
               mlib_m_ImageHistogram2_f.c           \
               mlib_m_ImageThresh5.c                \
               mlib_m_ImageThresh4.c                \
               mlib_m_ImageLookUp.c                 \
               mlib_m_ImageLookUp_8.c               \
               mlib_m_ImageLookUp_16.c              \
               mlib_m_ImageLookUp_32.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/../include -I$(HOME_TXT) -I$(HOME_TXT)/../copy -I$(HOME_INCLUDE)/../../mmx

#
# define depends
#

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O_STDC)/mlib_ImageColorErrorDiffusionMxN$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageColorErrorDiffusionMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_ImageReplaceColor_Fp$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageReplaceColor.h
$(DIR_O_STDC)/mlib_ImageColorTrue2Index$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageColormap.h  \
                                                            $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_ImageColorTrue2IndexInit$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_ImageHistogram2_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O_STDC)/mlib_ImageLookUpMask_64$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O_STDC)/mlib_ImageLookUpMask_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O_STDC)/mlib_ImageLookUp_64$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O_STDC)/mlib_ImageLookUp_Bit$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O_STDC)/mlib_ImageThresh1_D64$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_ImageThresh1_F32$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_ImageThresh1_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_c_ImageColorErrDiffMxN_Bit$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_c_ImageClrOrdDither8x8$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_c_ImageClrOrdDitherMxN$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_c_ImageClrOrdDitherMxN_Bit$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h  \
                                                            $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O_STDC)/mlib_c_ImageReplaceColor$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageReplaceColor.h
$(DIR_O_STDC)/mlib_c_ImageHistogram2$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O_STDC)/mlib_c_ImageLookUp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h \
                                                            $(HOME_TXT)/mlib_c_ImageLookUp.h
$(DIR_O_STDC)/mlib_c_ImageLookUpMask$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O_STDC)/mlib_c_ImageLookUp_f$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h \
                                                            $(HOME_TXT)/mlib_c_ImageLookUp.h
$(DIR_O_STDC)/mlib_c_ImageThresh1$(OBJ_EXT)              :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_c_ImageThresh1_S16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_c_ImageThresh1_S32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_c_ImageThresh1_U16$(OBJ_EXT)          :  $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O_STDC)/mlib_c_ImageThresh1_U8$(OBJ_EXT)           :  $(HOME_TXT)/mlib_c_ImageThresh1.h

$(ARCH_DIR_O)/mlib_m_ImageHistogram2$(OBJ_EXT)   :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageLookUp$(OBJ_EXT)       :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageLookUp_16$(OBJ_EXT)    :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageLookUp_32$(OBJ_EXT)    :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageLookUp_8$(OBJ_EXT)     :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageThresh4$(OBJ_EXT)      :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageThresh5$(OBJ_EXT)      :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
