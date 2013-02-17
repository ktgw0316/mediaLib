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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/copy

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g'
UTILS_STDC = $(UTILS_SH:sh)

SRCS_FPSH=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *SRCS_FP *=" | $(SED) -e 's/^.*=//g'
SRCS_STDC_FP = $(SRCS_FPSH:sh)

SRCS_SV = mlib_v_ImageCopy_blk.S

SRCS_V  = mlib_v_ImageClear.c \
          mlib_v_ImageClearEdge.c \
          mlib_v_ImageClearEdge_f.c \
          mlib_v_ImageClear_f.c \
          mlib_v_ImageCopy.c \
          mlib_v_ImageCopyArea.c \
          mlib_v_ImageCopyArea_f.c \
          mlib_v_ImageCopyMask.c \
          mlib_v_ImageCopyMask_f.c \
          mlib_v_ImageCopySubimage.c \
          mlib_v_ImageCopy_f.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES +=  -I$(HOME_TXT) -I$(HOME_TXT)/../include

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_v_ImageClear$(OBJ_EXT)        :  ./mlib_v_ImageClear_f.h
$(DIR_O)/mlib_v_ImageClearEdge$(OBJ_EXT)    :  ./mlib_v_ImageClearEdge_f.h
$(DIR_O)/mlib_v_ImageClearEdge_f$(OBJ_EXT)  :  ./mlib_v_ImageClearEdge_f.h
$(DIR_O)/mlib_v_ImageClear_f$(OBJ_EXT)      :  ./mlib_v_ImageClear_f.h
$(DIR_O)/mlib_v_ImageCopy$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageCopy.h ./mlib_v_ImageCopy_f.h
$(DIR_O)/mlib_v_ImageCopyArea$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageCopy.h \
                                               ./mlib_v_ImageCopyArea_f.h \
                                               ./mlib_v_ImageCopy_f.h
$(DIR_O)/mlib_v_ImageCopyArea_f$(OBJ_EXT)   :  ./mlib_v_ImageCopyArea_f.h
$(DIR_O)/mlib_v_ImageCopyMask$(OBJ_EXT)     :  ./mlib_v_ImageCopyMask_f.h
$(DIR_O)/mlib_v_ImageCopyMask_f$(OBJ_EXT)   :  ./mlib_v_ImageCopyMask_f.h
$(DIR_O)/mlib_v_ImageCopySubimage$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageCopy.h ./mlib_v_ImageCopy_f.h
$(DIR_O)/mlib_v_ImageCopy_f$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageCopy.h ./mlib_v_ImageCopy_f.h
