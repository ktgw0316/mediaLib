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

#ifndef _MLIB_I_IMAGEZOOM_BL_H
#define	_MLIB_I_IMAGEZOOM_BL_H

#pragma ident	"@(#)mlib_i_ImageZoom_BL.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_NO_LONGLONG

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	DTYPE	mlib_u8
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) & MLIB_MASK))
#define	RINT(x)	(mlib_s32)((((x) << 1) + 1) >> 1)
#define	VARIABLE_EXT(FORMAT, NCHAN)	mlib_s32 ssize = 0; FORMAT *buffs
#define	COPY_LINE(buffs, sp, ssize)	buffs = sp
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_U8_##CC

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	DTYPE	mlib_s16
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) & MLIB_MASK))
#define	RINT(x)	(mlib_s32)(x)
#define	VARIABLE_EXT(FORMAT, NCHAN)                             \
	mlib_s32 ssize = NCHAN * (param->sline_size);           \
	mlib_s32 *buffs
#define	COPY_LINE(buffs, sp, ssize)                             \
	buffs[0] = sp[0];                                       \
	i = ((mlib_s32)sp >> 1) & 1;                            \
	for (; i < ssize; i += 2) {                             \
	    mlib_s32 x = *(mlib_s32 *)(sp + i);                 \
	                                                        \
	    buffs[i + IND0] = x >> 16;                          \
	    buffs[i + IND1] = (x << 16) >> 16;                  \
	}
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_S16_##CC

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	DTYPE	mlib_u16
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) & MLIB_MASK))
#define	RINT(x)	(mlib_s32)(x)
#define	VARIABLE_EXT(FORMAT, NCHAN)                             \
	mlib_s32 ssize = NCHAN * (param->sline_size);           \
	mlib_s32 *buffs
#define	COPY_LINE(buffs, sp, ssize)                             \
	buffs[0] = sp[0];                                       \
	i = ((mlib_s32)sp >> 1) & 1;                            \
	for (; i < ssize; i += 2) {                             \
	    mlib_u32 x = *(mlib_u32 *)(sp + i);                 \
	                                                        \
	    buffs[i + IND0] = x >> 16;                          \
	    buffs[i + IND1] = x & 0xFFFF;                       \
	}
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_U16_##CC

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	IND0	1
#define	IND1	0
#else		   /* _LITTLE_ENDIAN */
#define	IND0	0
#define	IND1	1
#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	BUFF_SIZE	300

/* *********************************************************** */

    typedef union
{
	mlib_s64 d64;
#if IMG_TYPE == 3  /* IMG_TYPE == 3 (MLIB_USHORT) */
	struct
	{
		mlib_u32 f0;
		mlib_u32 f1;
	} f32s;
#else /* IMG_TYPE == 3 (MLIB_USHORT) */
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} f32s;
#endif /* IMG_TYPE == 3 (MLIB_USHORT) */
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                          \
	FORMAT *sp = GetElemSubStruct(current, sp),               \
		*dp = GetElemSubStruct(current, dp);              \
	mlib_s32 i, j, x,                                         \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,  \
		dx = GetElemStruct(DX),                           \
		dy = GetElemStruct(DY),                           \
		src_stride = GetElemStruct(src_stride),           \
		dst_stride = GetElemStruct(dst_stride),           \
		width  = GetElemSubStruct(current, width),        \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#define	VARIABLE_BL(N)                                          \
	mlib_s64 a0##N, a1##N, a2##N, a3##N;                    \
	FP_TYPE r0##N, r1##N;                                   \
	RTYPE res##N

/* *********************************************************** */

#define	FP_TYPE	mlib_s64

/* *********************************************************** */

#define	RTYPE	mlib_s64

/* *********************************************************** */

#define	LOAD(N, index)                                          \
	a0##N = TABLE(tsp[cx + N]);                             \
	a3##N = TABLE(tsp[cx + src_stride + N]);                \
	a1##N = TABLE(tsp[cx + index]);                         \
	a2##N = TABLE(tsp[cx + src_stride + index])

/* *********************************************************** */

#define	CALC(N)                                                 \
	r0##N = (a0##N << 16) + fdy * (a3##N - a0##N);          \
	r1##N = (a1##N << 16) + fdy * (a2##N - a1##N)

/* *********************************************************** */

