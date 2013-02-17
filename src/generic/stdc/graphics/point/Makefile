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
MODNAME = graphics
ALT_MOD = image
ALT_UTILS_MOD = $(ALT_MOD)

ALT_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/copy
ALT_UTILS_DIR = $(TOP_DIR)/src/generic/stdc/$(ALT_UTILS_MOD)/common

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS = mlib_GraphicsDrawPoint.c \
        mlib_GraphicsDrawPolypoint.c \
        mlib_GraphicsDrawPoint_B.c \
        mlib_GraphicsDrawPolypoint_B.c

UTILS_ALT = mlib_ImageCreate.c mlib_ImageDivTables.c

SRCS_ALT = mlib_c_ImageClear.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../../image/include

#
# define depends
#
ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)
__CDIF_A = ./../../image/include

$(ALT_DIR_O)/mlib_ImageCreate$(OBJ_EXT)         : ./../../image/include/mlib_ImageRowTable.h ./../../image/include/mlib_ImageCreate.h
$(ALT_DIR_O)/mlib_c_ImageClear$(OBJ_EXT)        : ./../../image/include/mlib_ImageCheck.h
$(DIR_O)/mlib_GraphicsDrawPoint_B$(OBJ_EXT)     :  $(__CDIF_A)/mlib_ImageDivTables.h
$(DIR_O)/mlib_GraphicsDrawPolypoint$(OBJ_EXT)   :  $(__CDIF_A)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolypoint_B$(OBJ_EXT) :  ./../include/mlib_GraphicsBlend.h $(__CDIF_A)/mlib_ImageDivTables.h \
                                                   $(__CDIF_A)/mlib_ImageRowTable.h
