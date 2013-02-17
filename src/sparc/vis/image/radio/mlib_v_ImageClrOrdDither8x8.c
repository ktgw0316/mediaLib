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

#pragma ident	"@(#)mlib_v_ImageClrOrdDither8x8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorOrderedDither8x8 - convert a true color image to an
 *                                        indexed color image by ordered
 *                                        dithering
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageColorOrderedDither8x8(mlib_image       *dst,
 *                                                   const mlib_image *src,
 *                                                   const mlib_s32   *dmask,
 *                                                   mlib_s32         scale,
 *                                                   const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      dmask     Pointer to the dither mask, in row major order.
 *      scale     The scaling factor for dmask to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      Convert a true color image to a pseudo color image with the
 *      method of ordered dithering.
 *
 * RESTRICTIONS
 *      The src can be an MLIB_BYTE or MLIB_SHORT image with 3 or 4 channels.
 *      The dst must be a 1-channel MLIB_BYTE or MLIB_SHORT image.
 *
 *      The colormap must be created by
 *      mlib_ImageColorTrue2IndexInit with colorcube
 *      included (bits != 0) or by mlib_ImageColorDitherInit()
 *      with dimensions != NULL.
 *
 *      0 <= dmask[i][j] < 2^scale;
 *      scale >= 0
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorOrderedDither8x8 = \
	__mlib_ImageColorOrderedDither8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorOrderedDither8x8) mlib_ImageColorOrderedDither8x8
    __attribute__((weak, alias("__mlib_ImageColorOrderedDither8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_v_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

mlib_status
__mlib_ImageColorOrderedDither8x8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *dmask,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_status stat;
	mlib_s32 src_nch;
	mlib_type src_dtype, dst_dtype;
	mlib_s32 method;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	if (dmask == NULL || colormap == NULL)
		return (MLIB_NULLPOINTER);

	src_dtype = mlib_ImageGetType(src);
	dst_dtype = mlib_ImageGetType(dst);
	src_nch = mlib_ImageGetChannels(src);

	if (scale < 0) {
		stat = MLIB_OUTOFRANGE;
	} else {
		method = mlib_ImageGetMethod(colormap);

		if (method != LUT_COLOR_CUBE_SEARCH) {
			if (method == LUT_COLOR_DIMENSIONS) {
				mlib_s32 dmask1[64], dmask2[64], dmask3[64],
				    *pmask[4];
				mlib_s32 i, j;

				for (j = 0; j < 8; j++) {
					for (i = 0; i < 8; i++) {
						dmask3[8 * (7 - j) + (7 - i)] =
						    dmask2[8 * (7 - j) + i] =
						    dmask1[8 * j + (7 - i)] =
						    dmask[8 * j + i];
					}
				}

				pmask[0] = (void *)dmask;
				pmask[1] = dmask1;
				pmask[2] = dmask2;
				pmask[3] = dmask3;
				return __mlib_ImageColorOrderedDitherMxN(dst,
				    src, (const mlib_s32 **)pmask, 8, 8, scale,
				    colormap);
			}

			return (MLIB_FAILURE);
		} else {
			switch (src_dtype) {
			case MLIB_BYTE:
				switch (dst_dtype) {
				case MLIB_BYTE:
					switch (src_nch) {
					case 3:
					    stat =
						mlib_v_OrderedDither8x8_3_8_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_v_OrderedDither8x8_4_8_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
					switch (src_nch) {
					case 3:
					    stat =
						mlib_v_OrderedDither8x8_3_8_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_v_OrderedDither8x8_4_8_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_BIT:
				default:
					stat = MLIB_FAILURE;
					break;
				}

				break;
			case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
				switch (dst_dtype) {
				case MLIB_BYTE:
					switch (src_nch) {
					case 3:
					    stat =
						mlib_v_OrderedDither8x8_3_16_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_v_OrderedDither8x8_4_16_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
					switch (src_nch) {
					case 3:
					    stat =
						mlib_v_OrderedDither8x8_3_16_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_v_OrderedDither8x8_4_16_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_BIT:
				default:
					stat = MLIB_FAILURE;
					break;
				}

				break;
			case MLIB_BIT:
			default:
				stat = MLIB_FAILURE;
				break;
			}
		}
	}

	return (stat);
}

/* *********************************************************** */

#define	CALCULATE_STORE_INTO_INTERM_BUF_U8(d, koeff_0, koeff_1)   \
	tmp0 = vis_fpmerge(fzero, vis_read_hi(d));                \
/*                                                                \
 * U8 -> S16                                                      \
 */                                                               \
	tmp0 = vis_fpadd16(tmp0, koeff_0);                        \
	tmp1 = vis_fmul8x16al(vis_read_lo(d), fone);              \
/*                                                                \
 * U8 -> S16                                                      \
 */                                                               \
	tmp1 = vis_fpadd16(tmp1, koeff_1);                        \
	(*intrm++) = vis_fpack16_pair(tmp0, tmp1)

/* *********************************************************** */

#define	CALCULATE_STORE_INTO_INTERM_BUF_S16(d, koeff_0, koeff_1)   \
	tmp0 = vis_fmuld8ulx16(fone, vis_read_hi(d));              \
/*                                                                 \
 * S16 -> S32                                                      \
 */                                                                \
	tmp0 = vis_fpadd32(tmp0, koeff_0);                         \
	tmp1 = vis_fmuld8ulx16(fone, vis_read_lo(d));              \
/*                                                                 \
 * S16 -> S32                                                      \
 */                                                                \
	tmp1 = vis_fpadd32(tmp1, koeff_1);                         \
	(*intrm++) = vis_fpackfix_pair(tmp0, tmp1)

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;

/* dither koeff */
	mlib_d64 dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
	mlib_d64 d1, d2, d3, d4, d5;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][6];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(7 << 3);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 1,    2, 3, 1, 2   etc. */
	for (row = 0; row < 8; row++) {
		double_dither[row][0] =
		    vis_to_double(((dither[row][0] << 16) | (dither[row][7] &
		    0xFFFF)),
		    ((dither[7 - row][0] << 16) | (dither[row][1] & 0xFFFF)));

		double_dither[row][1] =
		    vis_to_double(((dither[row][6] << 16) | (dither[7 -
		    row][1] & 0xFFFF)),
		    ((dither[row][2] << 16) | (dither[row][5] & 0xFFFF)));

		double_dither[row][2] =
		    vis_to_double(((dither[7 -
		    row][2] << 16) | (dither[row][3] & 0xFFFF)),
		    ((dither[row][4] << 16) | (dither[7 - row][3] & 0xFFFF)));

		double_dither[row][3] =
		    vis_to_double(((dither[row][4] << 16) | (dither[row][3] &
		    0xFFFF)),
		    ((dither[7 - row][4] << 16) | (dither[row][5] & 0xFFFF)));

		double_dither[row][4] =
		    vis_to_double(((dither[row][2] << 16) | (dither[7 -
		    row][5] & 0xFFFF)),
		    ((dither[row][6] << 16) | (dither[row][1] & 0xFFFF)));

		double_dither[row][5] =
		    vis_to_double(((dither[7 -
		    row][6] << 16) | (dither[row][7] & 0xFFFF)),
		    ((dither[row][0] << 16) | (dither[7 - row][7] & 0xFFFF)));
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc((((sw * 3) >> 3) +
	    2) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		d1 = vis_ld_d64_nf(sa); sa++;
#pragma pipeloop(0)
		for (i = 0; i <= sw - 6; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa); sa++;
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d5, dith_4, dith_5);
			d1 = d2;
		}

		if (i <= sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
		} else if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_U8_U8_3((mlib_u8 *)interm_buf, da,
		    sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;
	mlib_d64 d1, d2, d3, d4, d5, d6;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][8];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(7 << 3);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			double_dither[row][i] =
			    vis_to_double(((dither[row][i] << 16) |
			    (dither[row][7 - i] & 0xFFFF)),
			    ((dither[7 - row][i] << 16) | (dither[7 - row][7 -
			    i] & 0xFFFF)));
		}
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc(((sw + 1) << 1) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 7; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d6 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d6, dith_6, dith_7);
		}

		if (i <= sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			i += 4;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_U8_U8_4((mlib_u8 *)interm_buf, da,
		    sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;

/* dither koeff */
	mlib_d64 dith_8, dith_9, dith_10, dith_11;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x20002);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][12];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(15 << 3);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    ((shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult)) << 1;
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3 */
	for (row = 0; row < 8; row++) {
		double_dither[row][0] =
		    vis_to_double(dither[row][0], dither[row][7]);
		double_dither[row][1] =
		    vis_to_double(dither[7 - row][0], dither[row][1]);
		double_dither[row][2] =
		    vis_to_double(dither[row][6], dither[7 - row][1]);
		double_dither[row][3] =
		    vis_to_double(dither[row][2], dither[row][5]);
		double_dither[row][4] =
		    vis_to_double(dither[7 - row][2], dither[row][3]);
		double_dither[row][5] =
		    vis_to_double(dither[row][4], dither[7 - row][3]);
		double_dither[row][6] =
		    vis_to_double(dither[row][4], dither[row][3]);
		double_dither[row][7] =
		    vis_to_double(dither[7 - row][4], dither[row][5]);
		double_dither[row][8] =
		    vis_to_double(dither[row][2], dither[7 - row][5]);
		double_dither[row][9] =
		    vis_to_double(dither[row][6], dither[row][1]);
		double_dither[row][10] =
		    vis_to_double(dither[7 - row][6], dither[row][7]);
		double_dither[row][11] =
		    vis_to_double(dither[row][0], dither[7 - row][7]);
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc((((sw * 3) << 2) +
	    2) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		dith_8 = d_d[8];
		dith_9 = d_d[9];
		dith_10 = d_d[10];
		dith_11 = d_d[11];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 8; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d6 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d7 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d8 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d7, dith_8, dith_9);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d8, dith_10,
			    dith_11);
		}

		if (i <= sw - 4) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa); sa++;
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			i += 4;
			dith_0 = dith_6;
			dith_1 = dith_7;
			dith_2 = dith_8;
			dith_3 = dith_9;
			dith_4 = dith_10;
			dith_5 = dith_11;
			d1 = d2;
		}

		if (i == sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa);
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
		} else if (i == sw - 2) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa);
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
		} else if (i == sw - 1) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_U8_3((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;
	mlib_d64 dith_8, dith_9, dith_10, dith_11,
/* dither koeff */
	    dith_12, dith_13, dith_14, dith_15;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x20002);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][16];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(15 << 3);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    ((shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult)) << 1;
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			double_dither[row][2 * i] =
			    vis_to_double(dither[row][i], dither[row][7 - i]);
			double_dither[row][2 * i + 1] =
			    vis_to_double(dither[7 - row][i],
			    dither[7 - row][7 - i]);
		}
	}

	interm_buf = (mlib_d64 *)__mlib_malloc(sw * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		dith_8 = d_d[8];
		dith_9 = d_d[9];
		dith_10 = d_d[10];
		dith_11 = d_d[11];
		dith_12 = d_d[12];
		dith_13 = d_d[13];
		dith_14 = d_d[14];
		dith_15 = d_d[15];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 8; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d6 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d7 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d8 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d9 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d10 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d7, dith_8, dith_9);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d8, dith_10,
			    dith_11);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d9, dith_12,
			    dith_13);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d10, dith_14,
			    dith_15);
		}

		if (i <= sw - 4) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d6 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			i += 4;
			dith_0 = dith_8;
			dith_1 = dith_9;
			dith_2 = dith_10;
			dith_3 = dith_11;
			dith_4 = dith_12;
			dith_5 = dith_13;
		}

		if (i <= sw - 2) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			i += 2;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_U8_4((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;

/* dither koeff */
	mlib_d64 dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
	mlib_d64 d1, d2, d3, d4, d5;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][6];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(7 << 3);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 1,    2, 3, 1, 2   etc. */
	for (row = 0; row < 8; row++) {
		double_dither[row][0] =
		    vis_to_double(((dither[row][0] << 16) | (dither[row][7] &
		    0xFFFF)),
		    ((dither[7 - row][0] << 16) | (dither[row][1] & 0xFFFF)));

		double_dither[row][1] =
		    vis_to_double(((dither[row][6] << 16) | (dither[7 -
		    row][1] & 0xFFFF)),
		    ((dither[row][2] << 16) | (dither[row][5] & 0xFFFF)));

		double_dither[row][2] =
		    vis_to_double(((dither[7 -
		    row][2] << 16) | (dither[row][3] & 0xFFFF)),
		    ((dither[row][4] << 16) | (dither[7 - row][3] & 0xFFFF)));

		double_dither[row][3] =
		    vis_to_double(((dither[row][4] << 16) | (dither[row][3] &
		    0xFFFF)),
		    ((dither[7 - row][4] << 16) | (dither[row][5] & 0xFFFF)));

		double_dither[row][4] =
		    vis_to_double(((dither[row][2] << 16) | (dither[7 -
		    row][5] & 0xFFFF)),
		    ((dither[row][6] << 16) | (dither[row][1] & 0xFFFF)));

		double_dither[row][5] =
		    vis_to_double(((dither[7 -
		    row][6] << 16) | (dither[row][7] & 0xFFFF)),
		    ((dither[row][0] << 16) | (dither[7 - row][7] & 0xFFFF)));
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc((((sw * 3) >> 3) +
	    2) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		d1 = vis_ld_d64_nf(sa); sa++;
#pragma pipeloop(0)
		for (i = 0; i <= sw - 6; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa); sa++;
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d5, dith_4, dith_5);
			d1 = d2;
		}

		if (i <= sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
		} else if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_U8_S16_3((mlib_u8 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;
	mlib_d64 d1, d2, d3, d4, d5, d6;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][8];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(7 << 3);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			double_dither[row][i] =
			    vis_to_double(((dither[row][i] << 16) |
			    (dither[row][7 - i] & 0xFFFF)),
			    ((dither[7 - row][i] << 16) | (dither[7 - row][7 -
			    i] & 0xFFFF)));
		}
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc(((sw + 1) << 1) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 7; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d6 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d6, dith_6, dith_7);
		}

		if (i <= sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_2, dith_3);
			i += 4;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_U8_S16_4((mlib_u8 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;

/* dither koeff */
	mlib_d64 dith_8, dith_9, dith_10, dith_11;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x20002);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][12];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(15 << 3);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    ((shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult)) << 1;
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3 */
	for (row = 0; row < 8; row++) {
		double_dither[row][0] =
		    vis_to_double(dither[row][0], dither[row][7]);
		double_dither[row][1] =
		    vis_to_double(dither[7 - row][0], dither[row][1]);
		double_dither[row][2] =
		    vis_to_double(dither[row][6], dither[7 - row][1]);
		double_dither[row][3] =
		    vis_to_double(dither[row][2], dither[row][5]);
		double_dither[row][4] =
		    vis_to_double(dither[7 - row][2], dither[row][3]);
		double_dither[row][5] =
		    vis_to_double(dither[row][4], dither[7 - row][3]);
		double_dither[row][6] =
		    vis_to_double(dither[row][4], dither[row][3]);
		double_dither[row][7] =
		    vis_to_double(dither[7 - row][4], dither[row][5]);
		double_dither[row][8] =
		    vis_to_double(dither[row][2], dither[7 - row][5]);
		double_dither[row][9] =
		    vis_to_double(dither[row][6], dither[row][1]);
		double_dither[row][10] =
		    vis_to_double(dither[7 - row][6], dither[row][7]);
		double_dither[row][11] =
		    vis_to_double(dither[row][0], dither[7 - row][7]);
	}

	interm_buf =
	    (mlib_d64 *)__mlib_malloc((((sw * 3) << 2) +
	    2) * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		dith_8 = d_d[8];
		dith_9 = d_d[9];
		dith_10 = d_d[10];
		dith_11 = d_d[11];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 8; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d6 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d7 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d8 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d7, dith_8, dith_9);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d8, dith_10,
			    dith_11);
		}

		if (i <= sw - 4) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa); sa++;
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			i += 4;
			dith_0 = dith_6;
			dith_1 = dith_7;
			dith_2 = dith_8;
			dith_3 = dith_9;
			dith_4 = dith_10;
			dith_5 = dith_11;
			d1 = d2;
		}

		if (i == sw - 3) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = vis_ld_d64_nf(sa);
			d5 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
		} else if (i == sw - 2) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa);
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
		} else if (i == sw - 1) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_S16_3((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	mlib_d64 *interm_buf, *intrm;

/* pointer to row in diter array */
	mlib_d64 *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	mlib_d64 dith_0, dith_1, dith_2, dith_3,
/* dither koeff */
	    dith_4, dith_5, dith_6, dith_7;
	mlib_d64 dith_8, dith_9, dith_10, dith_11,
/* dither koeff */
	    dith_12, dith_13, dith_14, dith_15;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	mlib_d64 tmp0, tmp1;
	mlib_f32 fone = vis_to_float(0x20002);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* mlib_d64 dither array */
	mlib_d64 double_dither[8][16];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	vis_write_gsr(15 << 3);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    ((shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult)) << 1;
		}
	}

