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

#pragma ident	"@(#)mlib_m_ImageAffine_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Image affine transformation with Nearest Neighbor filtering
 *
 */

#include <mlib_ImageAffine.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#undef  MLIB_POINTER_SHIFT
#define	MLIB_POINTER_SHIFT(P)	(P >> (MLIB_SHIFT-(sizeof (mlib_addr) / 4 + \
	1))) &~ (sizeof (mlib_addr)-1)
#undef  MLIB_POINTER_GET
#define	MLIB_POINTER_GET(A, P)	(*(MLIB_TYPE **)((mlib_addr)A + P))

/* *********************************************************** */

#define	DECLAREVAR_NN1()                                          \
	DECLAREVAR_NN();                                          \
	DTYPE *dstLineEnd;                                        \
	__m64 Stride64;                                           \
	mlib_s32 srcYStride = lineAddr[1] - lineAddr[0];          \
	MLIB_TYPE *srcPixelPtr0 = MLIB_POINTER_GET(lineAddr, 0)

/* *********************************************************** */

#define	DTYPE	MLIB_TYPE
#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_u8

mlib_status
mlib_ImageAffine_u8_1ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();

	if (srcYStride <= 0xFFFF) {

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));

		for (j = yStart; j <= yFinish; j++) {
			mlib_u8 pix0;
			mlib_s32 ind0, ind1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 2) {
				pix0 = srcPixelPtr0[ind0];
				dstPixelPtr[0] = pix0;
				pix0 = srcPixelPtr0[ind1];
				dstPixelPtr[1] = pix0;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd) {
				dstPixelPtr[0] = srcPixelPtr0[ind0];
			}
		}

		_mm_empty();
	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_u8 pix0;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			for (; dstPixelPtr < dstLineEnd; dstPixelPtr++) {
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				dstPixelPtr[0] = pix0;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				pix0 = srcPixelPtr[xSrc];
			}
			dstPixelPtr[0] = pix0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_3ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();
	__m64 three;

	if (srcYStride <= 0xFFFF) {

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));
		three = mmx_to_double_dup(3);

		for (j = yStart; j <= yFinish; j++) {
			mlib_u8 tail;
			mlib_s32 pix0, ind0, ind1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_madd_pi16(x64, three);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 6) {
				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind0);
				*(mlib_s32 *)dstPixelPtr = pix0;
				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind1);
				*(mlib_s32 *)(dstPixelPtr + 3) = pix0;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_madd_pi16(x64, three);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd) {
				*(mlib_s32 *)dstPixelPtr =
				    *(mlib_s32 *)(srcPixelPtr0 + ind0);
				dstPixelPtr += 3;
			}
			dstPixelPtr[0] = tail;
		}

		_mm_empty();
	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_u8 tail;
			mlib_s32 pix0;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			pix0 = *(mlib_s32 *)srcPixelPtr;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 3) {
				srcPixelPtr =
				    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				*(mlib_s32 *)dstPixelPtr = pix0;
				pix0 = *(mlib_s32 *)srcPixelPtr;
			}

			*(mlib_s32 *)dstPixelPtr = pix0;
			dstPixelPtr[3] = tail;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_u16

mlib_status
mlib_ImageAffine_s16_1ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();

	if (((srcYStride & 1) == 0) && (srcYStride <= 0x1FFFE)) {

		srcYStride >>= 1;

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));

		for (j = yStart; j <= yFinish; j++) {
			mlib_u16 pix0;
			mlib_s32 ind0, ind1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 2) {
				pix0 = srcPixelPtr0[ind0];
				dstPixelPtr[0] = pix0;
				pix0 = srcPixelPtr0[ind1];
				dstPixelPtr[1] = pix0;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd)
				dstPixelPtr[0] = srcPixelPtr0[ind0];
		}

		_mm_empty();
	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 pix0;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			for (; dstPixelPtr < dstLineEnd; dstPixelPtr++) {
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				dstPixelPtr[0] = pix0;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				pix0 = srcPixelPtr[xSrc];
			}
			dstPixelPtr[0] = pix0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_3ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();
	__m64 three;

	srcYStride >>= 1;

	if (srcYStride <= 0xFFFF) {

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));
		three = mmx_to_double_dup(3);

		for (j = yStart; j <= yFinish; j++) {
			mlib_u16 tail;
			mlib_s32 ind0, ind1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;
			__m64 pix0;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_madd_pi16(x64, three);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 6) {
				pix0 = *(__m64 *) (srcPixelPtr0 + ind0);
				*(__m64 *) dstPixelPtr = pix0;
				pix0 = *(__m64 *) (srcPixelPtr0 + ind1);
				*(__m64 *) (dstPixelPtr + 3) = pix0;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_madd_pi16(x64, three);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd) {
				*(__m64 *) dstPixelPtr =
				    *(__m64 *) (srcPixelPtr0 + ind0);
				dstPixelPtr += 3;
			}
			dstPixelPtr[0] = tail;
		}
	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_u16 tail;
			__m64 pix0;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			pix0 = *(__m64 *) srcPixelPtr;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 3) {
				srcPixelPtr =
				    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				*(__m64 *) dstPixelPtr = pix0;
				pix0 = *(__m64 *) srcPixelPtr;
			}

			*(__m64 *) dstPixelPtr = pix0;
			dstPixelPtr[3] = tail;
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s32

