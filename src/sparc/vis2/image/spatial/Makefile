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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/spatial
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/spatial

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in $(UTILS_RM); do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS_RM = mlib_ImageConv2x2_f.c           \
           mlib_ImageConv_8nw.c            \
           mlib_ImageConv_8ext.c           \
           mlib_ImageConv_16nw.c           \
           mlib_ImageConv_16ext.c          \
           mlib_ImageConv_u16ext.c         \
           mlib_ImageConv_u16nw.c

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" "SRCS_S=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_SV     = mlib_v_ImageLineMax.S \
              mlib_v_ImageLineMin.S

SRCS_V = mlib_v_ImageConv2x2_8nw.c     \
         mlib_v_ImageConv3x3_8nw.c     \
         mlib_v_ImageConv4x4_8nw.c     \
         mlib_v_ImageConv5x5_8nw.c     \
         mlib_v_ImageConv7x7_8nw.c     \
         mlib_v_ImageConvMxN_8.c       \
         mlib_v_ImageConv_8nw.c        \
         mlib_v_ImageConv2x2_f.c       \
         mlib_v_ImageConv_8nw_f.c      \
         mlib_v_ImageConv_8ext.c       \
         mlib_v_ImageConv_16nw.c       \
         mlib_v_ImageConv_16ext.c      \
         mlib_v_ImageConv_u16ext.c     \
         mlib_v_ImageConv_u16nw.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_TXT)/../copy -I$(HOME_TXT)/../format \
            -I$(HOME_VIS_TXT) -I$(HOME_VIS_TXT)/../format \
            -I./../format

#
# define depends
#


$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h  ${HOME_VIS_TXT}/mlib_v_ImageUnroll.h

$(DIR_O)/mlib_ImageClipping$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv2x2_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv3x3$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                    $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Bit$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv3x3_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv4x4_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv5x5$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                    $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConv5x5_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConv7x7$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageConvClearEdge_Bit$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvClearEdge_Fp$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Bit$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvCopyEdge_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageConvIndex_16_16ext$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_16nw$(OBJ_EXT)    : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8ext$(OBJ_EXT)    : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_16_8nw$(OBJ_EXT)     : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16ext$(OBJ_EXT)    : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_16nw$(OBJ_EXT)     : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8ext$(OBJ_EXT)     : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvIndex_8_8nw$(OBJ_EXT)      : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConvIndex.h
$(DIR_O)/mlib_ImageConvKernelConvert$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConv.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h  \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_ImageConvMxNIndex$(OBJ_EXT)         : $(HOME_TXT)/../include/mlib_ImageColormap.h \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/mlib_ImageConvIndex.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                    $(HOME_TXT)/mlib_ImageClipping.h
$(DIR_O)/mlib_ImageConvMxN_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvMxN_ext$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_32ext$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_32nw$(OBJ_EXT)            : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64ext$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_D64nw$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32ext$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConv_F32nw$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageConv.h
$(DIR_O)/mlib_ImageConvolveMxN$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageDilate4$(OBJ_EXT)              : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_1nw$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_32nw$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_D64$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_F32$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageDilate4_Fp$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageDilate4.h
$(DIR_O)/mlib_ImageErode4$(OBJ_EXT)               : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_1nw$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_32nw$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_D64$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_F32$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageErode4_Fp$(OBJ_EXT)            : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageErode4.h
$(DIR_O)/mlib_ImageGradient3x3$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h  \
                                                    $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageGradientFunc.h  \
                                                    $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradient3x3_Fp$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageGradientFunc.h
$(DIR_O)/mlib_ImageGradientMxN$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h  \
                                                    $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageGradientFunc.h  \
                                                    $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageGradientMxN_Fp$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h  \
                                                    $(HOME_TXT)/../include/mlib_ImageDivTables.h $(HOME_TXT)/mlib_ImageGradientFunc.h  \
                                                    $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageMaxFilter3x3$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_1nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter3x3_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter5x5_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMaxFilter7x7_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMaxFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3Func$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter3x3_Fp$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5Func$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter5x5_Fp$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter7x7_Fp$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterFunc$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/../include/mlib_ImageCreate.h \
                                                    $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxNFunc$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilterMxN_Fp$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMedianFilter_US$(OBJ_EXT)      : $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_1nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter3x3_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter5x5_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_32nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_D64$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_F32$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageMinFilter7x7_Fp$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageMinFilter.h
$(DIR_O)/mlib_ImageRankFilter3x3_Fp$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageRankFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageMedianFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_Fp$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageMedianFilter.h
$(DIR_O)/mlib_ImageRankFilterMxN_S16$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilterMxN_U16$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageRankFilter.h $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageRankFilter_US$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageRankFilterFunc.h
$(DIR_O)/mlib_ImageSConv3x3$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv3x3_Fp$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv5x5_Fp$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv7x7_Fp$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/../include/mlib_ImageCreate.h $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConvKernelConvert$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageVisVersion.h
$(DIR_O)/mlib_ImageSConv_32ext$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_32nw$(OBJ_EXT)           : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64ext$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_D64nw$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32ext$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSConv_F32nw$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_ImageSobel$(OBJ_EXT)                : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h \
                                                    $(HOME_TXT)/mlib_ImageSobel.h $(HOME_TXT)/mlib_ImageSqrtTable.h
$(DIR_O)/mlib_ImageSobel_Fp$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageClipping.h $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_ImageSqrtTable_U16$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageSqrtTable.h

