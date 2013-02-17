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
# ident	"@(#)Makefile	9.3	07/10/09 SMI"
#

TOP_DIR = ../../../../..
MODNAME = signal

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  = mlib_SignalLPCAutoCorrel_Fp.c   \
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

SRCS_C  =  mlib_c_SignalLPCAutoCorrel.c  \
           mlib_c_SignalLPCCov_S162F32.c \
           mlib_c_SignalLSP2LPC.c        \
           mlib_c_vlog.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../window -I./../fourier

# define depends
#

$(DIR_O)/mlib_SignalCepstral$(OBJ_EXT)            :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalCepstral_FFT$(OBJ_EXT)        :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalCepstral_Fp$(OBJ_EXT)         :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPC2Cepstral$(OBJ_EXT)        :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPC2Cepstral_Fp$(OBJ_EXT)     :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalLPCCovariance$(OBJ_EXT)       :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLPCPerceptWeight$(OBJ_EXT)    :  ./mlib_SignalLPCPerceptWeight.h
$(DIR_O)/mlib_SignalLPCPerceptWeight_Fp$(OBJ_EXT) :  ./mlib_SignalLPCPerceptWeight.h
$(DIR_O)/mlib_SignalLPCPitchAnalyze$(OBJ_EXT)     :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLPCPitchAnalyze_Fp$(OBJ_EXT)  :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_SignalLSP2LPC_Fp$(OBJ_EXT)          :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_SignalMelCepstral$(OBJ_EXT)         :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_SignalMelCepstral_Fp$(OBJ_EXT)      :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h \
                                                     ./../window/mlib_SignalWindowUtil.h
$(DIR_O)/mlib_c_SignalLPCCov_S162F32$(OBJ_EXT)    :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_c_SignalLSP2LPC$(OBJ_EXT)           :  ./mlib_SignalLPC.h
$(DIR_O)/mlib_c_vlog$(OBJ_EXT)                    :  ./mlib_SignalCepstral.h ./../fourier/mlib_SignalFFTSettings.h \
                                                     ./../fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_c_SignalLPCAutoCorrel$(OBJ_EXT)     :  ./mlib_SignalLPC.h
