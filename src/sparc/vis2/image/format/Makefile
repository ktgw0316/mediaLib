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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/format
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/format

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in $(UTILS_RM); do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS_RM = mlib_ImageReformat_f2.c

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_ImageChannelExtract.c       \
         mlib_v_ImageChannelExtract_1.c     \
         mlib_v_ImageChannelExtract_2.c     \
         mlib_v_ImageChannelExtract_43.c    \
         mlib_v_ImageChannelExtract_f.c     \
         mlib_v_ImageChannelInsert.c        \
         mlib_v_ImageChannelInsert_1.c      \
         mlib_v_ImageChannelInsert_2.c      \
         mlib_v_ImageChannelInsert_34.c     \
         mlib_v_ImageChannelMerge_U8_3.c    \
         mlib_v_ImageChannelMerge_S16.c     \
         mlib_v_ImageDataTypeConvert_f.c    \
         mlib_v_ImageDataTypeConvert.c      \
         mlib_v_ImageReformat_f2.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(HOME_TXT)/../include -I$(HOME_TXT) -I$(HOME_VIS_TXT)

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

#
# C
#
$(DIR_O)/mlib_ImageChannelMerge$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_ImageChannelMerge_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_ImageChannelSplit$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_ImageChannelSplit_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_ImageReformat$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f1$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f2$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_fp$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageReformat.h

#
# VIS
#
$(DIR_O)/mlib_v_ImageChannelCopy$(OBJ_EXT)        : $(HOME_VIS_TXT)/mlib_v_ImageChannelCopy.h
$(DIR_O)/mlib_v_ImageChannelCopy_f$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageChannelCopy.h
$(DIR_O)/mlib_v_ImageChannelMerge_FS32$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_v_ImageChannelMerge_U8_24$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_v_ImageChannelSplit_FS32$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_v_ImageChannelSplit_S16$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_v_ImageChannelSplit_U8_24$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_v_ImageChannelSplit_U8_3$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageChannelSplit.h

#
# VIS2
#
$(DIR_O)/mlib_ImageReformat_f2$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_v_ImageChannelExtract$(OBJ_EXT)     : $(HOME_VIS_TXT)/mlib_v_ImageChannelExtract.h \
                                                    ./mlib_v_ImageChannelExtract_2.h
$(DIR_O)/mlib_v_ImageChannelExtract_1$(OBJ_EXT)   : $(HOME_VIS_TXT)/mlib_v_ImageChannelExtract.h \
                                                    ./mlib_v_ImageChannelExtract_2.h
$(DIR_O)/mlib_v_ImageChannelExtract_2$(OBJ_EXT)   : ./mlib_v_ImageChannelExtract_2.h
$(DIR_O)/mlib_v_ImageChannelExtract_43$(OBJ_EXT)  : ./mlib_v_ImageChannelExtract_2.h
$(DIR_O)/mlib_v_ImageChannelExtract_f$(OBJ_EXT)   : ./mlib_v_ImageChannelExtract_2.h
$(DIR_O)/mlib_v_ImageChannelInsert$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageChannelInsert.h \
                                                    ./mlib_v_ImageChannelInsert_2.h
$(DIR_O)/mlib_v_ImageChannelInsert_1$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageChannelInsert.h \
                                                    ./mlib_v_ImageChannelInsert_2.h
$(DIR_O)/mlib_v_ImageChannelInsert_2$(OBJ_EXT)    : ./mlib_v_ImageChannelInsert_2.h
$(DIR_O)/mlib_v_ImageChannelInsert_34$(OBJ_EXT)   : $(HOME_VIS_TXT)/mlib_v_ImageChannelInsert.h \
                                                    ./mlib_v_ImageChannelInsert_2.h
$(DIR_O)/mlib_v_ImageChannelMerge_S16$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_v_ImageChannelMerge_U8_3$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_v_ImageDataTypeConvert$(OBJ_EXT)    : $(HOME_VIS_TXT)/mlib_v_ImageDataTypeConvert.h
$(DIR_O)/mlib_v_ImageDataTypeConvert_f$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDataTypeConvert.h
$(DIR_O)/mlib_v_ImageReformat_f2$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageReformat.h
