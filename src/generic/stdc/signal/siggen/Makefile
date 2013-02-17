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
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

SRCS_C  = mlib_c_SignalGaussNoise.c \
          mlib_c_SignalSineWave.c

SRCS = $(SRCS_C)

UTILS = mlib_SignalWhiteNoise.c \
        mlib_SignalWhiteNoise_Fp.c \
        mlib_SignalGaussNoise_Fp.c \
        mlib_SignalSineWave_Fp.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
$(DIR_O)/mlib_SignalWhiteNoise$(OBJ_EXT)     :  ./mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalWhiteNoise_Fp$(OBJ_EXT)  :  ./mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalGaussNoise_Fp$(OBJ_EXT)  :  ./mlib_SignalSiggen.h
$(DIR_O)/mlib_c_SignalGaussNoise$(OBJ_EXT)   :  ./mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalSineWave_Fp$(OBJ_EXT)    :  ./mlib_SignalSiggen.h
$(DIR_O)/mlib_c_SignalSineWave$(OBJ_EXT)     :  ./mlib_SignalSiggen.h