#
# VIS
#

$(DIR_O)/mlib_v_ImageConv2x2Index$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConv2x2_16ext$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv2x2_16nw$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv2x2_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv2x2_u16ext$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv2x2_u16nw$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv3x3Index$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConv3x3_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv4x4Index$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConv4x4_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv5x5Index$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConv5x5_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv7x7Index$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConv7x7_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/../copy/mlib_ImageCopy.h  \
                                                    $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h
$(DIR_O)/mlib_v_ImageConvClearEdge$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_v_ImageConvCopyEdge$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConvEdge.h
$(DIR_O)/mlib_v_ImageConvIndex3_8_16ext$(OBJ_EXT) : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex3_8_16nw$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex3_8_8ext$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex3_8_8nw$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex4_8_16ext$(OBJ_EXT) : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex4_8_16nw$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex4_8_8ext$(OBJ_EXT)  : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvIndex4_8_8nw$(OBJ_EXT)   : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvMxNIndex$(OBJ_EXT)       : $(HOME_TXT)/../include/mlib_ImageColormap.h $(HOME_TXT)/mlib_ImageConv.h  \
                                                    $(HOME_TXT)/mlib_ImageConvEdge.h $(HOME_TXT)/mlib_ImageConvIndex.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageConvIndex.h
$(DIR_O)/mlib_v_ImageConvMxN_8ext$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/../copy/mlib_ImageCopy.h  \
                                                    $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h
$(DIR_O)/mlib_v_ImageDilateErode$(OBJ_EXT)        : $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h
$(DIR_O)/mlib_v_ImageGradient3x3Func$(OBJ_EXT)    : $(HOME_TXT)/../format/mlib_ImageChannelSplit.h $(HOME_TXT)/../copy/mlib_ImageCopy.h  \
                                                    $(HOME_TXT)/mlib_ImageGradientFunc.h $(HOME_TXT)/mlib_ImageSqrtTable.h  \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageGradientMxNFunc$(OBJ_EXT)    : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageGradientFunc.h  \
                                                    $(HOME_TXT)/mlib_ImageSqrtTable.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h
$(DIR_O)/mlib_v_ImageMedianFilter3x3$(OBJ_EXT)    : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageMedianFilter.h  \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h $(HOME_VIS_TXT)/mlib_v_ImageMedianFilter.h
$(DIR_O)/mlib_v_ImageMedianFilter5x5$(OBJ_EXT)    : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageMedianFilter.h  \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h $(HOME_VIS_TXT)/mlib_v_ImageMedianFilter.h
$(DIR_O)/mlib_v_ImageMedianFilterFunc$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageMedianFilter.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageMedianFilter.h
$(DIR_O)/mlib_v_ImageMinMaxFilter$(OBJ_EXT)       : $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h
$(DIR_O)/mlib_v_ImageRankFilter3x3$(OBJ_EXT)      : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageRankFilter.h  \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageDataTypeConvert.h \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageDilateErode.h  \
                                                    $(HOME_VIS_TXT)/mlib_v_ImageLineMaxMin.h
$(DIR_O)/mlib_v_ImageSConv3x3_16ext$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_16nw$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_32$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_8ext$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_8nw$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_u16ext$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv3x3_u16nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_16ext$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_16nw$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_32$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_8ext$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_8nw$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_u16ext$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv5x5_u16nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_16ext$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_16nw$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_32$(OBJ_EXT)        : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_8ext$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_8nw$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_u16ext$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSConv7x7_u16nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageSConv.h
$(DIR_O)/mlib_v_ImageSobel_U8$(OBJ_EXT)           : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_TXT)/mlib_ImageSobel.h  \
                                                    $(HOME_TXT)/mlib_ImageSqrtTable.h \
                                                    $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h

#
# VIS2
#

$(DIR_O)/mlib_v_ImageConv2x2_f$(OBJ_EXT)     : $(HOME_TXT)/mlib_c_ImageConv.h
$(DIR_O)/mlib_v_ImageConv_16ext$(OBJ_EXT)    : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_ext.h
$(DIR_O)/mlib_v_ImageConv_16nw$(OBJ_EXT)     : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_nw.h
$(DIR_O)/mlib_v_ImageConv_8ext$(OBJ_EXT)     : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_ext.h
$(DIR_O)/mlib_v_ImageConv_8nw_f$(OBJ_EXT)    : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_nw.h
$(DIR_O)/mlib_v_ImageConv_u16ext$(OBJ_EXT)   : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_ext.h
$(DIR_O)/mlib_v_ImageConv_u16nw$(OBJ_EXT)    : $(HOME_TXT)/mlib_c_ImageConv.h ./mlib_v_ImageConv_nw.h

$(DIR_O)/mlib_v_ImageConv2x2_8nw$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConv3x3_8nw$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConv4x4_8nw$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConv5x5_8nw$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConv7x7_8nw$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConvMxN_8$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageConv.h $(HOME_TXT)/mlib_c_ImageConv.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
$(DIR_O)/mlib_v_ImageConv_8nw$(OBJ_EXT)    : $(HOME_TXT)/../copy/mlib_ImageCopy.h $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelExtract.h  \
                                             $(HOME_VIS_TXT)/../format/mlib_v_ImageChannelInsert.h  \
                                             ./../format/mlib_v_ImageChannelExtract_2.h \
                                             ./../format/mlib_v_ImageChannelInsert_2.h $(HOME_VIS_TXT)/mlib_v_ImageConv.h
