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

#pragma ident	"@(#)mlib_ImageLookUpMask_f.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLookUpMask.h>

/* *********************************************************** */

#ifdef _MSC_VER

#define	TABLE_SHIFT_S32	(mlib_u32)2147483648

#else /* _MSC_VER */

#define	TABLE_SHIFT_S32	2147483648u

#endif /* _MSC_VER */

/* *********************************************************** */

#ifdef i386	/* do not copy by mlib_d64 data type for x86 */

#define	TYPE_32BIT	mlib_u32

#else /* i386 ( do not copy by mlib_d64 data type for x86 ) */

#define	TYPE_32BIT	mlib_f32

#endif /* i386 ( do not copy by mlib_d64 data type for x86 ) */

/* *********************************************************** */

void
mlib_ImageLookUpMask_U8_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
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
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 3, 4; chan == 2		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_3_4_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan)
{
	mlib_u16 tab[256];
	const mlib_u8 *tab0 = table[table_i[0]];
	const mlib_u8 *tab1 = table[table_i[1]];
	mlib_s32 i, j, s0, s1, s2;

	s0 = tab0[0];
	s1 = tab1[0];
	for (i = 1; i < 256; i++) {
		s2 = (s0 << 8) + s1;
		s0 = tab0[i];
		s1 = tab1[i];
		tab[i - 1] = (mlib_u16)s2;
	}

	s2 = (s0 << 8) + s1;
	tab[255] = (mlib_u16)s2;

	for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
		mlib_u32 *sa;
		mlib_u8 *dp = dst;
		mlib_u8 *dp1, *dp2;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 t0, t1, t2, t3, s0;
		mlib_s32 size = xsize;

		dp1 = dp + dst_i[0];
		dp2 = dp + dst_i[1];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u8)(t0 >> 8);
			dp2[0] = (mlib_u8)t0;
			dp1 += dchan;
			dp2 += dchan;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 4 * dchan, dp2 += 4 * dchan) {
#ifdef _LITTLE_ENDIAN
				t3 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 23) & 0x1FE));
				t2 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 15) & 0x1FE));
				t1 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 7) & 0x1FE));
				t0 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 << 1) & 0x1FE));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 23) & 0x1FE));
				t1 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 15) & 0x1FE));
				t2 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 >> 7) & 0x1FE));
				t3 = *(mlib_u16 *)((mlib_u8 *)tab +
				    ((s0 << 1) & 0x1FE));
#endif /* _LITTLE_ENDIAN */
				s0 = (*sa++);
				dp1[0] = (mlib_u8)(t0 >> 8);
				dp2[0] = (mlib_u8)t0;
				dp1[dchan] = (mlib_u8)(t1 >> 8);
				dp2[dchan] = (mlib_u8)t1;
				dp1[2 * dchan] = (mlib_u8)(t2 >> 8);
				dp2[2 * dchan] = (mlib_u8)t2;
				dp1[3 * dchan] = (mlib_u8)(t3 >> 8);
				dp2[3 * dchan] = (mlib_u8)t3;
			}

#ifdef _LITTLE_ENDIAN
			t3 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 23) & 0x1FE));
			t2 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 15) & 0x1FE));
			t1 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 7) & 0x1FE));
			t0 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 << 1) & 0x1FE));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 23) & 0x1FE));
			t1 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 15) & 0x1FE));
			t2 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 >> 7) & 0x1FE));
			t3 = *(mlib_u16 *)((mlib_u8 *)tab +
			    ((s0 << 1) & 0x1FE));
