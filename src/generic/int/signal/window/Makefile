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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/window

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS = mlib_SignalMulKaiser.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP =

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  = mlib_i_SignalMulRectangular.c	\
	    mlib_i_SignalMulWindow.c		\
	    mlib_i_SignalMulBartlett.c		\
	    mlib_i_SignalMulHanning.c		\
	    mlib_i_SignalMulHamming.c		\
	    mlib_i_SignalMulBlackman.c		\
	    mlib_i_SignalWindow.c		\
	    mlib_i_SignalWinTab.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include

# define depends

$(DIR_O)/mlib_SignalMulBartlett1_Fp$(OBJ_EXT)   : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulBlackman_Fp$(OBJ_EXT)    : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulHamming_Fp$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulHanning_Fp$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalMulKaiser_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalWindow_Fp$(OBJ_EXT)         : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h

$(DIR_O)/mlib_SignalMulKaiser$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                              $(HOME_TXT)/mlib_SignalWindow.h \
					      ./mlib_i_SignalWinTab.h
$(DIR_O)/mlib_SignalWindowUtil$(OBJ_EXT)    : $(HOME_TXT)/mlib_SignalWindowUtil.h
$(DIR_O)/mlib_i_SignalMulBartlett$(OBJ_EXT)     :$(HOME_TXT)/mlib_SignalWindowUtil.h \
						$(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_i_SignalMulHanning$(OBJ_EXT)     :$(HOME_TXT)/mlib_SignalWindowUtil.h \
						$(HOME_TXT)/mlib_SignalWindow.h \
					       ./mlib_i_SignalWinTab.h
$(DIR_O)/mlib_i_SignalMulHamming$(OBJ_EXT)     :$(HOME_TXT)/mlib_SignalWindowUtil.h \
						$(HOME_TXT)/mlib_SignalWindow.h \
					       ./mlib_i_SignalWinTab.h
$(DIR_O)/mlib_i_SignalMulBlackman$(OBJ_EXT)     :$(HOME_TXT)/mlib_SignalWindowUtil.h \
						$(HOME_TXT)/mlib_SignalWindow.h \
					       ./mlib_i_SignalWinTab.h
$$(DIR_O)/mlib_i_SignalWindow$(OBJ_EXT)     :$(HOME_TXT)/mlib_SignalWindowUtil.h \
						$(HOME_TXT)/mlib_SignalWindow.h \
					       ./mlib_i_SignalWinTab.h
(DIR_O)/mlib_i_SignalWinTab$(OBJ_EXT)     : 	./mlib_i_SignalWinTab.h
