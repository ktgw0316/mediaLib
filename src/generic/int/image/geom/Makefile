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
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/geom

UTILS_1_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS_1 *=" | $(SED) -e 's/^.*=//g'
UTILS_1_SH = $(UTILS_1_SH_TMP:sh)

UTILS_1_STDC_TMP = aa="$(UTILS_1_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_1_STDC = $(UTILS_1_STDC_TMP:sh)

UTILS_2_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS_2 *=" | $(SED) -e 's/^.*=//g'
UTILS_2_SH = $(UTILS_2_SH_TMP:sh)

UTILS_2_STDC_TMP = bb="$(UTILS_2_SH)"; for i in `echo $(UTILS)`; do bb=`echo $$bb | $(SED) -e "s/\<$$i\>//g"`; done; echo $$bb
UTILS_2_STDC = $(UTILS_2_STDC_TMP:sh)

UTILS_STDC = $(UTILS_1_STDC) $(UTILS_2_STDC)

UTILS = mlib_ImageAffineEdge.c          	\
        mlib_ImageAffine_NN.c           	\
        mlib_ImageAffineTable_16_3nw.c  	\
        mlib_ImageAffineTable_16ext.c   	\
        mlib_ImageAffineTable_16nw.c    	\
        mlib_ImageAffineTable_32_3nw.c  	\
        mlib_ImageAffineTable_32ext.c   	\
        mlib_ImageAffineTable_32nw.c    	\
        mlib_ImageAffineTable_8_3nw.c   	\
        mlib_ImageAffineTable_8ext.c    	\
        mlib_ImageAffineTable_8nw.c     	\
        mlib_ImageAffineTable.c         	\
        mlib_ImageAffineTable_u16_3nw.c 	\
        mlib_ImageAffineTable_u16ext.c  	\
        mlib_ImageAffineTable_u16nw.c   	\
        mlib_ImageAffineTransform.c     	\
        mlib_ImageFilteredSubsample_S16.c       \
        mlib_ImageFilteredSubsample_U16.c       \
        mlib_ImageFilteredSubsample_U8.c        \
        mlib_ImageFilteredSubsam_S16_f.c        \
        mlib_ImageFilteredSubsam_U16_f.c        \
        mlib_ImageFilteredSubsam_U8_f.c         \
        mlib_ImageFilters.c             	\
        mlib_ImageFlipMainDiag.c        	\
        mlib_ImageFlipRotate.c         	 	\
        mlib_ImageFlipY_f.c             	\
        mlib_ImageGridWarp.c                    \
        mlib_ImageGridWarpTable.c		\
        mlib_ImagePolynomialWarp_0.c            \
        mlib_ImagePolynomialWarp_NN_2.c         \
        mlib_ImagePolynomialWarp_NN_3.c         \
        mlib_ImagePolynomialWarp_NN_4.c         \
        mlib_ImagePolynomialWarp_NN_5.c         \
        mlib_ImagePolynomialWarpTable_f.c       \
        mlib_ImageSubsampleAverage_S32.c	\
        mlib_ImageZoomBlendEdge.c      		\
        mlib_ImageZoomEdge.c            	\
        mlib_ImageZoomIn2X_BC2_S32.c    	\
        mlib_ImageZoomIn2X_BC_S32.c     	\
        mlib_ImageZoomIn2X_BL_S32.c     	\
        mlib_ImageZoomIn2X_Edge.c       	\
        mlib_ImageZoomIn2X_NN_S32.c     	\
        mlib_ImageZoom_NN.c             	\
        mlib_ImageZoomOut2X_BC2_S32.c   	\
        mlib_ImageZoomOut2X_BC_S32.c    	\
        mlib_ImageZoomOut2X_BL_S32.c    	\
        mlib_ImageZoomOut2X_NN_S32.c    	\
        mlib_ImageZoomTranslateTable.c          \
        mlib_ImageZoomTransTable_16ext.c        \
        mlib_ImageZoomTransTable_16nw.c         \
        mlib_ImageZoomTransTable_8ext.c         \
        mlib_ImageZoomTransTable_8nw.c          \
        mlib_ImageZoomTransTableBl_8ext.c       \
        mlib_ImageZoomTransTableBl_8nw.c        \
        mlib_ImageZoomTransTableBlEdge.c        \
        mlib_ImageZoomTransTableBlend.c         \
        mlib_ImageZoomTransTable_u16ext.c       \
        mlib_ImageZoomTransTable_u16nw.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP = mlib_ImagePolynomialWarp_NN_2_Fp.c	\
	  mlib_ImagePolynomialWarp_NN_3_Fp.c	\
	  mlib_ImagePolynomialWarp_NN_4_Fp.c	\
	  mlib_ImagePolynomialWarp_NN_5_Fp.c	\
	  mlib_ImageZoomIn2X_NN_D64_Fp.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT = mlib_i_ImageAffine_BC.c        	\
           mlib_i_ImageAffine_BC_S16.c    	\
           mlib_i_ImageAffine_BC_U16.c    	\
           mlib_i_ImageAffine_BL.c        	\
           mlib_i_ImageAffine_BL_S16.c    	\
           mlib_i_ImageAffine_BL_U16.c    	\
           mlib_i_ImageFlipMainDiag_f.c   	\
           mlib_i_ImagePolynomialWarp_BC.c      \
           mlib_i_ImagePolynomialWarp_BL.c      \
           mlib_i_ImagePolynomialWarp.c		\
           mlib_i_ImageSubsampleAverage_S16.c   \
           mlib_i_ImageSubsampleAverage_U16.c   \
           mlib_i_ImageSubsampleAverage_U8.c    \
           mlib_i_ImageZoom_BC.c          	\
           mlib_i_ImageZoom_BC_S16.c      	\
           mlib_i_ImageZoom_BC_S32.c      	\
           mlib_i_ImageZoom_BC_U16.c      	\
           mlib_i_ImageZoomBlend_BC.c     	\
           mlib_i_ImageZoomBlend_BL.c     	\
           mlib_i_ImageZoomBlend_NN.c     	\
           mlib_i_ImageZoom_BL_S16.c      	\
           mlib_i_ImageZoom_BL_U16.c      	\
           mlib_i_ImageZoomTranslateBlend.c     \
           mlib_i_ImageZoomTranslate.c    	\
           mlib_i_ImageZoom_BL.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I . \
		-I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(HOME_TXT)/../blend \
		-I$(HOME_TXT)/../copy \
		-I$(HOME_TXT)/../format \
		-I$(HOME_TXT)/../radio \
		-I$(HOME_TXT)/../../signal/fourier

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAffineTransform$(OBJ_EXT)               :  ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageGridWarpTable$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffineTable.h $(HOME_TXT)/mlib_ImageGridWarp.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageGridWarp$(OBJ_EXT)                      :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImagePolynomialWarpTable_f$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_0$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImagePolynomialWarp$(OBJ_EXT)              :  ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_i_ImagePolynomialWarp_BL$(OBJ_EXT)           :  ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImagePolynomialWarp_BC$(OBJ_EXT)           :  ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_2_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_3_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_4_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN_5_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTransTableBlend_f.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoomTransTableBl_8nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTransTableBlend_f.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsample_S16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsampleFunc.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsam_S16_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsample_f.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsample_U16$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsampleFunc.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsam_U16_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsample_f.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsample_U8$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsampleFunc.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageFilteredSubsam_U8_f$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h ./mlib_i_ImageFilteredSubsample_f.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoomTransTable_16ext$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_16nw$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8ext$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_8nw$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_u16ext$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_u16nw$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_i_ImageZoomTranslateTable_f.h

