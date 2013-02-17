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

#pragma ident	"@(#)mlib_i_ImageCopySubimage.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopySubimage      - Copy a sub-image of a larger image
 *                                    into a sub-region of another image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopySubimage(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         mlib_s32         xd,
 *                                         mlib_s32         yd,
 *                                         mlib_s32         xs,
 *                                         mlib_s32         ys,
 *                                         mlib_s32         w,
 *                                         mlib_s32         h);
 *
 * ARGUMENT
 *    dst     pointer to output or destination image
 *    src     pointer to input or source image
 *    xd      x coordinate of origin in the destination
 *    yd      y coordinate of origin in the destination
 *    xs      x coordinate of origin in the source
 *    ys      y coordinate of origin in the source
 *    w       width of the sub-image
 *    h       height of the sub-image
 *
 * RESTRICTION
 *    src and dst must have the same type and number of channels.
 *    They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *    MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *    Copy a sub-image of a larger image into a sub-region of another
 *    image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopySubimage = __mlib_ImageCopySubimage

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopySubimage) mlib_ImageCopySubimage
    __attribute__((weak, alias("__mlib_ImageCopySubimage")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* do not perform the copying by mlib_s64 data type for _NO_LONGLONG */

#ifdef _NO_LONGLONG
typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* _NO_LONGLONG */

#define	TYPE_64BIT	mlib_s64
#endif /* _NO_LONGLONG */

/* *********************************************************** */

static void mlib_c_ImageCopySubimage_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h);

static void mlib_c_ImageCopySubimage_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h);

static void mlib_c_ImageCopySubimage_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h);

static void mlib_c_ImageCopySubimage_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h);

/* *********************************************************** */

mlib_status
__mlib_ImageCopySubimage(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 src_width, src_height, dst_width, dst_height;

/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* number of channels of src and dst */
	mlib_s32 channels;

/* stride in bytes in src */
	mlib_s32 stride;

/* stride in bytes in dst */
	mlib_s32 dstride;

/* bit offset of src */
	mlib_s32 s_offset;

/* bit offset of dst */
	mlib_s32 d_offset;
	mlib_s32 j, size, offset;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	src_width = mlib_ImageGetWidth(src);
	src_height = mlib_ImageGetHeight(src);
	dst_width = mlib_ImageGetWidth(dst);
	dst_height = mlib_ImageGetHeight(dst);

	if ((w <= 0) || (h <= 0) || ((xs + w) <= 0) || ((ys + h) <= 0) ||
	    (xs >= src_width) || (ys >= src_height)) {
		return (MLIB_OUTOFRANGE);
	}

	if (((xd + w) <= 0) || ((yd + h) <= 0) || (xd >= dst_width) ||
	    (yd >= dst_height)) {
		return (MLIB_SUCCESS);
	} else {
		if (xs < 0) {
			w += xs;
			xd -= xs;
			xs = 0;
		}

		if (ys < 0) {
			h += ys;
			yd -= ys;
			ys = 0;
		}

		if (xd < 0) {
			w += xd;
			xs -= xd;
			xd = 0;
		}

		if (yd < 0) {
			h += yd;
			ys -= yd;
			yd = 0;
		}

		if ((xs + w) > src_width) {
			w = src_width - xs;

			if (w <= 0)
				return (MLIB_SUCCESS);
		}

		if ((ys + h) > src_height) {
			h = src_height - ys;

			if (h <= 0)
				return (MLIB_SUCCESS);
		}

		if ((xd + w) > dst_width) {
			w = dst_width - xd;

			if (w <= 0)
				return (MLIB_SUCCESS);
		}

		if ((yd + h) > dst_height) {
			h = dst_height - yd;

			if (h <= 0)
				return (MLIB_SUCCESS);
		}
	}

	switch (mlib_ImageGetType(dst)) {
	case MLIB_BIT:
/* in byte */
		stride = mlib_ImageGetStride(src);
/* in byte */
		dstride = mlib_ImageGetStride(dst);
/* in bits */
		s_offset = mlib_ImageGetBitOffset(src);
/* in bits */
		d_offset = mlib_ImageGetBitOffset(dst);
		channels = mlib_ImageGetChannels(dst);
		offset = xs * channels + s_offset;
		sa = (mlib_u8 *)mlib_ImageGetData(src) + ys * stride +
		    (offset >> 3);
		s_offset = offset & 7;
		offset = xd * channels + d_offset;
		da = (mlib_u8 *)mlib_ImageGetData(dst) + yd * dstride +
		    (offset >> 3);
		d_offset = offset & 7;

		size = w * channels;
/* row loop */

		if (s_offset == d_offset) {
			for (j = 0; j < h; j++) {
				mlib_ImageCopy_bit_al(sa, da, size, s_offset);
				sa += stride;
				da += dstride;
			}
		} else {
			for (j = 0; j < h; j++) {
				mlib_ImageCopy_bit_na(sa, da, size, s_offset,
				    d_offset);
				sa += stride;
				da += dstride;
			}
		}

		return (MLIB_SUCCESS);
	case MLIB_BYTE:
		mlib_c_ImageCopySubimage_u8(dst, src, xs, ys, xd, yd, w, h);
		return (MLIB_SUCCESS);
	case MLIB_SHORT:
	case MLIB_USHORT:
		mlib_c_ImageCopySubimage_s16(dst, src, xs, ys, xd, yd, w, h);
		return (MLIB_SUCCESS);
	case MLIB_INT:
	case MLIB_FLOAT:
		mlib_c_ImageCopySubimage_s32(dst, src, xs, ys, xd, yd, w, h);
		return (MLIB_SUCCESS);
	case MLIB_DOUBLE:
		mlib_c_ImageCopySubimage_d64(dst, src, xs, ys, xd, yd, w, h);
		return (MLIB_SUCCESS);
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

#define	PREPAREVARS(type)                                                 \
	mlib_s32 src_stride = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                                      \
		dst_stride = mlib_ImageGetStride(dst) / sizeof (type);    \
	    mlib_s32                                                      \
		chan = mlib_ImageGetChannels(dst);                        \
	    type *                                                        \
		psrc =                                                    \
		(type *) mlib_ImageGetData(src) + ys * src_stride +       \
	    xs * chan, *pdst =                                            \
	    (type *) mlib_ImageGetData(dst) + yd * dst_stride +           \
	    xd * chan;                                                    \
	    mlib_s32                                                      \
		i,                                                        \
		j;                                                        \
	                                                                  \
	w *= chan;                                                        \
	if (w == src_stride && w == dst_stride)                           \
	{                                                                 \
	w *= h;                                                           \
	h = 1;                                                            \
	}

/* *********************************************************** */

#define	STRIP(pd, ps, w, h, data_type)                          \
	{                                                       \
	    data_type s0, s1;                                   \
	                                                        \
	    for (i = 0; i < h; i++) {                           \
		if (j = w & 1)                                  \
		    pd[i * dst_stride] = ps[i * src_stride];    \
		for (; j < w; j += 2) {                         \
		    s0 = ps[i * src_stride + j];                \
		    s1 = ps[i * src_stride + j + 1];            \
		    pd[i * dst_stride + j] = s0;                \
		    pd[i * dst_stride + j + 1] = s1;            \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

void
mlib_c_ImageCopySubimage_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h)
{
	PREPAREVARS(mlib_u8);

	if (w < 16) {
		STRIP(pdst, psrc, w, h, mlib_u8);

		return;
	}

	for (i = 0; i < h; i++) {
		mlib_u8 *psrc_row = psrc + i * src_stride;
		mlib_u8 *pdst_row = pdst + i * dst_stride;

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			for (j = 0;
			    j < (mlib_s32)((8 - (mlib_addr)psrc_row) & 7);
			    j++) {
				pdst_row[j] = psrc_row[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 16); j += 16) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (psrc_row + j)), dsrc1 =
				    *((TYPE_64BIT *) (psrc_row + j + 8));

				*((TYPE_64BIT *) (pdst_row + j)) = dsrc0;
				*((TYPE_64BIT *) (pdst_row + j + 8)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			for (j = 0;
			    j < (mlib_s32)((4 - (mlib_addr)pdst_row) & 3);
			    j++) {
				pdst_row[j] = psrc_row[j];
			}

			if (!(((mlib_addr)psrc_row ^
				(mlib_addr)pdst_row) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 4); j += 4) {
					*((mlib_s32 *)(pdst_row + j)) =
					    *((mlib_s32 *)(psrc_row + j));
				}
			} else {
				mlib_u32 *ps, shl, shr, src0, src1;

				ps = (mlib_u32 *)(psrc_row + j);
				shl = (mlib_addr)ps & 3;
				ps = (mlib_u32 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 32 - shl;

				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 4); j += 4) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s32 *)(pdst_row + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(pdst_row + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#else /* _NO_LONGLONG */

			for (j = 0;
			    j < (mlib_s32)((8 - (mlib_addr)pdst_row) & 7);
			    j++) {
				pdst_row[j] = psrc_row[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(psrc_row + j);
				shl = (mlib_addr)ps & 7;
				ps = (mlib_u64 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 64 - shl;

				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 8); j += 8) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			pdst_row[j] = psrc_row[j];
	}
}

/* *********************************************************** */

void
mlib_c_ImageCopySubimage_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h)
{
	PREPAREVARS(mlib_u16);

	if (w < 8) {
		STRIP(pdst, psrc, w, h, mlib_u16);

		return;
	}

	for (i = 0; i < h; i++) {
		mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			for (j = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)psrc_row) & 7) >> 1);
			    j++) {
				pdst_row[j] = psrc_row[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 8); j += 8) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (psrc_row + j)), dsrc1 =
				    *((TYPE_64BIT *) (psrc_row + j + 4));

				*((TYPE_64BIT *) (pdst_row + j)) = dsrc0;
				*((TYPE_64BIT *) (pdst_row + j + 4)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			if (j = (((mlib_addr)pdst_row & 2) != 0)) {
				pdst_row[0] = psrc_row[0];
			}

			if (!(((mlib_addr)psrc_row ^
				(mlib_addr)pdst_row) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 2); j += 2) {
					*((mlib_s32 *)(pdst_row + j)) =
					    *((mlib_s32 *)(psrc_row + j));
				}
			} else {
				mlib_u32 *ps, src0, src1;

				ps = (mlib_u32 *)(psrc_row + j - 1);
				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 2); j += 2) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s32 *)(pdst_row + j)) =
					    (src0 >> 16) | (src1 << 16);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(pdst_row + j)) =
					    (src0 << 16) | (src1 >> 16);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#else /* _NO_LONGLONG */

			for (j = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)pdst_row) & 7) >> 1);
			    j++) {
				pdst_row[j] = psrc_row[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(psrc_row + j);
				shl = (mlib_addr)ps & 7;
				ps = (mlib_u64 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 64 - shl;

				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 4); j += 4) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			pdst_row[j] = psrc_row[j];
	}
}

