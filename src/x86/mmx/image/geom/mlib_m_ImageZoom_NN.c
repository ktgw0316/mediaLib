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

#pragma ident	"@(#)mlib_m_ImageZoom_NN.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image  *dst,
 *                                 mlib_image  *src,
 *                                 mlib_f32    zoomx,
 *                                 mlib_f32    zoomy,
 *                                 mlib_filter filter,
 *                                 mlib_edge   edge)
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

/* *********************************************************** */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageCopy.h>
#include <mlib_mmx_utils.h>

#define	MLIB_COPY_FUNC	mlib_ImageCopy_na

/* *********************************************************** */

#define	CLAMP_U8(X)	((X >= 256) ? (255) : ((X) &~ ((X) >> 31)))

/* *********************************************************** */

static mlib_u32 mlib_bit_mask4[16] = {
	0x00000000u, 0xFF000000u, 0x00FF0000u, 0xFFFF0000u,
	0x0000FF00u, 0xFF00FF00u, 0x00FFFF00u, 0xFFFFFF00u,
	0x000000FFu, 0xFF0000FFu, 0x00FF00FFu, 0xFFFF00FFu,
	0x0000FFFFu, 0xFF00FFFFu, 0x00FFFFFFu, 0xFFFFFFFFu
};

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                                  \
	mlib_s32 j, dx = GetElemStruct(DX), dy = GetElemStruct(DY), x =   \
	GetElemSubStruct(current, srcX), y =                              \
	GetElemSubStruct(current, srcY), src_stride =                     \
	GetElemStruct(src_stride), dst_stride =                           \
	GetElemStruct(dst_stride), width =                                \
	GetElemSubStruct(current, width), height =                        \
	GetElemSubStruct(current, height);                                \
	FORMAT *sp = (FORMAT *) GetElemSubStruct(current, sp), *dp =      \
	    (FORMAT *) GetElemSubStruct(current, dp);

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
	VARIABLE(mlib_u8)
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
		buff = mlib_malloc(sizeof (mlib_s32) * n_al);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

/* save shifts for bit extracting */
	x = srcX + num0 * dx;
#if 0
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
#else
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
#endif

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
#else
			for (i = 0; i < (n_al >> 3); i++) {
				dp[i] = dt[i];
			}
			if (n_al & 7) {
				dp[i] = (dp[i] & ~mask1) | (dt[i] & mask1);
			}
