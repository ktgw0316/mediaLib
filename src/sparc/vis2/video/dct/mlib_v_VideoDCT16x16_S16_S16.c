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

#pragma ident	"@(#)mlib_v_VideoDCT16x16_S16_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT16x16_S16_S16 - Computes the forward DCT
 *                                   in the inter mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDCT16x16_S16_S16(mlib_s16       *coeffs,
 *                                             const mlib_s16 *block);
 *
 * ARGUMENTS
 *      coeffs          Pointer to the output DCT coefficients
 *      block           Pointer to an 16x16
 *                      motion-compensated block which is the
 *                      difference between the reference block and
 *                      current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *    Input block values should be in [-128, 127] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,15; y = 0, 1, ... ,15
 *   F = F(u, v)  u = 0, 1, ... ,15; v = 0, 1, ... ,15
 *
 *             1             15  15             pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             8            x = 0 y = 0             32                  32
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT_VIS2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT16x16_S16_S16 = __mlib_VideoDCT16x16_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT16x16_S16_S16) mlib_VideoDCT16x16_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT16x16_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_PTRANSPOSE16_4x4(val_t, dind)                         \
	m02 = vis_fpmerge(vis_read_hi(p0), vis_read_hi(p2));       \
	m13 = vis_fpmerge(vis_read_hi(p1), vis_read_hi(p3));       \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));   \
	val_t[dind] =                                              \
	vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));       \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));   \
	val_t[dind + 4] =                                          \
	vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));       \
	m02 = vis_fpmerge(vis_read_lo(p0), vis_read_lo(p2));       \
	m13 = vis_fpmerge(vis_read_lo(p1), vis_read_lo(p3));       \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));   \
	val_t[dind + 8] =                                          \
	vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));       \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));   \
	val_t[dind + 12] =                                         \
	vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213))

#define	SCALE8(val)	vis_fmul8x16(fscale, val)

/* *********************************************************** */

#define	FCOS_4	COS_4
#define	FSIN_8	SIN_8
#define	FCOS_8	COS_8

/* *********************************************************** */

static const mlib_u32 mlib_dct16vtab[16] = {
	0xb5b5b5b5,
	0x62626262,
	0xecececec,
	0x32323232,
	0xfbfbfbfb,
	0xd5d5d5d5,
	0x8e8e8e8e,

	0x19191919,
	0xffffffff,
	0xf5f5f5f5,
	0x4a4a4a4a,
	0xe2e2e2e2,
	0x79797979,
	0xc6c6c6c6,
	0xa2a2a2a2,

	0x20202020,
};

/* *********************************************************** */

