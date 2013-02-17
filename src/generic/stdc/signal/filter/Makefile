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

UTILS  = mlib_SignalLMSFilter.c  \
         mlib_SignalNLMSFilter.c \
         mlib_SignalFIR.c        \
         mlib_SignalFIR_f.c      \
         mlib_SignalIIR_P4.c

SRCS_FP = mlib_SignalEmphasize_Fp.c  \
          mlib_SignalIIR_Biquad_Fp.c \
          mlib_SignalIIR_P4_Fp.c     \
          mlib_SignalLMSFilter_Fp.c  \
          mlib_SignalNLMSFilter_Fp.c

SRCS_C  = mlib_c_SignalEmphasize.c  \
          mlib_c_SignalIIR_Biquad.c \
          mlib_c_SignalVisVersion.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
__DEP_DIR_B = ./../../include

$(DIR_O)/mlib_SignalFIR$(OBJ_EXT)           :  ./mlib_SignalFIR.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalFIR_f$(OBJ_EXT)         :  ./mlib_SignalFIR.h
$(DIR_O)/mlib_SignalIIR_P4$(OBJ_EXT)        :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalLMSFilter$(OBJ_EXT)     :  ./mlib_SignalFIR.h ./mlib_SignalLMS.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalNLMSFilter$(OBJ_EXT)    :  ./mlib_SignalFIR.h ./mlib_SignalLMS.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalLMSFilter_Fp$(OBJ_EXT)  :  ./mlib_SignalFIR.h ./mlib_SignalLMS.h
$(DIR_O)/mlib_SignalNLMSFilter_Fp$(OBJ_EXT) :  ./mlib_SignalFIR.h ./mlib_SignalLMS.h
$(DIR_O)/mlib_c_SignalVisVersion$(OBJ_EXT)  :  ./mlib_SignalVisVersion.h

