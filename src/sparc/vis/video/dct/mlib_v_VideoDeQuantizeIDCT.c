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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_VideoDeQuantizeIDCT.c	9.4	07/11/05 SMI"


/*
 * FUNCTIONS
 * mlib_VideoDeQuantizeIDCT8x8_U8_S16[_NA] - performs Dequantization and
 *				computes the inverse DCT in Intra mode.
 *
 * mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA]- performs Dequantization
 *				and computes the inverse DCT in the
 *				inter mode.
 *
 * SYNOPSIS
 *      mlib_status  mlib_VideoDeQuantizeIDCT8x8_U8_S16[_NA](
 *      			mlib_u8	*block,
 *      			const mlib_s16 coeffs[64],
 *      			const mlib_d64 dqtable[64],
 *      			mlib_s32       stride)
 *
 *      mlib_status  mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA](
 *      			mlib_s16       *block,
 *      			const mlib_s16 coeffs[64],
 *      			const mlib_d64 dqtable[64])
 *
 * ARGUMENTS
 *      block        Pointer to 8x8 block in the current frame or
 *                   motion-compensated reference block.
 *      coeffs       Pointer to input quantized DCT coefficients.
 *      dqtable	     Pointer to Dequantization table.
 *      stride       Stride in bytes between adjacent rows in a block,
 *			and the stride applies to the block which is the
 *			part of the current frame being reconstructed,in
 *			case of an intra mode.
 *
 * RESTRICTIONS
 *	The block and the coeffs and the dqtable must be 8-byte aligned
 *	mlib_VideoDeQuantizeIDCT8x8_U8_S16
 *	and mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12.
 *	the stride must be a multiple of 8 for
 *	mlib_VideoDeQuantizeIDCT8x8_U8_S16.
 *
 *	The output of the mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA]
 *	should be within range [-2048, 2047] if coeffs is obtained from
 *	correspondent direct DCT8x8Quantize. This function can be used
 *	in JPEG with the 12 bits sample precision. For MPEG, the output
 *	which is really the difference between the current block and
 *	the reference block can occupy 9 bits, and is also represented
 *	in 16-bit data.
 *	The output needs to be added to the motion-compensated
 *	reference block to reconstruct the current block.
 *
 *	dqtable should be the output of mlib_VideDeQuantizeInit_S16.
 *
 * DESCRIPTION
 *
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *   Dequantization algorithm:
 *
 *   F(u,v) = F(u,v) * dqtable(u,v);
 *
 *  IDCT algorithm:
 *   f(x, y) =
 *    1   7   7                       pi*(2*x+1)*u      pi*(2*y+1)*v
 *   ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *    4  u=0 v=0                           16                16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT.h>
#include <mlib_v_VideoDCTHybrid.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12 =\
			__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12
#pragma weak mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA =\
			__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA
#pragma weak mlib_VideoDeQuantizeIDCT8x8_U8_S16 =\
			__mlib_VideoDeQuantizeIDCT8x8_U8_S16
#pragma weak mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA =\
			__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12)
	mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA)
	mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_U8_S16)
	mlib_VideoDeQuantizeIDCT8x8_U8_S16
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_U8_S16")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA)
	mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_U8_S16(
	mlib_u8 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64],
	mlib_s32 stride)
{
	mlib_d64 *dPtr = (mlib_d64 *)coeffs;
	mlib_d64 *dPtr1 = (mlib_d64 *)dqtable;
	mlib_d64 *outPtr = (mlib_d64 *)block;
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30, p40, p50, p60, p70;
	mlib_d64 p01, p11, p21, p31, p41, p51, p61, p71;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 t0, t1;
	mlib_s32 str;

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_d64 rounder;

	vis_write_gsr(15 << 3);
/*
 * First pass
 */
	LOAD_AA1_DEQUANT;
	LOAD_AA2_DEQUANT;

	vis_write_gsr(5 << 3);
	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];
	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	rounder = ((mlib_d64 *)mlib_cTable)[3];

	p00 = vis_fpadd16(p00, rounder);
	IDCT(p00, p10, p20, p30, p40, p50, p60, p70);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p40 = vis_fpsub16(dx8, dx6);
	p50 = vis_fpsub16(dx0, dx4);
	p60 = vis_fpsub16(dx3, dx2);
	p70 = vis_fpsub16(dx7, dx1);

	IDCT(p01, p11, p21, p31, p41, p51, p61, p71);
	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);
	p01 = vis_fpadd16(dx7, dx1);
	p11 = vis_fpadd16(dx3, dx2);
	p21 = vis_fpadd16(dx0, dx4);
	p31 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