#endif /* _LITTLE_ENDIAN */
			dp1[0] = (mlib_u8)(t0 >> 8);
			dp2[0] = (mlib_u8)t0;
			dp1[dchan] = (mlib_u8)(t1 >> 8);
			dp2[dchan] = (mlib_u8)t1;
			dp1[2 * dchan] = (mlib_u8)(t2 >> 8);
			dp2[2 * dchan] = (mlib_u8)t2;
			dp1[3 * dchan] = (mlib_u8)(t3 >> 8);
			dp2[3 * dchan] = (mlib_u8)t3;
			i += 4;
			dp1 += 4 * dchan;
			dp2 += 4 * dchan;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u8)(t0 >> 8);
			dp2[0] = (mlib_u8)t0;
			dp1 += dchan;
			dp2 += dchan;
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 4; chan == 3		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_4_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i)
{
	mlib_u32 tab[256];
	const mlib_u8 *tab0 = table[table_i[0]];
	const mlib_u8 *tab1 = table[table_i[1]];
	const mlib_u8 *tab2 = table[table_i[2]];
	mlib_s32 i, j;
	mlib_u32 s0, s1, s2, s3;

	s0 = tab0[0];
	s1 = tab1[0];
	s2 = tab2[0];
	for (i = 1; i < 256; i++) {
		s3 = (s0 << 16) + (s1 << 8) + s2;
		s0 = tab0[i];
		s1 = tab1[i];
		s2 = tab2[i];
		tab[i - 1] = s3;
	}

	s3 = (s0 << 16) + (s1 << 8) + s2;
	tab[255] = s3;

	for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
		mlib_u32 *sa;
		mlib_u8 *dp = dst;
		mlib_u8 *dp1, *dp2, *dp3;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 t0, t1, t2, t3, s0;
		mlib_s32 size = xsize;

		dp1 = dp + dst_i[0];
		dp2 = dp + dst_i[1];
		dp3 = dp + dst_i[2];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u8)(t0 >> 16);
			dp2[0] = (mlib_u8)(t0 >> 8);
			dp3[0] = (mlib_u8)t0;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= size - 8;
			    i += 4, dp1 += 16, dp2 += 16, dp3 += 16) {
#ifdef _LITTLE_ENDIAN
				t3 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 22) & 0x3FC));
				t2 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 14) & 0x3FC));
				t1 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 6) & 0x3FC));
				t0 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 << 2) & 0x3FC));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 22) & 0x3FC));
				t1 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 14) & 0x3FC));
				t2 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 6) & 0x3FC));
				t3 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 << 2) & 0x3FC));
#endif /* _LITTLE_ENDIAN */
				s0 = (*sa++);
				dp1[0] = (mlib_u8)(t0 >> 16);
				dp2[0] = (mlib_u8)(t0 >> 8);
				dp3[0] = (mlib_u8)t0;
				dp1[4] = (mlib_u8)(t1 >> 16);
				dp2[4] = (mlib_u8)(t1 >> 8);
				dp3[4] = (mlib_u8)t1;
				dp1[8] = (mlib_u8)(t2 >> 16);
				dp2[8] = (mlib_u8)(t2 >> 8);
				dp3[8] = (mlib_u8)t2;
				dp1[12] = (mlib_u8)(t3 >> 16);
				dp2[12] = (mlib_u8)(t3 >> 8);
				dp3[12] = (mlib_u8)t3;
			}

#ifdef _LITTLE_ENDIAN
			t3 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 22) & 0x3FC));
			t2 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 14) & 0x3FC));
			t1 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 6) & 0x3FC));
			t0 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 << 2) & 0x3FC));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 22) & 0x3FC));
			t1 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 14) & 0x3FC));
			t2 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 6) & 0x3FC));
			t3 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 << 2) & 0x3FC));
