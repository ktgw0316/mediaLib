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
# ident	"@(#)Makefile	9.2	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/dct

UTILS_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_SH = $(UTILS_SH_TMP:sh)

UTILS_STDC_TMP = aa="$(UTILS_SH)"; for i in `echo $(UTILS)`; do aa=`echo $$aa | $(SED) -e "s/\<$$i\>//g"`; done; echo $$aa
UTILS_STDC = $(UTILS_STDC_TMP:sh)

UTILS = mlib_VideoIDCT_IEEE.c

SRCS_STDC_SH_TMP=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS=" com | $(EGREP) "^ *SRCS_C *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_SH = $(SRCS_STDC_SH_TMP:sh)

SRCS_STDC_TMP = qq="$(SRCS_STDC_SH)"; for i in `echo $(SRCS_INT) | $(SED) -e 's/mlib_._/mlib_c_/g'`; do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_STDC = $(SRCS_STDC_TMP:sh)

SRCS_INT  =	mlib_i_VideoDCT8x8.c			\
		mlib_i_VideoDCT4x4_S16_S16.c		\
		mlib_i_VideoDCT16x16_S16_S16.c		\
		mlib_i_VideoDCT16x16_S16_S16_B10.c	\
		mlib_i_VideoDCT8x8_S16_S16_B12.c	\
		mlib_i_VideoIDCT8x8.c			\
		mlib_i_VideoIDCT8x8_Q1.c		\
		mlib_i_VideoIDCT8x8_S16_S16_QM.c	\
		mlib_i_VideoQuantize_S16.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) \
		-I$(HOME_TXT)/../include

# define depends
