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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/blend
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/blend

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n SRCS="" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_V = mlib_v_ImageBlend_ZERO_SAS.c  \
         mlib_v_ImageBlend_ONE_SAS.c   \
         mlib_v_ImageBlend_ONE_OMSA.c  \
         mlib_v_ImageBlend_ONE_OMDA.c  \
         mlib_v_ImageBlend_OMSC_OMDC.c \
         mlib_v_ImageBlend_OMSA_SA.c   \
         mlib_v_ImageBlend_OMSC_SAS.c  \
         mlib_v_ImageBlend_SA_OMSA.c   \
         mlib_v_ImageBlend_OMSA_OMSA.c \
         mlib_v_ImageBlend_SA_SA.c     \
         mlib_v_ImageBlend_OMSC_SA.c   \
         mlib_v_ImageBlend_OMSC_OMDA.c \
         mlib_v_ImageBlend_OMSC_DA.c   \
         mlib_v_ImageBlend_OMSA_SAS.c  \
         mlib_v_ImageBlend_SA_SAS.c    \
         mlib_v_ImageBlend_OMDA_SAS.c  \
         mlib_v_ImageBlend_DA_SAS.c    \
         mlib_v_ImageBlend_SA_OMDA.c   \
         mlib_v_ImageBlend_SC_SAS.c    \
         mlib_v_ImageBlend_DA_SA.c     \
         mlib_v_ImageBlend_SA_DA.c     \
         mlib_v_ImageBlend_OMDA_OMSA.c \
         mlib_v_ImageBlend_SC_OMDA.c   \
         mlib_v_ImageBlend_SC_DA.c     \
         mlib_v_ImageBlend_SC_SA.c     \
         mlib_v_ImageBlend_DA_OMSA.c   \
         mlib_v_ImageBlend_OMSC_OMSA.c \
         mlib_v_ImageBlend_SC_OMSA.c   \
         mlib_v_ImageBlend_OMSA_OMDA.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(HOME_TXT)/../copy -I$(HOME_TXT)/../include -I$(HOME_TXT) -I$(HOME_VIS_TXT)

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h


#
# VIS
#

$(DIR_O)/mlib_v_ImageBlendTable$(OBJ_EXT)      :  $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_ONE$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlend_ONE_DA$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ONE_OMDC$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlend_ONE_SA$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_ONE$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_DA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_OMDA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_OMDC$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_OMSA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_SA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h

#
# VIS2
#
$(DIR_O)/mlib_v_ImageBlend_DA_OMSA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_DA_SA$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_DA_SAS$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMDA_OMSA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMDA_SAS$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSA_OMDA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSA_OMSA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSA_SA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSA_SAS$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_DA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_OMDA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_OMDC$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_OMSA$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_SA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_OMSC_SAS$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ONE_OMDA$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ONE_OMSA$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ONE_SAS$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SA_DA$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SA_OMDA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SA_OMSA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SA_SA$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SA_SAS$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_DA$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_OMDA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_OMSA$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_SA$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_SC_SAS$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
$(DIR_O)/mlib_v_ImageBlend_ZERO_SAS$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageBlendUtil.h $(HOME_TXT)/../copy/mlib_ImageCopy.h \
                                                  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                  $(HOME_VIS_TXT)/mlib_v_ImageBlendTable.h