/*
 * Second pass
 */

	str = stride >> 3;

	TRANSPOSE(p01, p11, p21, p31, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE(p40, p50, p60, p70, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p01 = vis_fpsub16(dx8, dx6);
	p11 = vis_fpsub16(dx0, dx4);
	p21 = vis_fpsub16(dx3, dx2);
	p31 = vis_fpsub16(dx7, dx1);

	TRANSPOSE(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPACK_8X4(p00, p10, p20, p30,
		p01, p11, p21, p31,
		outPtr[0], outPtr[str], outPtr[str * 2], outPtr[str * 3]);

	outPtr += 4 * str;

	p40 = vis_fpadd16(dx7, dx1);
	p50 = vis_fpadd16(dx3, dx2);
	p60 = vis_fpadd16(dx0, dx4);
	p70 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

	TRANSPACK_8X4(p40, p50, p60, p70,
		p41, p51, p61, p71,
		outPtr[0], outPtr[str], outPtr[str * 2], outPtr[str * 3]);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA(
	mlib_u8 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64],
	mlib_s32 stride)
{
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30, p40, p50, p60, p70;
	mlib_d64 p01, p11, p21, p31, p41, p51, p61, p71;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 t0, t1;

	mlib_u8 *pdst = block;
	mlib_d64 *dpdst;
	mlib_s32 emask;

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_d64 rounder;

	if (coeffs == NULL || block == NULL)
		return (MLIB_NULLPOINTER);

	if (!(((mlib_addr)block | (mlib_addr)coeffs | stride) & 7 |
				(mlib_addr)dqtable & 7))
		return (__mlib_VideoDeQuantizeIDCT8x8_U8_S16(block,
					coeffs, dqtable, stride));


/*
 * First pass
 */
	mlib_d64 *dPtr = (mlib_d64 *)vis_alignaddr((void *)coeffs, 0);
	LOAD_DATA_GE1;
	LOAD_DATA_GE2;

	vis_write_gsr(15 << 3);
	mlib_d64 *dPtr1 = (mlib_d64 *)vis_alignaddr((void *)dqtable, 0);
	LOAD_DEQUANT_GE1;
	LOAD_DEQUANT_GE2;

	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];
	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];

	vis_write_gsr(5 << 3);

	rounder = ((mlib_d64 *)mlib_cTable)[3];
	p00 = vis_fpadd16(p00, rounder);

	IDCT(p00, p10, p20, p30, p40, p50, p60, p70);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p40 = vis_fpsub16(dx8, dx6);
	p50 = vis_fpsub16(dx0, dx4);
	p60 = vis_fpsub16(dx3, dx2);
	p70 = vis_fpsub16(dx7, dx1);

	IDCT(p01, p11, p21, p31, p41, p51, p61, p71);
	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);
	p01 = vis_fpadd16(dx7, dx1);
	p11 = vis_fpadd16(dx3, dx2);
	p21 = vis_fpadd16(dx0, dx4);
	p31 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

