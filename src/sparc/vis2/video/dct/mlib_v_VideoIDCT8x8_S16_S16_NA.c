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

#pragma ident	"@(#)mlib_v_VideoIDCT8x8_S16_S16_NA.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoIDCT8x8_S16_S16_B12_NA - computes the inverse DCT
 *      in the inter mode.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoIDCT8x8_S16_S16_B12(mlib_s16       *block,
 *                                                const mlib_s16 *coeffs);
 *
 * ARGUMENTS
 *      block        Pointer to motion-compensated reference block.
 *      coeffs       Pointer to input DCT coefficients.
 *
 * RESTRICTIONS
 *	The output of the mlib_VideoIDCT8x8_S16_S16_B12_NA
 *	should be within range [-2048, 2047] if coeffs is obtained from
 *	correspondent direct DCT. This function can be used
 *	in JPEG with the 12 bits sample precision. For MPEG, the output
 *	which is really the difference between the current block and
 *	the reference block can occupy 9 bits, and is also represented
 *	in 16-bit data.
 *	The output needs to be added to the motion-compensated
 *	reference block to reconstruct the current block.
 *
 * DESCRIPTION
 *
 *   F = F(u, v)  u = 0, 1, ..., 7; v = 0, 1, ..., 7
 *   f = f(x, y)  x = 0, 1, ..., 7; y = 0, 1, ..., 7
 *
 *   f(x,y) =
 *     1   7   7                       pi*(2*x+1)*u      pi*(2*y+1)*v
 *    ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *     4  u=0 v=0                           16                16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <vis_proto.h>
#include <mlib_video.h>
#include <mlib_v_VideoDCT.h>
#include <mlib_v_VideoDCT_VIS2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_B12_NA = \
		__mlib_VideoIDCT8x8_S16_S16_B12_NA
#pragma weak mlib_VideoIDCT8x8_S16_S16_NA = __mlib_VideoIDCT8x8_S16_S16_B12_NA
#pragma weak __mlib_VideoIDCT8x8_S16_S16_NA = __mlib_VideoIDCT8x8_S16_S16_B12_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_B12_NA) mlib_VideoIDCT8x8_S16_S16_B12_NA
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_B12_NA")));
__typeof__(__mlib_VideoIDCT8x8_S16_S16_NA) mlib_VideoIDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_B12_NA")));
__typeof__(__mlib_VideoIDCT8x8_S16_S16_NA) __mlib_VideoIDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_B12_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_B12_NA(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_d64 *dPtr = (mlib_d64 *)vis_alignaddr((void *)coeffs, 0);
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

	vis_write_bmask(0x018923ab, 0x0);

	if (coeffs == NULL || block == NULL)
		return (MLIB_NULLPOINTER);

	if (!(((mlib_addr)block | (mlib_addr)coeffs) & 7))
		return (__mlib_VideoIDCT8x8_S16_S16_B12(block, coeffs));

/*
 * First pass
 */

	LOAD_DATA_GE1;
	COS_1_16 = ((mlib_f32 *)mlib_cTable)[0];

	COS_2_16 = ((mlib_f32 *)mlib_cTable)[1];
	COS_6_16 = ((mlib_f32 *)mlib_cTable)[2];
	COS_7_16 = ((mlib_f32 *)mlib_cTable)[3];
	COS_4_16 = ((mlib_f32 *)mlib_cTable)[4];
	C_1_4 = ((mlib_f32 *)mlib_cTable)[5];

	TRANSPOSE_VIS2(p00, p10, p20, p30, d0, d1, d2, d3);
	TRANSPOSE_VIS2(p01, p11, p21, p31, d4, d5, d6, d7);
	LOAD_DATA_GE2;
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);

	TRANSPOSE_VIS2(p40, p50, p60, p70, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);

	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p01 = vis_fpsub16(dx8, dx6);
	p11 = vis_fpsub16(dx0, dx4);
	p21 = vis_fpsub16(dx3, dx2);
	p31 = vis_fpsub16(dx7, dx1);

	TRANSPOSE_VIS2(p41, p51, p61, p71, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE_VIS2(p00, p10, p20, p30, d0, d1, d2, d3);
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

	TRANSPOSE_VIS2(p40, p50, p60, p70, d4, d5, d6, d7);
	IDCT(d0, d1, d2, d3, d4, d5, d6, d7);
	TRANSPOSE_VIS2(p01, p11, p21, p31, d0, d1, d2, d3);
	p00 = vis_fpadd16(dx7, dx1);
	p10 = vis_fpadd16(dx3, dx2);
	p20 = vis_fpadd16(dx0, dx4);
	p30 = vis_fpadd16(dx8, dx6);
	p40 = vis_fpsub16(dx8, dx6);
	p50 = vis_fpsub16(dx0, dx4);
	p60 = vis_fpsub16(dx3, dx2);
	p70 = vis_fpsub16(dx7, dx1);

	TRANSPOSE_VIS2(p41, p51, p61, p71, d4, d5, d6, d7);
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
