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

#pragma ident	"@(#)mlib_v_SignalFFT_Layer.S	9.2	07/11/05 SMI"

! FUNCTION
!      mlib_v_fftK_Layer_Div2 -
!              performs one layer of FFT with dividing by two
!
! SYNOPSIS
!      void mlib_v_fftK_Layer_Div2(mlib_s16 *data_r,
!                                  mlib_s16 *data_i,
!                                  mlib_s32 order,
!                                  mlib_s32 layer,
!                                  mlib_d64 *tbl)
!
! ARGUMENT
!      data_r  real  parts, size of array 2^order
!      data_i  image parts, size of array 2^order
!      order   order of FFT
!      layer   number of currently processing layer
!      tbl     table of rotating vectors
!
! NOTES
!      data_r and data_i must point to 8-byte aligned addresses
!

		.register %g2, #scratch
		.register %g3, #scratch

#ifdef __sparcv9
#define MINFRAME	176
#define BIAS		2047
#else
#define MINFRAME	96
#define BIAS		0
#endif

! ENTRY provides the standard procedure entry code
#define ENTRY(x)                                \
        .align  32;                             \
        .global x;                              \
x:

! SET_SIZE trails a function and sets the size for the ELF symbol table
#define SET_SIZE(x)                             \
        .type	x, 2;                           \
        .size   x, (.-x)


#define GET_INDS0(N)                                    \
                add         j, 8*(0 + 3*N), jj;         \
                andn        jj, mask, ind0_s;           \
                and         jj, mask, ind0_t;           \
                add         ind0_s, jj, ind0_s;         \
                add         ind0_t, ind0_t, ind0_t

#define GET_INDS1(N)                                    \
                add         j, 8*(1 + 3*N), jj;         \
                andn        jj, mask, ind1_s;           \
                and         jj, mask, ind1_t;           \
                add         ind1_s, jj, ind1_s;         \
                add         ind1_t, ind1_t, ind1_t

#define GET_INDS2(N)                                    \
                add         j, 8*(2 + 3*N), jj;         \
                andn        jj, mask, ind2_s;           \
                and         jj, mask, ind2_t;           \
                add         ind2_s, jj, ind2_s;         \
                add         ind2_t, ind2_t, ind2_t


#define psrc0_r   %i0
#define psrc0_i   %i1
#define order     %i2
#define layer     %i3
#define tbl       %i4

#define psrc1_r   %l0
#define psrc1_i   %l1
#define tbl1      %l2
#define j         %l3
#define jj        %l4
#define nn        %l5
#define mask      %l6
#define ind2_s    %l7
#define ind2_d    %g1
#define ind2_t    %g2
#define ind0_s    %g3
#define ind0_d    %g4
#define ind0_t    %o1
#define ind1_s    %o2
#define ind1_d    %o3
#define ind1_t    %o4

#define ZERO      %g0

#define scale     %f0

#define a0        %f2
#define a1        %f4
#define a2        %f6
#define a3        %f8
#define a4        %f10
#define a5        %f12
#define a6        %f14
#define a7        %f16
#define a8        %f18
#define a9        %f20
#define b0        %f22
#define b1        %f24
#define b2        %f26
#define b3        %f28
#define b4        %f30
#define b5        %f32
#define b6        %f34
#define b7        %f36
#define b8        %f38
#define b9        %f40
#define c0        %f42
#define c1        %f44
#define c2        %f46
#define c3        %f48
#define c4        %f50
#define c5        %f52
#define c6        %f54
#define c7        %f56
#define c8        %f58
#define c9        %f60

#define TT        %f62

