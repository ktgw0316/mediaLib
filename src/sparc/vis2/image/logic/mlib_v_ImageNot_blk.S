/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageNot_blk.S	9.2	07/11/05 SMI"

! FUNCTION
!      mlib_v_ImageNot_blk - Not an image into another
!                            (with Block Load/Store)
!
! SYNOPSIS
!      void mlib_v_ImageNot_blk(const void *src,
!                               void       *dst,
!                               int        size);
!
! ARGUMENT
!      src     source image data
!      dst     destination image data
!      size    image size in bytes
!
! NOTES
!      src and dst must point to 64-byte aligned addresses
!      size must be multiple of 64
!
! DESCRIPTION
!      dst = ~src
!

#include "vis_asi.h"

! Minimum size of stack frame according to SPARC ABI
#ifdef __sparcv9
#define MINFRAME    176
#else
#define MINFRAME    96
#endif

! ENTRY provides the standard procedure entry code
#define ENTRY(x)                \
        .align  4;              \
        .global x;              \
        .type x, #function;     \
x:

! SET_SIZE trails a function and sets the size for the ELF symbol table
#define SET_SIZE(x) \
        .size   x, (.-x)

! SPARC have four integer register groups. i-registers %i0 to %i7
! hold input data. o-registers %o0 to %o7 hold output data. l-registers
! %l0 to %l7 hold local data. g-registers %g0 to %g7 hold global data.
! Note that %g0 is alway zero, write to it has no program-visible effect.

! When calling an assembly function, the first 6 arguments are stored
! in i-registers from %i0 to %i5. The rest arguments are stored in stack.
! Note that %i6 is reserved for stack pointer and %i7 for return address.

! Only the first 32 f-registers can be used as 32-bit registers.
! The last 32 f-registers can only be used as 16 64-bit registers.

#define src     %i0
#define dst     %i1
#define sz      %i2

#define sa      %l0
#define da      %l1

        .section        ".text",#alloc,#execinstr

        ENTRY(mlib_v_ImageNot_blk)      ! function name

        save            %sp,-MINFRAME,%sp       ! reserve space for stack
                                                ! and adjust register window

        deccc           sz                      ! do some error checking (size > 1)
        ble,pn          %ncc,2f
        mov             src, sa
        mov             dst, da

        prefetch        [sa], #one_read
        prefetch        [sa + (1 * 64)], #one_read
        prefetch        [sa + (2 * 64)], #one_read
        prefetch        [sa + (3 * 64)], #one_read
        ldd             [sa], %f0
        prefetch        [sa + (4 * 64)], #one_read
        ldd             [sa + 0x8], %f2
        prefetch        [sa + (5 * 64)], #one_read
        ldd             [sa + 0x10], %f4
        prefetch        [sa + (6 * 64)], #one_read
        fnot1           %f0, %f32
        ldd             [sa + 0x18], %f6
        fnot1           %f2, %f34
        ldd             [sa + 0x20], %f8
        fnot1           %f4, %f36
        ldd             [sa + 0x28], %f10
        fnot1           %f6, %f38
        ldd             [sa + 0x30], %f12
        fnot1           %f8, %f40
        ldd             [sa + 0x38], %f14
        fnot1           %f10, %f42
        ldd             [sa + 0x40], %f0
        add             sa, 0x48, sa

1:
        ldd             [sa], %f2
        fnot1           %f12, %f44
        ldd             [sa + 0x8], %f4
        fnot1           %f14, %f46
        stda            %f32, [da] ASI_BLK_P
        ldd             [sa + 0x10], %f6
        fnot1           %f0, %f32
        add             da, 0x40, da
        ldd             [sa + 0x18], %f8
        fnot1           %f2, %f34
        ldd             [sa + 0x20], %f10
        fnot1           %f4, %f36
        ldd             [sa + 0x28], %f12
        deccc           sz
        fnot1           %f6, %f38
        ldd             [sa + 0x30], %f14
        fnot1           %f8, %f40
        ldd             [sa + 0x38], %f0
        prefetch        [sa + (6 * 64)], #one_read
        fnot1           %f10, %f42
        bgu,pt          %ncc, 1b
        add             sa, 0x40, sa

        fnot1           %f12, %f44
        fnot1           %f14, %f46
        stda            %f32, [da] ASI_BLK_P

                                        ! issue memory barrier instruction
        membar  #StoreLoad|#StoreStore  ! to ensure all previous memory load
                                        ! and store has completed
2:
        ret                             ! return
        restore                         ! restore register window

        SET_SIZE(mlib_v_ImageNot_blk)
