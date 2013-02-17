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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/stat

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP =

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT =	mlib_i_ImageAutoCorrel.c	\
                mlib_i_ImageCrossCorrel.c	\
                mlib_i_ImageMean.c		\
                mlib_i_ImageMoment2.c		\
                mlib_i_ImageNormCrossCorrel.c	\
                mlib_i_ImageStdDev.c		\
                mlib_i_ImageXProj.c		\
                mlib_i_ImageYProj.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAutoCorrel_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageCorrel.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageCrossCorrel_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageCorrel.h
$(DIR_O)/mlib_ImageExtrema2$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageExtrema2_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageMaximumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageExtremaLocations$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageExtremaLocations_Fp$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageMinimumFuncS32$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageMoment2_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageMoment2.h
$(DIR_O)/mlib_ImageStdDev_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageMoment2.h
