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

SUBDIRS = vis vis2

ALL_TAGS = $(SUBDIRS:%=%.all)
ALL_V9_TAGS = $(SUBDIRS:%=%.allv9)
CLEAN_TAGS = $(SUBDIRS:%=%.clean)
CLEAN_ALL_TAGS = $(SUBDIRS:%=%.cleanall)
CP_TAGS = $(SUBDIRS:%=%.cp_incl)

all : $(ALL_TAGS)

all_v9 : $(ALL_V9_TAGS)

clean : $(CLEAN_TAGS)

clean_all : $(CLEAN_ALL_TAGS)

cp_include : $(CP_TAGS)

%.all :
	@(cd $(*); $(MAKE))

%.allv9 :
	@(cd $(*); $(MAKE) all_v9)

%.clean :
	@(cd $(*); $(MAKE) clean)

%.cleanall :
	@(cd $(*); $(MAKE) clean_all)

%.cp_incl :
	@(cd $(*); $(MAKE) cp_include)

force:
