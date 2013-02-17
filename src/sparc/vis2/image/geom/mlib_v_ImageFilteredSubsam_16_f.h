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

#ifndef _MLIB_V_IMAGEFILTEREDSUBSAM_16_F_H
#define	_MLIB_V_IMAGEFILTEREDSUBSAM_16_F_H

#pragma ident	"@(#)mlib_v_ImageFilteredSubsam_16_f.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <mlib_v_ImageFilteredSubsample_f.h>

/* *********************************************************** */

    typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#define	CLAMP(max, min)                                         \
	if (sat_tmp >= max)                                     \
	    sat_tmp = max;                                      \
	if (sat_tmp <= min)                                     \
	    sat_tmp = min

/* *********************************************************** */

#if IMG_TYPE == 2

#define	STYPE	mlib_s16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_d64

#define	FS_FUNC(SUFF)	mlib_v_ImageFilteredSubsample_16nw_##SUFF

#ifdef MLIB_USE_FTOI_CLAMPING

#define	DSCALE	65536.0

#define	SAT_OFF	(0.5*DSCALE)

#define	STORE_SAT(res, tmp, x)                                  \
	tmp = (mlib_s32)(x);                                    \
	res = ((tmp) >> 16);

#else /* MLIB_USE_FTOI_CLAMPING */

#define	DSCALE	1.0

#define	SAT_OFF	0.0

#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x);                                \
	                                                        \
	    CLAMP(MLIB_S16_MAX, MLIB_S16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

#elif IMG_TYPE == 3	/* IMG_TYPE == 2 */

#define	STYPE	mlib_u16
#define	ITYPE	mlib_s32
#define	FTYPE	mlib_d64

#define	FS_FUNC(SUFF)	mlib_v_ImageFilteredSubsample_u16nw_##SUFF

#ifdef MLIB_USE_FTOI_CLAMPING

#define	DSCALE	65536.0

#define	SAT_OFF	((0.5 - 32768.0) * DSCALE)

#define	STORE_SAT(res, tmp, x)                                  \
	tmp = (mlib_s32)(x);                                    \
	res = (((tmp) >> 16) ^ 32768);

#else /* MLIB_USE_FTOI_CLAMPING */

#define	DSCALE	1.0

#define	SAT_OFF	-32768.0

#define	STORE_SAT(res, tmp, x)                                  \
	{                                                       \
	    FTYPE sat_tmp = (x) - SAT_OFF;                      \
	                                                        \
	    CLAMP(MLIB_U16_MAX, MLIB_U16_MIN);                  \
	    tmp = (mlib_s32)sat_tmp;                            \
	    res = tmp;                                          \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE == 2 */

/* *********************************************************** */

#define	I2F(x)	(FTYPE)(x)

/* *********************************************************** */

#define	MAX_KERN	32

/* *********************************************************** */

static void FS_FUNC(1x2) (
    FTYPE *pbuff,
    STYPE *sp,
    FTYPE *hKernel,
    mlib_s32 xsize,
    mlib_s32 nchan,
    mlib_s32 step)
{
	FTYPE ss, k0 = hKernel[0];
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp[0] + sp[nchan]);
		pbuff[i] = k0 * ss;
		sp += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(1x2_add) (
    FTYPE *pbuff,
    STYPE *sp,
    FTYPE *hKernel,
    mlib_s32 xsize,
    mlib_s32 nchan,
    mlib_s32 step)
{
	FTYPE ss, k0 = hKernel[0];
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp[0] + sp[nchan]);
		pbuff[i] += k0 * ss;
		sp += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(1x3) (
    FTYPE *pbuff,
    STYPE *sp,
    FTYPE *hKernel,
    mlib_s32 xsize,
    mlib_s32 nchan,
    mlib_s32 step)
{
	FTYPE ss, k0 = hKernel[0], k1 = hKernel[1];
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = I2F((ITYPE) sp[0] + sp[2 * nchan]);
		pbuff[i] = k0 * I2F(sp[nchan]) + k1 * ss;
		sp += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(1x4) (
    FTYPE *pbuff,
    STYPE *sp,
    FTYPE *hKernel,
    mlib_s32 xsize,
    mlib_s32 nchan,
    mlib_s32 step)
{
	FTYPE s0, s1, k0 = hKernel[0], k1 = hKernel[1];
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		s0 = I2F((ITYPE) sp[nchan] + sp[2 * nchan]);
		s1 = I2F((ITYPE) sp[0] + sp[3 * nchan]);
		pbuff[i] = k0 * s0 + k1 * s1;
		sp += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(1x5) (
    FTYPE *pbuff,
    STYPE *sp,
    FTYPE *hKernel,
    mlib_s32 xsize,
    mlib_s32 nchan,
    mlib_s32 step)
{
	FTYPE k0 = hKernel[2], k1 = hKernel[1], k2 = hKernel[0];
	FTYPE s0, s1;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		s0 = I2F((ITYPE) sp[0] + sp[4 * nchan]);
		s1 = I2F((ITYPE) sp[nchan] + sp[3 * nchan]);
		pbuff[i] = k2 * I2F(sp[2 * nchan]) + k1 * s1 + k0 * s0;
		sp += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(y2_add) (
    FTYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 off)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[off];
	FTYPE k0 = vKernel[0];
	FTYPE ss;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = k0 * (buff0[i] + buff1[i]);
		dp[i] += ss;
	}
}

/* *********************************************************** */

#define	SUM(i)	(FTYPE)(SAT_OFF + k0*(buff0[i] + buff1[i]))

static void FS_FUNC(y2) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE k0 = vKernel[0];
	FTYPE ss, val0 = SAT_OFF;
	d64_2x32 dd;

#if IMG_TYPE == 3
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
#endif /* IMG_TYPE == 3 */
	mlib_s32 i;

	i = 0;

	if ((mlib_s32)dp & 3) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
		dp += nchan;
		i++;
	}
#pragma pipeloop(0)
	for (; i <= (xsize - 2); i += 2) {
		dd.i32s.f0 = SUM(i);
		dd.i32s.f1 = SUM(i + 1);
		dd.f32s.f1 = vis_fpackfix(dd.d64);
#if IMG_TYPE == 3
		dd.d64 = vis_fxor(dd.d64, mask8000);
#endif /* IMG_TYPE == 3 */
		vis_st_u16(vis_faligndata(dd.d64, dd.d64), dp);
		vis_st_u16(dd.d64, dp + nchan);
		dp += 2 * nchan;
	}

	if (i < xsize) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
	}
}