/*
 * Second pass
 */

	TRANSPOSE(p01, p11, p21, p31, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE(p40, p50, p60, p70, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p01 = vis_fpsub16(dx8, dx6);
	p11 = vis_fpsub16(dx0, dx4);
	p21 = vis_fpsub16(dx3, dx2);
	p31 = vis_fpsub16(dx7, dx1);

	TRANSPACK_8X4(p00, p10, p20, p30,
		p01, p11, p21, p31, p00, p10, p20, p30);

	STORE_DATA_GE(p00);
	pdst += stride;
	STORE_DATA_GE(p10);
	pdst += stride;
	STORE_DATA_GE(p20);
	pdst += stride;
	STORE_DATA_GE(p30);
	pdst += stride;

	TRANSPOSE(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);

	p40 = vis_fpadd16(dx7, dx1);
	p50 = vis_fpadd16(dx3, dx2);
	p60 = vis_fpadd16(dx0, dx4);
	p70 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

	TRANSPACK_8X4(p40, p50, p60, p70,
		p41, p51, p61, p71, p40, p50, p60, p70);

	STORE_DATA_GE(p40);
	pdst += stride;
	STORE_DATA_GE(p50);
	pdst += stride;
	STORE_DATA_GE(p60);
	pdst += stride;
	if ((mlib_addr)pdst & 7) {
		STORE_DATA_GE(p70);
	} else
		((mlib_d64 *)pdst)[0] = p70;

	return (MLIB_SUCCESS);

}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12(
	mlib_s16 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_d64 *dPtr = (mlib_d64 *)coeffs;
	mlib_d64 *dPtr1 = (mlib_d64 *)dqtable;
	mlib_d64 *outPtr = (mlib_d64 *)block;
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30, p40, p50, p60, p70;
	mlib_d64 p01, p11, p21, p31, p41, p51, p61, p71;
	mlib_d64 t0, t1;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;
	mlib_d64 int_FP_transfer[16];
	mlib_u64 I0, I1, I2, I3, I4, I5, I6, I7, MASK;
	mlib_u64 T0, T1, T2, T3, T4, T5, T6, T7;

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_f32 C_1_4;

	MASK = 0xffff0000ffff0000ULL;

/*
 * First pass
 */

	vis_write_gsr(15 << 3);

	LOAD_AA1_DEQUANT;
	LOAD_AA2_DEQUANT;

	LOAD_INT_FP_VAR;


	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];
	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	C_1_4 = ((mlib_f32 *)mlib_cTable)[5];

	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);
	INT_TRANSPOSE(I0, I1, I2, I3, T0, T1, T2, T3);


	INT_STORE_0;
	TRANSPOSE(p01, p11, p21, p31, d4, d5, d6, d7);
	INT_TRANSPOSE(I4, I5, I6, I7, T4, T5, T6, T7);

	INT_STORE_1;

	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);

	LOAD_INT_FP_0;

	int_FP_transfer[0] = vis_fpadd16(dx7, dx1);
	int_FP_transfer[2] = vis_fpadd16(dx3, dx2);
	int_FP_transfer[4] = vis_fpadd16(dx0, dx4);
	int_FP_transfer[6] = vis_fpadd16(dx8, dx6);
	int_FP_transfer[8] = vis_fpsub16(dx8, dx6);
	int_FP_transfer[10] = vis_fpsub16(dx0, dx4);
	int_FP_transfer[12] = vis_fpsub16(dx3, dx2);
	int_FP_transfer[14] = vis_fpsub16(dx7, dx1);

	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);

	LOAD_FP_INT_0;

	INT_TRANSPOSE(I0, I1, I2, I3, T0, T1, T2, T3);

	INT_STORE_2;

	p40 = vis_fpadd16(dx7, dx1);
	p50 = vis_fpadd16(dx3, dx2);
	p60 = vis_fpadd16(dx0, dx4);
	p70 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

	LOAD_INT_FP_1;

