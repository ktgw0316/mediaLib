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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_i_ImageCopyArea.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopyArea    - Copy a specified rectangular area from
 *                              one portion of the image to another.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopyArea(mlib_image *img,
 *                                     mlib_s32   x,
 *                                     mlib_s32   y,
 *                                     mlib_s32   w,
 *                                     mlib_s32   h,
 *                                     mlib_s32   dx,
 *                                     mlib_s32   dy);
 *
 * ARGUMENT
 *      img     pointer to an image
 *      x       x coordinate of origin in the source
 *      y       y coordinate of origin in the source
 *      w       width of the area to be copied
 *      h       height of the area to be copied
 *      dx      horizontal displacement
 *      dy      vertical displacement
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Copy a specified rectangular area from one portion of the image
 *      to another (within the same image).
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopyArea = __mlib_ImageCopyArea

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopyArea) mlib_ImageCopyArea
    __attribute__((weak, alias("__mlib_ImageCopyArea")));

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

static void mlib_ImageCopyArea_bit_al_r(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset);

static void mlib_c_ImageCopyArea_u8(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_u8_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_s16(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_s16_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_s32(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_s32_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_d64(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_c_ImageCopyArea_d64_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

/* *********************************************************** */

mlib_status
__mlib_ImageCopyArea(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* width of the image */
	mlib_s32 width;

/* height of the image */
	mlib_s32 height;

/* start point in source */
	mlib_u8 *sa, *sp;

/* start points in destination */
	mlib_u8 *da;

/* number of channels of src and dst */
	mlib_s32 channels;

/* stride in bytes in src */
	mlib_s32 stride;

/* bit offset of src */
	mlib_s32 s_offset;

/* bit offset of dst */
	mlib_s32 d_offset;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size, offset;

	MLIB_IMAGE_CHECK(img);

	width = mlib_ImageGetWidth(img);
	height = mlib_ImageGetHeight(img);

/* clip the src and dst with respect to the image */

	if ((w == 0) || (h == 0) || ((x + w) <= 0) || ((y + h) <= 0) ||
	    (x >= width) || (y >= height) || ((x + dx + w) <= 0) ||
	    ((y + dy + h) <= 0) || ((x + dx) >= width) ||
	    ((y + dy) >= height) || (dx >= width) ||
	    (dy >= height) || (dx <= -width) ||
	    (dy <= -height)) {
		return (MLIB_OUTOFRANGE);
	} else {
/* clip WRT src first */
		if (x < 0) {
			w += x;
			x = 0;
		}

		if (y < 0) {
			h += y;
			y = 0;
		}

		if ((x + w) > width) {
			w = width - x;
		}

		if ((y + h) > height) {
			h = height - y;
		}

/* then clip WRT dst */
		if ((x + dx) < 0) {
			w += (x + dx);
			x = -dx;
		}

		if ((y + dy) < 0) {
			h += (y + dy);
			y = -dy;
		}

		if ((x + dx + w) > width) {
			w = width - (x + dx);
		}

		if ((y + dy + h) > height) {
			h = height - (y + dy);
		}
	}

	if ((dx == 0) && (dy == 0)) {
		return (MLIB_SUCCESS);
	} else {
		switch (mlib_ImageGetType(img)) {
		case MLIB_BIT:
			channels = mlib_ImageGetChannels(img);
/* in byte */
			stride = mlib_ImageGetStride(img);
/* in bits */
			s_offset = mlib_ImageGetBitOffset(img);
			size = w * channels;
			sp = (mlib_u8 *)mlib_ImageGetData(img);

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				offset = (x + w) * channels + s_offset - 1;
				sa = sp + (y + h - 1) * stride + (offset >> 3);
				s_offset = offset & 7;
				offset += dx * channels;
				da = sp + (y + h + dy - 1) * stride +
				    (offset >> 3);
				d_offset = offset & 7;

				if (s_offset == d_offset) {
					for (j = 0; j < h; j++) {
						mlib_ImageCopyArea_bit_al_r(sa,
						    da, size, s_offset + 1);
						sa -= stride;
						da -= stride;
					}
				} else {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_na_r(sa, da,
						    size, s_offset + 1,
						    d_offset + 1);
						sa -= stride;
						da -= stride;
					}
				}
			} else {
				offset = x * channels + s_offset;
				sa = sp + y * stride + (offset >> 3);
				s_offset = offset & 7;
				offset += dx * channels;
				da = sp + (y + dy) * stride + (offset >> 3);
				d_offset = offset & 7;

				if (s_offset == d_offset) {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_al(sa, da,
						    size, s_offset);
						sa += stride;
						da += stride;
					}
				} else {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_na(sa, da,
						    size, s_offset, d_offset);
						sa += stride;
						da += stride;
					}
				}
			}

			break;
		case MLIB_BYTE:

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				mlib_c_ImageCopyArea_u8_r(img, x, y, w, h, dx,
				    dy);
			} else {
				mlib_c_ImageCopyArea_u8(img, x, y, w, h, dx,
				    dy);
			}

			break;

		case MLIB_SHORT:
		case MLIB_USHORT:

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				mlib_c_ImageCopyArea_s16_r(img, x, y, w, h, dx,
				    dy);
			} else {
				mlib_c_ImageCopyArea_s16(img, x, y, w, h, dx,
				    dy);
			}

			break;

		case MLIB_INT:
		case MLIB_FLOAT:

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				mlib_c_ImageCopyArea_s32_r(img, x, y, w, h, dx,
				    dy);
			} else {
				mlib_c_ImageCopyArea_s32(img, x, y, w, h, dx,
				    dy);
			}

			break;

		case MLIB_DOUBLE:

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				mlib_c_ImageCopyArea_d64_r(img, x, y, w, h, dx,
				    dy);
			} else {
				mlib_c_ImageCopyArea_d64(img, x, y, w, h, dx,
				    dy);
			}

			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Both bit offsets of source and destination are the same
 * Copy is performed from end to beginning (reverse)
 * This function is for C version only (NO_LONGLONG case)
 */