/* *********************************************************** */

#undef  SUM
#define	SUM(i)	(val0 + k0*buff1[i] + k1*(buff0[i] + buff2[i]))

static void FS_FUNC(y3) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE *buff2 = buff_arr[2];
	FTYPE k0 = vKernel[0], k1 = vKernel[1];
	FTYPE ss, val0 = SAT_OFF;
	d64_2x32 dd;

#if IMG_TYPE == 3
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
#endif /* IMG_TYPE == 3 */
	mlib_s32 i;

	i = 0;

	if ((mlib_s32)dp & 3) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
		dp += nchan;
		i++;
	}
#pragma pipeloop(0)
	for (; i <= (xsize - 2); i += 2) {
		dd.i32s.f0 = SUM(i);
		dd.i32s.f1 = SUM(i + 1);
		dd.f32s.f1 = vis_fpackfix(dd.d64);
#if IMG_TYPE == 3
		dd.d64 = vis_fxor(dd.d64, mask8000);
#endif /* IMG_TYPE == 3 */
		vis_st_u16(vis_faligndata(dd.d64, dd.d64), dp);
		vis_st_u16(dd.d64, dp + nchan);
		dp += 2 * nchan;
	}

	if (i < xsize) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
	}
}

/* *********************************************************** */

#undef  SUM
#define	SUM(i)	(val0 + k0*(buff1[i] + buff2[i]) + k1*(buff0[i] + buff3[i]))

static void FS_FUNC(y4) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE *buff2 = buff_arr[2];
	FTYPE *buff3 = buff_arr[3];
	FTYPE k0 = vKernel[0], k1 = vKernel[1];
	FTYPE ss, val0 = SAT_OFF;
	d64_2x32 dd;

#if IMG_TYPE == 3
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
#endif /* IMG_TYPE == 3 */
	mlib_s32 i;

	i = 0;

	if ((mlib_s32)dp & 3) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
		dp += nchan;
		i++;
	}
#pragma pipeloop(0)
	for (; i <= (xsize - 2); i += 2) {
		dd.i32s.f0 = SUM(i);
		dd.i32s.f1 = SUM(i + 1);
		dd.f32s.f1 = vis_fpackfix(dd.d64);
#if IMG_TYPE == 3
		dd.d64 = vis_fxor(dd.d64, mask8000);
#endif /* IMG_TYPE == 3 */
		vis_st_u16(vis_faligndata(dd.d64, dd.d64), dp);
		vis_st_u16(dd.d64, dp + nchan);
		dp += 2 * nchan;
	}

	if (i < xsize) {
		ss = SUM(i);
		STORE_SAT(dp[0], buffo[i], ss)
	}
}

