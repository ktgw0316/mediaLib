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
MODNAME = graphics

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/polygon

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_V  = mlib_v_GraphicsFillPolygon_G.c \
          mlib_v_GraphicsFillPolygon_Z.c \
          mlib_v_GraphicsFillPolygon_GZ.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR = $(TOP_DIR)/src/generic/stdc/image/include

INCLUDES += -I$(ALT_DIR) -I$(HOME_TXT)/../include

# define depends
INCLUDE1=$(HOME_TXT)/../include/mlib_GraphicsDrawSet.h \
         $(HOME_TXT)/../include/mlib_GraphicsMacro.h \
         $(ALT_DIR)/mlib_ImageRowTable.h

$(DIR_O)/mlib_GraphicsDrawPolygon$(OBJ_EXT)     :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_A$(OBJ_EXT)   :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_AB$(OBJ_EXT)  :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_AG$(OBJ_EXT)  :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_ABG$(OBJ_EXT) :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_AGZ$(OBJ_EXT) :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_ABGZ$(OBJ_EXT):  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_AZ$(OBJ_EXT)  :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_G$(OBJ_EXT)   :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_BG$(OBJ_EXT)  :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_GZ$(OBJ_EXT)  :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_BGZ$(OBJ_EXT) :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_X$(OBJ_EXT)   :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolygon_B$(OBJ_EXT)   :  $(INCLUDE1) ./../include/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawPolygon_Z$(OBJ_EXT)   :  $(INCLUDE1)
$(DIR_O)/mlib_GraphicsFillPolygon$(OBJ_EXT)     :  ./../include/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillPolygon_X$(OBJ_EXT)   :  ./../include/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillPolygon_B$(OBJ_EXT)   :  ./../include/mlib_GraphicsFillRow.h ./../include/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsFillPolygon_AB$(OBJ_EXT)  :  ./../include/mlib_GraphicsFillRow.h ./../include/mlib_GraphicsBlend.h