#endif /* _LITTLE_ENDIAN */
			dp1[0] = (mlib_u8)(t0 >> 16);
			dp2[0] = (mlib_u8)(t0 >> 8);
			dp3[0] = (mlib_u8)t0;
			dp1[4] = (mlib_u8)(t1 >> 16);
			dp2[4] = (mlib_u8)(t1 >> 8);
			dp3[4] = (mlib_u8)t1;
			dp1[8] = (mlib_u8)(t2 >> 16);
			dp2[8] = (mlib_u8)(t2 >> 8);
			dp3[8] = (mlib_u8)t2;
			dp1[12] = (mlib_u8)(t3 >> 16);
			dp2[12] = (mlib_u8)(t3 >> 8);
			dp3[12] = (mlib_u8)t3;
			i += 4;
			dp1 += 16;
			dp2 += 16;
			dp3 += 16;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u8)(t0 >> 16);
			dp2[0] = (mlib_u8)(t0 >> 8);
			dp3[0] = (mlib_u8)t0;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_U8_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
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
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]] + 32768;

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]] + 32768;
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_U8_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
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
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_u8 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S16_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
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
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 1;
				s1 = src[schan] << 1;
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
					s0 = src[0] << 1;
					s1 = src[schan] << 1;
					dst[0] = (mlib_s16)t0;
					dst[dchan] = (mlib_s16)t1;
				}

				t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
				dst[0] = (mlib_s16)t0;
				dst[dchan] = (mlib_s16)t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_U8_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
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
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_u8 *tab =
				    &table[table_i[k]][TABLE_SHIFT_S32];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_u8 *dst = ((mlib_u8 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_u8 *tab =
				    &table[table_i[k]][TABLE_SHIFT_S32];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S16_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
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
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 1;
				s1 = src[schan] << 1;
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
					s0 = src[0] << 1;
					s1 = src[schan] << 1;
					dst[0] = (mlib_s16)t0;
					dst[dchan] = (mlib_s16)t1;
				}

				t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
				dst[0] = (mlib_s16)t0;
				dst[dchan] = (mlib_s16)t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 3, 4; chan == 2		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_3_4_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan)
{
	mlib_u32 tab[256];
	mlib_u16 *tab0 = (mlib_u16 *)table[table_i[0]];
	mlib_u16 *tab1 = (mlib_u16 *)table[table_i[1]];
	mlib_s32 i, j;
	mlib_u32 s0, s1, s2;

	s0 = tab0[0];
	s1 = tab1[0];
	for (i = 1; i < 256; i++) {
		s2 = (s0 << 16) + s1;
		s0 = tab0[i];
		s1 = tab1[i];
		tab[i - 1] = s2;
	}

	s2 = (s0 << 16) + s1;
	tab[255] = s2;

	for (j = 0; j < ysize; j++, dst += (dlb >> 1), src += slb) {
		mlib_u32 *sa;
		mlib_u16 *dp = (mlib_u16 *)dst;
		mlib_u16 *dp1, *dp2;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 t0, t1, t2, t3, s0;
		mlib_s32 size = xsize;

		dp1 = (mlib_u16 *)dp + dst_i[0];
		dp2 = (mlib_u16 *)dp + dst_i[1];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u16)(t0 >> 16);
			dp2[0] = (mlib_u16)t0;
			dp1 += dchan;
			dp2 += dchan;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 4 * dchan, dp2 += 4 * dchan) {
#ifdef _LITTLE_ENDIAN
				t3 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 22) & 0x3FC));
				t2 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 14) & 0x3FC));
				t1 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 6) & 0x3FC));
				t0 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 << 2) & 0x3FC));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 22) & 0x3FC));
				t1 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 14) & 0x3FC));
				t2 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 >> 6) & 0x3FC));
				t3 = *(mlib_u32 *)((mlib_u8 *)tab +
				    ((s0 << 2) & 0x3FC));
#endif /* _LITTLE_ENDIAN */
				s0 = (*sa++);
				dp1[0] = (mlib_u16)(t0 >> 16);
				dp2[0] = (mlib_u16)t0;
				dp1[dchan] = (mlib_u16)(t1 >> 16);
				dp2[dchan] = (mlib_u16)t1;
				dp1[2 * dchan] = (mlib_u16)(t2 >> 16);
				dp2[2 * dchan] = (mlib_u16)t2;
				dp1[3 * dchan] = (mlib_u16)(t3 >> 16);
				dp2[3 * dchan] = (mlib_u16)t3;
			}

#ifdef _LITTLE_ENDIAN
			t3 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 22) & 0x3FC));
			t2 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 14) & 0x3FC));
			t1 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 6) & 0x3FC));
			t0 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 << 2) & 0x3FC));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 22) & 0x3FC));
			t1 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 14) & 0x3FC));
			t2 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 >> 6) & 0x3FC));
			t3 = *(mlib_u32 *)((mlib_u8 *)tab +
			    ((s0 << 2) & 0x3FC));
