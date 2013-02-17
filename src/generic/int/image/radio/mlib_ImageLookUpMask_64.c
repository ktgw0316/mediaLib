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

#pragma ident	"@(#)mlib_ImageLookUpMask_64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      void mlib_ImageLookUpMask_D64_U8
 *      void mlib_ImageLookUpMask_3_4_D64_U8
 *      void mlib_ImageLookUpMask_4_D64_U8
 *      void mlib_ImageLookUpMask_D64_S16
 *      void mlib_ImageLookUpMask_D64_U16
 *      void mlib_ImageLookUpMask_D64_S32
 *
 * DESCRIPTION
 *      Internal function for mlib_LookUpMask (c, vis version)
 */

#include <mlib_image.h>
#include <mlib_ImageLookUpMask.h>

/* *********************************************************** */

#ifdef _MSC_VER

#define	TABLE_SHIFT_S32	(mlib_u32)2147483648

#else /* _MSC_VER */

#define	TABLE_SHIFT_S32	2147483648u

#endif /* _MSC_VER */

/* *********************************************************** */

void
mlib_ImageLookUpMask_D64_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan)
{
	mlib_s32 i, j, k;

	if (xsize < 2) {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*((mlib_s64 *) dst) =
						*((mlib_s64 *) &tab[*src]);
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]];
				mlib_s32 s0, s1;
				mlib_s64  t0, t1;

				s0 = src[0] << 3;
				s1 = src[schan] << 3;
				src += 2 * schan;

				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
					s0 = src[0] << 3;
					s1 = src[schan] << 3;
					*((mlib_s64 *) &dst[0]) = t0;
					*((mlib_s64 *) &dst[dchan]) = t1;
				}

				t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
				*((mlib_s64 *) &dst[0]) = t0;
				*((mlib_s64 *) &dst[dchan]) = t1;

				if (xsize & 1)
					*((mlib_s64 *) &dst[2 * dchan]) =
						*((mlib_s64 *) &tab[src[0]]);
			}
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 3, 4; chan == 2		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_3_4_D64_U8(
    mlib_d64 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan)
{
	mlib_d64 *tab0 = (mlib_d64 *)table[table_i[0]];
	mlib_d64 *tab1 = (mlib_d64 *)table[table_i[1]];
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++, dst += (dlb >> 3), src += slb) {
		mlib_u32 *sa;
		mlib_d64 *dp = (mlib_d64 *)dst;
		mlib_d64 *dp1, *dp2;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_s64 t0, t1, t2, t3;
		mlib_s64 t4, t5, t6, t7;
		mlib_u32 s00, s01, s02, s03, s0;
		mlib_s32 size = xsize;

		dp1 = (mlib_d64 *)dp + dst_i[0];
		dp2 = (mlib_d64 *)dp + dst_i[1];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = *((mlib_s64 *) &tab0[s0]);
			t1 = *((mlib_s64 *) &tab1[s0]);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			dp1 += dchan;
			dp2 += dchan;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
			s00 = (s0 << 3) & 0x7F8;
			s01 = (s0 >> 5) & 0x7F8;
#else /* _LITTLE_ENDIAN */
			s00 = (s0 >> 21) & 0x7F8;
			s01 = (s0 >> 13) & 0x7F8;
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 4 * dchan, dp2 += 4 * dchan) {
#ifdef _LITTLE_ENDIAN
				s02 = (s0 >> 13) & 0x7F8;
				s03 = (s0 >> 21) & 0x7F8;
#else /* _LITTLE_ENDIAN */
				s02 = (s0 >> 5) & 0x7F8;
				s03 = (s0 << 3) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 + s00);
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 + s00);
				t2 = *(mlib_s64 *)((mlib_u8 *)tab0 + s01);
				t3 = *(mlib_s64 *)((mlib_u8 *)tab1 + s01);
				t4 = *(mlib_s64 *)((mlib_u8 *)tab0 + s02);
				t5 = *(mlib_s64 *)((mlib_u8 *)tab1 + s02);
				t6 = *(mlib_s64 *)((mlib_u8 *)tab0 + s03);
				t7 = *(mlib_s64 *)((mlib_u8 *)tab1 + s03);
				s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
				s00 = (s0 << 3) & 0x7F8;
				s01 = (s0 >> 5) & 0x7F8;
#else /* _LITTLE_ENDIAN */
				s00 = (s0 >> 21) & 0x7F8;
				s01 = (s0 >> 13) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp1[0]) = t0;
				*((mlib_s64 *) &dp2[0]) = t1;
				*((mlib_s64 *) &dp1[dchan]) = t2;
				*((mlib_s64 *) &dp2[dchan]) = t3;
				*((mlib_s64 *) &dp1[2 * dchan]) = t4;
				*((mlib_s64 *) &dp2[2 * dchan]) = t5;
				*((mlib_s64 *) &dp1[3 * dchan]) = t6;
				*((mlib_s64 *) &dp2[3 * dchan]) = t7;
			}

#ifdef _LITTLE_ENDIAN
			s02 = (s0 >> 13) & 0x7F8;
			s03 = (s0 >> 21) & 0x7F8;
