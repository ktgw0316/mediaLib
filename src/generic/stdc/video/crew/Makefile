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
MODNAME = video

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS   = mlib_VideoWaveletFwdTwoTenTrans.c \
          mlib_VideoWaveletInvTwoTenTrans.c

SRCS_C  = mlib_c_VideoRvrsibleClrRGB2YUV.c \
          mlib_c_VideoRvrsibleClrYUV2RGB.c \
          mlib_c_VideoSignMagnitudeConvert.c \
          mlib_c_VideoWaveletTwoTenTrans_f.c

SRCS_ALT_C = mlib_c_VectorCopy.c mlib_c_VectorZero.c mlib_c_VectorAdd.c

SRCS = $(SRCS_C)
SRCS_ALT = $(SRCS_ALT_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends

$(DIR_O)/mlib_VideoWaveletFwdTwoTenTrans$(OBJ_EXT) : ./mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O)/mlib_VideoWaveletInvTwoTenTrans$(OBJ_EXT) : ./mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O)/mlib_c_VideoWaveletTwoTenTrans_f$(OBJ_EXT)    : ./mlib_VideoWaveletTwoTenTrans_f.h