void
mlib_ImageCopyArea_bit_al_r(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 offset)
{
/* 8-byte aligned start points in dst */
	TYPE_64BIT *dp;

/* 8-byte aligned start point in src */
	TYPE_64BIT *sp;

/* offset of address in dst */
	mlib_s32 i, j;

/* edge mask */
	mlib_s32 b_size;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 src, mask;

	if (size <= 0)
		return;

	if (size <= offset) {
		mask = mask0 << (8 - size);
		mask >>= (offset - size);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
		return;
	}

	mask = mask0 << (8 - offset);
	src = da[0];
	da[0] = (src & (~mask)) | (sa[0] & mask);
	da--;
	sa--;
	size = size - offset;
	b_size = size >> 3;
/* j = size & 7; */

	for (j = 0; (j < b_size) && (((mlib_addr)da & 7) != 7); j++)
		*da-- = *sa--;

	if ((((mlib_addr)sa & 7) ^ ((mlib_addr)da & 7)) == 0) {
		sp = (TYPE_64BIT *) (sa - 7);
		dp = (TYPE_64BIT *) (da - 7);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (b_size - 8); j += 8) {
			*dp-- = *sp--;
		}

		sa = (mlib_u8 *)sp + 7;
		da = (mlib_u8 *)dp + 7;
	} else {
#ifdef _NO_LONGLONG

		if ((((mlib_addr)sa & 3) ^ ((mlib_addr)da & 3)) == 0) {
			mlib_u32 *pws, *pwd;

			pws = (mlib_u32 *)(sa - 3);
			pwd = (mlib_u32 *)(da - 3);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (b_size - 4); j += 4) {
				*pwd-- = *pws--;
			}

			sa = (mlib_u8 *)pws + 3;
			da = (mlib_u8 *)pwd + 3;
		} else {
			mlib_u32 *pws, *pwd, src0, src1;
			mlib_s32 lshift = (mlib_addr)(sa + 1) & 3, rshift;

			pwd = (mlib_u32 *)(da - 3);
			pws = (mlib_u32 *)((mlib_addr)sa & ~3);
			lshift <<= 3;
			rshift = 32 - lshift;

			src1 = *pws--;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; j <= (b_size - 4); j += 4, i++) {
				src0 = src1;
				src1 = *pws--;
#ifdef _LITTLE_ENDIAN
				*pwd-- = (src1 >> lshift) | (src0 << rshift);
#else /* _LITTLE_ENDIAN */
				*pwd-- = (src1 << lshift) | (src0 >> rshift);
#endif /* _LITTLE_ENDIAN */
			}

			sa -= i << 2;
			da -= i << 2;
		}

#else /* _NO_LONGLONG */
		mlib_u64 *pws, *pwd, src0, src1;
		mlib_s32 lshift = (mlib_addr)(sa + 1) & 7, rshift;

		pwd = (mlib_u64 *)(da - 7);
		pws = (mlib_u64 *)((mlib_addr)sa & ~7);
		lshift <<= 3;
		rshift = 64 - lshift;

		src1 = *pws--;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; j <= (b_size - 8); j += 8, i++) {
			src0 = src1;
			src1 = *pws--;
			*pwd-- = (src1 << lshift) | (src0 >> rshift);
		}

		sa -= i << 3;
		da -= i << 3;
#endif /* _NO_LONGLONG */
	}

	for (; j < b_size; j++)
		*da-- = *sa--;

	j = size & 7;

	if (j > 0) {
		mask = mask0 >> (8 - j);
		src = da[0];
		da[0] = (src & (~mask)) | (sa[0] & mask);
	}
}

/* *********************************************************** */

#define	COPY(pd, ps, w, h, data_type)                           \
	{                                                       \
	    data_type s0, s1;                                   \
	                                                        \
	    for (i = 0; i < h; i++) {                           \
		if (j = w & 1)                                  \
		    pd[i * stride] = ps[i * stride];            \
		for (; j < w; j += 2) {                         \
		    s0 = ps[i * stride + j];                    \
		    s1 = ps[i * stride + j + 1];                \
		    pd[i * stride + j] = s0;                    \
		    pd[i * stride + j + 1] = s1;                \
		}                                               \
	    }                                                   \
	    return;                                             \
	}

/* *********************************************************** */

#define	COPY_R(pd, ps, w, h, data_type)                         \
	{                                                       \
	    data_type s0, s1;                                   \
	                                                        \
	    for (i = 0; i < h; i++) {                           \
		for (j = w - 1; j > 0; j -= 2) {                \
		    s0 = ps[-i * stride + j];                   \
		    s1 = ps[-i * stride + j - 1];               \
		    pd[-i * stride + j] = s0;                   \
		    pd[-i * stride + j - 1] = s1;               \
		}                                               \
		if (j == 0)                                     \
		    pd[-i * stride] = ps[-i * stride];          \
	    }                                                   \
	    return;                                             \
	}

/* *********************************************************** */

/*
 *      data are copied beginning from the start and proceeding
 *      to the end of the area.
 */

void
mlib_c_ImageCopyArea_u8(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u8 *sp;

/* pointer for pixel in dst */
	mlib_u8 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in bytes */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img);
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u8 *)mlib_ImageGetData(img) + y * stride + x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 16)
		COPY(dp, sp, w, h, mlib_u8);

	for (i = 0; i < h; i++) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			for (j = 0; j < (mlib_s32)((8 - (mlib_addr)sp) & 7);
			    j++) {
				dp[j] = sp[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 16); j += 16) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j + 8));

				*((TYPE_64BIT *) (dp + j)) = dsrc0;
				*((TYPE_64BIT *) (dp + j + 8)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			for (j = 0; j < (mlib_s32)((4 - (mlib_addr)dp) & 3);
			    j++) {
				dp[j] = sp[j];
			}

			if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 4); j += 4) {
					*((mlib_s32 *)(dp + j)) =
					    *((mlib_s32 *)(sp + j));
				}
			} else {
				mlib_u32 *ps, shl, shr, src0, src1;

				ps = (mlib_u32 *)(sp + j);
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
					*((mlib_s32 *)(dp + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(dp + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#else /* _NO_LONGLONG */

			for (j = 0; j < (mlib_s32)((8 - (mlib_addr)dp) & 7);
			    j++) {
				dp[j] = sp[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(sp + j);
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
					*((mlib_s64 *)(dp + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			dp[j] = sp[j];
		sp += stride;
		dp += stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_c_ImageCopyArea_u8_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u8 *sp;

/* pointer for pixel in dst */
	mlib_u8 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in bytes */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img);
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u8 *)mlib_ImageGetData(img) + (y + h - 1) * stride +
	    x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 16)
		COPY_R(dp, sp, w, h, mlib_u8);

	for (i = h - 1; i >= 0; i--) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			for (j = w - 1; ((mlib_addr)(sp + j) & 7) != 7; j--) {
				dp[j] = sp[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j >= 15; j -= 16) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j - 7)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j - 15));

				*((TYPE_64BIT *) (dp + j - 7)) = dsrc0;
				*((TYPE_64BIT *) (dp + j - 15)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			for (j = w - 1; ((mlib_addr)(dp + j) & 3) != 3; j--) {
				dp[j] = sp[j];
			}

			if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 3; j -= 4) {
					*((mlib_s32 *)(dp + j - 3)) =
					    *((mlib_s32 *)(sp + j - 3));
				}
			} else {
				mlib_u32 *ps, shl, shr, src0, src1;

				ps = (mlib_u32 *)(sp + j - 3);
				shl = (mlib_addr)ps & 3;
				ps = (mlib_u32 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 32 - shl;

				src0 = ps[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 3; j -= 4) {
					src1 = src0;
					src0 = ps[0];
#ifdef _LITTLE_ENDIAN
					*((mlib_s32 *)(dp + j - 3)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(dp + j - 3)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps--;
				}
			}

#else /* _NO_LONGLONG */

			for (j = w - 1; ((mlib_addr)(dp + j) & 7) != 7; j--) {
				dp[j] = sp[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(sp + j - 7);
				shl = (mlib_addr)ps & 7;
				ps = (mlib_u64 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 64 - shl;

				src0 = ps[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 7; j -= 8) {
					src1 = src0;
					src0 = ps[0];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(dp + j - 7)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j - 7)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps--;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j >= 0; j--)
			dp[j] = sp[j];
		sp -= stride;
		dp -= stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the start and proceeding
 *      to the end of the area.
 */

void
mlib_c_ImageCopyArea_s16(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u16 *sp;

/* pointer for pixel in dst */
	mlib_u16 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) / 2;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u16 *)mlib_ImageGetData(img) + y * stride + x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY(dp, sp, w, h, mlib_u16);

	for (i = 0; i < h; i++) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			for (j = 0;
			    j < (mlib_s32)(((8 - (mlib_addr)sp) & 7) >> 1);
			    j++) {
				dp[j] = sp[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 8); j += 8) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j + 4));

				*((TYPE_64BIT *) (dp + j)) = dsrc0;
				*((TYPE_64BIT *) (dp + j + 4)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			if (j = (((mlib_addr)dp & 2) != 0)) {
				dp[0] = sp[0];
			}

			if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 2); j += 2) {
					*((mlib_s32 *)(dp + j)) =
					    *((mlib_s32 *)(sp + j));
				}
			} else {
				mlib_u32 *ps, src0, src1;

				ps = (mlib_u32 *)(sp + j - 1);
				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 2); j += 2) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s32 *)(dp + j)) =
					    (src0 >> 16) | (src1 << 16);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(dp + j)) =
					    (src0 << 16) | (src1 >> 16);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#else /* _NO_LONGLONG */

			for (j = 0;
			    j < (mlib_s32)(((8 - (mlib_addr)dp) & 7) >> 1);
			    j++) {
				dp[j] = sp[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(sp + j);
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
					*((mlib_s64 *)(dp + j)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			dp[j] = sp[j];
		sp += stride;
		dp += stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_c_ImageCopyArea_s16_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u16 *sp;

/* pointer for pixel in dst */
	mlib_u16 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) / 2;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u16 *)mlib_ImageGetData(img) + (y + h - 1) * stride +
	    x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY_R(dp, sp, w, h, mlib_u16);

	for (i = h - 1; i >= 0; i--) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			for (j = w - 1; ((mlib_addr)(sp + j) & 7) != 6; j--) {
				dp[j] = sp[j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j >= 7; j -= 8) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j - 3)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j - 7));

				*((TYPE_64BIT *) (dp + j - 3)) = dsrc0;
				*((TYPE_64BIT *) (dp + j - 7)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG

			j = w - 1;

			if (((mlib_addr)(dp + j) & 2) == 0) {
				dp[j] = sp[j];
				j--;
			}

			if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 1; j -= 2) {
					*((mlib_s32 *)(dp + j - 1)) =
					    *((mlib_s32 *)(sp + j - 1));
				}
			} else {
				mlib_u32 *ps, src0, src1;

				ps = (mlib_u32 *)(sp + j - 2);
				src0 = ps[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 1; j -= 2) {
					src1 = src0;
					src0 = ps[0];
#ifdef _LITTLE_ENDIAN
					*((mlib_s32 *)(dp + j - 1)) =
					    (src0 >> 16) | (src1 << 16);
#else /* _LITTLE_ENDIAN */
					*((mlib_s32 *)(dp + j - 1)) =
					    (src0 << 16) | (src1 >> 16);
#endif /* _LITTLE_ENDIAN */
					ps--;
				}
			}

