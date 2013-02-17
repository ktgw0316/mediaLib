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

#pragma ident	"@(#)mlib_c_ImageZoomBlend_BL.c	9.3	07/10/09 SMI"

/*
 * Local functions for
 *	mlib_ImageZoomBlend()
 *	mlib_ImageZoomTranslateBlend()
 */

#include <mlib_image.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	IMG_TYPE	1

#define	NO_ZOOM_FUNC

#include <mlib_c_ImageZoom_BL.h>

/* *********************************************************** */

#define	TTYPE	mlib_f32
#define	F_ONE	((TTYPE)1)
#define	I2D(x)	mlib_U82F32[x]

/* *********************************************************** */

#define	BLEND(dst, fres)	dst = (mlib_s32)(a0*(fres - I2D(dst))) + dst

/* *********************************************************** */

#define	BLEND2(dst, fres)	dst = (DTYPE)((a0*fres + a1*I2D(dst))*rw)

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	(((x) << 1) + (x))

mlib_status FUNC_NAME(
    3to34) (
    mlib_work_image * param)
{
	mlib_d64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 width2, size, y_step = 2, l;
	mlib_s32 chan_d = 0, alp_ind = param->alp_ind;
	TTYPE a0 = 0, a1s = 0;
	TTYPE *buffd;

	if (!height)
		return (MLIB_SUCCESS);

	if (alp_ind) {
		chan_d = param->chan_d;
		a0 = (TTYPE) param->alpha;

		if (chan_d == 3)
			a0 *= (F_ONE / 255);
		else
			a1s = F_ONE - a0 * (F_ONE / 255);
	}

	width2 = (width + 1) / 2;
	size =
	    7 * width2 * sizeof (mlib_d64) + 3 * width * sizeof (TTYPE) +
	    ssize * sizeof (mlib_s32) + 8;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width2;
	buff1 = buff0 + 3 * width2;
	buffd = (void *)(buff1 + 3 * width2);
	buffs = (void *)(buffd + 3 * width);

	for (j = 0; j < 3 * width2; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height; j++) {
		mlib_f32 fdy = FTABLE(y);
		mlib_f32 fry = 1.f - fdy;

		if (y_step == 0) {
			mlib_s32 k;

			for (k = 0; k < 3; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					d64_2x32 y0, y1;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					buffd[6 * i] =
					    (fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0);
					buffd[6 * i + 3] =
					    (fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1);
				}

				if (width & 1) {
					d64_2x32 y0, y1;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					buffd[6 * i] =
					    (fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0);
				}

				sp += 1;
				buffd += 1;
				buff0 += width2;
				buff1 += width2;
			}

			sp -= 3;
			buffd -= 3;
			buff0 -= 3 * width2;
			buff1 -= 3 * width2;

		} else
			for (l = 0; l < y_step; l++) {
				mlib_s32 k;
				mlib_d64 *buffT;

/* convert source line to mlib_s32 type */
				COPY_LINE(buffs, sp, ssize);

				buffT = buff0;
				buff0 = buff1;
				buff1 = buffT;

				for (k = 0; k < 3; k++) {
					x = GetElemSubStruct(current,
					    srcX) & MLIB_MASK;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width / 2; i++) {
						mlib_s32 cxh, cxl;
						mlib_f32 xh2, xh3, fxh;
						mlib_f32 xl2, xl3, fxl;
						d64_2x32 y0, y1;

						cxh =
						    MUL_NCHAN(x >> MLIB_SHIFT);
						cxl =
						    MUL_NCHAN((x +
						    dx) >> MLIB_SHIFT);

						fxh = FTABLE(x);
						fxl = FTABLE(x + dx);

						xh2 =
						    (mlib_f32)TABLE(buffs[cxh]);
						xh3 =
						    (mlib_f32)TABLE(buffs[cxh +
						    NCHAN]);
						xl2 =
						    (mlib_f32)TABLE(buffs[cxl]);
						xl3 =
						    (mlib_f32)TABLE(buffs[cxl +
						    NCHAN]);

						y1.f32s.f0 =
						    xh2 + fxh * (xh3 - xh2);
						y1.f32s.f1 =
						    xl2 + fxl * (xl3 - xl2);

						buff1[i] = y1.d64;
						y0.d64 = buff0[i];

						buffd[6 * i] =
						    (fry * y0.f32s.f0 +
						    fdy * y1.f32s.f0);
						buffd[6 * i + 3] =
						    (fry * y0.f32s.f1 +
						    fdy * y1.f32s.f1);

						x += 2 * dx;
					}

					if (width & 1) {
						mlib_s32 cxh;
						mlib_f32 xh2, xh3, fx;
						d64_2x32 y0, y1;

						cxh =
						    MUL_NCHAN(x >> MLIB_SHIFT);
						fx = FTABLE(x);
						xh2 =
						    (mlib_f32)TABLE(buffs[cxh]);
						xh3 =
						    (mlib_f32)TABLE(buffs[cxh +
						    NCHAN]);

						y1.f32s.f0 =
						    xh2 + fx * (xh3 - xh2);
						y1.f32s.f1 = 0.f;
						buff1[i] = y1.d64;
						y0.d64 = buff0[i];

						buffd[6 * i] =
						    (y0.f32s.f0 +
						    fdy * (y1.f32s.f0 -
						    y0.f32s.f0));
					}

					buffd += 1;
					buffs += 1;
					buff0 += width2;
					buff1 += width2;
				}

				buffd -= 3;
				buffs -= 3;
				buff0 -= 3 * width2;
				buff1 -= 3 * width2;
				sp += src_stride;
			}

		if (chan_d == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 3 * width; i++) {
				BLEND(dp[i], buffd[i]);
			}

		} else {	/* if (chan_d == 4) */

			TTYPE a1, w, rw;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				a1 = a1s * dp[4 * i + alp_ind];
				w = a0 + a1;
				rw = F_ONE / w;
				BLEND2(dp[4 * i], buffd[3 * i]);
				BLEND2(dp[4 * i + 1], buffd[3 * i + 1]);
				BLEND2(dp[4 * i + 2], buffd[3 * i + 2]);
				dp[4 * i + alp_ind] = (DTYPE) w;
			}
		}

		if (y_step >= 0) {
			dp += dst_stride;
			y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
			y += dy;
		} else {
			y_step = 1;
		}
	}

	if (buff != buff_arr)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	RSLT(x)	RES(x)