#endif /* _LITTLE_ENDIAN */
			dp1[0] = (mlib_u16)(t0 >> 16);
			dp2[0] = (mlib_u16)t0;
			dp1[dchan] = (mlib_u16)(t1 >> 16);
			dp2[dchan] = (mlib_u16)t1;
			dp1[2 * dchan] = (mlib_u16)(t2 >> 16);
			dp2[2 * dchan] = (mlib_u16)t2;
			dp1[3 * dchan] = (mlib_u16)(t3 >> 16);
			dp2[3 * dchan] = (mlib_u16)t3;
			i += 4;
			dp1 += 4 * dchan;
			dp2 += 4 * dchan;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab[s0];
			dp1[0] = (mlib_u16)(t0 >> 16);
			dp2[0] = (mlib_u16)t0;
			dp1 += dchan;
			dp2 += dchan;
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 4; chan == 3		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_4_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i)
{
	mlib_u32 tab[512];
	mlib_u16 *tab0 = (mlib_u16 *)table[table_i[0]];
	mlib_u16 *tab1 = (mlib_u16 *)table[table_i[1]];
	mlib_u16 *tab2 = (mlib_u16 *)table[table_i[2]];
	mlib_s32 i, j;
	mlib_u32 s0, s1, s2, s3, s4;

	s0 = tab0[0];
	s1 = tab1[0];
	s2 = tab2[0];
	for (i = 1; i < 256; i++) {
		s3 = s0;
		s4 = (s1 << 16) + s2;
		s0 = tab0[i];
		s1 = tab1[i];
		s2 = tab2[i];
		tab[2 * i - 2] = s3;
		tab[2 * i - 1] = s4;
	}

	s4 = (s1 << 16) + s2;
	tab[510] = s0;
	tab[511] = s4;

	for (j = 0; j < ysize; j++, dst += (dlb >> 1), src += slb) {
		mlib_u32 *sa;
		mlib_u16 *dp = (mlib_u16 *)dst;
		mlib_u16 *dp1, *dp2, *dp3;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 s00, s01, s02, s03, s0;
		mlib_u32 t0, t1, t2, t3;
		mlib_u32 t4, t5, t6, t7;
		mlib_s32 size = xsize;

		dp1 = (mlib_u16 *)dp + dst_i[0];
		dp2 = (mlib_u16 *)dp + dst_i[1];
		dp3 = (mlib_u16 *)dp + dst_i[2];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab[2 * s0];
			t1 = tab[2 * s0 + 1];
			dp1[0] = (mlib_u16)t0;
			dp2[0] = (mlib_u16)(t1 >> 16);
			dp3[0] = (mlib_u16)t1;
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
				t0 = *(mlib_u32 *)((mlib_u8 *)tab + s00);
				t1 = *(mlib_u32 *)((mlib_u8 *)tab + s00 + 4);
				t2 = *(mlib_u32 *)((mlib_u8 *)tab + s01);
				t3 = *(mlib_u32 *)((mlib_u8 *)tab + s01 + 4);
				t4 = *(mlib_u32 *)((mlib_u8 *)tab + s02);
				t5 = *(mlib_u32 *)((mlib_u8 *)tab + s02 + 4);
				t6 = *(mlib_u32 *)((mlib_u8 *)tab + s03);
				t7 = *(mlib_u32 *)((mlib_u8 *)tab + s03 + 4);
				s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
				s00 = (s0 << 3) & 0x7F8;
				s01 = (s0 >> 5) & 0x7F8;
#else /* _LITTLE_ENDIAN */
				s00 = (s0 >> 21) & 0x7F8;
				s01 = (s0 >> 13) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
				dp1[0] = (mlib_u16)t0;
				dp2[0] = (mlib_u16)(t1 >> 16);
				dp3[0] = (mlib_u16)t1;
				dp1[4] = (mlib_u16)t2;
				dp2[4] = (mlib_u16)(t3 >> 16);
				dp3[4] = (mlib_u16)t3;
				dp1[8] = (mlib_u16)t4;
				dp2[8] = (mlib_u16)(t5 >> 16);
				dp3[8] = (mlib_u16)t5;
				dp1[12] = (mlib_u16)t6;
				dp2[12] = (mlib_u16)(t7 >> 16);
				dp3[12] = (mlib_u16)t7;
			}

#ifdef _LITTLE_ENDIAN
			s02 = (s0 >> 13) & 0x7F8;
			s03 = (s0 >> 21) & 0x7F8;
#else /* _LITTLE_ENDIAN */
			s02 = (s0 >> 5) & 0x7F8;
			s03 = (s0 << 3) & 0x7F8;
#endif /* _LITTLE_ENDIAN */
			t0 = *(mlib_u32 *)((mlib_u8 *)tab + s00);
			t1 = *(mlib_u32 *)((mlib_u8 *)tab + s00 + 4);
			t2 = *(mlib_u32 *)((mlib_u8 *)tab + s01);
			t3 = *(mlib_u32 *)((mlib_u8 *)tab + s01 + 4);
			t4 = *(mlib_u32 *)((mlib_u8 *)tab + s02);
			t5 = *(mlib_u32 *)((mlib_u8 *)tab + s02 + 4);
			t6 = *(mlib_u32 *)((mlib_u8 *)tab + s03);
			t7 = *(mlib_u32 *)((mlib_u8 *)tab + s03 + 4);
			dp1[0] = (mlib_u16)t0;
			dp2[0] = (mlib_u16)(t1 >> 16);
			dp3[0] = (mlib_u16)t1;
			dp1[4] = (mlib_u16)t2;
			dp2[4] = (mlib_u16)(t3 >> 16);
			dp3[4] = (mlib_u16)t3;
			dp1[8] = (mlib_u16)t4;
			dp2[8] = (mlib_u16)(t5 >> 16);
			dp3[8] = (mlib_u16)t5;
			dp1[12] = (mlib_u16)t6;
			dp2[12] = (mlib_u16)(t7 >> 16);
			dp3[12] = (mlib_u16)t7;
			i += 4;
			dp1 += 16;
			dp2 += 16;
			dp3 += 16;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab[2 * s0];
			t1 = tab[2 * s0 + 1];
			dp1[0] = (mlib_u16)t0;
			dp2[0] = (mlib_u16)(t1 >> 16);
			dp3[0] = (mlib_u16)t1;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S16_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
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
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]] + 32768;

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_s16 *tab = table[table_i[k]] + 32768;
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 1;
				s1 = src[schan] << 1;
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
					s0 = src[0] << 1;
					s1 = src[schan] << 1;
					dst[0] = (mlib_s16)t0;
					dst[dchan] = (mlib_s16)t1;
				}

				t0 = *(mlib_s16 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s16 *)((mlib_addr)tab + s1);
				dst[0] = (mlib_s16)t0;
				dst[dchan] = (mlib_s16)t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S16_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
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
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_s16 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s16 *dst = ((mlib_s16 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_s16 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = (mlib_s16)t0;
					dst[dchan] = (mlib_s16)t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = (mlib_s16)t0;
				dst[dchan] = (mlib_s16)t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S32_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
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
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_u8 *src = ((mlib_u8 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 2;
				s1 = src[schan] << 2;
				src += 2 * schan;

				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
					s0 = src[0] << 2;
					s1 = src[schan] << 2;
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 3, 4; chan == 2		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_3_4_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan)
{
	TYPE_32BIT *tab0 = (TYPE_32BIT *) table[table_i[0]];
	TYPE_32BIT *tab1 = (TYPE_32BIT *) table[table_i[1]];
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++, dst += (dlb >> 2), src += slb) {
		mlib_u32 *sa;
		TYPE_32BIT *dp = (TYPE_32BIT *) dst;
		TYPE_32BIT *dp1, *dp2;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		TYPE_32BIT t0, t1, t2, t3;
		TYPE_32BIT t4, t5, t6, t7;
		mlib_u32 s00, s01, s02, s03, s0;
		mlib_s32 size = xsize;

		dp1 = (TYPE_32BIT *) dp + dst_i[0];
		dp2 = (TYPE_32BIT *) dp + dst_i[1];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab0[s0];
			t1 = tab1[s0];
			dp1[0] = t0;
			dp2[0] = t1;
			dp1 += dchan;
			dp2 += dchan;
		}

		size -= off;

		sa = (mlib_u32 *)sp;
		i = 0;

		if (size >= 4) {
			s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
			s00 = (s0 << 2) & 0x3FC;
			s01 = (s0 >> 6) & 0x3FC;
#else /* _LITTLE_ENDIAN */
			s00 = (s0 >> 22) & 0x3FC;
			s01 = (s0 >> 14) & 0x3FC;
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 4 * dchan, dp2 += 4 * dchan) {
#ifdef _LITTLE_ENDIAN
				s02 = (s0 >> 14) & 0x3FC;
				s03 = (s0 >> 22) & 0x3FC;
#else /* _LITTLE_ENDIAN */
				s02 = (s0 >> 6) & 0x3FC;
				s03 = (s0 << 2) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
				t0 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s00);
				t1 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s00);
				t2 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s01);
				t3 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s01);
				t4 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s02);
				t5 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s02);
				t6 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s03);
				t7 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s03);
				s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
				s00 = (s0 << 2) & 0x3FC;
				s01 = (s0 >> 6) & 0x3FC;
