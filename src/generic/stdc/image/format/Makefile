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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  =  mlib_ImageReformat.c             \
          mlib_ImageReformat_f1.c          \
          mlib_ImageReformat_f2.c          \
          mlib_ImageReformat_fp.c          \
          mlib_ImageChannelSplit.c         \
          mlib_ImageChannelMerge.c         \
          mlib_ImageChannelSplit_D64.c     \
          mlib_ImageChannelMerge_D64.c

SRCS_C  = mlib_c_ImageChannelCopy.c        \
          mlib_c_ImageChannelCopy_f.c      \
          mlib_c_ImageChannelExtract.c     \
          mlib_c_ImageChannelExtract_f.c   \
          mlib_c_ImageChannelInsert.c      \
          mlib_c_ImageChannelInsert_f.c    \
          mlib_c_ImageDataTypeConvert.c    \
          mlib_c_ImageDataTypeConvert_f.c  \
          mlib_c_ImageChannelSplit_U8_24.c \
          mlib_c_ImageChannelSplit_U8_3.c  \
          mlib_c_ImageChannelMerge_U8_24.c \
          mlib_c_ImageChannelMerge_U8_3.c  \
          mlib_c_ImageChannelSplit_S16.c   \
          mlib_c_ImageChannelMerge_S16.c   \
          mlib_c_ImageChannelSplit_FS32.c  \
          mlib_c_ImageChannelMerge_FS32.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I../include

#
# define depends
#

$(OBJS) : ./../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageChannelMerge$(OBJ_EXT)         :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_ImageChannelMerge_D64$(OBJ_EXT)     :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_ImageChannelSplit$(OBJ_EXT)         :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_ImageChannelSplit_D64$(OBJ_EXT)     :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_ImageReformat$(OBJ_EXT)             :  ./mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f1$(OBJ_EXT)          :  ./mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_f2$(OBJ_EXT)          :  ./mlib_ImageReformat.h
$(DIR_O)/mlib_ImageReformat_fp$(OBJ_EXT)          :  ./mlib_ImageReformat.h
$(DIR_O)/mlib_c_ImageChannelCopy$(OBJ_EXT)        :  ./mlib_c_ImageChannelCopy.h
$(DIR_O)/mlib_c_ImageChannelCopy_f$(OBJ_EXT)      :  ./mlib_c_ImageChannelCopy.h
$(DIR_O)/mlib_c_ImageChannelExtract$(OBJ_EXT)     :  ./mlib_c_ImageChannelExtract.h
$(DIR_O)/mlib_c_ImageChannelExtract_f$(OBJ_EXT)   :  ./mlib_c_ImageChannelExtract.h
$(DIR_O)/mlib_c_ImageChannelInsert$(OBJ_EXT)      :  ./mlib_c_ImageChannelInsert.h
$(DIR_O)/mlib_c_ImageChannelInsert_f$(OBJ_EXT)    :  ./mlib_c_ImageChannelInsert.h
$(DIR_O)/mlib_c_ImageChannelMerge_FS32$(OBJ_EXT)  :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_S16$(OBJ_EXT)   :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_U8_24$(OBJ_EXT) :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelMerge_U8_3$(OBJ_EXT)  :  ./mlib_ImageChannelMerge.h
$(DIR_O)/mlib_c_ImageChannelSplit_FS32$(OBJ_EXT)  :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_S16$(OBJ_EXT)   :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_U8_24$(OBJ_EXT) :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageChannelSplit_U8_3$(OBJ_EXT)  :  ./mlib_ImageChannelSplit.h
$(DIR_O)/mlib_c_ImageDataTypeConvert$(OBJ_EXT)    :  ./mlib_c_ImageDataTypeConvert.h
$(DIR_O)/mlib_c_ImageDataTypeConvert_f$(OBJ_EXT)  :  ./../include/mlib_ImageDivTables.h \
                                                     ./mlib_c_ImageDataTypeConvert.h