/* *********************************************************** */

#undef  RTYPE
#define	RTYPE	FP_TYPE

/* *********************************************************** */

#undef  STOR
#define	STOR(N, index)	buff[index] = res##N

/* *********************************************************** */

#define	BLEND33(dst, fres)	dst += (mlib_s32)(a0*(fres - I2D(dst)))

/* *********************************************************** */

#define	BLEND43(dst, fres)	dst += (mlib_s32)(a0*fres - a1*I2D(dst))

/* *********************************************************** */

#define	BLEND34(dst, fres)	dst += (DTYPE)((fres - I2D(dst))*rw)

#define	BLEND34z(dst, fres)	dst = (DTYPE)((w0*fres + w1*I2D(dst))*rw)

/* *********************************************************** */

#define	BLEND44(dst, fres)                                      \
	dst =                                                   \
	    mlib_sat_u8[dst + (mlib_s32)((dalpha * fres -       \
	    w0 * I2D(dst)) * rw)]

#define	BLEND44z(dst, fres)                                     \
	dst =                                                   \
	    mlib_sat_u8[(mlib_s32)((dalpha * fres +             \
	    w1 * I2D(dst)) * rw)]

/* *********************************************************** */

#define	STORE(dst, fres)	dst = (DTYPE)fres

/* *********************************************************** */

#define	BUFF_TYPE	FP_TYPE

/* *********************************************************** */