#define	RES(N)	res##N = (RTYPE)(((r0##N << 16) + fdx * (r1##N - r0##N)) >> 32)

/* *********************************************************** */

#define	STOR(N, index)	dp[index] = res##N

/* *********************************************************** */

#ifndef NO_ZOOM_FUNC

#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param)
{
	mlib_s64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 width2, size, y_step = 2;

	width2 = (width + 1) / 2;
	size = 3 * width2 + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width2;
	buff1 = buff0 + width2;
	buffs = (void *)(buff1 + width2);

	for (j = 0; j < height; j++) {
		mlib_s64 fdy = FTABLE(y);
		mlib_s64 fry = (1 << 16) - fdy;

		if (dy > (1 << MLIB_SHIFT)) {
			mlib_s64 x0, x1, x2, x3, y0, y1, fdx;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;
			mlib_s32 *ibuff = (mlib_s32 *)buff;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = (x0 << 16) + fdx * (x1 - x0);
				y1 = (x2 << 16) + fdx * (x3 - x2);

				ibuff[i] = RINT((fry * y0 + fdy * y1) >> 32);
				dp[i] = ibuff[i];

				x += dx;
			}

		} else if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 2; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

			if (width & 1) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;

				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
			}

		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_s64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 2; i++) {
				mlib_s32 cxh, cxl;
				mlib_s64 xh2, xh3, fxh;
				mlib_s64 xl2, xl3, fxl;
				d64_2x32 y0, y1, rr;

				cxh = x >> MLIB_SHIFT;
				cxl = (x + dx) >> MLIB_SHIFT;

				fxh = FTABLE(x);
				fxl = FTABLE(x + dx);

				xh2 = (mlib_s64)TABLE(buffs[cxh]);
				xh3 = (mlib_s64)TABLE(buffs[cxh + NCHAN]);
				xl2 = (mlib_s64)TABLE(buffs[cxl]);
				xl3 = (mlib_s64)TABLE(buffs[cxl + NCHAN]);

				y1.f32s.f0 = (xh2 << 16) + fxh * (xh3 - xh2);
				y1.f32s.f1 = (xl2 << 16) + fxl * (xl3 - xl2);

				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];

				x += 2 * dx;
			}

			if (width & 1) {
				mlib_s32 cxh;
				mlib_s64 xh2, xh3, fx;
				d64_2x32 y0, y1;

				cxh = x >> MLIB_SHIFT;
				fx = FTABLE(x);
				xh2 = (mlib_s64)TABLE(buffs[cxh]);
				xh3 = (mlib_s64)TABLE(buffs[cxh + NCHAN]);

				y1.f32s.f0 = (xh2 << 16) + fx * (xh3 - xh2);
				y1.f32s.f1 = 0;
				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				mlib_s64 tmp0, tmp1;
				tmp0 = (mlib_s64)y0.f32s.f0;
				tmp1 = (mlib_s64)y1.f32s.f0;
				dp[2 * i] =
				    RINT(((tmp0 << 16) + fdy *
					(tmp1 - tmp0)) >> 32);
			}

		} else {
			mlib_s64 x0, x1, x2, x3, y0, y1, fdx;
			mlib_s32 *fbuff0 = (mlib_s32 *)buff0;
			mlib_s32 *fbuff1 = (mlib_s32 *)buff1;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = (x0 << 16) + fdx * (x1 - x0);
				y1 = (x2 << 16) + fdx * (x3 - x2);
				fbuff0[i] = y0;
				fbuff1[i] = y1;

				dp[i] = RINT(((y0 << 16) + fdy * (y1 - y0))
					>> 32);

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 1)

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image *param)
{
	mlib_s64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 size, y_step = 2;

	size = 3 * width + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width;
	buff1 = buff0 + width;
	buffs = (void *)(buff1 + width);

	for (j = 0; j < height; j++) {
		mlib_s64 fdy = FTABLE(y);
		mlib_s64 fry = (1 << 16) - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_s64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_s32 cx;
				mlib_s64 a2, a3, b2, b3, fdx, frx;
				d64_2x32 y0, y1, rr;

				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a2 = (mlib_s64)TABLE(buffs[cx]);
				a3 = (mlib_s64)TABLE(buffs[cx + NCHAN]);
				b2 = (mlib_s64)TABLE(buffs[cx + 1]);
				b3 = (mlib_s64)TABLE(buffs[cx + NCHAN + 1]);

				frx = (1 << 16) - fdx;
				y1.f32s.f0 = (frx * a2 + fdx * a3);
				y1.f32s.f1 = (frx * b2 + fdx * b3);

				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;
				dp[NCHAN * i] = ((mlib_s32 *)(buff + i))[0];
				dp[NCHAN * i + 1] = ((mlib_s32 *)(buff + i))[1];

				x += dx;
			}

		} else {
			mlib_s64 a0, a1, a2, a3, ay0, ay1, fdx;
			mlib_s64 b0, b1, b2, b3, by0, by1;
			mlib_s32 *fbuff0 = (mlib_s32 *)buff0;
			mlib_s32 *fbuff1 = (mlib_s32 *)buff1;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a0 = TABLE(tsp[cx]);
				a1 = TABLE(tsp[cx + NCHAN]);
				a2 = TABLE(tsp[cx + src_stride]);
				a3 = TABLE(tsp[cx + src_stride + NCHAN]);

				b0 = TABLE(tsp[cx + 1]);
				b1 = TABLE(tsp[cx + NCHAN + 1]);
				b2 = TABLE(tsp[cx + src_stride + 1]);
				b3 = TABLE(tsp[cx + src_stride + NCHAN + 1]);

				ay0 = (a0 << 16) + fdx * (a1 - a0);
				ay1 = (a2 << 16) + fdx * (a3 - a2);
				fbuff0[2 * i] = ay0;
				fbuff1[2 * i] = ay1;

				by0 = (b0 << 16) + fdx * (b1 - b0);
				by1 = (b2 << 16) + fdx * (b3 - b2);
				fbuff0[2 * i + 1] = by0;
				fbuff1[2 * i + 1] = by1;

				dp[NCHAN * i] = RINT(((ay0 << 16) +
					fdy * (ay1 - ay0)) >> 32);
				dp[NCHAN * i + 1] = RINT(((by0 << 16) +
					fdy * (by1 - by0)) >> 32);

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	(((x) << 1) + (x))

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param)
{
	mlib_s64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 width2, size, y_step = 2, l;

	if (!height)
		return (MLIB_SUCCESS);

	width2 = (width + 1) / 2;
	size = 7 * width2 + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width2;
	buff1 = buff0 + 3 * width2;
	buffs = (void *)(buff1 + 3 * width2);

	for (j = 0; j < 3 * width2; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height; j++) {
		mlib_s64 fdy = FTABLE(y);
		mlib_s64 fry = (1 << 16) - fdy;

		if (y_step == 0) {
			mlib_s32 k;

			for (k = 0; k < 3; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width / 2; i++) {
					d64_2x32 y0, y1, rr;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					rr.i32s.f0 =
					    RINT((fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0) >> 32);
					rr.i32s.f1 =
					    RINT((fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1) >> 32);

					buff[i] = rr.d64;
					dp[6 * i] = ((mlib_s32 *)(buff + i))[0];
					dp[6 * i + 3] =
					    ((mlib_s32 *)(buff + i))[1];
				}

				if (width & 1) {
					d64_2x32 y0, y1, rr;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					rr.i32s.f0 =
					    RINT((fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0) >> 32);
					rr.i32s.f1 =
					    RINT((fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1) >> 32);

					buff[i] = rr.d64;

					dp[6 * i] = ((mlib_s32 *)(buff + i))[0];
				}

				sp += 1;
				dp += 1;
				buff0 += width2;
				buff1 += width2;
			}

			sp -= 3;
			dp -= 3;
			buff0 -= 3 * width2;
			buff1 -= 3 * width2;

		} else
			for (l = 0; l < y_step; l++) {
				mlib_s32 k;
				mlib_s64 *buffT;

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
						mlib_s64 xh2, xh3, fxh;
						mlib_s64 xl2, xl3, fxl;
						d64_2x32 y0, y1, rr;

						cxh =
						    MUL_NCHAN(x >> MLIB_SHIFT);
						cxl =
						    MUL_NCHAN((x +
						    dx) >> MLIB_SHIFT);

						fxh = FTABLE(x);
						fxl = FTABLE(x + dx);

						xh2 =
						    (mlib_s64)TABLE(buffs[cxh]);
						xh3 =
						    (mlib_s64)TABLE(buffs[cxh +
						    NCHAN]);
						xl2 =
						    (mlib_s64)TABLE(buffs[cxl]);
						xl3 =
						    (mlib_s64)TABLE(buffs[cxl +
						    NCHAN]);

						y1.f32s.f0 = ((xh2 << 16) +
							fxh * (xh3 - xh2));
						y1.f32s.f1 = ((xl2 << 16) +
							fxl * (xl3 - xl2));

						buff1[i] = y1.d64;
						y0.d64 = buff0[i];

						rr.i32s.f0 =
						    RINT((fry * y0.f32s.f0 +
						    fdy * y1.f32s.f0) >> 32);
						rr.i32s.f1 =
						    RINT((fry * y0.f32s.f1 +
						    fdy * y1.f32s.f1) >> 32);

						buff[i] = rr.d64;
						dp[6 * i] =
						    ((mlib_s32 *)(buff + i))[0];
						dp[6 * i + 3] =
						    ((mlib_s32 *)(buff + i))[1];

						x += 2 * dx;
					}

					if (width & 1) {
						mlib_s32 cxh;
						mlib_s64 xh2, xh3, fx;
						d64_2x32 y0, y1;

						cxh =
						    MUL_NCHAN(x >> MLIB_SHIFT);
						fx = FTABLE(x);
						xh2 =
						    (mlib_s64)TABLE(buffs[cxh]);
						xh3 =
						    (mlib_s64)TABLE(buffs[cxh +
						    NCHAN]);

						y1.f32s.f0 = (xh2 << 16) +
							fx * (xh3 - xh2);
						y1.f32s.f1 = 0;
						buff1[i] = y1.d64;
						y0.d64 = buff0[i];

						mlib_s64 tmp0, tmp1;
						tmp0 = (mlib_s64)y0.f32s.f0;
						tmp1 = (mlib_s64)y1.f32s.f0;
						dp[6 * i] =
						    RINT(((tmp0 << 16) + fdy *
							(tmp1 - tmp0)) >> 32);
					}

					dp += 1;
					buffs += 1;
					buff0 += width2;
					buff1 += width2;
				}

				dp -= 3;
				buffs -= 3;
				buff0 -= 3 * width2;
				buff1 -= 3 * width2;
				sp += src_stride;
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

