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

#pragma ident	"@(#)mlib_v_VideoIDCT8x8_S16_S16_QM.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *    mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch - Computes the inverse IDCT
 *                                            in the inter mode
 *
 * SYNOPSIS
 *
 *    mlib_status mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(mlib_s16       *block,
 *                                                      const mlib_s16 *coeffs)
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
 *   function is similar to mlib_v_VideoIDCT8x8_S16_S16_Q1: it should only
 *   be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4. However,
 *   this function also allows element coeff[7][7]
 *   to be non-zero. The primary benefit of
 *   this modification is that it can handle
 *   situations where coeffs[7][7] has been
 *   made non-zero by MPEG mismatch-control, allowing a
 *   simplified version of the IDCT to be undertaken
 *   for a much larger number of situations.
 */

#include <vis_proto.h>
#include <mlib_video.h>
#include <mlib_v_VideoDCT.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch = \
	__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch)
	mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch
	__attribute__((weak,
	alias("__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u64 val0 = 0x0000ffff0001ffffULL;
static const mlib_u64 val1 = 0x0001ffff00010000ULL;

/* *********************************************************** */

#define	LOAD_DATA_AA44                                          \
	p00 = dPtr[0];                                          \
	p10 = dPtr[2];                                          \
	p20 = dPtr[4];                                          \
	p30 = dPtr[6]

/* *********************************************************** */

#define	LOAD_MISMATCH	d7 = dPtr[15]

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

#define	IDCTS(zero, d7)                                                \
	dx0 = vis_fmul8x16(COS_4_16, d7);                              \
	dx6 = vis_fmul8x16(COS_4_16, vis_fpsub16(zero, d7));           \
	dx1 = MULf(d7, COS_7_16);                                      \
	dx4 = vis_fpsub16(MULf(dx0, COS_1_16), MULf(dx6, COS_7_16));   \
	dx2 = vis_fpadd16(MULf(dx6, COS_1_16), MULf(dx0, COS_7_16));   \
	dx6 = vis_fpsub16(zero, MULf(d7, COS_1_16))

/* *********************************************************** */

#define	IDCTS1(d0, d1, d2, d3, d7)                                     \
	dx2 = vis_fmul8x16(COS_4_16, d3);                              \
	dx4 = dx2;                                                     \
	dx1 = vis_fmul8x16(COS_4_16, vis_fpadd16(d1, d7));             \
	dx6 = vis_fmul8x16(COS_4_16, vis_fpsub16(d1, d7));             \
	dx7 = vis_fpadd16(d1, dx2);                                    \
	dx8 = vis_fpadd16(d7, dx4);                                    \
	dx3 = vis_fpsub16(dx1, d3);                                    \
	dx0 = dx6;                                                     \
	dx1 = vis_fpadd16(MULf(dx7, COS_1_16), MULf(dx8, COS_7_16));   \
	dx4 = vis_fpsub16(MULf(dx3, COS_1_16), MULf(dx0, COS_7_16));   \
	dx2 = vis_fpadd16(MULf(dx0, COS_1_16), MULf(dx3, COS_7_16));   \
	dx6 = vis_fpsub16(MULf(dx7, COS_7_16), MULf(dx8, COS_1_16));   \
	t0 = MULf(d2, COS_2_16);                                       \
	t1 = MULf(d2, COS_6_16);                                       \
	dx3 = vis_fmul8x16(COS_4_16, d0);                              \
	dx8 = vis_fmul8x16(COS_4_16, d0);                              \
	dx7 = vis_fpadd16(dx8, t0);                                    \
	dx0 = vis_fpsub16(dx3, t1);                                    \
	dx3 = vis_fpadd16(dx3, t1);                                    \
	dx8 = vis_fpsub16(dx8, t0)

/* *********************************************************** */

#define	TRANSPOSE1(p0, p1, p2, p3, r3)                                  \
	{                                                               \
	    mlib_d64 m02, m13, m0213;                                   \
	                                                                \
	    m02 = vis_fpmerge(vis_read_lo(p0), vis_read_lo(p2));        \
	    m13 = vis_fpmerge(vis_read_lo(p1), vis_read_lo(p3));        \
	    m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	    r3 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_d64 *dPtr = (mlib_d64 *)coeffs;
	mlib_d64 *outPtr = (mlib_d64 *)block;
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30, p01, p11, p21, p31, p40, p50, p60, p70;
	mlib_d64 p41, p51, p61, p71;
	mlib_d64 t0, t1;
	mlib_d64 d0, d1, d2, d3, d7, zero = vis_fzero();

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_f32 C_1_4;

/* First pass */

	LOAD_DATA_AA44;

	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];
	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	C_1_4 = ((mlib_f32 *)mlib_cTable)[5];

	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3)

		LOAD_MISMATCH;

	IDCT44(d0, d1, d2, d3);

	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p40 = vis_fpsub16(dx8, dx6);
	p50 = vis_fpsub16(dx0, dx4);
	p60 = vis_fpsub16(dx3, dx2);
	p70 = vis_fpsub16(dx7, dx1);

/* Special case when element#63 == 1 */

	if (coeffs[63] != 1) {
		IDCTS(zero, d7);
		p01 = dx1;
		p11 = dx2;
		p21 = dx4;
		p31 = dx6;
		p41 = vis_fpsub16(zero, dx6);
		p51 = vis_fpsub16(zero, dx4);
		p61 = vis_fpsub16(zero, dx2);
		p71 = vis_fpsub16(zero, dx1);

		TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3)
			TRANSPOSE1(p01, p11, p21, p31, d7)

/* Second pass */
			IDCTS1(d0, d1, d2, d3, d7);
		TRANSPOSE(p40, p50, p60, p70, d0, d1, d2, d3)
			outPtr[0] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
		outPtr[2] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
		outPtr[4] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
		outPtr[6] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
		outPtr[8] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
		outPtr[10] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
		outPtr[12] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
		outPtr[14] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

		TRANSPOSE1(p41, p51, p61, p71, d7)
			IDCTS1(d0, d1, d2, d3, d7);
		outPtr[1] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
		outPtr[3] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
		outPtr[5] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
		outPtr[7] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
		outPtr[9] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
		outPtr[11] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
		outPtr[13] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
		outPtr[15] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

		return (MLIB_SUCCESS);
	} else {
/* Second pass */
		TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3)
			d7 = *((mlib_d64 *)&val0);

		IDCTS1(d0, d1, d2, d3, d7);
		TRANSPOSE(p40, p50, p60, p70, d0, d1, d2, d3)
			outPtr[0] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
		outPtr[2] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
		outPtr[4] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
		outPtr[6] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
		outPtr[8] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
		outPtr[10] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
		outPtr[12] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
		outPtr[14] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

		d7 = *((mlib_d64 *)&val1);
		IDCTS1(d0, d1, d2, d3, d7);
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
}

/* *********************************************************** */
