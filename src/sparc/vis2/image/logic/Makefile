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

HOME_TXT = $(MLIB_HOME)/src/generic/stdc/$(MODNAME)/logic
HOME_VIS_TXT = $(MLIB_HOME)/src/sparc/vis/$(MODNAME)/logic

UTILS_SH=cd ${HOME_TXT}; $(MAKE) -D -n "SRCS_S=" com | $(EGREP) "^ *UTILS *=" | $(SED) -e 's/^.*=//g' | $(SORT) -u
UTILS_STDC = $(UTILS_SH:sh)

SRCS_VIS1_SH_TMP=cd ${HOME_VIS_TXT}; $(MAKE) -D -n "SRCS=" "SRCS_S=" com | $(EGREP) "^ *SRCS_V *=" | $(SED) -e 's/^.*=//g'
SRCS_VIS1_SH = $(SRCS_VIS1_SH_TMP:sh)

SRCS_VIS1_TMP = qq="$(SRCS_VIS1_SH)"; for i in $(SRCS_V); do qq=`echo $$qq | $(SED) -e "s/\<$$i\>//g"`; done; echo $$qq
SRCS_VIS1 = $(SRCS_VIS1_TMP:sh)

SRCS_SV = mlib_v_ImageNot_blk.S

SRCS_V = mlib_v_ImageNot.c

include $(PROJECT_ROOT)/make/rules/leaf.cfg

INCLUDES += -I$(HOME_TXT)/../include -I$(HOME_VIS_TXT) -I$(HOME_TXT)

#
# define depends
#

$(OBJS) : $(HOME_TXT)/../include/mlib_ImageCheck.h

$(DIR_O)/mlib_ImageAndNot_Bit$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageAnd_Bit$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstAndNot_Bit$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstAnd_Bit$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotAnd_Bit$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotOr_Bit$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstNotXor_Bit$(OBJ_EXT) : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstOrNot_Bit$(OBJ_EXT)  : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstOr_Bit$(OBJ_EXT)     : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageConstXor_Bit$(OBJ_EXT)    : $(HOME_TXT)/mlib_ImageConstLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotAnd_Bit$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotOr_Bit$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageNotXor_Bit$(OBJ_EXT)      : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageOrNot_Bit$(OBJ_EXT)       : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageOr_Bit$(OBJ_EXT)          : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h
$(DIR_O)/mlib_ImageXor_Bit$(OBJ_EXT)         : $(HOME_TXT)/mlib_ImageLogic_Bit.h  $(HOME_TXT)/mlib_ImageLogic_proto.h


$(DIR_O)/mlib_v_ImageAnd$(OBJ_EXT)             : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageAndNot$(OBJ_EXT)          : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageAndNot_Bit$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageAnd_Bit$(OBJ_EXT)         : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstAnd$(OBJ_EXT)        : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstAndNot$(OBJ_EXT)     : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstAndNot_Bit$(OBJ_EXT) : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstAnd_Bit$(OBJ_EXT)    : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotAnd$(OBJ_EXT)     : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotAnd_Bit$(OBJ_EXT) : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotOr$(OBJ_EXT)      : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotOr_Bit$(OBJ_EXT)  : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotXor$(OBJ_EXT)     : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstNotXor_Bit$(OBJ_EXT) : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstOr$(OBJ_EXT)         : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstOrNot$(OBJ_EXT)      : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstOrNot_Bit$(OBJ_EXT)  : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstOr_Bit$(OBJ_EXT)     : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstXor$(OBJ_EXT)        : ./mlib_v_ImageConstLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageConstXor_Bit$(OBJ_EXT)    : ./mlib_v_ImageConstLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNot$(OBJ_EXT)             : $(HOME_TXT)/mlib_ImageLogic_proto.h $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotAnd$(OBJ_EXT)          : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotAnd_Bit$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotOr$(OBJ_EXT)           : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotOr_Bit$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotXor$(OBJ_EXT)          : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNotXor_Bit$(OBJ_EXT)      : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNot_Bit$(OBJ_EXT)         : $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageNot_f$(OBJ_EXT)           : $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageOr$(OBJ_EXT)              : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageOrNot$(OBJ_EXT)           : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageOrNot_Bit$(OBJ_EXT)       : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageOr_Bit$(OBJ_EXT)          : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageXor$(OBJ_EXT)             : $(HOME_VIS_TXT)/mlib_v_ImageLogic.h  $(HOME_TXT)/mlib_ImageLogic_proto.h \
                                                 $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h
$(DIR_O)/mlib_v_ImageXor_Bit$(OBJ_EXT)         : $(HOME_VIS_TXT)/mlib_v_ImageLogic_Bit.h  $(HOME_VIS_TXT)/mlib_v_ImageLogic_proto.h

