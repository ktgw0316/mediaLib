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
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.2	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

ALT_MOD = algebra
ALT_DIR = $(TOP_DIR)/src/generic/int/$(ALT_MOD)/vector

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/format

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS = mlib_SignalUpSampleFIR.c  \
	mlib_SignalReSampleFIR.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP = mlib_SignalUpSample_Fp.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  = 	mlib_i_SignalConvertShift.c   \
		mlib_i_SignalUpSample.c       \
		mlib_i_SignalDownSample.c     \
		mlib_i_SignalDownSample_Fp.c  \
		mlib_i_SignalUpSampleFIR_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(HOME_TXT)/../filter

#
# define depends
#
__DEP_DIR_B = $(TOP_DIR)/src/generic/stdc/include

$(DIR_O)/mlib_SignalConvertShift_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_SignalConvertShift.h $(__DEP_DIR_B)/mlib_Utils.h
$(DIR_O)/mlib_SignalReSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/mlib_SignalReSampleFIRFunc.h
$(DIR_O)/mlib_SignalUpSampleFIR$(OBJ_EXT)       :  $(HOME_TXT)/../filter/mlib_SignalFIR.h $(HOME_TXT)/mlib_SignalUpSampleFIRFunc.h
