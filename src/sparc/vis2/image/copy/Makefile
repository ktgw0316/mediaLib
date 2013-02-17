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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/copy
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/copy

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FPSH = $(SRCS_FPSH_TMP:sh)

SRCS_STDC_FP_TMP = aa="$(SRCS_FPSH)"; for i in $(SRCS_FP_RM); do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP_RM = mlib_ImageClearEdge_Fp.c

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" "SRCS_S=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_SV = mlib_v_ImageCopy_blk.S

SRCS_V = mlib_v_ImageCopy.c         \
         mlib_v_ImageCopy_f.c       \
         mlib_v_ImageClearEdge_f.c  \
         mlib_v_ImageClearEdge_Fp.c \
         mlib_v_ImageCopyMask_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(HOME_TXT)/../include -I$(HOME_TXT) -I$(HOME_VIS_TXT)

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

#
# C
#

$(DIR_O)/mlib_ImageCopy_Bit$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageCopy.h

#
# VIS
#

$(DIR_O)/mlib_v_ImageClear$(OBJ_EXT)        :  $(HOME_VIS_TXT)/mlib_v_ImageClear_f.h
$(DIR_O)/mlib_v_ImageClearEdge$(OBJ_EXT)    :  $(HOME_VIS_TXT)/mlib_v_ImageClearEdge_f.h
$(DIR_O)/mlib_v_ImageClear_f$(OBJ_EXT)      :  $(HOME_VIS_TXT)/mlib_v_ImageClear_f.h
$(DIR_O)/mlib_v_ImageCopyArea$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageCopy.h \
                                               $(HOME_VIS_TXT)/mlib_v_ImageCopyArea_f.h \
                                               $(HOME_VIS_TXT)/mlib_v_ImageCopy_f.h
$(DIR_O)/mlib_v_ImageCopyArea_f$(OBJ_EXT)   :  $(HOME_VIS_TXT)/mlib_v_ImageCopyArea_f.h
$(DIR_O)/mlib_v_ImageCopyMask$(OBJ_EXT)     :  $(HOME_VIS_TXT)/mlib_v_ImageCopyMask_f.h
$(DIR_O)/mlib_v_ImageCopySubimage$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageCopy_f.h

#
# VIS2
#

$(DIR_O)/mlib_v_ImageClearEdge_f$(OBJ_EXT) :  $(HOME_VIS_TXT)/mlib_v_ImageClearEdge_f.h
$(DIR_O)/mlib_v_ImageCopy$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageCopy_f.h
$(DIR_O)/mlib_v_ImageCopyMask_f$(OBJ_EXT)  :  $(HOME_VIS_TXT)/mlib_v_ImageCopyMask_f.h
$(DIR_O)/mlib_v_ImageCopy_f$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageCopy_f.h
