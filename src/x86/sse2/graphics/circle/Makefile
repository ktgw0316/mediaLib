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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/circle

UTILS_STDC = mlib_GraphicsDrawArc.c \
        mlib_GraphicsDrawArc_X.c \
        mlib_GraphicsDrawArc_B.c \
        mlib_GraphicsDrawCircle.c \
        mlib_GraphicsDrawCircle_X.c \
        mlib_GraphicsDrawCircle_B.c \
        mlib_GraphicsDrawEllipse.c \
        mlib_GraphicsDrawEllipse_X.c \
        mlib_GraphicsDrawEllipse_B.c \
        mlib_GraphicsFillArc.c \
        mlib_GraphicsFillArc_X.c \
        mlib_GraphicsFillArc_B.c \
        mlib_GraphicsFillCircle.c \
        mlib_GraphicsFillCircle_X.c \
        mlib_GraphicsFillCircle_B.c \
        mlib_GraphicsFillEllipse.c \
        mlib_GraphicsFillEllipse_X.c \
        mlib_GraphicsFillEllipse_B.c \
        mlib_GraphicsFillArc_A.c

SRCS_STDC  = mlib_c_GraphicsDrawArc_A.c \
					mlib_c_GraphicsDrawArc_AB.c \
          mlib_c_GraphicsDrawCircle_A.c \
          mlib_c_GraphicsDrawCircle_AB.c \
          mlib_c_GraphicsDrawEllipse_A.c \
	        mlib_c_GraphicsDrawEllipse_AB.c \
	        mlib_c_GraphicsFillArc_AB.c \
          mlib_c_GraphicsFillCircle_A.c \
	        mlib_c_GraphicsFillCircle_AB.c \
          mlib_c_GraphicsFillEllipse_A.c \
	        mlib_c_GraphicsFillEllipse_AB.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/./../include -I$(HOME_TXT)/./../line -I$(HOME_TXT)/./../../image/include -I$(HOME_INCLUDE)/../../sse

#
# define depends
#
__CDIF_B = $(HOME_TXT)/./../../image/include

$(DIR_O_STDC)/mlib_GraphicsDrawArc_B$(OBJ_EXT)       :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsDrawCircle_B$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsDrawEllipse_B$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillArc$(OBJ_EXT)         :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillArc_A$(OBJ_EXT)       :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillArc_B$(OBJ_EXT)       :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillArc_X$(OBJ_EXT)       :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillCircle$(OBJ_EXT)      :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillCircle_B$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillCircle_X$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillEllipse$(OBJ_EXT)     :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_GraphicsFillEllipse_B$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_GraphicsFillEllipse_X$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawArc_A$(OBJ_EXT)     :  $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawArc_AB$(OBJ_EXT)    :  $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawCircle_A$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsMacro.h $(__CDIF_B)/mlib_ImageRowTable.h $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawEllipse_A$(OBJ_EXT) :  $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawCircle_AB$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsMacro.h $(__CDIF_B)/mlib_ImageRowTable.h $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsDrawEllipse_A$(OBJ_EXT) :  $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsFillCircle_A$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(HOME_TXT)/./../include/mlib_GraphicsMacro.h $(__CDIF_B)/mlib_ImageRowTable.h $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
$(DIR_O_STDC)/mlib_c_GraphicsFillEllipse_A$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsFillRow.h $(HOME_TXT)/./../line/mlib_c_GraphicsTabAlias.h
