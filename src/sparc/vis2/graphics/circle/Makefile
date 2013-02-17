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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/circle
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/circle
CHOME = ${HOME_TXT}

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH=cd ${HOME_VIS_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1 = $(SRCS_VIS1_SH:sh)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_VIS_TXT)/../include -I$(CHOME)/../include -I$(CHOME)/../../image/include

# define depends
__P_SUB_A = $(HOME_VIS_TXT)/../include
__P_SUB_CV = $(CHOME)/../include

$(DIR_O)/mlib_GraphicsFillArc$(OBJ_EXT)         : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillArc_A$(OBJ_EXT)       : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillArc_X$(OBJ_EXT)       : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillArc_B$(OBJ_EXT)       : $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsFillCircle$(OBJ_EXT)      : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillCircle_X$(OBJ_EXT)    : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillCircle_B$(OBJ_EXT)    : $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsFillEllipse$(OBJ_EXT)     : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillEllipse_X$(OBJ_EXT)   : $(__P_SUB_A)/mlib_GraphicsFillRow.h
$(DIR_O)/mlib_GraphicsFillEllipse_B$(OBJ_EXT)   : $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawArc_B$(OBJ_EXT)       : $(__P_SUB_A)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawCircle_B$(OBJ_EXT)    : $(__P_SUB_A)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsDrawEllipse_B$(OBJ_EXT)   : $(__P_SUB_A)/mlib_GraphicsBlend.h

$(DIR_O)/mlib_v_GraphicsFillCircle_A$(OBJ_EXT)  : $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsFillEllipse_A$(OBJ_EXT) : $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsDrawArc_A$(OBJ_EXT)     : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsDrawCircle_A$(OBJ_EXT)  : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsDrawEllipse_A$(OBJ_EXT) : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsFillArc_AB$(OBJ_EXT)    : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h \
						   $(__P_SUB_CV)/mlib_GraphicsDrawSet.h  $(__P_SUB_CV)/mlib_GraphicsMacro.h
$(DIR_O)/mlib_v_GraphicsFillCircle_AB$(OBJ_EXT) :  $(__P_SUB_A)/mlib_GraphicsFillRow.h $(__P_SUB_A)/mlib_GraphicsBlend.h \
						   $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsFillEllipse_AB$(OBJ_EXT) : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
$(DIR_O)/mlib_v_GraphicsDrawEllipse_AB$(OBJ_EXT) : $(__P_SUB_A)/mlib_v_GraphicsTabAlias.h
