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
MODNAME = volume

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/ray

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS_STDC   = mlib_VolumeRayCast_GDT.c   \
          mlib_VolumeRayCast_GDN.c   \
          mlib_VolumeRayCast_U8_Bit.c

SRCS_STDC  = mlib_c_VolumeRayCast_BDN.c \
          mlib_c_VolumeRayCast_BDT.c \
          mlib_c_VolumeRayCast_BPN.c \
          mlib_c_VolumeRayCast_BPT.c \
          mlib_c_VolumeRayCast_GPN.c \
          mlib_c_VolumeRayCast_GPT.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends
$(DIR_O_STDC)/mlib_c_VolumeRayCast_BDN$(OBJ_EXT) : $(HOME_TXT)/./mlib_c_VolumeRayCast_Blocked.h
$(DIR_O_STDC)/mlib_c_VolumeRayCast_BDT$(OBJ_EXT) : $(HOME_TXT)/./mlib_c_VolumeRayCast_Blocked.h
$(DIR_O_STDC)/mlib_c_VolumeRayCast_BPN$(OBJ_EXT) : $(HOME_TXT)/./mlib_c_VolumeRayCast_Blocked.h
$(DIR_O_STDC)/mlib_c_VolumeRayCast_BPT$(OBJ_EXT) : $(HOME_TXT)/./mlib_c_VolumeRayCast_Blocked.h

