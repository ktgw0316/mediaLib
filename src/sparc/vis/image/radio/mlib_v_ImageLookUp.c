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

#pragma ident	"@(#)mlib_v_ImageLookUp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp - table lookup
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUp(mlib_image       *dst,
 *                                   const mlib_image *src,
 *                                   const void       **table)
 *
 * ARGUMENT
 *      dst      Pointer to destination image.
 *      src      Pointer to source image.
 *      table    Lookup table.
 *
 * DESCRIPTION
 *      The mlib_ImageLookUp function performs general table lookup on an
 *      image. The destination image is obtained by passing a source image
 *      through a lookup table.
 *
 *      The source image may be 1-, 2-, 3-, or 4-channeled of data types
 *      MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, or MLIB_INT. The lookup
 *      table may be 1-, 2-, 3-, or 4-channeled of data types MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT, or MLIB_DOUBLE.
 *      The destination image must have the same
 *      number of channels as either source image or the lookup table,
 *      whichever is greater, and the same data type as the lookup table.
 *
 *      It is the user's responsibility to make sure that the lookup table
 *      supplied is suitable for the source image. Specifically, the table
 *      entries cover the entire range of source data. Otherwise, the result
 *      of this function is undefined.
 *
 *      The pixel values of the destination image are defined as the following:
 *
 *      If the source image is single-channeled and the destination image is
 *      multi-channeled, then the lookup table has the same number of channels
 *      as the destination image:
 *
 *          dst[x][y][c] = table[c][src[x][y][0]]
 *
 *      If the source image is multi-channeled and the destination image is
 *      multi-channeled, with the same number of channels as the source image,
 *      then the lookup table will have the same number of channels as
 *      the source image:
 *
 *          dst[x][y][c] = table[c][src[x][y][c]]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLookUp.h>
