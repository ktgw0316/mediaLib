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
#define ENTRY(x) \
        .align  4; \
        .global x; \
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

!frame pointer  %i6
!return addr    %i7

!stack pointer  %o6
!call link      %o7

#define sa      %l0
#define da      %l1
#define se      %l2
#define ns      %l3

#define O0      %f16
#define O1      %f18
#define O2      %f20
#define O3      %f22
#define O4      %f24
#define O5      %f26
#define O6      %f28
#define O7      %f30

#define A0      %f32
#define A1      %f34
#define A2      %f36
#define A3      %f38
#define A4      %f40
#define A5      %f42
#define A6      %f44
#define A7      %f46

#define B0      %f48
#define B1      %f50
#define B2      %f52
#define B3      %f54
#define B4      %f56
#define B5      %f58
#define B6      %f60
#define B7      %f62

#define USE_BLD
#define USE_BST

#define MEMBAR_BEFORE_BLD        membar  #StoreLoad
#define MEMBAR_AFTER_BLD         membar  #Sync

#ifdef USE_BLD
#define BLD_A0                                  \
        cmp     sa,se;                          \
        blu,a,pt %icc,1f;                       \
        ldda    [sa]ASI_BLK_P,A0;               \
1:      inc     64,sa
#else
#define BLD_A0                                  \
        cmp     sa,se;                          \
        bgeu,pn %icc,1f;                        \
        nop;                                    \
        ldd     [sa +  0],A0;                   \
        ldd     [sa +  8],A1;                   \
        ldd     [sa + 16],A2;                   \
        ldd     [sa + 24],A3;                   \
        ldd     [sa + 32],A4;                   \
        ldd     [sa + 40],A5;                   \
        ldd     [sa + 48],A6;                   \
        ldd     [sa + 56],A7;                   \
1:      inc     64,sa
#endif

#ifdef USE_BLD
#define BLD_B0                                  \
        cmp     sa,se;                          \
        blu,a,pt %icc,1f;                       \
        ldda    [sa]ASI_BLK_P,B0;               \
1:      inc     64,sa
#else
#define BLD_B0                                  \
        cmp     sa,se;                          \
        bgeu,pn %icc,1f;                        \
        nop;                                    \
        ldd     [sa +  0],B0;                   \
        ldd     [sa +  8],B1;                   \
        ldd     [sa + 16],B2;                   \
        ldd     [sa + 24],B3;                   \
        ldd     [sa + 32],B4;                   \
        ldd     [sa + 40],B5;                   \
        ldd     [sa + 48],B6;                   \
        ldd     [sa + 56],B7;                   \
1:      inc     64,sa
#endif

#ifdef USE_BST
#define BST                                     \
        stda    O0,[da]ASI_BLK_P;               \
        inc     64,da;                          \
        deccc   ns;                             \
        ble,pn  %icc,mlib_v_ImageNot_end;       \
        nop
#else
#define BST                                     \
        std     O0,[da +  0];                   \
        std     O1,[da +  8];                   \
        std     O2,[da + 16];                   \
        std     O3,[da + 24];                   \
        std     O4,[da + 32];                   \
        std     O5,[da + 40];                   \
        std     O6,[da + 48];                   \
        std     O7,[da + 56];                   \
        inc     64,da;                          \
        deccc   ns;                             \
        ble,pn  %icc,mlib_v_ImageNot_end;       \
        nop
#endif

#define INVERSE_A0                              \
        fnot1 A0, O0;                           \
        fnot1 A1, O1;                           \
        fnot1 A2, O2;                           \
        fnot1 A3, O3;                           \
        fnot1 A4, O4;                           \
        fnot1 A5, O5;                           \
        fnot1 A6, O6;                           \
        fnot1 A7, O7

#define INVERSE_B0                              \
        fnot1 B0, O0;                           \
        fnot1 B1, O1;                           \
        fnot1 B2, O2;                           \
        fnot1 B3, O3;                           \
        fnot1 B4, O4;                           \
        fnot1 B5, O5;                           \
        fnot1 B6, O6;                           \
        fnot1 B7, O7

        .section        ".text",#alloc,#execinstr

        ENTRY(mlib_v_ImageNot_blk)      ! function name

        save    %sp,-MINFRAME,%sp       ! reserve space for stack
                                        ! and adjust register window
! do some error checking
        tst     sz                      ! size > 0
        ble,pn  %icc,mlib_v_ImageNot_ret

! calculate loop count
        sra     sz,6,ns                 ! 64 bytes per BST

        add     src,sz,se               ! end address of source
        mov     src,sa
        mov     dst,da
                                        ! issue memory barrier instruction
        MEMBAR_BEFORE_BLD               ! to ensure all previous memory load
                                        ! and store has completed
        BLD_A0
        BLD_B0                          ! issue the 2nd block load instruction
                                        ! to synchronize with returning data

                                        ! issue memory barrier instruction
        MEMBAR_AFTER_BLD                ! to ensure all previous memory load
                                        ! and store has completed.

mlib_v_ImageNot_bgn:

        INVERSE_A0                      ! process data returned by BLD_A0
        BLD_A0                          ! block load and sync data from BLD_B0
        BST                             ! block store data from BLD_A0

        INVERSE_B0                      ! process data returned by BLD_B0
        BLD_B0                          ! block load and sync data from BLD_A0
        BST                             ! block store data from BLD_B0

        bg,pt   %icc,mlib_v_ImageNot_bgn

mlib_v_ImageNot_end:
                                        ! issue memory barrier instruction
        MEMBAR_AFTER_BLD                ! to ensure all previous memory load
                                        ! and store has completed.
mlib_v_ImageNot_ret:
        ret                             ! return
        restore                         ! restore register window

        SET_SIZE(mlib_v_ImageNot_blk)