#undef  NCHAN
#define	NCHAN	4

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 2)

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param)
{
	mlib_s64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 size, y_step = -1;

	if (!height)
		return (MLIB_SUCCESS);

	size = 6 * width + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + 2 * width;
	buff1 = buff0 + 2 * width;
	buffs = (void *)(buff1 + 2 * width);

	for (j = 0; j < 2 * width; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height + 1; j++) {
		mlib_s64 fdy = FTABLE(y);
		mlib_s64 fry = (1 << 16) - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 2 * width; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT((fry * y0.f32s.f0 + fdy * y1.f32s.f0)
					>> 32);
				rr.i32s.f1 =
				    RINT((fry * y0.f32s.f1 + fdy * y1.f32s.f1)
					>> 32);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

		} else {
			mlib_s32 k;
			mlib_s64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, sp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (k = 0; k < 2; k++) {
				x = GetElemSubStruct(current, srcX) & MLIB_MASK;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					mlib_s64 a2, a3, b2, b3, fdx, frx;
					d64_2x32 y0, y1, rr;

					cx = MUL_NCHAN(x >> MLIB_SHIFT);
					fdx = FTABLE(x);

					a2 = (mlib_s64)TABLE(buffs[cx]);
					a3 = (mlib_s64)TABLE(buffs[cx + NCHAN]);
					b2 = (mlib_s64)TABLE(buffs[cx + 1]);
					b3 = (mlib_s64)TABLE(buffs[cx + NCHAN +
					    1]);

					frx = (1 << 16) - fdx;
					y1.f32s.f0 = frx * a2 + fdx * a3;
					y1.f32s.f1 = frx * b2 + fdx * b3;

					buff1[2 * i] = y1.d64;
					y0.d64 = buff0[2 * i];

					rr.i32s.f0 =
					    RINT((fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0) >> 32);
					rr.i32s.f1 =
					    RINT((fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1) >> 32);

					buff[i] = rr.d64;
					dp[NCHAN * i] =
					    ((mlib_s32 *)(buff + i))[0];
					dp[NCHAN * i + 1] =
					    ((mlib_s32 *)(buff + i))[1];

					x += dx;
				}

				dp += 2;
				buffs += 2;
				buff0 += 1;
				buff1 += 1;
			}

			dp -= 4;
			buffs -= 4;
			buff0 -= 2;
			buff1 -= 2;
			sp += src_stride;
		}

		if (y_step >= 0) {
			dp += dst_stride;
			y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
			y += dy;
		} else {
			y_step = 1;
		}
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    1s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = x >> MLIB_SHIFT;
			fdx = FTABLE(x);

			LOAD(0, 1);
			CALC(0);
			RES(0);
			STOR(0, i);

			x += dx;
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
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 2;
			fdx = FTABLE(x);

			LOAD(0, 2);
			LOAD(1, 3);
			CALC(0);
			CALC(1);
			RES(0);
			RES(1);
			STOR(0, 2 * i);
			STOR(1, 2 * i + 1);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    3s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

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
			RES(0);
			RES(1);
			RES(2);

			STOR(0, 3 * i);
			STOR(1, 3 * i + 1);
			STOR(2, 3 * i + 2);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    4s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(3);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE(x);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(3, 7);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* NO_ZOOM_FUNC */

