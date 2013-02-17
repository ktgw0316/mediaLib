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

#pragma ident	"@(#)mlib_ImageZoom_NN.c	9.2	07/11/05"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <mlib_image.h>
#include <mlib_ImageZoom.h>

#define	MLIB_COPY_FUNC	mlib_ImageCopy_na

/* *********************************************************** */

#ifdef _NO_LONGLONG	/* do not perform the copying by 64-bits data type */

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64	two_int

#else /* _NO_LONGLONG (do not perform the copying by 64-bits data type) */

#define	TYPE_64	mlib_s64

#endif /* _NO_LONGLONG (do not perform the copying by 64-bits data type) */

/* *********************************************************** */

typedef union
{
	TYPE_64 d64;
	struct
	{
		mlib_s32 f0, f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

#define	CLAMP_U8(X)	((X >= 256) ? (255) : ((X) &~ ((X) >> 31)))

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

static const mlib_u32 mlib_bit_mask4[16] = {
	0x00000000u, 0xFF000000u, 0x00FF0000u, 0xFFFF0000u,
	0x0000FF00u, 0xFF00FF00u, 0x00FFFF00u, 0xFFFFFF00u,
	0x000000FFu, 0xFF0000FFu, 0x00FF00FFu, 0xFFFF00FFu,
	0x0000FFFFu, 0xFF00FFFFu, 0x00FFFFFFu, 0xFFFFFFFFu
};

#else /* _LITTLE_ENDIAN */

static const mlib_u32 mlib_bit_mask4[16] = {
	0x00000000u, 0x000000FFu, 0x0000FF00u, 0x0000FFFFu,
	0x00FF0000u, 0x00FF00FFu, 0x00FFFF00u, 0x00FFFFFFu,
	0xFF000000u, 0xFF0000FFu, 0xFF00FF00u, 0xFF00FFFFu,
	0xFFFF0000u, 0xFFFF00FFu, 0xFFFFFF00u, 0xFFFFFFFFu
};

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	FORMAT *sp = (FORMAT *)GetElemSubStruct(current, sp),        \
		*dp = (FORMAT *)GetElemSubStruct(current, dp)

/* *********************************************************** */

#define	BUFF_SIZE	256
#define	BYTE_POS_MASK	((1 << (MLIB_SHIFT + 3)) - 1)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_BIT_1_Nearest(
    mlib_work_image * param,
    mlib_s32 s_bitoff,
    mlib_s32 d_bitoff)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;
	mlib_s32 srcX = GetElemSubStruct(current, srcX);
	mlib_s32 dstX = GetElemSubStruct(current, dstX);
	mlib_u8 *sl = sp - (srcX >> MLIB_SHIFT), *dl = dp - dstX, *dt;
	mlib_s32 bit, res, k, y_step = -1;
	mlib_s32 num0, n_al, mask0, mask1;

	srcX += (s_bitoff << MLIB_SHIFT);
	dstX += d_bitoff;

	num0 = 8 - (dstX & 7);

	if (num0 > width)
		num0 = width;
	num0 &= 7;
	mask0 = ((0xFF00 >> num0) & 0xFF) >> (dstX & 7);
	n_al = width - num0;
	mask1 = ((0xFF00 >> (n_al & 7)) & 0xFF);

	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	if (n_al > BUFF_SIZE) {
		buff = __mlib_malloc(sizeof (mlib_s32) * n_al);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

/* save shifts for bit extracting */
	x = srcX + num0 * dx;
#if 0
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n_al >> 3); i++) {
		buff[8 * i] = (((x >> MLIB_SHIFT)) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 1] =
		    (((x >> MLIB_SHIFT) - 1) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 2] =
		    (((x >> MLIB_SHIFT) - 2) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 3] =
		    (((x >> MLIB_SHIFT) - 3) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 4] =
		    (((x >> MLIB_SHIFT) - 4) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 5] =
		    (((x >> MLIB_SHIFT) - 5) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 6] =
		    (((x >> MLIB_SHIFT) - 6) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
		buff[8 * i + 7] =
		    (((x >> MLIB_SHIFT) - 7) & 7) | (x & ~BYTE_POS_MASK);
		x += dx;
	}

	x_last = x;
#else /* 0 */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n_al >> 3); i++) {
		buff[8 * i] = (((x >> MLIB_SHIFT)) & 7);
		x += dx;
		buff[8 * i + 1] = (((x >> MLIB_SHIFT) - 1) & 7);
		x += dx;
		buff[8 * i + 2] = (((x >> MLIB_SHIFT) - 2) & 7);
		x += dx;
		buff[8 * i + 3] = (((x >> MLIB_SHIFT) - 3) & 7);
		x += dx;
		buff[8 * i + 4] = (((x >> MLIB_SHIFT) - 4) & 7);
		x += dx;
		buff[8 * i + 5] = (((x >> MLIB_SHIFT) - 5) & 7);
		x += dx;
		buff[8 * i + 6] = (((x >> MLIB_SHIFT) - 6) & 7);
		x += dx;
		buff[8 * i + 7] = (((x >> MLIB_SHIFT) - 7) & 7);
		x += dx;
	}

