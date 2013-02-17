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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  =  mlib_ImageColorErrorDiffusionMxN.c \
          mlib_ImageColorTrue2IndexInit.c \
          mlib_ImageColorTrue2Index.c \
          mlib_ImageHistogram.c \
          mlib_ImageHistogram2_f.c \
          mlib_ImageLookUp2.c \
          mlib_ImageLookUpMask_f.c \
          mlib_ImageLookUpMask_64.c \
          mlib_ImageLookUp_64.c \
          mlib_ImageLookUp_Bit.c \
          mlib_ImageThresh_Inp.c \
          mlib_ImageLookUp_Inp.c

SRCS_FP = mlib_ImageReplaceColor_Fp.c \
          mlib_ImageThresh1_Fp.c \
          mlib_ImageThresh1_F32.c \
          mlib_ImageThresh1_D64.c \
          mlib_ImageThresh2_Fp.c \
          mlib_ImageThresh3_Fp.c \
          mlib_ImageThresh4_Fp.c \
          mlib_ImageThresh5_Fp.c \
          mlib_ImageThresh_Fp_Inp.c

SRCS_C  = mlib_c_ImageColorErrDiff3x3.c \
          mlib_c_ImageColorErrDiffMxN_Bit.c \
          mlib_c_ImageClrOrdDither8x8.c \
          mlib_c_ImageClrOrdDitherMxN.c \
          mlib_c_ImageClrOrdDitherMxN_Bit.c \
          mlib_c_ImageReplaceColor.c \
          mlib_c_ImageHistogram2.c \
          mlib_c_ImageLookUp.c \
          mlib_c_ImageLookUp_f.c \
          mlib_c_ImageLookUpMask.c \
          mlib_c_ImageThresh1.c \
          mlib_c_ImageThresh1_U8.c \
          mlib_c_ImageThresh1_S16.c \
          mlib_c_ImageThresh1_U16.c \
          mlib_c_ImageThresh1_S32.c \
          mlib_c_ImageThresh2.c \
          mlib_c_ImageThresh3.c \
          mlib_c_ImageThresh4.c \
          mlib_c_ImageThresh5.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../copy -I./../include

#
# define depends
#

$(OBJS) : ./../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageColorErrorDiffusionMxN$(OBJ_EXT)       :  ./mlib_ImageColorErrorDiffusionMxN.h ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_ImageReplaceColor_Fp$(OBJ_EXT)              :  ./mlib_ImageReplaceColor.h
$(DIR_O)/mlib_ImageColorTrue2Index$(OBJ_EXT)              :  ./../include/mlib_ImageColormap.h ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageColorTrue2IndexInit$(OBJ_EXT)          :  ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_ImageHistogram2_f$(OBJ_EXT)                 :  ./mlib_ImageHistogram.h
$(DIR_O)/mlib_ImageLookUpMask_64$(OBJ_EXT)                :  ./mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUpMask_f$(OBJ_EXT)                 :  ./mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUp_64$(OBJ_EXT)                    :  ./../copy/mlib_ImageCopy.h ./mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageLookUp_Bit$(OBJ_EXT)                   :  ./../copy/mlib_ImageCopy.h ./mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageThresh1_D64$(OBJ_EXT)                  :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_F32$(OBJ_EXT)                  :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_Fp$(OBJ_EXT)                   :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageColorErrDiffMxN_Bit$(OBJ_EXT) :  ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageClrOrdDither8x8$(OBJ_EXT)      :  ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageClrOrdDitherMxN$(OBJ_EXT)      :  ./mlib_ImageColorOrderedDitherMxN.h ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageClrOrdDitherMxN_Bit$(OBJ_EXT)  :  ./mlib_ImageColorOrderedDitherMxN.h ./../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageReplaceColor$(OBJ_EXT)               :  ./mlib_ImageReplaceColor.h
$(DIR_O)/mlib_c_ImageHistogram2$(OBJ_EXT)                 :  ./mlib_ImageHistogram.h
$(DIR_O)/mlib_c_ImageLookUp$(OBJ_EXT)                     :  ./../copy/mlib_ImageCopy.h ./mlib_ImageLookUp.h \
                                                             ./mlib_c_ImageLookUp.h
$(DIR_O)/mlib_c_ImageLookUpMask$(OBJ_EXT)                 :  ./mlib_ImageLookUpMask.h
$(DIR_O)/mlib_c_ImageLookUp_f$(OBJ_EXT)                   :  ./../copy/mlib_ImageCopy.h ./mlib_ImageLookUp.h \
                                                             ./mlib_c_ImageLookUp.h
$(DIR_O)/mlib_c_ImageThresh1$(OBJ_EXT)                    :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_S16$(OBJ_EXT)                :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_S32$(OBJ_EXT)                :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_U16$(OBJ_EXT)                :  ./mlib_c_ImageThresh1.h
$(DIR_O)/mlib_c_ImageThresh1_U8$(OBJ_EXT)                 :  ./mlib_c_ImageThresh1.h