/*
 * Second pass
 */

	TRANSPOSE(p40, p50, p60, p70, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	INT_TRANSPOSE(I4, I5, I6, I7, T4, T5, T6, T7);

	INT_STORE_3;

	outPtr[0] = vis_fmul8x16(C_1_4, vis_fpadd16(dx7, dx1));
	outPtr[2] = vis_fmul8x16(C_1_4, vis_fpadd16(dx3, dx2));
	outPtr[4] = vis_fmul8x16(C_1_4, vis_fpadd16(dx0, dx4));
	outPtr[6] = vis_fmul8x16(C_1_4, vis_fpadd16(dx8, dx6));
	outPtr[8] = vis_fmul8x16(C_1_4, vis_fpsub16(dx8, dx6));
	outPtr[10] = vis_fmul8x16(C_1_4, vis_fpsub16(dx0, dx4));
	outPtr[12] = vis_fmul8x16(C_1_4, vis_fpsub16(dx3, dx2));
	outPtr[14] = vis_fmul8x16(C_1_4, vis_fpsub16(dx7, dx1));

	LOAD_INT_FP_2;

	TRANSPOSE(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
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

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA(
	mlib_s16 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_d64 *outPtr;
	mlib_d64 dx0, dx1, dx2, dx3, dx4, dx6, dx7, dx8;
	mlib_d64 p00, p10, p20, p30, p40, p50, p60, p70;
	mlib_d64 p01, p11, p21, p31, p41, p51, p61, p71;
	mlib_d64 t0, t1;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 mask;

	mlib_f32 COS_1_16;
	mlib_f32 COS_2_16;
	mlib_f32 COS_6_16;
	mlib_f32 COS_7_16;
	mlib_f32 COS_4_16;
	mlib_f32 C_1_4;

	if (coeffs == NULL || block == NULL)
		return (MLIB_NULLPOINTER);

	if (!(((mlib_addr)block | (mlib_addr)coeffs) & 7 |
				(mlib_addr) dqtable & 7))
		return (__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12(
					block, coeffs, dqtable));

/*
 * First pass
 */

	mlib_d64 *dPtr = (mlib_d64 *)vis_alignaddr((void *)coeffs, 0);
	LOAD_DATA_GE1;
	LOAD_DATA_GE2;

	vis_write_gsr(15 << 3);
	mlib_d64 *dPtr1 = (mlib_d64 *)vis_alignaddr((void *)dqtable, 0);
	LOAD_DEQUANT_GE1;
	LOAD_DEQUANT_GE2;

	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];

	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	C_1_4 = ((mlib_f32 *)mlib_cTable)[5];

	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);
	TRANSPOSE(p01, p11, p21, p31, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);

	TRANSPOSE(p40, p50, p60, p70, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);

	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p01 = vis_fpsub16(dx8, dx6);
	p11 = vis_fpsub16(dx0, dx4);
	p21 = vis_fpsub16(dx3, dx2);
	p31 = vis_fpsub16(dx7, dx1);

	TRANSPOSE(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE(p00, p10, p20, p30, d0, d1, d2, d3);
	p40 = vis_fpadd16(dx7, dx1);
	p50 = vis_fpadd16(dx3, dx2);
	p60 = vis_fpadd16(dx0, dx4);
	p70 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

/*
 * Second pass
 */

	outPtr = (mlib_d64 *)vis_alignaddr(block, -1);
	mask = 0xFF >> ((mlib_addr)block - (mlib_addr)outPtr);
	vis_alignaddrl((void *)block, 0);

	TRANSPOSE(p40, p50, p60, p70, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE(p01, p11, p21, p31, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p40 = vis_fpsub16(dx8, dx6);
	p50 = vis_fpsub16(dx0, dx4);
	p60 = vis_fpsub16(dx3, dx2);
	p70 = vis_fpsub16(dx7, dx1);

	TRANSPOSE(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	if ((mlib_addr)block & 7)
		vis_pst_8(vis_fmul8x16(C_1_4, vis_faligndata(p00, p00)),
			outPtr, mask);
	p01 = vis_fpadd16(dx7, dx1);
	p11 = vis_fpadd16(dx3, dx2);
	p21 = vis_fpadd16(dx0, dx4);
	p31 = vis_fpadd16(dx8, dx6);
	p41 = vis_fpsub16(dx8, dx6);
	p51 = vis_fpsub16(dx0, dx4);
	p61 = vis_fpsub16(dx3, dx2);
	p71 = vis_fpsub16(dx7, dx1);

	outPtr[1] = vis_fmul8x16(C_1_4, vis_faligndata(p00, p01));
	outPtr[2] = vis_fmul8x16(C_1_4, vis_faligndata(p01, p10));
	outPtr[3] = vis_fmul8x16(C_1_4, vis_faligndata(p10, p11));
	outPtr[4] = vis_fmul8x16(C_1_4, vis_faligndata(p11, p20));
	outPtr[5] = vis_fmul8x16(C_1_4, vis_faligndata(p20, p21));
	outPtr[6] = vis_fmul8x16(C_1_4, vis_faligndata(p21, p30));
	outPtr[7] = vis_fmul8x16(C_1_4, vis_faligndata(p30, p31));
	outPtr[8] = vis_fmul8x16(C_1_4, vis_faligndata(p31, p40));
	outPtr[9] = vis_fmul8x16(C_1_4, vis_faligndata(p40, p41));
	outPtr[10] = vis_fmul8x16(C_1_4, vis_faligndata(p41, p50));
	outPtr[11] = vis_fmul8x16(C_1_4, vis_faligndata(p50, p51));
	outPtr[12] = vis_fmul8x16(C_1_4, vis_faligndata(p51, p60));
	outPtr[13] = vis_fmul8x16(C_1_4, vis_faligndata(p60, p61));
	outPtr[14] = vis_fmul8x16(C_1_4, vis_faligndata(p61, p70));
	outPtr[15] = vis_fmul8x16(C_1_4, vis_faligndata(p70, p71));
	vis_pst_8(vis_fmul8x16(C_1_4, vis_faligndata(p71, p71)), outPtr + 16,
		~mask);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
