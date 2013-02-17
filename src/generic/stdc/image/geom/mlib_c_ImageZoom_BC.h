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

#ifndef _MLIB_C_IMAGEZOOM_BC_H
#define	_MLIB_C_IMAGEZOOM_BC_H

#pragma ident	"@(#)mlib_c_ImageZoom_BC.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_ImageDivTables.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#define	FLT_TYPE	mlib_f32
#define	TTYPE	mlib_f32
#define	F_ONE	((TTYPE)1)
#define	I2D(x)	mlib_U82F32[x]

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	FUNC_NAME(CC)	mlib_c_ImageZoomBicubic_U8_##CC
#define	DTYPE	mlib_u8
#define	TBL_BITS	8
#define	TABLE(x)	(mlib_U82F32[(x)])
#define	STORE(mem, itmp, x)                                     \
	x -= (TTYPE) 0x7F800000;                                \
	D2I(itmp, x);                                           \
	mem = (itmp >> 24) ^ 0x80
#define	_SPLITx2(r0, r1, px)                                    \
	mlib_s32 x = *(mlib_u16 *)(px);                         \
	                                                        \
	r0 = x >> 8;                                            \
	r1 = x & 0xFF

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	FUNC_NAME(CC)	mlib_c_ImageZoomBicubic_S16_##CC
#define	DTYPE	mlib_s16
#define	TBL_BITS	9
#define	TABLE(x)	(x)
#define	STORE(mem, itmp, x)	D2I(itmp, x); mem = (itmp >> 16)
#define	_SPLITx2(r0, r1, px)                                    \
	mlib_s32 x = *(mlib_s32 *)(px);                         \
	                                                        \
	r0 = x >> 16;                                           \
	r1 = (x << 16) >> 16

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	FUNC_NAME(CC)	mlib_c_ImageZoomBicubic_U16_##CC
#define	DTYPE	mlib_u16
#define	TBL_BITS	9
#define	TABLE(x)	(x)
#define	STORE(mem, itmp, x)                                     \
	x -= (TTYPE) 0x7FFF8000;                                \
	D2I(itmp, x);                                           \
	mem = (itmp >> 16) ^ 0x8000
#define	_SPLITx2(r0, r1, px)                                    \
	mlib_u32 x = *(mlib_u32 *)(px);                         \
	                                                        \
	r0 = x >> 16;                                           \
	r1 = x & 0xFFFF

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN
#define	SPLITx2(r0, r1, px)	_SPLITx2(r0, r1, px)
#else		   /* _LITTLE_ENDIAN */
#define	SPLITx2(r0, r1, px)	_SPLITx2(r1, r0, px)
#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING
#define	D2I(res, x)	res = (mlib_s32)x
#else		   /* MLIB_USE_FTOI_CLAMPING */
#define	D2I(res, x)                                             \
	{                                                       \
	    mlib_d64 xx = x;                                    \
	                                                        \
	    if (xx >= (TTYPE) MLIB_S32_MAX)                     \
		xx = MLIB_S32_MAX;                              \
	    if (xx <= (TTYPE) MLIB_S32_MIN)                     \
		xx = MLIB_S32_MIN;                              \
	    res = (mlib_s32)xx;                                 \
	}
#endif		   /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	DEF_VAR(NCHAN)                                               \
	DTYPE *sl = GetElemSubStruct(current, sp);                   \
	DTYPE *dl = GetElemSubStruct(current, dp);                   \
	TTYPE xf0, xf1, xf2, xf3;                                    \
	TTYPE yf0, yf1, yf2, yf3;                                    \
	TTYPE h0, h1, h2, h3, res;                                   \
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2,   \
		*buff3;                                              \
	mlib_s32 i, j, k, x, y_step = 4, bind = 0,                   \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width1 = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height),          \
		width = NCHAN * width1;                              \
	mlib_s32 ssize = NCHAN * (param->sline_size);                \
	TTYPE yd = (TTYPE) GetElemStruct(ystart);                    \
	TTYPE ady = (TTYPE) (1.0 / GetElemStruct(zoomy));            \
	mlib_s32 *ibuff;                                             \
	mlib_s32 x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;   \
	mlib_s32 ibuff_size, filterpos, dx = GetElemStruct(DX);      \
	const FLT_TYPE *fPtr

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dp = GetElemSubStruct(current, dp);                 \
	FLT_TYPE *fPtr;                                              \
	TTYPE xf0, xf1, xf2, xf3,                                    \
		yf0, yf1, yf2, yf3;                                  \
	mlib_s32 i, j, cx,                                           \
		filterpos,                                           \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x, y,                                                \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#define	VARIABLE_BC(N)	TTYPE k0##N, k1##N, k2##N, k3##N, fres

