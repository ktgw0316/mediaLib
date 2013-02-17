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

#pragma ident	"@(#)mlib_c_VideoWaveletTwoTenTrans_f.c	9.2	07/10/09 SMI"

#include <mlib_video.h>
#include <mlib_VideoWaveletTwoTenTrans_f.h>

/* *********************************************************** */

/* Forward transform */

/* *********************************************************** */

#define	TT_1D_FORWARD(FRM_IN, FRM_OUT)                                    \
	{                                                                 \
	    mlib_s32 i, j, p, p1, p2;                                     \
	    FRM_OUT *d, *s, *buf, *buf_line = l_buffer;                   \
	    FRM_IN *x;                                                    \
	                                                                  \
	    for (j = 0; j < (xx_height); j++) {                           \
		x = (FRM_IN *) (xx_ptr) + j * (xx_width);                 \
		s = (FRM_OUT *) (ss_ptr) + j;                             \
		buf = buf_line + 2;                                       \
		for (i = 0; i < ((xx_width) >> 1); i++) {                 \
		    (*buf++) = *s = (x[0] + x[1]) >> 1;                   \
		    x += 2;                                               \
		    s += (ss_width);                                      \
		}                                                         \
		if ((xx_width) & 1)                                       \
		    (*buf++) = *s = *x;                                   \
		buf_line[1] = buf_line[2];                                \
		x = (FRM_IN *) (xx_ptr) + j * (xx_width);                 \
		d = (FRM_OUT *) (dd_ptr) + j;                             \
		buf[0] = buf[-1];                                         \
		buf[1] = buf[-2];                                         \
		buf = buf_line + 2;                                       \
		buf[-1] = buf[0];                                         \
		buf[-2] = buf[1];                                         \
		for (i = 0; i < ((xx_width) >> 1); i++) {                 \
		    p1 = buf[-2] - buf[2];                                \
		    p2 = buf[1] - buf[-1];                                \
		    p = (3 * p1 + 16 * p2 + 4 * p2 + 2 * p2 + 32) >> 6;   \
		    *d = x[0] - x[1] + p;                                 \
		    d += (dd_width);                                      \
		    x += 2;                                               \
		    buf++;                                                \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_U8(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer)
{
	TT_1D_FORWARD(mlib_u8,
		mlib_s16)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S16_S16(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer)
{
	TT_1D_FORWARD(mlib_s16,
		mlib_s16)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S16(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer)
{
	TT_1D_FORWARD(mlib_s16,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_ForwardTransform_S32_S32(
	void *ss_ptr,
	mlib_s32 ss_width,
	void *dd_ptr,
	mlib_s32 dd_width,
	void *xx_ptr,
	mlib_s32 xx_width,
	mlib_s32 xx_height,
	void *l_buffer)
{
	TT_1D_FORWARD(mlib_s32,
		mlib_s32)
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S16_U8(
	mlib_s16 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer;
	mlib_s16 *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += width * height + 5;

	s_buffer = (void *)__mlib_malloc(sizeof (mlib_s16) * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	l_buffer = s_buffer + height * width;
	d_buffer = s_buffer + height * ss_x;
	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S16_U8(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer);

	__mlib_free(s_buffer);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += width * height + 5;

	s_buffer = (void *)__mlib_malloc(sizeof (mlib_s16) * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	l_buffer = s_buffer + height * width;
	d_buffer = s_buffer + height * ss_x;
	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S16_S16(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S16_S16(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer);

	__mlib_free(s_buffer);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S32_S16(
	mlib_s32 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += width * height + 5;

	s_buffer = (void *)__mlib_malloc(sizeof (mlib_s32) * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	l_buffer = s_buffer + height * width;
	d_buffer = s_buffer + height * ss_x;
	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S32_S16(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer);

	__mlib_free(s_buffer);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_FTTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += width * height + 5;

	s_buffer = (void *)__mlib_malloc(sizeof (mlib_s32) * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	l_buffer = s_buffer + height * width;
	d_buffer = s_buffer + height * ss_x;
	ss_buffer = dp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + ss_x * dd_y;

	mlib_TwoTen_1D_ForwardTransform_S32_S32(s_buffer, height, d_buffer,
		height, sp, width, height, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ss_buffer, ss_x, sd_buffer,
		ss_x, s_buffer, height, ss_x, l_buffer);

	mlib_TwoTen_1D_ForwardTransform_S32_S32(ds_buffer, dd_x, dd_buffer,
		dd_x, d_buffer, height, dd_x, l_buffer);

	__mlib_free(s_buffer);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* Inverse transform */

/* *********************************************************** */

#define	TT_1D_INVERSE(FRM_IN, FRM_OUT, FRM_BUF)                    \
	{                                                          \
	    FRM_BUF *c22, *c3, *c22_b =                            \
		    (FRM_BUF *) coeff_22 + 2, *c3_b =              \
		    (FRM_BUF *) coeff_3 + 2;                       \
	    mlib_s32 i, j, p, p1, p2;                              \
	    FRM_IN *buf, *buf_line =                               \
		    (FRM_IN *) l_buffer + 2, *s, *d, *pss, *pdd;   \
	    FRM_OUT *x, *pxx;                                      \
	                                                           \
	    pxx = xx_ptr;                                          \
	    pss = ss_ptr;                                          \
	    pdd = dd_ptr;                                          \
	    for (j = 0; j < ss_width; j++) {                       \
		s = pss;                                           \
		buf = buf_line;                                    \
		buf[-1] = *s;                                      \
		c22 = c22_b;                                       \
		c3 = c3_b;                                         \
		for (i = 0; i < ss_height; i++) {                  \
		    (*buf++) = *s;                                 \
		    (*c3++) = *s * 3;                              \
		    (*c22++) = *s * 22;                            \
		    s += ss_width;                                 \
		}                                                  \
		buf[0] = buf[-1];                                  \
		buf[1] = buf[-2];                                  \
		c3[0] = buf[-1] * 3;                               \
		c3[1] = buf[-2] * 3;                               \
		c22[0] = buf[-1] * 22;                             \
		c22[1] = buf[-2] * 22;                             \
		buf = buf_line;                                    \
		c3 = c3_b;                                         \
		c22 = c22_b;                                       \
		buf[-1] = buf[0];                                  \
		buf[-2] = buf[1];                                  \
		c3[-1] = buf[0] * 3;                               \
		c3[-2] = buf[1] * 3;                               \
		c22[-1] = buf[0] * 22;                             \
		c22[-2] = buf[1] * 22;                             \
		d = pdd;                                           \
		x = pxx;                                           \
		s = buf;                                           \
		for (i = 0; i < dd_height; i++) {                  \
		    p1 = c3[i - 2] - c3[i + 2];                    \
		    p2 = c22[i + 1] - c22[i - 1];                  \
		    p = (p1 + p2 + 32) >> 6;                       \
		    x[0] = *s + ((*d - p + 1) >> 1);               \
		    x[1] = *s - ((*d - p) >> 1);                   \
		    x += 2;                                        \
		    s++;                                           \
		    d += dd_width;                                 \
		}                                                  \
		if (dd_height != ss_height)                        \
		    *x = *s;                                       \
		pss++;                                             \
		pdd++;                                             \
		pxx += xx_width;                                   \
	    }                                                      \
	}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_U8_S16(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s16,
		mlib_u8,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S16_S16(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s16,
		mlib_s16,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S16_S32(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s32,
		mlib_s16,
		mlib_s32)
}

/* *********************************************************** */

static void
mlib_TwoTen_1D_InverseTransform_S32_S32(
	void *xx_ptr,
	mlib_s32 xx_width,
	void *ss_ptr,
	mlib_s32 ss_width,
	mlib_s32 ss_height,
	void *dd_ptr,
	mlib_s32 dd_width,
	mlib_s32 dd_height,
	void *l_buffer,
	void *coeff_22,
	void *coeff_3)
{
	TT_1D_INVERSE(mlib_s32,
		mlib_s32,
		mlib_s32)
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_U8_S16(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s16 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max, max1, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max1 = max * sizeof (mlib_s32);
	max *= sizeof (mlib_s16);

	size = (width * height + 4) * sizeof (mlib_s16);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 2 * (max + max1));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max1);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max1 * 2);

	mlib_TwoTen_1D_InverseTransform_S16_S16(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_U8_S16(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s16 *s_buffer, *d_buffer, *l_buffer;
	mlib_s16 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s16 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max, max1, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max1 = max * sizeof (mlib_s32);
	max *= sizeof (mlib_s16);

	size = (width * height + 4) * sizeof (mlib_s16);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 2 * (max + max1));

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max1);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max1 * 2);

	mlib_TwoTen_1D_InverseTransform_S16_S16(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S16(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S16_S32(
	mlib_s16 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max *= sizeof (mlib_s32);

	size = (width * height + 4) * sizeof (mlib_s32);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 4 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max * 2);

	mlib_TwoTen_1D_InverseTransform_S32_S32(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S16_S32(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_1_Level_ITTT_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height)
{
	void *sp = (void *)src, *dp = (void *)dst;
	mlib_s32 *s_buffer, *d_buffer, *l_buffer;
	mlib_s32 *ss_buffer, *sd_buffer, *ds_buffer, *dd_buffer;
	mlib_s32 *coeff_22, *coeff_3;
	mlib_s32 ss_x = (width + 1) >> 1, ss_y = (height + 1) >> 1;
	mlib_s32 dd_x = width - ss_x, dd_y = height - ss_y, max, size;

	max = width;
	max -= ((max - height) & ((max - height) >> 31));
	max += 9;
	max *= sizeof (mlib_s32);

	size = (width * height + 4) * sizeof (mlib_s32);
	size &= ~7;

	s_buffer = __mlib_malloc(size + 4 * max);

	if (s_buffer == NULL)
		return (MLIB_NULLPOINTER);

	d_buffer = s_buffer + height * ss_x;
	ss_buffer = sp;
	ds_buffer = ss_buffer + ss_x * ss_y;
	sd_buffer = ss_buffer + width * ss_y;
	dd_buffer = sd_buffer + dd_y * ss_x;

	coeff_22 = (void *)((mlib_u8 *)s_buffer + size);
	coeff_3 = (void *)((mlib_u8 *)coeff_22 + max);
	l_buffer = (void *)((mlib_u8 *)coeff_22 + max * 2);

	mlib_TwoTen_1D_InverseTransform_S32_S32(s_buffer, height, ss_buffer,
		ss_x, ss_y, sd_buffer, ss_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(d_buffer, height, ds_buffer,
		dd_x, ss_y, dd_buffer, dd_x, dd_y, l_buffer, coeff_22, coeff_3);

	mlib_TwoTen_1D_InverseTransform_S32_S32(dp, width, s_buffer, height,
		ss_x, d_buffer, height, dd_x, l_buffer, coeff_22, coeff_3);

	__mlib_free(s_buffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