/* *********************************************************** */

void
mlib_c_ImageCopySubimage_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h)
{
	PREPAREVARS(mlib_u32);

	if (w < 8) {
		STRIP(pdst, psrc, w, h, mlib_u32);

		return;
	}

	for (i = 0; i < h; i++) {
		mlib_u32 *psrc_row = psrc + i * src_stride;
		mlib_u32 *pdst_row = pdst + i * dst_stride;

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			if (j = (((mlib_addr)psrc_row & 4) >> 2)) {
				pdst_row[0] = psrc_row[0];
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 8); j += 8) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (psrc_row + j)), dsrc1 =
				    *((TYPE_64BIT *) (psrc_row + j + 2)),
				    dsrc2 =
				    *((TYPE_64BIT *) (psrc_row + j + 4)),
				    dsrc3 =
				    *((TYPE_64BIT *) (psrc_row + j + 6));

				*((TYPE_64BIT *) (pdst_row + j)) = dsrc0;
				*((TYPE_64BIT *) (pdst_row + j + 2)) = dsrc1;
				*((TYPE_64BIT *) (pdst_row + j + 4)) = dsrc2;
				*((TYPE_64BIT *) (pdst_row + j + 6)) = dsrc3;
			}
		} else {

#ifdef _NO_LONGLONG
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 2); j += 2) {
				*((mlib_s32 *)(pdst_row + j)) =
				    *((mlib_s32 *)(psrc_row + j));
				*((mlib_s32 *)(pdst_row + j + 1)) =
				    *((mlib_s32 *)(psrc_row + j + 1));
			}

#else /* _NO_LONGLONG */

			{
				mlib_u64 *ps, src0, src1, src2;

				if (j = (((mlib_addr)pdst_row & 4) >> 2))
					pdst_row[0] = psrc_row[0];
				ps = (mlib_u64 *)((mlib_addr)(psrc_row + j) -
				    4);

				src2 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 4); j += 4) {
					src0 = src2;
					src1 = ps[1];
					src2 = ps[2];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 >> 32) | (src1 << 32);
					*((mlib_s64 *)(pdst_row + j) + 1) =
					    (src1 >> 32) | (src2 << 32);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(pdst_row + j)) =
					    (src0 << 32) | (src1 >> 32);
					*((mlib_s64 *)(pdst_row + j) + 1) =
					    (src1 << 32) | (src2 >> 32);
#endif /* _LITTLE_ENDIAN */
					ps += 2;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			pdst_row[j] = psrc_row[j];
	}
}

/* *********************************************************** */

#ifdef _NO_LONGLONG
void
mlib_c_ImageCopySubimage_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h)
{
	PREPAREVARS(mlib_d64);

	for (i = 0; i < h; i++) {
		mlib_d64 *psrc_row = psrc + i * src_stride;
		mlib_d64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (w - 4); j += 4) {
			*((mlib_d64 *)(pdst_row + j)) =
			    *((mlib_d64 *)(psrc_row + j));
			*((mlib_d64 *)(pdst_row + j + 1)) =
			    *((mlib_d64 *)(psrc_row + j + 1));
			*((mlib_d64 *)(pdst_row + j + 2)) =
			    *((mlib_d64 *)(psrc_row + j + 2));
			*((mlib_d64 *)(pdst_row + j + 3)) =
			    *((mlib_d64 *)(psrc_row + j + 3));
		}

		for (; j < w; j++)
			pdst_row[j] = psrc_row[j];
	}
}
#else /* _NO_LONGLONG */
void
mlib_c_ImageCopySubimage_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 w,
    mlib_s32 h)
{
	PREPAREVARS(mlib_s64);

	for (i = 0; i < h; i++) {
		mlib_s64 *psrc_row = psrc + i * src_stride;
		mlib_s64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (w - 4); j += 4) {
			*((mlib_s64 *)(pdst_row + j)) =
			    *((mlib_s64 *)(psrc_row + j));
			*((mlib_s64 *)(pdst_row + j + 1)) =
			    *((mlib_s64 *)(psrc_row + j + 1));
			*((mlib_s64 *)(pdst_row + j + 2)) =
			    *((mlib_s64 *)(psrc_row + j + 2));
			*((mlib_s64 *)(pdst_row + j + 3)) =
			    *((mlib_s64 *)(psrc_row + j + 3));
		}

		for (; j < w; j++)
			pdst_row[j] = psrc_row[j];
	}
}

#endif
/* *********************************************************** */
