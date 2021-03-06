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
# ident	"@(#)Makefile	9.12	07/11/06 SMI"
#

TOP_DIR = ../../../../..

include $(TOP_DIR)/make/custom/$(MLIB_CUSTOM).cfg

MODNAME  = image
ALT_MOD  = image
ALT_DIR  = $(TOP_DIR)/src/x86/mmx/$(ALT_MOD)/blend
HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/blend

# common source under src/stdc/generic
UTILS_STDC = mlib_ImageComposite.c \
             mlib_ImageBlend_DA_DA.c \
             mlib_ImageBlend_DA_DC.c \
             mlib_ImageBlend_DA_OMDA.c \
             mlib_ImageBlend_DA_OMDC.c \
             mlib_ImageBlend_DA_ONE.c \
             mlib_ImageBlend_DA_ZERO.c \
             mlib_ImageBlend_OMDA_DA.c \
             mlib_ImageBlend_OMDA_DC.c \
             mlib_ImageBlend_OMDA_OMDA.c \
             mlib_ImageBlend_OMDA_OMDC.c \
             mlib_ImageBlend_OMDA_ONE.c \
             mlib_ImageBlend_OMDA_SA.c \
             mlib_ImageBlend_OMDA_ZERO.c \
             mlib_ImageBlend_SA_DC.c \
             mlib_ImageBlend_OMSA_DA.c \
             mlib_ImageBlend_OMSA_DC.c \
             mlib_ImageBlend_OMSA_OMDC.c \
             mlib_ImageBlend_OMSA_ONE.c \
             mlib_ImageBlend_OMSA_ZERO.c \
             mlib_ImageBlend_OMSC_DC.c \
             mlib_ImageBlend_OMSC_ZERO.c \
             mlib_ImageBlend_ONE_DC.c \
             mlib_ImageBlend_ONE_ONE.c \
             mlib_ImageBlend_SA_OMDC.c \
             mlib_ImageBlend_SA_ONE.c \
             mlib_ImageBlend_SA_ZERO.c \
             mlib_ImageBlend_SC_OMDC.c \
             mlib_ImageBlend_SC_ZERO.c \
             mlib_ImageBlend_ZERO_ONE.c \
             mlib_ImageBlend_ZERO_ZERO.c \
             mlib_ImageBlend_BDST_BSRC_Inp.c

# common source for floating point under src/stdc/generic
SRCS_STDC_FP =

# C specific source under src/stdc/generic
SRCS_STDC = mlib_c_ImageBlendTable.c \

# MMX specific source under src/x86/mmx
SRCS_MMX =


# SSE2 specific source under src/x86/sse2
SRCS_SSE2 = \
            mlib_s_ImageBlend_DA_OMSA.c \
            mlib_s_ImageBlend_DA_SA.c \
            mlib_s_ImageBlend_DA_SAS.c \
            mlib_s_ImageBlend_ONE_DA.c \
            mlib_s_ImageBlend_ONE_OMDA.c \
            mlib_s_ImageBlend_ONE_OMDC.c \
            mlib_s_ImageBlend_ONE_OMSA.c \
            mlib_s_ImageBlend_ONE_SA.c \
            mlib_s_ImageBlend_ONE_SAS.c \
            mlib_s_ImageBlend_ZERO_DA.c \
            mlib_s_ImageBlend_ZERO_DC.c \
            mlib_s_ImageBlend_ZERO_OMDA.c \
            mlib_s_ImageBlend_ZERO_OMDC.c \
            mlib_s_ImageBlend_ZERO_OMSA.c \
            mlib_s_ImageBlend_ZERO_SA.c \
            mlib_s_ImageBlend_ZERO_SAS.c \
            mlib_s_ImageBlend_SC_DA.c \
            mlib_s_ImageBlend_SC_DC.c \
            mlib_s_ImageBlend_SC_OMDA.c \
            mlib_s_ImageBlend_SC_OMSA.c \
            mlib_s_ImageBlend_SC_ONE.c \
            mlib_s_ImageBlend_SC_SA.c \
            mlib_s_ImageBlend_SC_SAS.c \
            mlib_s_ImageBlend_OMSC_DA.c \
            mlib_s_ImageBlend_OMSC_OMDA.c \
            mlib_s_ImageBlend_OMSC_OMDC.c \
            mlib_s_ImageBlend_OMSC_OMSA.c \
            mlib_s_ImageBlend_OMSC_ONE.c \
            mlib_s_ImageBlend_OMSC_SA.c \
            mlib_s_ImageBlend_OMSC_SAS.c \
            mlib_s_ImageBlend_SA_DA.c \
            mlib_s_ImageBlend_SA_OMSA.c \
            mlib_s_ImageBlend_SA_OMDA.c \
            mlib_s_ImageBlend_SA_SA.c \
            mlib_s_ImageBlend_SA_SAS.c \
            mlib_s_ImageBlend_OMSA_OMDA.c \
            mlib_s_ImageBlend_OMSA_OMSA.c \
            mlib_s_ImageBlend_OMSA_SA.c \
            mlib_s_ImageBlend_OMSA_SAS.c \
            mlib_s_ImageBlend_OMDA_OMSA.c \
            mlib_s_ImageBlend_OMDA_SAS.c \
            mlib_s_ImageBlend_ONE_ZERO.c \

# MMX as alternative source
SRCS_ALT = $(SRCS_MMX)

# SSE2 as local source
SRCS = $(SRCS_SSE2)

include $(TOP_DIR)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)
INCLUDES += -I$(HOME_TXT)/../include
INCLUDES += -I$(HOME_TXT)/../blend
INCLUDES += -I$(HOME_TXT)/../copy
INCLUDES += -I$(HOME_TXT)/../spatial
INCLUDES += -I$(ALT_DIR)
INCLUDES += -I$(ALT_DIR)/../../mmx
INCLUDES += -I../../sse


#
# dependencies
#

$(OBJS) $(OBJS_STDC) : \
	$(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ModFUnc$(OBJ_EXT) : \
        $(HOME_TXT)/...
$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_c_ImageBlendTable$(OBJ_EXT)        : $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_DA_OMSA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_DA_SA$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_DA_SAS$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMDA_OMSA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMDA_SAS$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMSA_OMDA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMSA_OMSA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMSA_SA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_c_ImageBlend_OMSA_SAS$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_DA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_OMDA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_OMDC$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_OMSA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_ONE$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_SA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_OMSC_SAS$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_DA$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_OMDA$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_OMDC$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_OMSA$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_SA$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ONE_SAS$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SA_DA$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SA_OMDA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SA_OMSA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SA_SA$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SA_SAS$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_DA$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_DC$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_OMDA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_OMSA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_ONE$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_SA$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_SC_SAS$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_DA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_DC$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_OMDA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_OMDC$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_OMSA$(OBJ_EXT)   : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_SA$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
$(DIR_O)/mlib_s_ImageBlend_ZERO_SAS$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                   $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                   $(HOME_TXT)/mlib_c_ImageBlendTable.h
