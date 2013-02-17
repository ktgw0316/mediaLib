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

#pragma ident	"@(#)mlib_v_ImageCopy.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageCopy   - Direct copy from one image to another.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopy(mlib_image       *dst,
 *                                 const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *
 * RESTRICTION
 *      src and dst must have the same size, type and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Direct copy from one image to another.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageCopy_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopy = __mlib_ImageCopy

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopy) mlib_ImageCopy
    __attribute__((weak, alias("__mlib_ImageCopy")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CASHSIZE	(4 * 1024 * 1024)

mlib_status
__mlib_ImageCopy(
    mlib_image *dst,
    const mlib_image *src)
{
/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* width in bytes of src and dst */
	mlib_s32 width;

/* height in lines of src and dst */
	mlib_s32 height;

/* bit offset of src */
	mlib_s32 s_offset;

/* bit offset of dst */
	mlib_s32 d_offset;

/* stride in bytes in src */
	mlib_s32 stride;

/* stride in bytes in dst */
	mlib_s32 dstride;

/* indices for x, y */
	mlib_s32 i, j;
	mlib_s32 size;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	width = mlib_ImageGetWidth(dst) * mlib_ImageGetChannels(dst);
	height = mlib_ImageGetHeight(dst);
	sa = (mlib_u8 *)mlib_ImageGetData(src);
	da = (mlib_u8 *)mlib_ImageGetData(dst);

	switch (mlib_ImageGetType(dst)) {
	case MLIB_BIT:

		if (!mlib_ImageIsNotOneDvector(src) &&
		    !mlib_ImageIsNotOneDvector(dst)) {
			size = height * (width >> 3);

			if (!mlib_ImageIsNotAligned8(src) &&
			    !mlib_ImageIsNotAligned8(dst) &&
			    ((size & 7) == 0)) {

				mlib_v_ImageCopy_a1((mlib_d64 *)sa,
				    (mlib_d64 *)da, size >> 3);
			} else {

				mlib_ImageCopy_na(sa, da, size);
			}
		} else {
/* in byte */
			stride = mlib_ImageGetStride(src);
/* in byte */
			dstride = mlib_ImageGetStride(dst);
/* in bits */
			s_offset = mlib_ImageGetBitOffset(src);
/* in bits */
			d_offset = mlib_ImageGetBitOffset(dst);

			if (s_offset == d_offset) {
				for (j = 0; j < height; j++) {
					mlib_ImageCopy_bit_al(sa, da, width,
					    s_offset);
					sa += stride;
					da += dstride;
				}
			} else {
				for (j = 0; j < height; j++) {
					mlib_ImageCopy_bit_na(sa, da, width,
					    s_offset, d_offset);
					sa += stride;
					da += dstride;
				}
			}
		}

		return (MLIB_SUCCESS);

	case MLIB_BYTE:
		break;
	case MLIB_SHORT:
	case MLIB_USHORT:
		width *= 2;
		break;
	case MLIB_INT:
	case MLIB_FLOAT:
		width *= 4;
		break;
	case MLIB_DOUBLE:
		width *= 8;
		break;
	default:
		return (MLIB_FAILURE);
	}

	if (!mlib_ImageIsNotOneDvector(src) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		size = height * width;

		if (size > CASHSIZE) {
			mlib_s32 tail = 0x40 - ((mlib_addr)da & 0x3F);

			mlib_ImageCopy_na(sa, da, tail & 0x3F);
			sa += tail & 0x3F;
			da += tail & 0x3F;
			size -= tail & 0x3F;
/* (size >> 6) should be > 1 */
			mlib_v_ImageCopy_blk(sa, da, size >> 6);
			sa += size & ~0x3F;
			da += size & ~0x3F;
			mlib_ImageCopy_na(sa, da, size & 0x3F);
			return (MLIB_SUCCESS);
		}

		if (!mlib_ImageIsNotAligned8(src) &&
		    !mlib_ImageIsNotAligned8(dst) && ((size & 7) == 0)) {
			mlib_v_ImageCopy_a1((mlib_d64 *)sa, (mlib_d64 *)da,
			    size >> 3);
		} else {
			mlib_ImageCopy_na(sa, da, size);
		}
	} else {
/* in byte */
		stride = mlib_ImageGetStride(src);
/* in byte */
		dstride = mlib_ImageGetStride(dst);

		for (i = 0; i < height; i++) {
			if (((mlib_addr)sa | (mlib_addr)da) & 7) {
/* end points in dst */
				mlib_u8 *dend;

/* 8-byte aligned start points in dst */
				mlib_d64 *dp;

/* 8-byte aligned start point in src */
				mlib_d64 *sp;

/* 8-byte source data */
				mlib_d64 s0, s1;

/* edge mask */
				mlib_s32 emask;

/* prepare the destination addresses */
				dp = (mlib_d64 *)((mlib_addr)da & (~7));
				j = (mlib_addr)dp - (mlib_addr)da;
				dend = da + width - 1;

/* prepare the source address */
				sp = (mlib_d64 *)vis_alignaddr(sa, j);
/* generate edge mask for the start point */
				emask = vis_edge8(da, dend);

				s1 = vis_ld_d64_nf(sp);

				if (emask != 0xff) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					s0 = vis_faligndata(s0, s1);
					vis_pst_8(s0, dp++, emask);
					sp++;
					j += 8;
				}
#pragma pipeloop(0)
				for (; j <= (width - 16); j += 8) {
					s0 = s1;
					s1 = sp[1];
					(*dp++) = vis_faligndata(s0, s1);
					sp++;
				}

				if (j <= (width - 8)) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					(*dp++) = vis_faligndata(s0, s1);
					sp++;
					j += 8;
				}

				if (j < width) {
					s0 = vis_faligndata(s1,
					    vis_ld_d64_nf(sp + 1));
					emask = vis_edge8(dp, dend);
					vis_pst_8(s0, dp, emask);
				}
			} else {
				mlib_d64 *sp = (mlib_d64 *)sa;
				mlib_d64 *dp = (mlib_d64 *)da;

#pragma pipeloop(0)
				for (j = 0; j < width / 8; j++) {
					(*dp++) = (*sp++);
				}

				if (width & 7) {
					vis_pst_8(vis_ld_d64_nf(sp), dp,
					    0xFF00 >> (width & 7));
				}
			}

			sa += stride;
			da += dstride;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