#else /* _LITTLE_ENDIAN */
			s02 = (s0 >> 5) & 0x7F8;
			s03 = (s0 << 3) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 + s00);
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 + s00);
			t2 = *(mlib_s64 *)((mlib_u8 *)tab0 + s01);
			t3 = *(mlib_s64 *)((mlib_u8 *)tab1 + s01);
			t4 = *(mlib_s64 *)((mlib_u8 *)tab0 + s02);
			t5 = *(mlib_s64 *)((mlib_u8 *)tab1 + s02);
			t6 = *(mlib_s64 *)((mlib_u8 *)tab0 + s03);
			t7 = *(mlib_s64 *)((mlib_u8 *)tab1 + s03);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			*((mlib_s64 *) &dp1[dchan]) = t2;
			*((mlib_s64 *) &dp2[dchan]) = t3;
			*((mlib_s64 *) &dp1[2 * dchan]) = t4;
			*((mlib_s64 *) &dp2[2 * dchan]) = t5;
			*((mlib_s64 *) &dp1[3 * dchan]) = t6;
			*((mlib_s64 *) &dp2[3 * dchan]) = t7;
			i += 4;
			dp1 += 4 * dchan;
			dp2 += 4 * dchan;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = *((mlib_s64 *) &tab0[s0]);
			t1 = *((mlib_s64 *) &tab1[s0]);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			dp1 += dchan;
			dp2 += dchan;
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 4; chan == 3		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_4_D64_U8(
    mlib_d64 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i)
{
	mlib_d64 *tab0 = (mlib_d64 *)table[table_i[0]];
	mlib_d64 *tab1 = (mlib_d64 *)table[table_i[1]];
	mlib_d64 *tab2 = (mlib_d64 *)table[table_i[2]];
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++, dst += (dlb >> 3), src += slb) {
		mlib_u32 *sa;
		mlib_d64 *dp = (mlib_d64 *)dst;
		mlib_d64 *dp1, *dp2, *dp3;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 s00, s01, s02, s03, s0;
		mlib_s64 t0, t1, t2, t3;
		mlib_s64 t4, t5, t6, t7;
		mlib_s64 t8, t9, t10, t11;

		mlib_s32 size = xsize;

		dp1 = (mlib_d64 *)dp + dst_i[0];
		dp2 = (mlib_d64 *)dp + dst_i[1];
		dp3 = (mlib_d64 *)dp + dst_i[2];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = *((mlib_s64 *) &tab0[s0]);
			t1 = *((mlib_s64 *) &tab1[s0]);
			t2 = *((mlib_s64 *) &tab2[s0]);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			*((mlib_s64 *) &dp3[0]) = t2;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
			s00 = (s0 << 3) & 0x7F8;
			s01 = (s0 >> 5) & 0x7F8;
#else /* _LITTLE_ENDIAN */
			s00 = (s0 >> 21) & 0x7F8;
			s01 = (s0 >> 13) & 0x7F8;
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 16, dp2 += 16, dp3 += 16) {
#ifdef _LITTLE_ENDIAN
				s02 = (s0 >> 13) & 0x7F8;
				s03 = (s0 >> 21) & 0x7F8;
#else /* _LITTLE_ENDIAN */
				s02 = (s0 >> 5) & 0x7F8;
				s03 = (s0 << 3) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 + s00);
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 + s00);
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 + s00);
				t3 = *(mlib_s64 *)((mlib_u8 *)tab0 + s01);
				t4 = *(mlib_s64 *)((mlib_u8 *)tab1 + s01);
				t5 = *(mlib_s64 *)((mlib_u8 *)tab2 + s01);
				t6 = *(mlib_s64 *)((mlib_u8 *)tab0 + s02);
				t7 = *(mlib_s64 *)((mlib_u8 *)tab1 + s02);
				t8 = *(mlib_s64 *)((mlib_u8 *)tab2 + s02);
				t9 = *(mlib_s64 *)((mlib_u8 *)tab0 + s03);
				t10 = *(mlib_s64 *)((mlib_u8 *)tab1 + s03);
				t11 = *(mlib_s64 *)((mlib_u8 *)tab2 + s03);
				s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
				s00 = (s0 << 3) & 0x7F8;
				s01 = (s0 >> 5) & 0x7F8;
#else /* _LITTLE_ENDIAN */
				s00 = (s0 >> 21) & 0x7F8;
				s01 = (s0 >> 13) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp1[0]) = t0;
				*((mlib_s64 *) &dp2[0]) = t1;
				*((mlib_s64 *) &dp3[0]) = t2;
				*((mlib_s64 *) &dp1[4]) = t3;
				*((mlib_s64 *) &dp2[4]) = t4;
				*((mlib_s64 *) &dp3[4]) = t5;
				*((mlib_s64 *) &dp1[8]) = t6;
				*((mlib_s64 *) &dp2[8]) = t7;
				*((mlib_s64 *) &dp3[8]) = t8;
				*((mlib_s64 *) &dp1[12]) = t9;
				*((mlib_s64 *) &dp2[12]) = t10;
				*((mlib_s64 *) &dp3[12]) = t11;
			}