/* *********************************************************** */

#define	MLIB_XY_SHIFT	(12 - TBL_BITS)
#define	MLIB_XY_MASK	(((1 << TBL_BITS) - 1) << 4)

/* *********************************************************** */

#define	MLIB_FILTER(X, filter)                                  \
	filterpos = ((X) >> MLIB_XY_SHIFT) & MLIB_XY_MASK;      \
	fPtr = (FLT_TYPE *) ((mlib_u8 *)filter + filterpos);    \
	X##f0 = fPtr[0];                                        \
	X##f1 = fPtr[1];                                        \
	X##f2 = fPtr[2];                                        \
	X##f3 = fPtr[3]

/* *********************************************************** */

#define	CALCULATION(a0, a1, a2, a3)                             \
	xf0 * TABLE(tt[a0]) + xf1 * TABLE(tt[a1]) +             \
	xf2 * TABLE(tt[a2]) + xf3 * TABLE(tt[a3])

/* *********************************************************** */

#define	MLIB_LIMITS_BC(N)                                       \
	fres =                                                  \
	    k0##N * yf0 + k1##N * yf1 + k2##N * yf2 +           \
	    k3##N * yf3;                                        \
	STORE(ires, ires, fres);                                \
	tdp[N] = ires

/* *********************************************************** */

#define	BC_SUM(NCHAN, k)                                               \
	xf0 *sp[k] + xf1 * sp[k + NCHAN] + xf2 * sp[k + 2 * NCHAN] +   \
	    xf3 * sp[k + 3 * NCHAN]

/* *********************************************************** */

#define	SUM_Y(fres, i)                                           \
	fres =                                                   \
	    yf0 * buff0[i] + yf1 * buff1[i] + yf2 * buff2[i] +   \
	    yf3 * buff3[i];                                      \
	fres *= 1.f / (1 << 24)

/* *********************************************************** */

#define	BLEND(dst, fres)	dst = (mlib_s32)(a0*(fres - I2D(dst))) + dst

/* *********************************************************** */

#define	BLEND34(dst, fres)	dst += (mlib_s32)((fres - I2D(dst))*rw)
#define	BLEND34z(dst, fres)	dst = (DTYPE)((a0*fres + a1*I2D(dst))*rw)

/* *********************************************************** */

#ifndef NO_ZOOM_FUNC
#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	DEF_VAR(NCHAN);

	ibuff_size = (ssize > width) ? ssize : width;
	buff =
	    __mlib_malloc(4 * width * sizeof (TTYPE) +
	    ibuff_size * sizeof (mlib_s32) + 8);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	ibuff = (mlib_s32 *)(buff + 4 * width);

	sl -= src_stride + NCHAN;

	for (j = 0; j < height; j++) {
		filterpos =
		    (mlib_s32)(yd * (1 << TBL_BITS)) & ((1 << TBL_BITS) - 1);
		fPtr = flt_table + 4 * filterpos;
		yf0 = fPtr[0];
		yf1 = fPtr[1];
		yf2 = fPtr[2];
		yf3 = fPtr[3];

		for (k = 4 - y_step; k < 4; k++) {
/* convert source line to mlib_s32 type */
			ibuff[0] = sl[0];
			i = ((int)sl >> (sizeof (DTYPE) - 1)) & 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < ssize; i += 2) {
				SPLITx2(ibuff[i], ibuff[i + 1], sl + i);
			}

/* sum on X */
			x = x0;
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width1; i++) {
				mlib_s32 *sp =
				    ibuff + NCHAN * (x >> MLIB_SHIFT);

				MLIB_FILTER(x, flt_table);
				x += dx;

				pbuff[NCHAN * i] = BC_SUM(NCHAN, 0);
			}

			sl += src_stride;
		}

