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
# ident	"@(#)Makefile	9.6	07/11/05 SMI"
#

TOP_DIR = ../../../../..
MODNAME = video

include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/dct

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)\

SRCS_V  = mlib_v_VideoDCT16x16_S16_S16.c \
          mlib_v_VideoDCT16x16_S16_S16_B10.c \
          mlib_v_VideoDCT4x4_S16_S16.c \
          mlib_v_VideoDCT8x8_S16_S16.c \
          mlib_v_VideoDCT8x8_S16_S16_B12.c \
          mlib_v_VideoDCT8x8_S16_S16_NA.c \
          mlib_v_VideoDCT8x8_S16_U8.c \
          mlib_v_VideoDCT8x8_S16_U8_NA.c \
          mlib_v_VideoDCTTables.c \
          mlib_v_VideoIDCT8x8_S16_S16.c \
          mlib_v_VideoIDCT8x8_S16_S16_NA.c \
          mlib_v_VideoIDCT8x8_S16_S16_Q1.c \
          mlib_v_VideoIDCT8x8_U8_S16.c \
          mlib_v_VideoIDCT8x8_U8_S16_NA.c \
          mlib_v_VideoIDCT8x8_U8_S16_Q1.c \
          mlib_v_VideoQuantize_S16.c \
	  mlib_v_VideoDCTQuantize.c	\
	  mlib_v_VideoDeQuantizeIDCT.c	\
          mlib_v_VideoIDCT8x8_S16_S16_QM.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

$(DIR_O)/mlib_v_VideoIDCT8x8_S16_S16$(OBJ_EXT) := CFLAGS += mlib_v_VideoDCTHybrid.il
$(DIR_O)/mlib_v_VideoDeQuantizeIDCT$(OBJ_EXT) := CFLAGS += mlib_v_VideoDCTHybrid.il

# define depends
$(DIR_O)/mlib_v_VideoDCT8x8_S16_S16$(OBJ_EXT)              :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoDeQuantizeIDCT$(OBJ_EXT)              :  ./mlib_v_VideoDCT.h ./mlib_v_VideoDCTHybrid.h
$(DIR_O)/mlib_v_VideoDCT8x8_S16_S16_B12$(OBJ_EXT)          :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoDCT8x8_S16_S16_NA$(OBJ_EXT)           :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoDCT8x8_S16_U8$(OBJ_EXT)               :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoDCT8x8_S16_U8_NA$(OBJ_EXT)            :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoDCTTables$(OBJ_EXT)                   :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_S16_S16$(OBJ_EXT)             :  ./mlib_v_VideoDCT.h ./mlib_v_VideoDCTHybrid.h  ./mlib_v_VideoDCTHybrid.il
$(DIR_O)/mlib_v_VideoDeQuantizeIDCT$(OBJ_EXT)             :  ./mlib_v_VideoDCT.h ./mlib_v_VideoDCTHybrid.h  ./mlib_v_VideoDCTHybrid.il
$(DIR_O)/mlib_v_VideoIDCT8x8_S16_S16_NA$(OBJ_EXT)          :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_S16_S16_Q1$(OBJ_EXT)          :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_S16_S16_QM$(OBJ_EXT)          :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_U8_S16$(OBJ_EXT)              :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_U8_S16_NA$(OBJ_EXT)           :  ./mlib_v_VideoDCT.h
$(DIR_O)/mlib_v_VideoIDCT8x8_U8_S16_Q1$(OBJ_EXT)           :  ./mlib_v_VideoDCT.h

