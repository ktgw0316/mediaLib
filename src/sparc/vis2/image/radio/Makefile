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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/radio
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/radio

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in $(UTILS_RM); do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n SRCS="" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

UTILS_RM  = mlib_ImageHistogram.c

SRCS_V = mlib_v_ImageHistogram.c            \
         mlib_v_ImageLookUpU8U8Func.c       \
         mlib_v_ImageLookUpU8S16Func.c      \
         mlib_v_ImageLookUpS16U8Func.c      \
         mlib_v_ImageLookUpS16S16Func.c     \
         mlib_v_ImageLookUpU16S16Func.c     \
         mlib_v_ImageLookUpU16U8Func.c      \
         mlib_v_ImageLookUpS32U8Func.c      \
         mlib_v_ImageLookUpS32S16Func.c     \
         mlib_v_ImageLookUpSIU8U8Func.c     \
         mlib_v_ImageLookUpSIU8S16Func.c    \
         mlib_v_ImageLookUpSIS16U8Func.c    \
         mlib_v_ImageLookUpSIU16U8Func.c    \
         mlib_v_ImageLookUpSIS16S16Func.c   \
         mlib_v_ImageLookUpSIU16S16Func.c   \
         mlib_v_ImageLookUpSIS32U8Func.c    \
         mlib_v_ImageLookUpSIS32S16Func.c   \
         mlib_v_ImageLookUpMaskU8U8Func.c   \
         mlib_v_ImageLookUpMaskSIU8U8Func.c \
         mlib_v_ImageLookUpMaskS16U8Func.c  \
         mlib_v_ImageLookUpMaskSIS16U8_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_VIS_TXT) -I$(HOME_TXT) -I$(HOME_TXT)/../copy -I$(HOME_TXT)/../include

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

# C

$(DIR_O)/mlib_ImageColorErrorDiffusionMxN$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageColorErrorDiffusionMxN.h
$(DIR_O)/mlib_ImageReplaceColor_Fp$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageReplaceColor.h
$(DIR_O)/mlib_ImageColorTrue2Index$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageThreshFunc.h
$(DIR_O)/mlib_ImageHistogram2_f$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O)/mlib_ImageLookUpMask_64$(OBJ_EXT)            : $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUpMask_f$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUp_64$(OBJ_EXT)                : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageLookUp_Bit$(OBJ_EXT)               : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageThresh1_D64$(OBJ_EXT)              : $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_F32$(OBJ_EXT)              : $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_Fp$(OBJ_EXT)               : $(HOME_TXT)/mlib_c_ImageThresh1.h

# VIS

$(DIR_O)/mlib_v_ImageColorErrDiff3x3$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_v_ImageColorErrorDiffusionMxN$(OBJ_EXT) : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_v_ImageColorErrDiffMxN_Bit$(OBJ_EXT)    : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_v_ImageClrOrdDither8x8$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_v_ImageClrOrdDitherMxN$(OBJ_EXT)        : $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                        $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h
$(DIR_O)/mlib_v_ImageClrOrdDitherMxN_Bit$(OBJ_EXT)    : $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                        $(HOME_TXT)/mlib_ImageColorOrderedDitherMxN.h
$(DIR_O)/mlib_v_ImageReplaceColor$(OBJ_EXT)           : ./mlib_v_ImageConstOper.h
$(DIR_O)/mlib_v_ImageHistogram2$(OBJ_EXT)             : $(HOME_VIS_TXT)/mlib_v_ImageHistogram2.h
$(DIR_O)/mlib_v_ImageHistogram2_f$(OBJ_EXT)           : $(HOME_VIS_TXT)/mlib_v_ImageHistogram2.h
$(DIR_O)/mlib_v_ImageLookUp$(OBJ_EXT)                 : $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                        $(HOME_TXT)/mlib_ImageLookUp.h  \
                                                        $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpMask$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageLookUpMask.h \
                                                        $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpMaskS16U8Func$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpMaskSIS16U8_f$(OBJ_EXT)  : $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpMaskSIS32U8Func$(OBJ_EXT)  : $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpMaskSIU8U8Func$(OBJ_EXT)   : $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpMaskU8U8Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpMaskFunc.h
$(DIR_O)/mlib_v_ImageLookUpS16S32Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpS32S32Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS16S32Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS32S32Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU16S32Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU8S32Func$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU16S32Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU8S32Func$(OBJ_EXT)        : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageThresh1$(OBJ_EXT)                : ./mlib_v_ImageConstOper.h \
                                                        $(HOME_VIS_TXT)/mlib_v_ImageThreshFunc.h
$(DIR_O)/mlib_v_ImageThresh1BFunc$(OBJ_EXT)           : $(HOME_VIS_TXT)/mlib_v_ImageThreshFunc.h
$(DIR_O)/mlib_v_ImageThresh2$(OBJ_EXT)                : ./mlib_v_ImageConstOper.h
$(DIR_O)/mlib_v_ImageThresh3$(OBJ_EXT)                : ./mlib_v_ImageConstOper.h
$(DIR_O)/mlib_v_ImageThresh4$(OBJ_EXT)                : ./mlib_v_ImageConstOper.h
$(DIR_O)/mlib_v_ImageThresh5$(OBJ_EXT)                : ./mlib_v_ImageConstOper.h

# VIS2

$(DIR_O)/mlib_v_ImageLookUpS16S16Func$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpS16U8Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpS32S16Func$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpS32U8Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS16S16Func$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS16U8Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS32S16Func$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIS32U8Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU16S16Func$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU16U8Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU8S16Func$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpSIU8U8Func$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU16S16Func$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU16U8Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU8S16Func$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_v_ImageLookUpU8U8Func$(OBJ_EXT)        : $(HOME_VIS_TXT)/mlib_v_ImageLookUpFunc.h
