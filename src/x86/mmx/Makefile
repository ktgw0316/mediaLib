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
# ident	"@(#)Makefile	9.5	07/11/05 SMI"
#

.INIT : cp_include get_include create_proto
	@($(TOUCH) -c -t 199605090000 $(DIR_SYS)/*ver*.*)

PROTO_SFX	=
PROTO_OPT	=

TOP_DIR = ../../..
LIBNAME = mlib
include $(MLIB_HOME)/make/custom/$(MLIB_CUSTOM).cfg

SHARE_INCLUDE_MLIB     = mlib.h mlib_status.h mlib_types.h
SHARE_INCLUDE_SYS      = mlib_sys.h mlib_sys_proto.h
SHARE_INCLUDE_ALGEBRA  = mlib_algebra.h mlib_algebra_proto.h
SHARE_INCLUDE_GRAPHICS = mlib_graphics.h mlib_graphics_proto.h  mlib_graphics_drawset_proto.h
SHARE_INCLUDE_IMAGE    = mlib_image.h mlib_image_types.h mlib_image_get.h mlib_image_proto.h  mlib_image_blend_proto.h
SHARE_INCLUDE_SIGNAL   = mlib_signal.h mlib_signal_types.h mlib_signal_proto.h
SHARE_INCLUDE_VIDEO    = mlib_video.h mlib_video_proto.h
SHARE_INCLUDE_VOLUME   = mlib_volume.h mlib_volume_types.h mlib_volume_proto.h

DIR_INCLUDE  = $(TOP_DIR)/include
DIR_PROTO  = $(TOP_DIR)/src/proto

SHARE_DIR    = $(TOP_DIR)/src/include/

STDC_SYS_DIR = $(TOP_DIR)/src/generic/stdc/

NO_COPY_SHARE_INCLUDE_SYS = mlib_SysMath.h
NO_COPY_INCLUDE = $(NO_COPY_SHARE_INCLUDE_SYS:%.h=$(STDC_SYS_DIR)sys/include/%.h)

SHARE_INCLUDE_NAME = $(SHARE_INCLUDE_MLIB) $(SHARE_INCLUDE_SYS) \
                     $(SHARE_INCLUDE_ALGEBRA) $(SHARE_INCLUDE_DATA) \
                     $(SHARE_INCLUDE_GRAPHICS) $(SHARE_INCLUDE_IMAGE) \
                     $(SHARE_INCLUDE_SIGNAL) $(SHARE_INCLUDE_VIDEO) \
                     $(SHARE_INCLUDE_VOLUME)

COPY_INCLUDE = $(SHARE_INCLUDE_NAME:%.h=$(DIR_INCLUDE)/%.h)
SHARE_INCLUDE = $(SHARE_INCLUDE_NAME:%.h=$(SHARE_DIR)/%.h)
PROTO_INCLUDE = $(SHARE_INCLUDE_NAME:%.h=$(DIR_PROTO)/$(PROTO_SFX)%.h)

i386_LIB_DIR_V = $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)/lib/$(CHIP_DIR)
x86_LIB_DIR_V  = $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)/lib/$($(TARCH)_ICC_DIR)
LIB_DIR   = $($(TARCH)_LIB_DIR_V)

include $(PROJECT_ROOT)/make/main.cfg
include $(CONFIG.$(HOST_ARCH_NAME).PROJECT)
include $(CONFIG.$(TARCH).OVERRIDES)
include $(CONFIG.$(TARCH).C)

VERSION_TARGET = mmx

SUB_DIRS = image

OBJS = $(DIR_SYS)/*$(OBJ_EXT) \
       $($(TARCH)_OBJS_SUBDIRS)

x86_OBJS_SUBDIRS  = $(OBJS_SUBDIRS_GMAKE)
i386_OBJS_SUBDIRS = $(OBJS_SUBDIRS_MAKE:sh)

OBJS_SUBDIRS_GMAKE = $(shell \
  for i in ${SUB_DIRS}; do \
    echo $(DIR_O)/$$i/\*$(OBJ_EXT); \
    [ "$$i" = image ] && { \
      echo $(DIR_O)/signal/\*$(OBJ_EXT); \
      continue; }; \
    [ "$$i" = graphics ] && { \
      echo $(DIR_O)/image/\*$(OBJ_EXT); \
      continue; }; \
    [ "$$i" = signal -o "$$i" = video ] && { \
      echo $(DIR_O)/algebra/\*$(OBJ_EXT); \
      echo $(DIR_O)/image/\*$(OBJ_EXT); \
      continue; }; \
  done )

OBJS_SUBDIRS_MAKE = \
    for i in ${SUB_DIRS}; do \
    echo $(DIR_O)/$$i/\*$(OBJ_EXT); \
    [ "$$i" = image ] && { \
      echo $(DIR_O)/signal/\*$(OBJ_EXT); \
      continue; }; \
    [ "$$i" = graphics ] && { \
      echo $(DIR_O)/image/\*$(OBJ_EXT); \
      continue; }; \
    [ "$$i" = signal -o "$$i" = video ] && { \
      echo $(DIR_O)/algebra/\*$(OBJ_EXT); \
      echo $(DIR_O)/image/\*$(OBJ_EXT); \
      continue; }; \
  done

MAPFILE_DIR = $(TOP_DIR)/src/generic/stdc

MAPFILES_CMD = \
  for i in ${SUB_DIRS}; do \
    [ "$$i" = image ] && { echo $(MAPFILE_DIR)/image/mapfile_common; echo $(MAPFILE_DIR)/signal/mapfile_common; break ; }; \
    [ "$$i" = graphics ] && { echo $(MAPFILE_DIR)/image/mapfile_common; break ; }; \
    [ "$$i" = signal ] && { echo $(MAPFILE_DIR)/signal/mapfile_common; break ; }; \
    [ "$$i" = algebra ] && { echo $(MAPFILE_DIR)/algebra/mapfile_common; continue; }; \
    echo ""; \
  done
MAPFILES = $(MAPFILE_DIR)/mapfile $(SUB_DIRS:%=$(MAPFILE_DIR)/%/mapfile) $(MAPFILES_CMD:sh)

include $(PROJECT_ROOT)/make/rules/directory.cfg
include $(PROJECT_ROOT)/make/rules/lib.cfg

all    : $(ARCH)
asmc    :: $(TARCH)
i386    : cp_lib_i386
win32   : cp_lib_win32
linux   : cp_lib_linux

cp_include : $(DIR_INCLUDE) $(SHARE_INCLUDE) $(COPY_INCLUDE)

get_include : $(NO_COPY_INCLUDE)

$(DIR_INCLUDE) :
	$(MKDIR) $@

$(DIR_INCLUDE)/%.h : $(SHARE_DIR)/%.h
	$(CP) -f $< $@

#
# Create proto
#
H_CONV = $(TOP_DIR)/tools/h_conv

create_proto : $(H_CONV) $(DIR_PROTO) $(SHARE_INCLUDE) $(PROTO_INCLUDE)

$(DIR_PROTO) :
	$(MKDIR) $@

$(DIR_PROTO)/$(PROTO_SFX)%.h : $(SHARE_DIR)/%.h
	$(H_CONV) $(PROTO_OPT) -o $@ $<

$(PROTO_INCLUDE)	: $(H_CONV)

#
#
#
cp_lib_i386 : $(LIB_DIR)
	[ -d $(TOP_DIR)/lib/$(CHIP_DIR) ] || $(MKDIR) $(TOP_DIR)/lib/$(CHIP_DIR)
	echo Copying $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)/lib/$(CHIP_DIR) to $(TOP_DIR)/lib/$(CHIP_DIR)
	$(CP) -f $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)/lib/$(CHIP_DIR)/* $(TOP_DIR)/lib/$(CHIP_DIR)/

cp_lib_win32 : $(LIB_DIR)
	@[ -d $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_INTELC_DIR) ] || $(MKDIR) $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_INTELC_DIR)
	echo Copying $(TOP_DIR)/builds/$(ARCH)/$(TARGET_OS)/lib/$($(TARCH)_INTELC_DIR) to $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_INTELC_DIR)
	@$(CP) -f $(TOP_DIR)/builds/$(ARCH)/$(TARGET_OS)/lib/$($(TARCH)_INTELC_DIR)/* $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_INTELC_DIR)/

cp_lib_linux : $(LIB_DIR)
	@[ -d $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_ICC_DIR) ] || $(MKDIR) $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_ICC_DIR)
	echo Copying $(TOP_DIR)/builds/$(ARCH)/$(TARGET_OS)/lib/$($(TARCH)_ICC_DIR) to $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_ICC_DIR)
	@$(CP) -f $(TOP_DIR)/builds/$(ARCH)/$(TARGET_OS)/lib/$($(TARCH)_ICC_DIR)/* $(TOP_DIR)/lib/$(ARCH)+$($(TARCH)_ICC_DIR)/

force:
