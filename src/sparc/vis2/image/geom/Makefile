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
# ident	"@(#)Makefile	9.4	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/geom
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/geom

UTILS_1_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS_1 *=" | $(SED) -e 's/^.*=//g'
UTILS_1_STDC = $(UTILS_1_SH:sh)

UTILS_2_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS_2 *=" | $(SED) -e 's/^.*=//g'
UTILS_2_STDC = $(UTILS_2_SH:sh)

UTILS_STDC = $(UTILS_1_STDC) $(UTILS_2_STDC)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_ImageFlipMainDiag_f.c           \
         mlib_v_ImageFlipY.c                    \
         mlib_v_ImageZoomIn2X.c                 \
         mlib_v_ImageZoomIn2X_NN.c              \
         mlib_v_ImageZoomOut2X_BL.c             \
         mlib_v_ImageZoomOut2X_NN.c             \
         mlib_v_ImageZoom_NN_f.c                \
         mlib_v_ImageZoom_BC_S32.c              \
         mlib_v_ImageAffine_NN.c                \
         mlib_v_ImageAffineTable_8nw_1ch.c      \
         mlib_v_ImageAffineTable_8nw_3ch.c      \
         mlib_v_ImageZoomTransTable_3_8nw.c     \
         mlib_v_ImageFilteredSubsample.c        \
         mlib_v_ImageFilteredSubsam_S16.c       \
         mlib_v_ImageFilteredSubsam_U16.c       \
         mlib_v_ImageFilteredSubsam_S16_f.c     \
         mlib_v_ImageFilteredSubsam_U16_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../../signal/fourier -I$(HOME_VIS_TXT) -I$(HOME_VIS_TXT)/../format -I$(HOME_VIS_TXT)/../radio \
             -I$(HOME_TXT) -I$(HOME_TXT)/../copy -I$(HOME_TXT)/../include -I$(HOME_TXT)/../../signal/fourier

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAffine$(OBJ_EXT)                        :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineEdge$(OBJ_EXT)                    :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineIndex$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineTable$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/mlib_ImageAffineTable.h \
                                                              $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16_3nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16ext$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_16nw$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32_3nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32ext$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_32nw$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8_3nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8ext$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_8nw$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64_3nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_D64nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32_3nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_F32nw_2$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16_3nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffineTable_u16nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageAffine_BC_D64$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_F32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_S32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_D64$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_F32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_S32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_Fp$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_NN$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_NN_Bit$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageFilteredSubsample_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_D64_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_F32_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_S16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_S16_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_S32_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_U16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_U16_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_U8$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_U8_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilters$(OBJ_EXT)                       :  $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageFlipAntiDiag$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipAntiDiag_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Bit$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipX$(OBJ_EXT)                         :  $(HOME_TXT)/../include/mlib_ImageCreate.h ./../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipX_Fp$(OBJ_EXT)                      :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageFlipY_Bit$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageFlipY.h ./../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipY_Fp$(OBJ_EXT)                      :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_ImageFlipY_f$(OBJ_EXT)                       :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_ImageGridWarp$(OBJ_EXT)                      :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageGridWarpTable$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageGridWarp.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageGridWarpTable_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageGridWarp_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageInterpTable$(OBJ_EXT)                   :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomialWarpTable$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_f$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_ft$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTable_f.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_it$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTable_f.h
$(DIR_O)/mlib_ImagePolynomWarpTable_0$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomWarpTable_0_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTools$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarpTools.h
$(DIR_O)/mlib_ImagePolynomialWarp_0$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_0_Fp$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_1$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_S32$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_S32$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Call_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImageRotate180$(OBJ_EXT)                     :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageRotate180_Fp$(OBJ_EXT)                  :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageRotate270$(OBJ_EXT)                     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate270_Fp$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate90$(OBJ_EXT)                      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageRotate90_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageScanPoly$(OBJ_EXT)                      :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_ImageSubsampleAverage_D64$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_ImageSubsampleAverage_F32$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_ImageSubsampleAverage_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageSubsampleAverage_Fp.h
$(DIR_O)/mlib_ImageSubsampleAverage_S32$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_ImageSubsamBinToGray4x4$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h \
                                                           $(HOME_VIS_TXT)/../radio/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_ImageSubsamBinToGray2x2$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h
$(DIR_O)/mlib_ImageSubsampleBinaryToGray$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h \
                                                           $(HOME_VIS_TXT)/../radio/mlib_v_ImageLookUpFunc.h
$(DIR_O)/mlib_ImageZoomBlendEdge$(OBJ_EXT)                 :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/../../include/mlib_Utils.h
$(DIR_O)/mlib_ImageZoomClipping$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdge$(OBJ_EXT)                      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdgeIndex$(OBJ_EXT)                 :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdge_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC2_Edge$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC_Edge$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BL_Edge$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_Edge$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_Fp$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_D64_Fp$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_S32$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomOut2X$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XEdge$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XIndex_Edge$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoom2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_D64$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_F32$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_S32$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_D64$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_F32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_S32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_D64$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_F32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_S32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_Fp$(OBJ_EXT)                  :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_Fp$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_S32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomTranslateTable$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTable_16ext$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_16nw$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_32ext$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_32nw$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8ext$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8nw$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_D64ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_D64nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_F32ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_F32nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTranslateTable_Fp$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTable_u16ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_u16nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTransTableBlend_f.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTransTableBlend_f.h
$(DIR_O)/mlib_ImageZoomTranslateTableBlend$(OBJ_EXT)       :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTableBlEdge$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTranslateToGray$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomTranslate_Fp$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoom_BC_Fp$(OBJ_EXT)                    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h
$(DIR_O)/mlib_ImageZoom_BC_Fp_D64$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h
$(DIR_O)/mlib_ImageZoom_BL_Fp$(OBJ_EXT)                    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_Fp_D64$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_S32$(OBJ_EXT)                   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_Fp$(OBJ_EXT)                       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoom_NN$(OBJ_EXT)                       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h


