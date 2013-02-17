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
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/codec

UTILS_STDC  = mlib_SignalADPCM2Bits.c \
          mlib_SignalADPCM3Bits.c \
          mlib_SignalADPCM4Bits.c \
          mlib_SignalADPCM5Bits.c \
          mlib_SignalADPCMInitFree.c \
          mlib_SignalCodecTable.c

SRCS_STDC  = \

# SSE2 specific source under src/x86/sse2
SRCS_SSE2  = mlib_s_SignalLaw2Law.c \
          mlib_s_SignalLaw2Linear.c \
          mlib_s_SignalLinear2Law.c

SRCS = $(SRCS_SSE2)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_INCLUDE)/../../sse

# define depends
$(DIR_O_STDC)/mlib_SignalADPCM2Bits$(OBJ_EXT)    : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_SignalADPCM3Bits$(OBJ_EXT)    : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_SignalADPCM4Bits$(OBJ_EXT)    : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_SignalADPCM5Bits$(OBJ_EXT)    : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_SignalADPCMInitFree$(OBJ_EXT) : $(HOME_TXT)/./mlib_SignalCodec.h

$(DIR_O_STDC)/mlib_s_SignalLaw2Law$(OBJ_EXT)     : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_s_SignalLaw2Linear$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalCodec.h
$(DIR_O_STDC)/mlib_s_SignalLinear2Law$(OBJ_EXT)  : $(HOME_TXT)/./mlib_SignalCodec.h
