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

#pragma ident	"@(#)mlib_i_ImageClrOrdDither8x8.c	9.2	07/11/05 SMI"

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

static mlib_status mlib_c_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_c_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

#define	FUNC_NAME(F)	mlib_c_OrderedDither8x8_##F

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
						    FUNC_NAME(3_8_8)
						    (src, dst, dmask, scale,
						    colormap);
						break;
					case 4:
						stat =
						    FUNC_NAME(4_8_8)
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
 * convert wid from
 * pixels to bytes
 */
					switch (src_nch) {
					case 3:
						stat =
						    FUNC_NAME(3_8_16)
						    (src, dst, dmask, scale,
						    colormap);
						break;
					case 4:
						stat =
						    FUNC_NAME(4_8_16)
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
 * convert wid from pixels to
 * bytes
 */
				switch (dst_dtype) {
				case MLIB_BYTE:
					switch (src_nch) {
					case 3:
						stat =
						    FUNC_NAME(3_16_8)
						    (src, dst, dmask, scale,
						    colormap);
						break;
					case 4:
						stat =
						    FUNC_NAME(4_16_8)
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
 * convert wid from
 * pixels to bytes
 */
					switch (src_nch) {
					case 3:
						stat =
						    FUNC_NAME(3_16_16)
						    (src, dst, dmask, scale,
						    colormap);
						break;
					case 4:
						stat =
						    FUNC_NAME(4_16_16)
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
#if defined(_NO_LONGLONG)
/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS(src_format, dst_format)           \
	src_format *sa, *adr_src;                               \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	dst_format *da, *adr_dst;                               \
	                                                        \
/*                                                              \
 * pointers for pixel and line of destination                   \
 */                                                             \
	src_format *interm_buf, *intr_da;                       \
	mlib_s32 *d;                                            \
	                                                        \
/*                                                              \
 * pointer to row in diter array                                \
 */                                                             \
	mlib_s32 sll,                                           \
/*                                                              \
 * line length (in pixels) of source image                      \
 */                                                             \
	    dll,                                                \
/*                                                              \
 * line length (in pixels) of destination image                 \
 */                                                             \
	    sw,                                                 \
/*                                                              \
 * src width in pixels                                          \
 */                                                             \
	    sh;                                                 \
	                                                        \
/*                                                              \
 * src height in pixels                                         \
 */                                                             \
	mlib_s32 row, i, chan;                                  \
	                                                        \
/*                                                              \
 * indices                                                      \
 */                                                             \
	mlib_s32 d0, d1, d2, d3, d4, d5, d6, d7;                \
	                                                        \
/*                                                              \
 * dither koeff                                                 \
 */                                                             \
	mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;                \
	mlib_d64 mult;                                          \
	src_format dd0, dd1, dd2, dd3, dd4, dd5, dd6, dd7;      \
	mlib_s32 bits, shift

/* *********************************************************** */
#else
/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS(src_format, dst_format)           \
	src_format *sa, *adr_src;                               \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	dst_format *da, *adr_dst;                               \
	                                                        \
/*                                                              \
 * pointers for pixel and line of destination                   \
 */                                                             \
	src_format *interm_buf, *intr_da;                       \
	mlib_s32 *d;                                            \
	                                                        \
/*                                                              \
 * pointer to row in diter array                                \
 */                                                             \
	mlib_s32 sll,                                           \
/*                                                              \
 * line length (in pixels) of source image                      \
 */                                                             \
	    dll,                                                \
/*                                                              \
 * line length (in pixels) of destination image                 \
 */                                                             \
	    sw,                                                 \
/*                                                              \
 * src width in pixels                                          \
 */                                                             \
	    sh;                                                 \
	                                                        \
/*                                                              \
 * src height in pixels                                         \
 */                                                             \
	mlib_s32 row, i, chan;                                  \
	                                                        \
/*                                                              \
 * indices                                                      \
 */                                                             \
	mlib_s32 d0, d1, d2, d3, d4, d5, d6, d7;                \
	                                                        \
/*                                                              \
 * dither koeff                                                 \
 */                                                             \
	mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;                \
	mlib_s64 mult;                                          \
	src_format dd0, dd1, dd2, dd3, dd4, dd5, dd6, dd7;      \
	mlib_s32 bits, shift

/* *********************************************************** */
#endif
/* *********************************************************** */

#define	ADDITION_3_CHANNELS_IDENTIFICATORS(src_format)          \
	mlib_s32 dither[3][8][8]                                \
/*                                                              \
 * dither array                                                 \
 */                                                             \


/* *********************************************************** */

#define	ADDITION_4_CHANNELS_IDENTIFICATORS(src_format)          \
	mlib_s32 dither[4][8][8]                                \
/*                                                              \
 * dither array                                                 \
 */                                                             \


/* *********************************************************** */

#define	ADDITION_IDENTIFICATORS_U8()                             \
	mlib_s32 imask, mask1 = 0x000000FF, mask2 = 0x0000FF00

/* *********************************************************** */

#define	ADDITION_IDENTIFICATORS_S16()                           \
	mlib_u32 mask32[] = {                                   \
	    0x0000FFFF, 0x7FFF0000, 0x80000000, 0x0000FFFF      \
	}                                                       \
	                                                        \
	;                                                       \
	mlib_u32 mask;                                          \
	mlib_u8 *pmask = (mlib_u8 *)mask32

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(src_format, dst_format)          \
	sh = mlib_ImageGetHeight(src);                          \
	sw = mlib_ImageGetWidth(src);                           \
	sll = mlib_ImageGetStride(src) / sizeof (src_format);   \
	dll = mlib_ImageGetStride(dst) / sizeof (dst_format);   \
	sa = adr_src = (src_format *) mlib_ImageGetData(src);   \
	da = adr_dst = (dst_format *) mlib_ImageGetData(dst);   \
	bits = mlib_ImageGetBits(colormap)

/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */

#define	PREPARE_MULT_FACTOR_U8()                                \
	shift = (1 << (8 - bits));                              \
	mult = (mlib_d64)shift;                                 \
	while (scalef_expon > 30) {                             \
	    mult /= (1 << 30);                                  \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	mult /= (1 << scalef_expon)

/* *********************************************************** */

#define	PREPARE_MULT_FACTOR_S16()                               \
	shift = (1 << (16 - bits));                             \
	mult = (mlib_d64)shift;                                 \
	while (scalef_expon > 30) {                             \
	    mult /= (1 << 30);                                  \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	mult /= (1 << scalef_expon)

/* *********************************************************** */

#define	PREPARE_DITHER_ARRAY_FOR_3_CHANNELS()                       \
	for (row = 0; row < 8; row++) {                             \
	    for (i = 0; i < 8; i++) {                               \
		dither[0][row][i] =                                 \
		    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +   \
		    i] * mult);                                     \
	    }                                                       \
	}                                                           \
	                                                            \
	for (row = 0; row < 8; row++) {                             \
	    for (i = 0; i < 8; i++) {                               \
		dither[2][7 - row][i] = dither[1][row][7 - i] =     \
		    dither[0][row][i];                              \
	    }                                                       \
	}

/* *********************************************************** */

#define	PREPARE_DITHER_ARRAY_FOR_4_CHANNELS()                         \
	for (row = 0; row < 8; row++) {                               \
	    for (i = 0; i < 8; i++) {                                 \
		dither[0][row][i] =                                   \
		    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +     \
		    i] * mult);                                       \
	    }                                                         \
	}                                                             \
	                                                              \
	for (row = 0; row < 8; row++) {                               \
	    for (i = 0; i < 8; i++) {                                 \
		dither[3][7 - row][7 - i] = dither[2][7 - row][i] =   \
		    dither[1][row][7 - i] = dither[0][row][i];        \
	    }                                                         \
	}

/* *********************************************************** */
#else
/* *********************************************************** */

#define	PREPARE_MULT_FACTOR_U8()                                \
	shift = (1 << (8 - bits));                              \
	mult = 1;                                 \
	while (scalef_expon > 30) {                             \
	    mult *= (1 << 30);                                  \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	mult *= (1 << scalef_expon)

/* *********************************************************** */

#define	PREPARE_MULT_FACTOR_S16()                               \
	shift = (1 << (16 - bits));                             \
	mult = 1;                                 \
	while (scalef_expon > 30) {                             \
	    mult *= (1 << 30);                                  \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	mult *= (1 << scalef_expon)

/* *********************************************************** */

#define	PREPARE_DITHER_ARRAY_FOR_3_CHANNELS()                       \
	for (row = 0; row < 8; row++) {                             \
	    for (i = 0; i < 8; i++) {                               \
		dither[0][row][i] =                                 \
		    (shift >> 1) - 1 - (mlib_s32)((dmask[8 * row +   \
		    i] * shift) / mult);                                \
	    }                                                       \
	}                                                           \
	                                                            \
	for (row = 0; row < 8; row++) {                             \
	    for (i = 0; i < 8; i++) {                               \
		dither[2][7 - row][i] = dither[1][row][7 - i] =     \
		    dither[0][row][i];                              \
	    }                                                       \
	}

/* *********************************************************** */

#define	PREPARE_DITHER_ARRAY_FOR_4_CHANNELS()                         \
	for (row = 0; row < 8; row++) {                               \
	    for (i = 0; i < 8; i++) {                                 \
		dither[0][row][i] =                                   \
		    (shift >> 1) - 1 - (mlib_s32)((dmask[8 * row +     \
		    i] * shift) / mult);                                 \
	    }                                                         \
	}                                                             \
	                                                              \
	for (row = 0; row < 8; row++) {                               \
	    for (i = 0; i < 8; i++) {                                 \
		dither[3][7 - row][7 - i] = dither[2][7 - row][i] =   \
		    dither[1][row][7 - i] = dither[0][row][i];        \
	    }                                                         \
	}

/* *********************************************************** */
#endif
/* *********************************************************** */

#define	SAT_U8(sum, out)                                        \
	imask = (sum >> 5) & 0x18;                              \
	out = (sum & (mask1 >> imask)) | (mask2 >> imask)

/* *********************************************************** */

#define	SAT_S16(sum, out)                                        \
	mask = *((mlib_u32 *)(pmask + (((sum) >> 13) & 0xC)));   \
	out = (mlib_s16)(((sum) & mask) | (mask >> 16))

/* *********************************************************** */

#define	INDEX_LINE(F)	mlib_ImageColorTrue2IndexLine_##F

#define	ORDERED_DITHER_8x8(src_format, dst_format, src_type,           \
	dst_type, nchannels)                                           \
	{                                                              \
	    DEFINE_IDENTIFICATORS(src_format, dst_format);             \
	    ADDITION_##nchannels##_CHANNELS_IDENTIFICATORS             \
		(src_format);                                          \
	    ADDITION_IDENTIFICATORS_##src_type();                      \
	    GET_SRC_DST_PARAMETERS(src_format, dst_format);            \
	    PREPARE_MULT_FACTOR_##src_type();                          \
	    PREPARE_DITHER_ARRAY_FOR_##nchannels##_CHANNELS();         \
/*                                                                     \
 * alloc. memory for interm_row                                        \
 */                                                                    \
	    interm_buf =                                               \
		(src_format *) __mlib_malloc(sw * nchannels *          \
		sizeof (src_format));                                  \
	    if (interm_buf == NULL)                                    \
		return (MLIB_FAILURE);                                 \
	    for (row = 0; row < sh; row++) {                           \
		da = adr_dst;                                          \
		for (chan = 0; chan < nchannels; chan++) {             \
		    sa = adr_src + chan;                               \
		    intr_da = interm_buf + chan;                       \
		    d = dither[chan][row & 7];                         \
		    d0 = d[0];                                         \
		    d1 = d[1];                                         \
		    d2 = d[2];                                         \
		    d3 = d[3];                                         \
		    d4 = d[4];                                         \
		    d5 = d[5];                                         \
		    d6 = d[6];                                         \
		    d7 = d[7];                                         \
		    for (i = 0; i < sw - 8; i += 8) {                  \
			s0 = (mlib_s32)(*sa) + d0;                     \
			sa += nchannels;                               \
			SAT_##src_type(s0, dd0);                       \
			s1 = (mlib_s32)(*sa) + d1;                     \
			sa += nchannels;                               \
			SAT_##src_type(s1, dd1);                       \
			s2 = (mlib_s32)(*sa) + d2;                     \
			sa += nchannels;                               \
			SAT_##src_type(s2, dd2);                       \
			s3 = (mlib_s32)(*sa) + d3;                     \
			sa += nchannels;                               \
			SAT_##src_type(s3, dd3);                       \
			s4 = (mlib_s32)(*sa) + d4;                     \
			sa += nchannels;                               \
			SAT_##src_type(s4, dd4);                       \
			s5 = (mlib_s32)(*sa) + d5;                     \
			sa += nchannels;                               \
			SAT_##src_type(s5, dd5);                       \
			s6 = (mlib_s32)(*sa) + d6;                     \
			sa += nchannels;                               \
			SAT_##src_type(s6, dd6);                       \
			s7 = (mlib_s32)(*sa) + d7;                     \
			sa += nchannels;                               \
			SAT_##src_type(s7, dd7);                       \
			*intr_da = dd0;                                \
			intr_da += nchannels;                          \
			*intr_da = dd1;                                \
			intr_da += nchannels;                          \
			*intr_da = dd2;                                \
			intr_da += nchannels;                          \
			*intr_da = dd3;                                \
			intr_da += nchannels;                          \
			*intr_da = dd4;                                \
			intr_da += nchannels;                          \
			*intr_da = dd5;                                \
			intr_da += nchannels;                          \
			*intr_da = dd6;                                \
			intr_da += nchannels;                          \
			*intr_da = dd7;                                \
			intr_da += nchannels;                          \
		    }                                                  \
		    for (; i < sw; i++) {                              \
			d0 = d[i & 7];                                 \
			s0 = (mlib_s32)(*sa) + d0;                     \
			SAT_##src_type(s0, dd0);                       \
			*intr_da = dd0;                                \
			intr_da += nchannels;                          \
			sa += nchannels;                               \
		    }                                                  \
		}                                                      \
		INDEX_LINE(src_type##_##dst_type##_##nchannels)      \
		    (interm_buf, da, sw, colormap);                    \
		adr_src += sll;                                        \
		adr_dst += dll;                                        \
	    }                                                          \
	    __mlib_free(interm_buf);                                   \
	    return (MLIB_SUCCESS);                                     \
	}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_u8,
	    mlib_u8,
	    U8,
	    U8,
	    3)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_u8,
	    mlib_u8,
	    U8,
	    U8,
	    4)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_s16,
	    mlib_u8,
	    S16,
	    U8,
	    3)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_s16,
	    mlib_u8,
	    S16,
	    U8,
	    4)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_u8,
	    mlib_s16,
	    U8,
	    S16,
	    3)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_u8,
	    mlib_s16,
	    U8,
	    S16,
	    4)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_s16,
	    mlib_s16,
	    S16,
	    S16,
	    3)
}

/* *********************************************************** */

mlib_status
mlib_c_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	ORDERED_DITHER_8x8(mlib_s16,
	    mlib_s16,
	    S16,
	    S16,
	    4)
}

/* *********************************************************** */
