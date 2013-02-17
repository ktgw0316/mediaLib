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
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.2	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

ALT_MOD_AL = algebra
ALT_MOD = image
ALT_UTILS_MOD = $(ALT_MOD)
STDC_ALT_MOD = $(ALT_UTILS_MOD)

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/color

ALT_DIR = $(TOP_DIR)/src/generic/int/$(ALT_MOD)/blend
ALT_UTILS_DIR = $(TOP_DIR)/src/generic/int/$(ALT_UTILS_MOD)/geom
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/geom

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  =	mlib_i_VideoJFIFYCC2RGB444_S16.c	\
		mlib_i_VideoRGB2JFIFYCC444_S16.c	\
		mlib_i_VideoColorAVDBlendUtil.c		\
		mlib_i_VideoColorAVDBlendUtil_RA.c	\
		mlib_i_VideoColorAVDResize_BL.c		\
		mlib_i_VideoColorAVDResize_BC.c		\
		mlib_i_VideoColorAVDYUV411.c		\
		mlib_i_VideoDownSample420.c		\
		mlib_i_VideoBGR2JFIFYCC444_S16.c	\
		mlib_i_VideoJFIFYCC2RGB422_NN.c		\
		mlib_i_VideoJFIFYCC2RGB420_NN.c


SRCS_STDC_ALT = mlib_ImageFilters.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(ALT_UTILS_DIR)	\
		-I$(HOME_ALT_TXT) \
		-I$(HOME_ALT_TXT)/../include \
		-I$(HOME_ALT_TXT)/../blend \
		-I$(HOME_ALT_TXT)/../copy

# define depends

$(DIR_O)/../$(STDC_ALT_MOD)/mlib_ImageFilters$(OBJ_EXT)    : $(ALT_UTILS_DIR)/mlib_ImageFilters.h

$(DIR_O)/mlib_VideoColorAVDBlend$(OBJ_EXT)          : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA$(OBJ_EXT)       : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_Inp$(OBJ_EXT)      : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDBlend_RA_Inp$(OBJ_EXT)   : ${HOME_TXT}/mlib_VideoColorAVDBlend_proto.h
$(DIR_O)/mlib_VideoColorAVDResize_NN$(OBJ_EXT)      : ${HOME_TXT}/mlib_VideoColorAVDResize.h
$(DIR_O)/mlib_VideoColorAVDResize$(OBJ_EXT)         : ${HOME_TXT}/mlib_VideoColorAVDResize.h
