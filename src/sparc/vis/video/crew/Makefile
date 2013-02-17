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
MODNAME = video
STDC_ALT_MOD = image
ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/sparc/vis/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/crew

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/common
SRCS_STDC_ALT = mlib_ImageDivTables.c

SRCS_V  = mlib_v_VideoRvrsibleClrRGB2YUV.c \
          mlib_v_VideoRvrsibleClrYUV2RGB.c \
          mlib_v_VideoSignMagnitudeConvert.c \
          mlib_v_VideoWaveletTwoTenTrans_f.c

SRCS_ALT_V = mlib_v_VectorCopy.c \
             mlib_v_VectorSplit.c \
             mlib_v_VectorZero.c


include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_ALT_TXT)/../include

DIR_O_ALT = $(DIR_O)/../$(STDC_ALT_MOD)

# define depends
$(DIR_O)/mlib_VideoWaveletFwdTwoTenTrans$(OBJ_EXT) : ${HOME_TXT}/mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O)/mlib_VideoWaveletInvTwoTenTrans$(OBJ_EXT) : ${HOME_TXT}/mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O)/mlib_v_VideoWaveletTwoTenTrans_f$(OBJ_EXT)    : ${HOME_TXT}/mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O_ALT)/mlib_ImageDivTables$(OBJ_EXT)             : $(HOME_ALT_TXT)/../include/mlib_ImageDivTables.h
