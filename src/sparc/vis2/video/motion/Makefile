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
MODNAME = video

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/motion
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/motion

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" "SRCS_S=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_SV =

SRCS_V = mlib_v_VideoAddBlock.c                 \
         mlib_v_VideoSumAbsDiff.c               \
         mlib_v_VideoH263OvrlapMC_U8_U8.c       \
         mlib_v_VideoH263OvrlapMC_S16_U8.c      \
         mlib_v_VideoP64Decimate.c              \
         mlib_v_VideoP64Loop_U8_U8.c            \
         mlib_v_VideoP64Loop_S16_U8.c           \
         mlib_v_VideoInterpX_U8_U8.c            \
         mlib_v_VideoInterpY_U8_U8.c            \
         mlib_v_VideoInterpXY_U8_U8.c           \
         mlib_v_VideoInterpX_Y_XY_U8_U8.c       \
         mlib_v_VideoInterpX_S16_U8.c           \
         mlib_v_VideoInterpY_S16_U8.c           \
         mlib_v_VideoInterpXY_S16_U8.c          \
         mlib_v_VideoInterpAveX_U8_U8.c         \
         mlib_v_VideoInterpAveY_U8_U8.c         \
         mlib_v_VideoInterpAveXY_U8_U8.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

# define depends