/* *********************************************************** */

static void FS_FUNC(y5) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE *buff2 = buff_arr[2];
	FTYPE *buff3 = buff_arr[3];
	FTYPE *buff4 = buff_arr[4];
	FTYPE k0 = vKernel[0], k1 = vKernel[1], k2 = vKernel[2];
	FTYPE ss, val0 = SAT_OFF;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = val0 + k0 * buff2[i] + k1 * (buff1[i] + buff3[i]) +
		    k2 * (buff0[i] + buff4[i]);
		STORE_SAT(dp[i * nchan], buffo[i], ss)
	}
}

/* *********************************************************** */

static void FS_FUNC(y4_add) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan,
    FTYPE * buffd)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE *buff2 = buff_arr[2];
	FTYPE *buff3 = buff_arr[3];
	FTYPE k0 = vKernel[0], k1 = vKernel[1];
	FTYPE ss, val0 = SAT_OFF;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = buffd[i] + k0 * (buff1[i] + buff2[i]) + k1 * (buff0[i] +
		    buff3[i]);
		STORE_SAT(dp[i * nchan], buffo[i], ss)
	}
}

/* *********************************************************** */

static void FS_FUNC(y5_add) (
    STYPE *dp,
    FTYPE *vKernel,
    FTYPE **buff_arr,
    mlib_s32 *buffo,
    mlib_s32 xsize,
    mlib_s32 nchan,
    FTYPE * buffd)
{
	FTYPE *buff0 = buff_arr[0];
	FTYPE *buff1 = buff_arr[1];
	FTYPE *buff2 = buff_arr[2];
	FTYPE *buff3 = buff_arr[3];
	FTYPE *buff4 = buff_arr[4];
	FTYPE k0 = vKernel[0], k1 = vKernel[1], k2 = vKernel[2];
	FTYPE ss, val0 = SAT_OFF;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		ss = buffd[i] + k0 * buff2[i] + k1 * (buff1[i] + buff3[i]) +
		    k2 * (buff0[i] + buff4[i]);
		STORE_SAT(dp[i * nchan], buffo[i], ss)
	}
}

/* *********************************************************** */

mlib_status FS_FUNC(2)  (
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
	STYPE *psrc, *pdst, *sl, *dl, *sp;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_type type;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 kw1 = hSize - hParity;
	mlib_s32 kh1 = vSize - vParity;
	mlib_s32 step;
	FTYPE *buff, *pbuff, *buffd, *buff_arr_lcl[MAX_KERN], **buff_arr =
	    buff_arr_lcl;
	mlib_s32 *buffo, bind, ind;
	FTYPE kern_buff[MAX_KERN], *hKern = kern_buff, *vKern;
	mlib_s32 x_min, x_max, y_min, y_max, xsize;
	mlib_s32 i, j, k, l, off;

#ifdef USE_FP_TBL
	FTYPE ss = 0;

#pragma pipeloop(0)
	for (i = 0; i < 1024; i++) {
		I2F(i) = ss;
		ss += 1.f;
	}

#endif /* USE_FP_TBL */

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dwidth, dheight, dlb, pdst);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, swidth, sheight, slb, psrc);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

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

	buff = buffd =
	    __mlib_malloc(((kh + 1) * sizeof (FTYPE) +
	    sizeof (mlib_s32)) * xsize);

	if (buff == NULL)
		return (MLIB_FAILURE);
	if (2 * kh > MAX_KERN) {
		buff_arr = __mlib_malloc(sizeof (FTYPE *) * 2 * kh);

		if (buff_arr == NULL) {
			__mlib_free(buff);
			return (MLIB_FAILURE);
		}
	}

	for (j = 0; j < kh; j++) {
		buff_arr[j] = buffd;
		buffd += xsize;
	}

	for (j = 0; j < kh; j++)
		buff_arr[kh + j] = buff_arr[j];
	buffo = (mlib_s32 *)(buffd + xsize);

	if (hSize + vSize > MAX_KERN) {
		hKern = __mlib_malloc(sizeof (FTYPE) * (hSize + vSize));

		if (hKern == NULL) {
			if (buff_arr != buff_arr_lcl)
				__mlib_free(buff_arr);
			__mlib_free(buff);
			return (MLIB_FAILURE);
		}
	}

	vKern = hKern + hSize;
	for (l = 0; l < hSize; l++)
		hKern[l] = (FTYPE) hKernel[l];
	for (l = 0; l < vSize; l++)
		vKern[l] = (FTYPE) vKernel[l] * (FTYPE) DSCALE;
