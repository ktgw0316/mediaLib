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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/fourier
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/fourier
HOME_VIS2_TXT = $(MLIB_HOME)/src/sparc/vis2/$(MODNAME)/fourier

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH=cd ${HOME_VIS_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1 = $(SRCS_VIS1_SH:sh)

SRCS_S_VIS1 = mlib_v_SignalFFT_Layer.S

include $(PROJECT_ROOT)/make/rules/leaf.cfg

#
# order of '-I' is IMPORTANT here!
# make must:
#   - use mlib_SignalFFTSettings.h from $(HOME_VIS2_TXT)
#   - skip mlib_SignalFFTSettings.h from $(HOME_TXT)
#   - get mlib_SignalFFTUtil.h from $(HOME_TXT)
#
INCLUDES += -I$(HOME_VIS2_TXT) -I$(HOME_TXT) -I$(HOME_VIS_TXT)

# define depends
#
INCLUDE_FFT = $(HOME_TXT)/mlib_SignalFFTUtil.h $(HOME_VIS2_TXT)/mlib_SignalFFTSettings.h

$(DIR_O)/mlib_SignalFFT_D64$(OBJ_EXT)        : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64Disp$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64Disp_24$(OBJ_EXT) : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64Disp_32$(OBJ_EXT) : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64Disp_8$(OBJ_EXT)  : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64_1$(OBJ_EXT)      : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64_2$(OBJ_EXT)      : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_D64_3$(OBJ_EXT)      : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_F32$(OBJ_EXT)        : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_F32Disp$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_S16Disp$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_Frw$(OBJ_EXT)        : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFT_Inv$(OBJ_EXT)        : $(INCLUDE_FFT)
$(DIR_O)/mlib_SignalFFTMain$(OBJ_EXT)        : $(INCLUDE_FFT)

$(DIR_O)/mlib_v_SignalFFTDisp$(OBJ_EXT)   : $(INCLUDE_FFT) $(HOME_VIS_TXT)/mlib_v_SignalFFT_Tbl.h $(HOME_VIS_TXT)/mlib_v_SignalFFTDisp.h
$(DIR_O)/mlib_v_SignalFFTDisp_4$(OBJ_EXT) : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFTMain$(OBJ_EXT)   : $(INCLUDE_FFT) $(HOME_VIS_TXT)/mlib_v_SignalFFT_Tbl.h $(HOME_VIS_TXT)/mlib_v_SignalFFTDisp.h
$(DIR_O)/mlib_v_SignalFFTMain_4$(OBJ_EXT) : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFTW_1$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFTW_2$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFTW_3$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFTW_4$(OBJ_EXT)    : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFT_1$(OBJ_EXT)     : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFT_2$(OBJ_EXT)     : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFT_3$(OBJ_EXT)     : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFT_4$(OBJ_EXT)     : $(INCLUDE_FFT)
$(DIR_O)/mlib_v_SignalFFT_First$(OBJ_EXT) : $(INCLUDE_FFT) $(HOME_VIS_TXT)/mlib_v_SignalFFTDisp.h

$(DIR_O)/mlib_v_SignalFFT_2Lay$(OBJ_EXT)  : $(HOME_VIS_TXT)/mlib_v_SignalFFT_Tbl.h $(HOME_VIS_TXT)/mlib_v_SignalFFTDisp.h
$(DIR_O)/mlib_v_SignalFFT_Tbl$(OBJ_EXT)   : $(HOME_VIS_TXT)/mlib_v_SignalFFT_Tbl.h $(HOME_VIS_TXT)/mlib_v_SignalFFTDisp.h
