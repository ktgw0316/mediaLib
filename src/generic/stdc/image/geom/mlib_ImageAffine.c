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

#pragma ident	"@(#)mlib_ImageAffine.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffine - image affine transformation with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAffine(mlib_image       *dst,
 *                                   const mlib_image *src,
 *                                   const mlib_d64   *mtx,
 *                                   mlib_filter      filter,
 *                                   mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      mtx       Transformation matrix, where
 *                  mtx[0] holds a;  mtx[1] holds b;
 *                  mtx[2] holds tx; mtx[3] holds c;
 *                  mtx[4] holds d;  mtx[5] holds ty.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *                      xd = a*xs + b*ys + tx
 *                      yd = c*xs + d*ys + ty
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *      MLIB_NEAREST
 *      MLIB_BILINEAR
 *      MLIB_BICUBIC
 *      MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *      MLIB_EDGE_DST_NO_WRITE  (default)
 *      MLIB_EDGE_DST_FILL_ZERO
 *      MLIB_EDGE_OP_NEAREST
 *      MLIB_EDGE_SRC_EXTEND
 *      MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffine = __mlib_ImageAffine

#elif defined(__GNUC__)	/* defined( __SUNPRO_C ) */
__typeof__(__mlib_ImageAffine) mlib_ImageAffine
	__attribute__((weak, alias("__mlib_ImageAffine")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	600

/* *********************************************************** */

const type_affine_fun mlib_AffineFunArr_nn[] = {
	mlib_ImageAffine_u8_1ch_nn, mlib_ImageAffine_u8_2ch_nn,
	mlib_ImageAffine_u8_3ch_nn, mlib_ImageAffine_u8_4ch_nn,
	mlib_ImageAffine_s16_1ch_nn, mlib_ImageAffine_s16_2ch_nn,
	mlib_ImageAffine_s16_3ch_nn, mlib_ImageAffine_s16_4ch_nn,
	mlib_ImageAffine_s32_1ch_nn, mlib_ImageAffine_s32_2ch_nn,
	mlib_ImageAffine_s32_3ch_nn, mlib_ImageAffine_s32_4ch_nn,
	mlib_ImageAffine_d64_1ch_nn, mlib_ImageAffine_d64_2ch_nn,
	mlib_ImageAffine_d64_3ch_nn, mlib_ImageAffine_d64_4ch_nn,
};

/* *********************************************************** */

const type_affine_fun mlib_AffineFunArr_bl[] = {
	mlib_ImageAffine_u8_1ch_bl, mlib_ImageAffine_u8_2ch_bl,
	mlib_ImageAffine_u8_3ch_bl, mlib_ImageAffine_u8_4ch_bl,
	mlib_ImageAffine_s16_1ch_bl, mlib_ImageAffine_s16_2ch_bl,
	mlib_ImageAffine_s16_3ch_bl, mlib_ImageAffine_s16_4ch_bl,
	mlib_ImageAffine_s32_1ch_bl, mlib_ImageAffine_s32_2ch_bl,
	mlib_ImageAffine_s32_3ch_bl, mlib_ImageAffine_s32_4ch_bl,
	mlib_ImageAffine_u16_1ch_bl, mlib_ImageAffine_u16_2ch_bl,
	mlib_ImageAffine_u16_3ch_bl, mlib_ImageAffine_u16_4ch_bl,
	mlib_ImageAffine_f32_1ch_bl, mlib_ImageAffine_f32_2ch_bl,
	mlib_ImageAffine_f32_3ch_bl, mlib_ImageAffine_f32_4ch_bl,
	mlib_ImageAffine_d64_1ch_bl, mlib_ImageAffine_d64_2ch_bl,
	mlib_ImageAffine_d64_3ch_bl, mlib_ImageAffine_d64_4ch_bl
};

/* *********************************************************** */

const type_affine_fun mlib_AffineFunArr_bc[] = {
	mlib_ImageAffine_u8_1ch_bc, mlib_ImageAffine_u8_2ch_bc,
	mlib_ImageAffine_u8_3ch_bc, mlib_ImageAffine_u8_4ch_bc,
	mlib_ImageAffine_s16_1ch_bc, mlib_ImageAffine_s16_2ch_bc,
	mlib_ImageAffine_s16_3ch_bc, mlib_ImageAffine_s16_4ch_bc,
	mlib_ImageAffine_s32_1ch_bc, mlib_ImageAffine_s32_2ch_bc,
	mlib_ImageAffine_s32_3ch_bc, mlib_ImageAffine_s32_4ch_bc,
	mlib_ImageAffine_u16_1ch_bc, mlib_ImageAffine_u16_2ch_bc,
	mlib_ImageAffine_u16_3ch_bc, mlib_ImageAffine_u16_4ch_bc,
	mlib_ImageAffine_f32_1ch_bc, mlib_ImageAffine_f32_2ch_bc,
	mlib_ImageAffine_f32_3ch_bc, mlib_ImageAffine_f32_4ch_bc,
	mlib_ImageAffine_d64_1ch_bc, mlib_ImageAffine_d64_2ch_bc,
	mlib_ImageAffine_d64_3ch_bc, mlib_ImageAffine_d64_4ch_bc
};

/* *********************************************************** */

const type_affine_i_fun mlib_AffineFunArr_bc_i[] = {
	mlib_ImageAffineIndex_U8_U8_3CH_BC,
	mlib_ImageAffineIndex_U8_U8_4CH_BC,
	mlib_ImageAffineIndex_S16_U8_3CH_BC,
	mlib_ImageAffineIndex_S16_U8_4CH_BC,
	mlib_ImageAffineIndex_U8_S16_3CH_BC,
	mlib_ImageAffineIndex_U8_S16_4CH_BC,
	mlib_ImageAffineIndex_S16_S16_3CH_BC,
	mlib_ImageAffineIndex_S16_S16_4CH_BC
};

/* *********************************************************** */

#ifdef i386	/* do not perform the copying by mlib_d64 data type for x86 */
#define	MAX_T_IND	2
#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

#define	MAX_T_IND	3
#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

/* *********************************************************** */

mlib_status
mlib_ImageAffine_alltypes(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge,
	const void *colormap)
{
	mlib_affine_param param[1];
	mlib_status res;
	mlib_type type;
	mlib_s32 nchan, t_ind = 0, kw, kw1, align;
	mlib_d64 buff_lcl[BUFF_SIZE / 8];
	mlib_u8 **lineAddr = NULL;
	mlib_s32 affine = 1;

/*
 * check for obvious errors
 */
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	switch (filter) {
		case MLIB_NEAREST:
			kw = 1;
			kw1 = 0;
			break;

		case MLIB_BILINEAR:
			kw = 2;
			kw1 = 0;
			break;

		case MLIB_BICUBIC:
		case MLIB_BICUBIC2:
			kw = 4;
			kw1 = 1;
			break;

		default:
			return (MLIB_FAILURE);
	}

	STORE_PARAM(param, affine);
	STORE_PARAM(param, lineAddr);
	STORE_PARAM(param, filter);

	res = mlib_AffineEdges(param, dst, src, buff_lcl, BUFF_SIZE,
		kw, kw, kw1, kw1, edge, mtx, MLIB_SHIFT, MLIB_SHIFT);

	if (res != MLIB_SUCCESS)
		return (res);

	lineAddr = param->lineAddr;

	if (type == MLIB_BYTE)
		t_ind = 0;
	else if (type == MLIB_SHORT)
		t_ind = 1;
	else if (type == MLIB_INT)
		t_ind = 2;
	else if (type == MLIB_USHORT)
		t_ind = 3;
	else if (type == MLIB_FLOAT)
		t_ind = 4;
	else if (type == MLIB_DOUBLE)
		t_ind = 5;

	if (colormap != NULL && filter != MLIB_NEAREST) {
		if (t_ind != 0 && t_ind != 1)
			return (MLIB_FAILURE);

		if (mlib_ImageGetLutType(colormap) == MLIB_SHORT)
			t_ind += 2;
		t_ind = 2 * t_ind;

		if (mlib_ImageGetLutChannels(colormap) == 4)
			t_ind++;
	}

	if (type == MLIB_BIT) {
		mlib_s32 s_bitoff = mlib_ImageGetBitOffset(src);
		mlib_s32 d_bitoff = mlib_ImageGetBitOffset(dst);

		if (nchan != 1 || filter != MLIB_NEAREST)
			return (MLIB_FAILURE);
		mlib_ImageAffine_bit_1ch_nn(param, s_bitoff, d_bitoff);
	} else {
		switch (filter) {
			case MLIB_NEAREST:

				if (t_ind >= 3)
/* correct types USHORT, FLOAT, DOUBLE; new values: 1, 2, 3 */
					t_ind -= 2;

/*
 * two channels as one channel of next type
 */
				align = (mlib_s32)(param->
					dstData) | (mlib_s32)lineAddr[0];
				align |= param->dstYStride | param->srcYStride;
				while (((nchan | (align >> t_ind)) & 1) == 0 &&
					t_ind < MAX_T_IND) {
					nchan >>= 1;
					t_ind++;
				}

				res = mlib_AffineFunArr_nn[4 * t_ind + (nchan -
					1)] (param);
				break;

			case MLIB_BILINEAR:

				if (colormap != NULL) {
					res = mlib_AffineFunArr_bl_i[t_ind]
						(param, colormap);
				} else {
					res = mlib_AffineFunArr_bl[4 * t_ind +
						(nchan - 1)] (param);
				}

				break;

			case MLIB_BICUBIC:
			case MLIB_BICUBIC2:

				if (colormap != NULL) {
					res = mlib_AffineFunArr_bc_i[t_ind]
						(param, colormap);
				} else {
					res = mlib_AffineFunArr_bc[4 * t_ind +
						(nchan - 1)] (param);
				}

				break;
		}

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (res);
		}
	}

	if (edge == MLIB_EDGE_SRC_PADDED)
		edge = MLIB_EDGE_DST_NO_WRITE;

	if (filter != MLIB_NEAREST && edge != MLIB_EDGE_DST_NO_WRITE) {
		mlib_affine_param param_e[1];
		mlib_d64 buff_lcl1[BUFF_SIZE / 8];

		STORE_PARAM(param_e, lineAddr);
		STORE_PARAM(param_e, filter);

		res = mlib_AffineEdges(param_e, dst, src, buff_lcl1, BUFF_SIZE,
			kw, kw, kw1, kw1, -1, mtx, MLIB_SHIFT, MLIB_SHIFT);

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (res);
		}

		switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageAffineEdgeZero(param, param_e,
					colormap);
				break;

			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageAffineEdgeNearest(param, param_e);
				break;

			case MLIB_EDGE_SRC_EXTEND:

				if (filter == MLIB_BILINEAR) {
					res = mlib_ImageAffineEdgeExtend_BL
						(param, param_e, colormap);
				} else {
					res = mlib_ImageAffineEdgeExtend_BC
						(param, param_e, colormap);
				}

				break;
		default:
				res = MLIB_SUCCESS;
				break;
		}

		if (param_e->buff_malloc != NULL)
			__mlib_free(param_e->buff_malloc);
	}

	if (param->buff_malloc != NULL)
		__mlib_free(param->buff_malloc);

	return (res);
}

/* *********************************************************** */

mlib_status
__mlib_ImageAffine(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if (type != MLIB_BIT && type != MLIB_BYTE &&
		type != MLIB_SHORT && type != MLIB_USHORT && type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return (mlib_ImageAffine_alltypes(dst, src, mtx, filter, edge, NULL));
}

/* *********************************************************** */
