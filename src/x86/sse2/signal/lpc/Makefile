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

HOME_TXT     = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/lpc

UTILS_STDC  = mlib_SignalLPCAutoCorrel_Fp.c   \
         mlib_SignalLPC2LSP.c            \
         mlib_SignalLPC2LSP_Fp.c         \
         mlib_SignalLPC2Cepstral_Fp.c    \
         mlib_SignalLPCCovariance.c      \
         mlib_SignalLPCCovariance_Fp.c   \
         mlib_SignalLSP2LPC_Fp.c         \
         mlib_SignalLPCPitchAnalyze_Fp.c \
         mlib_SignalLPCPitchAnalyze.c    \
         mlib_SignalCepstral_FFT.c       \
         mlib_SignalCepstral.c           \
         mlib_SignalCepstral_Fp.c        \
         mlib_SignalMelCepstral.c        \
         mlib_SignalMelCepstral_Fp.c     \
         mlib_SignalLPC2Cepstral.c       \
         mlib_SignalLPCPerceptWeight_Fp.c\
         mlib_SignalLPCPerceptWeight.c

SRCS_STDC  =  mlib_c_SignalLPCAutoCorrel.c  \
           mlib_c_SignalLPCCov_S162F32.c \
           mlib_c_SignalLSP2LPC.c        \
           mlib_c_vlog.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/. -I$(HOME_TXT)/./../window -I$(HOME_TXT)/./../fourier -I$(HOME_INCLUDE)/../../sse

# define depends
#

$(DIR_O_STDC)/mlib_SignalCepstral$(OBJ_EXT)            :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalCepstral_FFT$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalCepstral_Fp$(OBJ_EXT)         :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalLPC2Cepstral$(OBJ_EXT)        :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalLPC2Cepstral_Fp$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalLPCCovariance$(OBJ_EXT)       :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_SignalLPCPerceptWeight$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalLPCPerceptWeight.h
$(DIR_O_STDC)/mlib_SignalLPCPerceptWeight_Fp$(OBJ_EXT) :  $(HOME_TXT)/./mlib_SignalLPCPerceptWeight.h
$(DIR_O_STDC)/mlib_SignalLPCPitchAnalyze$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_SignalLPCPitchAnalyze_Fp$(OBJ_EXT)  :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_SignalLSP2LPC_Fp$(OBJ_EXT)          :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_SignalMelCepstral$(OBJ_EXT)         :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_SignalMelCepstral_Fp$(OBJ_EXT)      :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h \
                                                     $(HOME_TXT)/./../window/mlib_SignalWindowUtil.h
$(DIR_O_STDC)/mlib_c_SignalLPCCov_S162F32$(OBJ_EXT)    :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_c_SignalLSP2LPC$(OBJ_EXT)           :  $(HOME_TXT)/./mlib_SignalLPC.h
$(DIR_O_STDC)/mlib_c_vlog$(OBJ_EXT)                    :  $(HOME_TXT)/./mlib_SignalCepstral.h $(HOME_TXT)/./../fourier/mlib_SignalFFTSettings.h \
                                                     $(HOME_TXT)/./../fourier/mlib_SignalFFTUtil.h
$(DIR_O_STDC)/mlib_c_SignalLPCAutoCorrel$(OBJ_EXT)     :  $(HOME_TXT)/./mlib_SignalLPC.h
