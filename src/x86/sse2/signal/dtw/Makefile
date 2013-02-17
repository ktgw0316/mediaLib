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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/x86/sse2/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/dtw
HOME_ALT_TXT = $(TOP_DIR)/src/generic/stdc/$(ALT_MOD)/vector

UTILS_STDC  = mlib_SignalDTWScalar.c \
         mlib_SignalDTWVector.c \
         mlib_SignalDTWK_Kernel.c \
         mlib_SignalDTWKScalar.c \
         mlib_SignalDTWKVector.c

SRCS_ALT_C =

SRCS_ALT = mlib_s_VectorCopy.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

#
# define depends
#
$(ALT_DIR_O)/mlib_s_VectorCopy$(OBJ_EXT)  : $(HOME_ALT_TXT)/../include/mlib_AlgebraUtil.h
$(DIR_O_STDC)/mlib_SignalDTWScalar$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalDTW.h
$(DIR_O_STDC)/mlib_SignalDTWVector$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalDTW.h
$(DIR_O_STDC)/mlib_SignalDTWKScalar$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalDTWK_Kernel.h
$(DIR_O_STDC)/mlib_SignalDTWKVector$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalDTWK_Kernel.h
$(DIR_O_STDC)/mlib_SignalDTWK_Kernel$(OBJ_EXT) : $(HOME_TXT)/./mlib_SignalDTWK_Kernel.h
