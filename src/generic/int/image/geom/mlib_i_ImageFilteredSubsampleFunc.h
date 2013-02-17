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

#ifndef _MLIB_I_IMAGEFILTEREDSUBSAMPLEFUNC_H
#define	_MLIB_I_IMAGEFILTEREDSUBSAMPLEFUNC_H

#pragma ident	"@(#)mlib_i_ImageFilteredSubsampleFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_NO_LONGLONG

#include <mlib_ImageFilteredSubsample.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#undef	SHIFT
#undef	DTOLL
#define	DTOLL(l, d)	BASIC_DTOLL(l, d, SHIFT)
#define	HALFONE	(1u<<(SHIFT-1))   /* 0.5 */

/* *********************************************************** */

#define	CLAMP(max, min)                                         \
	if (sat_tmp >= max)                                     \
	    sat_tmp = max;                                      \
	if (sat_tmp <= min)                                     \
	    sat_tmp = min

/* *********************************************************** */

#if IMG_TYPE == 1

#define	STYPE	mlib_u8
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_s64
#define	SHIFT	12
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_8nw_##SUFF
#define	DSCALE	1
#define	SAT_OFF	HALFONE

#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = ((x)>>SHIFT);                       \
	                                                        \
	    CLAMP(MLIB_U8_MAX, MLIB_U8_MIN);                    \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_s64
#define	SHIFT	25
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_16nw_##SUFF
#define	DSCALE	1
#define	SAT_OFF	HALFONE
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = ((x)>>SHIFT);                       \
	                                                        \
	    CLAMP(MLIB_S16_MAX, MLIB_S16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	STYPE	mlib_u16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_s64
#define	SHIFT	25
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_u16nw_##SUFF
#define	CASE_D	0
#define	DSCALE	1
#define	SAT_OFF	HALFONE
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = ((x)>>SHIFT);                       \
	                                                        \
	    CLAMP(MLIB_U16_MAX, MLIB_U16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s32
#define	ITYPE	mlib_s64
#define	FTYPE	mlib_s64
#define	SHIFT	30
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_32nw_##SUFF
#define	DSCALE	1
#define	SAT_OFF	0
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = ((x)>>SHIFT);                       \
	                                                        \
	    CLAMP(MLIB_S32_MAX, MLIB_S32_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#endif		   /* IMG_TYPE == 1 */

/* *********************************************************** */

#define	I2F(x)	(FTYPE)(x)

/* *********************************************************** */

static void FS_FUNC(2x2i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, kk = pkk[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		buff[i] += kk * ss;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x3i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[1];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		buff[i] += kk * ss + k0 * s0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x2i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp2[0] + sp2[off]);
		buff[i] += kk * ss + k0 * s0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x3i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, s1;
	FTYPE kk = pkk[0], k0 = pkk[1], k1 = pkk[1], k2 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		s1 = I2F((ITYPE) sp2[0] + sp2[off]);
		buff[i] =
		    buff[i] + k2 * I2F(sp2[nchan]) + k0 * s0 + k1 * s1 +
		    kk * ss;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x2l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF, ss, kk = pkk[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x3l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF, ss, s0, kk = pkk[0], k0 = pkk[1];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + k0 * s0 + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x2l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF;
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp2[0] + sp2[off]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + k0 * s0 + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x3l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF;
	FTYPE ss, s0, s1;
	FTYPE kk = pkk[0], k0 = pkk[1], k1 = pkk[2], k2 = pkk[3];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		s1 = I2F((ITYPE) sp2[0] + sp2[off]);
		STORE_SAT(dp[i * nchan], buffo[i],
		    buff[i] + k2 * I2F(sp2[nchan]) + k0 * s0 + k1 * s1 +
		    kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

mlib_status FS_FUNC(1)  (
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    const mlib_d64 *hKernel,
    const mlib_d64 *vKernel,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	STYPE *psrc, *pdst, *sl, *slk, *dl;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 step, stepRight, stepDown;
	FTYPE *buff, kk[4], val0 = SAT_OFF;
	FTYPE kernz[2], *Kernel_int, *hKernel_int, *vKernel_int;
	mlib_s32 *buffo;
	mlib_s32 x_min, x_max, y_min, y_max, xsize;
	mlib_s32 i, j, k, ix, iy;

#ifdef USE_FP_TBL
	FTYPE ss = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 1024; i++) {
		I2F(i) = ss;
		ss += 1.f;
	}
#endif /* USE_FP_TBL */

	dwidth = mlib_ImageGetWidth(dst);
	dheight = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	nchan = mlib_ImageGetChannels(src);
	swidth = mlib_ImageGetWidth(src);
	sheight = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	psrc = (void *)mlib_ImageGetData(src);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	Kernel_int = __mlib_malloc(sizeof (FTYPE) * (hSize + vSize));
	if (Kernel_int == NULL)
		return (MLIB_FAILURE);
	hKernel_int = Kernel_int;
	vKernel_int = (FTYPE *) (Kernel_int + hSize);

	for (iy = vSize - 1; iy >= 0; iy--) {
		DTOLL(vKernel_int[iy], vKernel[iy]);
	}
	for (ix = hSize - 1; ix >= 0; ix--) {
		DTOLL(hKernel_int[ix], hKernel[ix]);
	}

	if (kw == 1) {
		hParity = 0;
		mlib_d64 temp1;
		temp1 = hKernel[0] * 0.5;
		DTOLL(kernz[0], temp1);
		hKernel_int = kernz;
	}

	if (kh == 1) {
		vParity = 0;
		mlib_d64 temp2;
		temp2 = vKernel[0] * 0.5;
		DTOLL(kernz[1], temp2);
		vKernel_int = kernz + 1;
	}

	step = scaleX * nchan;
	stepRight = (kw - 1) * nchan;
	stepDown = (kh - 1) * slb;

/* 0.5*scaleX - 0.5*(kw-1) */
	transX += (scaleX - (kw - 1)) >> 1;
/* 0.5*scaleY - 0.5*(kh-1) */
	transY += (scaleY - (kh - 1)) >> 1;

	x_min = scaleX - 1 - transX;
	y_min = scaleY - 1 - transY;
	x_max = swidth - kw - transX;
	y_max = sheight - kh - transY;

	if (x_min >= 0)
		x_min /= scaleX;
	else
		x_min = 0;

	if (y_min >= 0)
		y_min /= scaleY;
	else
		y_min = 0;

	if (x_max >= 0)
		x_max /= scaleX;
	else
		x_max = -1;

	if (y_max >= 0)
		y_max /= scaleY;
	else
		y_max = -1;

	if (x_max >= dwidth)
		x_max = dwidth - 1;

	if (y_max >= dheight)
		y_max = dheight - 1;

	xsize = x_max - x_min + 1;

	if (xsize <= 0)
		return (MLIB_SUCCESS);

	buff = __mlib_malloc((sizeof (FTYPE) + sizeof (mlib_s32)) * xsize);

	if (buff == NULL)
		return (MLIB_FAILURE);
	buffo = (mlib_s32 *)(buff + xsize);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		buff[i] = val0;
	}

	sl = psrc + (y_min * scaleY + transY) * slb + (x_min * scaleX +
	    transX) * nchan;
	dl = pdst + y_min * dlb + x_min * nchan;

	for (j = y_min; j <= y_max; j++) {
		slk = sl;

		for (k = 0; k < nchan; k++) {
			STYPE *sl0 = slk;
			STYPE *sl1 = slk + stepDown;

			for (iy = vSize - 1; iy >= vParity; iy--) {
				mlib_s32 off = stepRight;
				STYPE *sp0 = sl0;
				STYPE *sp1 = sl1;

				for (ix = hSize - 1; ix >= hParity; ix--) {
					mlib_s32 flagl = 0, flagh = 0, flagv =
					    0;

					if (iy == vParity && ix == hParity)
						flagl = 1;

					if (vParity && iy == vParity)
						flagv = 1;

					if (hParity && ix == hParity)
						flagh = 1;

					kk[0] =
					    (FTYPE) ((hKernel_int[ix] *
					    vKernel_int[iy] * DSCALE) >> SHIFT);

					if (flagv || flagh) {
						kk[1] =
						    (FTYPE) ((hKernel_int[0] *
						    vKernel_int[iy] * DSCALE) >>
						    SHIFT);
						kk[2] =
						    (FTYPE) ((hKernel_int[ix] *
						    vKernel_int[0] * DSCALE) >>
						    SHIFT);
						kk[3] =
						    (FTYPE) ((hKernel_int[0] *
						    vKernel_int[0] * DSCALE) >>
						    SHIFT);
					}

					switch (4 * flagl + 2 * flagv + flagh) {
					case 0:
						FS_FUNC(2x2i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 1:
						FS_FUNC(2x3i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 2:
						FS_FUNC(3x2i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 3:
						FS_FUNC(3x3i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 4:
						FS_FUNC(2x2l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 5:
						FS_FUNC(2x3l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 6:
						FS_FUNC(3x2l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 7:
						FS_FUNC(3x3l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					}

					sp0 += nchan;
					sp1 += nchan;
					off -= 2 * nchan;
				}

				sl0 += slb;
				sl1 -= slb;
			}

			slk++;
		}

		sl += scaleY * slb;
		dl += dlb;
	}

	__mlib_free(buff);
	__mlib_free(Kernel_int);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#else /* _NO_LONGLONG */

#include <mlib_ImageFilteredSubsample.h>

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING
#define	CLAMP(max, min)                                         \
	if (sat_tmp >= max)                                     \
	    sat_tmp = max;                                      \
	if (sat_tmp <= min)                                     \
	    sat_tmp = min
#endif		   /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#if IMG_TYPE == 1

#define	STYPE	mlib_u8
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_f32
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_8nw_##SUFF
#ifdef MLIB_USE_FTOI_CLAMPING
#define	DSCALE	(1 << 24)
#define	SAT_OFF	((0.5 - 128) * DSCALE)
#define	STORE_SAT(res, tmp, x)                                  \
	tmp = (mlib_s32)(x);                                    \
	res = (((tmp) >> 24) ^ 128)
#else		   /* MLIB_USE_FTOI_CLAMPING */
#define	DSCALE	1
#define	SAT_OFF	0.5
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x);                                \
	                                                        \
	    CLAMP(MLIB_U8_MAX, MLIB_U8_MIN);                    \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}
#endif		   /* MLIB_USE_FTOI_CLAMPING */

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_f32
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_16nw_##SUFF
#ifdef MLIB_USE_FTOI_CLAMPING
#define	DSCALE	65536.0
#define	SAT_OFF	(0.5*DSCALE)
#define	STORE_SAT(res, tmp, x)                                  \
	tmp = (mlib_s32)(x);                                    \
	res = ((tmp) >> 16)
#else		   /* MLIB_USE_FTOI_CLAMPING */
#define	DSCALE	1
#define	SAT_OFF	0.5
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x);                                \
	                                                        \
	    CLAMP(MLIB_S16_MAX, MLIB_S16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}
#endif		   /* MLIB_USE_FTOI_CLAMPING */

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	STYPE	mlib_u16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_f32
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_u16nw_##SUFF
#ifdef MLIB_USE_FTOI_CLAMPING
#define	CASE_D	1
#define	DSCALE	(1 << 16)
#define	SAT_OFF	((0.5 - 32768.) * DSCALE)
#define	STORE_SAT(res, tmp, x)                                  \
	tmp = (mlib_s32)(x);                                    \
	res = (((tmp) >> 16) ^ 32768)
#else		   /* MLIB_USE_FTOI_CLAMPING */
#define	CASE_D	0
#define	DSCALE	1
#define	SAT_OFF	0.5
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x);                                \
	                                                        \
	    CLAMP(MLIB_U16_MAX, MLIB_U16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}
#endif		   /* MLIB_USE_FTOI_CLAMPING */

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 */

#define	STYPE	mlib_s32
#define	ITYPE	mlib_d64
#define	FTYPE	mlib_d64
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_32nw_##SUFF
#define	DSCALE	1.0
#define	SAT_OFF	0.0
#ifdef MLIB_USE_FTOI_CLAMPING
#define	STORE_SAT(res, tmp, x)	res = (mlib_s32)(x)
#else		   /* MLIB_USE_FTOI_CLAMPING */
#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x);                                \
	                                                        \
	    CLAMP(MLIB_S32_MAX, MLIB_S32_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}
#endif		   /* MLIB_USE_FTOI_CLAMPING */

#elif IMG_TYPE == 5	/* IMG_TYPE == 1 */

#define	STYPE	mlib_f32
#define	ITYPE	mlib_f32
#define	FTYPE	mlib_f32
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_f32nw_##SUFF
#define	DSCALE	1.0
#define	SAT_OFF	0.0
#define	STORE_SAT(res, tmp, x)	res = x
#elif IMG_TYPE == 6	/* IMG_TYPE == 1 */
#define	STYPE	mlib_d64
#define	ITYPE	mlib_d64
#define	FTYPE	mlib_d64
#define	FS_FUNC(SUFF)	mlib_ImageFilteredSubsample_d64nw_##SUFF
#define	DSCALE	1.0
#define	SAT_OFF	0.0
#define	STORE_SAT(res, tmp, x)	res = x
#endif		   /* IMG_TYPE == 1 */

/* *********************************************************** */

#define	I2F(x)	(FTYPE)(x)

/* *********************************************************** */

static void FS_FUNC(2x2i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, kk = pkk[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		buff[i] += kk * ss;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x3i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[1];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		buff[i] += kk * ss + k0 * s0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x2i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp2[0] + sp2[off]);
		buff[i] += kk * ss + k0 * s0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x3i) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan)
{
	FTYPE ss, s0, s1;
	FTYPE kk = pkk[0], k0 = pkk[1], k1 = pkk[1], k2 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		s1 = I2F((ITYPE) sp2[0] + sp2[off]);
		buff[i] =
		    buff[i] + k2 * I2F(sp2[nchan]) + k0 * s0 + k1 * s1 +
		    kk * ss;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x2l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF, ss, kk = pkk[0];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x3l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF, ss, s0, kk = pkk[0], k0 = pkk[1];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + k0 * s0 + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x2l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF;
	FTYPE ss, s0, kk = pkk[0], k0 = pkk[2];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp2[0] + sp2[off]);
		STORE_SAT(dp[i * nchan], buffo[i], buff[i] + k0 * s0 + kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(3x3l) (
    FTYPE *buff,
    const STYPE *sp0,
    const STYPE *sp1,
    const FTYPE *pkk,
    mlib_s32 xsize,
    mlib_s32 off,
    mlib_s32 step,
    mlib_s32 nchan,
    STYPE *dp,
    mlib_s32 *buffo)
{
	FTYPE val0 = SAT_OFF;
	FTYPE ss, s0, s1;
	FTYPE kk = pkk[0], k0 = pkk[1], k1 = pkk[2], k2 = pkk[3];
	const STYPE *sp2 = sp0 + ((sp1 - sp0) >> 1);
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp0[0] + sp0[off] + sp1[0] + sp1[off]);
		s0 = I2F((ITYPE) sp0[nchan] + sp1[nchan]);
		s1 = I2F((ITYPE) sp2[0] + sp2[off]);
		STORE_SAT(dp[i * nchan], buffo[i],
		    buff[i] + k2 * I2F(sp2[nchan]) + k0 * s0 + k1 * s1 +
		    kk * ss);
		buff[i] = val0;
		sp0 += step;
		sp1 += step;
		sp2 += step;
	}
}

/* *********************************************************** */

mlib_status FS_FUNC(1)  (
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    const mlib_d64 *hKernel,
    const mlib_d64 *vKernel,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	STYPE *psrc, *pdst, *sl, *slk, *dl;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 step, stepRight, stepDown;
	FTYPE *buff, kk[4], val0 = SAT_OFF;
	mlib_d64 kernz[2];
	mlib_s32 *buffo;
	mlib_s32 x_min, x_max, y_min, y_max, xsize;
	mlib_s32 i, j, k, ix, iy;

#ifdef USE_FP_TBL
	FTYPE ss = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 1024; i++) {
		I2F(i) = ss;
		ss += 1.f;
	}
#endif /* USE_FP_TBL */

	dwidth = mlib_ImageGetWidth(dst);
	dheight = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	nchan = mlib_ImageGetChannels(src);
	swidth = mlib_ImageGetWidth(src);
	sheight = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	psrc = (void *)mlib_ImageGetData(src);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	if (kw == 1) {
		hParity = 0;
		kernz[0] = hKernel[0] * 0.5;
		hKernel = kernz;
	}

	if (kh == 1) {
		vParity = 0;
		kernz[1] = vKernel[0] * 0.5;
		vKernel = kernz + 1;
	}

	step = scaleX * nchan;
	stepRight = (kw - 1) * nchan;
	stepDown = (kh - 1) * slb;

/* 0.5*scaleX - 0.5*(kw-1) */
	transX += (scaleX - (kw - 1)) >> 1;
/* 0.5*scaleY - 0.5*(kh-1) */
	transY += (scaleY - (kh - 1)) >> 1;

	x_min = scaleX - 1 - transX;
	y_min = scaleY - 1 - transY;
	x_max = swidth - kw - transX;
	y_max = sheight - kh - transY;

	if (x_min >= 0)
		x_min /= scaleX;
	else
		x_min = 0;

	if (y_min >= 0)
		y_min /= scaleY;
	else
		y_min = 0;

	if (x_max >= 0)
		x_max /= scaleX;
	else
		x_max = -1;

	if (y_max >= 0)
		y_max /= scaleY;
	else
		y_max = -1;

	if (x_max >= dwidth)
		x_max = dwidth - 1;

	if (y_max >= dheight)
		y_max = dheight - 1;

	xsize = x_max - x_min + 1;

	if (xsize <= 0)
		return (MLIB_SUCCESS);

	buff = __mlib_malloc((sizeof (FTYPE) + sizeof (mlib_s32)) * xsize);

	if (buff == NULL)
		return (MLIB_FAILURE);
	buffo = (mlib_s32 *)(buff + xsize);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		buff[i] = val0;
	}

	sl = psrc + (y_min * scaleY + transY) * slb + (x_min * scaleX +
	    transX) * nchan;
	dl = pdst + y_min * dlb + x_min * nchan;

	for (j = y_min; j <= y_max; j++) {
		slk = sl;

		for (k = 0; k < nchan; k++) {
			STYPE *sl0 = slk;
			STYPE *sl1 = slk + stepDown;

			for (iy = vSize - 1; iy >= vParity; iy--) {
				mlib_s32 off = stepRight;
				STYPE *sp0 = sl0;
				STYPE *sp1 = sl1;

				for (ix = hSize - 1; ix >= hParity; ix--) {
					mlib_s32 flagl = 0, flagh = 0, flagv =
					    0;

					if (iy == vParity && ix == hParity)
						flagl = 1;

					if (vParity && iy == vParity)
						flagv = 1;

					if (hParity && ix == hParity)
						flagh = 1;

					kk[0] =
					    (FTYPE) (hKernel[ix] * vKernel[iy] *
					    DSCALE);

					if (flagv || flagh) {
						kk[1] =
						    (FTYPE) (hKernel[0] *
						    vKernel[iy] * DSCALE);
						kk[2] =
						    (FTYPE) (hKernel[ix] *
						    vKernel[0] * DSCALE);
						kk[3] =
						    (FTYPE) (hKernel[0] *
						    vKernel[0] * DSCALE);
					}

					switch (4 * flagl + 2 * flagv + flagh) {
					case 0:
						FS_FUNC(2x2i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 1:
						FS_FUNC(2x3i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 2:
						FS_FUNC(3x2i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 3:
						FS_FUNC(3x3i) (buff, sp0, sp1,
						    kk, xsize, off, step,
						    nchan);
						break;
					case 4:
						FS_FUNC(2x2l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 5:
						FS_FUNC(2x3l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 6:
						FS_FUNC(3x2l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					case 7:
						FS_FUNC(3x3l) (buff, sp0, sp1,
						    kk, xsize, off, step, nchan,
						    dl + k, buffo);
						break;
					}

					sp0 += nchan;
					sp1 += nchan;
					off -= 2 * nchan;
				}

				sl0 += slb;
				sl1 -= slb;
			}

			slk++;
		}

		sl += scaleY * slb;
		dl += dlb;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* _NO_LONGLONG */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGEFILTEREDSUBSAMPLEFUNC_H */
