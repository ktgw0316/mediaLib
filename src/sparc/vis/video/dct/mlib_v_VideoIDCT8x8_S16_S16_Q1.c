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

#pragma ident	"@(#)mlib_v_VideoIDCT8x8_S16_S16_Q1.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_VideoIDCT8x8_S16_S16_Q1 - Computes the inverse IDCT
 *                                     in the inter mode
 *
 * SYNOPSIS
 *
 *
 *      mlib_status mlib_VideoIDCT8x8_S16_S16_Q1(mlib_s16       *block,
 *                                               const mlib_s16 *coeffs);
 *
 * ARGUMENT
 *      block           Pointer to an 8x8 motion-compensated block which is the
 *                      difference between the reference block and current block
 *      coeffs          Pointer to the input DCT coefficients
 *
 *      It is assumed that block and coeffs are 8-byte aligned
 *      Input DCT coeffs should be in S12 range or it should be obtained
 *      from the correspondent direct DCT.
 *
 * DESCRIPTION
 *   function should be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4
 *
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *             1   3   3                        pi*(2*x+1)*u      pi*(2*y+1)*v
 *   f(x, y) = ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *             4  u = 0 v = 0                      16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1 = __mlib_VideoIDCT8x8_S16_S16_Q1

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1) mlib_VideoIDCT8x8_S16_S16_Q1
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_Q1")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOAD_DATA_AA44                                          \
	p00 = dPtr[0];                                          \
	p10 = dPtr[2];                                          \
	p20 = dPtr[4];                                          \
	p30 = dPtr[6]

/* *********************************************************** */

#define	IDCT44(d0, d1, d2, d3)                                         \
	dx8 = vis_fmul8x16(COS_4_16, d3);                              \
	dx0 = vis_fmul8x16(COS_4_16, d1);                              \
	dx7 = vis_fpadd16(d1, dx8);                                    \
	dx3 = vis_fpsub16(dx0, d3);                                    \
	dx1 = vis_fpadd16(MULf(dx7, COS_1_16), MULf(dx8, COS_7_16));   \
	dx4 = vis_fpsub16(MULf(dx3, COS_1_16), MULf(dx0, COS_7_16));   \
	dx2 = vis_fpadd16(MULf(dx0, COS_1_16), MULf(dx3, COS_7_16));   \
	dx6 = vis_fpsub16(MULf(dx7, COS_7_16), MULf(dx8, COS_1_16));   \
	t0 = MULf(d2, COS_2_16);                                       \
	t1 = MULf(d2, COS_6_16);                                       \
	dx8 = vis_fmul8x16(COS_4_16, d0);                              \
	dx7 = vis_fpadd16(dx8, t0);                                    \
	dx0 = vis_fpsub16(dx8, t1);                                    \
	dx3 = vis_fpadd16(dx8, t1);                                    \
	dx8 = vis_fpsub16(dx8, t0)

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_d64 *dPtr = (mlib_d64 *)coeffs;
	mlib_d64 *outPtr = (mlib_d64 *)block;
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30;
	mlib_d64 p01, p11, p21, p31;
	mlib_d64 t0, t1;
	mlib_d64 d0, d1, d2, d3;

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_f32 C_1_4;

/*
 * First pass
 */

	LOAD_DATA_AA44;

	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];
	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	C_1_4 = ((mlib_f32 *)mlib_cTable)[5];

	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);

	IDCT44(d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p01 = vis_fpsub16(dx8, dx6);
	p11 = vis_fpsub16(dx0, dx4);
	p21 = vis_fpsub16(dx3, dx2);
	p31 = vis_fpsub16(dx7, dx1);

	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);

/*
 * Second pass
 */

	IDCT44(d0, d1, d2, d3);
	TRANSPOSE(p01, p11, p21, p31, d0, d1, d2, d3);
	outPtr[0] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
	outPtr[2] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
	outPtr[4] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
	outPtr[6] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
	outPtr[8] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
	outPtr[10] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
	outPtr[12] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
	outPtr[14] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

	IDCT44(d0, d1, d2, d3);
	outPtr[1] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
	outPtr[3] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
	outPtr[5] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
	outPtr[7] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
	outPtr[9] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
	outPtr[11] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
	outPtr[13] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
	outPtr[15] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