$(DIR_O)/mlib_i_ImageZoomTranslateBlend$(OBJ_EXT)          :  ./mlib_ImageFilters.h ./mlib_i_ImageZoom.h
$(DIR_O)/mlib_i_ImageZoomTranslate$(OBJ_EXT)               :  ./mlib_ImageFilters.h ./mlib_i_ImageZoom.h
$(DIR_O)/mlib_ImageZoomBlendEdge$(OBJ_EXT)                 :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomEdge$(OBJ_EXT)                      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineEdge$(OBJ_EXT)                    :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_32_3nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_32ext$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_32nw$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_u16_3nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_u16ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_u16nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_8_3nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_8ext$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_8nw$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_16nw$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_16ext$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable_16_3nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffineTable.h ./mlib_i_ImageAffineTable_func.h \
                                                              $(HOME_TXT)/mlib_ImageAffineTable_func.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageAffineTable$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/mlib_ImageAffineTable.h \
                                                              $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImageFilters$(OBJ_EXT)                       :  ./mlib_ImageFilters.h
$(DIR_O)/mlib_i_ImageAffine_BC_S16$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_i_ImageAffine_BC_U16$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_i_ImageAffine_BC$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_i_ImageAffine_BL_S16$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_i_ImageAffine_BL_U16$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_i_ImageAffine_BL$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h
$(DIR_O)/mlib_ImageAffine_NN$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_i_ImageFlipMainDiag_f$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipY_f$(OBJ_EXT)                       :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_ImageFlipMainDiag$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageSubsampleAverage_S32$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h ./mlib_i_ImageSubsampleAverage.h
$(DIR_O)/mlib_i_ImageSubsampleAverage_S16$(OBJ_EXT)        :  ./mlib_i_ImageSubsampleAverage.h
$(DIR_O)/mlib_i_ImageSubsampleAverage_U16$(OBJ_EXT)        :  ./mlib_i_ImageSubsampleAverage.h
$(DIR_O)/mlib_i_ImageSubsampleAverage_U8$(OBJ_EXT)         :  ./mlib_i_ImageSubsampleAverage.h
$(DIR_O)/mlib_i_ImageZoomBlend_BC$(OBJ_EXT)                :  ./mlib_i_ImageZoom_BC.h ./mlib_i_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoomBlend_BL$(OBJ_EXT)                :  ./mlib_i_ImageZoom_BL.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoomBlend_NN$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoom_BC_S32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageZoom.h ./mlib_ImageFilters.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoom_BC$(OBJ_EXT)                     :  ./mlib_i_ImageZoom_BC.h ./mlib_i_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoom_BC_U16$(OBJ_EXT)                 :  ./mlib_i_ImageZoom_BC.h ./mlib_i_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoom_BC_S16$(OBJ_EXT)                 :  ./mlib_i_ImageZoom_BC.h ./mlib_i_ImageZoom.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_i_ImageZoom_BL$(OBJ_EXT)                     :  ./mlib_i_ImageZoom_BL.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_i_ImageZoom_BL_U16$(OBJ_EXT)                 :  ./mlib_i_ImageZoom_BL.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_i_ImageZoom_BL_S16$(OBJ_EXT)                 :  ./mlib_i_ImageZoom_BL.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_D64_Fp$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoom_NN$(OBJ_EXT)                       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_S32$(OBJ_EXT)              :  ./mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_S32$(OBJ_EXT)              :  ./mlib_ImageZoomOut2X.h ./mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_S32$(OBJ_EXT)              :  ./mlib_ImageZoomOut2X.h ./mlib_ImageZoomOut2X_BC.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_S32$(OBJ_EXT)             :  ./mlib_ImageZoomOut2X.h ./mlib_ImageZoomOut2X_BC.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomIn2X_NN_S32$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_S32$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_S32$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_S32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageAffine$(OBJ_EXT)                        :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                              $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffineIndex$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
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
$(DIR_O)/mlib_ImageAffine_BC_D64$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_F32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BC_S32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_D64$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_F32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_BL_S32$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_Fp$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageAffine_NN_Bit$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageFilteredSubsample_D64$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_D64_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_F32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_F32_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFilteredSubsample_S32$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsampleFunc.h
$(DIR_O)/mlib_ImageFilteredSubsam_S32_f$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageFilteredSubsample.h $(HOME_TXT)/mlib_ImageFilteredSubsample_f.h
$(DIR_O)/mlib_ImageFlipAntiDiag$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipAntiDiag_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Bit$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipMainDiag_Fp$(OBJ_EXT)               :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              $(HOME_TXT)/mlib_ImageFlipMainDiag.h
$(DIR_O)/mlib_ImageFlipX$(OBJ_EXT)                         :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipX_Fp$(OBJ_EXT)                      :  $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageFlipY_Bit$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageFlipY.h $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTSettings.h \
                                                              $(HOME_TXT)/../../signal/fourier/mlib_SignalFFTUtil.h
