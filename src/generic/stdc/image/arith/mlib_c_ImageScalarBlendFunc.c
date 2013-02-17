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

#pragma ident	"@(#)mlib_c_ImageScalarBlendFunc.c	9.2	07/10/09 SMI"

/*
 * Internal function.
 *
 * void mlib_c_ImageScalarBlend_U8_124(mlib_u8  *sa1,
 *                                     mlib_s32 slb1,
 *                                     mlib_u8  *sa2,
 *                                     mlib_s32 slb2,
 *                                     mlib_u8  *da,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_u8  **lut);
 *
 * void mlib_c_ImageScalarBlend_U8_3(mlib_u8  *sa1,
 *                                   mlib_s32 slb1,
 *                                   mlib_u8  *sa2,
 *                                   mlib_s32 slb2,
 *                                   mlib_u8  *da,
 *                                   mlib_s32 dlb,
 *                                   mlib_s32 xsize,
 *                                   mlib_s32 ysize,
 *                                   mlib_u8  **lut);
 *
 * void mlib_c_ImageScalarBlend_S16_1(mlib_s16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S16_2(mlib_s16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S16_3(mlib_s16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S16_4(mlib_s16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_U16_1(mlib_u16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_U16_2(mlib_u16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_U16_3(mlib_u16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_U16_4(mlib_u16 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u16 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u16 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S32_1(mlib_s32 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s32 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s32 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S32_2(mlib_s32 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s32 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s32 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S32_3(mlib_s32 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s32 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s32 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 *
 * void mlib_c_ImageScalarBlend_S32_4(mlib_s32 *sa1,
 *                                    mlib_s32 slb1,
 *                                    mlib_s32 *sa2,
 *                                    mlib_s32 slb2,
 *                                    mlib_s32 *da,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_s32 *alpha);
 */

#include <mlib_image.h>
#include <mlib_c_ImageScalarBlendFunc.h>

/* *********************************************************** */

#define	MASK	0x7fffffff
#define	SIZE	512

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	CHAR3(src)	((mlib_s32)(((mlib_u32) src) >> 24))
#define	CHAR2(src)	((mlib_s32)((src >> 16) & 0xff))
#define	CHAR1(src)	((mlib_s32)((src >> 8) & 0xff))
#define	CHAR0(src)	((mlib_s32)(src & 0xff))
#else /* _LITTLE_ENDIAN */
#define	CHAR0(src)	((mlib_s32)(((mlib_u32) src) >> 24))
#define	CHAR1(src)	((mlib_s32)((src >> 16) & 0xff))
#define	CHAR2(src)	((mlib_s32)((src >> 8) & 0xff))
#define	CHAR3(src)	((mlib_s32)(src & 0xff))
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U8_124(
    mlib_u8 *sa1,
    mlib_s32 slb1,
    mlib_u8 *sa2,
    mlib_s32 slb2,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_u8 **lut)
{
	mlib_u8 *lut0, *lut1, *lut2, *lut3;
	mlib_u8 *psrc1 = sa1;
	mlib_u8 *psrc2 = sa2;
	mlib_u32 *ps;
	mlib_s32 src2, shl, src1_0, src1_1;

/* indices for x and y */
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++) {
		if (i = (mlib_addr)psrc1 & 1) {
			src2 = psrc2[0];
			da[j * dlb] = lut[0][psrc1[0] - src2] + src2;
		}

		lut0 = lut[i];
		lut1 = lut[i + 1];
		lut2 = lut[i + 2];
		lut3 = lut[(i + 3) & 3];
		ps = (mlib_u32 *)(psrc2 + i);

		if (shl = (mlib_addr)ps & 3) {
			mlib_u32 shr, src2_0, src2_1;

			ps = (mlib_u32 *)((mlib_addr)ps - shl);
			shl <<= 3;
			shr = 32 - shl;
			src2_1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (xsize - 4); i += 4) {
				src1_0 = *((mlib_u16 *)(psrc1 + i));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 2));
				src2_0 = src2_1;
				src2_1 = ps[1];
