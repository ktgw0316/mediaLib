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
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.2	07/10/09 SMI"
#

TOP_DIR = ../../../../..
MODNAME = image

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  =  mlib_ImageExtrema2.c \
          mlib_ImageExtremaLocations.c \
          mlib_ImageMaximumFuncS32.c \
          mlib_ImageMinimumFuncS32.c

SRCS_FP = mlib_ImageAutoCorrel_Fp.c \
          mlib_ImageCrossCorrel_Fp.c \
          mlib_ImageNormCrossCorrel_Fp.c \
          mlib_ImageExtrema2_Fp.c \
          mlib_ImageExtremaLocations_Fp.c \
          mlib_ImageMaximum_Fp.c \
          mlib_ImageMinimum_Fp.c \
          mlib_ImageMean_Fp.c \
          mlib_ImageMoment2_Fp.c \
          mlib_ImageStdDev_Fp.c \
          mlib_ImageXProj_Fp.c \
          mlib_ImageYProj_Fp.c

SRCS_C  = mlib_c_ImageAutoCorrel.c \
          mlib_c_ImageCrossCorrel.c \
          mlib_c_ImageNormCrossCorrel.c \
          mlib_c_ImageExtrema2.c \
          mlib_c_ImageMaximum.c \
          mlib_c_ImageMinimum.c \
          mlib_c_ImageMean.c \
          mlib_c_ImageMoment2.c \
          mlib_c_ImageStdDev.c \
          mlib_c_ImageXProj.c \
          mlib_c_ImageYProj.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I./../include

#
# define depends
#

$(DIR_O)/mlib_ImageAutoCorrel_Fp$(OBJ_EXT)       :  ./mlib_ImageCorrel.h ./../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageCrossCorrel_Fp$(OBJ_EXT)      :  ./mlib_ImageCorrel.h
$(DIR_O)/mlib_ImageNormCrossCorrel_Fp$(OBJ_EXT)  :  ./mlib_ImageCorrel.h
$(DIR_O)/mlib_ImageExtrema2$(OBJ_EXT)            :  ./../include/mlib_ImageCreate.h ./mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageExtrema2_Fp$(OBJ_EXT)         :  ./../include/mlib_ImageCreate.h
$(DIR_O)/mlib_ImageExtremaLocations$(OBJ_EXT)    :  ./mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageExtremaLocations_Fp$(OBJ_EXT) :  ./mlib_ImageExtremaLocations.h
$(DIR_O)/mlib_ImageMaximumFuncS32$(OBJ_EXT)      :  ./mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageMinimumFuncS32$(OBJ_EXT)      :  ./mlib_ImageExtrema.h
$(DIR_O)/mlib_ImageMoment2_Fp$(OBJ_EXT)          :  ./mlib_ImageMoment2.h
$(DIR_O)/mlib_ImageStdDev_Fp$(OBJ_EXT)           :  ./mlib_ImageMoment2.h

$(DIR_O)/mlib_c_ImageAutoCorrel$(OBJ_EXT)        :  ./../include/mlib_ImageCreate.h ./mlib_c_ImageCorrel.h
$(DIR_O)/mlib_c_ImageCrossCorrel$(OBJ_EXT)       :  ./../include/mlib_ImageDivTables.h ./mlib_c_ImageCorrel.h
$(DIR_O)/mlib_c_ImageNormCrossCorrel$(OBJ_EXT)   :  ./../include/mlib_ImageDivTables.h ./mlib_c_ImageCorrel.h
$(DIR_O)/mlib_c_ImageExtrema2$(OBJ_EXT)          :  ./mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMaximum$(OBJ_EXT)           :  ./mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMinimum$(OBJ_EXT)           :  ./mlib_ImageExtrema.h
$(DIR_O)/mlib_c_ImageMoment2$(OBJ_EXT)           :  ./../include/mlib_ImageDivTables.h ./mlib_c_ImageMoment2.h
$(DIR_O)/mlib_c_ImageStdDev$(OBJ_EXT)            :  ./../include/mlib_ImageDivTables.h ./mlib_c_ImageMoment2.h