$(DIR_O)/mlib_ImageFlipY_Fp$(OBJ_EXT)                      :  $(HOME_TXT)/mlib_ImageFlipY.h
$(DIR_O)/mlib_ImageGridWarpTable_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageGridWarp_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageGridWarp.h
$(DIR_O)/mlib_ImageInterpTable$(OBJ_EXT)                   :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h
$(DIR_O)/mlib_ImagePolynomialWarpTable$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImagePolynomialWarp.h
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
$(DIR_O)/mlib_ImagePolynomialWarp_0_Fp$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_1$(OBJ_EXT)              :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BC_S32$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_Fp$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_BL_S32$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Call_Fp$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_Fp$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
$(DIR_O)/mlib_ImagePolynomialWarp_NN$(OBJ_EXT)             :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImagePolynomialWarp.h
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
                                                              ./mlib_ImageFilters.h
$(DIR_O)/mlib_ImageSubsampleAverage_D64$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_ImageSubsampleAverage_F32$(OBJ_EXT)          :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleAverage.h
$(DIR_O)/mlib_ImageSubsampleAverage_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageSubsampleAverage_Fp.h
$(DIR_O)/mlib_ImageSubsamBinToGray4x4$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h
$(DIR_O)/mlib_ImageSubsamBinToGray2x2$(OBJ_EXT)            :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h
$(DIR_O)/mlib_ImageSubsampleBinaryToGray$(OBJ_EXT)         :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSubsampleBinaryToGray.h
$(DIR_O)/mlib_ImageZoomClipping$(OBJ_EXT)                  :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomEdgeIndex$(OBJ_EXT)                 :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                              ./mlib_ImageFilters.h \
                                                              $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC2_Edge$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BC_Edge$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_BL_Edge$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2XIndex_Edge$(OBJ_EXT)            :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoomIn2XIndex.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_Fp$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC2_S32$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BC_S32$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_Fp$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_BL_S32$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Edge_Fp$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomIn2X_Fp$(OBJ_EXT)                   :  $(HOME_TXT)/mlib_ImageZoomIn2X.h
