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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/triangleset

UTILS_STDC   = mlib_GraphicsClipTriangleHor.c \
          mlib_GraphicsClipTriangleHor_G.c \
          mlib_GraphicsClipTriangleHor_GZ.c \
          mlib_GraphicsClipTriangleHor_Z.c \
          mlib_GraphicsClipTriangleSet.c \
          mlib_GraphicsClipTriangleSet_AB.c \
          mlib_GraphicsClipTriangleSet_B.c \
          mlib_GraphicsClipTriangleSet_G.c \
          mlib_GraphicsClipTriangleSet_BG.c \
          mlib_GraphicsClipTriangleSet_GZ.c \
          mlib_GraphicsClipTriangleSet_BGZ.c \
          mlib_GraphicsClipTriangleSet_Z.c \
          mlib_GraphicsClipTriangleSet_BZ.c

SRCS_STDC  = mlib_c_GraphicsSpamLineHor.c \
          mlib_c_GraphicsSpamLineHor_G.c \
          mlib_c_GraphicsSpamLineHor_B.c \
          mlib_c_GraphicsSpamLineHor_BG.c \
          mlib_c_GraphicsSpamLineHor_GZ.c \
          mlib_c_GraphicsSpamLineHor_BGZ.c \
          mlib_c_GraphicsSpamLineHor_X.c \
          mlib_c_GraphicsSpamLineHor_Z.c \
	  mlib_c_GraphicsSpamLineHor_BZ.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../include -I$(HOME_TXT)/../../image/include -I$(HOME_INCLUDE)/../../sse

#
# define depends
#
__CDIF_B = $(HOME_TXT)/./../../image/include

$(DIR_O_STDC)/mlib_GraphicsClipTriangleHor$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleHor_G$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleHor_GZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleHor_Z$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_AB$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_B$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_BZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_G$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_BG$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_GZ$(OBJ_EXT) :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_BGZ$(OBJ_EXT):  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_GraphicsClipTriangleSet_Z$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor$(OBJ_EXT)      :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_B$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_BZ$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsBlend.h $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h \
                                                      $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_G$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_BG$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_GZ$(OBJ_EXT)   :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_BGZ$(OBJ_EXT)  :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_X$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
$(DIR_O_STDC)/mlib_c_GraphicsSpamLineHor_Z$(OBJ_EXT)    :  $(HOME_TXT)/./../include/mlib_GraphicsDrawSet.h