#endif /* 0 */

	for (j = 0; j < height; j++) {

		if (!y_step) {
			dp = dl + (dstX >> 3);
			dt = dp - dst_stride;

			if (num0) {
				dp[0] = (dp[0] & ~mask0) | ((*dt++) & mask0);
				dp++;
			}
#if 0
			MLIB_COPY_FUNC(dt, dp, n_al >> 3);

			if (n_al & 7) {
				dp[n_al >> 3] =
				    (dp[n_al >> 3] & ~mask1) | (dt[n_al >> 3] &
				    mask1);
			}
#else /* 0 */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n_al >> 3); i++) {
				dp[i] = dt[i];
			}

			if (n_al & 7) {
				dp[i] = (dp[i] & ~mask1) | (dt[i] & mask1);
			}
#endif /* 0 */
		} else {

			x = srcX;
			dp = dl + (dstX >> 3);

			if (num0) {
				mlib_s32 res = dp[0] & ~mask0;

				for (k = dstX; k < (dstX + num0); k++) {
					bit = 7 - (k & 7);
					res |=
					    (((sl[x >> (MLIB_SHIFT + 3)] >> (7 -
					    (x >> MLIB_SHIFT) & 7)) & 1) <<
					    bit);
					x += dx;
				}

				(*dp++) = res;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n_al >> 3); i++) {
#if 0
				res =
				    ((sl[buff[8 * i] >> (MLIB_SHIFT +
				    3)] << buff[8 * i]) & 0x8080);
				res |=
				    ((sl[buff[8 * i + 1] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 1]) & 0x4040);
				res |=
				    ((sl[buff[8 * i + 2] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 2]) & 0x2020);
				res |=
				    ((sl[buff[8 * i + 3] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 3]) & 0x1010);
				res |=
				    ((sl[buff[8 * i + 4] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 4]) & 0x0808);
				res |=
				    ((sl[buff[8 * i + 5] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 5]) & 0x0404);
				res |=
				    ((sl[buff[8 * i + 6] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 6]) & 0x0202);
				res |=
				    ((sl[buff[8 * i + 7] >> (MLIB_SHIFT +
				    3)] << buff[8 * i + 7]) & 0x0101);
#else /* 0 */
				res =
				    ((sl[x >> (MLIB_SHIFT +
				    3)] << buff[8 * i]) & 0x8080);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    1]) & 0x4040);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    2]) & 0x2020);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    3]) & 0x1010);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    4]) & 0x0808);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    5]) & 0x0404);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    6]) & 0x0202);
				x += dx;
				res |=
				    ((sl[x >> (MLIB_SHIFT + 3)] << buff[8 * i +
				    7]) & 0x0101);
				x += dx;
