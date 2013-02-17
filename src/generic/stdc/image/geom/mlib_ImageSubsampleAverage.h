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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_IMAGESUBSAMPLEAVERAGE_H
#define	_MLIB_IMAGESUBSAMPLEAVERAGE_H

#pragma ident	"@(#)mlib_ImageSubsampleAverage.h	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#define	KH_MAX	4
#define	KW_MAX	4

/* *********************************************************** */

#define	BUFF_SIZE	2048

/* *********************************************************** */

    typedef struct
{
	mlib_s32 *x_ind;
	DTYPE *dl;
	XTYPE *buff;
	mlib_s32 xsize;
	mlib_s32 slb;
	mlib_s32 shift;
	mlib_d64 d_mul;
	mlib_d64 d_ini;
} mlib_SubsAdap;

/* *********************************************************** */

typedef void (*func_y)	(
    YTYPE *buff,
    DTYPE *sp,
    mlib_s32 slb,
    mlib_s32 xsize);

static const func_y func_arr_yi[KH_MAX];
static const func_y func_arr_ya[KH_MAX];

/* *********************************************************** */

typedef void (*func_x) (
    mlib_SubsAdap *ws,
    mlib_u8 *sl);

static const func_x func_arr_xa[KW_MAX * 4];
static const func_x func_arr_xm[KW_MAX * 4];

#ifdef USE_SHIFT
static const func_x func_arr_xs[KW_MAX * 4];
#endif /* USE_SHIFT */

/* *********************************************************** */

mlib_status FUNC(
    main) (
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin)
{
	mlib_SubsAdap ws[1];
	mlib_d64 buff_lcl[BUFF_SIZE / 8], *pbuff = buff_lcl;
	mlib_type type;
	mlib_s32 *x_ind, bchan;
	mlib_s32 i, j, xsize, dsize, ysize, x_ker, y_ker;
	mlib_s32 x_off, y_off, kw = 0, kh;
	mlib_s32 s_width, s_height, d_width, d_height, nchan, slb, dlb;
	mlib_s32 ker_size, ker_shift, rnd_val, ssize, bsize, xsize8;

#ifdef USE_SHIFT
	mlib_s32 ker_mask;
#endif /* USE_SHIFT */
	mlib_d64 dw, dh, x_step, y_step, xx, yy, kmul = 0, p_ini[4], d_ini, tmp,
	    fTol;
	DTYPE *dl, *dp, *sl, *sl_end;
	DTYPE *buffd;
	XTYPE *buffx;
	YTYPE *buffy, *buffY;
	const func_x *func_xa = func_arr_xa;
	const func_x *func_xm = func_arr_xm;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, d_width, d_height, dlb, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, s_width, s_height, slb, sl);
	if (type == MLIB_BIT)
		return (MLIB_FAILURE);
	dlb /= sizeof (DTYPE);
	slb /= sizeof (DTYPE);

	x_step = 1.0 / xscale;
	y_step = 1.0 / yscale;
	yy = 0.5 + ybegin * y_step;

	dw = s_width * xscale;
	dh = s_height * yscale;
	s_height -= (mlib_s32)yy;
	sl += slb * (mlib_s32)yy;
	fTol = xscale / (dw + 1.0);
	tmp = yscale / (dh + 1.0);
	if (tmp < fTol)
		fTol = tmp;
	xsize = (mlib_s32)(dw + fTol);
	ysize = (mlib_s32)(dh + fTol) - ybegin;
	if (xsize > d_width)
		xsize = d_width;
	if (ysize > d_height)
		ysize = d_height;

	if (!xsize)
		return (MLIB_SUCCESS);
	ws->xsize = xsize;
	ws->slb = slb;
	dsize = nchan * xsize;

	x_ker = (mlib_s32)x_step;
	y_ker = (mlib_s32)y_step;
	if (x_ker < x_step)
		x_ker++;
	if (y_ker < y_step)
		y_ker++;
	if (x_ker > s_width)
		x_ker = s_width;
	if (y_ker > s_height)
		y_ker = s_height;
	xx = 0.5;

	ssize = ((mlib_s32)(x_step * xsize) + 2) * nchan;

	xsize8 = (xsize + 3 + 7) & ~7;
	if (nchan == 2) {
		xsize8 *= 2;
	} else if (nchan >= 3) {
#ifndef USE_VIS
		if (nchan == 3)
			xsize8 *= 3;
		else
#endif /* USE_VIS */
			xsize8 *= 4;
	}
	bsize =
	    xsize8 * (sizeof (mlib_s32) + sizeof (XTYPE)) +
	    ssize * sizeof (YTYPE) + 32;
#ifdef USE_VIS
/* buffer for dst line */
	bsize += xsize8 * sizeof (DTYPE);
#endif /* USE_VIS */
	if (bsize > BUFF_SIZE) {
		pbuff = __mlib_malloc(bsize);
		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}
	x_ind = (void *)(pbuff);
	buffd = (void *)(x_ind + xsize8);
#ifdef USE_VIS
	buffx = (void *)(buffd + xsize8);
#else /* USE_VIS */
/* buffd not used */
	buffx = (void *)(buffd);
#endif /* USE_VIS */
	buffY = (void *)(buffx + xsize8);
	ws->buff = buffx;
	ws->x_ind = x_ind;

/* Number of pixels is kernel */
	ker_size = x_ker * y_ker;
	rnd_val = ker_size >> 1;

