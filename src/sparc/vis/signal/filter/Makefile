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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/filter

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_V  = mlib_v_SignalEmphasize.c \
          mlib_v_SignalIIR_Biquad.c \
          mlib_v_SignalVisVersion.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I${HOME_TXT} -I$(HOME_TXT)/../../include

# define depends
MLIB_UTILS_H = $(HOME_TXT)/../../include/mlib_Utils.h

$(DIR_O)/mlib_SignalFIR$(OBJ_EXT)           : ${HOME_TXT}/mlib_SignalFIR.h $(MLIB_UTILS_H)
$(DIR_O)/mlib_SignalFIR_f$(OBJ_EXT)         : ${HOME_TXT}/mlib_SignalFIR.h
$(DIR_O)/mlib_SignalIIR_P4$(OBJ_EXT)        : $(MLIB_UTILS_H)
$(DIR_O)/mlib_SignalLMSFilter$(OBJ_EXT)     : ${HOME_TXT}/mlib_SignalFIR.h ${HOME_TXT}/mlib_SignalLMS.h $(MLIB_UTILS_H)
$(DIR_O)/mlib_SignalNLMSFilter$(OBJ_EXT)    : ${HOME_TXT}/mlib_SignalFIR.h ${HOME_TXT}/mlib_SignalLMS.h $(MLIB_UTILS_H)
$(DIR_O)/mlib_SignalLMSFilter_Fp$(OBJ_EXT)  : ${HOME_TXT}/mlib_SignalFIR.h ${HOME_TXT}/mlib_SignalLMS.h
$(DIR_O)/mlib_SignalNLMSFilter_Fp$(OBJ_EXT) : ${HOME_TXT}/mlib_SignalFIR.h ${HOME_TXT}/mlib_SignalLMS.h
$(DIR_O)/mlib_v_SignalIIR_Biquad$(OBJ_EXT)  : $(MLIB_UTILS_H)
$(DIR_O)/mlib_v_SignalVisVersion$(OBJ_EXT)  : ${HOME_TXT}/mlib_SignalVisVersion.h
