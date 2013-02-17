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
MODNAME = video

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

SRCS_C  = mlib_c_VideoAddBlock.c \
          mlib_c_VideoCopyRefAve_U8_U8.c \
          mlib_c_VideoCopyRef_S16_U8.c \
          mlib_c_VideoCopyRef_U8_U8.c \
          mlib_c_VideoH263OvrlapMC_S16_U8.c \
          mlib_c_VideoH263OvrlapMC_U8_U8.c \
          mlib_c_VideoInterpAveXY_U8_U8.c \
          mlib_c_VideoInterpAveX_U8_U8.c \
          mlib_c_VideoInterpAveY_U8_U8.c \
          mlib_c_VideoInterpXY_S16_U8.c \
          mlib_c_VideoInterpXY_U8_U8.c \
          mlib_c_VideoInterpX_S16_U8.c \
          mlib_c_VideoInterpX_U8_U8.c \
          mlib_c_VideoInterpX_Y_XY_U8_U8.c \
          mlib_c_VideoInterpY_S16_U8.c \
          mlib_c_VideoInterpY_U8_U8.c \
          mlib_c_VideoP64Decimate.c \
          mlib_c_VideoP64Loop_S16_U8.c \
          mlib_c_VideoP64Loop_U8_U8.c \
          mlib_c_VideoSumAbsDiff.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
__DEP_DIR_B = ./../../include

$(DIR_O)/mlib_c_VideoCopyRef_U8_U8$(OBJ_EXT)    :  $(__DEP_DIR_B)/mlib_Utils.h

