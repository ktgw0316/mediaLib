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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/format

UTILS_STDC = mlib_ImageReformat.c \
             mlib_ImageReformat_f1.c \
             mlib_ImageReformat_f2.c \
             mlib_ImageReformat_fp.c \
             mlib_ImageChannelSplit.c \
             mlib_ImageChannelMerge.c \
             mlib_ImageChannelSplit_D64.c \
             mlib_ImageChannelMerge_D64.c

SRCS_STDC = mlib_c_ImageChannelCopy.c \
            mlib_c_ImageChannelCopy_f.c \
            mlib_c_ImageChannelInsert.c \
            mlib_c_ImageChannelInsert_f.c \
            mlib_c_ImageDataTypeConvert.c \
            mlib_c_ImageDataTypeConvert_f.c \
            mlib_c_ImageChannelSplit_U8_3.c \
            mlib_c_ImageChannelSplit_U8_24.c \
            mlib_c_ImageChannelMerge_U8_3.c \
            mlib_c_ImageChannelMerge_U8_24.c \
            mlib_c_ImageChannelSplit_S16.c \
            mlib_c_ImageChannelMerge_S16.c \
            mlib_c_ImageChannelSplit_FS32.c \
            mlib_c_ImageChannelMerge_FS32.c

win32_SRCS =  mlib_m_ImageChannelExtract.c \
              mlib_m_ImageChannelExtract_f.c \
              mlib_m_ImageChannelExtract_1.c \
              mlib_m_ImageChannelExtract_43.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_INCLUDE)/../../mmx

#
# define depends
#

$(OBJS_STDC) $(OBJS) : $(MLIB_HOME)/src/generic/stdc/image/include/mlib_ImageCheck.h

$(ARCH_DIR_O)/mlib_m_ImageChannelExtract_1$(OBJ_EXT)  :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageChannelExtract_43$(OBJ_EXT) :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageChannelExtract_f$(OBJ_EXT)  :  $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h

$(DIR_O_STDC)/mlib_ImageChannelMerge$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_ImageChannelMerge_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_ImageChannelSplit$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_ImageChannelSplit_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_ImageReformat$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O_STDC)/mlib_ImageReformat_f1$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O_STDC)/mlib_ImageReformat_f2$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O_STDC)/mlib_ImageReformat_fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageReformat.h
$(DIR_O_STDC)/mlib_c_ImageChannelCopy$(OBJ_EXT)        :  $(HOME_TXT)/mlib_c_ImageChannelCopy.h
$(DIR_O_STDC)/mlib_c_ImageChannelCopy_f$(OBJ_EXT)      :  $(HOME_TXT)/mlib_c_ImageChannelCopy.h
$(DIR_O_STDC)/mlib_c_ImageChannelExtract$(OBJ_EXT)     :  $(HOME_TXT)/mlib_c_ImageChannelExtract.h
$(DIR_O_STDC)/mlib_c_ImageChannelExtract_f$(OBJ_EXT)   :  $(HOME_TXT)/mlib_c_ImageChannelExtract.h
$(DIR_O_STDC)/mlib_c_ImageChannelInsert$(OBJ_EXT)      :  $(HOME_TXT)/mlib_c_ImageChannelInsert.h
$(DIR_O_STDC)/mlib_c_ImageChannelInsert_f$(OBJ_EXT)    :  $(HOME_TXT)/mlib_c_ImageChannelInsert.h
$(DIR_O_STDC)/mlib_c_ImageChannelMerge_FS32$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_c_ImageChannelMerge_S16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_c_ImageChannelMerge_U8_24$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_c_ImageChannelMerge_U8_3$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelMerge.h
$(DIR_O_STDC)/mlib_c_ImageChannelSplit_FS32$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_c_ImageChannelSplit_S16$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_c_ImageChannelSplit_U8_24$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_c_ImageChannelSplit_U8_3$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageChannelSplit.h
$(DIR_O_STDC)/mlib_c_ImageDataTypeConvert$(OBJ_EXT)    :  $(HOME_TXT)/mlib_c_ImageDataTypeConvert.h
$(DIR_O_STDC)/mlib_c_ImageDataTypeConvert_f$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_c_ImageDataTypeConvert.h