#else /* _NO_LONGLONG */

			for (j = w - 1; (((mlib_addr)(dp + j) & 6) != 6); j--) {
				dp[j] = sp[j];
			}

			{
				mlib_s32 shl, shr;
				mlib_u64 *ps, src0, src1;

				ps = (mlib_u64 *)(sp + j - 3);
				shl = (mlib_addr)ps & 7;
				ps = (mlib_u64 *)((mlib_addr)ps - shl);
				shl <<= 3;
				shr = 64 - shl;

				src0 = ps[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 3; j -= 4) {
					src1 = src0;
					src0 = ps[0];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(dp + j - 3)) =
					    (src0 >> shl) | (src1 << shr);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j - 3)) =
					    (src0 << shl) | (src1 >> shr);
#endif /* _LITTLE_ENDIAN */
					ps--;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j >= 0; j--)
			dp[j] = sp[j];
		sp -= stride;
		dp -= stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the start and proceeding
 *      to the end of the area.
 */

void
mlib_c_ImageCopyArea_s32(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u32 *sp;

/* pointer for pixel in dst */
	mlib_u32 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) / 4;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u32 *)mlib_ImageGetData(img) + y * stride + x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY(dp, sp, w, h, mlib_u32);

	for (i = 0; i < h; i++) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			if (j = (((mlib_addr)sp & 4) >> 2)) {
				dp[0] = sp[0];
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (w - 4); j += 4) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j + 2));

				*((TYPE_64BIT *) (dp + j)) = dsrc0;
				*((TYPE_64BIT *) (dp + j + 2)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 1); j++) {
				*((mlib_s32 *)(dp + j)) =
				    *((mlib_s32 *)(sp + j));
			}

#else /* _NO_LONGLONG */

			{
				mlib_u64 *ps, src0, src1;

				if (j = (((mlib_addr)dp & 4) >> 2))
					dp[0] = sp[0];
				ps = (mlib_u64 *)((mlib_addr)(sp + j) - 4);
				src1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (w - 2); j += 2) {
					src0 = src1;
					src1 = ps[1];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(dp + j)) =
					    (src0 >> 32) | (src1 << 32);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j)) =
					    (src0 << 32) | (src1 >> 32);
#endif /* _LITTLE_ENDIAN */
					ps++;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j < w; j++)
			dp[j] = sp[j];
		sp += stride;
		dp += stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_c_ImageCopyArea_s32_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_u32 *sp;

/* pointer for pixel in dst */
	mlib_u32 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) / 4;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_u32 *)mlib_ImageGetData(img) + (y + h - 1) * stride +
	    x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY_R(dp, sp, w, h, mlib_u32);

	for (i = h - 1; i >= 0; i--) {
		if (!(((mlib_addr)sp ^ (mlib_addr)dp) & 7)) {
			j = w - 1;

			if (!((mlib_addr)(sp + j) & 4)) {
				dp[j] = sp[j];
				j--;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j >= 3; j -= 4) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (sp + j - 1)), dsrc1 =
				    *((TYPE_64BIT *) (sp + j - 3));

				*((TYPE_64BIT *) (dp + j - 1)) = dsrc0;
				*((TYPE_64BIT *) (dp + j - 3)) = dsrc1;
			}
		} else {

#ifdef _NO_LONGLONG
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = w - 1; j >= 0; j--) {
				*((mlib_s32 *)(dp + j)) =
				    *((mlib_s32 *)(sp + j));
			}

