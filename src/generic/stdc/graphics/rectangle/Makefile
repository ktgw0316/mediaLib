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

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS = mlib_GraphicsDrawRectangle.c \
        mlib_GraphicsDrawRectangle_X.c \
        mlib_GraphicsDrawRectangle_B.c \
        mlib_GraphicsFillRectangle.c \
        mlib_GraphicsFillRectangle_X.c \
        mlib_GraphicsFillRectangle_B.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../../image/include

#
# define depends
#
__CDIF_B = ./../../image/include

$(DIR_O)/mlib_GraphicsDrawRectangle$(OBJ_EXT)   :  ./../include/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsDrawRectangle_B$(OBJ_EXT) :  ./../include/mlib_GraphicsBlend.h ./../include/mlib_GraphicsFillRow.h \
                                                   $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_GraphicsDrawRectangle_X$(OBJ_EXT) :  ./../include/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillRectangle$(OBJ_EXT)   :  ./../include/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillRectangle_B$(OBJ_EXT) :  ./../include/mlib_GraphicsBlend.h ./../include/mlib_GraphicsFillRow.h \
                                                   $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_GraphicsFillRectangle_X$(OBJ_EXT) :  ./../include/mlib_GraphicsFillRow.h