#ifdef USE_SHIFT
	ker_mask = ker_size & (ker_size - 1);
#if (VIS_GSR_SHIFT0 == 16) && (MLIB_VIS < 0x200)

/* ker_shift = 0 impossible on VIS1 */
	if (ker_size == 1)
		ker_mask = 1;
#endif /* (VIS_GSR_SHIFT0 == 16) && (MLIB_VIS < 0x200) */
/* ker_shift = 0 impossible on VIS1 ) */

/* ker_size is POW of 2 */
	if (!ker_mask) {
		float ftmp[1] = { (float)ker_size };
		ker_shift = ((((int *)ftmp)[0] >> 23) & 0xff) - 127;
		ws->shift = ker_shift;
		func_xm = func_arr_xs;
	} else
#endif /* USE_SHIFT */
	{
		kmul = 1.0 / ((mlib_d64)ker_size);
#if VIS_GSR_SHIFT0 == 7	/* for U8 type */
		{
			mlib_s32 ker_scale = (mlib_s32)(kmul * (1 << 16) + 0.5);

			if (ker_scale >= MLIB_S16_MAX)
				ker_scale = MLIB_S16_MAX;
			kmul = vis_to_double_dup((ker_scale << 16) | ker_scale);
		}
#endif /* VIS_GSR_SHIFT0 == 7 ( for U8 type ) */
#if (VIS_GSR_SHIFT0 == 16) && (MLIB_VIS < 0x200)
/* ker_shift = 0 impossible on VIS1, need to mul by (1 << 16) */
		ker_shift = 16;
#else /* (VIS_GSR_SHIFT0 == 16) && (MLIB_VIS < 0x200) */
		ker_shift = 0;
#endif /* (VIS_GSR_SHIFT0 == 16) && (MLIB_VIS < 0x200) */
		rnd_val = 0;
	}
	ws->d_mul = kmul;

/* Fill x_ind[] */
	bchan = nchan * sizeof (YTYPE);
	for (i = 0; i < xsize; i++) {
		x_ind[i] = bchan * (mlib_s32)xx;
		xx += x_step;
	}

/* Initialize buffx */
	for (i = 0; i < 4; i++) {
		((XTYPE *) p_ini)[i] = (XTYPE) rnd_val;
	}
	d_ini = p_ini[0];
	ws->d_ini = d_ini;
	for (i = 0; i < (mlib_s32)(xsize8 / (8 / sizeof (XTYPE))); i++) {
		((mlib_d64 *)buffx)[i] = d_ini;
	}

/* check boundary of last box */
	if (xsize > 0) {
		if (x_ind[xsize - 1] > bchan * (s_width - x_ker))
			x_ind[xsize - 1] -= bchan;
	}
	x_ind[xsize] = x_ind[xsize + 1] = x_ind[xsize + 2] = x_ind[xsize - 1];
	ssize = (x_ind[xsize - 1] / sizeof (YTYPE)) + x_ker * nchan;

	sl_end = sl + (s_height - y_ker) * slb;

	func_xa += (4 * nchan - 5);
	func_xm += (4 * nchan - 5);

	for (j = 0; j < ysize; j++) {
		if (sl > sl_end)
			sl -= slb;

/* first summation along Y */
		buffy = buffY;
		kh = y_ker;
		if (kh > KH_MAX) {
			if (kh > 2 * KH_MAX)
				kh = KH_MAX;
			else
				kh = kh / 2;
		}
		func_arr_yi[kh - 1] (buffy, sl, slb, ssize);
		sl += kh * slb;

/* additional summation's along Y */
		for (y_off = kh; y_off < y_ker; y_off += kh) {
			kh = y_ker - y_off;
			if (kh > KH_MAX) {
				if (kh > 2 * KH_MAX)
					kh = KH_MAX;
				else
					kh = kh / 2;
			}
			func_arr_ya[kh - 1] (buffy, sl, slb, ssize);
			sl += kh * slb;
		}

#ifdef VIS_GSR_SHIFT0
		vis_write_gsr(((VIS_GSR_SHIFT0 - ker_shift) << 3) | 6);
#endif /* VIS_GSR_SHIFT0 */

/* summation along X */
		for (x_off = 0; x_off < x_ker; x_off += kw) {
			kw = x_ker - x_off;
			if (kw > KW_MAX) {
				if (kw > 2 * KW_MAX)
					kw = KW_MAX;
				else
					kw = kw / 2;
			} else {
				break;
			}
			func_xa[kw] (ws, (void *)buffy);
			buffy += nchan * kw;
		}

/* last summation & divide by kernel size */
		dp = dl;
#ifdef USE_VIS
		if ((((mlib_s32)dl | (dsize * sizeof (DTYPE))) & 3) ||
		    (nchan == 3))
			dp = buffd;
#endif /* USE_VIS */
		ws->dl = dp;
		func_xm[kw] (ws, (void *)buffy);
#ifdef USE_VIS
		if (nchan == 3) {
			FUNC_EXTRACT_43((void *)dp, (void *)dl, xsize);
		} else
#endif /* USE_VIS */
		if (dp != dl) {
			mlib_ImageCopy_na((void *)dp, (void *)dl,
			    dsize * sizeof (DTYPE));
		}

		if ((mlib_s32)(yy + y_step) - (mlib_s32)yy < y_ker)
			sl -= slb;
		yy += y_step;
		dl += dlb;
	}

	if (pbuff != buff_lcl)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGESUBSAMPLEAVERAGE_H */
