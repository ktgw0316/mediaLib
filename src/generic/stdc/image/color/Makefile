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
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

SRCS_FP = mlib_ImageColorConvert_Fp.c \
          mlib_ImageColorHSL2RGB_Fp.c \
          mlib_ImageColorHSV2RGB_Fp.c \
          mlib_ImageColorRGB2HSL_Fp.c \
          mlib_ImageColorRGB2HSV_Fp.c \
          mlib_ImageColorRGB2Mono_Fp.c

SRCS_C  = mlib_c_ImageColorConvert.c \
          mlib_c_ImageColorHSL2RGB.c \
          mlib_c_ImageColorHSV2RGB.c \
          mlib_c_ImageColorRGB2HSL.c \
          mlib_c_ImageColorRGB2HSV.c \
          mlib_c_ImageColorRGB2Mono.c \
          mlib_c_ImageColorRGB2XYZ.c \
          mlib_c_ImageColorRGB2YCC.c \
          mlib_c_ImageColorTables.c \
          mlib_c_ImageColorXYZ2RGB.c \
          mlib_c_ImageColorYCC2RGB.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../include

#
# define depends
#

$(DIR_O)/mlib_c_ImageColorConvert$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorHSL2RGB$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorHSV2RGB$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorRGB2HSL$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorRGB2HSV$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h
$(DIR_O)/mlib_c_ImageColorTables$(OBJ_EXT)    :  ./mlib_c_ImageColorTables.h
$(DIR_O)/mlib_c_ImageColorXYZ2RGB$(OBJ_EXT)   :  ./mlib_c_ImageColorTables.h
$(DIR_O)/mlib_c_ImageColorYCC2RGB$(OBJ_EXT)   :  ./mlib_c_ImageColorTables.h
