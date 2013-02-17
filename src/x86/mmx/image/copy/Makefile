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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/copy

UTILS_STDC = mlib_ImageCopy_Bit.c

SRCS_STDC_FP = mlib_ImageClearEdge_Fp.c \
               mlib_ImageClear_Fp.c \
               mlib_ImageCopyMask_Fp.c

SRCS_STDC = mlib_c_ImageClear.c \
            mlib_c_ImageClearEdge.c \
            mlib_c_ImageCopyArea.c \
            mlib_c_ImageCopyMask.c \
            mlib_c_ImageCopySubimage.c

win32_SRCS = mlib_m_ImageCopy.c \
             mlib_m_ImageCopy_na.c

SRCS = $($(TARCH)_SRCS)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include

# define depends

$(OBJS) $(DIR_O_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O_STDC)/mlib_ImageCopy_Bit$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O_STDC)/mlib_c_ImageCopyArea$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O_STDC)/mlib_c_ImageCopySubimage$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O_STDC)/mlib_m_ImageCopy$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageCopy.h
$(DIR_O_STDC)/mlib_m_ImageCopy_na$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageCopy.h
