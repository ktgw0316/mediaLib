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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.3	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/fourier
HOME_INT_TXT = $(MLIB_HOME)/src/generic/int/$(MODNAME)/fourier

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =		mlib_SignalFFT_Frw.c	\
		mlib_SignalFFT_Inv.c	\
		mlib_SignalFFT_D64.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  =	mlib_i_SignalFFTDisp.c	\
		mlib_i_SignalFFTMain.c	\
		mlib_i_SignalFFTW_1.c	\
		mlib_i_SignalFFTW_2.c	\
		mlib_i_SignalFFTW_3.c	\
		mlib_i_SignalFFTW_4.c	\
		mlib_i_SignalFFT_1.c	\
		mlib_i_SignalFFT_2.c	\
		mlib_i_SignalFFT_3.c	\
		mlib_i_SignalFFT_4.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_INT_TXT) \
		-I$(HOME_TXT)/../include

#
# define depends
#
$(DIR_O)/mlib_SignalFFT_D64$(OBJ_EXT)        :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_24$(OBJ_EXT) :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_32$(OBJ_EXT) :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64Disp_8$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_1$(OBJ_EXT)      :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_2$(OBJ_EXT)      :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_D64_3$(OBJ_EXT)      :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_F32$(OBJ_EXT)        :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_F32Disp$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_S16Disp$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_Frw$(OBJ_EXT)        :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFT_Inv$(OBJ_EXT)        :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_SignalFFTMain$(OBJ_EXT)        :  $(HOME_TXT)/mlib_SignalFFTSettings.h $(HOME_TXT)/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTDisp.c$(OBJ_EXT)    :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTMain.c$(OBJ_EXT)    :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTW_1.c$(OBJ_EXT)     :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTW_2.c$(OBJ_EXT)     :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTW_3.c$(OBJ_EXT)     :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFTW_4.c$(OBJ_EXT)     :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFT_1.c$(OBJ_EXT)      :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFT_2.c$(OBJ_EXT)      :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFT_3.c$(OBJ_EXT)      :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h
$(DIR_O)/mlib_i_SignalFFT_4.c$(OBJ_EXT)      :  $(HOME_INT_TXT)/mlib_i_SignalFFTSettings.h $(HOME_INT_TXT)/mlib_i_SignalFFTUtil.h