mlib_status
__mlib_VideoDCT16x16_S16_S16(
	mlib_s16 *coeffs,
	const mlib_s16 *block)
{
	mlib_s32 j;
	mlib_d64 val_m[16 * 4];
	mlib_d64 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9;
	mlib_d64 b10, b11, b12, b13, b14, b15;
	mlib_d64 t0, t1, t2, t3, t4, t5, t6, t7, t9;
	mlib_d64 t10, t11, t12, t13, t14;
	mlib_d64 m02, m13, m0213, p0, p1, p2, p3;

	mlib_d64 c1, c2, c3, c4;

	mlib_f32 COS_4 = ((mlib_f32 *)mlib_dct16vtab)[0];
	mlib_f32 SIN_8 = ((mlib_f32 *)mlib_dct16vtab)[1];
	mlib_f32 COS_8 = ((mlib_f32 *)mlib_dct16vtab)[2];
	mlib_f32 SIN_16 = ((mlib_f32 *)mlib_dct16vtab)[3];
	mlib_f32 COS_16 = ((mlib_f32 *)mlib_dct16vtab)[4];
	mlib_f32 COS_3_16 = ((mlib_f32 *)mlib_dct16vtab)[5];
	mlib_f32 SIN_3_16 = ((mlib_f32 *)mlib_dct16vtab)[6];

	mlib_f32 SIN_32 = ((mlib_f32 *)mlib_dct16vtab)[7];
	mlib_f32 COS_32 = ((mlib_f32 *)mlib_dct16vtab)[8];
	mlib_f32 COS_3_32 = ((mlib_f32 *)mlib_dct16vtab)[9];
	mlib_f32 SIN_3_32 = ((mlib_f32 *)mlib_dct16vtab)[10];
	mlib_f32 COS_5_32 = ((mlib_f32 *)mlib_dct16vtab)[11];
	mlib_f32 SIN_5_32 = ((mlib_f32 *)mlib_dct16vtab)[12];
	mlib_f32 COS_7_32 = ((mlib_f32 *)mlib_dct16vtab)[13];
	mlib_f32 SIN_7_32 = ((mlib_f32 *)mlib_dct16vtab)[14];

	mlib_f32 fscale = ((mlib_f32 *)mlib_dct16vtab)[15];

	mlib_d64 *bptr = (mlib_d64 *)block;
	mlib_d64 *coeffs64 = (mlib_d64 *)coeffs;

/*
 * first column based 1-D 16x16 DCT
 */

#pragma pipeloop(0)
	for (j = 0; j < 4; j++) {

/*
 * first butter-fly
 */
		b0 = vis_fpadd16(bptr[j], bptr[j + 4 * 15]);
		b15 = vis_fpsub16(bptr[j], bptr[j + 4 * 15]);

		b1 = vis_fpadd16(bptr[j + 4 * 1], bptr[j + 4 * 14]);
		b14 = vis_fpsub16(bptr[j + 4 * 1], bptr[j + 4 * 14]);

		b2 = vis_fpadd16(bptr[j + 4 * 2], bptr[j + 4 * 13]);
		b13 = vis_fpsub16(bptr[j + 4 * 2], bptr[j + 4 * 13]);

		b3 = vis_fpadd16(bptr[j + 4 * 3], bptr[j + 4 * 12]);
		b12 = vis_fpsub16(bptr[j + 4 * 3], bptr[j + 4 * 12]);

		b4 = vis_fpadd16(bptr[j + 4 * 4], bptr[j + 4 * 11]);
		b11 = vis_fpsub16(bptr[j + 4 * 4], bptr[j + 4 * 11]);

		b5 = vis_fpadd16(bptr[j + 4 * 5], bptr[j + 4 * 10]);
		b10 = vis_fpsub16(bptr[j + 4 * 5], bptr[j + 4 * 10]);

		b6 = vis_fpadd16(bptr[j + 4 * 6], bptr[j + 4 * 9]);
		b9 = vis_fpsub16(bptr[j + 4 * 6], bptr[j + 4 * 9]);

		b7 = vis_fpadd16(bptr[j + 4 * 7], bptr[j + 4 * 8]);
		b8 = vis_fpsub16(bptr[j + 4 * 7], bptr[j + 4 * 8]);

/*
 * second butter-fly
 */
		t0 = vis_fpadd16(b0, b7);
		t1 = vis_fpadd16(b1, b6);
		t2 = vis_fpadd16(b2, b5);
		t3 = vis_fpadd16(b3, b4);
		t4 = vis_fpsub16(b3, b4);
		t5 = vis_fpsub16(b2, b5);
		t6 = vis_fpsub16(b1, b6);
		t7 = vis_fpsub16(b0, b7);
		c1 = vis_fpsub16(b13, b10);
		c2 = vis_fpsub16(b12, b11);
		c3 = vis_fpadd16(b11, b12);
		c4 = vis_fpadd16(b10, b13);
		t10 = vis_fmul8x16(FCOS_4, c1);
		t11 = vis_fmul8x16(FCOS_4, c2);
		t12 = vis_fmul8x16(FCOS_4, c3);
		t13 = vis_fmul8x16(FCOS_4, c4);

/*
 * third butter-fly
 */
		b0 = vis_fpadd16(t0, t3);
		b1 = vis_fpadd16(t1, t2);
		b2 = vis_fpsub16(t1, t2);
		b3 = vis_fpsub16(t0, t3);
		c1 = vis_fpsub16(t6, t5);
		c2 = vis_fpadd16(t6, t5);
		b5 = vis_fmul8x16(FCOS_4, c1);
		b6 = vis_fmul8x16(FCOS_4, c2);
		b11 = vis_fpsub16(b8, t11);
		b8 = vis_fpadd16(b8, t11);
		b10 = vis_fpsub16(b9, t10);
		b9 = vis_fpadd16(b9, t10);
		b12 = vis_fpsub16(b15, t12);
		b13 = vis_fpsub16(b14, t13);
		b14 = vis_fpadd16(b14, t13);
		b15 = vis_fpadd16(b15, t12);

/*
 * fourth butter-fly
 */
		c1 = vis_fpadd16(b0, b1);
		c2 = vis_fpsub16(b0, b1);
		p0 = vis_fmul8x16(COS_4, c1);
		p2 = vis_fmul8x16(COS_4, c2);
		c1 = vis_fmul8x16(SIN_8, b2);
		c2 = vis_fmul8x16(COS_8, b3);
		c3 = vis_fmul8x16(SIN_8, b3);
		c4 = vis_fmul8x16(COS_8, b2);
		p1 = vis_fpadd16(c1, c2);
		p3 = vis_fpsub16(c3, c4);
		MLIB_PTRANSPOSE16_4x4(val_m, 16 * j);
		t5 = vis_fpsub16(t4, b5);
		t4 = vis_fpadd16(t4, b5);
		t6 = vis_fpsub16(t7, b6);
		t7 = vis_fpadd16(t7, b6);
		c1 = vis_fmul8x16(FSIN_8, b14);
		c2 = vis_fmul8x16(FCOS_8, b9);
		c3 = vis_fmul8x16(FSIN_8, b10);
		c4 = vis_fmul8x16(FCOS_8, b13);
		t9 = vis_fpsub16(c1, c2);
		t10 = vis_fpadd16(c3, c4);
		c1 = vis_fmul8x16(FSIN_8, b13);
		c2 = vis_fmul8x16(FCOS_8, b10);
		c3 = vis_fmul8x16(FSIN_8, b9);
		c4 = vis_fmul8x16(FCOS_8, b14);
		t13 = vis_fpsub16(c1, c2);
		t14 = vis_fpadd16(c3, c4);

/*
 * fifth butter-fly
 */
		c1 = vis_fmul8x16(SIN_16, t4);
		c2 = vis_fmul8x16(COS_16, t7);
		c3 = vis_fmul8x16(COS_3_16, t5);
		c4 = vis_fmul8x16(SIN_3_16, t6);
		p0 = vis_fpadd16(c1, c2);
		p2 = vis_fpadd16(c3, c4);
		c1 = vis_fmul8x16(COS_3_16, t6);
		c2 = vis_fmul8x16(SIN_3_16, t5);
		c3 = vis_fmul8x16(SIN_16, t7);
		c4 = vis_fmul8x16(COS_16, t4);
		p1 = vis_fpsub16(c1, c2);
		p3 = vis_fpsub16(c3, c4);
		MLIB_PTRANSPOSE16_4x4(val_m, 16 * j + 2);
		b9 = vis_fpsub16(b8, t9);
		b8 = vis_fpadd16(b8, t9);
		b10 = vis_fpadd16(b11, t10);
		b11 = vis_fpsub16(b11, t10);
		b13 = vis_fpsub16(b12, t13);
		b12 = vis_fpadd16(b12, t13);
		b14 = vis_fpsub16(b15, t14);
		b15 = vis_fpadd16(b15, t14);

/*
 * sixth butter-fly
 */
		c1 = vis_fmul8x16(SIN_32, b8);
		c2 = vis_fmul8x16(COS_32, b15);
		c3 = vis_fmul8x16(COS_7_32, b9);
		c4 = vis_fmul8x16(SIN_7_32, b14);
		p0 = vis_fpadd16(c1, c2);
		p2 = vis_fpadd16(c3, c4);

		c1 = vis_fmul8x16(SIN_5_32, b10);
		c2 = vis_fmul8x16(COS_5_32, b13);
		c3 = vis_fmul8x16(COS_3_32, b11);
		c4 = vis_fmul8x16(SIN_3_32, b12);
		p1 = vis_fpadd16(c1, c2);
		p3 = vis_fpadd16(c3, c4);
		MLIB_PTRANSPOSE16_4x4(val_m, 16 * j + 1);

		c1 = vis_fmul8x16(COS_3_32, b12);
		c2 = vis_fmul8x16(SIN_3_32, b11);
		c3 = vis_fmul8x16(SIN_5_32, b13);
		c4 = vis_fmul8x16(COS_5_32, b10);
		p0 = vis_fpsub16(c1, c2);
		p2 = vis_fpsub16(c3, c4);

		c1 = vis_fmul8x16(COS_7_32, b14);
		c2 = vis_fmul8x16(SIN_7_32, b9);
		c3 = vis_fmul8x16(SIN_32, b15);
		c4 = vis_fmul8x16(COS_32, b8);
		p1 = vis_fpsub16(c1, c2);
		p3 = vis_fpsub16(c3, c4);
		MLIB_PTRANSPOSE16_4x4(val_m, 16 * j + 3);
	}

/*
 * then row based 1-D 16x16 DCT
 */

	vis_write_bmask(0x018923ab, 0x0);

#pragma pipeloop(0)
	for (j = 0; j < 4; j++) {

/*
 * first butter-fly
 */
		b0 = vis_fpadd16(val_m[j], val_m[j + 4 * 15]);
		b15 = vis_fpsub16(val_m[j], val_m[j + 4 * 15]);

		b1 = vis_fpadd16(val_m[j + 4 * 1], val_m[j + 4 * 14]);
		b14 = vis_fpsub16(val_m[j + 4 * 1], val_m[j + 4 * 14]);

		b2 = vis_fpadd16(val_m[j + 4 * 2], val_m[j + 4 * 13]);
		b13 = vis_fpsub16(val_m[j + 4 * 2], val_m[j + 4 * 13]);

		b3 = vis_fpadd16(val_m[j + 4 * 3], val_m[j + 4 * 12]);
		b12 = vis_fpsub16(val_m[j + 4 * 3], val_m[j + 4 * 12]);

		b4 = vis_fpadd16(val_m[j + 4 * 4], val_m[j + 4 * 11]);
		b11 = vis_fpsub16(val_m[j + 4 * 4], val_m[j + 4 * 11]);

		b5 = vis_fpadd16(val_m[j + 4 * 5], val_m[j + 4 * 10]);
		b10 = vis_fpsub16(val_m[j + 4 * 5], val_m[j + 4 * 10]);

		b6 = vis_fpadd16(val_m[j + 4 * 6], val_m[j + 4 * 9]);
		b9 = vis_fpsub16(val_m[j + 4 * 6], val_m[j + 4 * 9]);

		b7 = vis_fpadd16(val_m[j + 4 * 7], val_m[j + 4 * 8]);
		b8 = vis_fpsub16(val_m[j + 4 * 7], val_m[j + 4 * 8]);

/*
 * second butter-fly
 */
		t0 = vis_fpadd16(b0, b7);
		t1 = vis_fpadd16(b1, b6);
		t2 = vis_fpadd16(b2, b5);
		t3 = vis_fpadd16(b3, b4);
		t4 = vis_fpsub16(b3, b4);
		t5 = vis_fpsub16(b2, b5);
		t6 = vis_fpsub16(b1, b6);
		t7 = vis_fpsub16(b0, b7);
		c1 = vis_fpsub16(b13, b10);
		c2 = vis_fpsub16(b12, b11);
		c3 = vis_fpadd16(b11, b12);
		c4 = vis_fpadd16(b10, b13);
		t10 = vis_fmul8x16(FCOS_4, c1);
		t11 = vis_fmul8x16(FCOS_4, c2);
		t12 = vis_fmul8x16(FCOS_4, c3);
		t13 = vis_fmul8x16(FCOS_4, c4);

/*
 * third butter-fly
 */
		b0 = vis_fpadd16(t0, t3);
		b1 = vis_fpadd16(t1, t2);
		b2 = vis_fpsub16(t1, t2);
		b3 = vis_fpsub16(t0, t3);
		c1 = vis_fpsub16(t6, t5);
		c2 = vis_fpadd16(t6, t5);
		b5 = vis_fmul8x16(FCOS_4, c1);
		b6 = vis_fmul8x16(FCOS_4, c2);
		b11 = vis_fpsub16(b8, t11);
		b8 = vis_fpadd16(b8, t11);
		b10 = vis_fpsub16(b9, t10);
		b9 = vis_fpadd16(b9, t10);
		b12 = vis_fpsub16(b15, t12);
		b13 = vis_fpsub16(b14, t13);
		b14 = vis_fpadd16(b14, t13);
		b15 = vis_fpadd16(b15, t12);

/*
 * fourth butter-fly
 */
		b0 = vis_fmul8x16(COS_4, b0);
		b1 = vis_fmul8x16(COS_4, b1);
		c1 = vis_fpadd16(b0, b1);
		c2 = vis_fpsub16(b0, b1);
		b0 = SCALE8(c1);
		b1 = SCALE8(c2);
		c1 = vis_fmul8x16(SIN_8, b2);
		c2 = vis_fmul8x16(COS_8, b3);
		c3 = vis_fmul8x16(SIN_8, b3);
		c4 = vis_fmul8x16(COS_8, b2);
		b2 = SCALE8(vis_fpadd16(c1, c2));
		b3 = SCALE8(vis_fpsub16(c3, c4));
		t5 = vis_fpsub16(t4, b5);
		t4 = vis_fpadd16(t4, b5);
		t6 = vis_fpsub16(t7, b6);
		t7 = vis_fpadd16(t7, b6);
		c1 = vis_fmul8x16(FSIN_8, b14);
		c2 = vis_fmul8x16(FCOS_8, b9);
		c3 = vis_fmul8x16(FSIN_8, b10);
		c4 = vis_fmul8x16(FCOS_8, b13);
		t9 = vis_fpsub16(c1, c2);
		t10 = vis_fpadd16(c3, c4);
		c1 = vis_fmul8x16(FSIN_8, b13);
		c2 = vis_fmul8x16(FCOS_8, b10);
		c3 = vis_fmul8x16(FSIN_8, b9);
		c4 = vis_fmul8x16(FCOS_8, b14);
		t13 = vis_fpsub16(c1, c2);
		t14 = vis_fpadd16(c3, c4);

/*
 * fifth butter-fly
 */
		c1 = vis_fmul8x16(COS_3_16, t6);
		c2 = vis_fmul8x16(SIN_3_16, t5);
		c3 = vis_fmul8x16(COS_3_16, t5);
		c4 = vis_fmul8x16(SIN_3_16, t6);
		b6 = SCALE8(vis_fpsub16(c1, c2));
		b5 = SCALE8(vis_fpadd16(c3, c4));
		b9 = vis_fpsub16(b8, t9);
		b8 = vis_fpadd16(b8, t9);
		b10 = vis_fpadd16(b11, t10);
		b11 = vis_fpsub16(b11, t10);
		b13 = vis_fpsub16(b12, t13);
		b12 = vis_fpadd16(b12, t13);
		b14 = vis_fpsub16(b15, t14);
		b15 = vis_fpadd16(b15, t14);

/*
 * sixth butter-fly
 */
		c1 = vis_fmul8x16(COS_7_32, b14);
		c2 = vis_fmul8x16(SIN_7_32, b9);
		c3 = vis_fmul8x16(COS_7_32, b9);
		c4 = vis_fmul8x16(SIN_7_32, b14);
		b14 = SCALE8(vis_fpsub16(c1, c2));
		b9 = SCALE8(vis_fpadd16(c3, c4));

		c1 = vis_fmul8x16(SIN_5_32, b10);
		c2 = vis_fmul8x16(COS_5_32, b13);
		c3 = vis_fmul8x16(SIN_5_32, b13);
		c4 = vis_fmul8x16(COS_5_32, b10);
		b10 = SCALE8(vis_fpadd16(c1, c2));
		b13 = SCALE8(vis_fpsub16(c3, c4));

		TRANSPOSE_VIS2(b1, b9, b5, b13, coeffs64[4 * j + 2],
			coeffs64[4 * j + 2 + 16], coeffs64[4 * j + 2 + 32],
			coeffs64[4 * j + 2 + 48]);
		TRANSPOSE_VIS2(b2, b10, b6, b14, coeffs64[4 * j + 1],
			coeffs64[4 * j + 1 + 16], coeffs64[4 * j + 1 + 32],
			coeffs64[4 * j + 1 + 48]);

		c1 = vis_fmul8x16(SIN_16, t4);
		c2 = vis_fmul8x16(COS_16, t7);
		c3 = vis_fmul8x16(SIN_16, t7);
		c4 = vis_fmul8x16(COS_16, t4);
		b4 = SCALE8(vis_fpadd16(c1, c2));
		b7 = SCALE8(vis_fpsub16(c3, c4));

		c1 = vis_fmul8x16(SIN_32, b8);
		c2 = vis_fmul8x16(COS_32, b15);
		c3 = vis_fmul8x16(SIN_32, b15);
		c4 = vis_fmul8x16(COS_32, b8);
		b8 = SCALE8(vis_fpadd16(c1, c2));
		b15 = SCALE8(vis_fpsub16(c3, c4));

		c1 = vis_fmul8x16(COS_3_32, b12);
		c2 = vis_fmul8x16(SIN_3_32, b11);
		c3 = vis_fmul8x16(COS_3_32, b11);
		c4 = vis_fmul8x16(SIN_3_32, b12);
		b12 = SCALE8(vis_fpsub16(c1, c2));
		b11 = SCALE8(vis_fpadd16(c3, c4));

		TRANSPOSE_VIS2(b0, b8, b4, b12, coeffs64[4 * j],
			coeffs64[4 * j + 16], coeffs64[4 * j + 32],
			coeffs64[4 * j + 48]);
		TRANSPOSE_VIS2(b3, b11, b7, b15, coeffs64[4 * j + 3],
			coeffs64[4 * j + 3 + 16], coeffs64[4 * j + 3 + 32],
			coeffs64[4 * j + 3 + 48]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