#ifdef _LITTLE_ENDIAN
			s02 = (s0 >> 13) & 0x7F8;
			s03 = (s0 >> 21) & 0x7F8;
#else /* _LITTLE_ENDIAN */
			s02 = (s0 >> 5) & 0x7F8;
			s03 = (s0 << 3) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 + s00);
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 + s00);
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 + s00);
			t3 = *(mlib_s64 *)((mlib_u8 *)tab0 + s01);
			t4 = *(mlib_s64 *)((mlib_u8 *)tab1 + s01);
			t5 = *(mlib_s64 *)((mlib_u8 *)tab2 + s01);
			t6 = *(mlib_s64 *)((mlib_u8 *)tab0 + s02);
			t7 = *(mlib_s64 *)((mlib_u8 *)tab1 + s02);
			t8 = *(mlib_s64 *)((mlib_u8 *)tab2 + s02);
			t9 = *(mlib_s64 *)((mlib_u8 *)tab0 + s03);
			t10 = *(mlib_s64 *)((mlib_u8 *)tab1 + s03);
			t11 = *(mlib_s64 *)((mlib_u8 *)tab2 + s03);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			*((mlib_s64 *) &dp3[0]) = t2;
			*((mlib_s64 *) &dp1[4]) = t3;
			*((mlib_s64 *) &dp2[4]) = t4;
			*((mlib_s64 *) &dp3[4]) = t5;
			*((mlib_s64 *) &dp1[8]) = t6;
			*((mlib_s64 *) &dp2[8]) = t7;
			*((mlib_s64 *) &dp3[8]) = t8;
			*((mlib_s64 *) &dp1[12]) = t9;
			*((mlib_s64 *) &dp2[12]) = t10;
			*((mlib_s64 *) &dp3[12]) = t11;
			i += 4;
			dp1 += 16;
			dp2 += 16;
			dp3 += 16;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = *((mlib_s64 *) &tab0[s0]);
			t1 = *((mlib_s64 *) &tab1[s0]);
			t2 = *((mlib_s64 *) &tab2[s0]);
			*((mlib_s64 *) &dp1[0]) = t0;
			*((mlib_s64 *) &dp2[0]) = t1;
			*((mlib_s64 *) &dp3[0]) = t2;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_D64_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan)
{
	mlib_s32 i, j, k;

	if (xsize < 2) {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]] + 32768;

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*((mlib_s64 *)dst) =
						*((mlib_s64 *) &tab[*src]);
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]] + 32768;
				mlib_s32 s0, s1;
				mlib_s64  t0, t1;

				s0 = src[0] << 3;
				s1 = src[schan] << 3;
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
					s0 = src[0] << 3;
					s1 = src[schan] << 3;
					*((mlib_s64 *) &dst[0]) = t0;
					*((mlib_s64 *) &dst[dchan]) = t1;
				}

				t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
				*((mlib_s64 *) &dst[0]) = t0;
				*((mlib_s64 *) &dst[dchan]) = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_D64_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan)
{
	mlib_s32 i, j, k;

	if (xsize < 2) {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*((mlib_s64 *)dst) =
						*((mlib_s64 *) &tab[*src]);
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_d64 *tab = table[table_i[k]];
				mlib_s32 s0, s1;
				mlib_s64 t0, t1;

				s0 = src[0] << 3;
				s1 = src[schan] << 3;
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
					s0 = src[0] << 3;
					s1 = src[schan] << 3;
					*((mlib_s64 *) &dst[0]) = t0;
					*((mlib_s64 *) &dst[dchan]) = t1;
				}

				t0 = *(mlib_s64 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s64 *)((mlib_addr)tab + s1);
				*((mlib_s64 *) &dst[0]) = t0;
				*((mlib_s64 *) &dst[dchan]) = t1;

				if (xsize & 1)
					*((mlib_s64 *) &dst[2 * dchan]) =
						*((mlib_s64 *) &tab[src[0]]);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_D64_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan)
{
	mlib_s32 i, j, k;

	if (xsize < 2) {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_d64 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*((mlib_s64 *) dst) =
						*((mlib_s64 *) &tab[*src]);
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_d64 *dst = ((mlib_d64 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_d64 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */
				mlib_s32 s0, s1;
				mlib_s64 t0, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *((mlib_s64 *) &tab[s0]);
					t1 = *((mlib_s64 *) &tab[s1]);
					s0 = src[0];
					s1 = src[schan];
					*((mlib_s64 *) &dst[0]) = t0;
					*((mlib_s64 *) &dst[dchan]) = t1;
				}

				t0 = *((mlib_s64 *) &tab[s0]);
				t1 = *((mlib_s64 *) &tab[s1]);
				*((mlib_s64 *) &dst[0]) = t0;
				*((mlib_s64 *) &dst[dchan]) = t1;

				if (xsize & 1)
					*((mlib_s64 *) &dst[2 * dchan]) =
						*((mlib_s64 *) &tab[src[0]]);
			}
		}
	}
}

/* *********************************************************** */
