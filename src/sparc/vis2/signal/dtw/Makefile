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
MODNAME = signal

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/sparc/vis/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/dtw

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_ALT_V = mlib_v_VectorCopy.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

INCLUDES += -I$(HOME_TXT)

#
# define depends
#

$(DIR_O)/mlib_SignalDTWScalar$(OBJ_EXT)  : $(HOME_TXT)/mlib_SignalDTW.h
$(DIR_O)/mlib_SignalDTWVector$(OBJ_EXT)  : $(HOME_TXT)/mlib_SignalDTW.h
$(DIR_O)/mlib_SignalDTWK_Kernel$(OBJ_EXT) : $(HOME_TXT)/mlib_SignalDTWK_Kernel.h
$(DIR_O)/mlib_SignalDTWKScalar$(OBJ_EXT)  : $(HOME_TXT)/mlib_SignalDTWK_Kernel.h
$(DIR_O)/mlib_SignalDTWKVector$(OBJ_EXT)  : $(HOME_TXT)/mlib_SignalDTWK_Kernel.h

