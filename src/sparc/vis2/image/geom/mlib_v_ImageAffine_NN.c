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

#pragma ident	"@(#)mlib_v_ImageAffine_NN.c	9.2	07/11/05 SMI"

/*
 *  Image affine transformation with Nearest Neighbor filtering
 *
 */

#include <vis_proto.h>
#include <mlib_ImageAffine.h>

#define	BUFF_SIZE	256

/* *********************************************************** */

#define	DECLAREVAR_1                                            \
	DECLAREVAR0();                                          \
	mlib_s32 *warp_tbl = param->warp_tbl;                   \
	DTYPE *dstPixelPtr

/* *********************************************************** */

#define	sp	srcPixelPtr
#define	dp	dstPixelPtr

/* *********************************************************** */

#define	CLIP_1(N)                                               \
	dstData += dstYStride;                                  \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	X = xStarts[j];                                         \
	Y = yStarts[j];                                         \
	PREPARE_DELTAS;                                         \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	dstPixelPtr = (DTYPE *) dstData + N * xLeft

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u8

#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8_i(sp, ((x) >> MLIB_SHIFT)))

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_1ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	mlib_s32 i, size;

#ifndef _NO_LONGLONG
	mlib_s64 Y0, Y1, dYl;
#endif /* _NO_LONGLONG */

	for (j = yStart; j <= yFinish; j++) {
		mlib_d64 s0, s1;

		CLIP_1(1);
		size = xRight - xLeft + 1;

		while (((mlib_s32)dp & 3) && (size > 0)) {
			*dp = *(S_PTR(Y) + (X >> MLIB_SHIFT));
			dp++;
			X += dX;
			Y += dY;
			size--;
		}

#ifdef _NO_LONGLONG
#pragma pipeloop(0)
		for (i = 0; i <= (size - 4); i += 4) {
			mlib_u8 *sp0, *sp1, *sp2, *sp3;

			sp0 = S_PTR(Y);
			sp1 = S_PTR(Y + dY);
			sp2 = S_PTR(Y + 2 * dY);
			sp3 = S_PTR(Y + 3 * dY);

			s0 = vis_fpmerge(LD_U8(sp0, X), LD_U8(sp2, X + 2 * dX));
			s1 = vis_fpmerge(LD_U8(sp1, X + dX), LD_U8(sp3,
			    X + 3 * dX));
			s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));

			*(mlib_f32 *)dp = vis_read_lo(s0);

			dp += 4;
			X += 4 * dX;
			Y += 4 * dY;
		}

#else /* _NO_LONGLONG */
		Y0 = ((mlib_s64)(Y + dY) << 32) | Y;

		if (dY >= 0) {
			dYl = ((mlib_s64)dY << 33) | (dY << 1);
		} else {
			dYl = -(((mlib_s64)(-dY) << 33) | ((-dY) << 1));
		}

#pragma pipeloop(0)
		for (i = 0; i <= (size - 4); i += 4) {
			mlib_u8 *sp0, *sp1, *sp2, *sp3;

			Y1 = Y0 + dYl;
			sp0 = S_PTRl(Y0, 16);
			sp1 = S_PTRl(Y0, 48);
			sp2 = S_PTRl(Y1, 16);
			sp3 = S_PTRl(Y1, 48);

			s0 = vis_fpmerge(LD_U8(sp0, X), LD_U8(sp2, X + 2 * dX));
			s1 = vis_fpmerge(LD_U8(sp1, X + dX), LD_U8(sp3,
			    X + 3 * dX));
			s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));

			*(mlib_f32 *)dp = vis_read_lo(s0);

			dp += 4;
			X += 4 * dX;
			Y0 += 2 * dYl;
		}

		Y = Y0 & ((1u << 31) - 1);
