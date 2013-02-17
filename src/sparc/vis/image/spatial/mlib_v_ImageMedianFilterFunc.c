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

#pragma ident	"@(#)mlib_v_ImageMedianFilterFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilter*
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMedianFilter.h>

/* *********************************************************** */

#define	BUFF_SIZE	4500

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter_fun(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state * fs)
{
	mlib_d64 dbuff[BUFF_SIZE / 8];
	mlib_median_mask mmask = fs->mmask;
	mlib_type type;
	mlib_image edst[1], esrc[1];
	mlib_s32 nchan, xsize, ysize, m = fs->m, n = fs->n;
	mlib_s32 mn_version = fs->mn_version;
	mlib_s32 i, a_size = 0, b_size = 0, s_size = 0, t_sh, alloc_flag = 0;
	mlib_status res = MLIB_SUCCESS;
	mlib_u8 *buff;
	mlib_s32 num_buff = 0;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);

	fs->type = type;

	if (!(fs->fp_version)) {
		if (type != MLIB_BYTE && type != MLIB_SHORT &&
		    type != MLIB_USHORT && type != MLIB_INT)
			return (MLIB_FAILURE);
	} else {

		if (type != MLIB_FLOAT && type != MLIB_DOUBLE)
			return (MLIB_FAILURE);
	}

	if (nchan == 1)
		fs->cmask = 1;
	t_sh = (type == MLIB_DOUBLE) ? 3 : 2;

/* size for 'psl' and 'x_off' arrays */
	a_size =
	    (ysize + 2 * n) * sizeof (void *) + (xsize +
	    2 * m) * sizeof (mlib_s32);
	a_size = (a_size + 7) & ~7;

	if (mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE) {
		num_buff = n;
	}

	if (mn_version) {
		num_buff = n;
	} else if ((type == MLIB_BYTE || type == MLIB_SHORT ||
	    type == MLIB_USHORT) && (n <= 5)) {
/* VIS */
		if (mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE) {
			num_buff = 2 * n;
		} else if (mmask == MLIB_MEDIAN_MASK_RECT) {

			if (n == 3) {
				num_buff = 11;
			} else if (n == 5 && type != MLIB_BYTE) {
				num_buff = 45;
			}
		} else {
/* PLUS and X masks */
			num_buff = 2 * n - 1;
		}
	}

	if (mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE || mn_version) {
		b_size = ((xsize + m + 1) * nchan) << t_sh;

		if (mn_version && mmask == MLIB_MEDIAN_MASK_X) {
			b_size = 2 * b_size + ((ysize + n + 1) * nchan) << t_sh;
		}
	} else {
		b_size = xsize * nchan * sizeof (mlib_s16);
	}

	b_size = (b_size + 7) & ~7;
	s_size = num_buff * b_size + 3 * num_buff * sizeof (void *);

	if (mn_version) {
		mlib_s32 col_size = (ysize + n) << t_sh;

		if (col_size > s_size)
			s_size = col_size;
	}

	if ((a_size + s_size) > BUFF_SIZE) {
		buff = __mlib_malloc(a_size + s_size);

		if (buff == NULL)
			return (MLIB_FAILURE);
		alloc_flag = 1;
	} else {
		buff = (mlib_u8 *)dbuff;
	}

	fs->pbuff = buff;

	mlib_ImageGetEdges(edst, esrc, dst, src, fs);

	if (num_buff) {
		mlib_u8 *s_buff = buff + a_size;
		void **buff_arr = (void **)(s_buff + num_buff * b_size);

		fs->buff_n_rows = s_buff;
		fs->buff_size = b_size >> t_sh;
		fs->buff_row = buff_arr;

/* cyclic buffers */
		if (mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE || mn_version) {
			fs->buff_ind = n;
			for (i = 0; i < n; i++)
				buff_arr[i] = buff_arr[n + i] =
				    s_buff + i * b_size;
			for (i = 0; i < n; i++)
				buff_arr[2 * n + i] =
				    (s_buff + n * b_size) + i * b_size;
		} else {
			fs->buff_ind = 0;
			for (i = 0; i < num_buff; i++)
				buff_arr[i] = s_buff + i * b_size;
		}
	}

	mlib_ImageMedianFilter_row(fs);

	switch (fs->edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		res = mlib_ImageFilterClearEdge(edst, fs);
		break;
	case MLIB_EDGE_DST_COPY_SRC:
		res = mlib_ImageFilterCopyEdge(edst, esrc, fs);
		break;
	case MLIB_EDGE_SRC_EXTEND:
		res = mlib_ImageMedianFilter_col(fs);
		break;
	}

	if (alloc_flag)
		__mlib_free(buff);

	return (res);
}

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter3x3_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version)
{
	mlib_type type;
	mlib_s32 nchan, amask;
	mlib_filter_state fs;

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	amask = (1 << nchan) - 1;
	cmask &= amask;

	fs.m = 3;
	fs.n = 3;
	fs.mmask = mmask;
	fs.cmask = cmask;
	fs.edge = edge;
	fs.fp_version = fp_version;
	fs.mn_version = 0;

	if ((type == MLIB_BYTE || type == MLIB_SHORT || type == MLIB_USHORT) &&
	    (cmask == amask)) {
/* VIS version */
		fs.row_funs = mlib_v_median_row_3x3_funs;
		fs.init_funs = mlib_v_median_init_3x3_funs;
	} else {
		fs.row_funs = mlib_median_row_3x3_funs;
		fs.init_funs = mlib_median_init_3x3_funs;
	}

	fs.col_funs = mlib_median_col_3x3_funs;

	return (mlib_ImageMedianFilter_fun(dst, src, &fs));
}

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter5x5_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version)
{
	mlib_type type;
	mlib_s32 nchan, amask;
	mlib_filter_state fs;

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	amask = (1 << nchan) - 1;
	cmask &= amask;

	fs.m = 5;
	fs.n = 5;
	fs.mmask = mmask;
	fs.cmask = cmask;
	fs.edge = edge;
	fs.fp_version = fp_version;
	fs.mn_version = 0;

	if ((type == MLIB_BYTE || type == MLIB_SHORT || type == MLIB_USHORT) &&
	    (cmask == amask)) {
/* VIS version */
		fs.row_funs = mlib_v_median_row_5x5_funs;
		fs.init_funs = mlib_v_median_init_5x5_funs;
	} else {
		fs.row_funs = mlib_median_row_5x5_funs;
		fs.init_funs = mlib_median_init_5x5_funs;
	}

	fs.col_funs = mlib_median_col_5x5_funs;

	return (mlib_ImageMedianFilter_fun(dst, src, &fs));
}

/* *********************************************************** */
