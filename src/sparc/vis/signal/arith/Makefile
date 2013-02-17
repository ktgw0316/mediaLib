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
ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/arith

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_V  = mlib_v_SignalAutoCorrel.c \
          mlib_v_SignalConv_f.c \
          mlib_v_SignalCrossCorrel.c \
          mlib_v_SignalMul.c \
          mlib_v_SignalMulS.c \
          mlib_v_SignalMulSAdd.c \
          mlib_v_SignalMulSShift.c \
          mlib_v_SignalMulSShiftAdd.c \
          mlib_v_SignalMulShift.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(TOP_DIR)/src/generic/stdc/signal/fourier \
             -I$(TOP_DIR)/src/generic/stdc/include        \
             -I$(TOP_DIR)/src/generic/stdc/signal/arith

#
# define depends
#
__DEP_DIR_A = $(TOP_DIR)/src/generic/stdc/signal/arith
__DEP_DIR_B = $(TOP_DIR)/src/generic/stdc/signal/fourier
__DEP_DIR_C = $(TOP_DIR)/src/generic/stdc/include

$(DIR_O)/mlib_SignalAutoCorrel_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalCorrel.h
$(DIR_O)/mlib_SignalConv$(OBJ_EXT)           :  $(HOME_TXT)/mlib_SignalConv.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O)/mlib_SignalCrossCorrel_Fp$(OBJ_EXT) :  $(HOME_TXT)/mlib_SignalCorrel.h


$(DIR_O)/mlib_v_SignalAutoCorrel$(OBJ_EXT)   :  $(__DEP_DIR_A)/mlib_SignalCorrel.h
$(DIR_O)/mlib_v_SignalConv_f$(OBJ_EXT)       :  $(__DEP_DIR_A)/mlib_SignalConv.h $(__DEP_DIR_B)/mlib_SignalFFTSettings.h \
                                                $(__DEP_DIR_B)/mlib_SignalFFTUtil.h \
                                                $(__DEP_DIR_C)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalCrossCorrel$(OBJ_EXT)  :  $(__DEP_DIR_A)/mlib_SignalCorrel.h ./mlib_v_SignalArith.h