/* kw = 1 via kw = 2; kh >= 2*scaleY >= 2 */
	if (kw == 1)
		hKern[0] *= (FTYPE) 0.5;

	step = scaleX * nchan;

	psrc +=
	    (y_min * scaleY + transY) * slb + (x_min * scaleX + transX) * nchan;
	pdst += y_min * dlb + x_min * nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
/* shift >>16 */
	vis_write_gsr((0 << 3) | 6);
#else /* MLIB_USE_FTOI_CLAMPING */
/* without shift */
	vis_write_gsr((16 << 3) | 6);
#endif /* MLIB_USE_FTOI_CLAMPING */

	for (k = 0; k < nchan; k++) {
		sl = psrc + k;
		dl = pdst + k;
		bind = 0;

		for (j = y_min; j <= y_max; j++) {
			mlib_s32 l0;

/* number of lines we have from previous iteration */
			l0 = kh - scaleY;

/* but process all lines in first iteration of 'j' */
			if (j == y_min)
				l0 = 0;

			for (l = l0; l < kh; l++) {
				pbuff = buff_arr[bind + l];

				kw1 = kw;
/* off>0 if kw>=6 */
				off = (kw - 4) >> 1;

				if (off < 0)
					off = 0;
				sp = sl + off * nchan;

				switch (kw - 2 * off) {
				case 1:
					FS_FUNC(1x2) (pbuff, sp, hKern, xsize,
					    0, step);
					break;
				case 2:
					FS_FUNC(1x2) (pbuff, sp, hKern, xsize,
					    nchan, step);
					break;
				case 3:
					FS_FUNC(1x3) (pbuff, sp, hKern, xsize,
					    nchan, step);
					break;
				case 4:
					FS_FUNC(1x4) (pbuff, sp, hKern, xsize,
					    nchan, step);
					break;
				case 5:
					FS_FUNC(1x5) (pbuff, sp, hKern, xsize,
					    nchan, step);
					break;
				}

				if (off) {
					FTYPE *flt = hKern + hSize - off;

					for (; off; off--) {
						sp -= nchan;
						FS_FUNC(1x2_add) (pbuff, sp,
						    flt, xsize,
						    nchan * (kw + 1 - 2 * off),
						    step);
						flt++;
					}
				}

				sl += slb;
			}

			ind = bind;
			off = kh - 1;
			for (kh1 = kh; kh1 > 5; kh1 -= 2) {
				FTYPE *flt = vKern + ((kh1 - 1) >> 1);

/* clear buffd[] before first iteration */
				if (kh1 == kh) {
					FTYPE val0 = SAT_OFF;

#pragma pipeloop(0)
					for (i = 0; i < xsize; i++) {
						buffd[i] = val0;
					}
				}

				FS_FUNC(y2_add) (buffd, flt, buff_arr + ind,
				    buffo, xsize, off);
				ind++;
				off -= 2;
			}

			if (kh1 == kh) {
				switch (kh) {
				case 2:
					FS_FUNC(y2) (dl, vKern, buff_arr + ind,
					    buffo, xsize, nchan);
					break;
				case 3:
					FS_FUNC(y3) (dl, vKern, buff_arr + ind,
					    buffo, xsize, nchan);
					break;
				case 4:
					FS_FUNC(y4) (dl, vKern, buff_arr + ind,
					    buffo, xsize, nchan);
					break;
				case 5:
					FS_FUNC(y5) (dl, vKern, buff_arr + ind,
					    buffo, xsize, nchan);
					break;
				}

			} else {
				switch (kh1) {
				case 4:
					FS_FUNC(y4_add) (dl, vKern,
					    buff_arr + ind, buffo, xsize, nchan,
					    buffd);
					break;
				case 5:
					FS_FUNC(y5_add) (dl, vKern,
					    buff_arr + ind, buffo, xsize, nchan,
					    buffd);
					break;
				}
			}

			bind += scaleY;

			if (bind >= kh)
				bind -= kh;

			dl += dlb;
		}
	}

	if (hKern != kern_buff)
		__mlib_free(hKern);
	if (buff_arr != buff_arr_lcl)
		__mlib_free(buff_arr);
	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEFILTEREDSUBSAM_16_F_H */
