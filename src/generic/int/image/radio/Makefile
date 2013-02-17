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
# ident	"@(#)Makefile	9.3	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/radio

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =		mlib_ImageColorErrorDiffusionMxN.c	\
		mlib_ImageColorTrue2Index.c		\
		mlib_ImageHistogram.c			\
		mlib_ImageHistogram2_f.c		\
		mlib_ImageLookUp_64.c			\
		mlib_ImageLookUpMask_64.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP = 	mlib_ImageReplaceColor_Fp.c	\
		mlib_ImageThresh1_D64.c		\
		mlib_ImageThresh1_F32.c		\
		mlib_ImageThresh2_Fp.c		\
		mlib_ImageThresh3_Fp.c		\
		mlib_ImageThresh4_Fp.c		\
		mlib_ImageThresh5_Fp.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT =	mlib_i_ImageClrOrdDitherMxN_Bit.c	\
		mlib_i_ImageClrOrdDitherMxN.c		\
		mlib_i_ImageClrOrdDither8x8.c		\
		mlib_i_ImageColorErrDiffMxN_Bit.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(HOME_TXT)/../copy

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

# C

$(DIR_O)/mlib_ImageColorErrorDiffusionMxN$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageColorErrorDiffusionMxN.h
$(DIR_O)/mlib_ImageColorTrue2IndexInit$(OBJ_EXT)      : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_ImageColorTrue2Index$(OBJ_EXT)          : $(HOME_TXT)/../include/mlib_ImageColormap.h
$(DIR_O)/mlib_c_ImageReplaceColor$(OBJ_EXT)           : ./mlib_ImageReplaceColor.h
$(DIR_O)/mlib_ImageReplaceColor_Fp$(OBJ_EXT)          : ./mlib_ImageReplaceColor.h
$(DIR_O)/mlib_ImageHistogram2_f$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageHistogram.h
$(DIR_O)/mlib_ImageLookUpMask_64$(OBJ_EXT)            : $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUpMask_f$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageLookUpMask.h
$(DIR_O)/mlib_ImageLookUp_64$(OBJ_EXT)                : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageLookUp_Bit$(OBJ_EXT)               : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageLookUp.h
$(DIR_O)/mlib_ImageThresh1_D64$(OBJ_EXT)              : $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_F32$(OBJ_EXT)              : $(HOME_TXT)/mlib_c_ImageThresh1.h
$(DIR_O)/mlib_ImageThresh1_Fp$(OBJ_EXT)               : $(HOME_TXT)/mlib_c_ImageThresh1.h