#define TMP       8

        .section        ".text",#alloc,#execinstr

        ENTRY(mlib_v_fftK_Layer_Div2)

                save        %sp, -MINFRAME-TMP, %sp

                or          ZERO, 1, nn
                sll         nn, layer, mask
                or          ZERO, ZERO, j
                sll         nn, order, order
                add         tbl, mask, tbl
                add         psrc0_r, mask, psrc1_r
                add         tbl, mask, tbl
                add         psrc0_i, mask, psrc1_i
                add         tbl, 8, tbl1
                sub         mask, 1, mask
                sub         order, 3*8, nn

                sethi       %hi(0x80808000), jj
                add         jj, 0x80, jj
                st          jj, [%fp + BIAS - TMP]

                ldd         [psrc1_r + ZERO], b0
                GET_INDS0(1)
                ldd         [psrc1_i + ZERO], b1
                GET_INDS1(1)
                ldd         [tbl + ZERO], b2
                GET_INDS2(0)
                ldd         [tbl1 + ZERO], b3
                ldd         [psrc0_r + ZERO], b4
                fmul8sux16  b0, b2, b8
                ldd         [psrc0_i + ZERO], b5
                fmul8ulx16  b0, b2, b6
                ld          [%fp + BIAS - TMP], scale
                fmul8sux16  b1, b3, b9
                ldd         [psrc1_r + 8], c0
                fmul8ulx16  b1, b3, b7
                ldd         [psrc1_i + 8], c1
                fmul8sux16  b1, b2, TT
                ldd         [tbl + 16], c2
                fmul8ulx16  b1, b2, b2
                ldd         [tbl1 + 16], c3
                fmul8sux16  b0, b3, b1
                ldd         [psrc0_r + 8], c4
                fmul8ulx16  b0, b3, b3
                ldd         [psrc0_i + 8], c5
                fmul8x16    scale, b4, b4
                fpadd16     b6, b8, b6
                fmul8x16    scale, b5, b5
                ldd         [psrc1_r + ind0_s], b0
                fmul8sux16  c0, c2, c8
                ldd         [psrc1_r + ind2_s], a0
                fpadd16     b7, b9, b7
                fmul8ulx16  c0, c2, c6
                ldd         [psrc1_i + ind2_s], a1
                fpadd16     b2, TT, b2
                fmul8sux16  c1, c3, c9
                ldd         [tbl + ind2_t], a2
                fpadd16     b3, b1, b3
                fmul8ulx16  c1, c3, c7
                ldd         [tbl1 + ind2_t], a3
                fpsub16     b6, b7, b6
                ldd         [psrc1_i + ind0_s], b1
                fmul8sux16  c1, c2, TT
                fpadd16     b3, b2, b7
                fmul8ulx16  c1, c2, c2
                ldd         [tbl + ind0_t], b2
                fpadd16     b4, b6, b8
                fmul8sux16  c0, c3, c1
                ldd         [tbl1 + ind0_t], b3
                fmul8ulx16  c0, c3, c3
                ldd         [psrc0_r + ind2_s], a4
                fpsub16     b4, b6, b6
                fmul8x16    scale, c4, c4
                ldd         [psrc0_i + ind2_s], a5
                fpadd16     b5, b7, b9
                ldd         [psrc0_r + ind0_s], b4
                fmul8x16    scale, c5, c5
                fpsub16     b5, b7, b7
                or          ZERO, ZERO, ind0_d
                or          ZERO, 8, ind1_d

        LOOP:
                ldd         [psrc0_i + ind0_s], b5
                fmul8sux16  a0, a2, a8
                fpadd16     c6, c8, c6
                add         j, 8*(2 + 3*1), jj

                ldd         [psrc1_r + ind1_s], c0
                fmul8ulx16  a0, a2, a6
                fpadd16     c7, c9, c7
                andn        jj, mask, ind2_s

                std         b8, [psrc0_r + ind0_d]
                fmul8sux16  a1, a3, a9
                fpadd16     c2, TT, c2
                and         jj, mask, ind2_t

                std         b6, [psrc1_r + ind0_d]
                fmul8ulx16  a1, a3, a7
                fpadd16     c3, c1, c3
                add         ind2_s, jj, ind2_s

                ldd         [psrc1_i + ind1_s], c1
                fmul8sux16  a1, a2, TT
                fpsub16     c6, c7, c6
                add         ind2_t, ind2_t, ind2_t

                std         b9, [psrc0_i + ind0_d]
                fmul8ulx16  a1, a2, a2
                fpadd16     a6, a8, a6
                add         j, 8*(0 + 3*2), jj

                std         b7, [psrc1_i + ind0_d]
                fmul8sux16  a0, a3, a1
                fpadd16     c3, c2, c7
                andn        jj, mask, ind0_s

                ldd         [tbl + ind1_t], c2
                fmul8ulx16  a0, a3, a3
                fpadd16     a7, a9, a7
                and         jj, mask, ind0_t

                ldd         [tbl1 + ind1_t], c3
                fmul8x16    scale, a4, a4
                fpadd16     c4, c6, c8
                add         ind0_s, jj, ind0_s

                ldd         [psrc1_r + ind2_s], a0
                fmul8x16    scale, a5, a5
                fpsub16     c4, c6, c6
                add         ind0_t, ind0_t, ind0_t

                ldd         [psrc0_r + ind1_s], c4
                fpadd16     c5, c7, c9
                add         j, 8*2, jj

                std         c8, [psrc0_r + ind1_d]
                fmul8sux16  b0, b2, b8
                fpadd16     a2, TT, a2
                andn        jj, mask, ind2_d

                std         c6, [psrc1_r + ind1_d]
                fmul8ulx16  b0, b2, b6
                fpadd16     a3, a1, a3
                add         ind2_d, jj, ind2_d

                std         c9, [psrc0_i + ind1_d]
                fmul8sux16  b1, b3, b9
                fpsub16     a6, a7, a6

                ldd         [psrc1_i + ind2_s], a1
                fmul8ulx16  b1, b3, b7
                fpsub16     c5, c7, c7

                ldd         [psrc0_i + ind1_s], c5
                fmul8sux16  b1, b2, TT
                fpadd16     a3, a2, a7
                add         j, 8*(1 + 3*2), jj

                ldd         [tbl + ind2_t], a2
                fmul8ulx16  b1, b2, b2
                fpadd16     a4, a6, a8
                andn        jj, mask, ind1_s

                ldd         [tbl1 + ind2_t], a3
                fmul8sux16  b0, b3, b1
                fpsub16     a4, a6, a6
                and         jj, mask, ind1_t

                ldd         [psrc0_r + ind2_s], a4
                fmul8ulx16  b0, b3, b3
                fpadd16     a5, a7, a9
                add         ind1_s, jj, ind1_s

                !prefetch    [psrc1_i + 520], 0
                fmul8x16    scale, b4, b4
                fpsub16     a5, a7, a7
                add         ind1_t, ind1_t, ind1_t

                ldd         [psrc0_i + ind2_s], a5
                fmul8x16    scale, b5, b5
                fpadd16     b6, b8, b6
                add         j, 8*3, j

                ldd         [psrc1_r + ind0_s], b0
                fmul8sux16  c0, c2, c8
                fpadd16     b7, b9, b7

                std         c7, [psrc1_i + ind1_d]
                fmul8ulx16  c0, c2, c6
                fpadd16     b2, TT, b2

                std         a8, [psrc0_r + ind2_d]
                fmul8sux16  c1, c3, c9
                fpadd16     b3, b1, b3
                cmp         j, nn

                ldd         [psrc1_i + ind0_s], b1
                fmul8ulx16  c1, c3, c7
                fpsub16     b6, b7, b6
                andn        j, mask, ind0_d

                std         a6, [psrc1_r + ind2_d]
                fmul8sux16  c1, c2, TT
                add         ind0_d, j, ind0_d

                std         a9, [psrc0_i + ind2_d]
                fmul8ulx16  c1, c2, c2
                fpadd16     b3, b2, b7

                ldd         [tbl + ind0_t], b2
                fmul8sux16  c0, c3, c1
                fpadd16     b4, b6, b8
                add         j, 8*1, jj

                ldd         [tbl1 + ind0_t], b3
                fmul8ulx16  c0, c3, c3
                fpsub16     b4, b6, b6
                andn        jj, mask, ind1_d

                ldd         [psrc0_r + ind0_s], b4
                fmul8x16    scale, c4, c4
                fpadd16     b5, b7, b9
                add         ind1_d, jj, ind1_d

                std         a7, [psrc1_i + ind2_d]
                fmul8x16    scale, c5, c5
                ble,pt      %icc, LOOP
                fpsub16     b5, b7, b7

        TAIL:
                fpadd16     c6, c8, c6
                cmp         j, order
                fpadd16     c7, c9, c7
                bge         %icc, EXIT
                sub         order, 8*1, order
                std         b8, [psrc0_r + ind0_d]
                std         b6, [psrc1_r + ind0_d]
                cmp         j, order
                std         b9, [psrc0_i + ind0_d]
                std         b7, [psrc1_i + ind0_d]
                bge         %icc, EXIT

                fpadd16     c2, TT, c2
                fpadd16     c3, c1, c3
                fpsub16     c6, c7, c6
                fpadd16     c3, c2, c7
                fpadd16     c4, c6, c8
                std         c8, [psrc0_r + ind1_d]
                fpsub16     c4, c6, c6
                std         c6, [psrc1_r + ind1_d]
                fpadd16     c5, c7, c9
                std         c9, [psrc0_i + ind1_d]
                fpsub16     c5, c7, c7
                std         c7, [psrc1_i + ind1_d]

        EXIT:
                ret
                restore

        SET_SIZE(mlib_v_fftK_Layer_Div2)