$(DIR_O)/mlib_ImageZoomOut2X$(OBJ_EXT)                     :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XEdge$(OBJ_EXT)                 :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2XIndex_Edge$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageZoom2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_D64$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC2_F32$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_D64$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BC_F32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoom2X.h $(HOME_TXT)/mlib_ImageZoomOut2X.h \
                                                              $(HOME_TXT)/mlib_ImageZoomOut2X_BC.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_D64$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_BL_F32$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageZoomOut2X.h $(HOME_TXT)/mlib_ImageZoomOut2X_BL.h
$(DIR_O)/mlib_ImageZoomOut2X_Fp$(OBJ_EXT)                  :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_Fp$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomOut2X_NN_S32$(OBJ_EXT)              :  ./mlib_ImageZoomOut2X.h
$(DIR_O)/mlib_ImageZoomTranslateTable$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageAffine.h $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                              ./mlib_ImageFilters.h \
                                                              $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTable_32ext$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable_f.h
$(DIR_O)/mlib_ImageZoomTransTable_32nw$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
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
$(DIR_O)/mlib_ImageZoomTransTableBlend$(OBJ_EXT)           :  $(HOME_TXT)/../include/mlib_ImageInterpTable.h $(HOME_TXT)/mlib_ImageZoomTranslateTable.h
$(DIR_O)/mlib_ImageZoomTransTableBlEdge$(OBJ_EXT)          :  $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/../include/mlib_ImageInterpTable.h \
                                                              $(HOME_TXT)/mlib_ImageZoomTranslateTable.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoomTranslateToGray$(OBJ_EXT)           :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoomTranslate_Fp$(OBJ_EXT)              :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
$(DIR_O)/mlib_ImageZoom_BC_Fp$(OBJ_EXT)                    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoom_BC_Fp_D64$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h ./mlib_ImageFilters.h $(HOME_TXT)/mlib_ImageZoom.h \
                                                              $(HOME_TXT)/mlib_ImageZoom_BC.h \
                                                              ./mlib_GeomBasicIntOp.h
$(DIR_O)/mlib_ImageZoom_BL_Fp$(OBJ_EXT)                    :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_Fp_D64$(OBJ_EXT)                :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_BL_S32$(OBJ_EXT)                   :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h $(HOME_TXT)/mlib_ImageZoom_BL.h
$(DIR_O)/mlib_ImageZoom_Fp$(OBJ_EXT)                       :  $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageZoom.h
