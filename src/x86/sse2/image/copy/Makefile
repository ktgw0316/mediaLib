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
# ident	"@(#)Makefile	9.9	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/copy
HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/copy

# common source under src/stdc/generic
UTILS_STDC =

# common source for floating point under src/stdc/generic
SRCS_STDC_FP = mlib_ImageClearEdge_Fp.c \
               mlib_ImageClear_Fp.c

# C specific source under src/stdc/generic
SRCS_STDC =


# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageCopy_na.c	\
	    mlib_s_ImageCopy_Bit.c	\
	    mlib_s_ImageCopy.c		\
	    mlib_s_ImageCopySubimage.c	\
	    mlib_s_ImageCopy_f.c	\
	    mlib_s_ImageCopyArea_f.c	\
	    mlib_s_ImageCopyArea.c	\
	    mlib_s_ImageCopyMask.c	\
	    mlib_s_ImageCopyMask_s16.c	\
	    mlib_s_ImageCopyMask_s32.c	\
	    mlib_s_ImageCopyMask_u8u16.c\
	    mlib_s_ImageCopyMask_Fp.c	\
	    mlib_s_ImageClear.c		\
	    mlib_s_ImageClear_f.c	\
	    mlib_s_ImageClearEdge.c	\
	    mlib_s_ImageClearEdge_f.c

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

include $(TOP_DIR)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)
INCLUDES += -I$(HOME_TXT)/../include
INCLUDES += -I$(HOME_TXT)/../blend
INCLUDES += -I$(HOME_TXT)/../copy
INCLUDES += -I$(HOME_TXT)/../spatial
INCLUDES += -I$(ALT_DIR)
INCLUDES += -I$(ALT_DIR)/../../mmx
INCLUDES += -I../../sse


#
# dependencies
#

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_s_ImageCopy_Bit$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O)/mlib_s_ImageCopyArea$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageCopy.h \
					      ./mlib_s_ImageCopy_f.h       \
					      ./mlib_s_ImageCopyArea_f.h
$(DIR_O)/mlib_s_ImageCopyArea_f$(OBJ_EXT)   : ./mlib_s_ImageCopyArea_f.h
$(DIR_O)/mlib_s_ImageClear_f$(OBJ_EXT)      : ./mlib_s_ImageClear_f.h
$(DIR_O)/mlib_s_ImageClearEdge$(OBJ_EXT)    : ./mlib_s_ImageClearEdge_f.h
$(DIR_O)/mlib_s_ImageClearEdge_f$(OBJ_EXT)  : ./mlib_s_ImageClearEdge_f.h
$(DIR_O)/mlib_s_ImageCopy_na$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O)/mlib_s_ImageCopy$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O)/mlib_s_ImageCopySubimage$(OBJ_EXT) : ./mlib_s_ImageCopy_f.h	   \
					      $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O)/mlib_s_ImageCopy_f$(OBJ_EXT) 	    : ./mlib_s_ImageCopy_f.h	   \
					      $(HOME_TXT)/mlib_ImageCopy.h