#endif /* 0 */
				dp[i] = res | (res >> 8);
			}

			if (mask1) {
				mlib_s32 res = dp[i] & ~mask1;

				for (k = 0; k < (n_al & 7); k++) {
					bit = 7 - (k & 7);
					res |=
					    (((sl[x >> (MLIB_SHIFT + 3)] >> (7 -
					    (x >> MLIB_SHIFT) & 7)) & 1) <<
					    bit);
					x += dx;
				}

				dp[i] = res;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dl = (void *)((mlib_u8 *)dl + dst_stride);
		sl = (void *)((mlib_u8 *)sl + y_step * src_stride);
	}

	if (buff != buff_loc)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_SHIFT3	(MLIB_SHIFT + 3)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_BitToGray_1_Nearest(
    mlib_work_image * param,
    mlib_s32 s_bitoff,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	TYPE_64 gray_mask[256], *da;
	mlib_u32 *pgray = (mlib_u32 *)gray_mask;
	mlib_u8 buff_loc[2 * BUFF_SIZE], *buff = buff_loc;
	mlib_u8 *sl, *dl, gray_val[2];
	mlib_s32 srcX = GetElemSubStruct(current, srcX);
	mlib_u32 gray_val0, gray_val1;
	mlib_s32 width8, res, y_step = -1;
	mlib_s32 off;

	sl = sp - (srcX >> MLIB_SHIFT);
	dl = dp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	srcX += (s_bitoff << MLIB_SHIFT);

	width8 = (width + 7) / 8;

	if (width8 * 8 > 2 * BUFF_SIZE) {
		buff = __mlib_malloc(width8 * 8 * sizeof (mlib_u8));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

/* save shifts for bit extracting */
	x = srcX;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width8; i++) {
		buff[8 * i    ] = (((x >> MLIB_SHIFT)) & 7);
		x += dx;
		buff[8 * i + 1] = (((x >> MLIB_SHIFT) - 1) & 7);
		x += dx;
		buff[8 * i + 2] = (((x >> MLIB_SHIFT) - 2) & 7);
		x += dx;
		buff[8 * i + 3] = (((x >> MLIB_SHIFT) - 3) & 7);
		x += dx;
		buff[8 * i + 4] = (((x >> MLIB_SHIFT) - 4) & 7);
		x += dx;
		buff[8 * i + 5] = (((x >> MLIB_SHIFT) - 5) & 7);
		x += dx;
		buff[8 * i + 6] = (((x >> MLIB_SHIFT) - 6) & 7);
		x += dx;
		buff[8 * i + 7] = (((x >> MLIB_SHIFT) - 7) & 7);
		x += dx;
	}

/* calculate lookup table */
	gray_val0 = CLAMP_U8(glow[0]);
	gray_val1 = CLAMP_U8(ghigh[0]);
	gray_val[0] = gray_val0;
	gray_val[1] = gray_val1;
	gray_val0 |= (gray_val0 << 8);
	gray_val0 |= (gray_val0 << 16);
	gray_val1 |= (gray_val1 << 8);
	gray_val1 |= (gray_val1 << 16);

	for (i = 0; i < 16; i++) {
		mlib_u32 v, mask = mlib_bit_mask4[i];

		v = (gray_val0 & ~mask) | (gray_val1 & mask);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < 16; j++) {
			pgray[2 * (16 * i + j)] = v;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < 16; j++) {
			pgray[2 * (i + 16 * j) + 1] = v;
		}
	}

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dl - dst_stride, dl, width);
		} else {
			dp = dl;
			x = srcX;

			for (i = 0; ((mlib_addr)dp & 7) && (i < width); i++) {
				*dp++ =
				    gray_val[(sl[x >> MLIB_SHIFT3] >>
				    (7 - (x >> MLIB_SHIFT) & 7)) & 1];
				x += dx;
			}

			da = (void*)dp;
			off = i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (width - 8); i += 8) {
				res  = (sl[x >> MLIB_SHIFT3] << buff[i    ]) &
					(0x808080 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 1]) &
					(0x404040 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 2]) &
					(0x202020 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 3]) &
					(0x101010 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 4]) &
					(0x080808 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 5]) &
					(0x040404 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 6]) &
					(0x020202 >> off);
				x += dx;
				res |= (sl[x >> MLIB_SHIFT3] << buff[i + 7]) &
					(0x010101 >> off);
				x += dx;

				res <<= off;

				res = (res | (res >> 8) | (res >> 16)) & 0xff;
				*da++ = gray_mask[res];
			}
			dp = (void*)da;

			for (; i < width; i++) {
				*dp++ =
				    gray_val[(sl[x >> MLIB_SHIFT3] >>
				    (7 - (x >> MLIB_SHIFT) & 7)) & 1];
				x += dx;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dl += dst_stride;
		sl += y_step * src_stride;
	}

	if (buff != buff_loc)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_2_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_u8 *tdp, *tsp, tmp0, tmp1;
	mlib_s32 cx, y_step = -1;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, dp,
			    2 * width);
		} else {
			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			cx = (x >> (MLIB_SHIFT - 1)) & ~1;
			tmp0 = tsp[cx];
			tmp1 = tsp[cx + 1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width - 1; i++, tdp += 2) {
				tdp[0] = tmp0;
				tdp[1] = tmp1;
				x += dx;
				cx = (x >> (MLIB_SHIFT - 1)) & ~1;
				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
			}

			tdp[0] = tmp0;
			tdp[1] = tmp1;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_4_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_u8 *tdp, *tsp, tmp0, tmp1, tmp2, tmp3;
	mlib_u16 utmp0, utmp1;
	mlib_s32 cx, y_step = -1;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, dp,
			    4 * width);
		} else {
			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

/* sp & dp pointers not aligned */
			if (((mlib_addr)tdp | (mlib_addr)tsp) & 1) {

				cx = (x >> (MLIB_SHIFT - 2)) & ~3;
				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];
				tmp3 = tsp[cx + 3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++) {
					tdp[0] = tmp0;
					tdp[1] = tmp1;
					tdp[2] = tmp2;
					tdp[3] = tmp3;

					x += dx;
					cx = (x >> (MLIB_SHIFT - 2)) & ~3;

					tmp0 = tsp[cx];
					tmp1 = tsp[cx + 1];
					tmp2 = tsp[cx + 2];
					tmp3 = tsp[cx + 3];

					tdp += 4;
				}

				tdp[0] = tmp0;
				tdp[1] = tmp1;
				tdp[2] = tmp2;
				tdp[3] = tmp3;
			} else {

				cx = (x >> (MLIB_SHIFT - 2)) & ~3;
				utmp0 = *(mlib_u16 *)(tsp + cx);
				utmp1 = *(mlib_u16 *)(tsp + cx + 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++) {
					*(mlib_u16 *)tdp = utmp0;
					*(mlib_u16 *)(tdp + 2) = utmp1;

					x += dx;
					cx = (x >> (MLIB_SHIFT - 2)) & ~3;

					utmp0 = *(mlib_u16 *)(tsp + cx);
					utmp1 = *(mlib_u16 *)(tsp + cx + 2);

					tdp += 4;
				}

				*(mlib_u16 *)tdp = utmp0;
				*(mlib_u16 *)(tdp + 2) = utmp1;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_2_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16);
	mlib_s32 i;
	mlib_u8 *tsp, *tdp;
	mlib_u16 tmp0, tmp1;
	mlib_s32 cx, y_step = -1;
	mlib_u32 utmp;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, (void *)dp,
			    4 * width);
		} else {
			tdp = (mlib_u8 *)dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

/* sp & dp pointers not aligned */
			if (((mlib_addr)tdp | (mlib_addr)tsp) & 3) {

				cx = (x >> (MLIB_SHIFT - 2)) & ~3;
				tmp0 = *(mlib_u16 *)(tsp + cx);
				tmp1 = *(mlib_u16 *)(tsp + cx + 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++, tdp += 4) {

					*(mlib_u16 *)tdp = tmp0;
					*(mlib_u16 *)(tdp + 2) = tmp1;

					x += dx;
					cx = (x >> (MLIB_SHIFT - 2)) & ~3;

					tmp0 = *(mlib_u16 *)(tsp + cx);
					tmp1 = *(mlib_u16 *)(tsp + cx + 2);
				}

				*(mlib_u16 *)tdp = tmp0;
				*(mlib_u16 *)(tdp + 2) = tmp1;
			} else {

				cx = (x >> (MLIB_SHIFT - 2)) & ~3;
				utmp = *(mlib_u32 *)(tsp + cx);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++, tdp += 4) {

					*(mlib_u32 *)tdp = utmp;
					x += dx;
					cx = (x >> (MLIB_SHIFT - 2)) & ~3;

					utmp = *(mlib_u32 *)(tsp + cx);
				}

				*(mlib_u32 *)tdp = utmp;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_4_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16);
	mlib_s32 i;
	mlib_u8 *tsp, *tdp;
	mlib_s32 cx, y_step = -1;
	mlib_u16 tmp0, tmp1, tmp2, tmp3;
	TYPE_64 dtmp;
	mlib_s32 ftmp0, ftmp1;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, (void *)dp,
			    8 * width);
		} else {
			tdp = (mlib_u8 *)dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if (((mlib_addr)tdp | (mlib_addr)tsp) & 7) {
				if (((mlib_addr)tdp | (mlib_addr)tsp) & 3) {

					cx = (x >> (MLIB_SHIFT - 3)) & ~7;
					tmp0 = *(mlib_u16 *)(tsp + cx);
					tmp1 = *(mlib_u16 *)(tsp + cx + 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width - 1; i++) {

						tmp2 =
						    *(mlib_u16 *)(tsp + cx + 4);
						tmp3 =
						    *(mlib_u16 *)(tsp + cx + 6);

						*(mlib_u16 *)tdp = tmp0;
						*(mlib_u16 *)(tdp + 2) = tmp1;
						*(mlib_u16 *)(tdp + 4) = tmp2;
						*(mlib_u16 *)(tdp + 6) = tmp3;

						x += dx;
						cx = (x >> (MLIB_SHIFT -
						    3)) & ~7;

						tmp0 = *(mlib_u16 *)(tsp + cx);
						tmp1 =
						    *(mlib_u16 *)(tsp + cx + 2);

						tdp += 8;
					}

					tmp2 = *(mlib_u16 *)(tsp + cx + 4);
					tmp3 = *(mlib_u16 *)(tsp + cx + 6);

					*(mlib_u16 *)tdp = tmp0;
					*(mlib_u16 *)(tdp + 2) = tmp1;
					*(mlib_u16 *)(tdp + 4) = tmp2;
					*(mlib_u16 *)(tdp + 6) = tmp3;
				} else {
/* align to word */

					cx = (x >> (MLIB_SHIFT - 3)) & ~7;
					ftmp0 = *(mlib_s32 *)(tsp + cx);
					ftmp1 = *(mlib_s32 *)(tsp + cx + 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width - 1; i++) {

						*(mlib_s32 *)tdp = ftmp0;
						*(mlib_s32 *)(tdp + 4) = ftmp1;

						x += dx;
						cx = (x >> (MLIB_SHIFT -
						    3)) & ~7;

						ftmp0 = *(mlib_s32 *)(tsp + cx);
						ftmp1 =
						    *(mlib_s32 *)(tsp + cx + 4);

						tdp += 8;
					}

					*(mlib_s32 *)tdp = ftmp0;
					*(mlib_s32 *)(tdp + 4) = ftmp1;
				}
			} else {
/* align to mlib_s64 word */

				cx = (x >> (MLIB_SHIFT - 3)) & ~7;
				dtmp = *(TYPE_64 *) (tsp + cx);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++) {

					*(TYPE_64 *) tdp = dtmp;

					x += dx;
					cx = (x >> (MLIB_SHIFT - 3)) & ~7;

					dtmp = *(TYPE_64 *) (tsp + cx);

					tdp += 8;
				}

				*(TYPE_64 *) tdp = dtmp;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32);
	mlib_s32 *dl = dp, *tsp;
	mlib_s32 y_step = -1;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dl - dst_stride, (void *)dl,
			    4 * width);
		} else {
			mlib_s32 *dp = dl, *dend = dl + width;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if ((mlib_addr)dp & 7) {
				(*dp++) = tsp[x >> MLIB_SHIFT];
				x += dx;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= dend - 2; dp += 2) {
				d64_2_f32 dd;

				dd.f32s.f0 =
				    *(mlib_s32 *)((mlib_u8 *)tsp +
				    ((x >> (MLIB_SHIFT - 2)) & ~3));
				x += dx;
				dd.f32s.f1 =
				    *(mlib_s32 *)((mlib_u8 *)tsp +
				    ((x >> (MLIB_SHIFT - 2)) & ~3));
				x += dx;
				*(TYPE_64 *) dp = dd.d64;
			}

			if (dp < dend) {
				(*dp++) = tsp[x >> MLIB_SHIFT];
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dl = (void *)((mlib_u8 *)dl + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_2_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32);
	mlib_s32 i;
	mlib_u8 *tsp;
	mlib_s32 cx, y_step = -1, tmp0, tmp1, tmp2, tmp3, x_max;
	TYPE_64 dtmp0;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	x_max = (param->sline_size) << MLIB_SHIFT;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, (void *)dp,
			    8 * width);
		} else {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if (!(((mlib_addr)dp | (mlib_addr)tsp) & 7)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					cx = (x >> (MLIB_SHIFT - 3)) & ~7;
					x += dx;
					dtmp0 = *(TYPE_64 *) (tsp + cx);
					((TYPE_64 *) dp)[i] = dtmp0;
				}
			} else {

				cx = (x >> (MLIB_SHIFT - 3)) & ~7;
				x += dx;
				tmp0 = *(mlib_s32 *)(tsp + cx);
				tmp1 = *(mlib_s32 *)(tsp + cx + 4);
				cx = ((x >> (MLIB_SHIFT - 3)) & ~7) & ((x -
				    x_max) >> 31);
				x += dx;
				tmp2 = *(mlib_s32 *)(tsp + cx);
				tmp3 = *(mlib_s32 *)(tsp + cx + 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= width - 2; i += 2) {
					dp[2 * i] = tmp0;
					dp[2 * i + 1] = tmp1;
					dp[2 * i + 2] = tmp2;
					dp[2 * i + 3] = tmp3;

					cx = ((x >> (MLIB_SHIFT -
					    3)) & ~7) & ((x - x_max) >> 31);
					x += dx;
					tmp0 = *(mlib_s32 *)(tsp + cx);
					tmp1 = *(mlib_s32 *)(tsp + cx + 4);
					cx = ((x >> (MLIB_SHIFT -
					    3)) & ~7) & ((x - x_max) >> 31);
					x += dx;
					tmp2 = *(mlib_s32 *)(tsp + cx);
					tmp3 = *(mlib_s32 *)(tsp + cx + 4);
				}

				if (width & 1) {
					dp[2 * i] = tmp0;
					dp[2 * i + 1] = tmp1;
				}
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32);
	mlib_s32 i;
	mlib_u8 *tsp;
	mlib_s32 cx, y_step = -1, tmp0, tmp1, tmp2;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, (void *)dp,
			    12 * width);
		} else {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			cx = (x >> MLIB_SHIFT) * 12;
			x += dx;
			tmp0 = *(mlib_s32 *)(tsp + cx);
			tmp1 = *(mlib_s32 *)(tsp + cx + 4);
			tmp2 = *(mlib_s32 *)(tsp + cx + 8);

			cx = (x >> MLIB_SHIFT) * 12;
			x += dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width - 1; i++) {
				dp[3 * i + 0] = tmp0;
				dp[3 * i + 1] = tmp1;
				dp[3 * i + 2] = tmp2;

				tmp0 = *(mlib_s32 *)(tsp + cx);
				tmp1 = *(mlib_s32 *)(tsp + cx + 4);
				tmp2 = *(mlib_s32 *)(tsp + cx + 8);

				cx = (x >> MLIB_SHIFT) * 12;
				x += dx;
			}

			dp[3 * i + 0] = tmp0;
			dp[3 * i + 1] = tmp1;
			dp[3 * i + 2] = tmp2;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_4_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32);
	mlib_s32 i;
	mlib_u8 *tsp;
	mlib_s32 cx, y_step = -1, tmp0, tmp1, tmp2, tmp3;
	TYPE_64 dtmp0, dtmp1;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dp - dst_stride, (void *)dp,
			    16 * width);
		} else {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if (((mlib_addr)dp | (mlib_addr)tsp) & 7) {
				cx = (x >> (MLIB_SHIFT - 4)) & ~15;
				x += dx;

				tmp0 = *(mlib_s32 *)(tsp + cx);
				tmp1 = *(mlib_s32 *)(tsp + cx + 4);
				tmp2 = *(mlib_s32 *)(tsp + cx + 8);
				tmp3 = *(mlib_s32 *)(tsp + cx + 12);

				cx = (x >> (MLIB_SHIFT - 4)) & ~15;
				x += dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++) {
					dp[4 * i + 0] = tmp0;
					dp[4 * i + 1] = tmp1;
					dp[4 * i + 2] = tmp2;
					dp[4 * i + 3] = tmp3;

					tmp0 = *(mlib_s32 *)(tsp + cx);
					tmp1 = *(mlib_s32 *)(tsp + cx + 4);
					tmp2 = *(mlib_s32 *)(tsp + cx + 8);
					tmp3 = *(mlib_s32 *)(tsp + cx + 12);

					cx = (x >> (MLIB_SHIFT - 4)) & ~15;
					x += dx;
				}

				dp[4 * i + 0] = tmp0;
				dp[4 * i + 1] = tmp1;
				dp[4 * i + 2] = tmp2;
				dp[4 * i + 3] = tmp3;
			} else {

				cx = (x >> (MLIB_SHIFT - 4)) & ~15;
				x += dx;

				dtmp0 = *(TYPE_64 *) (tsp + cx);
				dtmp1 = *(TYPE_64 *) (tsp + cx + 8);

				cx = (x >> (MLIB_SHIFT - 4)) & ~15;
				x += dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width - 1; i++) {
					*(TYPE_64 *) (dp + 4 * i) = dtmp0;
					*(TYPE_64 *) (dp + 4 * i + 2) = dtmp1;

					dtmp0 = *(TYPE_64 *) (tsp + cx);
					dtmp1 = *(TYPE_64 *) (tsp + cx + 8);

					cx = (x >> (MLIB_SHIFT - 4)) & ~15;
					x += dx;
				}

				*(TYPE_64 *) (dp + 4 * i) = dtmp0;
				*(TYPE_64 *) (dp + 4 * i + 2) = dtmp1;
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(TYPE_64);
	mlib_s32 i;
	TYPE_64 *tsp, tmp;
	mlib_s32 cx, y_step;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = x >> MLIB_SHIFT;
			tmp = tsp[cx];
			dp[i] = tmp;
			x += dx;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_2_Nearest(
    mlib_work_image * param)
{
	VARIABLE(TYPE_64);
	mlib_s32 i;
	TYPE_64 *tsp, tmp, tmp1;
	mlib_s32 cx, y_step;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> (MLIB_SHIFT - 1)) & ~1;
			tmp = tsp[cx];
			tmp1 = tsp[cx + 1];
			dp[2 * i] = tmp;
			dp[2 * i + 1] = tmp1;
			x += dx;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(TYPE_64);
	mlib_s32 i;
	TYPE_64 *tsp, tmp, tmp1, tmp2;
	mlib_s32 cx, y_step;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = (x >> MLIB_SHIFT) * 3;
		tmp = tsp[cx];
		tmp1 = tsp[cx + 1];
		tmp2 = tsp[cx + 2];
		x += dx;

		cx = (x >> MLIB_SHIFT) * 3;
		x += dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width - 1; i++) {
			dp[3 * i] = tmp;
			dp[3 * i + 1] = tmp1;
			dp[3 * i + 2] = tmp2;
			tmp = tsp[cx];
			tmp1 = tsp[cx + 1];
			tmp2 = tsp[cx + 2];
			cx = (x >> MLIB_SHIFT) * 3;
			x += dx;
		}

		dp[3 * i] = tmp;
		dp[3 * i + 1] = tmp1;
		dp[3 * i + 2] = tmp2;

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_4_Nearest(
    mlib_work_image * param)
{
	VARIABLE(TYPE_64);
	mlib_s32 i;
	TYPE_64 *tsp, tmp, tmp1, tmp2, tmp3;
	mlib_s32 cx, y_step;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = (x >> (MLIB_SHIFT - 2)) & ~3;
		tmp = tsp[cx];
		tmp1 = tsp[cx + 1];
		tmp2 = tsp[cx + 2];
		tmp3 = tsp[cx + 3];
		x += dx;

		cx = (x >> (MLIB_SHIFT - 2)) & ~3;
		x += dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width - 1; i++) {
			dp[4 * i] = tmp;
			dp[4 * i + 1] = tmp1;
			dp[4 * i + 2] = tmp2;
			dp[4 * i + 3] = tmp3;
			tmp = tsp[cx];
			tmp1 = tsp[cx + 1];
			tmp2 = tsp[cx + 2];
			tmp3 = tsp[cx + 3];
			cx = (x >> (MLIB_SHIFT - 2)) & ~3;
			x += dx;
		}

		dp[4 * i] = tmp;
		dp[4 * i + 1] = tmp1;
		dp[4 * i + 2] = tmp2;
		dp[4 * i + 3] = tmp3;

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (void *)((mlib_u8 *)tsp + y_step * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
