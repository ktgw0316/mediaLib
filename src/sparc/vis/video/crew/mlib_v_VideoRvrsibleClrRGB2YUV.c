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

#pragma ident	"@(#)mlib_v_VideoRvrsibleClrRGB2YUV.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16
 *                                    - convert from RGB to YUV
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16(mlib_s16       *Y,
 *                                               mlib_s16       *U,
 *                                               mlib_s16       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8(mlib_s16      *Y,
 *                                              mlib_s16      *U,
 *                                              mlib_s16      *V,
 *                                              const mlib_u8 *R,
 *                                              const mlib_u8 *G,
 *                                              const mlib_u8 *B,
 *                                              mlib_s32      n,
 *                                              mlib_s32      depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16(mlib_s32       *Y,
 *                                               mlib_s32       *U,
 *                                               mlib_s32       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8(mlib_u8       *Y,
 *                                             mlib_u8       *U,
 *                                             mlib_u8       *V,
 *                                             const mlib_u8 *R,
 *                                             const mlib_u8 *G,
 *                                             const mlib_u8 *B,
 *                                             mlib_s32      n,
 *                                             mlib_s32      depth);
 * ARGUMENT
 *      Y     pointer to output Y component
 *      U     pointer to output U component
 *      V     pointer to output V component
 *      R     pointer to input R component
 *      G     pointer to input G component
 *      B     pointer to input B component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8   -> MLIB_U8           depth = 0 .. 7
 *      MLIB_U8   -> MLIB_S16          depth = 0 .. 8
 *      MLIB_S16  -> MLIB_S16          depth = 0 .. 15
 *      MLIB_S16  -> MLIB_S32          depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *      Y = (R + 2 * G + B) / 4;
 *      U = R - G + 2^depth - 1;
 *      V = B - G + 2^depth - 1;
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_U8
#pragma weak mlib_VideoReversibleColorRGB2YUV_S32_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S32_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_U8_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_S16)
	mlib_VideoReversibleColorRGB2YUV_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_U8)
	mlib_VideoReversibleColorRGB2YUV_S16_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_U8")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S32_S16)
	mlib_VideoReversibleColorRGB2YUV_S32_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S32_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_U8_U8)
	mlib_VideoReversibleColorRGB2YUV_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_U8_U8(
	mlib_u8 *Y,
	mlib_u8 *U,
	mlib_u8 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, mask;
	mlib_d64 *pR = (mlib_d64 *)R;
	mlib_d64 *pG = (mlib_d64 *)G;
	mlib_d64 *pB = (mlib_d64 *)B;
	mlib_d64 *pY = (mlib_d64 *)Y;
	mlib_d64 *pU = (mlib_d64 *)U;
	mlib_d64 *pV = (mlib_d64 *)V;
	mlib_d64 dR, dG, dB;
	mlib_d64 dR0, dG0, dB0, dR1, dG1, dB1;
	mlib_d64 dY0, dY00, dY01, dY1, dY10, dY11;
	mlib_d64 dU0, dU1, dV0, dV1, dDP;

	mlib_f32 scale = vis_to_float(0x80808080 >> 1);
	mlib_f32 fone = vis_to_float(0x1000);

	depth = ((1 << depth) - 1) << 4;
	dDP = vis_to_double_dup(depth | (depth << 16));
	vis_write_gsr(3 << 3);

	for (i = 0; i < (n >> 3); i++) {

		dR = (*pR++);
		dG = (*pG++);
		dB = (*pB++);

		dR0 = vis_fmul8x16al(vis_read_hi(dR), fone);
		dR1 = vis_fmul8x16al(vis_read_lo(dR), fone);
		dG0 = vis_fmul8x16al(vis_read_hi(dG), fone);
		dG1 = vis_fmul8x16al(vis_read_lo(dG), fone);
		dB0 = vis_fmul8x16al(vis_read_hi(dB), fone);
		dB1 = vis_fmul8x16al(vis_read_lo(dB), fone);

		dY00 = vis_fpadd16(dR0, dG0);
		dY10 = vis_fpadd16(dR1, dG1);
		dY01 = vis_fpadd16(dB0, dG0);
		dY11 = vis_fpadd16(dB1, dG1);
		dY0 = vis_fpadd16(dY00, dY01);
		dY1 = vis_fpadd16(dY10, dY11);
		dY0 = vis_fmul8x16(scale, dY0);
		dY1 = vis_fmul8x16(scale, dY1);

		dU0 = vis_fpsub16(dR0, dG0);
		dU1 = vis_fpsub16(dR1, dG1);
		dU0 = vis_fpadd16(dU0, dDP);
		dU1 = vis_fpadd16(dU1, dDP);

		dV0 = vis_fpsub16(dB0, dG0);
		dV1 = vis_fpsub16(dB1, dG1);
		dV0 = vis_fpadd16(dV0, dDP);
		dV1 = vis_fpadd16(dV1, dDP);

		(*pY++) = vis_fpack16_pair(dY0, dY1);
		(*pU++) = vis_fpack16_pair(dU0, dU1);
		(*pV++) = vis_fpack16_pair(dV0, dV1);
	}

	if (n & 7) {

		mask = 0xFF << ((8 - (n & 7)) & 7);

		dR = *pR;
		dG = *pG;
		dB = *pB;

		dR0 = vis_fmul8x16al(vis_read_hi(dR), fone);
		dR1 = vis_fmul8x16al(vis_read_lo(dR), fone);
		dG0 = vis_fmul8x16al(vis_read_hi(dG), fone);
		dG1 = vis_fmul8x16al(vis_read_lo(dG), fone);
		dB0 = vis_fmul8x16al(vis_read_hi(dB), fone);
		dB1 = vis_fmul8x16al(vis_read_lo(dB), fone);

		dY00 = vis_fpadd16(dR0, dG0);
		dY10 = vis_fpadd16(dR1, dG1);
		dY01 = vis_fpadd16(dB0, dG0);
		dY11 = vis_fpadd16(dB1, dG1);
		dY0 = vis_fpadd16(dY00, dY01);
		dY1 = vis_fpadd16(dY10, dY11);
		dY0 = vis_fmul8x16(scale, dY0);
		dY1 = vis_fmul8x16(scale, dY1);

		dU0 = vis_fpsub16(dR0, dG0);
		dU1 = vis_fpsub16(dR1, dG1);
		dU0 = vis_fpadd16(dU0, dDP);
		dU1 = vis_fpadd16(dU1, dDP);

		dV0 = vis_fpsub16(dB0, dG0);
		dV1 = vis_fpsub16(dB1, dG1);
		dV0 = vis_fpadd16(dV0, dDP);
		dV1 = vis_fpadd16(dV1, dDP);

		dY0 = vis_fpack16_pair(dY0, dY1);
		dU0 = vis_fpack16_pair(dU0, dU1);
		dV0 = vis_fpack16_pair(dV0, dV1);

		vis_pst_8(dY0, pY, mask);
		vis_pst_8(dU0, pU, mask);
		vis_pst_8(dV0, pV, mask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_U8(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, mask;
	mlib_d64 *pY = (mlib_d64 *)Y;
	mlib_d64 *pU = (mlib_d64 *)U;
	mlib_d64 *pV = (mlib_d64 *)V;
	mlib_d64 dR, dG, dB;
	mlib_d64 dY, dY0, dY1;
	mlib_d64 dU, dV, dDP;
	mlib_d64 dmask = vis_to_double_dup(0x10001 << 1);

	mlib_f32 *pR = (mlib_f32 *)R;
	mlib_f32 *pG = (mlib_f32 *)G;
	mlib_f32 *pB = (mlib_f32 *)B;
	mlib_f32 scale = vis_to_float(0x80808080 >> 1);
	mlib_f32 fone = vis_to_float(0x100);

	depth = ((1 << depth) - 1);
	dDP = vis_to_double_dup(depth | (depth << 16));
	vis_write_gsr(3 << 3);

	for (i = 0; i < (n >> 2); i++) {

		dR = vis_fmul8x16al((*pR++), fone);
		dG = vis_fmul8x16al((*pG++), fone);
		dB = vis_fmul8x16al((*pB++), fone);

		dY0 = vis_fpadd16(dR, dG);
		dY1 = vis_fpadd16(dB, dG);
		dY = vis_fpadd16(dY0, dY1);
		dY = vis_fandnot(dmask, dY);
		dY = vis_fmul8x16(scale, dY);

		dU = vis_fpsub16(dR, dG);
		dU = vis_fpadd16(dU, dDP);

		dV = vis_fpsub16(dB, dG);
		dV = vis_fpadd16(dV, dDP);

		(*pY++) = dY;
		(*pU++) = dU;
		(*pV++) = dV;
	}

	if (n & 3) {

		mask = 0xF << ((4 - (n & 3)) & 3);

		dR = vis_fmul8x16al(*pR, fone);
		dG = vis_fmul8x16al(*pG, fone);
		dB = vis_fmul8x16al(*pB, fone);

		dY0 = vis_fpadd16(dR, dG);
		dY1 = vis_fpadd16(dB, dG);
		dY = vis_fpadd16(dY0, dY1);
		dY = vis_fandnot(dmask, dY);
		dY = vis_fmul8x16(scale, dY);

		dU = vis_fpsub16(dR, dG);
		dU = vis_fpadd16(dU, dDP);

		dV = vis_fpsub16(dB, dG);
		dV = vis_fpadd16(dV, dDP);

		vis_pst_16(dY, pY, mask);
		vis_pst_16(dU, pU, mask);
		vis_pst_16(dV, pV, mask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_S16(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, mask, shift;
	mlib_d64 *pR = (mlib_d64 *)R;
	mlib_d64 *pG = (mlib_d64 *)G;
	mlib_d64 *pB = (mlib_d64 *)B;
	mlib_d64 *pY = (mlib_d64 *)Y;
	mlib_d64 *pU = (mlib_d64 *)U;
	mlib_d64 *pV = (mlib_d64 *)V;
	mlib_d64 dR, dG, dB;
	mlib_d64 dY, dY0, dY1;
	mlib_d64 dU, dV, dDP;
	mlib_d64 dmask = vis_to_double_dup(0x10001 << 1);

	mlib_f32 scale = vis_to_float(0x80808080 >> 1);

	if (depth > 13) {

		shift = ((1 << depth) - 1);

		for (i = 0; i < n; i++) {
			mlib_s32 r = R[i], g = G[i], b = B[i];

			Y[i] = (r + 2 * g + b) >> 2;
			U[i] = r - g + shift;
			V[i] = b - g + shift;
		}
	} else {

		depth = ((1 << depth) - 1);
		dDP = vis_to_double_dup(depth | (depth << 16));
		vis_write_gsr(3 << 3);

		for (i = 0; i < (n >> 2); i++) {

			dR = (*pR++);
			dG = (*pG++);
			dB = (*pB++);

			dY0 = vis_fpadd16(dR, dG);
			dY1 = vis_fpadd16(dB, dG);
			dY = vis_fpadd16(dY0, dY1);
			dY = vis_fandnot(dmask, dY);
			dY = vis_fmul8x16(scale, dY);

			dU = vis_fpsub16(dR, dG);
			dU = vis_fpadd16(dU, dDP);

			dV = vis_fpsub16(dB, dG);
			dV = vis_fpadd16(dV, dDP);

			(*pY++) = dY;
			(*pU++) = dU;
			(*pV++) = dV;
		}

		if (n & 3) {

			mask = 0xF << ((4 - (n & 3)) & 3);

			dR = (*pR++);
			dG = (*pG++);
			dB = (*pB++);

			dY0 = vis_fpadd16(dR, dG);
			dY1 = vis_fpadd16(dB, dG);
			dY = vis_fpadd16(dY0, dY1);
			dY = vis_fandnot(dmask, dY);
			dY = vis_fmul8x16(scale, dY);

			dU = vis_fpsub16(dR, dG);
			dU = vis_fpadd16(dU, dDP);

			dV = vis_fpsub16(dB, dG);
			dV = vis_fpadd16(dV, dDP);

			vis_pst_16(dY, pY, mask);
			vis_pst_16(dU, pU, mask);
			vis_pst_16(dV, pV, mask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S32_S16(
	mlib_s32 *Y,
	mlib_s32 *U,
	mlib_s32 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, shift = (1 << depth) - 1, r, g, b;

	for (i = 0; i < n; i++) {
		r = R[i];
		g = G[i];
		b = B[i];

		Y[i] = (r + 2 * g + b) >> 2;
		U[i] = r - g + shift;
		V[i] = b - g + shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
