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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/color

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_V  = mlib_v_ImageColorConvert.c \
          mlib_v_ImageColorConvertFunc.c \
          mlib_v_ImageColorConvertFuncU8.c \
          mlib_v_ImageColorHSL2RGB.c \
          mlib_v_ImageColorHSV2RGB.c \
          mlib_v_ImageColorRGB2HSL.c \
          mlib_v_ImageColorRGB2HSV.c \
          mlib_v_ImageColorRGB2Mono.c \
          mlib_v_ImageColorRGB2MonoFunc.c\
          mlib_v_ImageColorRGB2MonoFunc16.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/../include

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_v_ImageColorConvert$(OBJ_EXT)        :  ./mlib_v_ImageColorConvertFunc.h
$(DIR_O)/mlib_v_ImageColorConvertFunc$(OBJ_EXT)    :  ./mlib_v_ImageColorConvertFunc.h
$(DIR_O)/mlib_v_ImageColorConvertFuncU8$(OBJ_EXT)  :  $(HOME_TXT)/../include/mlib_ImageDivTables.h \
                                                      ./mlib_v_ImageColorConvertFunc.h
$(DIR_O)/mlib_v_ImageColorHSL2RGB$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageColorHSV2RGB$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageColorRGB2HSL$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageColorRGB2HSV$(OBJ_EXT)        :  $(HOME_TXT)/../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_v_ImageColorRGB2Mono$(OBJ_EXT)       :  ./mlib_v_ImageColorRGB2MonoFunc.h
$(DIR_O)/mlib_v_ImageColorRGB2MonoFunc$(OBJ_EXT)   :  ./mlib_v_ImageColorRGB2MonoFunc.h
$(DIR_O)/mlib_v_ImageColorRGB2MonoFunc16$(OBJ_EXT) :  ./mlib_v_ImageColorRGB2MonoFunc.h