/* sum on Y */
		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], ibuff[i], res);
		}

		y_step = (mlib_s32)(yd + ady) - (mlib_s32)yd;
		yd += ady;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	DEF_VAR(NCHAN);

	ibuff_size = (ssize > width) ? ssize : width;
	buff =
	    __mlib_malloc(4 * width * sizeof (TTYPE) +
	    ibuff_size * sizeof (mlib_s32) + 8);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	ibuff = (mlib_s32 *)(buff + 4 * width);

	sl -= src_stride + NCHAN;

	for (j = 0; j < height; j++) {
		filterpos =
		    (mlib_s32)(yd * (1 << TBL_BITS)) & ((1 << TBL_BITS) - 1);
		fPtr = flt_table + 4 * filterpos;
		yf0 = fPtr[0];
		yf1 = fPtr[1];
		yf2 = fPtr[2];
		yf3 = fPtr[3];

		for (k = 4 - y_step; k < 4; k++) {
/* convert source line to mlib_s32 type */
			ibuff[0] = sl[0];
			i = ((int)sl >> (sizeof (DTYPE) - 1)) & 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < ssize; i += 2) {
				SPLITx2(ibuff[i], ibuff[i + 1], sl + i);
			}

/* sum on X */
			x = x0;
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width1; i++) {
				mlib_s32 *sp =
				    ibuff + NCHAN * (x >> MLIB_SHIFT);

				MLIB_FILTER(x, flt_table);
				x += dx;

				pbuff[NCHAN * i] = BC_SUM(NCHAN, 0);
				pbuff[NCHAN * i + 1] = BC_SUM(NCHAN, 1);
			}

			sl += src_stride;
		}

/* sum on Y */
		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], ibuff[i], res);
		}

		y_step = (mlib_s32)(yd + ady) - (mlib_s32)yd;
		yd += ady;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	DEF_VAR(NCHAN);
	mlib_s32 chan_d = 0, alp_ind = param->alp_ind;
	mlib_blend blend = MLIB_BLEND_GTK_SRC;
	TTYPE a0 = 0, a1s = 0;

	if (alp_ind) {
		chan_d = param->chan_d;
		blend = param->blend;
		a0 = (TTYPE) param->alpha;
		if (chan_d == 3)
			a0 *= (F_ONE / 255);
		else
			a1s = F_ONE - a0 * (F_ONE / 255);
	}

	ibuff_size = (ssize > width) ? ssize : width;
	buff =
	    __mlib_malloc(4 * width * sizeof (TTYPE) +
	    ibuff_size * sizeof (mlib_s32) + 8);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	ibuff = (mlib_s32 *)(buff + 4 * width);

	sl -= src_stride + NCHAN;

	for (j = 0; j < height; j++) {
		filterpos =
		    (mlib_s32)(yd * (1 << TBL_BITS)) & ((1 << TBL_BITS) - 1);
		fPtr = flt_table + 4 * filterpos;
		yf0 = fPtr[0];
		yf1 = fPtr[1];
		yf2 = fPtr[2];
		yf3 = fPtr[3];

		for (k = 4 - y_step; k < 4; k++) {
/* convert source line to mlib_s32 type */
			ibuff[0] = sl[0];
			i = ((int)sl >> (sizeof (DTYPE) - 1)) & 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < ssize; i += 2) {
				SPLITx2(ibuff[i], ibuff[i + 1], sl + i);
			}

/* sum on X */
			x = x0;
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width1; i++) {
				mlib_s32 *sp =
				    ibuff + NCHAN * (x >> MLIB_SHIFT);

				MLIB_FILTER(x, flt_table);
				x += dx;

				pbuff[NCHAN * i] = BC_SUM(NCHAN, 0);
				pbuff[NCHAN * i + 1] = BC_SUM(NCHAN, 1);
				pbuff[NCHAN * i + 2] = BC_SUM(NCHAN, 2);
			}

			sl += src_stride;
		}