#endif /* _NO_LONGLONG */

		for (i = 0; i < (size & 3); i++) {
			dp[i] = *(S_PTR(Y) + (X >> MLIB_SHIFT));
			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  LD_U8
#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8_i(sp, x))

/* *********************************************************** */

#define	GET_POINTERS_2CH                                              \
	sp0 = S_PTR(Y) + 2 * (X >> MLIB_SHIFT);                       \
	sp1 = S_PTR(Y + dY) + 2 * ((X + dX) >> MLIB_SHIFT);           \
	sp2 = S_PTR(Y + 2 * dY) + 2 * ((X + 2 * dX) >> MLIB_SHIFT);   \
	sp3 = S_PTR(Y + 3 * dY) + 2 * ((X + 3 * dX) >> MLIB_SHIFT);   \
	X += 4 * dX;                                                  \
	Y += 4 * dY

/* *********************************************************** */

#define	AFFINE_U8_2CH                                           \
	s0 = vis_fpmerge(LD_U8(sp0, 0), LD_U8(sp2, 0));         \
	s1 = vis_fpmerge(LD_U8(sp0, 1), LD_U8(sp2, 1));         \
	s2 = vis_fpmerge(LD_U8(sp1, 0), LD_U8(sp3, 0));         \
	s3 = vis_fpmerge(LD_U8(sp1, 1), LD_U8(sp3, 1));         \
	s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s2));     \
	s1 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s3));     \
	dd = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1))

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_2ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	mlib_s32 i, size;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		mlib_u8 *sp0, *sp1, *sp2, *sp3;
		mlib_d64 *da, s0, s1, s2, s3, dd, d_old;
		mlib_s32 emask;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;
		size = xRight - xLeft + 1;
		dstLineEnd++;

		if (((mlib_s32)dp & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i <= (size - 4); i += 4) {
				GET_POINTERS_2CH;
				AFFINE_U8_2CH;
				*(mlib_d64 *)dp = dd;
				dp += 8;
			}

			if (i < size) {
				sp0 = sp1 = sp2 = sp3 =
				    S_PTR(Y) + 2 * (X >> MLIB_SHIFT);

				if (i + 1 < size)
					sp1 =
					    S_PTR(Y + dY) + 2 * ((X +
					    dX) >> MLIB_SHIFT);
				if (i + 2 < size)
					sp2 =
					    S_PTR(Y + 2 * dY) + 2 * ((X +
					    2 * dX) >> MLIB_SHIFT);
				if (i + 3 < size)
					sp3 =
					    S_PTR(Y + 3 * dY) + 2 * ((X +
					    3 * dX) >> MLIB_SHIFT);

				AFFINE_U8_2CH;
				emask = vis_edge8(dp, dstLineEnd);
				vis_pst_8(dd, dp, emask);
			}

		} else {
			da = vis_alignaddrl(dp, 0);
			emask = vis_edge8(dp, dstLineEnd);
			i = 0;

			if (size >= 4) {
				GET_POINTERS_2CH;
				AFFINE_U8_2CH;
				vis_pst_8(vis_faligndata(dd, dd), da++, emask);
				d_old = dd;
#pragma pipeloop(0)
				for (i = 4; i <= (size - 4); i += 4) {
					GET_POINTERS_2CH;
					AFFINE_U8_2CH;

					(*da++) = vis_faligndata(d_old, dd);
					d_old = dd;
				}

				emask = vis_edge8(da, dstLineEnd);
			}

			if (i < size) {
				sp0 = sp1 = sp2 = sp3 =
				    S_PTR(Y) + 2 * (X >> MLIB_SHIFT);

				if (i + 1 < size)
					sp1 =
					    S_PTR(Y + dY) + 2 * ((X +
					    dX) >> MLIB_SHIFT);
				if (i + 2 < size)
					sp2 =
					    S_PTR(Y + 2 * dY) + 2 * ((X +
					    2 * dX) >> MLIB_SHIFT);
				if (i + 3 < size)
					sp3 =
					    S_PTR(Y + 3 * dY) + 2 * ((X +
					    3 * dX) >> MLIB_SHIFT);

				AFFINE_U8_2CH;
			}

			vis_pst_8(vis_faligndata(d_old, dd), da++, emask);

			if ((mlib_u8 *)da <= dstLineEnd) {
				emask = vis_edge8(da, dstLineEnd);
				vis_pst_8(vis_faligndata(dd, dd), da, emask);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  LD_U8
#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8(sp + x))

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_3ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	DTYPE *srcPixelPtr;
	mlib_s32 i, size;

	for (j = yStart; j <= yFinish; j++) {
		mlib_d64 s0, s1, s2, s3, s4, s5;

		CLIP_1(3);
		size = xRight - xLeft + 1;

		while (((mlib_s32)dp & 3) && (size > 0)) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
			dp += 3;
			X += dX;
			Y += dY;
			size--;
		}

#pragma pipeloop(0)
		for (i = 0; i <= (size - 4); i += 4) {
			mlib_u8 *sp0, *sp1, *sp2, *sp3;

			sp0 = S_PTR(Y);
			sp1 = S_PTR(Y + dY);
			sp2 = S_PTR(Y + 2 * dY);
			sp3 = S_PTR(Y + 3 * dY);

			sp0 += 3 * (X >> MLIB_SHIFT);
			sp1 += 3 * ((X + dX) >> MLIB_SHIFT);
			sp2 += 3 * ((X + 2 * dX) >> MLIB_SHIFT);
			sp3 += 3 * ((X + 3 * dX) >> MLIB_SHIFT);

			s0 = vis_fpmerge(LD_U8(sp0, 0), LD_U8(sp0, 2));
			s1 = vis_fpmerge(LD_U8(sp0, 1), LD_U8(sp1, 0));
			s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));
			s2 = vis_fpmerge(LD_U8(sp1, 1), LD_U8(sp2, 0));
			s3 = vis_fpmerge(LD_U8(sp1, 2), LD_U8(sp2, 1));
			s2 = vis_fpmerge(vis_read_lo(s2), vis_read_lo(s3));
			s4 = vis_fpmerge(LD_U8(sp2, 2), LD_U8(sp3, 1));
			s5 = vis_fpmerge(LD_U8(sp3, 0), LD_U8(sp3, 2));
			s4 = vis_fpmerge(vis_read_lo(s4), vis_read_lo(s5));

			((mlib_f32 *)dp)[0] = vis_read_lo(s0);
			((mlib_f32 *)dp)[1] = vis_read_lo(s2);
			((mlib_f32 *)dp)[2] = vis_read_lo(s4);

			dp += 12;
			X += 4 * dX;
			Y += 4 * dY;
		}

		for (i = 0; i < (size & 3); i++) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
			dp += 3;
			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  LD_U8