#ifdef __cplusplus
}
#endif

#else /* _NO_LONGLONG */

#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageBlendTable.h>

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	DTYPE	mlib_u8
#define	TABLE(A)	mlib_U82F32[(A)]
#define	FTABLE(A)	mlib_c_blend_Q8[((A) >> 8) & 0xFF]
#define	RINT(x)	(mlib_s32)((x) + 0.5f)
#define	VARIABLE_EXT(FORMAT, NCHAN)	mlib_s32 ssize = 0; FORMAT *buffs
#define	COPY_LINE(buffs, sp, ssize)	buffs = sp
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_U8_##CC

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	DTYPE	mlib_s16
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) & MLIB_MASK) * (1.f / (1 << 16)))
#define	RINT(x)	(mlib_s32)(x)
#define	VARIABLE_EXT(FORMAT, NCHAN)                             \
	mlib_s32 ssize = NCHAN * (param->sline_size);           \
	mlib_s32 *buffs
#define	COPY_LINE(buffs, sp, ssize)                             \
	buffs[0] = sp[0];                                       \
	i = ((mlib_s32)sp >> 1) & 1;                            \
	for (; i < ssize; i += 2) {                             \
	    mlib_s32 x = *(mlib_s32 *)(sp + i);                 \
	                                                        \
	    buffs[i + IND0] = x >> 16;                          \
	    buffs[i + IND1] = (x << 16) >> 16;                  \
	}
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_S16_##CC

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	DTYPE	mlib_u16
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) & MLIB_MASK) * (1.f / (1 << 16)))
#define	RINT(x)	(mlib_s32)(x)
#define	VARIABLE_EXT(FORMAT, NCHAN)                             \
	mlib_s32 ssize = NCHAN * (param->sline_size);           \
	mlib_s32 *buffs
