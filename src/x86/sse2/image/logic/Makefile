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
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/logic

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/logic

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageConstNotAnd_Inp.c    \
             mlib_ImageConstNotOr_Inp.c     \
             mlib_ImageConstNotXor_Inp.c    \
             mlib_ImageConstOrNot_Inp.c     \
             mlib_ImageConstOr_Inp.c        \
             mlib_ImageConstXor_Inp.c       \
             mlib_ImageNotAnd_Inp.c         \
             mlib_ImageNotOr_Inp.c          \
             mlib_ImageNotXor_Inp.c         \
             mlib_ImageNot_Inp.c            \
             mlib_ImageOrNot_Inp.c          \
             mlib_ImageAnd_Inp.c            \
             mlib_ImageAndNot_Inp.c         \
             mlib_ImageConstAnd_Inp.c       \
             mlib_ImageConstAndNot_Inp.c    \
             mlib_ImageOr_Inp.c             \
             mlib_ImageXor_Inp.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic
SRCS_STDC =

# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageNot.c		\
	mlib_s_ImageNot_Bit.c		\
	mlib_s_ImageAnd.c		\
	mlib_s_ImageAndNot.c		\
	mlib_s_ImageNotAnd.c		\
	mlib_s_ImageNotOr.c		\
	mlib_s_ImageNotXor.c		\
	mlib_s_ImageOr.c		\
	mlib_s_ImageOrNot.c		\
	mlib_s_ImageXor.c		\
	mlib_s_ImageAnd_Bit.c		\
	mlib_s_ImageAndNot_Bit.c	\
	mlib_s_ImageNotAnd_Bit.c	\
	mlib_s_ImageNotOr_Bit.c		\
	mlib_s_ImageNotXor_Bit.c	\
	mlib_s_ImageOrNot_Bit.c		\
	mlib_s_ImageOr_Bit.c		\
	mlib_s_ImageXor_Bit.c		\
	mlib_s_ImageConstAnd.c		\
	mlib_s_ImageConstAndNot.c	\
	mlib_s_ImageConstNotAnd.c	\
	mlib_s_ImageConstNotOr.c	\
	mlib_s_ImageConstNotXor.c	\
	mlib_s_ImageConstOr.c		\
	mlib_s_ImageConstOrNot.c	\
	mlib_s_ImageConstXor.c		\
	mlib_s_ImageConstAndNot_Bit.c	\
	mlib_s_ImageConstAnd_Bit.c	\
	mlib_s_ImageConstNotAnd_Bit.c	\
	mlib_s_ImageConstNotOr_Bit.c	\
	mlib_s_ImageConstNotXor_Bit.c	\
	mlib_s_ImageConstOrNot_Bit.c	\
	mlib_s_ImageConstOr_Bit.c	\
	mlib_s_ImageConstXor_Bit.c

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

$(DIR_O)/mlib_s_ImageNot$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_s_ImageNot_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_s_ImageAnd$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageAnd$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageAndNot$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageNotAnd$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageNotOr$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageNotXor$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageOr$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageOrNot$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageXor$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic.h
$(DIR_O)/mlib_s_ImageAnd_Bit$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageAndNot_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageNotAnd_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageNotOr_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageNotXor_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageOrNot_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageOr_Bit$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageXor_Bit$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstAnd$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstAndNot$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstNotAnd$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstNotOr$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstNotXor$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstOr$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstOrNot$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstXor$(OBJ_EXT)		: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic.h
$(DIR_O)/mlib_s_ImageConstAndNot_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstAnd_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstNotAnd_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstNotOr_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstNotXor_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstOrNot_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstOr_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h
$(DIR_O)/mlib_s_ImageConstXor_Bit$(OBJ_EXT)	: $(HOME_TXT)/mlib_ImageLogic_proto.h ./mlib_s_ImageConstLogic_Bit.h