#else /* _NO_LONGLONG */

			{
				mlib_u64 *ps, src0, src1;

				j = w - 1;

				if (!((mlib_addr)(dp + j) & 4)) {
					dp[j] = sp[j];
					j--;
				}

				ps = (mlib_u64 *)((mlib_addr)(sp + j) - 8);
				src0 = ps[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j >= 1; j -= 2) {
					src1 = src0;
					src0 = ps[0];
#ifdef _LITTLE_ENDIAN
					*((mlib_s64 *)(dp + j - 1)) =
					    (src0 >> 32) | (src1 << 32);
#else /* _LITTLE_ENDIAN */
					*((mlib_s64 *)(dp + j - 1)) =
					    (src0 << 32) | (src1 >> 32);
#endif /* _LITTLE_ENDIAN */
					ps--;
				}
			}

#endif /* _NO_LONGLONG */
		}

		for (; j >= 0; j--)
			dp[j] = sp[j];
		sp -= stride;
		dp -= stride;
	}
}


#ifdef _NO_LONGLONG

/* *********************************************************** */

/*
 *      data are copied beginning from the start and proceeding
 *      to the end of the area.
 */

void
mlib_c_ImageCopyArea_d64(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_d64 *sp;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) >> 3;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_d64 *)mlib_ImageGetData(img) + y * stride + x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY(dp, sp, w, h, mlib_d64);

	for (i = 0; i < h; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (w - 1); j++) {
			*((mlib_d64 *)(dp + j)) = *((mlib_d64 *)(sp + j));
		}

		sp += stride;
		dp += stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_c_ImageCopyArea_d64_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_d64 *sp;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) >> 3;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_d64 *)mlib_ImageGetData(img) + (y + h - 1) * stride +
	    x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY_R(dp, sp, w, h, mlib_d64);

	for (i = h - 1; i >= 0; i--) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = w - 1; j >= 0; j--) {
			*((mlib_d64 *)(dp + j)) = *((mlib_d64 *)(sp + j));
		}

		sp -= stride;
		dp -= stride;
	}
}

#else	/* _NO_LONGLONG */

/* *********************************************************** */

/*
 *      data are copied beginning from the start and proceeding
 *      to the end of the area.
 */

void
mlib_c_ImageCopyArea_d64(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_s64 *sp;

/* pointer for pixel in dst */
	mlib_s64 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) >> 3;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_s64 *)mlib_ImageGetData(img) + y * stride + x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY(dp, sp, w, h, mlib_s64);

	for (i = 0; i < h; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (w - 1); j++) {
			*((mlib_s64 *)(dp + j)) = *((mlib_s64 *)(sp + j));
		}

		sp += stride;
		dp += stride;
	}
}

/* *********************************************************** */

/*
 *      data are copied beginning from the end and proceeding
 *      to the start of the area.
 */

void
mlib_c_ImageCopyArea_d64_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* pointers for pixel in src */
	mlib_s64 *sp;

/* pointer for pixel in dst */
	mlib_s64 *dp;

/* number of channels of img */
	mlib_s32 channels;

/* stride of img in SHORT */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 i, j;

	stride = mlib_ImageGetStride(img) >> 3;
	channels = mlib_ImageGetChannels(img);

	sp = (mlib_s64 *)mlib_ImageGetData(img) + (y + h - 1) * stride +
	    x * channels;
	dp = sp + dy * stride + dx * channels;
	w *= channels;

	if (w < 8)
		COPY_R(dp, sp, w, h, mlib_s64);

	for (i = h - 1; i >= 0; i--) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = w - 1; j >= 0; j--) {
			*((mlib_s64 *)(dp + j)) = *((mlib_s64 *)(sp + j));
		}

		sp -= stride;
		dp -= stride;
	}
}

#endif

/* *********************************************************** */
