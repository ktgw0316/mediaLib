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

#pragma ident	"@(#)mlib_v_ImageMulAlphaFunc_Inp.c	9.2	07/11/05 SMI"

/*
 * Internal functions fot ImageMulAlpha_Inp function.
 *
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_1(mlib_d64 *src_dst,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_2(mlib_d64 *src_dst,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_3(mlib_d64 *src_dst,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_4(mlib_d64 *src_dst,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_1(mlib_d64 *src_dst,
 *                                                  mlib_s32 src_dst_stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_2(mlib_d64 *src_dst,
 *                                                  mlib_s32 src_dst_stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_3(mlib_d64 *src_dst,
 *                                                  mlib_s32 src_dst_stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_4(mlib_d64 *src_dst,
 *                                                  mlib_s32 src_dst_stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_1(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_2(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_3(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_4(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_1(mlib_d64 *src_dst,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_2(mlib_d64 *src_dst,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_3(mlib_d64 *src_dst,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_4(mlib_d64 *src_dst,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_1(mlib_d64 *src_dst,
 *                                                   mlib_s32 src_dst_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_2(mlib_d64 *src_dst,
 *                                                   mlib_s32 src_dst_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_3(mlib_d64 *src_dst,
 *                                                   mlib_s32 src_dst_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_4(mlib_d64 *src_dst,
 *                                                   mlib_s32 src_dst_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_2(mlib_s16 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S16_3(mlib_s16 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_1(mlib_d64 *src,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_2(mlib_d64 *src,
 *                                                mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_1(mlib_d64 *src,
 *                                                  mlib_s32 stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_2(mlib_d64 *src,
 *                                                  mlib_s32 stride,
 *                                                  mlib_s32 size,
 *                                                  mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_2_OFF_1(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U8_2_OFF_2(mlib_u8  *s_data,
 *                                                     mlib_s32 s_stride,
 *                                                     mlib_s32 width,
 *                                                     mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S32_2(mlib_s32 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S32_3(mlib_s32 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_S32_4(mlib_s32 *src,
 *                                         mlib_s32 stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_1(mlib_d64 *src,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_2(mlib_d64 *src,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_3(mlib_d64 *src,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_4(mlib_d64 *src,
 *                                                 mlib_s32 size);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_1(mlib_d64 *src,
 *                                                   mlib_s32 src_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_2(mlib_d64 *src,
 *                                                   mlib_s32 src_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_3(mlib_d64 *src,
 *                                                   mlib_s32 src_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_4(mlib_d64 *src,
 *                                                   mlib_s32 src_stride,
 *                                                   mlib_s32 size,
 *                                                   mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_1(mlib_u8  *s_data,
 *                                                      mlib_s32 s_stride,
 *                                                      mlib_s32 width,
 *                                                      mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_2(mlib_u8  *s_data,
 *                                                      mlib_s32 s_stride,
 *                                                      mlib_s32 width,
 *                                                      mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_3(mlib_u8  *s_data,
 *                                                      mlib_s32 s_stride,
 *                                                      mlib_s32 width,
 *                                                      mlib_s32 height);
 *
 *     mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_4(mlib_u8  *s_data,
 *                                                      mlib_s32 s_stride,
 *                                                      mlib_s32 width,
 *                                                      mlib_s32 height);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_3(mlib_u16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_u16 *dst,
 *                                         mlib_s32 d_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 *     void mlib_v_ImageMulAlpha_Inp_U16_2(mlib_u16 *src,
 *                                         mlib_s32 s_stride,
 *                                         mlib_u16 *dst,
 *                                         mlib_s32 d_stride,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 alpha);
 *
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 - shift) << 20

/* *********************************************************** */

#define	MAX_SIZE	1024

/* *********************************************************** */

