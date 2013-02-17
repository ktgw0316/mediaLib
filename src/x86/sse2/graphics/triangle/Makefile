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
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = graphics

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/triangle

UTILS_STDC = mlib_GraphicsDrawTriangle.c \
        mlib_GraphicsDrawTriangle_A.c \
        mlib_GraphicsDrawTriangle_B.c \
        mlib_GraphicsDrawTriangle_AG.c \
        mlib_GraphicsDrawTriangle_AB.c \
        mlib_GraphicsDrawTriangle_ABG.c \
        mlib_GraphicsDrawTriangle_AGZ.c \
        mlib_GraphicsDrawTriangle_ABGZ.c \
        mlib_GraphicsDrawTriangle_AZ.c \
        mlib_GraphicsDrawTriangle_G.c \
        mlib_GraphicsDrawTriangle_BG.c \
        mlib_GraphicsDrawTriangle_GZ.c \
        mlib_GraphicsDrawTriangle_BGZ.c \
        mlib_GraphicsDrawTriangle_X.c \
        mlib_GraphicsDrawTriangle_Z.c \
        mlib_GraphicsDrawTriangle_BZ.c \
        mlib_GraphicsDrawTriangle_ABZ.c \
        mlib_GraphicsFillTriangle.c \
        mlib_GraphicsFillTriangle_A.c \
        mlib_GraphicsFillTriangle_B.c \
        mlib_GraphicsFillTriangle_AG.c \
        mlib_GraphicsFillTriangle_AB.c \
        mlib_GraphicsFillTriangle_ABG.c \
        mlib_GraphicsFillTriangle_AGZ.c \
        mlib_GraphicsFillTriangle_ABGZ.c \
        mlib_GraphicsFillTriangle_AZ.c \
        mlib_GraphicsFillTriangle_G.c \
        mlib_GraphicsFillTriangle_BG.c \
        mlib_GraphicsFillTriangle_GZ.c \
        mlib_GraphicsFillTriangle_BGZ.c \
        mlib_GraphicsFillTriangle_X.c \
        mlib_GraphicsFillTriangle_Z.c \
        mlib_GraphicsFillTriangle_BZ.c \
        mlib_GraphicsFillTriangle_ABZ.c

SRCS_STDC =

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../include -I$(HOME_TXT)/./../../image/include -I$(HOME_INCLUDE)/../../sse

#
# define depends
#
__CDIF_B = $(HOME_TXT)/./../../image/include

$(DIR_O_STDC)/mlib_GraphicsDrawTriangle$(OBJ_EXT)     :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_A$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_B$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h \
						    $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_AG$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_AB$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_ABG$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_AGZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_ABGZ$(OBJ_EXT):  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_AZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_BZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_ABZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_G$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_BG$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_GZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_BGZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_X$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawTriangle_Z$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle$(OBJ_EXT)     :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_A$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_B$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_AG$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_AB$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_ABG$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_AGZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_ABGZ$(OBJ_EXT):  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_AZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_BZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_ABZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_G$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_BG$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_GZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_BGZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_X$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillTriangle_Z$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
