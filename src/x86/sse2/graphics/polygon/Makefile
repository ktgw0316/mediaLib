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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/polygon

UTILS_STDC = mlib_GraphicsDrawPolygon.c \
        mlib_GraphicsDrawPolygon_A.c \
        mlib_GraphicsDrawPolygon_AB.c \
        mlib_GraphicsDrawPolygon_AG.c \
        mlib_GraphicsDrawPolygon_ABG.c \
        mlib_GraphicsDrawPolygon_AGZ.c \
        mlib_GraphicsDrawPolygon_ABGZ.c \
        mlib_GraphicsDrawPolygon_ABZ.c \
        mlib_GraphicsDrawPolygon_AZ.c \
        mlib_GraphicsDrawPolygon_G.c \
        mlib_GraphicsDrawPolygon_BG.c \
        mlib_GraphicsDrawPolygon_GZ.c \
        mlib_GraphicsDrawPolygon_BGZ.c \
        mlib_GraphicsDrawPolygon_BZ.c \
        mlib_GraphicsDrawPolygon_X.c \
        mlib_GraphicsDrawPolygon_B.c \
        mlib_GraphicsDrawPolygon_Z.c \
        mlib_GraphicsFillPolygon.c \
        mlib_GraphicsFillPolygon_X.c \
        mlib_GraphicsFillPolygon_B.c \
        mlib_GraphicsFillPolygon_A.c \
	mlib_GraphicsFillPolygon_AB.c \
        mlib_GraphicsFillPolygon_AGZ.c \
        mlib_GraphicsFillPolygon_ABGZ.c \
        mlib_GraphicsFillPolygon_ABZ.c \
        mlib_GraphicsFillPolygon_AZ.c \
        mlib_GraphicsFillPolygon_AG.c \
        mlib_GraphicsFillPolygon_ABG.c

SRCS_STDC = mlib_c_GraphicsFillPolygon_G.c \
         mlib_c_GraphicsFillPolygon_Z.c \
         mlib_c_GraphicsFillPolygon_GZ.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../include -I$(HOME_TXT)/./../../image/include -I$(HOME_INCLUDE)/../../sse

#
# define depends
#
__CDIF_B = $(HOME_TXT)/./../../image/include

$(DIR_O_STDC)/mlib_GraphicsDrawPolygon$(OBJ_EXT)      :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_A$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_AB$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_ABZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_AG$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_ABG$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_AGZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_ABGZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_AZ$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_B$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h \
                                                    $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageDivTables.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_BZ$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_G$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_BG$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_GZ$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_BGZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_X$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsDrawPolygon_Z$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillPolygon$(OBJ_EXT)      :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O_STDC)/mlib_GraphicsFillPolygon_AB$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h \
                                                    $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillPolygon_B$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h \
                                                    $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillPolygon_X$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_c_GraphicsFillPolygon_Z$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_GraphicsFillPolygon_GZ$(OBJ_EXT) :  $(__CDIF_B)/mlib_ImageDivTables.h