#define	ALPHA_NO_OFFSET(END_F, END_READ)                        \
	for (i = 0; i < size; i++) {                            \
	    p_src = *src_dst;                                   \
	    alpha_1 = vis_fexpand(vis_read_hi(p_src));          \
	    alpha_2 = vis_fexpand(vis_read_lo(p_src));          \
	    dest_1 =                                            \
		vis_fmul8x16##END_F(vis_read_hi(p_src),         \
		vis_read_##END_READ(alpha_1));                  \
	    dest_2 =                                            \
		vis_fmul8x16##END_F(vis_read_lo(p_src),         \
		vis_read_##END_READ(alpha_2));                  \
	    dest_1 = vis_fpack16_pair(dest_1, dest_2);          \
	    vis_pst_8(dest_1, src_dst, pst_maska);              \
	    src_dst++;                                          \
	}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 i;
	mlib_u8 pst_maska = 0x077;

	vis_write_gsr(3 << 3);
	size >>= 3;
#pragma pipeloop(0)
	ALPHA_NO_OFFSET(au, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 i;
	mlib_u8 pst_maska = 0x0BB;

	vis_write_gsr(3 << 3);
	size >>= 3;
#pragma pipeloop(0)
	ALPHA_NO_OFFSET(al, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 i;
	mlib_u8 pst_maska = 0x0DD;

	vis_write_gsr(3 << 3);
	size >>= 3;
#pragma pipeloop(0)
	ALPHA_NO_OFFSET(au, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 i;
	mlib_u8 pst_maska = 0x0EE;

	vis_write_gsr(3 << 3);
	size >>= 3;
#pragma pipeloop(0)
	ALPHA_NO_OFFSET(al, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 src_dst_offset, i, j;
	mlib_u8 pst_maska = 0x077;

	vis_write_gsr(3 << 3);

	size >>= 1;
	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET(au, hi)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 src_dst_offset, i, j;
	mlib_u8 pst_maska;

	pst_maska = 0x0BB;
	vis_write_gsr(3 << 3);

	size >>= 1;
	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET(al, hi)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 src_dst_offset, i, j;
	mlib_u8 pst_maska;

	pst_maska = 0x0DD;
	vis_write_gsr(3 << 3);

	size >>= 1;
	src_dst_offset = (src_dst_stride >> 3) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET(au, lo)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2;
	mlib_s32 src_dst_offset, i, j;
	mlib_u8 pst_maska;

	pst_maska = 0x0EE;
	vis_write_gsr(3 << 3);

	size >>= 1;
	src_dst_offset = (src_dst_stride >> 3) - size;

/* row loop */
	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET(al, lo)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

#define	ALPHA_MAKE_INP_U8(source, result, END_F, END_READ)      \
	alpha_1 = vis_fexpand(vis_read_hi(source));             \
	alpha_2 = vis_fexpand(vis_read_lo(source));             \
	dest_1 =                                                \
	vis_fmul8x16##END_F(vis_read_hi(source),                \
	    vis_read_##END_READ(alpha_1));                      \
	dest_2 =                                                \
	vis_fmul8x16##END_F(vis_read_lo(source),                \
	    vis_read_##END_READ(alpha_2));                      \
	result = vis_fpack16_pair(dest_1, dest_2)

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_4_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2, result;
	mlib_d64 p_src, *src = 0;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;
	mlib_u8 mask_write = 0x77, mask_write_end = 0x70;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	size_buf = (width + 1) << 2;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

#pragma pipeloop(0)
		for (i = 0; i <= width - 2; i += 2) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, au, hi);
			vis_pst_8(result, (void *)src, mask_write);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, au, hi);
			vis_pst_8(result, (void *)src, mask_write_end);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 2);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_4_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2, result;
	mlib_d64 p_src, *src = 0;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;
	mlib_u8 mask_write = 0xBB, mask_write_end = 0xB0;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	size_buf = (width + 1) << 2;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

#pragma pipeloop(0)
		for (i = 0; i <= width - 2; i += 2) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, al, hi);
			vis_pst_8(result, (void *)src, mask_write);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, al, hi);
			vis_pst_8(result, (void *)src, mask_write_end);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 2);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_4_OFF_3(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2, result;
	mlib_d64 p_src, *src = 0;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;
	mlib_u8 mask_write = 0xDD, mask_write_end = 0xD0;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	size_buf = (width + 1) << 2;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

#pragma pipeloop(0)
		for (i = 0; i <= width - 2; i += 2) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, au, lo);
			vis_pst_8(result, (void *)src, mask_write);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, au, lo);
			vis_pst_8(result, (void *)src, mask_write_end);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 2);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_4_OFF_4(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 alpha_1, alpha_2, dest_1, dest_2, result;
	mlib_d64 p_src, *src = 0;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;
	mlib_u8 mask_write = 0xEE, mask_write_end = 0xE0;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	size_buf = (width + 1) << 2;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

#pragma pipeloop(0)
		for (i = 0; i <= width - 2; i += 2) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, al, lo);
			vis_pst_8(result, (void *)src, mask_write);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_MAKE_INP_U8(p_src, result, al, lo);
			vis_pst_8(result, (void *)src, mask_write_end);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 2);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_MAKE_INP_S16(END_F, END_READ)                          \
	for (i = 0; i < size; i++) {                                  \
	    p_src = *src_dst;                                         \
	    d_alpha_new_ = vis_fand(p_src, d_help_);                  \
	    d_alpha_mul_ =                                            \
		vis_fmul8x16##END_F(f_mul_,                           \
		vis_read_##END_READ(d_alpha_new_));                   \
	    d_alpha_mul_ = vis_fpadd16(d_alpha_mul_, d_alpha_mul_);   \
	    d_hi_mul_ = vis_fmul8sux16(p_src, d_alpha_mul_);          \
	    d_lo_mul_ = vis_fmul8ulx16(p_src, d_alpha_mul_);          \
	    d_mul_ = vis_fpadd16(d_hi_mul_, d_lo_mul_);               \
	    d_mul_ = vis_fpadd16(d_mul_, d_mul_);                     \
	    vis_pst_8(d_mul_, src_dst, pst_maska);                    \
	    src_dst++;                                                \
	}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 pst_maska = 0x03F;
	mlib_f32 f_mul_ = vis_to_float(0x00808080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_S16(au, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 pst_maska = 0x0CF;
	mlib_f32 f_mul_ = vis_to_float(0x80008080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_S16(al, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 pst_maska = 0x0F3;
	mlib_f32 f_mul_ = vis_to_float(0x80800080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_S16(au, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size)
{
	mlib_u8 pst_maska = 0x0FC;
	mlib_f32 f_mul_ = vis_to_float(0x80808000);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_S16(al, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 pst_maska = 0x03F;
	mlib_f32 f_mul_ = vis_to_float(0x00808080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 src_dst_offset, i, j;

	src_dst_offset = (src_dst_stride >> 2) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_S16(au, hi)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 pst_maska = 0x0CF;
	mlib_f32 f_mul_ = vis_to_float(0x80008080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 src_dst_offset, i, j;

	src_dst_offset = (src_dst_stride >> 2) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_S16(al, hi)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 pst_maska = 0x0F3;
	mlib_f32 f_mul_ = vis_to_float(0x80800080);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 src_dst_offset, i, j;

	src_dst_offset = (src_dst_stride >> 2) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_S16(au, lo)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_u8 pst_maska = 0x0FC;
	mlib_f32 f_mul_ = vis_to_float(0x80808000);
	mlib_d64 d_help_ = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 d_alpha_mul_, d_hi_mul_, d_lo_mul_, d_mul_, d_alpha_new_;
	mlib_d64 p_src;
	mlib_s32 src_dst_offset, i, j;

	src_dst_offset = (src_dst_stride >> 2) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_S16(al, lo)
		    src_dst += src_dst_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_2(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a, d_0;

	stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = src[0];
				d_0 = (src[1] * a) >> 15;
				src[1] = d_0;
				src += 2;
			}

			src += stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a = src[1];
			d_0 = (src[0] * a) >> 15;
			src[0] = d_0;
			src += 2;
		}

		src += stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S16_3(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, a, d_0, d_1;

	stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = src[0];
				d_0 = (src[1] * a) >> 15;
				d_1 = (src[2] * a) >> 15;
				src[1] = d_0;
				src[2] = d_1;
				src += 3;
			}

			src += stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = src[1];
				d_0 = (src[0] * a) >> 15;
				d_1 = (src[2] * a) >> 15;
				src[0] = d_0;
				src[2] = d_1;
				src += 3;
			}

			src += stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a = src[2];
			d_0 = (src[0] * a) >> 15;
			d_1 = (src[1] * a) >> 15;
			src[0] = d_0;
			src[1] = d_1;
			src += 3;
		}

		src += stride;
	}
}

/* *********************************************************** */

#define	ALPHA_NO_OFFSET_2                                       \
	for (i = 0; i < size; i++) {                            \
	    p_src = *src;                                       \
	    alpha = vis_fand(dmask, p_src);                     \
	    without_alpha = vis_fandnot(dmask, p_src);          \
	    p_src = vis_faligndata(p_src, p_src);               \
	    rdh =                                               \
		vis_fmul8x16(vis_read_hi(p_src),                \
		vis_fexpand_hi(without_alpha));                 \
	    rdl =                                               \
		vis_fmul8x16(vis_read_lo(p_src),                \
		vis_fexpand_lo(without_alpha));                 \
	    result = vis_fpack16_pair(rdh, rdl);                \
	    *src = vis_for(result, alpha);                      \
	    src++;                                              \
	}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_1(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i;
	mlib_d64 dmask = vis_to_double_dup(0xFF00FF00);

	vis_write_gsr(24);
	vis_alignaddr(src, 7);
	size >>= 3;

#pragma pipeloop(0)
	ALPHA_NO_OFFSET_2
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_2(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_d64 p_src;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i;
	mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);

	vis_write_gsr(24);
	vis_alignaddr(src, 1);
	size >>= 3;

#pragma pipeloop(0)
	ALPHA_NO_OFFSET_2
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_1(
    mlib_d64 *src,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, offset;
	mlib_d64 dmask = vis_to_double_dup(0xFF00FF00);

	vis_write_gsr(24);
	vis_alignaddr(src, 7);
	size >>= 2;
	offset = (stride >> 3) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET_2 src += offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_2(
    mlib_d64 *src,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_d64 p_src;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, offset;
	mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);

	vis_write_gsr(24);
	vis_alignaddr(src, 1);
	size >>= 2;
	offset = (stride >> 3) - size;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		ALPHA_NO_OFFSET_2 src += offset;
	}
}

/* *********************************************************** */

#define	ALPHA_OFFSET_U8(SRC, DST)                                         \
	_alpha = vis_fand(_dmask, SRC);                                   \
	_without_alpha = vis_fandnot(_dmask, SRC);                        \
	SRC = vis_faligndata(SRC, SRC);                                   \
	rdh =                                                             \
	vis_fmul8x16(vis_read_hi(SRC), vis_fexpand_hi(_without_alpha));   \
	rdl =                                                             \
	vis_fmul8x16(vis_read_lo(SRC), vis_fexpand_lo(_without_alpha));   \
	_result = vis_fpack16_pair(rdh, rdl);                             \
	DST = vis_for(_result, _alpha)

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_2_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 _alpha, _without_alpha, _result;
	mlib_d64 _dmask = vis_to_double_dup(0xFF00FF00);
	mlib_u8 mask_array[] = { 0x00, 0xC0, 0xF0, 0xFC };
	mlib_d64 p_src, *src = 0;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;

	vis_write_gsr(24);

	if (s_stride == (width << 1)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 1;
	}

	size_buf = (width + 1) << 1;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		vis_alignaddr(src, 7);
#pragma pipeloop(0)
		for (i = 0; i <= width - 4; i += 4) {
			p_src = *src;

			alpha = vis_fand(_dmask, p_src);
			without_alpha = vis_fandnot(_dmask, p_src);
			p_src = vis_faligndata(p_src, p_src);

			rdh =
			    vis_fmul8x16(vis_read_hi(p_src),
			    vis_fexpand_hi(without_alpha));
			rdl =
			    vis_fmul8x16(vis_read_lo(p_src),
			    vis_fexpand_lo(without_alpha));

			result = vis_fpack16_pair(rdh, rdl);

			*src = vis_for(result, alpha);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_OFFSET_U8(p_src, result);
			vis_pst_8(result, (void *)src, mask_array[width - i]);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 1);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U8_2_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 _alpha, _without_alpha, _result;
	mlib_d64 _dmask = vis_to_double_dup(0x00FF00FF);
	mlib_u8 mask_array[] = { 0x00, 0xC0, 0xF0, 0xFC };
	mlib_d64 p_src, *src = 0;
	mlib_d64 alpha, without_alpha, result;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, size_buf;
	mlib_u8 *buf;

	vis_write_gsr(24);

	if (s_stride == (width << 1)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 1;
	}

	size_buf = (width + 1) << 1;
	buf = (mlib_u8 *)__mlib_malloc(size_buf + 16);

	if (buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, buf, size_buf);
			src = (mlib_d64 *)buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		vis_alignaddr(src, 1);
#pragma pipeloop(0)
		for (i = 0; i <= width - 4; i += 4) {
			p_src = *src;

			alpha = vis_fand(_dmask, p_src);
			without_alpha = vis_fandnot(_dmask, p_src);
			p_src = vis_faligndata(p_src, p_src);

			rdh =
			    vis_fmul8x16(vis_read_hi(p_src),
			    vis_fexpand_hi(without_alpha));
			rdl =
			    vis_fmul8x16(vis_read_lo(p_src),
			    vis_fexpand_lo(without_alpha));

			result = vis_fpack16_pair(rdh, rdl);

			*src = vis_for(result, alpha);
			src++;
		}

		if (i < width) {
			p_src = *src;
			ALPHA_OFFSET_U8(p_src, result);
			vis_pst_8(result, (void *)src, mask_array[width - i]);
		}

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(buf, s_data, width << 1);
		}

		s_data += s_stride;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S32_2(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0;
	mlib_d64 a, shift_const = 1. / 2147483648.;

	stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[0];
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				src[1] = d_0;
				src += 2;
			}

			src += stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a = (mlib_d64)src[1];
			d_0 = ((mlib_d64)src[0] * a * shift_const);
			src[0] = d_0;
			src += 2;
		}

		src += stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S32_3(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1;
	mlib_d64 a, shift_const = 1. / 2147483648.;

	stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[0];
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				src[1] = d_0;
				src[2] = d_1;
				src += 3;
			}

			src += stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[1];
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				src[0] = d_0;
				src[2] = d_1;
				src += 3;
			}

			src += stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a = (mlib_d64)src[2];
			d_0 = (mlib_s32)((mlib_d64)src[0] * a * shift_const);
			d_1 = (mlib_s32)((mlib_d64)src[1] * a * shift_const);
			src[0] = d_0;
			src[1] = d_1;
			src += 3;
		}

		src += stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_S32_4(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 d_0, d_1, d_2;
	mlib_d64 a, shift_const = 1. / 2147483648.;

	stride -= width << 2;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[0];
				d_0 = ((mlib_d64)src[1] * a * shift_const);
				d_1 = ((mlib_d64)src[2] * a * shift_const);
				d_2 = ((mlib_d64)src[3] * a * shift_const);
				src[1] = d_0;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[1];
				d_0 = ((mlib_d64)src[0] * a * shift_const);
				d_1 = ((mlib_d64)src[2] * a * shift_const);
				d_2 = ((mlib_d64)src[3] * a * shift_const);
				src[0] = d_0;
				src[2] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

		return;
	}

	if (alpha == 2) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				a = (mlib_d64)src[2];
				d_0 = ((mlib_d64)src[0] * a * shift_const);
				d_1 = ((mlib_d64)src[1] * a * shift_const);
				d_2 = ((mlib_d64)src[3] * a * shift_const);
				src[0] = d_0;
				src[1] = d_1;
				src[3] = d_2;
				src += 4;
			}

			src += stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			a = (mlib_d64)src[3];
			d_0 = ((mlib_d64)src[0] * a * shift_const);
			d_1 = ((mlib_d64)src[1] * a * shift_const);
			d_2 = ((mlib_d64)src[2] * a * shift_const);
			src[0] = d_0;
			src[1] = d_1;
			src[2] = d_2;
			src += 4;
		}

		src += stride;
	}
}

