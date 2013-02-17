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

#pragma ident	"@(#)mlib_c_VideoP64Loop_S16_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Loop_S16_U8 - Applies a 2-dimensional 3x3 spatial
 *                                 filter on the reference block
 *
 * SYNOPSIS
 *    mlib_VideoP64Loop_S16_U8(mlib_s16      mc_block[64],
 *                             const mlib_u8 *ref_block,
 *                             mlib_s32      stride)
 *
 * ARGUMENT
 *      mc_block   Pointer to motion-compensated reference block,
 *                 must be 8-byte aligned
 *      ref_block  Pointer to reference block
 *      stride     Stride in bytes between adjacent rows in reference
 *                 block, must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoP64Loop_S16_U8 = __mlib_VideoP64Loop_S16_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Loop_S16_U8) mlib_VideoP64Loop_S16_U8
	__attribute__((weak, alias("__mlib_VideoP64Loop_S16_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOADL(x)                                                \
	in0 = sl[0];                                            \
	in1 = sl[1];                                            \
	in2 = sl[2];                                            \
	in3 = sl[3];                                            \
	in4 = sl[4];                                            \
	in5 = sl[5];                                            \
	in6 = sl[6];                                            \
	in7 = sl[7]

#ifdef _LITTLE_ENDIAN

#define	FILTERX(x)                                              \
	d0 = in0 + in1;                                         \
	d1 = in1 + in2;                                         \
	in1 = d0 + d1;                                          \
	x##0 = (in1 << 16) | (in0 << 2);                        \
	d0 = in2 + in3;                                         \
	in2 = d1 + d0;                                          \
	d1 = in3 + in4;                                         \
	in3 = d0 + d1;                                          \
	x##1 = (in3 << 16) | in2;                               \
	d0 = in4 + in5;                                         \
	in4 = d1 + d0;                                          \
	d1 = in5 + in6;                                         \
	in5 = d0 + d1;                                          \
	x##2 = (in5 << 16) | in4;                               \
	d0 = in6 + in7;                                         \
	in6 = d1 + d0;                                          \
	x##3 = (in7 << 18) | in6

#else /* _LITTLE_ENDIAN */

#define	FILTERX(x)                                              \
	d0 = in0 + in1;                                         \
	d1 = in1 + in2;                                         \
	in1 = d0 + d1;                                          \
	x##0 = (in0 << 18) | in1;                               \
	d0 = in2 + in3;                                         \
	in2 = d1 + d0;                                          \
	d1 = in3 + in4;                                         \
	in3 = d0 + d1;                                          \
	x##1 = (in2 << 16) | in3;                               \
	d0 = in4 + in5;                                         \
	in4 = d1 + d0;                                          \
	d1 = in5 + in6;                                         \
	in5 = d0 + d1;                                          \
	x##2 = (in4 << 16) | in5;                               \
	d0 = in6 + in7;                                         \
	in6 = d1 + d0;                                          \
	x##3 = (in6 << 16) | (in7 << 2)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	ADDL(x, y)                                              \
	x##0 += y##0;                                           \
	x##1 += y##1;                                           \
	x##2 += y##2;                                           \
	x##3 += y##3

/* *********************************************************** */

#define	ADDLRND(x, y)                                           \
	x##0 += y##0 + 0x80008;                                 \
	x##1 += y##1 + 0x80008;                                 \
	x##2 += y##2 + 0x80008;                                 \
	x##3 += y##3 + 0x80008

/* *********************************************************** */

#define	STORB(x)                                                      \
	((mlib_u32 *)(sd))[0] = ((x##0 + 0x20002) >> 2) & 0xff00ff;   \
	((mlib_u32 *)(sd))[1] = ((x##1 + 0x20002) >> 2) & 0xff00ff;   \
	((mlib_u32 *)(sd))[2] = ((x##2 + 0x20002) >> 2) & 0xff00ff;   \
	((mlib_u32 *)(sd))[3] = ((x##3 + 0x20002) >> 2) & 0xff00ff

/* *********************************************************** */

#define	STORSUM(x, y)                                              \
	((mlib_u32 *)(sd))[0] = ((x##0 + y##0) >> 4) & 0xff00ff;   \
	((mlib_u32 *)(sd))[1] = ((x##1 + y##1) >> 4) & 0xff00ff;   \
	((mlib_u32 *)(sd))[2] = ((x##2 + y##2) >> 4) & 0xff00ff;   \
	((mlib_u32 *)(sd))[3] = ((x##3 + y##3) >> 4) & 0xff00ff

/* *********************************************************** */

mlib_status
__mlib_VideoP64Loop_S16_U8(
	mlib_s16 mc_block[64],
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	const mlib_u8 *sl;
	mlib_s16 *sd;
	mlib_u32 in0, in1, in2, in3, in4, in5, in6, in7;
	mlib_u32 a0, a1, a2, a3;
	mlib_u32 b0, b1, b2, b3;
	mlib_u32 c0, c1, c2, c3;
	mlib_u32 d0, d1;

	sd = mc_block;
	sl = ref_block;

	LOADL(a);
	sl += stride;
	FILTERX(a);

	LOADL(b);
	sl += stride;
	FILTERX(b);

	STORB(a);
	sd += 8;
	ADDL(a, b);

	LOADL(c);
	sl += stride;
	FILTERX(c);

	ADDLRND(b, c);
	STORSUM(a, b);
	sd += 8;

	LOADL(a);
	sl += stride;
	FILTERX(a);

	ADDL(c, a);
	STORSUM(b, c);
	sd += 8;

	LOADL(b);
	sl += stride;
	FILTERX(b);

	ADDLRND(a, b);
	STORSUM(c, a);
	sd += 8;

	LOADL(c);
	sl += stride;
	FILTERX(c);

	ADDL(b, c);
	STORSUM(a, b);
	sd += 8;

	LOADL(a);
	sl += stride;
	FILTERX(a);

	ADDLRND(c, a);
	STORSUM(b, c);
	sd += 8;

	LOADL(b);
	FILTERX(b);

	ADDL(a, b);
	STORSUM(c, a);
	sd += 8;

	STORB(b);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
