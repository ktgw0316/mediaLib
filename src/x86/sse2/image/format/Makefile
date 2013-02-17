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
# ident	"@(#)Makefile	9.12	07/11/05 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/format

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/format

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageReformat.c \
             mlib_ImageReformat_f1.c \
             mlib_ImageReformat_f2.c \
             mlib_ImageReformat_fp.c \
             mlib_ImageChannelSplit.c \
             mlib_ImageChannelMerge.c \

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic
SRCS_STDC = mlib_c_ImageChannelInsert_f.c \
            mlib_c_ImageChannelMerge_U8_3.c

# MMX specific source under src/x86/mmx
SRCS_MMX =

# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = mlib_s_ImageChannelExtract.c	\
	    mlib_s_ImageChannelExtract_f.c	\
	    mlib_s_ImageChannelExtract_1.c	\
            mlib_s_ImageChannelExtract_43.c	\
	    mlib_s_ImageChannelMerge_U8_24.c	\
	    mlib_s_ImageChannelMerge_S16.c	\
            mlib_s_ImageChannelMerge_FS32.c	\
            mlib_s_ImageChannelMerge_D64.c	\
	    mlib_s_ImageChannelCopy.c		\
            mlib_s_ImageChannelCopy_f.c		\
            mlib_s_ImageChannelSplit_U8_24.c	\
	    mlib_s_ImageChannelSplit_U8_3.c	\
            mlib_s_ImageChannelSplit_S16.c	\
            mlib_s_ImageChannelSplit_FS32.c 	\
	    mlib_s_ImageChannelSplit_D64.c	\
	    mlib_s_ImageChannelInsert.c		\
	    mlib_s_ImageChannelInsert_1.c	\
	    mlib_s_ImageDataTypeConvert.c	\
	    mlib_s_ImageDataTypeConvert_f.c

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

$(DIR_O)/mlib_m_ImageChannelExtract_1$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageChannelExtract_43$(OBJ_EXT) :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h
$(DIR_O)/mlib_m_ImageChannelExtract_f$(OBJ_EXT)  :  $(ALT_DIR)/../../mmx/mlib_mmx_utils.h

$(DIR_O)/mlib_ImageChannelMerge$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_s_ImageChannelMerge_D64$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_ImageChannelSplit$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_s_ImageChannelSplit_D64$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_ImageReformat$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f1$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O)/mlib_c_ImageChannelCopy$(OBJ_EXT)        :  $(HOME_TXT)/mlib_c_ImageChannelCopy.h
$(DIR_O)/mlib_c_ImageChannelCopy_f$(OBJ_EXT)      :  $(HOME_TXT)/mlib_c_ImageChannelCopy.h
$(DIR_O)/mlib_c_ImageChannelExtract$(OBJ_EXT)     :  $(HOME_TXT)/mlib_c_ImageChannelExtract.h
$(DIR_O)/mlib_c_ImageChannelExtract_f$(OBJ_EXT)   :  $(HOME_TXT)/mlib_c_ImageChannelExtract.h
$(DIR_O)/mlib_c_ImageChannelInsert$(OBJ_EXT)      :  $(HOME_TXT)/mlib_c_ImageChannelInsert.h
$(DIR_O)/mlib_c_ImageChannelInsert_f$(OBJ_EXT)    :  $(HOME_TXT)/mlib_c_ImageChannelInsert.h
$(DIR_O)/mlib_c_ImageChannelMerge_FS32$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_S16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_U8_24$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_U8_3$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelSplit_FS32$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_S16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_U8_24$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_U8_3$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageDataTypeConvert$(OBJ_EXT)    :  $(HOME_TXT)/mlib_c_ImageDataTypeConvert.h
$(DIR_O)/mlib_c_ImageDataTypeConvert_f$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_c_ImageDataTypeConvert.h