/* load diter koeff. into mlib_d64  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			double_dither[row][2 * i] =
			    vis_to_double(dither[row][i], dither[row][7 - i]);
			double_dither[row][2 * i + 1] =
			    vis_to_double(dither[7 - row][i],
			    dither[7 - row][7 - i]);
		}
	}

	interm_buf = (mlib_d64 *)__mlib_malloc(sw * sizeof (mlib_d64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);
		da = adr_dst;
		d_d = double_dither[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
		dith_8 = d_d[8];
		dith_9 = d_d[9];
		dith_10 = d_d[10];
		dith_11 = d_d[11];
		dith_12 = d_d[12];
		dith_13 = d_d[13];
		dith_14 = d_d[14];
		dith_15 = d_d[15];
		d1 = vis_ld_d64_nf(sa); sa++;

#pragma pipeloop(0)
		for (i = 0; i <= sw - 8; i += 8) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d6 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d7 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d8 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d9 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d10 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d7, dith_8, dith_9);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d8, dith_10,
			    dith_11);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d9, dith_12,
			    dith_13);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d10, dith_14,
			    dith_15);
		}

		if (i <= sw - 4) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = *sa++;
			d4 = vis_faligndata(d2, d1);
			d2 = *sa++;
			d5 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d6 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d5, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d6, dith_6, dith_7);
			i += 4;
			dith_0 = dith_8;
			dith_1 = dith_9;
			dith_2 = dith_10;
			dith_3 = dith_11;
			dith_4 = dith_12;
			dith_5 = dith_13;
		}

		if (i <= sw - 2) {
			d2 = *sa++;
			d3 = vis_faligndata(d1, d2);
			d1 = vis_ld_d64_nf(sa); sa++;
			d4 = vis_faligndata(d2, d1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_2, dith_3);
			i += 2;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d2 = vis_ld_d64_nf(sa);
			d3 = vis_faligndata(d1, d2);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_S16_4((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