#define	COPY_LINE(buffs, sp, ssize)                             \
	buffs[0] = sp[0];                                       \
	i = ((mlib_s32)sp >> 1) & 1;                            \
	for (; i < ssize; i += 2) {                             \
	    mlib_u32 x = *(mlib_u32 *)(sp + i);                 \
	                                                        \
	    buffs[i + IND0] = x >> 16;                          \
	    buffs[i + IND1] = x & 0xFFFF;                       \
	}
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_U16_##CC

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	IND0	1
#define	IND1	0
#else		   /* _LITTLE_ENDIAN */
#define	IND0	0
#define	IND1	1
#endif		   /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	BUFF_SIZE	300

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

#define	VARIABLE(FORMAT)                                          \
	FORMAT *sp = GetElemSubStruct(current, sp),               \
		*dp = GetElemSubStruct(current, dp);              \
	mlib_s32 i, j, x,                                         \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,  \
		dx = GetElemStruct(DX),                           \
		dy = GetElemStruct(DY),                           \
		src_stride = GetElemStruct(src_stride),           \
		dst_stride = GetElemStruct(dst_stride),           \
		width  = GetElemSubStruct(current, width),        \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#if IMG_TYPE == 1	/* MLIB_BYTE */

#define	VARIABLE_BL(N)                                          \
	FP_TYPE a0##N, a1##N, a2##N, a3##N;                     \
	FP_TYPE r0##N, r1##N;                                   \
	RTYPE res##N