/* *********************************************************** */

#define	MLIB_V_MAKE_INP_U16(END_F, END_READ)                          \
	for (i = 0; i < size; i++) {                                  \
	    p_src = *src;                                             \
	    p_src = vis_fxor(p_src, xor_mask);                        \
	    p_src_tmp = vis_fand(p_src, and_mask);                    \
	    d_alpha_half =                                            \
		vis_fmul8x16##END_F(f_mul,                            \
		vis_read_##END_READ(p_src_tmp));                      \
	    d_alpha_mull = vis_fpadd16(d_alpha_half, d_alpha_half);   \
	    p_src_tmp = vis_fmul8x16(f_mul, p_src_tmp);               \
	    d_hi_mul = vis_fmul8sux16(p_src, d_alpha_mull);           \
	    d_lo_mul = vis_fmul8ulx16(p_src, d_alpha_mull);           \
	    d_mul = vis_fpadd16(d_hi_mul, d_lo_mul);                  \
	    d_mul = vis_fpadd16(d_mul, sat_offset);                   \
	    d_mul = vis_fpadd16(d_mul, d_alpha_half);                 \
	    d_mul = vis_fpadd16(d_mul, p_src_tmp);                    \
	    vis_pst_16(d_mul, src, edge);                             \
	    src++;                                                    \
	}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_1(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_f32 f_mul = vis_to_float(0x00808080);
	mlib_u8 edge = 7;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_U16(au, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_2(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_f32 f_mul = vis_to_float(0x80008080);
	mlib_u8 edge = 11;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_U16(al, hi)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_3(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_f32 f_mul = vis_to_float(0x80800080);
	mlib_u8 edge = 13;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_U16(au, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_4(
    mlib_d64 *src,
    mlib_s32 size)
{
	mlib_f32 f_mul = vis_to_float(0x80808000);
	mlib_u8 edge = 14;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 i;

	size >>= 2;
#pragma pipeloop(0)
	MLIB_V_MAKE_INP_U16(al, lo)
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_1(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x00808080);
	mlib_u8 edge = 7;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 src_offset, i, j;

	src_offset = (src_stride >> 2) - size;

/* row loop */
	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(au, hi)
		    src += src_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_2(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80008080);
	mlib_u8 edge = 11;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 src_offset, i, j;

	src_offset = (src_stride >> 2) - size;

/* row loop */
	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(al, hi)
		    src += src_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_3(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80800080);
	mlib_u8 edge = 13;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 src_offset, i, j;

	src_offset = (src_stride >> 2) - size;

/* row loop */
	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(au, lo)
		    src += src_offset;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_4(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80808000);
	mlib_u8 edge = 14;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 src_offset, i, j;

	src_offset = (src_stride >> 2) - size;

/* row loop */
	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(al, lo)
		    src += src_offset;
	}
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U16_4_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x00808080);
	mlib_u8 edge = 7;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_d64 *src = 0;
	mlib_s32 i, j, size_buf, size;
	mlib_u8 *p_src_buf;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	s_stride <<= 1;

	size_buf = ((width + 1) >> 1) << 4;
	p_src_buf = (mlib_u8 *)__mlib_malloc((size_buf << 1) + 16);

	if (p_src_buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, p_src_buf, size_buf);
			src = (mlib_d64 *)p_src_buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		size = width;
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(au, hi)

		    if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(p_src_buf, s_data, width << 3);
		}

		s_data += s_stride;
	}

	__mlib_free(p_src_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U16_4_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80008080);
	mlib_u8 edge = 11;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 *src = 0;
	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_s32 i, j, size_buf, size;
	mlib_u8 *p_src_buf;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	s_stride <<= 1;

	size_buf = ((width + 1) >> 1) << 4;
	p_src_buf = (mlib_u8 *)__mlib_malloc((size_buf << 1) + 16);

	if (p_src_buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, p_src_buf, size_buf);
			src = (mlib_d64 *)p_src_buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		size = width;
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(al, hi)

		    if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(p_src_buf, s_data, width << 3);
		}

		s_data += s_stride;
	}

	__mlib_free(p_src_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U16_4_OFF_3(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80800080);
	mlib_u8 edge = 13;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;
	mlib_d64 *src = 0;
	mlib_s32 i, j, size_buf, size;
	mlib_u8 *p_src_buf;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	s_stride <<= 1;

	size_buf = ((width + 1) >> 1) << 4;
	p_src_buf = (mlib_u8 *)__mlib_malloc((size_buf << 1) + 16);

	if (p_src_buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, p_src_buf, size_buf);
			src = (mlib_d64 *)p_src_buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		size = width;
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(au, lo)

		    if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(p_src_buf, s_data, width << 3);
		}

		s_data += s_stride;
	}

	__mlib_free(p_src_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageMulAlpha_Inp_U16_4_OFF_4(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_f32 f_mul = vis_to_float(0x80808000);
	mlib_u8 edge = 14;

	mlib_d64 and_mask = vis_to_double_dup(0xFFFEFFFE);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);
	mlib_d64 *src = 0;
	mlib_d64 d_hi_mul, d_lo_mul, d_mul;
	mlib_d64 p_src, p_src_tmp, d_alpha_mull, d_alpha_half;

	mlib_s32 i, j, size_buf, size;
	mlib_u8 *p_src_buf;

	vis_write_gsr(24);

	if (s_stride == (width << 2)) {
		width *= height;
		height = 1;
	}

	if (width > MAX_SIZE && height == 1) {
		while ((width & 1) == 0 && width > MAX_SIZE) {
			width >>= 1;
			height <<= 1;
		}

		for (i = 3; width > MAX_SIZE; i += 2) {
			while ((width % i) == 0 && width > MAX_SIZE) {
				width /= i;
				height *= i;
			}
		}

		s_stride = width << 2;
	}

	s_stride <<= 1;

	size_buf = ((width + 1) >> 1) << 4;
	p_src_buf = (mlib_u8 *)__mlib_malloc((size_buf << 1) + 16);

	if (p_src_buf == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(s_data, p_src_buf, size_buf);
			src = (mlib_d64 *)p_src_buf;
		} else {
			src = (mlib_d64 *)s_data;
		}

		size = width;
#pragma pipeloop(0)
		MLIB_V_MAKE_INP_U16(al, lo)

		    if ((mlib_addr)s_data & 7) {
			mlib_ImageCopy_na(p_src_buf, s_data, width << 3);
		}

		s_data += s_stride;
	}

	__mlib_free(p_src_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1, d_2;
	mlib_f32 a;
	mlib_d64 dscale;
	mlib_f32 fscale;

/* dscale = 2 ** (-16) */
	SCALBN(dscale, 16);
	fscale = dscale;

	s_stride -= width + (width << 1);
	d_stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_2 = src[0];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[1] * a * fscale);
				d_1 = (mlib_s32)((mlib_f32)src[2] * a * fscale);
				dst[0] = d_2;
				dst[1] = d_0;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_2 = src[1];
				a = (mlib_f32)d_2;
				d_0 = (mlib_s32)((mlib_f32)src[0] * a * fscale);
				d_1 = (mlib_s32)((mlib_f32)src[2] * a * fscale);
				dst[0] = d_0;
				dst[1] = d_2;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			d_2 = src[2];
			a = (mlib_f32)d_2;
			d_0 = (mlib_s32)((mlib_f32)src[0] * a * fscale);
			d_1 = (mlib_s32)((mlib_f32)src[1] * a * fscale);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			src += 3;
			dst += 3;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulAlpha_Inp_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1;
	mlib_d64 dscale;
	mlib_f32 fscale;

/* dscale = 2 ** (-16) */
	SCALBN(dscale, 16);
	fscale = dscale;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_0 = src[0];
				d_1 =
				    (mlib_s32)((mlib_f32)src[1] *
				    (mlib_f32)d_0 * fscale);
				dst[0] = d_0;
				dst[1] = d_1;
				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			d_0 = src[1];
			d_1 =
			    (mlib_s32)((mlib_f32)src[0] * (mlib_f32)d_0 *
			    fscale);
			dst[0] = d_1;
			dst[1] = d_0;
			src += 2;
			dst += 2;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */
