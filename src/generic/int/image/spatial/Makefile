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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/spatial

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS =		mlib_ImageConv2x2_f.c          \
		mlib_ImageConvIndex_16_16ext.c \
		mlib_ImageConvIndex_16_16nw.c  \
		mlib_ImageConvIndex_16_8ext.c  \
		mlib_ImageConvIndex_16_8nw.c   \
		mlib_ImageConvIndex_8_16ext.c  \
		mlib_ImageConvIndex_8_16nw.c   \
		mlib_ImageConvIndex_8_8ext.c   \
		mlib_ImageConvIndex_8_8nw.c    \
		mlib_ImageConv_16ext.c         \
		mlib_ImageConv_16nw.c          \
		mlib_ImageConv_32ext.c         \
		mlib_ImageConv_32nw.c          \
		mlib_ImageConv_8ext.c          \
		mlib_ImageConv_8nw.c           \
		mlib_ImageConv_u16ext.c        \
		mlib_ImageConv_u16nw.c         \
		mlib_ImageDilate4_32nw.c       \
		mlib_ImageErode4_32nw.c        \
		mlib_ImageGradient3x3.c	       \
		mlib_ImageGradientMxN.c	       \
		mlib_ImageMaxFilter3x3_32nw.c  \
		mlib_ImageMaxFilter5x5_32nw.c  \
		mlib_ImageMaxFilter7x7_32nw.c  \
		mlib_ImageMinFilter5x5_32nw.c  \
		mlib_ImageMinFilter7x7_32nw.c  \
		mlib_ImageMedianFilter3x3.c    \
		mlib_ImageMedianFilter3x3Func.c\
		mlib_ImageMedianFilter5x5.c    \
		mlib_ImageMedianFilter5x5Func.c\
		mlib_ImageMedianFilter7x7.c    \
		mlib_ImageMedianFilterMxN.c    \
		mlib_ImageSobel.c              \
		mlib_ImageConvKernelConvert.c  \
		mlib_ImageSConvKernelConvert.c

SRCS_FP_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_FP_SH = $(SRCS_FP_SH_TMP:sh)

SRCS_STDC_FP_TMP = pp="$(SRCS_FP_SH)"; for i in `echo $(SRCS_FP)`; do pp=`echo $$pp | $(SED) -e "s/\<$$i\>//g"`; done; echo $$pp
SRCS_STDC_FP = $(SRCS_STDC_FP_TMP:sh)

SRCS_FP =	mlib_ImageConvClearEdge_Fp.c \
		mlib_ImageConvCopyEdge_Fp.c  \
		mlib_ImageDilate4_F32.c      \
		mlib_ImageDilate4_D64.c      \
		mlib_ImageErode4_F32.c       \
		mlib_ImageErode4_D64.c       \
		mlib_ImageMaxFilter3x3_F32.c \
		mlib_ImageMaxFilter3x3_D64.c \
		mlib_ImageMaxFilter5x5_F32.c \
		mlib_ImageMaxFilter5x5_D64.c \
		mlib_ImageMaxFilter7x7_F32.c \
		mlib_ImageMaxFilter7x7_D64.c \
		mlib_ImageMinFilter3x3_F32.c \
		mlib_ImageMinFilter3x3_D64.c \
		mlib_ImageMinFilter5x5_F32.c \
		mlib_ImageMinFilter5x5_D64.c \
		mlib_ImageMinFilter7x7_F32.c \
		mlib_ImageMinFilter7x7_D64.c \
		mlib_ImageRankFilter3x3_Fp.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT =	mlib_i_ImageSConv_16ext.c      \
		mlib_i_ImageSConv_16nw.c       \
		mlib_i_ImageSConv_u16ext.c     \
		mlib_i_ImageSConv_u16nw.c      \
		mlib_i_ImageSConv_8ext.c       \
		mlib_i_ImageSConv_8nw.c        \
		mlib_i_ImageSobel.c


include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += $(MLIB_HOME)/src/sparc/vis/image/spatial/ \
		-I$(HOME_TXT) \
		-I$(HOME_TXT)/../include \
		-I$(HOME_TXT)/../copy \
		-I$(HOME_TXT)/../format

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h  $(MLIB_HOME)/src/sparc/vis/image/spatial/mlib_v_ImageUnroll.h

$(DIR_O)/mlib_ImageClipping$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2_f$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Bit$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv5x5$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv5x5_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv7x7$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConvClearEdge_Bit$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvClearEdge_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Bit$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvIndex_16_16ext$(OBJ_EXT)   :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_16nw$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16ext$(OBJ_EXT)    :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16nw$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8ext$(OBJ_EXT)     :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8nw$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvKernelConvert$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConvMxNIndex$(OBJ_EXT)         :  $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                     $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageConvIndex.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageClipping.h
$(DIR_O)/mlib_ImageConvMxN_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN_ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_16ext$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_16nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_32ext$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_32nw$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_8ext$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_8nw$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_u16ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv_u16nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConvolveMxN$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageDilate4$(OBJ_EXT)              :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_1nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_32nw$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_D64$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_F32$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_Fp$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageErode4$(OBJ_EXT)               :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_1nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_32nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_D64$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_F32$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_Fp$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageGradient3x3$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradient3x3_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageGradientFunc.h
$(DIR_O)/mlib_ImageGradientMxN$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradientMxN_Fp$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                     $(HOME_TXT)/mlib_ImageGradientFunc.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageMaxFilter3x3$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_1nw$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3$(OBJ_EXT)      :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3Func$(OBJ_EXT)  :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3_Fp$(OBJ_EXT)   :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5$(OBJ_EXT)      :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5Func$(OBJ_EXT)  :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5_Fp$(OBJ_EXT)   :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7$(OBJ_EXT)      :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7_Fp$(OBJ_EXT)   :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterFunc$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN$(OBJ_EXT)      :  ./mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxNFunc$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN_Fp$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter_US$(OBJ_EXT)      :  $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_1nw$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_32nw$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_D64$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_F32$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_Fp$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageRankFilter3x3_Fp$(OBJ_EXT)     :  ./mlib_ImageRankFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageMedianFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_Fp$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN_S16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_U16$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilter_US$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageSConv3x3$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv3x3_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7_Fp$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                     $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConvKernelConvert$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageSConv_32ext$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_32nw$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32ext$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32nw$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSobel$(OBJ_EXT)                :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                     $(HOME_TXT)/mlib_ImageSobel.h \
                                                     $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageSobel_Fp$(OBJ_EXT)             :  $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageSqrtTable_U16$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageSqrtTable.h