/* sum on Y */
		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		if (!alp_ind) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				h0 = buff0[i];
				h1 = buff1[i];
				h2 = buff2[i];
				h3 = buff3[i];
				res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
				STORE(dl[i], ibuff[i], res);
			}
		} else {
			if (chan_d == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_Y(res, i);
					if (res <= MLIB_U8_MIN)
						res = MLIB_U8_MIN;
					if (res >= MLIB_U8_MAX)
						res = MLIB_U8_MAX;
					BLEND(dl[i], res);
				}
			} else {	/* if (chan_d == 4) */

				TTYPE a1, w, rw;
				mlib_f32 *buffd = (mlib_f32 *)ibuff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					SUM_Y(res, i);
					if (res <= MLIB_U8_MIN)
						res = MLIB_U8_MIN;
					if (res >= MLIB_U8_MAX)
						res = MLIB_U8_MAX;
					buffd[i] = res;
				}

				if (blend == MLIB_BLEND_GTK_SRC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						dl[4 * i] =
						    (DTYPE) buffd[3 * i];
						dl[4 * i + 1] =
						    (DTYPE) buffd[3 * i + 1];
						dl[4 * i + 2] =
						    (DTYPE) buffd[3 * i + 2];
						dl[4 * i + alp_ind] = 255;
					}
				} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						a1 = a1s * dl[4 * i + alp_ind];
						w = a0 + a1;
						FP_INV(rw, w);
						rw *= a0;

						BLEND34(dl[4 * i],
						    buffd[3 * i]);
						BLEND34(dl[4 * i + 1],
						    buffd[3 * i + 1]);
						BLEND34(dl[4 * i + 2],
						    buffd[3 * i + 2]);
						dl[4 * i + alp_ind] = (DTYPE) w;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						a1 = a1s * dl[4 * i + alp_ind];
						w = a0 + a1;
						FP_INV(rw, w);

						BLEND34z(dl[4 * i],
						    buffd[3 * i]);
						BLEND34z(dl[4 * i + 1],
						    buffd[3 * i + 1]);
						BLEND34z(dl[4 * i + 2],
						    buffd[3 * i + 2]);
						dl[4 * i + alp_ind] = (DTYPE) w;
					}
				}
			}
		}

		y_step = (mlib_s32)(yd + ady) - (mlib_s32)yd;
		yd += ady;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	4

/* *********************************************************** */

#define	BLEND43(dst, fres)	SATUR_U8(dst, a0*fres + a1*I2D(dst))

/* *********************************************************** */

#define	BLEND44(dst, fres)                                             \
	SATUR_U8(dst, dst + ((dalpha0 * fres - w0 * I2D(dst)) * rw))

#define	BLEND44z(dst, fres)                                     \
	SATUR_U8(dst, (dalpha0 * fres + w1 * I2D(dst)) * rw)

/* *********************************************************** */

#define	BLEND0(dst, fres)	dst = (DTYPE)fres

/* *********************************************************** */

#define	BLEND_GTK_SRC(dst, fres)	SATUR_U8(dst, fres*rw)

/* *********************************************************** */

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	DEF_VAR(NCHAN);
	mlib_s32 chan_d = 0, alp_ind = param->alp_ind;
	mlib_blend blend = MLIB_BLEND_GTK_SRC;
	TTYPE *sbuff;
	TTYPE a0 = 0, a1, dalpha = 0, dalpha0 = 0;

	if (alp_ind) {
		chan_d = param->chan_d;
		blend = param->blend;
		dalpha0 = (TTYPE) param->alpha;
		a0 = dalpha0 * (F_ONE / 255);
		dalpha = a0 * (F_ONE / 255);
	}

	ibuff_size = (ssize > width) ? ssize : width;
	buff =
	    __mlib_malloc((4 * width + ssize + 1) * sizeof (TTYPE) +
	    (ibuff_size + 1) * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	sbuff = buff + 4 * width;
	ibuff = (mlib_s32 *)(sbuff + ssize + 1);

	sl -= src_stride + NCHAN;

	if (alp_ind == -1)
		sl--;

	for (j = 0; j < height; j++) {
		filterpos =
		    (mlib_s32)(yd * (1 << TBL_BITS)) & ((1 << TBL_BITS) - 1);
		fPtr = flt_table + 4 * filterpos;
		yf0 = fPtr[0];
		yf1 = fPtr[1];
		yf2 = fPtr[2];
		yf3 = fPtr[3];

		for (k = 4 - y_step; k < 4; k++) {
/* convert source line to mlib_s32 type */
			ibuff[0] = sl[0];
			i = ((int)sl >> (sizeof (DTYPE) - 1)) & 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < ssize; i += 2) {
				SPLITx2(ibuff[i], ibuff[i + 1], sl + i);
			}

			if (!alp_ind) {
/* sum on X */
				x = x0;
				pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width1; i++) {
					mlib_s32 *sp =
					    ibuff + NCHAN * (x >> MLIB_SHIFT);

					MLIB_FILTER(x, flt_table);
					x += dx;

					pbuff[NCHAN * i] = BC_SUM(NCHAN, 0);
					pbuff[NCHAN * i + 1] = BC_SUM(NCHAN, 1);
					pbuff[NCHAN * i + 2] = BC_SUM(NCHAN, 2);
					pbuff[NCHAN * i + 3] = BC_SUM(NCHAN, 3);
				}
			} else {
				if (alp_ind == -1) {
					ibuff++;
					sbuff++;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < param->sline_size; i++) {
					mlib_s32 pix_alp =
					    ibuff[4 * i + alp_ind];
					TTYPE pix_dalp =
					    pix_alp * (F_ONE / 255);
					sbuff[4 * i] = ibuff[4 * i] * pix_dalp;
					sbuff[4 * i + 1] =
					    ibuff[4 * i + 1] * pix_dalp;
					sbuff[4 * i + 2] =
					    ibuff[4 * i + 2] * pix_dalp;
					sbuff[4 * i + alp_ind] =
					    (TTYPE) pix_alp;
				}
				if (alp_ind == -1) {
					ibuff--;
					sbuff--;
				}

/* sum on X */
				x = x0;
				pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width1; i++) {
					TTYPE *sp =
					    sbuff + NCHAN * (x >> MLIB_SHIFT);

					MLIB_FILTER(x, flt_table);
					x += dx;

					pbuff[NCHAN * i] = BC_SUM(NCHAN, 0);
					pbuff[NCHAN * i + 1] = BC_SUM(NCHAN, 1);
					pbuff[NCHAN * i + 2] = BC_SUM(NCHAN, 2);
					pbuff[NCHAN * i + 3] = BC_SUM(NCHAN, 3);
				}
			}

			sl += src_stride;
		}

