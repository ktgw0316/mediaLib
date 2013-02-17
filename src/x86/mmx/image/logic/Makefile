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

HOME_TXT = $(TOP_DIR)/src/generic/stdc/$(MODNAME)/logic

UTILS_STDC = mlib_ImageConstAnd_Bit.c       \
             mlib_ImageConstAndNot_Bit.c    \
             mlib_ImageConstNotAnd_Bit.c    \
             mlib_ImageConstNotOr_Bit.c     \
             mlib_ImageConstNotXor_Bit.c    \
             mlib_ImageConstOr_Bit.c        \
             mlib_ImageConstOrNot_Bit.c     \
             mlib_ImageConstXor_Bit.c       \
             mlib_ImageAnd_Bit.c            \
             mlib_ImageAndNot_Bit.c         \
             mlib_ImageNotAnd_Bit.c         \
             mlib_ImageNotOr_Bit.c          \
             mlib_ImageNotXor_Bit.c         \
             mlib_ImageOr_Bit.c             \
             mlib_ImageOrNot_Bit.c          \
             mlib_ImageXor_Bit.c            \
             mlib_ImageNot_Bit.c            \
             mlib_ImageConstNotAnd_Inp.c    \
             mlib_ImageConstNotOr_Inp.c     \
             mlib_ImageConstNotXor_Inp.c    \
             mlib_ImageConstOrNot_Inp.c     \
             mlib_ImageConstOr_Inp.c        \
             mlib_ImageConstXor_Inp.c       \
             mlib_ImageNotAnd_Inp.c         \
             mlib_ImageNotOr_Inp.c          \
             mlib_ImageNotXor_Inp.c         \
             mlib_ImageNot_Inp.c            \
             mlib_ImageOrNot_Inp.c          \
             mlib_ImageAnd_Inp.c            \
             mlib_ImageAndNot_Inp.c         \
             mlib_ImageConstAnd_Inp.c       \
             mlib_ImageConstAndNot_Inp.c    \
             mlib_ImageOr_Inp.c             \
             mlib_ImageXor_Inp.c

win32_SRCS = mlib_m_ImageAnd.c                    \
             mlib_m_ImageNot.c                    \
             mlib_m_ImageOr.c                     \
             mlib_m_ImageXor.c                    \
                                                  \
             mlib_m_ImageAndNot.c                 \
             mlib_m_ImageOrNot.c                  \
             mlib_m_ImageNotAnd.c                 \
             mlib_m_ImageNotOr.c                  \
             mlib_m_ImageNotXor.c                 \
                                                  \
             mlib_m_ImageConstAnd.c               \
             mlib_m_ImageConstAndNot.c            \
             mlib_m_ImageConstNotAnd.c            \
             mlib_m_ImageConstNotOr.c             \
             mlib_m_ImageConstNotXor.c            \
             mlib_m_ImageConstOr.c                \
             mlib_m_ImageConstOrNot.c             \
             mlib_m_ImageConstXor.c

SRCS = $($(TARCH)_SRCS)

HOME_INCLUDE = $($(TARCH)_HOME_INCLUDE)
ARCH_DIR_O   = $($(TARCH)_ARCH_DIR_O)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT) -I$(HOME_TXT)/../include -I$(HOME_INCLUDE)/../../mmx

#
# define depends
#

$(OBJS) $(OBJS_STDC) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(ARCH_DIR_O)/mlib_m_ImageAnd$(OBJ_EXT)         :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageAndNot$(OBJ_EXT)      :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstAnd$(OBJ_EXT)    :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstAndNot$(OBJ_EXT) :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstNotAnd$(OBJ_EXT) :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstNotOr$(OBJ_EXT)  :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstNotXor$(OBJ_EXT) :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstOr$(OBJ_EXT)     :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstOrNot$(OBJ_EXT)  :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageConstXor$(OBJ_EXT)    :  $(HOME_INCLUDE)/mlib_m_ImageConstLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageNot$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageNotAnd$(OBJ_EXT)      :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageNotOr$(OBJ_EXT)       :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageNotXor$(OBJ_EXT)      :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageOr$(OBJ_EXT)          :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageOrNot$(OBJ_EXT)       :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h
$(ARCH_DIR_O)/mlib_m_ImageXor$(OBJ_EXT)         :  $(HOME_INCLUDE)/mlib_m_ImageLogic.h $(HOME_INCLUDE)/../../mmx/mlib_mmx_utils.h

$(DIR_O_STDC)/mlib_ImageAndNot_Bit$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageAnd_Bit$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstAndNot_Bit$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstAnd_Bit$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstNotAnd_Bit$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstNotOr_Bit$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstNotXor_Bit$(OBJ_EXT) :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstOrNot_Bit$(OBJ_EXT)  :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstOr_Bit$(OBJ_EXT)     :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageConstXor_Bit$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageConstLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageNotAnd_Bit$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageNotOr_Bit$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageNotXor_Bit$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageNot_Bit$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageOrNot_Bit$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageOr_Bit$(OBJ_EXT)          :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_ImageXor_Bit$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_Bit.h $(HOME_TXT)/mlib_ImageLogic_proto.h

$(DIR_O_STDC)/mlib_c_ImageNotAnd$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_INCLUDE)/mlib_m_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageNotOr$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_INCLUDE)/mlib_m_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageNotXor$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_INCLUDE)/mlib_m_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageOrNot$(OBJ_EXT)         :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_INCLUDE)/mlib_m_ImageLogic.h

$(DIR_O_STDC)/mlib_c_ImageAnd$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageAndNot$(OBJ_EXT)        :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstAnd$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstAndNot$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstNotAnd$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstNotOr$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstNotXor$(OBJ_EXT)   :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstOr$(OBJ_EXT)       :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstOrNot$(OBJ_EXT)    :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageConstXor$(OBJ_EXT)      :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageConstLogic.h
$(DIR_O_STDC)/mlib_c_ImageNot$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O_STDC)/mlib_c_ImageOr$(OBJ_EXT)            :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageLogic.h
$(DIR_O_STDC)/mlib_c_ImageXor$(OBJ_EXT)           :  $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_TXT)/mlib_c_ImageLogic.h
