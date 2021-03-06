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

UTILS   = mlib_GraphicsAliasTab.c \
          mlib_GraphicsDrawSet.c \
          mlib_GraphicsClipLineSet.c \
          mlib_GraphicsClipLineSet_G.c \
          mlib_GraphicsClipLineSet_BG.c \
          mlib_GraphicsClipLineSet_GZ.c \
          mlib_GraphicsClipLineSet_BGZ.c \
          mlib_GraphicsClipLineSet_Z.c  \
          mlib_GraphicsClipLineSet_B.c  \
          mlib_GraphicsClipLineSet_AB.c \
          mlib_GraphicsClipLineSet_BZ.c

SRCS_C  = mlib_c_GraphicsDivTable.c \
          mlib_c_GraphicsSpamLineSet.c \
          mlib_c_GraphicsSpamLineSet_A.c \
          mlib_c_GraphicsSpamLineSet_AB.c \
          mlib_c_GraphicsSpamLineSet_ABG.c \
          mlib_c_GraphicsSpamLineSet_AG.c \
          mlib_c_GraphicsSpamLineSet_AGZ.c \
          mlib_c_GraphicsSpamLineSet_ABGZ.c \
          mlib_c_GraphicsSpamLineSet_AZ.c \
          mlib_c_GraphicsSpamLineSet_ABZ.c \
          mlib_c_GraphicsSpamLineSet_G.c \
          mlib_c_GraphicsSpamLineSet_BG.c \
          mlib_c_GraphicsSpamLineSet_GZ.c \
          mlib_c_GraphicsSpamLineSet_BGZ.c \
          mlib_c_GraphicsSpamLineSet_X.c \
          mlib_c_GraphicsSpamLineSet_B.c \
          mlib_c_GraphicsSpamLineSet_Z.c \
          mlib_c_GraphicsSpamLineSet_BZ.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I../../image/include

#
# define depends
#
__CDIF_B = ./../../image/include

$(DIR_O)/mlib_GraphicsAliasTab$(OBJ_EXT)          :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet$(OBJ_EXT)       :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_B$(OBJ_EXT)     :  ./../include/mlib_GraphicsBlend.h ./../include/mlib_GraphicsDrawSet.h \
                                                     $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_GraphicsClipLineSet_AB$(OBJ_EXT)    :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_BG$(OBJ_EXT)    :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_BZ$(OBJ_EXT)    :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_G$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_GZ$(OBJ_EXT)    :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_BGZ$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_Z$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsDrawSet$(OBJ_EXT)           :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsDivTable$(OBJ_EXT)        :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet$(OBJ_EXT)     :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_A$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_ABG$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_ABZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_AG$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_AGZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_ABGZ$(OBJ_EXT):  ./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_AZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_B$(OBJ_EXT)   :  ./../include/mlib_GraphicsBlend.h ./../include/mlib_GraphicsDrawSet.h \
                                                     $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_BG$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_BZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_G$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_GZ$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_BGZ$(OBJ_EXT) :  ./../include/mlib_GraphicsDrawSet.h $(__CDIF_B)/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_X$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_Z$(OBJ_EXT)   :  ./../include/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_c_GraphicsSpamLineSet_AB$(OBJ_EXT)  :  ./../include/mlib_GraphicsDrawSet.h
