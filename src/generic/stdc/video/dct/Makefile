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
# ident	"@(#)Makefile	9.6	07/10/09 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

UTILS  = mlib_VideoDCT2x2_S16_S16.c \
         mlib_VideoIDCT_IEEE.c \
         mlib_VideoIDCT8x8_DC.c

SRCS_C  = mlib_c_VideoDCT16x16_S16_S16.c \
          mlib_c_VideoDCT16x16_S16_S16_B10.c \
          mlib_c_VideoDCT4x4_S16_S16.c \
          mlib_c_VideoDCT8x8.c \
          mlib_c_VideoDCT8x8_S16_S16_B12.c \
          mlib_c_VideoIDCT8x8.c \
          mlib_c_VideoIDCT8x8_Q1.c \
          mlib_c_VideoIDCT8x8_S16_S16_QM.c \
	  mlib_c_VideoDCTQuantize.c   \
          mlib_c_VideoQuantize_S16.c	\
	  mlib_c_VideoDeQuantizeIDCT.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends
