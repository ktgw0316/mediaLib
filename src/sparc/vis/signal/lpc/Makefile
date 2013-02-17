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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/lpc
HOME_WIN = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/window

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_V  = mlib_v_SignalLPCAutoCorrel.c  \
          mlib_v_SignalLPCCov_S162F32.c \
          mlib_v_SignalLSP2LPC.c        \
          mlib_v_vlog.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(TOP_DIR)/src/generic/stdc/signal/fourier \
             -I$(TOP_DIR)/src/generic/stdc/signal/lpc     \
             -I$(TOP_DIR)/src/generic/stdc/signal/window

#
# define depends
#

$(DIR_O)/mlib_SignalCepstral$(OBJ_EXT)            :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalCepstral_FFT$(OBJ_EXT)        :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalCepstral_Fp$(OBJ_EXT)         :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPC2Cepstral$(OBJ_EXT)        :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPC2Cepstral_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPCCovariance$(OBJ_EXT)       :  $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLPCPerceptWeight$(OBJ_EXT)    :  $(HOME_TXT)/mlib_SignalLPCPerceptWeight.h
$(DIR_O)/mlib_SignalLPCPerceptWeight_Fp$(OBJ_EXT) :  $(HOME_TXT)/mlib_SignalLPCPerceptWeight.h
$(DIR_O)/mlib_SignalLPCPitchAnalyze$(OBJ_EXT)     :  $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLPCPitchAnalyze_Fp$(OBJ_EXT)  :  $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLSP2LPC_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_SignalMelCepstral$(OBJ_EXT)         :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalMelCepstral_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h \
                                                     $(HOME_TXT)/../window/mlib_SignalWindowUtil.h

$(DIR_O)/mlib_v_SignalLPCAutoCorrel$(OBJ_EXT)     : $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_v_SignalLPCCov_S162F32$(OBJ_EXT)    : $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_v_SignalLSP2LPC$(OBJ_EXT)           : $(HOME_TXT)/mlib_SignalLPC.h
$(DIR_O)/mlib_v_vlog$(OBJ_EXT)                    : $(HOME_TXT)/mlib_SignalCepstral.h $(HOME_TXT)/../fourier/mlib_SignalFFTSettings.h \
                                                    $(HOME_TXT)/../fourier/mlib_SignalFFTUtil.h

