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

TOP_DIR = ../../../..
MODNAME = image
ALT_MOD = signal
SUNMATH_MOD = sunmath

LIBNAME = mlib_$(MODNAME)

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

include $(PROJECT_ROOT)/make/main.cfg
include $(CONFIG.$(HOST_ARCH_NAME).PROJECT)
include $(CONFIG.$(TARCH).OVERRIDES)
include $(CONFIG.$(TARCH).C)

SUB_DIRS = arith blend color common copy format fourier geom logic radio spatial stat

OBJS_ALT = mlib_SignalFFT_Frw \
           mlib_SignalFFT_Inv \
           mlib_SignalFFT_D64 \
           mlib_SignalFFT_D64Disp \
           mlib_SignalFFT_D64Disp_8 \
           mlib_SignalFFT_D64Disp_24 \
           mlib_SignalFFT_D64Disp_32 \
           mlib_SignalFFT_D64_1 \
           mlib_SignalFFT_D64_2 \
           mlib_SignalFFT_D64_3


OBJS_SUNMATH_COM = _TBL_ipio2 \
                   __rem_pio2 \
                   __rem_pio2f_ \
                   __rem_pio2m  \
                   sincos

sparc_OBJS_SUNMATH =  _TBL_sin \
                      _TBL_cos \
                      __sincos \
                      $(OBJS_SUNMATH_COM)

sparc-debug_OBJS_SUNMATH = $($(TARCH)_OBJS_SUNMATH)

i386_OBJS_SUNMATH = $(OBJS_SUNMATH_COM) __reduction
x86_OBJS_SUNMATH  = $($(TARCH)_OBJS_SUNMATH)
x86-debug_OBJS_SUNMATH = $($(TARCH)_OBJS_SUNMATH)

OBJS =  $(DIR_O)/*$(OBJ_EXT) $(DIR_SYS)/*$(OBJ_EXT) \
        $(OBJS_ALT:%=$(DIR_O)/../$(ALT_MOD)/%$(OBJ_EXT)) \
        $($(USE_EMBEDDED_SUNMATH)$(ARCH)_OBJS_SUNMATH:%=$(DIR_O)/../$(SUNMATH_MOD)/%$(OBJ_EXT))

MAPFILES = ./mapfile ../mapfile ../signal/mapfile_common ./mapfile_common

include $(PROJECT_ROOT)/make/rules/directory.cfg
include $(CONFIG.$(TARCH).MAKE)

include $(PROJECT_ROOT)/make/rules/lib.cfg
