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
# ident	"@(#)Makefile	9.12	07/11/05 SMI"
#

TOP_DIR = ../../../../..

MODNAME = video
ALT_MOD = video
ALT_DIR = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/dct

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/dct

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS_STDC = mlib_VideoDCT2x2_S16_S16.c		\
             mlib_VideoIDCT_IEEE.c

SRCS_STDC  = mlib_c_VideoDCT16x16_S16_S16_B10.c	\
             mlib_c_VideoDCT8x8_S16_S16_B12.c	\

SRCS_SSE2  = mlib_s_VideoDCT8x8.c		\
             mlib_s_VideoIDCT8x8.c 		\
             mlib_s_VideoIDCT8x8_Q1.c 		\
             mlib_s_VideoIDCT8x8_S16_S16_QM.c	\
             mlib_s_VideoDCT16x16_S16_S16.c	\
             mlib_s_VideoDCT4x4_S16_S16.c	\
             mlib_s_VideoIDCT8x8_DC.c		\
             mlib_s_VideoDCTQuantize.c		\
	     mlib_s_VideoDeQuantizeIDCT.c	\
             mlib_s_VideoQuantize_S16.c

SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends

$(OBJS) $(OBJS_STDC) :

# define depends

UTIL_HEADERS = ./mlib_s_video.h

$(DIR_O)/mlib_s_VideoDCT8x8$(OBJ_EXT)  : $(UTIL_HEADERS)