mlib_status
mlib_ImageAffine_s32_1ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();

	if (((srcYStride & 3) == 0) && (srcYStride <= 0x3FFFC)) {

		srcYStride >>= 2;

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));

		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 pix0, ind0, ind1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 2) {
				pix0 = srcPixelPtr0[ind0];
				dstPixelPtr[0] = pix0;
				pix0 = srcPixelPtr0[ind1];
				dstPixelPtr[1] = pix0;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_srai_pi32(x64, MLIB_SHIFT);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd)
				dstPixelPtr[0] = srcPixelPtr0[ind0];
		}

		_mm_empty();
	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 pix0;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			for (; dstPixelPtr < dstLineEnd; dstPixelPtr++) {
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				dstPixelPtr[0] = pix0;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				pix0 = srcPixelPtr[xSrc];
			}
			dstPixelPtr[0] = pix0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_3ch_nn(
    mlib_affine_param *param)
{
	DECLAREVAR_NN1();
	__m64 three;

	srcYStride >>= 2;

	if (srcYStride <= 0xFFFF) {

		Stride64 = mmx_to_double_dup((((srcYStride) & 0xFFFF)));
		three = mmx_to_double_dup(3);

		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 ind0, ind1;
			mlib_s32 pix1;
			__m64 dx64, dy64, x64, y64;
			__m64 tmp0, tmp1, tmp2;
			__m64 pix0;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			dx64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dX << 1), 32),
			    _m_from_int(dX << 1));
			dy64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(dY << 1), 32),
			    _m_from_int(dY << 1));

			x64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(X), 32),
			    _m_from_int(X + dX));
			y64 =
			    _mm_or_si64(_mm_slli_si64(_m_from_int(Y), 32),
			    _m_from_int(Y + dY));

			tmp0 = _mm_madd_pi16(x64, three);
			tmp1 = _mm_madd_pi16(y64, Stride64);
			tmp2 = _mm_add_pi32(tmp0, tmp1);
			ind0 = mmx_read_lo(tmp2);
			ind1 = mmx_read_hi(tmp2);

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 6) {
				pix0 = *(__m64 *) (srcPixelPtr0 + ind0);
				*(__m64 *) dstPixelPtr = pix0;
				pix1 = srcPixelPtr0[ind0 + 2];
				dstPixelPtr[2] = pix1;
				pix0 = *(__m64 *) (srcPixelPtr0 + ind1);
				*(__m64 *) (dstPixelPtr + 3) = pix0;
				pix1 = srcPixelPtr0[ind1 + 2];
				dstPixelPtr[5] = pix1;

				x64 = _mm_add_pi32(x64, dx64);
				y64 = _mm_add_pi32(y64, dy64);

				tmp0 = _mm_madd_pi16(x64, three);
				tmp1 = _mm_madd_pi16(y64, Stride64);
				tmp2 = _mm_add_pi32(tmp0, tmp1);
				ind0 = mmx_read_lo(tmp2);
				ind1 = mmx_read_hi(tmp2);
			}

			if (dstPixelPtr == dstLineEnd) {
				pix0 = *(__m64 *) (srcPixelPtr0 + ind0);
				*(__m64 *) dstPixelPtr = pix0;
				pix1 = srcPixelPtr0[ind0 + 2];
				dstPixelPtr[2] = pix1;
			}
		}

		_mm_empty();

	} else {
		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 pix0, pix1, pix2;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			pix0 = srcPixelPtr[0];
			pix1 = srcPixelPtr[1];
			pix2 = srcPixelPtr[2];
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;

			for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 3) {
				srcPixelPtr =
				    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				dstPixelPtr[0] = pix0;
				dstPixelPtr[1] = pix1;
				dstPixelPtr[2] = pix2;
				pix0 = srcPixelPtr[0];
				pix1 = srcPixelPtr[1];
				pix2 = srcPixelPtr[2];
			}
			dstPixelPtr[0] = pix0;
			dstPixelPtr[1] = pix1;
			dstPixelPtr[2] = pix2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_2ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_s16_1ch_nn(param));
}

mlib_status
mlib_ImageAffine_u8_4ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_s32_1ch_nn(param));
}

mlib_status
mlib_ImageAffine_s16_2ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_s32_1ch_nn(param));
}

mlib_status
mlib_ImageAffine_s16_4ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_1ch_nn(param));
}

mlib_status
mlib_ImageAffine_s32_2ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_1ch_nn(param));
}

mlib_status
mlib_ImageAffine_s32_4ch_nn(
    mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_2ch_nn(param));
}

/* *********************************************************** */