#else /* _LITTLE_ENDIAN */
				s00 = (s0 >> 22) & 0x3FC;
				s01 = (s0 >> 14) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
				dp1[0] = t0;
				dp2[0] = t1;
				dp1[dchan] = t2;
				dp2[dchan] = t3;
				dp1[2 * dchan] = t4;
				dp2[2 * dchan] = t5;
				dp1[3 * dchan] = t6;
				dp2[3 * dchan] = t7;
			}

#ifdef _LITTLE_ENDIAN
			s02 = (s0 >> 14) & 0x3FC;
			s03 = (s0 >> 22) & 0x3FC;
#else /* _LITTLE_ENDIAN */
			s02 = (s0 >> 6) & 0x3FC;
			s03 = (s0 << 2) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
			t0 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s00);
			t1 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s00);
			t2 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s01);
			t3 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s01);
			t4 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s02);
			t5 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s02);
			t6 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s03);
			t7 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s03);
			dp1[0] = t0;
			dp2[0] = t1;
			dp1[dchan] = t2;
			dp2[dchan] = t3;
			dp1[2 * dchan] = t4;
			dp2[2 * dchan] = t5;
			dp1[3 * dchan] = t6;
			dp2[3 * dchan] = t7;
			i += 4;
			dp1 += 4 * dchan;
			dp2 += 4 * dchan;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab0[s0];
			t1 = tab1[s0];
			dp1[0] = t0;
			dp2[0] = t1;
			dp1 += dchan;
			dp2 += dchan;
		}
	}
}

