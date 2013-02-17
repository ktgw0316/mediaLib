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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/format
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/format

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_SignalConvertShift_Fp.c \
         mlib_v_SignalDownSample.c \
         mlib_v_SignalLimit.c

SRCS_ALT_V = mlib_v_VectorZero.c \
             mlib_v_VectorMerge.c \
             mlib_v_VectorSplit.c \
             mlib_v_VectorCopy.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(TOP_DIR)/src/generic/stdc/signal/format \
             -I$(TOP_DIR)/src/generic/stdc/include \
             -I$(HOME_TXT)/../filter

#
# define depends
#

__DEP_DIR_B = $(TOP_DIR)/src/generic/stdc/include

$(DIR_O)/mlib_SignalConvertShift_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalReSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/mlib_SignalReSampleFIRFunc.h
$(DIR_O)/mlib_SignalUpSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/../filter/mlib_SignalFIR.h $(HOME_TXT)/mlib_SignalUpSampleFIRFunc.h

$(DIR_O)/mlib_v_SignalConvertShift_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalConvertShift_S16$(OBJ_EXT) :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalConvertShift_S32$(OBJ_EXT) :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalConvertShift_S8$(OBJ_EXT)  :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalConvertShift_U8$(OBJ_EXT)  :  $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_v_SignalUpSampleFIR_f$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalUpSampleFIRFunc.h
