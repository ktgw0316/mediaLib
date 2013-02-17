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
ALT_MOD = image
STDC_ALT_MOD = $(ALT_MOD)

ALT_DIR = $(TOP_DIR)/src/sparc/vis/$(ALT_MOD)/copy

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/point
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(STDC_ALT_MOD)/common
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/point

SRCS_STDC_ALT = mlib_ImageCreate.c mlib_ImageDivTables.c

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH=cd ${HOME_VIS_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1 = $(SRCS_VIS1_SH:sh)

SRCS_ALT_V = mlib_v_ImageClear.c \
             mlib_v_ImageClear_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_ALT_TXT)/../include -I$(ALT_DIR) -I$(HOME_VIS_TXT)/../include

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

# define depends

$(ALT_DIR_O)/mlib_ImageCreate$(OBJ_EXT)         : $(HOME_ALT_TXT)/../include/mlib_ImageRowTable.h $(HOME_ALT_TXT)/../include/mlib_ImageCreate.h
$(ALT_DIR_O)/mlib_v_ImageClear$(OBJ_EXT)        : $(HOME_ALT_TXT)/../include/mlib_ImageCheck.h $(ALT_DIR)/mlib_v_ImageClear_f.h

$(DIR_O)/mlib_GraphicsDrawPolypoint$(OBJ_EXT)   : $(HOME_ALT_TXT)/../include/mlib_ImageRowTable.h
$(DIR_O)/mlib_GraphicsDrawPolypoint_B$(OBJ_EXT) : $(HOME_ALT_TXT)/../include/mlib_ImageRowTable.h $(HOME_VIS_TXT)/../include/mlib_GraphicsBlend.h

$(DIR_O)/mlib_GraphicsDrawPoint_B$(OBJ_EXT)     : $(HOME_ALT_TXT)/../include/mlib_ImageDivTables.h

