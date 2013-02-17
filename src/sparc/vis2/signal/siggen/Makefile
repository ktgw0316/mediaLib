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

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/siggen
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/siggen

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in $(UTILS_RM); do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS_RM = mlib_SignalWhiteNoise.c

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_SignalWhiteNoise.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(TOP_DIR)/src/generic/stdc/signal/siggen

# define depends

$(DIR_O)/mlib_v_SignalWhiteNoise$(OBJ_EXT)   :  $(HOME_TXT)/../../include/mlib_Utils.h \
                                                $(HOME_TXT)/mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalWhiteNoise_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalGaussNoise_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalSiggen.h
$(DIR_O)/mlib_c_SignalGaussNoise$(OBJ_EXT)   :  $(HOME_TXT)/mlib_SignalSiggen.h
$(DIR_O)/mlib_SignalSineWave_Fp$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalSiggen.h
$(DIR_O)/mlib_c_SignalSineWave$(OBJ_EXT)     :  $(HOME_TXT)/mlib_SignalSiggen.h