#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8_i(sp, x))

/* *********************************************************** */

#define	AFFINE_U8_4x2                                           \
	sp0 = S_PTR(Y) + 4 * (X >> MLIB_SHIFT);                 \
	sp1 = S_PTR(Y + dY) + 4 * ((X + dX) >> MLIB_SHIFT);     \
	s0 = vis_fpmerge(LD_U8(sp0, 0), LD_U8(sp1, 0));         \
	s1 = vis_fpmerge(LD_U8(sp0, 1), LD_U8(sp1, 1));         \
	s2 = vis_fpmerge(LD_U8(sp0, 2), LD_U8(sp1, 2));         \
	s3 = vis_fpmerge(LD_U8(sp0, 3), LD_U8(sp1, 3));         \
	s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s2));     \
	s1 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s3));     \
	dd = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));     \
	X += 2 * dX;                                            \
	Y += 2 * dY

/* *********************************************************** */

#define	AFFINE_U8_4x1                                           \
	sp0 = S_PTR(Y) + 4 * (X >> MLIB_SHIFT);                 \
	s0 = vis_fpmerge(LD_U8(sp0, 0), LD_U8(sp0, 2));         \
	s1 = vis_fpmerge(LD_U8(sp0, 1), LD_U8(sp0, 3));         \
	s0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));     \
	dd = vis_freg_pair(vis_read_lo(s0), vis_fzeros())

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_4ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	DTYPE *dstLineEnd;
	mlib_s32 i, size;

	for (j = yStart; j <= yFinish; j++) {
		mlib_u8 *sp0, *sp1;
		mlib_d64 *da, s0, s1, s2, s3, dd, d_old;
		mlib_s32 emask;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;
		size = xRight - xLeft + 1;

		if (((mlib_s32)dp & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i <= (size - 2); i += 2) {
				AFFINE_U8_4x2;
				*(mlib_d64 *)dp = dd;
				dp += 8;
			}

			if (i < size) {
				AFFINE_U8_4x1;
				*(mlib_f32 *)dp = vis_read_hi(dd);
			}

		} else {
			dstLineEnd += 3;
			da = vis_alignaddrl(dp, 0);
			emask = vis_edge8(dp, dstLineEnd);
			i = 0;

			if (size >= 2) {
				AFFINE_U8_4x2;
				vis_pst_8(vis_faligndata(dd, dd), da++, emask);
				d_old = dd;

#pragma pipeloop(0)
				for (i = 2; i <= (size - 2); i += 2) {
					AFFINE_U8_4x2;

					(*da++) = vis_faligndata(d_old, dd);
					d_old = dd;
				}

				emask = vis_edge8(da, dstLineEnd);
			}

			if (i < size) {
				AFFINE_U8_4x1;
			}

			vis_pst_8(vis_faligndata(d_old, dd), da++, emask);

			if ((mlib_u8 *)da <= dstLineEnd) {
				emask = vis_edge8(da, dstLineEnd);
				vis_pst_8(vis_faligndata(dd, dd), da, emask);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u16

/* *********************************************************** */

#undef  LD_U16
#define	LD_U16(sp, x)	vis_ld_u16_i(sp, x)

#define	SHIFT1(x)	(((x) >> (MLIB_SHIFT - 1)) &~ 1)

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_1ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	mlib_s32 i, size;

	vis_write_bmask(0x45CD67EF, 0);

	for (j = yStart; j <= yFinish; j++) {
		mlib_d64 s0, s1;

		CLIP_1(1);
		size = xRight - xLeft + 1;

		while (((mlib_s32)dp & 7) && (size > 0)) {
			*dp = *(S_PTR(Y) + (X >> MLIB_SHIFT));
			dp++;
			X += dX;
			Y += dY;
			size--;
		}

#pragma pipeloop(0)
		for (i = 0; i <= (size - 4); i += 4) {
			mlib_u16 *sp0, *sp1, *sp2, *sp3;

			sp0 = S_PTR(Y);
			sp1 = S_PTR(Y + dY);
			sp2 = S_PTR(Y + 2 * dY);
			sp3 = S_PTR(Y + 3 * dY);

			s0 = vis_bshuffle(LD_U16(sp0, SHIFT1(X)), LD_U16(sp2,
			    SHIFT1(X + 2 * dX)));
			s1 = vis_bshuffle(LD_U16(sp1, SHIFT1(X + dX)),
			    LD_U16(sp3, SHIFT1(X + 3 * dX)));
			s0 = vis_bshuffle(s0, s1);

			*(mlib_d64 *)dp = s0;

			dp += 4;
			X += 4 * dX;
			Y += 4 * dY;
		}

		for (i = 0; i < (size & 3); i++) {
			dp[i] = *(S_PTR(Y) + (X >> MLIB_SHIFT));
			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  LD_U8
#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8_i(sp, x))

/* *********************************************************** */

#define	AFFINE_S16_2x2                                          \
	sp0 = S_PTR(Y) + 2 * (X >> MLIB_SHIFT);                 \
	sp1 = S_PTR(Y + dY) + 2 * ((X + dX) >> MLIB_SHIFT);     \
	s0 = vis_bshuffle(LD_U16(sp0, 0), LD_U16(sp1, 0));      \
	s1 = vis_bshuffle(LD_U16(sp0, 2), LD_U16(sp1, 2));      \
	dd = vis_bshuffle(s0, s1);                              \
	X += 2 * dX;                                            \
	Y += 2 * dY

/* *********************************************************** */

#define	AFFINE_S16_2x1                                          \
	sp0 = S_PTR(Y) + 2 * (X >> MLIB_SHIFT);                 \
	s0 = vis_bshuffle(LD_U16(sp0, 0), LD_U16(sp0, 0));      \
	s1 = vis_bshuffle(LD_U16(sp0, 2), LD_U16(sp0, 2));      \
	dd = vis_bshuffle(s0, s1)

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_2ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	DTYPE *dstLineEnd;
	mlib_s32 i, size;

	vis_write_bmask(0x45CD67EF, 0);

	for (j = yStart; j <= yFinish; j++) {
		mlib_u16 *sp0, *sp1;
		mlib_d64 *da, s0, s1, dd, d_old;
		mlib_s32 emask;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;
		size = xRight - xLeft + 1;
		dstLineEnd++;

		if (((mlib_s32)dp & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i <= (size - 2); i += 2) {
				AFFINE_S16_2x2;
				*(mlib_d64 *)dp = dd;
				dp += 4;
			}

			if (i < size) {
				AFFINE_S16_2x1;
				*(mlib_f32 *)dp = vis_read_hi(dd);
			}

		} else {
			da = vis_alignaddrl(dp, 0);
			emask = vis_edge16(dp, dstLineEnd);
			i = 0;

			if (size >= 2) {
				AFFINE_S16_2x2;

				vis_pst_16(vis_faligndata(dd, dd), da++, emask);
				d_old = dd;

#pragma pipeloop(0)
				for (i = 2; i <= (size - 2); i += 2) {
					AFFINE_S16_2x2;

					(*da++) = vis_faligndata(d_old, dd);
					d_old = dd;
				}
				emask = vis_edge16(da, dstLineEnd);
			}

			if (i < size) {
				AFFINE_S16_2x1;
			}

			vis_pst_16(vis_faligndata(d_old, dd), da++, emask);

			if ((mlib_s16 *)da <= (mlib_s16 *)dstLineEnd) {
				emask = vis_edge16(da, dstLineEnd);
				vis_pst_16(vis_faligndata(dd, dd), da, emask);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  LD_U16
#define	LD_U16(sp, x)	vis_ld_u16(sp + x)

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_3ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_1;
	DTYPE *srcPixelPtr;
	mlib_s32 i, size;

	vis_alignaddr((void *)0, 6);

	for (j = yStart; j <= yFinish; j++) {
		mlib_d64 s0, s1, s2;

		CLIP_1(3);
		size = xRight - xLeft + 1;

		while (((mlib_s32)dp & 7) && (size > 0)) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
			dp += 3;
			X += dX;
			Y += dY;
			size--;
		}

#pragma pipeloop(0)
		for (i = 0; i <= (size - 4); i += 4) {
			mlib_u16 *sp0, *sp1, *sp2, *sp3;

			sp0 = S_PTR(Y);
			sp1 = S_PTR(Y + dY);
			sp2 = S_PTR(Y + 2 * dY);
			sp3 = S_PTR(Y + 3 * dY);

			sp0 += 3 * (X >> MLIB_SHIFT);
			sp1 += 3 * ((X + dX) >> MLIB_SHIFT);
			sp2 += 3 * ((X + 2 * dX) >> MLIB_SHIFT);
			sp3 += 3 * ((X + 3 * dX) >> MLIB_SHIFT);

			s2 = vis_faligndata(LD_U16(sp3, 2), s2);
			s2 = vis_faligndata(LD_U16(sp3, 1), s2);
			s2 = vis_faligndata(LD_U16(sp3, 0), s2);
			s2 = vis_faligndata(LD_U16(sp2, 2), s2);
			s1 = vis_faligndata(LD_U16(sp2, 1), s1);
			s1 = vis_faligndata(LD_U16(sp2, 0), s1);
			s1 = vis_faligndata(LD_U16(sp1, 2), s1);
			s1 = vis_faligndata(LD_U16(sp1, 1), s1);
			s0 = vis_faligndata(LD_U16(sp1, 0), s0);
			s0 = vis_faligndata(LD_U16(sp0, 2), s0);
			s0 = vis_faligndata(LD_U16(sp0, 1), s0);
			s0 = vis_faligndata(LD_U16(sp0, 0), s0);

			((mlib_d64 *)dp)[0] = s0;
			((mlib_d64 *)dp)[1] = s1;
			((mlib_d64 *)dp)[2] = s2;

			dp += 12;
			X += 4 * dX;
			Y += 4 * dY;
		}

		for (i = 0; i < (size & 3); i++) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
			dp += 3;
			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_4ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN();
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		mlib_s32 pix0, pix1, pix2, pix3;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		sp = MLIB_POINTER_GET(lineAddr, ySrc) + 4 * xSrc;
		pix0 = sp[0];
		pix1 = sp[1];
		pix2 = sp[2];
		pix3 = sp[3];
		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		for (; dp < dstLineEnd; dp += 4) {
			sp = MLIB_POINTER_GET(lineAddr, ySrc) + 4 * xSrc;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			dp[0] = pix0;
			dp[1] = pix1;
			dp[2] = pix2;
			dp[3] = pix3;
			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];
			pix3 = sp[3];
		}

		dp[0] = pix0;
		dp[1] = pix1;
		dp[2] = pix2;
		dp[3] = pix3;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
