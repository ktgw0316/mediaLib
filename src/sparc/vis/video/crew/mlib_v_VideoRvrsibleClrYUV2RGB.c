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

#pragma ident	"@(#)mlib_v_VideoRvrsibleClrYUV2RGB.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorYUV2RGB_U8_U8
 *      mlib_VideoReversibleColorYUV2RGB_U8_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S32
 *                                    - convert from YUV to RGB
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_v_VideoReversibleColorYUV2RGB_S16_S16(mlib_s16       *R,
 *                                                 const mlib_s16 *G,
 *                                                 const mlib_s16 *B,
 *                                                 const mlib_s16 *Y,
 *                                                 const mlib_s16 *U,
 *                                                 const mlib_s16 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_v_VideoReversibleColorYUV2RGB_S16_S32(mlib_s16       *R,
 *                                                 const mlib_s16 *G,
 *                                                 const mlib_s16 *B,
 *                                                 const mlib_s32 *Y,
 *                                                 const mlib_s32 *U,
 *                                                 const mlib_s32 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_v_VideoReversibleColorYUV2RGB_U8_S16(mlib_u8        *R,
 *                                                const mlib_u8  *G,
 *                                                const mlib_u8  *B,
 *                                                const mlib_s16 *Y,
 *                                                const mlib_s16 *U,
 *                                                const mlib_s16 *V,
 *                                                mlib_s32       n,
 *                                                mlib_s32       depth);
 *      mlib_status
 *      mlib_v_VideoReversibleColorYUV2RGB_U8_U8(mlib_u8       *R,
 *                                               const mlib_u8 *G,
 *                                               const mlib_u8 *B,
 *                                               const mlib_u8 *Y,
 *                                               const mlib_u8 *U,
 *                                               const mlib_u8 *V,
 *                                               mlib_s32      n,
 *                                               mlib_s32      depth);
 *
 * ARGUMENT
 *      R     pointer to output R component
 *      G     pointer to output G component
 *      B     pointer to output B component
 *      Y     pointer to input Y component
 *      U     pointer to input U component
 *      V     pointer to input V component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8    -> MLIB_U8             depth = 0 .. 7
 *      MLIB_S16   -> MLIB_U8             depth = 0 .. 8
 *      MLIB_S16   -> MLIB_S16            depth = 0 .. 15
 *      MLIB_S32   -> MLIB_S16            depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *  G = Y - (U - 2*(2^depth - 1) + V) / 4;
 *  R = U + G - 2^depth + 1;
 *  B = V + G - 2^depth + 1;
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S32 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S32
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_U8 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S16)
	mlib_VideoReversibleColorYUV2RGB_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S32)
	mlib_VideoReversibleColorYUV2RGB_S16_S32
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S32")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_S16)
	mlib_VideoReversibleColorYUV2RGB_U8_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_U8)
	mlib_VideoReversibleColorYUV2RGB_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_U8(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_u8 *Y,
	const mlib_u8 *U,
	const mlib_u8 *V,
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
	mlib_d64 dY, dU, dV, dR0, dG0, dB0;
	mlib_d64 dR1, dG1, dB1, dY0, dY1;
	mlib_d64 dU0, dU1, dV0, dV1, dDP, dDP_2;
	mlib_d64 dmask = vis_to_double_dup(0xFFF0FFF0);
	mlib_f32 scale = vis_to_float(0x80808080 >> 1);
	mlib_f32 fone = vis_to_float(0x1000);

	depth = ((1 << depth) - 1) << 4;
	dDP = vis_to_double_dup(depth | (depth << 16));
	dDP_2 = vis_to_double_dup((depth << 1) | (depth << 17));
	vis_write_gsr(3 << 3);

	for (i = 0; i < (n >> 3); i++) {

		dY = (*pY++);
		dU = (*pU++);
		dV = (*pV++);

		dY0 = vis_fmul8x16al(vis_read_hi(dY), fone);
		dY1 = vis_fmul8x16al(vis_read_lo(dY), fone);
		dU0 = vis_fmul8x16al(vis_read_hi(dU), fone);
		dU1 = vis_fmul8x16al(vis_read_lo(dU), fone);
		dV0 = vis_fmul8x16al(vis_read_hi(dV), fone);
		dV1 = vis_fmul8x16al(vis_read_lo(dV), fone);

		dG0 = vis_fpadd16(dU0, dV0);
		dG1 = vis_fpadd16(dU1, dV1);
		dG0 = vis_fpsub16(dG0, dDP_2);
		dG1 = vis_fpsub16(dG1, dDP_2);
		dG0 = vis_fmul8x16(scale, dG0);
		dG1 = vis_fmul8x16(scale, dG1);
		dG0 = vis_fand(dmask, dG0);
		dG1 = vis_fand(dmask, dG1);
		dG0 = vis_fpsub16(dY0, dG0);
		dG1 = vis_fpsub16(dY1, dG1);

		dR0 = vis_fpadd16(dU0, dG0);
		dR1 = vis_fpadd16(dU1, dG1);
		dR0 = vis_fpsub16(dR0, dDP);
		dR1 = vis_fpsub16(dR1, dDP);

		dB0 = vis_fpadd16(dV0, dG0);
		dB1 = vis_fpadd16(dV1, dG1);
		dB0 = vis_fpsub16(dB0, dDP);
		dB1 = vis_fpsub16(dB1, dDP);

		(*pR++) = vis_fpack16_pair(dR0, dR1);
		(*pG++) = vis_fpack16_pair(dG0, dG1);
		(*pB++) = vis_fpack16_pair(dB0, dB1);
	}

	if (n & 7) {

		mask = 0xFF << ((8 - (n & 7)) & 7);

		dY = *pY;
		dU = *pU;
		dV = *pV;

		dY0 = vis_fmul8x16al(vis_read_hi(dY), fone);
		dY1 = vis_fmul8x16al(vis_read_lo(dY), fone);
		dU0 = vis_fmul8x16al(vis_read_hi(dU), fone);
		dU1 = vis_fmul8x16al(vis_read_lo(dU), fone);
		dV0 = vis_fmul8x16al(vis_read_hi(dV), fone);
		dV1 = vis_fmul8x16al(vis_read_lo(dV), fone);

		dG0 = vis_fpadd16(dU0, dV0);
		dG1 = vis_fpadd16(dU1, dV1);
		dG0 = vis_fpsub16(dG0, dDP_2);
		dG1 = vis_fpsub16(dG1, dDP_2);
		dG0 = vis_fmul8x16(scale, dG0);
		dG1 = vis_fmul8x16(scale, dG1);
		dG0 = vis_fand(dmask, dG0);
		dG1 = vis_fand(dmask, dG1);
		dG0 = vis_fpsub16(dY0, dG0);
		dG1 = vis_fpsub16(dY1, dG1);

		dR0 = vis_fpadd16(dU0, dG0);
		dR1 = vis_fpadd16(dU1, dG1);
		dR0 = vis_fpsub16(dR0, dDP);
		dR1 = vis_fpsub16(dR1, dDP);

		dB0 = vis_fpadd16(dV0, dG0);
		dB1 = vis_fpadd16(dV1, dG1);
		dB0 = vis_fpsub16(dB0, dDP);
		dB1 = vis_fpsub16(dB1, dDP);

		dG0 = vis_fpack16_pair(dG0, dG1);
		dR0 = vis_fpack16_pair(dR0, dR1);
		dB0 = vis_fpack16_pair(dB0, dB1);

		vis_pst_8(dR0, pR, mask);
		vis_pst_8(dG0, pG, mask);
		vis_pst_8(dB0, pB, mask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_S16(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
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
	mlib_d64 dY0, dU0, dV0, dY1;
	mlib_d64 dU1, dV1, dG0, dR0, dB0;
	mlib_d64 dG1, dR1, dB1, dG;
	mlib_d64 dR, dB, dDP_2, dDP;
	mlib_d64 dmask = vis_to_double_dup(0x10001 << 1);
	mlib_f32 scale = vis_to_float(0x80808080 >> 1);

	depth = ((1 << depth) - 1);
	dDP = vis_to_double_dup(depth | (depth << 16));
	dDP_2 = vis_to_double_dup((depth << 1) | (depth << 17));
	vis_write_gsr(7 << 3);

	for (i = 0; i < (n >> 3); i++) {

		dY0 = (*pY++);
		dY1 = (*pY++);
		dU0 = (*pU++);
		dU1 = (*pU++);
		dV0 = (*pV++);
		dV1 = (*pV++);

		dG0 = vis_fpadd16(dU0, dV0);
		dG1 = vis_fpadd16(dU1, dV1);
		dG0 = vis_fpsub16(dG0, dDP_2);
		dG1 = vis_fpsub16(dG1, dDP_2);
		dG0 = vis_fandnot(dmask, dG0);
		dG1 = vis_fandnot(dmask, dG1);
		dG0 = vis_fmul8x16(scale, dG0);
		dG1 = vis_fmul8x16(scale, dG1);
		dG0 = vis_fpsub16(dY0, dG0);
		dG1 = vis_fpsub16(dY1, dG1);

		dR0 = vis_fpadd16(dU0, dG0);
		dR1 = vis_fpadd16(dU1, dG1);
		dR0 = vis_fpsub16(dR0, dDP);
		dR1 = vis_fpsub16(dR1, dDP);

		dB0 = vis_fpadd16(dV0, dG0);
		dB1 = vis_fpadd16(dV1, dG1);
		dB0 = vis_fpsub16(dB0, dDP);
		dB1 = vis_fpsub16(dB1, dDP);

		(*pR++) = vis_fpack16_pair(dR0, dR1);
		(*pG++) = vis_fpack16_pair(dG0, dG1);
		(*pB++) = vis_fpack16_pair(dB0, dB1);
	}

	if (n & 7) {

		mask = 0xFF << ((8 - (n & 7)) & 7);

		dY0 = vis_ld_d64_nf(pY);
		pY++;
		dY1 = vis_ld_d64_nf(pY);
		dU0 = vis_ld_d64_nf(pU);
		pU++;
		dU1 = vis_ld_d64_nf(pU);
		dV0 = vis_ld_d64_nf(pV);
		pV++;
		dV1 = vis_ld_d64_nf(pV);

		dG0 = vis_fpadd16(dU0, dV0);
		dG1 = vis_fpadd16(dU1, dV1);
		dG0 = vis_fpsub16(dG0, dDP_2);
		dG1 = vis_fpsub16(dG1, dDP_2);
		dG0 = vis_fandnot(dmask, dG0);
		dG1 = vis_fandnot(dmask, dG1);
		dG0 = vis_fmul8x16(scale, dG0);
		dG1 = vis_fmul8x16(scale, dG1);
		dG0 = vis_fpsub16(dY0, dG0);
		dG1 = vis_fpsub16(dY1, dG1);

		dR0 = vis_fpadd16(dU0, dG0);
		dR1 = vis_fpadd16(dU1, dG1);
		dR0 = vis_fpsub16(dR0, dDP);
		dR1 = vis_fpsub16(dR1, dDP);

		dB0 = vis_fpadd16(dV0, dG0);
		dB1 = vis_fpadd16(dV1, dG1);
		dB0 = vis_fpsub16(dB0, dDP);
		dB1 = vis_fpsub16(dB1, dDP);

		dR = vis_fpack16_pair(dR0, dR1);
		dG = vis_fpack16_pair(dG0, dG1);
		dB = vis_fpack16_pair(dB0, dB1);

		vis_pst_8(dR, pR, mask);
		vis_pst_8(dG, pG, mask);
		vis_pst_8(dB, pB, mask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S16(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
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
	mlib_d64 dR, dG, dB, dY, dU, dV, dDP, dDP_2;
	mlib_d64 dmask = vis_to_double_dup(0x10001 << 1);
	mlib_f32 scale = vis_to_float(0x80808080 >> 1);

	if (depth > 13) {

		shift = ((1 << depth) - 1);

		for (i = 0; i < n; i++) {
			mlib_s32 r, g, b, y = Y[i], u = U[i], v = V[i];

			g = y - ((u - 2 * shift + v) >> 2);
			r = u + g - shift;
			b = v + g - shift;

			R[i] = r;
			G[i] = g;
			B[i] = b;
		}
	} else {

		depth = ((1 << depth) - 1);
		dDP = vis_to_double_dup(depth | (depth << 16));
		dDP_2 = vis_to_double_dup((depth << 1) | (depth << 17));
		vis_write_gsr(3 << 3);

		for (i = 0; i < (n >> 2); i++) {

			dY = (*pY++);
			dU = (*pU++);
			dV = (*pV++);

			dG = vis_fpadd16(dU, dV);
			dG = vis_fpsub16(dG, dDP_2);
			dG = vis_fandnot(dmask, dG);
			dG = vis_fmul8x16(scale, dG);
			dG = vis_fpsub16(dY, dG);

			dR = vis_fpadd16(dU, dG);
			dR = vis_fpsub16(dR, dDP);

			dB = vis_fpadd16(dV, dG);
			dB = vis_fpsub16(dB, dDP);

			(*pR++) = dR;
			(*pG++) = dG;
			(*pB++) = dB;
		}

		if (n & 3) {

			mask = 0xF << ((4 - (n & 3)) & 3);

			dY = *pY;
			dU = *pU;
			dV = *pV;

			dG = vis_fpadd16(dU, dV);
			dG = vis_fpsub16(dG, dDP_2);
			dG = vis_fandnot(dmask, dG);
			dG = vis_fmul8x16(scale, dG);
			dG = vis_fpsub16(dY, dG);

			dR = vis_fpadd16(dU, dG);
			dR = vis_fpsub16(dR, dDP);

			dB = vis_fpadd16(dV, dG);
			dB = vis_fpsub16(dB, dDP);

			vis_pst_16(dR, pR, mask);
			vis_pst_16(dG, pG, mask);
			vis_pst_16(dB, pB, mask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S32(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s32 *Y,
	const mlib_s32 *U,
	const mlib_s32 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, r, g, b, y, u, v, shift = (1 << depth) - 1;

	for (i = 0; i < n; i++) {
		y = Y[i];
		u = U[i];
		v = V[i];

		g = y - ((u - 2 * shift + v) >> 2);
		r = u + g - shift;
		b = v + g - shift;

		R[i] = r;
		G[i] = g;
		B[i] = b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