#endif
		} else {

			x = srcX;
			dp = dl + (dstX >> 3);

			if (num0) {
				int res = dp[0] & ~mask0;

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
#else
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
#endif
				dp[i] = res | (res >> 8);
			}

			if (mask1) {
				int res = dp[i] & ~mask1;

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
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

typedef struct
{
	mlib_u32 uint1, uint0;
} two_uint;

#define	DTYPE	two_uint
#define	MASK(dst)	(dst).uint0 = (dst).uint1 = -1

#define	RSHIFT(dst, src, rshift)                                    \
	{                                                           \
	    DTYPE tmp = (src);                                      \
	                                                            \
	    if ((rshift) >= 32) {                                   \
		tmp.uint1 = tmp.uint0 >> ((rshift) - 32);           \
		tmp.uint0 = 0;                                      \
	    } else {                                                \
		tmp.uint1 =                                         \
		    (tmp.uint1 >> (rshift)) | (tmp.uint0 << (32 -   \
		    (rshift)));                                     \
		tmp.uint0 = tmp.uint0 >> (rshift);                  \
	    }                                                       \
	    (dst) = tmp;                                            \
	}

#define	LSHIFT(dst, src, lshift)                                    \
	{                                                           \
	    DTYPE tmp = (src);                                      \
	                                                            \
	    if ((lshift) >= 32) {                                   \
		tmp.uint0 = tmp.uint1 << ((lshift) - 32);           \
		tmp.uint1 = 0;                                      \
	    } else {                                                \
		tmp.uint0 =                                         \
		    (tmp.uint0 << (lshift)) | (tmp.uint1 >> (32 -   \
		    (lshift)));                                     \
		tmp.uint1 = tmp.uint1 << (lshift);                  \
	    }                                                       \
	    (dst) = tmp;                                            \
	}

#define	LOGIC(dst, src1, src2, OPERATION)                         \
	{                                                         \
	    DTYPE tmp;                                            \
	                                                          \
	    ((tmp).uint0 = (src1).uint0 OPERATION(src2).uint0);   \
	    ((tmp).uint1 = (src1).uint1 OPERATION(src2).uint1);   \
	    (dst) = tmp;                                          \
	}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_BitToGray_1_Nearest(
    mlib_work_image * param,
    mlib_s32 s_bitoff,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	VARIABLE(mlib_u8)
	mlib_s32 i;
	DTYPE gray_mask[256], dd, dd_old, *da, dtmp, dtmp1;
	mlib_u32 *pgray = (mlib_u32 *)gray_mask;
	mlib_u8 buff_loc[2 * BUFF_SIZE], *buff = buff_loc;
	mlib_u8 *sl, *dl, gray_val[2];
	mlib_s32 srcX = GetElemSubStruct(current, srcX);
	mlib_u32 gray_val0, gray_val1;
	mlib_s32 width8, res, y_step = -1;
	int k;

	sl = sp - (srcX >> MLIB_SHIFT);
	dl = dp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	srcX += (s_bitoff << MLIB_SHIFT);

	width8 = width / 8;
	if (width > 2 * BUFF_SIZE) {
		buff = mlib_malloc(width * sizeof (mlib_u8));
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

/* save shifts for bit extracting */
	x = srcX;
	for (i = 0; i < width8; i++) {
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

		for (j = 0; j < 16; j++) {
			pgray[2 * (16 * i + j)] = v;
		}

		for (j = 0; j < 16; j++) {
			pgray[2 * (i + 16 * j) + 1] = v;
		}
	}

	for (j = 0; j < height; j++) {

		if (!y_step) {
			MLIB_COPY_FUNC((mlib_u8 *)dl - dst_stride, dl, width);
		} else {
			int off = (mlib_addr)dl & 7;

			da = (DTYPE *) (dl - off);
			x = srcX;

/* not aligned */
			if (off) {
				DTYPE mask;

				MASK(mask);
				off *= 8;
				LSHIFT(dd_old, da[0], 64 - off);

				for (i = 0; i < (width / 8); i++) {
					res =
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i]) & 0x8080);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 1]) & 0x4040);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 2]) & 0x2020);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 3]) & 0x1010);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 4]) & 0x0808);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 5]) & 0x0404);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 6]) & 0x0202);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 7]) & 0x0101);
					x += dx;

					res = (res & 0xff) | (res >> 8);
					dd = gray_mask[res];
					RSHIFT(dd_old, dd_old, 64 - off);
					LSHIFT(dtmp, dd, off);
					LOGIC((*da++), dd_old, dtmp, |);
					dd_old = dd;
				}

				LSHIFT(dtmp, mask, off);
				LOGIC(dtmp, da[0], dtmp, &);
				RSHIFT(dtmp1, dd_old, 64 - off);
				LOGIC(da[0], dtmp, dtmp1, |);

			} else {
/* aligned */

				for (i = 0; i < (width / 8); i++) {
					res =
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i]) & 0x8080);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 1]) & 0x4040);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 2]) & 0x2020);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 3]) & 0x1010);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 4]) & 0x0808);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 5]) & 0x0404);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 6]) & 0x0202);
					x += dx;
					res |=
					    ((sl[x >> (MLIB_SHIFT +
					    3)] << buff[8 * i + 7]) & 0x0101);
					x += dx;

					res = (res & 0xff) | (res >> 8);
					(*da++) = gray_mask[res];
				}
			}

			if (width & 7) {
				dp = dl + (width & ~7);

				for (k = 0; k < (width & 7); k++) {
					dp[k] =
					    gray_val[(sl[x >> (MLIB_SHIFT +
					    3)] >> (7 -
					    (x >> MLIB_SHIFT) & 7)) & 1];
					x += dx;
				}
			}
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dl += dst_stride;
		sl += y_step * src_stride;
	}

	if (buff != buff_loc)
		mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