#else /* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	VARIABLE_BL(N)                                          \
	mlib_s32 a0##N, a1##N, a2##N, a3##N;                    \
	FP_TYPE r0##N, r1##N;                                   \
	RTYPE res##N

#endif /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#define	FP_TYPE	mlib_f32

/* *********************************************************** */

#define	RTYPE	mlib_s32

/* *********************************************************** */

#define	LOAD(N, index)                                          \
	a0##N = TABLE(tsp[cx + N]);                             \
	a3##N = TABLE(tsp[cx + src_stride + N]);                \
	a1##N = TABLE(tsp[cx + index]);                         \
	a2##N = TABLE(tsp[cx + src_stride + index])

/* *********************************************************** */

#define	CALC(N)                                                 \
	r0##N = a0##N + fdy * (a3##N - a0##N);                  \
	r1##N = a1##N + fdy * (a2##N - a1##N)

/* *********************************************************** */

#define	RES(N)	res##N = (RTYPE)(r0##N + fdx * (r1##N - r0##N))

/* *********************************************************** */

#define	STOR(N, index)	dp[index] = res##N

/* *********************************************************** */

#ifndef NO_ZOOM_FUNC

#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param)
{
	mlib_d64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 width2, size, y_step = 2;

	width2 = (width + 1) / 2;
	size = 3 * width2 + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_d64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width2;
	buff1 = buff0 + width2;
	buffs = (void *)(buff1 + width2);

	for (j = 0; j < height; j++) {
		mlib_f32 fdy = FTABLE(y);
		mlib_f32 fry = 1.f - fdy;

		if (dy > (1 << MLIB_SHIFT)) {
			mlib_f32 x0, x1, x2, x3, y0, y1, fdx;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;
			mlib_s32 *ibuff = (mlib_s32 *)buff;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = x0 + fdx * (x1 - x0);
				y1 = x2 + fdx * (x3 - x2);

				ibuff[i] = RINT(fry * y0 + fdy * y1);
				dp[i] = ibuff[i];

				x += dx;
			}

		} else if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 2; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

			if (width & 1) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;

				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
			}

		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_d64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width / 2; i++) {
				mlib_s32 cxh, cxl;
				mlib_f32 xh2, xh3, fxh;
				mlib_f32 xl2, xl3, fxl;
				d64_2x32 y0, y1, rr;

				cxh = x >> MLIB_SHIFT;
				cxl = (x + dx) >> MLIB_SHIFT;

				fxh = FTABLE(x);
				fxl = FTABLE(x + dx);

				xh2 = (mlib_f32)TABLE(buffs[cxh]);
				xh3 = (mlib_f32)TABLE(buffs[cxh + NCHAN]);
				xl2 = (mlib_f32)TABLE(buffs[cxl]);
				xl3 = (mlib_f32)TABLE(buffs[cxl + NCHAN]);

				y1.f32s.f0 = xh2 + fxh * (xh3 - xh2);
				y1.f32s.f1 = xl2 + fxl * (xl3 - xl2);

				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];

				x += 2 * dx;
			}

			if (width & 1) {
				mlib_s32 cxh;
				mlib_f32 xh2, xh3, fx;
				d64_2x32 y0, y1;

				cxh = x >> MLIB_SHIFT;
				fx = FTABLE(x);
				xh2 = (mlib_f32)TABLE(buffs[cxh]);
				xh3 = (mlib_f32)TABLE(buffs[cxh + NCHAN]);

				y1.f32s.f0 = xh2 + fx * (xh3 - xh2);
				y1.f32s.f1 = 0.f;
				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				dp[2 * i] =
				    RINT(y0.f32s.f0 + fdy * (y1.f32s.f0 -
				    y0.f32s.f0));
			}

		} else {
			mlib_f32 x0, x1, x2, x3, y0, y1, fdx;
			mlib_f32 *fbuff0 = (mlib_f32 *)buff0;
			mlib_f32 *fbuff1 = (mlib_f32 *)buff1;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = x0 + fdx * (x1 - x0);
				y1 = x2 + fdx * (x3 - x2);
				fbuff0[i] = y0;
				fbuff1[i] = y1;

				dp[i] = RINT(y0 + fdy * (y1 - y0));

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 1)

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image *param)
{
	mlib_d64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 size, y_step = 2;

	size = 3 * width + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_d64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width;
	buff1 = buff0 + width;
	buffs = (void *)(buff1 + width);

	for (j = 0; j < height; j++) {
		mlib_f32 fdy = FTABLE(y);
		mlib_f32 fry = 1.f - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_d64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_s32 cx;
				mlib_f32 a2, a3, b2, b3, fdx, frx;
				d64_2x32 y0, y1, rr;

				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a2 = (mlib_f32)TABLE(buffs[cx]);
				a3 = (mlib_f32)TABLE(buffs[cx + NCHAN]);
				b2 = (mlib_f32)TABLE(buffs[cx + 1]);
				b3 = (mlib_f32)TABLE(buffs[cx + NCHAN + 1]);

				frx = 1.f - fdx;
				y1.f32s.f0 = frx * a2 + fdx * a3;
				y1.f32s.f1 = frx * b2 + fdx * b3;

				buff1[i] = y1.d64;
				y0.d64 = buff0[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;
				dp[NCHAN * i] = ((mlib_s32 *)(buff + i))[0];
				dp[NCHAN * i + 1] = ((mlib_s32 *)(buff + i))[1];

				x += dx;
			}

		} else {
			mlib_f32 a0, a1, a2, a3, ay0, ay1, fdx;
			mlib_f32 b0, b1, b2, b3, by0, by1;
			mlib_f32 *fbuff0 = (mlib_f32 *)buff0;
			mlib_f32 *fbuff1 = (mlib_f32 *)buff1;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a0 = TABLE(tsp[cx]);
				a1 = TABLE(tsp[cx + NCHAN]);
				a2 = TABLE(tsp[cx + src_stride]);
				a3 = TABLE(tsp[cx + src_stride + NCHAN]);

				b0 = TABLE(tsp[cx + 1]);
				b1 = TABLE(tsp[cx + NCHAN + 1]);
				b2 = TABLE(tsp[cx + src_stride + 1]);
				b3 = TABLE(tsp[cx + src_stride + NCHAN + 1]);

				ay0 = a0 + fdx * (a1 - a0);
				ay1 = a2 + fdx * (a3 - a2);
				fbuff0[2 * i] = ay0;
				fbuff1[2 * i] = ay1;

				by0 = b0 + fdx * (b1 - b0);
				by1 = b2 + fdx * (b3 - b2);
				fbuff0[2 * i + 1] = by0;
				fbuff1[2 * i + 1] = by1;

				dp[NCHAN * i] = RINT(ay0 + fdy * (ay1 - ay0));
				dp[NCHAN * i + 1] =
				    RINT(by0 + fdy * (by1 - by0));

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	(((x) << 1) + (x))

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param)
{
	mlib_d64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 width2, size, y_step = 2, l;

	if (!height)
		return (MLIB_SUCCESS);

	width2 = (width + 1) / 2;
	size = 7 * width2 + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_d64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + width2;
	buff1 = buff0 + 3 * width2;
	buffs = (void *)(buff1 + 3 * width2);

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
					d64_2x32 y0, y1, rr;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					rr.i32s.f0 =
					    RINT(fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0);
					rr.i32s.f1 =
					    RINT(fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1);

					buff[i] = rr.d64;
					dp[6 * i] = ((mlib_s32 *)(buff + i))[0];
					dp[6 * i + 3] =
					    ((mlib_s32 *)(buff + i))[1];
				}

				if (width & 1) {
					d64_2x32 y0, y1, rr;

					y0.d64 = buff0[i];
					y1.d64 = buff1[i];

					rr.i32s.f0 =
					    RINT(fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0);
					rr.i32s.f1 =
					    RINT(fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1);

					buff[i] = rr.d64;

					dp[6 * i] = ((mlib_s32 *)(buff + i))[0];
				}

				sp += 1;
				dp += 1;
				buff0 += width2;
				buff1 += width2;
			}

			sp -= 3;
			dp -= 3;
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
						d64_2x32 y0, y1, rr;

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

						rr.i32s.f0 =
						    RINT(fry * y0.f32s.f0 +
						    fdy * y1.f32s.f0);
						rr.i32s.f1 =
						    RINT(fry * y0.f32s.f1 +
						    fdy * y1.f32s.f1);

						buff[i] = rr.d64;
						dp[6 * i] =
						    ((mlib_s32 *)(buff + i))[0];
						dp[6 * i + 3] =
						    ((mlib_s32 *)(buff + i))[1];

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

						dp[6 * i] =
						    RINT(y0.f32s.f0 +
						    fdy * (y1.f32s.f0 -
						    y0.f32s.f0));
					}

					dp += 1;
					buffs += 1;
					buff0 += width2;
					buff1 += width2;
				}

				dp -= 3;
				buffs -= 3;
				buff0 -= 3 * width2;
				buff1 -= 3 * width2;
				sp += src_stride;
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