mlib_status FUNC_NAME(
    33) (
    mlib_work_image * param,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;
	FP_TYPE a0 = (param->alpha) * (F_ONE / 255);
	BUFF_TYPE *buff;

	buff = __mlib_malloc(3 * width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			fdx = FTABLE(x);

			LOAD(0, 3);
			LOAD(1, 4);
			LOAD(2, 5);
			CALC(0);
			CALC(1);
			CALC(2);
			RSLT(0);
			RSLT(1);
			RSLT(2);
			STOR(0, 3 * i);
			STOR(1, 3 * i + 1);
			STOR(2, 3 * i + 2);

			x += dx;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 3 * width; i++) {
			BLEND33(dp[i], buff[i]);
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_FLT_ALPHA                                           \
	fdy0 = rdy * a0alp;                                     \
	fdy1 = rdy * a1alp;                                     \
	fdy2 = ady * a2alp;                                     \
	fdy3 = ady * a3alp

#define	CALCA(N)                                                \
	r0##N = fdy0 * a0##N + fdy3 * a3##N;                    \
	r1##N = fdy1 * a1##N + fdy2 * a2##N

/* *********************************************************** */

mlib_status FUNC_NAME(
    43) (
    mlib_work_image * param,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(alp);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy, ady, rdy, fdy0, fdy1, fdy2, fdy3;
	FP_TYPE a0 = (param->alpha) * (F_ONE / 255), a1, dalpha =
	    a0 * (F_ONE / 255);
	BUFF_TYPE *buff;

	buff = __mlib_malloc(4 * width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);
		ady = fdy * (F_ONE / 255);
		rdy = (FP_TYPE) ((1.0 - fdy) * (F_ONE / 255));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE(x);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(alp, alp + 4);
			MUL_FLT_ALPHA;
			CALCA(0);
			CALCA(1);
			CALCA(2);
			CALC(alp);
			RSLT(0);
			RSLT(1);
			RSLT(2);
			RSLT(alp);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			buff[4 * i + 3] = resalp;

			x += dx;
		}

		if (a0 < 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				STORE(dp[3 * i], buff[4 * i]);
				STORE(dp[3 * i + 1], buff[4 * i + 1]);
				STORE(dp[3 * i + 2], buff[4 * i + 2]);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				a1 = dalpha * buff[4 * i + 3];
				BLEND43(dp[3 * i], buff[4 * i]);
				BLEND43(dp[3 * i + 1], buff[4 * i + 1]);
				BLEND43(dp[3 * i + 2], buff[4 * i + 2]);
			}
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if 1

/* *********************************************************** */

#undef  FP_TYPE
#define	FP_TYPE	mlib_d64

/* *********************************************************** */

#undef  TABLE
#define	TABLE(A)	mlib_U82D64[(A)]

/* *********************************************************** */

#undef  FTABLE
#define	FTABLE(A)	mlib_bl_d64_tbl[((A) >> 8) & 0xFF]

const mlib_d64 mlib_bl_d64_tbl[] = {
	0.00000000, 0.00390625, 0.00781250, 0.01171875,
	0.01562500, 0.01953125, 0.02343750, 0.02734375,
	0.03125000, 0.03515625, 0.03906250, 0.04296875,
	0.04687500, 0.05078125, 0.05468750, 0.05859375,
	0.06250000, 0.06640625, 0.07031250, 0.07421875,
	0.07812500, 0.08203125, 0.08593750, 0.08984375,
	0.09375000, 0.09765625, 0.10156250, 0.10546875,
	0.10937500, 0.11328125, 0.11718750, 0.12109375,
	0.12500000, 0.12890625, 0.13281250, 0.13671875,
	0.14062500, 0.14453125, 0.14843750, 0.15234375,
	0.15625000, 0.16015625, 0.16406250, 0.16796875,
	0.17187500, 0.17578125, 0.17968750, 0.18359375,
	0.18750000, 0.19140625, 0.19531250, 0.19921875,
	0.20312500, 0.20703125, 0.21093750, 0.21484375,
	0.21875000, 0.22265625, 0.22656250, 0.23046875,
	0.23437500, 0.23828125, 0.24218750, 0.24609375,
	0.25000000, 0.25390625, 0.25781250, 0.26171875,
	0.26562500, 0.26953125, 0.27343750, 0.27734375,
	0.28125000, 0.28515625, 0.28906250, 0.29296875,
	0.29687500, 0.30078125, 0.30468750, 0.30859375,
	0.31250000, 0.31640625, 0.32031250, 0.32421875,
	0.32812500, 0.33203125, 0.33593750, 0.33984375,
	0.34375000, 0.34765625, 0.35156250, 0.35546875,
	0.35937500, 0.36328125, 0.36718750, 0.37109375,
	0.37500000, 0.37890625, 0.38281250, 0.38671875,
	0.39062500, 0.39453125, 0.39843750, 0.40234375,
	0.40625000, 0.41015625, 0.41406250, 0.41796875,
	0.42187500, 0.42578125, 0.42968750, 0.43359375,
	0.43750000, 0.44140625, 0.44531250, 0.44921875,
	0.45312500, 0.45703125, 0.46093750, 0.46484375,
	0.46875000, 0.47265625, 0.47656250, 0.48046875,
	0.48437500, 0.48828125, 0.49218750, 0.49609375,
	0.50000000, 0.50390625, 0.50781250, 0.51171875,
	0.51562500, 0.51953125, 0.52343750, 0.52734375,
	0.53125000, 0.53515625, 0.53906250, 0.54296875,
	0.54687500, 0.55078125, 0.55468750, 0.55859375,
	0.56250000, 0.56640625, 0.57031250, 0.57421875,
	0.57812500, 0.58203125, 0.58593750, 0.58984375,
	0.59375000, 0.59765625, 0.60156250, 0.60546875,
	0.60937500, 0.61328125, 0.61718750, 0.62109375,
	0.62500000, 0.62890625, 0.63281250, 0.63671875,
	0.64062500, 0.64453125, 0.64843750, 0.65234375,
	0.65625000, 0.66015625, 0.66406250, 0.66796875,
	0.67187500, 0.67578125, 0.67968750, 0.68359375,
	0.68750000, 0.69140625, 0.69531250, 0.69921875,
	0.70312500, 0.70703125, 0.71093750, 0.71484375,
	0.71875000, 0.72265625, 0.72656250, 0.73046875,
	0.73437500, 0.73828125, 0.74218750, 0.74609375,
	0.75000000, 0.75390625, 0.75781250, 0.76171875,
	0.76562500, 0.76953125, 0.77343750, 0.77734375,
	0.78125000, 0.78515625, 0.78906250, 0.79296875,
	0.79687500, 0.80078125, 0.80468750, 0.80859375,
	0.81250000, 0.81640625, 0.82031250, 0.82421875,
	0.82812500, 0.83203125, 0.83593750, 0.83984375,
	0.84375000, 0.84765625, 0.85156250, 0.85546875,
	0.85937500, 0.86328125, 0.86718750, 0.87109375,
	0.87500000, 0.87890625, 0.88281250, 0.88671875,
	0.89062500, 0.89453125, 0.89843750, 0.90234375,
	0.90625000, 0.91015625, 0.91406250, 0.91796875,
	0.92187500, 0.92578125, 0.92968750, 0.93359375,
	0.93750000, 0.94140625, 0.94531250, 0.94921875,
	0.95312500, 0.95703125, 0.96093750, 0.96484375,
	0.96875000, 0.97265625, 0.97656250, 0.98046875,
	0.98437500, 0.98828125, 0.99218750, 0.99609375,
	1.00000000, 1.00390625, 1.00781250, 1.01171875,
	1.01562500, 1.01953125, 1.02343750, 1.02734375,
	1.03125000, 1.03515625, 1.03906250, 1.04296875,
	1.04687500, 1.05078125, 1.05468750, 1.05859375,
	1.06250000, 1.06640625, 1.07031250, 1.07421875,
	1.07812500, 1.08203125, 1.08593750, 1.08984375,
	1.09375000, 1.09765625, 1.10156250, 1.10546875,
	1.10937500, 1.11328125, 1.11718750, 1.12109375,
	1.12500000, 1.12890625, 1.13281250, 1.13671875,
	1.14062500, 1.14453125, 1.14843750, 1.15234375,
	1.15625000, 1.16015625, 1.16406250, 1.16796875,
	1.17187500, 1.17578125, 1.17968750, 1.18359375,
	1.18750000, 1.19140625, 1.19531250, 1.19921875,
	1.20312500, 1.20703125, 1.21093750, 1.21484375,
	1.21875000, 1.22265625, 1.22656250, 1.23046875,
	1.23437500, 1.23828125, 1.24218750, 1.24609375,
	1.25000000, 1.25390625, 1.25781250, 1.26171875,
	1.26562500, 1.26953125, 1.27343750, 1.27734375,
	1.28125000, 1.28515625, 1.28906250, 1.29296875,
	1.29687500, 1.30078125, 1.30468750, 1.30859375,
	1.31250000, 1.31640625, 1.32031250, 1.32421875,
	1.32812500, 1.33203125, 1.33593750, 1.33984375,
	1.34375000, 1.34765625, 1.35156250, 1.35546875,
	1.35937500, 1.36328125, 1.36718750, 1.37109375,
	1.37500000, 1.37890625, 1.38281250, 1.38671875,
	1.39062500, 1.39453125, 1.39843750, 1.40234375,
	1.40625000, 1.41015625, 1.41406250, 1.41796875,
	1.42187500, 1.42578125, 1.42968750, 1.43359375,
	1.43750000, 1.44140625, 1.44531250, 1.44921875,
	1.45312500, 1.45703125, 1.46093750, 1.46484375,
	1.46875000, 1.47265625, 1.47656250, 1.48046875,
	1.48437500, 1.48828125, 1.49218750, 1.49609375,
	1.50000000, 1.50390625, 1.50781250, 1.51171875,
	1.51562500, 1.51953125, 1.52343750, 1.52734375,
	1.53125000, 1.53515625, 1.53906250, 1.54296875,
	1.54687500, 1.55078125, 1.55468750, 1.55859375,
	1.56250000, 1.56640625, 1.57031250, 1.57421875,
	1.57812500, 1.58203125, 1.58593750, 1.58984375,
	1.59375000, 1.59765625, 1.60156250, 1.60546875,
	1.60937500, 1.61328125, 1.61718750, 1.62109375,
	1.62500000, 1.62890625, 1.63281250, 1.63671875,
	1.64062500, 1.64453125, 1.64843750, 1.65234375,
	1.65625000, 1.66015625, 1.66406250, 1.66796875,
	1.67187500, 1.67578125, 1.67968750, 1.68359375,
	1.68750000, 1.69140625, 1.69531250, 1.69921875,
	1.70312500, 1.70703125, 1.71093750, 1.71484375,
	1.71875000, 1.72265625, 1.72656250, 1.73046875,
	1.73437500, 1.73828125, 1.74218750, 1.74609375,
	1.75000000, 1.75390625, 1.75781250, 1.76171875,
	1.76562500, 1.76953125, 1.77343750, 1.77734375,
	1.78125000, 1.78515625, 1.78906250, 1.79296875,
	1.79687500, 1.80078125, 1.80468750, 1.80859375,
	1.81250000, 1.81640625, 1.82031250, 1.82421875,
	1.82812500, 1.83203125, 1.83593750, 1.83984375,
	1.84375000, 1.84765625, 1.85156250, 1.85546875,
	1.85937500, 1.86328125, 1.86718750, 1.87109375,
	1.87500000, 1.87890625, 1.88281250, 1.88671875,
	1.89062500, 1.89453125, 1.89843750, 1.90234375,
	1.90625000, 1.91015625, 1.91406250, 1.91796875,
	1.92187500, 1.92578125, 1.92968750, 1.93359375,
	1.93750000, 1.94140625, 1.94531250, 1.94921875,
	1.95312500, 1.95703125, 1.96093750, 1.96484375,
	1.96875000, 1.97265625, 1.97656250, 1.98046875,
	1.98437500, 1.98828125, 1.99218750, 1.99609375,
	2.00000000
};

#endif /* 1 */

/* *********************************************************** */

mlib_status FUNC_NAME(
    34) (
    mlib_work_image * param,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;
	mlib_blend blend = param->blend;
	FP_TYPE w0 = param->alpha;
	FP_TYPE w1s = F_ONE - w0 * (F_ONE / 255);
	FP_TYPE w1, w, rw;
	BUFF_TYPE *buff;

	buff = __mlib_malloc(3 * width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			fdx = FTABLE(x);

			LOAD(0, 3);
			LOAD(1, 4);
			LOAD(2, 5);
			CALC(0);
			CALC(1);
			CALC(2);
			RSLT(0);
			RSLT(1);
			RSLT(2);

			STOR(0, 3 * i);
			STOR(1, 3 * i + 1);
			STOR(2, 3 * i + 2);

			x += dx;
		}

		if (blend == MLIB_BLEND_GTK_SRC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				dp[4 * i] = (DTYPE) buff[3 * i];
				dp[4 * i + 1] = (DTYPE) buff[3 * i + 1];
				dp[4 * i + 2] = (DTYPE) buff[3 * i + 2];
				dp[4 * i + alp] = 255;
			}
		} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				w1 = w1s * dp[4 * i + alp];
				w = w0 + w1;
				FP_DIV(rw, w0, w);

				BLEND34(dp[4 * i], buff[3 * i]);
				BLEND34(dp[4 * i + 1], buff[3 * i + 1]);
				BLEND34(dp[4 * i + 2], buff[3 * i + 2]);
				dp[4 * i + alp] = (DTYPE) w;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				w1 = w1s * dp[4 * i + alp];
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND34z(dp[4 * i], buff[3 * i]);
				BLEND34z(dp[4 * i + 1], buff[3 * i + 1]);
				BLEND34z(dp[4 * i + 2], buff[3 * i + 2]);
				dp[4 * i + alp] = (DTYPE) w;
			}
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    44) (
    mlib_work_image * param,
    mlib_s32 alp)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(alp);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy, ady, rdy, fdy0, fdy1, fdy2, fdy3;
	mlib_blend blend = param->blend;
	FP_TYPE dalpha = param->alpha, dalpha1 = dalpha * (F_ONE / 255);
	FP_TYPE w0, w1, rw, w;
	BUFF_TYPE *buff;

	buff = __mlib_malloc(4 * width * sizeof (BUFF_TYPE));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);
		ady = fdy * (F_ONE / 255);
		rdy = (1.0 - fdy) * (F_ONE / 255);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE(x);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(alp, alp + 4);
			MUL_FLT_ALPHA;
			CALCA(0);
			CALCA(1);
			CALCA(2);
			CALC(alp);
			RSLT(0);
			RSLT(1);
			RSLT(2);
			RSLT(alp);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			buff[4 * i + 3] = resalp;

			x += dx;
		}

		if (blend == MLIB_BLEND_GTK_SRC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				w = buff[4 * i + 3];
				FP_DIV(rw, 255, w);

				dp[4 * i] = (DTYPE) (rw * buff[4 * i]);
				dp[4 * i + 1] = (DTYPE) (rw * buff[4 * i + 1]);
				dp[4 * i + 2] = (DTYPE) (rw * buff[4 * i + 2]);
				dp[4 * i + alp] = (DTYPE) buff[4 * i + 3];
			}
		} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				w0 = dalpha1 * buff[4 * i + 3];
				w1 = (F_ONE - w0 * (F_ONE / 255)) * dp[4 * i +
				    alp];
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND44(dp[4 * i], buff[4 * i]);
				BLEND44(dp[4 * i + 1], buff[4 * i + 1]);
				BLEND44(dp[4 * i + 2], buff[4 * i + 2]);
				dp[4 * i + alp] = (DTYPE) w;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				w0 = dalpha1 * buff[4 * i + 3];
				w1 = (F_ONE - w0 * (F_ONE / 255)) * dp[4 * i +
				    alp];
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND44z(dp[4 * i], buff[4 * i]);
				BLEND44z(dp[4 * i + 1], buff[4 * i + 1]);
				BLEND44z(dp[4 * i + 2], buff[4 * i + 2]);
				dp[4 * i + alp] = (DTYPE) w;
			}
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