/* *********************************************************** */

/*		schan == 1; dchan == 4; chan == 3		*/

/* *********************************************************** */

void
mlib_ImageLookUpMask_4_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i)
{
	TYPE_32BIT *tab0 = (TYPE_32BIT *) table[table_i[0]];
	TYPE_32BIT *tab1 = (TYPE_32BIT *) table[table_i[1]];
	TYPE_32BIT *tab2 = (TYPE_32BIT *) table[table_i[2]];
	mlib_s32 i, j;

	for (j = 0; j < ysize; j++, dst += (dlb >> 2), src += slb) {
		mlib_u32 *sa;
		TYPE_32BIT *dp = (TYPE_32BIT *) dst;
		TYPE_32BIT *dp1, *dp2, *dp3;
		mlib_u8 *sp = (void *)src;
		mlib_s32 off;
		mlib_u32 s00, s01, s02, s03, s0;
		TYPE_32BIT t0, t1, t2, t3;
		TYPE_32BIT t4, t5, t6, t7;
		TYPE_32BIT t8, t9, t10, t11;
		mlib_s32 size = xsize;

		dp1 = (TYPE_32BIT *) dp + dst_i[0];
		dp2 = (TYPE_32BIT *) dp + dst_i[1];
		dp3 = (TYPE_32BIT *) dp + dst_i[2];

		off = (mlib_s32)((4 - ((mlib_addr)sp & 3)) & 3);
		off = (off < size) ? off : size;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < off; i++) {
			s0 = (*sp++);
			t0 = tab0[s0];
			t1 = tab1[s0];
			t2 = tab2[s0];
			dp1[0] = t0;
			dp2[0] = t1;
			dp3[0] = t2;
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
			s00 = (s0 << 2) & 0x3FC;
			s01 = (s0 >> 6) & 0x3FC;
#else /* _LITTLE_ENDIAN */
			s00 = (s0 >> 22) & 0x3FC;
			s01 = (s0 >> 14) & 0x3FC;
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= size - 8;
			    i += 4, dp1 += 16, dp2 += 16, dp3 += 16) {
#ifdef _LITTLE_ENDIAN
				s02 = (s0 >> 14) & 0x3FC;
				s03 = (s0 >> 22) & 0x3FC;
#else /* _LITTLE_ENDIAN */
				s02 = (s0 >> 6) & 0x3FC;
				s03 = (s0 << 2) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
				t0 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s00);
				t1 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s00);
				t2 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s00);
				t3 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s01);
				t4 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s01);
				t5 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s01);
				t6 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s02);
				t7 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s02);
				t8 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s02);
				t9 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s03);
				t10 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s03);
				t11 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s03);
				s0 = (*sa++);
#ifdef _LITTLE_ENDIAN
				s00 = (s0 << 2) & 0x3FC;
				s01 = (s0 >> 6) & 0x3FC;
