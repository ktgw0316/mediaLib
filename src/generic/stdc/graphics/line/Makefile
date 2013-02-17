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

UTILS =   mlib_GraphicsDrawLine.c \
          mlib_GraphicsDrawLine_A.c \
          mlib_GraphicsDrawLine_AB.c \
          mlib_GraphicsDrawLine_AG.c \
          mlib_GraphicsDrawLine_ABG.c \
          mlib_GraphicsDrawLine_AGZ.c \
          mlib_GraphicsDrawLine_ABGZ.c \
          mlib_GraphicsDrawLine_AZ.c \
          mlib_GraphicsDrawLine_ABZ.c \
          mlib_GraphicsDrawLine_G.c \
          mlib_GraphicsDrawLine_BG.c \
          mlib_GraphicsDrawLine_GZ.c \
          mlib_GraphicsDrawLine_X.c \
          mlib_GraphicsDrawLine_B.c \
          mlib_GraphicsDrawLine_Z.c \
          mlib_GraphicsDrawPolyline.c \
          mlib_GraphicsDrawPolyline_A.c \
          mlib_GraphicsDrawPolyline_AB.c \
          mlib_GraphicsDrawPolyline_AG.c \
          mlib_GraphicsDrawPolyline_ABG.c \
          mlib_GraphicsDrawPolyline_AGZ.c \
          mlib_GraphicsDrawPolyline_ABGZ.c \
          mlib_GraphicsDrawPolyline_AZ.c \
          mlib_GraphicsDrawPolyline_ABZ.c \
          mlib_GraphicsDrawPolyline_G.c \
          mlib_GraphicsDrawPolyline_BG.c \
          mlib_GraphicsDrawPolyline_GZ.c \
          mlib_GraphicsDrawPolyline_BGZ.c \
          mlib_GraphicsDrawPolyline_X.c \
          mlib_GraphicsDrawPolyline_B.c \
          mlib_GraphicsDrawPolyline_Z.c \
          mlib_GraphicsDrawPolyline_BZ.c

SRCS_C = mlib_c_GraphicsTabAlias.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../../image/include

#
# define depends
#
__CDIF_B = ./../../image/include

$(DIR_O)/mlib_GraphicsDrawLine$(OBJ_EXT)         :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_A$(OBJ_EXT)       :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_AB$(OBJ_EXT)      :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_ABG$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_ABZ$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_AG$(OBJ_EXT)      :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_ABG$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_AGZ$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_ABGZ$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h
$(DIR_O)/mlib_GraphicsDrawLine_AZ$(OBJ_EXT)      :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_B$(OBJ_EXT)       :  ./../include/mlib_GraphicsBlend.h ./../include/mlib_GraphicsDrawSet.h \
                                                    ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageDivTables.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_BG$(OBJ_EXT)      :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_G$(OBJ_EXT)       :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_GZ$(OBJ_EXT)      :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_X$(OBJ_EXT)       :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawLine_Z$(OBJ_EXT)       :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_A$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_AB$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_ABZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_AG$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_ABG$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_AGZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_ABGZ$(OBJ_EXT):  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_AZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_B$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_BG$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_BZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_G$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_GZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_BGZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_X$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolyline_Z$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h ./../include/mlib_GraphicsMacro.h \
                                                    $(__CDIF_B)/mlib_ImageRowTable.h
$(DIR_O)/mlib_c_GraphicsTabAlias$(OBJ_EXT)       :  ./mlib_c_GraphicsTabAlias.h