#undef  NCHAN
#define	NCHAN	4

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 2)

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param)
{
	mlib_d64 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 size, y_step = -1;

	if (!height)
		return (MLIB_SUCCESS);

	size = 6 * width + ssize / 2 + 1;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_d64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff + 2 * width;
	buff1 = buff0 + 2 * width;
	buffs = (void *)(buff1 + 2 * width);

	for (j = 0; j < 2 * width; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height + 1; j++) {
		mlib_f32 fdy = FTABLE(y);
		mlib_f32 fry = 1.f - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 2 * width; i++) {
				d64_2x32 y0, y1, rr;

				y0.d64 = buff0[i];
				y1.d64 = buff1[i];

				rr.i32s.f0 =
				    RINT(fry * y0.f32s.f0 + fdy * y1.f32s.f0);
				rr.i32s.f1 =
				    RINT(fry * y0.f32s.f1 + fdy * y1.f32s.f1);

				buff[i] = rr.d64;
				dp[2 * i] = ((mlib_s32 *)(buff + i))[0];
				dp[2 * i + 1] = ((mlib_s32 *)(buff + i))[1];
			}

		} else {
			mlib_s32 k;
			mlib_d64 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, sp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (k = 0; k < 2; k++) {
				x = GetElemSubStruct(current, srcX) & MLIB_MASK;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					mlib_f32 a2, a3, b2, b3, fdx, frx;
					d64_2x32 y0, y1, rr;

					cx = MUL_NCHAN(x >> MLIB_SHIFT);
					fdx = FTABLE(x);

					a2 = (mlib_f32)TABLE(buffs[cx]);
					a3 = (mlib_f32)TABLE(buffs[cx + NCHAN]);
					b2 = (mlib_f32)TABLE(buffs[cx + 1]);
					b3 = (mlib_f32)TABLE(buffs[cx + NCHAN +
					    1]);

					frx = 1.f - fdx;
					y1.f32s.f0 = frx * a2 + fdx * a3;
					y1.f32s.f1 = frx * b2 + fdx * b3;

					buff1[2 * i] = y1.d64;
					y0.d64 = buff0[2 * i];

					rr.i32s.f0 =
					    RINT(fry * y0.f32s.f0 +
					    fdy * y1.f32s.f0);
					rr.i32s.f1 =
					    RINT(fry * y0.f32s.f1 +
					    fdy * y1.f32s.f1);

					buff[i] = rr.d64;
					dp[NCHAN * i] =
					    ((mlib_s32 *)(buff + i))[0];
					dp[NCHAN * i + 1] =
					    ((mlib_s32 *)(buff + i))[1];

					x += dx;
				}

				dp += 2;
				buffs += 2;
				buff0 += 1;
				buff1 += 1;
			}

			dp -= 4;
			buffs -= 4;
			buff0 -= 2;
			buff1 -= 2;
			sp += src_stride;
		}

		if (y_step >= 0) {
			dp += dst_stride;
			y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
			y += dy;
		} else {
			y_step = 1;
		}
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    1s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = x >> MLIB_SHIFT;
			fdx = FTABLE(x);

			LOAD(0, 1);
			CALC(0);
			RES(0);
			STOR(0, i);

			x += dx;
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
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 2;
			fdx = FTABLE(x);

			LOAD(0, 2);
			LOAD(1, 3);
			CALC(0);
			CALC(1);
			RES(0);
			RES(1);
			STOR(0, 2 * i);
			STOR(1, 2 * i + 1);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    3s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

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
			RES(0);
			RES(1);
			RES(2);

			STOR(0, 3 * i);
			STOR(1, 3 * i + 1);
			STOR(2, 3 * i + 2);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    4s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(3);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE(x);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(3, 7);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* NO_ZOOM_FUNC */

#endif /* _NO_LONGLONG */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGEZOOM_BL_H */
