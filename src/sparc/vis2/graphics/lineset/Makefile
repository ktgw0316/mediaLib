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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/lineset
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/lineset

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n | $(EGREP) "^ *UTILS" | $(SED) -e 's/^.*=//g' | $(SORT) -u
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH=cd ${HOME_VIS_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1 = $(SRCS_VIS1_SH:sh)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_VIS_TXT)/../include -I$(HOME_TXT)/../include -I$(HOME_TXT)/../../image/include

# define depends
__P_SUB_A = $(HOME_TXT)/../include
__P_SUB_B = $(HOME_VIS_TXT)/../include

$(DIR_O)/mlib_GraphicsAliasTab$(OBJ_EXT)          :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet$(OBJ_EXT)       :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_G$(OBJ_EXT)     :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_GZ$(OBJ_EXT)    :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_BGZ$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsClipLineSet_B$(OBJ_EXT)     :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h $(__P_SUB_B)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsClipLineSet_AB$(OBJ_EXT)    :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h $(__P_SUB_B)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_GraphicsClipLineSet_Z$(OBJ_EXT)     :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_GraphicsDrawSet$(OBJ_EXT)           :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h

$(DIR_O)/mlib_v_GraphicsSpamLineSet$(OBJ_EXT)     :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_A$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_AG$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_AB$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_ABG$(OBJ_EXT) :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_AGZ$(OBJ_EXT) :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_ABGZ$(OBJ_EXT):  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_AZ$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_G$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_BG$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h $(HOME_TXT)/../../image/include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_GZ$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_BGZ$(OBJ_EXT) :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_X$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_Z$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_B$(OBJ_EXT)   :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h $(__P_SUB_B)/mlib_GraphicsBlend.h
$(DIR_O)/mlib_v_GraphicsSpamLineSet_BZ$(OBJ_EXT)  :  $(__P_SUB_A)/mlib_GraphicsDrawSet.h $(__P_SUB_B)/mlib_GraphicsBlend.h \
                                                     $(HOME_TXT)/../../image/include/mlib_ImageDivTables.h