#
# VIS
#

$(DIR_O)/mlib_v_ImageAffineIndex_BC$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                           $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffineTable_8nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageAffineTable_8nw_2ch$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h
$(DIR_O)/mlib_v_ImageAffineTable_8nw_4ch$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h
$(DIR_O)/mlib_v_ImageAffineTable_8nw_4ch2$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h
$(DIR_O)/mlib_v_ImageAffine_BC_S16$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffine_BC_U16$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageFilteredSubsample_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h
$(DIR_O)/mlib_v_ImageFilteredSubsample_U8$(OBJ_EXT)     :  $(HOME_VIS_TXT)/mlib_v_ImageFilteredSubsample.h
$(DIR_O)/mlib_v_ImageFilters$(OBJ_EXT)                  :  $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageGetAffineFunc$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_v_ImageGetZoomFunc$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_v_ImagePolynomialWarp$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImagePolynomialWarp_BC$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImagePolynomialWarp_BL$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_v_ImagePolynomialWarp_Call$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_v_ImageSubsampleAverage$(OBJ_EXT)         :  $(HOME_VIS_TXT)/mlib_v_ImageSubsampleAverage.h
$(DIR_O)/mlib_v_ImageSubsampleAverage_S16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageSubsampleAverage_U16$(OBJ_EXT)     :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageSubsampleAverage_U8$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h
$(DIR_O)/mlib_v_ImageSubsamAverage_U8_2$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h
$(DIR_O)/mlib_v_ImageZoomBlend_NN$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageZoomBlend_NN_f$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_v_ImageZoomIn2XIndex$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_v_ImageZoomIn2XIndex_BC$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_v_ImageZoomIn2XIndex_BC2$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_v_ImageZoomIn2XIndex_BL$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC2$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC2_S16$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC2_U16$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC_S16$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BC_U16$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BL$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BL_S16$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_BL_U16$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIndex$(OBJ_EXT)                :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_v_ImageZoomIndex_BC$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIndex_BC.h
$(DIR_O)/mlib_v_ImageZoomIndex_BL$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_v_ImageZoomOut2XIndex$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomOut2X_BC$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomOut2X_BC_S16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomOut2X_BC_U16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomTranslate$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoomTranslateBlend$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_ImageZoomTranslateBlend$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_v_ImageZoomTransTable_1_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomTransTable_8nw.h
$(DIR_O)/mlib_v_ImageZoomTransTable_2_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomTransTable_8nw.h
$(DIR_O)/mlib_v_ImageZoomTransTable_4_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomTransTable_8nw.h
$(DIR_O)/mlib_v_ImageZoomTransTable_8nw$(OBJ_EXT)       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomTransTable_8nw.h
$(DIR_O)/mlib_v_ImageZoom_BC$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoom_BC_S16$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoom_BC_U16$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoom_BC_U16$(OBJ_EXT).cc           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoom_BC_f$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageZoom_BL$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageZoom_BL_S16$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoom_BL.h
$(DIR_O)/mlib_v_ImageZoom_BL_U16$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoom_BL.h
#
# VIS2
#

$(DIR_O)/mlib_v_ImageAffineTable_8nw_1ch$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h
$(DIR_O)/mlib_v_ImageAffineTable_8nw_3ch$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageAffineTable_8nw.h
$(DIR_O)/mlib_v_ImageAffine_BC$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffine_BL$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                           $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffine_BL_S16$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                           $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffine_BL_U16$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                           $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                           $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h \
                                                           $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFilters.h
$(DIR_O)/mlib_v_ImageAffine_NN$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFilters.h
$(DIR_O)/mlib_v_ImageFilteredSubsample$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_VIS_TXT)/mlib_v_ImageFilteredSubsample.h \
                                                           ./mlib_v_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_v_ImageFilteredSubsam_S16$(OBJ_EXT)       :  ./mlib_v_ImageFilteredSubsample_16.h \
                                                           ./mlib_v_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_v_ImageFilteredSubsam_S16_f$(OBJ_EXT)     :  ./mlib_v_ImageFilteredSubsam_16_f.h \
                                                           ./mlib_v_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_v_ImageFilteredSubsam_U16$(OBJ_EXT)       :  ./mlib_v_ImageFilteredSubsample_16.h \
                                                           ./mlib_v_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_v_ImageFilteredSubsam_U16_f$(OBJ_EXT)     :  ./mlib_v_ImageFilteredSubsam_16_f.h \
                                                           ./mlib_v_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_v_ImageFlipMainDiag_f$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                           $(HOME_TXT)/mlib_ImageFlipMainDiag.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageFlipMainDiag.h
$(DIR_O)/mlib_v_ImageFlipY$(OBJ_EXT)                    :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_v_ImageZoomIn2X$(OBJ_EXT)                 :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomIn2X_NN$(OBJ_EXT)              :  $(HOME_VIS_TXT)/mlib_v_ImageZoomIn2X_proto.h
$(DIR_O)/mlib_v_ImageZoomOut2X_BL$(OBJ_EXT)             :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomOut2X_NN$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_v_ImageZoomTransTable_3_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                           $(HOME_VIS_TXT)/mlib_v_ImageZoomTransTable_8nw.h
$(DIR_O)/mlib_v_ImageZoom_BC_S32$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_v_ImageZoom_NN_f$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