/* sum on Y */
		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		if (!alp_ind) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				h0 = buff0[i];
				h1 = buff1[i];
				h2 = buff2[i];
				h3 = buff3[i];
				res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
				STORE(dl[i], ibuff[i], res);
			}
		} else {
			mlib_f32 *buffd = (mlib_f32 *)ibuff;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				SUM_Y(res, i);
				if (res <= MLIB_U8_MIN)
					res = MLIB_U8_MIN;
				if (res >= MLIB_U8_MAX)
					res = MLIB_U8_MAX;
				buffd[i] = res;
			}
			if (alp_ind == -1)
				buffd++;

			if (blend == MLIB_BLEND_GTK_SRC) {
				if (chan_d == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						BLEND0(dl[3 * i], buffd[4 * i]);
						BLEND0(dl[3 * i + 1],
						    buffd[4 * i + 1]);
						BLEND0(dl[3 * i + 2],
						    buffd[4 * i + 2]);
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						TTYPE w, rw;

						w = buffd[4 * i + alp_ind];
						FP_DIV(rw, 255, w);

						BLEND_GTK_SRC(dl[4 * i],
						    buffd[4 * i]);
						BLEND_GTK_SRC(dl[4 * i + 1],
						    buffd[4 * i + 1]);
						BLEND_GTK_SRC(dl[4 * i + 2],
						    buffd[4 * i + 2]);
						dl[4 * i + alp_ind] =
						    (DTYPE) buffd[4 * i +
						    alp_ind];
					}
				}
			} else if (chan_d == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width1; i++) {
					a1 = 1 - dalpha * buffd[4 * i +
					    alp_ind];
					BLEND43(dl[3 * i], buffd[4 * i]);
					BLEND43(dl[3 * i + 1],
					    buffd[4 * i + 1]);
					BLEND43(dl[3 * i + 2],
					    buffd[4 * i + 2]);
				}
			} else {	/* if (chan_d == 4) */

				TTYPE w0, w1, w, rw;

				if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						w0 = a0 * buffd[4 * i +
						    alp_ind];
						w1 = (F_ONE -
						    w0 * (F_ONE / 255)) * dl[4 *
						    i + alp_ind];
						w = w0 + w1;
						FP_INV(rw, w);

						BLEND44(dl[4 * i],
						    buffd[4 * i]);
						BLEND44(dl[4 * i + 1],
						    buffd[4 * i + 1]);
						BLEND44(dl[4 * i + 2],
						    buffd[4 * i + 2]);
						dl[4 * i + alp_ind] = (DTYPE) w;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width1; i++) {
						w0 = a0 * buffd[4 * i +
						    alp_ind];
						w1 = (F_ONE -
						    w0 * (F_ONE / 255)) * dl[4 *
						    i + alp_ind];
						w = w0 + w1;
						FP_INV(rw, w);

						BLEND44z(dl[4 * i],
						    buffd[4 * i]);
						BLEND44z(dl[4 * i + 1],
						    buffd[4 * i + 1]);
						BLEND44z(dl[4 * i + 2],
						    buffd[4 * i + 2]);
						dl[4 * i + alp_ind] = (DTYPE) w;
					}
				}
			}
		}

		y_step = (mlib_s32)(yd + ady) - (mlib_s32)yd;
		yd += ady;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    1s) (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	mlib_s32 ires;
	DTYPE *tsp, *tdp, *tt;

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	sp -= src_stride + 1;
	tsp = sp;
	for (j = 0; j < height; j++) {

		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {

			cx = x >> MLIB_SHIFT;
			tt = tsp + cx;
			MLIB_FILTER(x, flt_table);

			k00 = CALCULATION(0, 1, 2, 3);
			tt += src_stride;
			k10 = CALCULATION(0, 1, 2, 3);
			tt += src_stride;
			k20 = CALCULATION(0, 1, 2, 3);
			tt += src_stride;
			k30 = CALCULATION(0, 1, 2, 3);

			MLIB_LIMITS_BC(0);

			x += dx;
			tdp++;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    2s) (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	mlib_s32 ires;
	DTYPE *tsp, *tdp, *tt, *sp0, *dp0 = dp;
	mlib_s32 ch;

	sp -= src_stride + 2;
	sp0 = sp;

	for (ch = 1; ch <= 2; ch++) {

		tsp = sp;
		y = GetElemSubStruct(current, srcY) & MLIB_MASK;

		for (j = 0; j < height; j++) {

			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {

				cx = (x >> MLIB_SHIFT) * 2;
				MLIB_FILTER(x, flt_table);
				tt = tsp + cx;

				k00 = CALCULATION(0, 2, 4, 6);
				tt += src_stride;
				k10 = CALCULATION(0, 2, 4, 6);
				tt += src_stride;
				k20 = CALCULATION(0, 2, 4, 6);
				tt += src_stride;
				k30 = CALCULATION(0, 2, 4, 6);

				MLIB_LIMITS_BC(0);

				x += dx;
				tdp += 2;
			}

			y += dy;
			dp += dst_stride;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		}

		sp = sp0 + ch;
		dp = dp0 + ch;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    3s) (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	mlib_s32 ires;
	DTYPE *tsp, *tdp, *tt, *sp0, *dp0 = dp;
	mlib_s32 ch;

	sp -= src_stride + 3;
	sp0 = sp;
	for (ch = 1; ch <= 3; ch++) {

		tsp = sp;
		y = GetElemSubStruct(current, srcY) & MLIB_MASK;

		for (j = 0; j < height; j++) {

			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {

				cx = (x >> MLIB_SHIFT) * 3;
				MLIB_FILTER(x, flt_table);
				tt = tsp + cx;

				k00 = CALCULATION(0, 3, 6, 9);
				tt += src_stride;
				k10 = CALCULATION(0, 3, 6, 9);
				tt += src_stride;
				k20 = CALCULATION(0, 3, 6, 9);
				tt += src_stride;
				k30 = CALCULATION(0, 3, 6, 9);

				MLIB_LIMITS_BC(0);

				x += dx;
				tdp += 3;
			}

			y += dy;
			dp += dst_stride;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		}

		sp = sp0 + ch;
		dp = dp0 + ch;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    4s) (
    mlib_work_image *param,
    const FLT_TYPE *flt_table)
{
	VARIABLE(DTYPE);
	VARIABLE_BC(0);
	mlib_s32 ires;
	DTYPE *tsp, *tdp, *tt, *sp0, *dp0 = dp;
	mlib_s32 ch;

	sp -= src_stride + 4;
	sp0 = sp;

	for (ch = 1; ch <= 4; ch++) {

		tsp = sp;
		y = GetElemSubStruct(current, srcY) & MLIB_MASK;

		for (j = 0; j < height; j++) {

			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			MLIB_FILTER(y, flt_table);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {

				cx = (x >> MLIB_SHIFT) * 4;
				MLIB_FILTER(x, flt_table);
				tt = tsp + cx;

				k00 = CALCULATION(0, 4, 8, 12);
				tt += src_stride;
				k10 = CALCULATION(0, 4, 8, 12);
				tt += src_stride;
				k20 = CALCULATION(0, 4, 8, 12);
				tt += src_stride;
				k30 = CALCULATION(0, 4, 8, 12);

				MLIB_LIMITS_BC(0);

				x += dx;
				tdp += 4;
			}

			y += dy;
			dp += dst_stride;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		}

		sp = sp0 + ch;
		dp = dp0 + ch;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* NO_ZOOM_FUNC */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGEZOOM_BC_H */