#else /* _LITTLE_ENDIAN */
				s00 = (s0 >> 22) & 0x3FC;
				s01 = (s0 >> 14) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
				dp1[0] = t0;
				dp2[0] = t1;
				dp3[0] = t2;
				dp1[4] = t3;
				dp2[4] = t4;
				dp3[4] = t5;
				dp1[8] = t6;
				dp2[8] = t7;
				dp3[8] = t8;
				dp1[12] = t9;
				dp2[12] = t10;
				dp3[12] = t11;
			}

#ifdef _LITTLE_ENDIAN
			s02 = (s0 >> 14) & 0x3FC;
			s03 = (s0 >> 22) & 0x3FC;
#else /* _LITTLE_ENDIAN */
			s02 = (s0 >> 6) & 0x3FC;
			s03 = (s0 << 2) & 0x3FC;
#endif /* _LITTLE_ENDIAN */
			t0 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s00);
			t1 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s00);
			t2 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s00);
			t3 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s01);
			t4 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s01);
			t5 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s01);
			t6 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s02);
			t7 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s02);
			t8 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s02);
			t9 = *(TYPE_32BIT *) ((mlib_u8 *)tab0 + s03);
			t10 = *(TYPE_32BIT *) ((mlib_u8 *)tab1 + s03);
			t11 = *(TYPE_32BIT *) ((mlib_u8 *)tab2 + s03);
			dp1[0] = t0;
			dp2[0] = t1;
			dp3[0] = t2;
			dp1[4] = t3;
			dp2[4] = t4;
			dp3[4] = t5;
			dp1[8] = t6;
			dp2[8] = t7;
			dp3[8] = t8;
			dp1[12] = t9;
			dp2[12] = t10;
			dp3[12] = t11;
			i += 4;
			dp1 += 16;
			dp2 += 16;
			dp3 += 16;
		}

		sp = (mlib_u8 *)sa;

		for (; i < size; i++) {
			s0 = (*sp++);
			t0 = tab0[s0];
			t1 = tab1[s0];
			t2 = tab2[s0];
			dp1[0] = t0;
			dp2[0] = t1;
			dp3[0] = t2;
			dp1 += 4;
			dp2 += 4;
			dp3 += 4;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S32_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
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
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]] + 32768;

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_s16 *src = ((mlib_s16 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]] + 32768;
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 2;
				s1 = src[schan] << 2;
				src += 2 * schan;

				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
					s0 = src[0] << 2;
					s1 = src[schan] << 2;
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S32_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
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
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]];

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_u16 *src = ((mlib_u16 *)sa) + src_i[k];
				const mlib_s32 *tab = table[table_i[k]];
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0] << 2;
				s1 = src[schan] << 2;
				src += 2 * schan;

				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
					t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
					s0 = src[0] << 2;
					s1 = src[schan] << 2;
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = *(mlib_s32 *)((mlib_addr)tab + s0);
				t1 = *(mlib_s32 *)((mlib_addr)tab + s1);
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpMask_S32_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
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
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_s32 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */

				for (i = 0; i < xsize;
				    i++, dst += dchan, src += schan)
					*dst = tab[*src];
			}
		}
	} else {
		for (j = 0; j < ysize; j++, da += dlb, sa += slb) {
			for (k = 0; k < chan; k++) {
				mlib_s32 *dst = ((mlib_s32 *)da) + dst_i[k];
				mlib_s32 *src = ((mlib_s32 *)sa) + src_i[k];
				const mlib_s32 *tab =
#ifdef _MSC_VER
				    &table[table_i[k]][0];
#else		   /* _MSC_VER */
				    &table[table_i[k]][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */
				mlib_s32 s0, t0, s1, t1;

				s0 = src[0];
				s1 = src[schan];
				src += 2 * schan;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize - 3;
				    i += 2, dst += 2 * dchan, src +=
				    2 * schan) {
					t0 = tab[s0];
					t1 = tab[s1];
					s0 = src[0];
					s1 = src[schan];
					dst[0] = t0;
					dst[dchan] = t1;
				}

				t0 = tab[s0];
				t1 = tab[s1];
				dst[0] = t0;
				dst[dchan] = t1;

				if (xsize & 1)
					dst[2 * dchan] = tab[src[0]];
			}
		}
	}
}

/* *********************************************************** */
