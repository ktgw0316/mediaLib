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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/line

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_V  = mlib_v_GraphicsTabAlias.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR = $(TOP_DIR)/src/generic/stdc/image/include

INCLUDES += -I$(ALT_DIR) -I$(HOME_TXT)/../include

# define depends
INCLUDE1=$(HOME_TXT)/../include/mlib_GraphicsDrawSet.h \
         $(HOME_TXT)/../include/mlib_GraphicsMacro.h \
         $(ALT_DIR)/mlib_ImageRowTable.h

$(DIR_O)/mlib_GraphicsDrawLine$(OBJ_EXT)         : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_A$(OBJ_EXT)       : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_AG$(OBJ_EXT)      : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_AB$(OBJ_EXT)      : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_AGZ$(OBJ_EXT)     : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_AZ$(OBJ_EXT)      : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_G$(OBJ_EXT)       : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_GZ$(OBJ_EXT)      : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_X$(OBJ_EXT)       : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawLine_B$(OBJ_EXT)       : $(INCLUDE1) ./../include/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawLine_Z$(OBJ_EXT)       : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline$(OBJ_EXT)     : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_A$(OBJ_EXT)   : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_AG$(OBJ_EXT)  : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_AB$(OBJ_EXT)  : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_AGZ$(OBJ_EXT) : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_AZ$(OBJ_EXT)  : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_G$(OBJ_EXT)   : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_GZ$(OBJ_EXT)  : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_BGZ$(OBJ_EXT) : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_X$(OBJ_EXT)   : $(INCLUDE1)
$(DIR_O)/mlib_GraphicsDrawPolyline_B$(OBJ_EXT)   : $(INCLUDE1) ./../include/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawPolyline_Z$(OBJ_EXT)   : $(INCLUDE1)
$(DIR_O)/mlib_v_GraphicsTabAlias$(OBJ_EXT)       : ./../include/mlib_v_GraphicsTabAlias.h
