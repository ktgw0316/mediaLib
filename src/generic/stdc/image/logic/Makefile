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

UTILS  =  mlib_ImageConstAnd_Bit.c    \
          mlib_ImageConstAndNot_Bit.c \
          mlib_ImageConstNotAnd_Bit.c \
          mlib_ImageConstNotOr_Bit.c  \
          mlib_ImageConstNotXor_Bit.c \
          mlib_ImageConstOr_Bit.c     \
          mlib_ImageConstOrNot_Bit.c  \
          mlib_ImageConstXor_Bit.c    \
          mlib_ImageAnd_Bit.c         \
          mlib_ImageAndNot_Bit.c      \
          mlib_ImageNotAnd_Bit.c      \
          mlib_ImageNotOr_Bit.c       \
          mlib_ImageNotXor_Bit.c      \
          mlib_ImageOr_Bit.c          \
          mlib_ImageOrNot_Bit.c       \
          mlib_ImageXor_Bit.c         \
          mlib_ImageNot_Bit.c         \
          mlib_ImageConstNotAnd_Inp.c \
          mlib_ImageConstNotOr_Inp.c  \
          mlib_ImageConstNotXor_Inp.c \
          mlib_ImageConstOrNot_Inp.c  \
          mlib_ImageConstOr_Inp.c     \
          mlib_ImageConstXor_Inp.c    \
          mlib_ImageNotAnd_Inp.c      \
          mlib_ImageNotOr_Inp.c       \
          mlib_ImageNotXor_Inp.c      \
          mlib_ImageNot_Inp.c         \
          mlib_ImageOrNot_Inp.c       \
          mlib_ImageAnd_Inp.c         \
          mlib_ImageAndNot_Inp.c      \
          mlib_ImageConstAnd_Inp.c    \
          mlib_ImageConstAndNot_Inp.c \
          mlib_ImageOr_Inp.c          \
          mlib_ImageXor_Inp.c

SRCS_C  = mlib_c_ImageAnd.c \
          mlib_c_ImageAndNot.c \
          mlib_c_ImageConstAnd.c \
          mlib_c_ImageConstAndNot.c \
          mlib_c_ImageConstNotAnd.c \
          mlib_c_ImageConstNotOr.c \
          mlib_c_ImageConstNotXor.c \
          mlib_c_ImageConstOr.c \
          mlib_c_ImageConstOrNot.c \
          mlib_c_ImageConstXor.c \
          mlib_c_ImageNot.c \
          mlib_c_ImageNotAnd.c \
          mlib_c_ImageNotOr.c \
          mlib_c_ImageNotXor.c \
          mlib_c_ImageOr.c \
          mlib_c_ImageOrNot.c \
          mlib_c_ImageXor.c

SRCS = $(SRCS_C)

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I./../include

#
# define depends
#

$(OBJS) : ./../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAndNot_Bit$(OBJ_EXT)      :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageAnd_Bit$(OBJ_EXT)         :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstAndNot_Bit$(OBJ_EXT) :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstAnd_Bit$(OBJ_EXT)    :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotAnd_Bit$(OBJ_EXT) :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotOr_Bit$(OBJ_EXT)  :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotXor_Bit$(OBJ_EXT) :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstOrNot_Bit$(OBJ_EXT)  :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstOr_Bit$(OBJ_EXT)     :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstXor_Bit$(OBJ_EXT)    :  ./mlib_ImageConstLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotAnd_Bit$(OBJ_EXT)      :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotOr_Bit$(OBJ_EXT)       :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotXor_Bit$(OBJ_EXT)      :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNot_Bit$(OBJ_EXT)         :  ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageOrNot_Bit$(OBJ_EXT)       :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageOr_Bit$(OBJ_EXT)          :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageXor_Bit$(OBJ_EXT)         :  ./mlib_ImageLogic_Bit.h ./mlib_ImageLogic_proto.h

$(DIR_O)/mlib_c_ImageAnd$(OBJ_EXT)           :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageAndNot$(OBJ_EXT)        :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageConstAnd$(OBJ_EXT)      :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstAndNot$(OBJ_EXT)   :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstNotAnd$(OBJ_EXT)   :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstNotOr$(OBJ_EXT)    :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstNotXor$(OBJ_EXT)   :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstOr$(OBJ_EXT)       :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstOrNot$(OBJ_EXT)    :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageConstXor$(OBJ_EXT)      :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageConstLogic.h
$(DIR_O)/mlib_c_ImageNot$(OBJ_EXT)           :  ./mlib_ImageLogic_proto.h
$(DIR_O)/mlib_c_ImageNotAnd$(OBJ_EXT)        :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageNotOr$(OBJ_EXT)         :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageNotXor$(OBJ_EXT)        :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageOr$(OBJ_EXT)            :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageOrNot$(OBJ_EXT)         :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
$(DIR_O)/mlib_c_ImageXor$(OBJ_EXT)           :  ./mlib_ImageLogic_proto.h ./mlib_c_ImageLogic.h
