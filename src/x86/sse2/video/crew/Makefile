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
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/x86/sse2/$(ALT_MOD)/vector

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/crew

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS_STDC   = mlib_VideoWaveletFwdTwoTenTrans.c \
          mlib_VideoWaveletInvTwoTenTrans.c

SRCS_STDC  = mlib_c_VideoWaveletTwoTenTrans_f.c

SRCS_SSE2 = mlib_s_VideoRvrsibleClrRGB2YUV.c \
          mlib_s_VideoRvrsibleClrYUV2RGB.c \
          mlib_s_VideoSignMagnitudeConvert.c \

SRCS_ALT_C = mlib_s_VectorCopy.c mlib_s_VectorZero.c mlib_s_VectorAdd.c

#SRCS = $($(TARCH)_SRCS)
SRCS = $(SRCS_SSE2)
SRCS_ALT = $(SRCS_ALT_C)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends

$(DIR_O_STDC)/mlib_VideoWaveletFwdTwoTenTrans$(OBJ_EXT) : $(HOME_TXT)/./mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O_STDC)/mlib_VideoWaveletInvTwoTenTrans$(OBJ_EXT) : $(HOME_TXT)/./mlib_VideoWaveletTwoTenTrans_f.h
$(DIR_O_STDC)/mlib_c_VideoWaveletTwoTenTrans_f$(OBJ_EXT)    : $(HOME_TXT)/./mlib_VideoWaveletTwoTenTrans_f.h