#ifdef _LITTLE_ENDIAN
				src2 = (src2_0 >> shl) | (src2_1 << shr);
				da[j * dlb + i] =
				    lut0[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut3[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				src2 = (src2_0 << shl) | (src2_1 >> shr);
				da[j * dlb + i] =
				    lut0[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut3[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */
				ps++;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (xsize - 4); i += 4) {
				src1_0 = *((mlib_u16 *)(psrc1 + i));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 2));
				src2 = *((mlib_s32 *)(psrc2 + i));
#ifdef _LITTLE_ENDIAN
				da[j * dlb + i] =
				    lut0[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut3[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				da[j * dlb + i] =
				    lut0[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut3[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */
			}
		}

		for (; i < xsize; i++) {
			src2 = psrc2[i];
			da[j * dlb + i] =
			    lut[4 - xsize + i][psrc1[i] - src2] + src2;
		}

		psrc1 += slb1;
		psrc2 += slb2;
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U8_3(
    mlib_u8 *sa1,
    mlib_s32 slb1,
    mlib_u8 *sa2,
    mlib_s32 slb2,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_u8 **lut)
{
	mlib_u8 *lut0, *lut1, *lut2;
	mlib_u8 *psrc1 = sa1;
	mlib_u8 *psrc2 = sa2;
	mlib_u32 *ps;
	mlib_s32 src2, shl, src1_0, src1_1;

/* indices for x and y */
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++) {
		if (i = (mlib_addr)psrc1 & 1) {
			lut2 = lut[0];
			src2 = psrc2[0];
			da[j * dlb] = lut2[psrc1[0] - src2] + src2;
		} else
			lut2 = lut[2];
		lut0 = lut[i];
		lut1 = lut[i + 1];
		ps = (mlib_u32 *)(psrc2 + i);

		if (shl = (mlib_addr)ps & 3) {
			mlib_u32 shr, src2_0, src2_1;

			ps = (mlib_u32 *)((mlib_addr)ps - shl);
			shl <<= 3;
			shr = 32 - shl;
			src2_1 = ps[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (xsize - 12); i += 12) {
				src1_0 = *((mlib_u16 *)(psrc1 + i));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 2));
				src2_0 = src2_1;
				src2_1 = ps[1];
#ifdef _LITTLE_ENDIAN
				src2 = (src2_0 >> shl) | (src2_1 << shr);
				da[j * dlb + i] =
				    lut0[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut0[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				src2 = (src2_0 << shl) | (src2_1 >> shr);
				da[j * dlb + i] =
				    lut0[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut0[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */

				src1_0 = *((mlib_u16 *)(psrc1 + i + 4));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 6));
				src2_0 = src2_1;
				src2_1 = ps[2];
#ifdef _LITTLE_ENDIAN
				src2 = (src2_0 >> shl) | (src2_1 << shr);
				da[j * dlb + i + 4] =
				    lut1[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 5] =
				    lut2[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 6] =
				    lut0[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 7] =
				    lut1[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				src2 = (src2_0 << shl) | (src2_1 >> shr);
				da[j * dlb + i + 4] =
				    lut1[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 5] =
				    lut2[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 6] =
				    lut0[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 7] =
				    lut1[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */

				src1_0 = *((mlib_u16 *)(psrc1 + i + 8));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 10));
				src2_0 = src2_1;
				src2_1 = ps[3];
#ifdef _LITTLE_ENDIAN
				src2 = (src2_0 >> shl) | (src2_1 << shr);
				da[j * dlb + i + 8] =
				    lut2[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 9] =
				    lut0[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 10] =
				    lut1[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 11] =
				    lut2[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				src2 = (src2_0 << shl) | (src2_1 >> shr);
				da[j * dlb + i + 8] =
				    lut2[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 9] =
				    lut0[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 10] =
				    lut1[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 11] =
				    lut2[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */
				ps += 3;
			}
		} else {
#ifdef __SUNPRO_C
#ifdef _LP64
#pragma unroll(1)	/* cc Error */
#else /* _LP64 */
#pragma pipeloop(0)
#endif /* _LP64 */
#endif /* __SUNPRO_C */
			for (; i <= (xsize - 12); i += 12) {
				src1_0 = *((mlib_u16 *)(psrc1 + i));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 2));
				src2 = *((mlib_s32 *)(psrc2 + i));
#ifdef _LITTLE_ENDIAN
				da[j * dlb + i] =
				    lut0[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut0[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				da[j * dlb + i] =
				    lut0[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 1] =
				    lut1[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 2] =
				    lut2[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 3] =
				    lut0[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */

				src1_0 = *((mlib_u16 *)(psrc1 + i + 4));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 6));
				src2 = *((mlib_s32 *)(psrc2 + i + 4));
#ifdef _LITTLE_ENDIAN
				da[j * dlb + i + 4] =
				    lut1[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 5] =
				    lut2[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 6] =
				    lut0[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 7] =
				    lut1[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				da[j * dlb + i + 4] =
				    lut1[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 5] =
				    lut2[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 6] =
				    lut0[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 7] =
				    lut1[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */

				src1_0 = *((mlib_u16 *)(psrc1 + i + 8));
				src1_1 = *((mlib_u16 *)(psrc1 + i + 10));
				src2 = *((mlib_s32 *)(psrc2 + i + 8));
#ifdef _LITTLE_ENDIAN
				da[j * dlb + i + 8] =
				    lut2[CHAR0(src1_0) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 9] =
				    lut0[(src1_0 >> 8) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 10] =
				    lut1[CHAR0(src1_1) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 11] =
				    lut2[(src1_1 >> 8) - CHAR3(src2)] +
				    CHAR3(src2);
#else /* _LITTLE_ENDIAN */
				da[j * dlb + i + 8] =
				    lut2[(src1_0 >> 8) - CHAR0(src2)] +
				    CHAR0(src2);
				da[j * dlb + i + 9] =
				    lut0[CHAR3(src1_0) - CHAR1(src2)] +
				    CHAR1(src2);
				da[j * dlb + i + 10] =
				    lut1[(src1_1 >> 8) - CHAR2(src2)] +
				    CHAR2(src2);
				da[j * dlb + i + 11] =
				    lut2[CHAR3(src1_1) - CHAR3(src2)] +
				    CHAR3(src2);
#endif /* _LITTLE_ENDIAN */
			}
		}

		for (; i < xsize; i++) {
			src2 = psrc2[i];
			da[j * dlb + i] =
			    lut[(12 - xsize + i) % 3][psrc1[i] - src2] + src2;
		}

		psrc1 += slb1;
		psrc2 += slb2;
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S16_1(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s16 *sl1 = sa1;

/* pointer for line of source */
	mlib_s16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] = sl1[col + i] - sl2[col + i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[col + i] =
				    (mlib_s32)(a0 * (mlib_d64)buf[i]) +
				    sl2[col + i];
			}
		}

		sl1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S16_2(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s16 *sl1 = sa1;

/* pointer for line of source */
	mlib_s16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[2 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[2 * i] =
				    sl1[2 * (col + i)] - sl2[2 * (col + i)];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[2 * i + 1] =
				    sl1[2 * (col + i) + 1] - sl2[2 * (col + i) +
				    1];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[2 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[2 * i]) +
				    sl2[2 * (col + i)];
				dl[2 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[2 * i + 1]) +
				    sl2[2 * (col + i) + 1];
			}
		}

		sl1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S16_3(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s16 *sl1 = sa1;

/* pointer for line of source */
	mlib_s16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[3 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[3 * i] =
				    sl1[3 * (col + i)] - sl2[3 * (col + i)];
				buf[3 * i + 1] =
				    sl1[3 * (col + i) + 1] - sl2[3 * (col + i) +
				    1];
				buf[3 * i + 2] =
				    sl1[3 * (col + i) + 2] - sl2[3 * (col + i) +
				    2];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[3 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[3 * i]) +
				    sl2[3 * (col + i)];
				dl[3 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[3 * i + 1]) +
				    sl2[3 * (col + i) + 1];
				dl[3 * (col + i) + 2] =
				    (mlib_s32)(a2 * (mlib_d64)buf[3 * i + 2]) +
				    sl2[3 * (col + i) + 2];
			}
		}

		sl1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S16_4(
    mlib_s16 *sa1,
    mlib_s32 slb1,
    mlib_s16 *sa2,
    mlib_s32 slb2,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s16 *sl1 = sa1;

/* pointer for line of source */
	mlib_s16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[4 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a3 = -(alpha[3] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[4 * i] =
				    sl1[4 * (col + i)] - sl2[4 * (col + i)];
				buf[4 * i + 1] =
				    sl1[4 * (col + i) + 1] - sl2[4 * (col + i) +
				    1];
				buf[4 * i + 2] =
				    sl1[4 * (col + i) + 2] - sl2[4 * (col + i) +
				    2];
				buf[4 * i + 3] =
				    sl1[4 * (col + i) + 3] - sl2[4 * (col + i) +
				    3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[4 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[4 * i]) +
				    sl2[4 * (col + i)];
				dl[4 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[4 * i + 1]) +
				    sl2[4 * (col + i) + 1];
				dl[4 * (col + i) + 2] =
				    (mlib_s32)(a2 * (mlib_d64)buf[4 * i + 2]) +
				    sl2[4 * (col + i) + 2];
				dl[4 * (col + i) + 3] =
				    (mlib_s32)(a3 * (mlib_d64)buf[4 * i + 3]) +
				    sl2[4 * (col + i) + 3];
			}
		}

		sl1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U16_1(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_u16 *sl1 = sa1;

/* pointer for line of source */
	mlib_u16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_u16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++)
				buf[i] =
				    (mlib_s32)sl1[col + i] - (mlib_s32)sl2[col +
				    i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[col + i] =
				    (mlib_s32)(a0 * (mlib_d64)buf[i]) +
				    sl2[col + i];
			}
		}

		sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U16_2(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_u16 *sl1 = sa1;

/* pointer for line of source */
	mlib_u16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_u16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[2 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[2 * i] =
				    sl1[2 * (col + i)] - sl2[2 * (col + i)];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[2 * i + 1] =
				    sl1[2 * (col + i) + 1] - sl2[2 * (col + i) +
				    1];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[2 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[2 * i]) +
				    sl2[2 * (col + i)];
				dl[2 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[2 * i + 1]) +
				    sl2[2 * (col + i) + 1];
			}
		}

		sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U16_3(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_u16 *sl1 = sa1;

/* pointer for line of source */
	mlib_u16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_u16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[3 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[3 * i] =
				    sl1[3 * (col + i)] - sl2[3 * (col + i)];
				buf[3 * i + 1] =
				    sl1[3 * (col + i) + 1] - sl2[3 * (col + i) +
				    1];
				buf[3 * i + 2] =
				    sl1[3 * (col + i) + 2] - sl2[3 * (col + i) +
				    2];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[3 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[3 * i]) +
				    sl2[3 * (col + i)];
				dl[3 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[3 * i + 1]) +
				    sl2[3 * (col + i) + 1];
				dl[3 * (col + i) + 2] =
				    (mlib_s32)(a2 * (mlib_d64)buf[3 * i + 2]) +
				    sl2[3 * (col + i) + 2];
			}
		}

		sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_U16_4(
    mlib_u16 *sa1,
    mlib_s32 slb1,
    mlib_u16 *sa2,
    mlib_s32 slb2,
    mlib_u16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_u16 *sl1 = sa1;

/* pointer for line of source */
	mlib_u16 *sl2 = sa2;

/* pointer for line of destination */
	mlib_u16 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_s32 buf[4 * SIZE], num, col;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a3 = -(alpha[3] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				buf[4 * i] =
				    sl1[4 * (col + i)] - sl2[4 * (col + i)];
				buf[4 * i + 1] =
				    sl1[4 * (col + i) + 1] - sl2[4 * (col + i) +
				    1];
				buf[4 * i + 2] =
				    sl1[4 * (col + i) + 2] - sl2[4 * (col + i) +
				    2];
				buf[4 * i + 3] =
				    sl1[4 * (col + i) + 3] - sl2[4 * (col + i) +
				    3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dl[4 * (col + i)] =
				    (mlib_s32)(a0 * (mlib_d64)buf[4 * i]) +
				    sl2[4 * (col + i)];
				dl[4 * (col + i) + 1] =
				    (mlib_s32)(a1 * (mlib_d64)buf[4 * i + 1]) +
				    sl2[4 * (col + i) + 1];
				dl[4 * (col + i) + 2] =
				    (mlib_s32)(a2 * (mlib_d64)buf[4 * i + 2]) +
				    sl2[4 * (col + i) + 2];
				dl[4 * (col + i) + 3] =
				    (mlib_s32)(a3 * (mlib_d64)buf[4 * i + 3]) +
				    sl2[4 * (col + i) + 3];
			}
		}

		sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S32_1(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[i] =
			    (mlib_s32)(a0 * (mlib_d64)sl1[i] + (1 -
			    a0) * (mlib_d64)sl2[i]);
		}

		sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S32_2(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[2 * i] =
			    (mlib_s32)(a0 * (mlib_d64)sl1[2 * i] + (1 -
			    a0) * (mlib_d64)sl2[2 * i]);
			dl[2 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[2 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[2 * i + 1]);
		}

		sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S32_3(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[3 * i] =
			    (mlib_s32)(a0 * (mlib_d64)sl1[3 * i] + (1 -
			    a0) * (mlib_d64)sl2[3 * i]);
			dl[3 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[3 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[3 * i + 1]);
			dl[3 * i + 2] =
			    (mlib_s32)(a2 * (mlib_d64)sl1[3 * i + 2] + (1 -
			    a2) * (mlib_d64)sl2[3 * i + 2]);
		}

		sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_c_ImageScalarBlend_S32_4(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a3 = -(alpha[3] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[4 * i] =
			    (mlib_s32)(a0 * (mlib_d64)sl1[4 * i] + (1 -
			    a0) * (mlib_d64)sl2[4 * i]);
			dl[4 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[4 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[4 * i + 1]);
			dl[4 * i + 2] =
			    (mlib_s32)(a2 * (mlib_d64)sl1[4 * i + 2] + (1 -
			    a2) * (mlib_d64)sl2[4 * i + 2]);
			dl[4 * i + 3] =
			    (mlib_s32)(a3 * (mlib_d64)sl1[4 * i + 3] + (1 -
			    a3) * (mlib_d64)sl2[4 * i + 3]);
		}

		sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
