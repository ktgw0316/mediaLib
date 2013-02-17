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

#pragma ident	"@(#)mlib_c_ImageAdd.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAdd - add two images
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageAdd(mlib_image       *dst,
 *                                 const mlib_image *src1,
 *                                 const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = src1 + src2
 */

#include <mlib_ImageCheck.h>
#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAdd = __mlib_ImageAdd

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAdd) mlib_ImageAdd
    __attribute__((weak, alias("__mlib_ImageAdd")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAdd(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_type dtype;
	mlib_u8 *psrc1, *psrc2, *pdst;

/* indices for x, y  */
	mlib_s32 i, j, dd;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb1 = mlib_ImageGetStride(src1);
	psrc1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	psrc2 = mlib_ImageGetData(src2);

	xsize *= nchan;

	if (dtype == MLIB_BYTE) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				dd = psrc1[i] + (mlib_s32)psrc2[i];
				pdst[i] = ((dd << 23) >> 31) | dd;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {

		for (j = 0; j < ysize; j++) {

#ifdef _NO_LONGLONG

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 3) == 0 &&
			    (((mlib_addr)psrc1 ^ (mlib_addr)pdst) & 3) == 0) {
				i = 0;

				if (((mlib_addr)pdst) & 3) {
					((mlib_u16 *)pdst)[0] =
					    ((mlib_u16 *)psrc1)[0] +
					    ((mlib_u16 *)psrc2)[0];
					i = 1;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= (xsize - 2); i += 2) {
					mlib_s32 s1 =
					    *((mlib_s32 *)(psrc1 + 2 * i)), s2 =
					    *((mlib_s32 *)(psrc2 + 2 * i)), sd =
					    (s1 + s2) & 0xFFFF;

					sd |= (s1 & ~0xFFFF) + (s2 & ~0xFFFF);
					*((mlib_s32 *)(pdst + 2 * i)) = sd;
				}

				if (i < xsize) {
					((mlib_u16 *)pdst)[i] =
					    ((mlib_u16 *)psrc1)[i] +
					    ((mlib_u16 *)psrc2)[i];
				}
			}
#else /* _NO_LONGLONG */

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0 &&
			    (((mlib_addr)psrc1 ^ (mlib_addr)pdst) & 7) == 0) {
				for (i = 0;
				    (i < xsize) &&
				    (((mlib_addr)(pdst + 2 * i) & 7) != 0);
				    i++) {
					((mlib_u16 *)pdst)[i] =
					    ((mlib_u16 *)psrc1)[i] +
					    ((mlib_u16 *)psrc2)[i];
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= (xsize - 4); i += 4) {
					mlib_s64 ll1 =
					    *((mlib_s64 *)(psrc1 + 2 * i)),
					    ll2 =
					    *((mlib_s64 *)(psrc2 + 2 * i)),
					    lld =
					    (ll1 &
					    MLIB_U64_CONST(~0x8000800080008000))
					    +
					    (ll2 &
					    MLIB_U64_CONST
					    (~0x8000800080008000));

					lld ^=
					    (ll1 ^ ll2) &
					    MLIB_U64_CONST(0x8000800080008000);
					*((mlib_s64 *)(pdst + 2 * i)) = lld;
				}

				for (; i < xsize; i++) {
					((mlib_u16 *)pdst)[i] =
					    ((mlib_u16 *)psrc1)[i] +
					    ((mlib_u16 *)psrc2)[i];
				}
			}
#endif /* _NO_LONGLONG */
			else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					((mlib_u16 *)pdst)[i] =
					    ((mlib_u16 *)psrc1)[i] +
					    ((mlib_u16 *)psrc2)[i];
				}
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				dd = ((mlib_u16 *)psrc1)[i] +
				    (mlib_s32)((mlib_u16 *)psrc2)[i];
				((mlib_u16 *)pdst)[i] = ((dd << 15) >> 31) | dd;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {

		for (j = 0; j < ysize; j++) {

#ifdef _NO_LONGLONG
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				((mlib_s32 *)pdst)[i] =
				    ((mlib_s32 *)psrc1)[i] +
				    ((mlib_s32 *)psrc2)[i];
			}

#else /* _NO_LONGLONG */

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0 &&
			    (((mlib_addr)psrc1 ^ (mlib_addr)pdst) & 7) == 0) {
				i = 0;

				if (((mlib_addr)pdst) & 7) {

					((mlib_s32 *)pdst)[0] =
					    ((mlib_s32 *)psrc1)[0] +
					    ((mlib_s32 *)psrc2)[0];
					i = 1;
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= (xsize - 2); i += 2) {
					mlib_s64 ll1 =
					    *((mlib_s64 *)(psrc1 + 4 * i));
					mlib_s64 ll2 =
					    *((mlib_s64 *)(psrc2 + 4 * i));
					mlib_s64 lld =
					    (ll1 +
					    ll2) & MLIB_S64_CONST(0xFFFFFFFF);

					lld |=
					    (ll1 & MLIB_S64_CONST(~0xFFFFFFFF))
					    +
					    (ll2 & MLIB_S64_CONST(~0xFFFFFFFF));
					*((mlib_s64 *)(pdst + 4 * i)) = lld;
				}

				if (i < xsize) {

					((mlib_s32 *)pdst)[i] =
					    ((mlib_s32 *)psrc1)[i] +
					    ((mlib_s32 *)psrc2)[i];
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					((mlib_s32 *)pdst)[i] =
					    ((mlib_s32 *)psrc1)[i] +
					    ((mlib_s32 *)psrc2)[i];
				}
			}

#endif /* _NO_LONGLONG */

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
