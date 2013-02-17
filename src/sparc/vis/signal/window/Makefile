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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/window

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n SRCS_ALT="" com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com SRCS_ALT="" | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FPSH = $(SRCS_FPSH_TMP:sh)
SRCS_STDC_FP_TMP = qq="$(SRCS_FPSH)"; for i in $(SRCS_FP_RM); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP_RM = mlib_SignalMulBartlett_Fp.c

SRCS_V  = mlib_v_SignalMulBartlett.c \
          mlib_v_SignalMulBartlett_Fp.c \
          mlib_v_SignalMulBlackman.c \
          mlib_v_SignalMulHamming.c \
          mlib_v_SignalMulHanning.c \
          mlib_v_SignalMulRectangular.c \
          mlib_v_SignalMulWindow.c \
          mlib_v_SignalWindow.c

SRCS_ALT_V = mlib_v_VectorCopy.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

ALT_DIR_O = $(DIR_O)/../$(ALT_MOD)

INCLUDES += -I$(HOME_TXT)

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

$(DIR_O)/mlib_SignalMulKaiser$(OBJ_EXT)         : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_SignalWindowUtil$(OBJ_EXT)        : $(HOME_TXT)/mlib_SignalWindowUtil.h

$(DIR_O)/mlib_v_SignalWindow$(OBJ_EXT)          : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_v_SignalMulBartlett$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_v_SignalMulBartlett_Fp$(OBJ_EXT)  : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_v_SignalMulBlackman$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_v_SignalMulHamming$(OBJ_EXT)      : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
$(DIR_O)/mlib_v_SignalMulHanning$(OBJ_EXT)      : $(HOME_TXT)/mlib_SignalWindowUtil.h \
                                                  $(HOME_TXT)/mlib_SignalWindow.h