#include <mlib_v_ImageLookUpFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLookUp = __mlib_ImageLookUp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLookUp) mlib_ImageLookUp
    __attribute__((weak, alias("__mlib_ImageLookUp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALL_SIFUNC(STYPE, DTYPE, TYPE)                                   \
	switch (nchan) {                                                  \
	case 2:                                                           \
	    mlib_v_ImageLookUpSI_##STYPE##_##DTYPE##_2                    \
		(sa, slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                        \
	case 3:                                                           \
	    mlib_v_ImageLookUpSI_##STYPE##_##DTYPE##_3                    \
		(sa, slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                        \
	case 4:                                                           \
	    mlib_v_ImageLookUpSI_##STYPE##_##DTYPE##_4                    \
		(sa, slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                        \
	default:                                                          \
	    return (MLIB_FAILURE);                                        \
	}                                                                 \
	                                                                  \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	CALL_FUNC(STYPE, DTYPE, TYPE)                                \
	switch (nchan) {                                             \
	case 1:                                                      \
	    mlib_v_ImageLookUp_##STYPE##_##DTYPE##_1(sa,             \
		slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                   \
	case 2:                                                      \
	    mlib_v_ImageLookUp_##STYPE##_##DTYPE##_2(sa,             \
		slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                   \
	case 3:                                                      \
	    mlib_v_ImageLookUp_##STYPE##_##DTYPE##_3(sa,             \
		slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                   \
	case 4:                                                      \
	    mlib_v_ImageLookUp_##STYPE##_##DTYPE##_4(sa,             \
		slb, da, dlb, xsize, ysize, (const TYPE **)table);   \
	    break;                                                   \
	default:                                                     \
	    return (MLIB_FAILURE);                                   \
	}                                                            \
	                                                             \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_ImageLookUp(
    mlib_image *dst,
    const mlib_image *src,
    const void **table)
{
	mlib_s32 slb, dlb, xsize, ysize, nchan, ichan, bitoff_src;
	mlib_type stype, dtype;
	void *sa, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_SRC1_OR_EQ(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	ichan = mlib_ImageGetChannels(src);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (ichan == nchan) {
		if (dtype == MLIB_BYTE) {
			if (stype == MLIB_BYTE) {
				CALL_FUNC(U8, U8, mlib_u8);
			} else if (stype == MLIB_SHORT) {
				CALL_FUNC(S16, U8, mlib_u8);
			} else if (stype == MLIB_USHORT) {
				CALL_FUNC(U16, U8, mlib_u8);
			} else if (stype == MLIB_INT) {
				CALL_FUNC(S32, U8, mlib_u8);
			} else if (stype == MLIB_BIT) {
				if (nchan != 1)
					return (MLIB_FAILURE);

/* bits to first byte */
				bitoff_src = mlib_ImageGetBitOffset(src);
				return mlib_ImageLookUp_Bit_U8_1(sa, slb, da,
				    dlb, xsize, ysize, nchan, bitoff_src,
				    (const mlib_u8 **)table);
			}
		} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
			if (stype == MLIB_BYTE) {
				CALL_FUNC(U8, S16, mlib_s16);
			} else if (stype == MLIB_SHORT) {
				CALL_FUNC(S16, S16, mlib_s16);
			} else if (stype == MLIB_USHORT) {
				CALL_FUNC(U16, S16, mlib_s16);
			} else if (stype == MLIB_INT) {
				CALL_FUNC(S32, S16, mlib_s16);
			}
		} else if (dtype == MLIB_INT || dtype == MLIB_FLOAT) {
			if (stype == MLIB_BYTE) {
				CALL_FUNC(U8, S32, mlib_s32);
			} else if (stype == MLIB_SHORT) {
				CALL_FUNC(S16, S32, mlib_s32);
			} else if (stype == MLIB_USHORT) {
				CALL_FUNC(U16, S32, mlib_s32);
			} else if (stype == MLIB_INT) {
				if ((nchan >= 1) && (nchan <= 4)) {
					mlib_v_ImageLookUp_S32_S32(sa, slb, da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table, nchan);
					return (MLIB_SUCCESS);
				} else {
					return (MLIB_FAILURE);
				}
			}
		} else if (dtype == MLIB_DOUBLE) {
			if (stype == MLIB_BYTE) {
				mlib_ImageLookUp_U8_D64(sa, slb, da, dlb / 8,
				    xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				mlib_ImageLookUp_S16_D64(sa, slb / 2, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				mlib_ImageLookUp_U16_D64(sa, slb / 2, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				mlib_ImageLookUp_S32_D64(sa, slb / 4, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			}
		}
	} else if (ichan == 1) {
		if (dtype == MLIB_BYTE) {
			if (stype == MLIB_BYTE) {
				CALL_SIFUNC(U8, U8, mlib_u8);
			} else if (stype == MLIB_SHORT) {
				CALL_SIFUNC(S16, U8, mlib_u8);
			} else if (stype == MLIB_USHORT) {
				CALL_SIFUNC(U16, U8, mlib_u8);
			} else if (stype == MLIB_INT) {
				CALL_SIFUNC(S32, U8, mlib_u8);
			} else if (stype == MLIB_BIT) {
/* bits to first byte */
				bitoff_src = mlib_ImageGetBitOffset(src);

				if (nchan == 2) {
					return mlib_ImageLookUp_Bit_U8_2(sa,
					    slb, da, dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				} else if (nchan == 3) {
					return mlib_ImageLookUp_Bit_U8_3(sa,
					    slb, da, dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				} else {
/* (nchan == 4) */
					return mlib_ImageLookUp_Bit_U8_4(sa,
					    slb, da, dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				}
			}
		} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
			if (stype == MLIB_BYTE) {
				CALL_SIFUNC(U8, S16, mlib_s16);
			} else if (stype == MLIB_SHORT) {
				CALL_SIFUNC(S16, S16, mlib_s16);
			} else if (stype == MLIB_USHORT) {
				CALL_SIFUNC(U16, S16, mlib_s16);
			} else if (stype == MLIB_INT) {
				CALL_SIFUNC(S32, S16, mlib_s16);
			}
		} else if (dtype == MLIB_INT || dtype == MLIB_FLOAT) {

			if (stype == MLIB_BYTE) {
				CALL_SIFUNC(U8, S32, mlib_s32);
			} else if (stype == MLIB_SHORT) {
				CALL_SIFUNC(S16, S32, mlib_s32);
			} else if (stype == MLIB_USHORT) {
				CALL_SIFUNC(U16, S32, mlib_s32);
			} else if (stype == MLIB_INT) {
				if ((nchan >= 1) && (nchan <= 4)) {
					mlib_v_ImageLookUpSI_S32_S32(sa, slb,
					    da, dlb, xsize, ysize,
					    (const mlib_s32 **)table, nchan);
					return (MLIB_SUCCESS);
				} else {
					return (MLIB_FAILURE);
				}
			}
		} else if (dtype == MLIB_DOUBLE) {
			if (stype == MLIB_BYTE) {
				mlib_ImageLookUpSI_U8_D64(sa, slb, da, dlb / 8,
				    xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				mlib_ImageLookUpSI_S16_D64(sa, slb / 2, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				mlib_ImageLookUpSI_U16_D64(sa, slb / 2, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				mlib_ImageLookUpSI_S32_D64(sa, slb / 4, da,
				    dlb / 8, xsize, ysize, nchan,
				    (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			}
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
