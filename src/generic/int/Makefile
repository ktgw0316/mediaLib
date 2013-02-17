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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)Makefile	9.6	08/10/31 SMI"
#

.INIT : cp_include get_include create_proto
	@($(TOUCH) -c -t 199605090000 $(DIR_SYS_V8PLUS)/*ver*.*; \
	  $(TOUCH) -c -t 199605090000 $(DIR_SYS_V9)/*ver*.*)

PROTO_SFX	=
PROTO_OPT	=

TOP_DIR = ../../..
LIBNAME = mlib
DEFARCH = sparcv8plus
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

LIB_DIR_V = $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int/lib/$(CHIP_DIR)
LIB_DIR   = $(LIB_DIR_V)

include $(PROJECT_ROOT)/make/main.cfg
include $(CONFIG.$(HOST_ARCH_NAME).PROJECT)
include $(CONFIG.$(TARCH).OVERRIDES)
include $(CONFIG.$(TARCH).C)

SUB_DIRS = algebra image graphics signal video volume
sparc_LINK_DIRS = sparcv8plus sparcv9
i386_LINK_DIRS = i386 amd64
LINK_DIRS = $($(TARCH)_LINK_DIRS)

SUNMATH_OBJS_COM = cosf \
                   sincosf \
                   sinf \
                   _TBL_ipio2 \
                   __rem_pio2f_ \
                   __rem_pio2m \
                   __rem_pio2 \
                   expf \
                   logf \
                   _SVID_errorf \
                   rintf \
                   sincos

sparc_SUNMATH_OBJS = __cosf \
                     __sinf \
                     _TBL_r_log_ \
                     _TBL_sin \
                     _TBL_cos \
                     __sincos \
                     $(SUNMATH_OBJS_COM)

sparc-debug_SUNMATH_OBJS = $($(TARCH)_SUNMATH_OBJS)

i386_SUNMATH_OBJS = $(SUNMATH_OBJS_COM) __reduction __reductionf fabsf ieee_funcf fp_classf
x86_SUNMATH_OBJS  = $($(TARCH)_SUNMATH_OBJS)
x86-debug_SUNMATH_OBJS = $($(TARCH)_SUNMATH_OBJS)

SUNMATH_OBJS_LIST = \
  for i in ${SUB_DIRS}; do \
    [ "$$i" = graphics -o "$$i" = signal -o "$$i" = image ] && { \
      echo $($(TARCH)_SUNMATH_OBJS); \
      break; \
     } ; \
    echo ""; \
  done

SUNMATH_OBJS = $(SUNMATH_OBJS_LIST:sh)

OBJS = $(DIR_SYS)/*$(OBJ_EXT) \
       $(OBJS_SUBDIRS:sh) \
       $($(USE_EMBEDDED_SUNMATH)SUNMATH_OBJS:%=$(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/sunmath/%$(OBJ_EXT))

OBJS_SUBDIRS = \
  for i in ${SUB_DIRS}; do \
    echo $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/$$i/$(LIBNAME)\\*$(OBJ_EXT)\\; \
    echo "\n"; \
    [ "$$i" = image ] && { \
      echo $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/signal/$(LIBNAME)\\*$(OBJ_EXT)\\; \
      echo "\n"; continue; }; \
    [ "$$i" = graphics ] && { \
      echo $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/image/$(LIBNAME)\\*$(OBJ_EXT)\\; \
      echo "\n"; continue; }; \
    [ "$$i" = signal -o "$$i" = video ] && { \
      echo $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/algebra/$(LIBNAME)\\*$(OBJ_EXT)\\; \
      echo "\n"; \
      echo $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int$($(TARCH)_GCC_DIR)/obj/$(CHIP_DIR)/image/$(LIBNAME)\\*$(OBJ_EXT)\\; \
      echo "\n"; continue; }; \
    echo ""; \
  done | $(SORT) -u | $(SED) -e 's?\\??g'

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
v8 sparcv8             :: cp_lib clean_links make_links
v8p v8plus sparcv8plus :: cp_lib clean_links make_links
v9 sparcv9             :: cp_lib clean_links make_links
x86 386 i386           :: cp_lib clean_links make_links
sse2                   :: cp_lib clean_links make_links
x64 amd64              :: cp_lib clean_links make_links
v8 sparcv8             := LINK_DIRS = sparcv8
v8p v8plus sparcv8plus := LINK_DIRS = sparcv8plus
v9 sparcv9             := LINK_DIRS = sparcv9
x86 386 i386           := LINK_DIRS = i386
sse2                   := LINK_DIRS = sse2
x64 amd64              := LINK_DIRS = amd64

cp_include : $(DIR_INCLUDE) $(SHARE_INCLUDE) $(COPY_INCLUDE)

get_include : $(NO_COPY_INCLUDE)

$(DIR_INCLUDE) :
	$(MKDIR) $(DIR_INCLUDE)

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
cp_lib : $(LIB_DIR) clean_lib
	@[ -d $(TOP_DIR)/lib ] || $(MKDIR) $(TOP_DIR)/lib
	@for i in $(LINK_DIRS); do \
	  echo Copying $$i to $(TOP_DIR)/lib ; \
	  $(CP) -rf $(TOP_DIR)/builds/$(TARGET_OS)/$(ARCH)_int/lib/$$i $(TOP_DIR)/lib/$$i\_int; \
	done

clean_lib : force
	@for i in $(LINK_DIRS); do \
	  $(RM) -rf $(TOP_DIR)/lib/$$i\_int; \
	done

make_links : force
	@(cd $(TOP_DIR)/lib; \
	for j in $(LINK_DIRS); do \
	    cd $$j\_int; \
	    for i in $(SUB_DIRS); do \
	        echo Creating symbolic link for $$j/$$i; \
	        ln -s ./$(LIB_LIBNAME)_$$i$(SHAREDLIB_EXT)$(MLIB_API_VERS) \
	              ./$(LIB_LIBNAME)_$$i$(SHAREDLIB_EXT); \
	    done; \
	    ln -s ./$(LIB_LIBNAME)$(SHAREDLIB_EXT)$(MLIB_API_VERS) \
	          ./$(LIB_LIBNAME)$(SHAREDLIB_EXT); \
	    cd ..; \
	done)

clean_links: force
	@(cd $(TOP_DIR)/lib; \
	for i in $(LINK_DIRS); do \
	  $(RM) $$i\_int/$(LIB_LIBNAME)*$(SHAREDLIB_EXT); \
	done)

force:
