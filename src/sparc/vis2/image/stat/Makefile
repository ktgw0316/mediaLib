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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/stat
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/stat

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_ImageExtrema2.c \
         mlib_v_ImageMinimum.c \
         mlib_v_ImageMaximum.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/../include -I$(HOME_VIS_TXT) -I$(HOME_TXT)

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

#
# C
#

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

#
# VIS
#

$(DIR_O)/mlib_v_ImageAutoCorrel$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_VIS_TXT)/mlib_v_ImageCorrel.h
$(DIR_O)/mlib_v_ImageCrossCorrel$(OBJ_EXT) :  $(HOME_VIS_TXT)/mlib_v_ImageCorrel.h $(HOME_VIS_TXT)/mlib_v_ImageStat.h
$(DIR_O)/mlib_v_ImageMean$(OBJ_EXT)        :  $(HOME_VIS_TXT)/mlib_v_ImageStat.h
$(DIR_O)/mlib_v_ImageMoment2$(OBJ_EXT)     :  $(HOME_VIS_TXT)/mlib_v_ImageMoment2.h $(HOME_VIS_TXT)/mlib_v_ImageStat.h
$(DIR_O)/mlib_v_ImageStdDev$(OBJ_EXT)      :  $(HOME_VIS_TXT)/mlib_v_ImageMoment2.h $(HOME_VIS_TXT)/mlib_v_ImageStat.h

#
# VIS2
#

$(DIR_O)/mlib_v_ImageExtrema2$(OBJ_EXT) : $(HOME_VIS_TXT)/mlib_v_ImageStat.h
$(DIR_O)/mlib_v_ImageMaximum$(OBJ_EXT)  : $(HOME_VIS_TXT)/mlib_v_ImageStat.h
$(DIR_O)/mlib_v_ImageMinimum$(OBJ_EXT)  : $(HOME_VIS_TXT)/mlib_v_ImageStat.h
