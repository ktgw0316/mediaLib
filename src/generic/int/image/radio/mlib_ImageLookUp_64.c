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

#pragma ident	"@(#)mlib_ImageLookUp_64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp_U8D64 - table lookup
 *      mlib_ImageLookUp_S16D64 - table lookup
 *      mlib_ImageLookUp_U16D64 - table lookup
 *      mlib_ImageLookUp_S32D64 - table lookup
 *
 * SYNOPSIS
 *      void mlib_ImageLookUp_U8_D64(src, slb,
 *                                   dst, dlb,
 *                                   xsize, ysize,
 *                                   csize, table)
 *
 *      void mlib_ImageLookUp_S16_D64(src, slb,
 *                                    dst, dlb,
 *                                    xsize, ysize,
 *                                    csize, table)
 *
 *      void mlib_ImageLookUp_U16_D64(src, slb,
 *                                    dst, dlb,
 *                                    xsize, ysize,
 *                                    csize, table)
 *
 *      void mlib_ImageLookUp_S32_D64(src, slb,
 *                                    dst, dlb,
 *                                    xsize, ysize,
 *                                    csize, table)
 *
 * ARGUMENT
 *      src     pointer to input image (BYTE, SHORT, USHORT, INT)
 *      slb     stride of input image (in pixels)
 *      dst     pointer to output image (DOUBLE)
 *      dlb     stride of output image (in pixels)
 *      xsize   image width
 *      ysize   image height
 *      csize   number of channels
 *      table   lookup table
 *
 * DESCRIPTION
 *      dst = table[src] (c, vis version)
 */

#include <mlib_image.h>
#include <mlib_ImageLookUp.h>

/* *********************************************************** */
#define	MLIB_C_IMAGELOOKUP(DTYPE, STYPE, TABLE)               \
	{                                                         \
	    mlib_s32 i, j, k;                                     \
	                                                          \
	    if (xsize < 2) {                                      \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			const STYPE *sa = src + k;                        \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
	                                                          \
			for (i = 0; i < xsize;                            \
			    i++, da += csize, sa += csize)                \
			    *((mlib_s64 *)da) = *((mlib_s64 *) &tab[*sa]);\
		    }                                                 \
		}                                                     \
	    } else {                                              \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			const STYPE *sa = src + k;                        \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
			mlib_s32 s0, s1;                                  \
			mlib_s64 t0, t1;                                  \
	                                                          \
			s0 = (mlib_s32)sa[0];                             \
			s1 = (mlib_s32)sa[csize];                         \
			sa += 2 * csize;                                  \
			for (i = 0; i < xsize - 3;                        \
			    i += 2, da += 2 * csize, sa += 2 * csize) {   \
			    t0 = *((mlib_s64 *) &tab[s0]);                \
			    t1 = *((mlib_s64 *) &tab[s1]);                \
			    s0 = (mlib_s32)sa[0];                         \
			    s1 = (mlib_s32)sa[csize];                     \
			    *((mlib_s64 *) &da[0]) = t0;                  \
			    *((mlib_s64 *) &da[csize]) = t1;              \
			}                                                 \
			t0 = *((mlib_s64 *) &tab[s0]);                    \
			t1 = *((mlib_s64 *) &tab[s1]);                    \
			*((mlib_s64 *) &da[0]) = t0;                      \
			*((mlib_s64 *) &da[csize]) = t1;                  \
			if (xsize & 1)                                    \
			    *((mlib_s64 *) &da[2 * csize]) =              \
					*((mlib_s64 *) &tab[sa[0]]);\
		    }                                                 \
		}                                                     \
	    }                                                     \
	}
/* *********************************************************** */
#define	MLIB_C_IMAGELOOKUPSI(DTYPE, STYPE, TABLE)             \
	{                                                         \
	    mlib_s32 i, j, k;                                     \
	                                                          \
	    if (xsize < 2) {                                      \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			const STYPE *sa = (void *)src;                    \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
	                                                          \
			for (i = 0; i < xsize; i++, da += csize, sa++)    \
			    *((mlib_s64 *)da) = *((mlib_s64 *) &tab[*sa]);\
		    }                                                 \
		}                                                     \
	    } else {                                              \
		for (j = 0; j < ysize; j++, dst += dlb, src += slb) { \
		    for (k = 0; k < csize; k++) {                     \
			DTYPE *da = dst + k;                              \
			const STYPE *sa = (void *)src;                    \
			DTYPE *tab = (DTYPE *) TABLE[k];                  \
			mlib_s32 s0, s1;                                  \
			mlib_s64 t0, t1;                                  \
	                                                          \
			s0 = (mlib_s32)sa[0];                             \
			s1 = (mlib_s32)sa[1];                             \
			sa += 2;                                          \
			for (i = 0; i < xsize - 3;                        \
			    i += 2, da += 2 * csize, sa += 2) {           \
			    t0 = *((mlib_s64 *) &tab[s0]);                \
			    t1 = *((mlib_s64 *) &tab[s1]);                \
			    s0 = (mlib_s32)sa[0];                         \
			    s1 = (mlib_s32)sa[1];                         \
			    *((mlib_s64 *) &da[0]) = t0;                  \
			    *((mlib_s64 *) &da[csize]) = t1;              \
			}                                                 \
			t0 = *((mlib_s64 *) &tab[s0]);                    \
			t1 = *((mlib_s64 *) &tab[s1]);                    \
			*((mlib_s64 *) &da[0]) = t0;                      \
			*((mlib_s64 *) &da[csize]) = t1;                  \
			if (xsize & 1)                                    \
			    *((mlib_s64 *) &da[2 * csize]) =              \
					*((mlib_s64 *) &tab[sa[0]]);\
		    }                                                 \
		}                                                     \
	    }                                                     \
	}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */
#define	READ_U8_D64(table0, table1, table2, table3)                     \
	t0 = *(mlib_s64 *)((mlib_u8 *)table0 + ((s0 << 3) & 0x7F8));    \
	t1 = *(mlib_s64 *)((mlib_u8 *)table1 + ((s0 >> 5) & 0x7F8));    \
	t2 = *(mlib_s64 *)((mlib_u8 *)table2 + ((s0 >> 13) & 0x7F8));   \
	t3 = *(mlib_s64 *)((mlib_u8 *)table3 + ((s0 >> 21) & 0x7F8))

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	READ_U8_D64(table0, table1, table2, table3)                     \
	t0 = *(mlib_s64 *)((mlib_u8 *)table0 + ((s0 >> 21) & 0x7F8));   \
	t1 = *(mlib_s64 *)((mlib_u8 *)table1 + ((s0 >> 13) & 0x7F8));   \
	t2 = *(mlib_s64 *)((mlib_u8 *)table2 + ((s0 >> 5) & 0x7F8));    \
	t3 = *(mlib_s64 *)((mlib_u8 *)table3 + ((s0 << 3) & 0x7F8))

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_ImageLookUp_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	if (xsize * csize < 7) {
		MLIB_C_IMAGELOOKUP(mlib_d64,
		    mlib_u8,
		    table);
	} else if (csize == 1) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab = (mlib_d64 *)table[0];
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;
			mlib_s32 off;
			mlib_s32 size = xsize;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			for (i = 0; i < off; i++, sp++) {
				(*dp++) = tab[sp[0]];
				size--;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 4, sa++) {
				READ_U8_D64(tab, tab, tab, tab);
				s0 = sa[0];
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;
			}

			READ_U8_D64(tab, tab, tab, tab);
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;
			dp += 4;
			sp = (mlib_u8 *)sa;
			i += 4;
			for (; i < size; i++, dp++, sp++)
				dp[0] = tab[sp[0]];
		}
	} else if (csize == 2) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_d64 *tab;
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;

			mlib_s32 off;
			mlib_s32 size = xsize * 2;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			for (i = 0; i < off - 1; i += 2, sp += 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				size -= 2;
			}

			if ((off & 1) != 0) {
				(*dp++) = tab0[*sp];
				size--;
				sp++;
				tab = tab0;
				tab0 = tab1;
				tab1 = tab;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 4, sa++) {
				READ_U8_D64(tab0, tab1, tab0, tab1);
				s0 = sa[0];
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;

			}

			READ_U8_D64(tab0, tab1, tab0, tab1);

			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;

			dp += 4;
			sp = (mlib_u8 *)sa;
			i += 4;

			for (; i < size - 1; i += 2, sp += 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
			}

			if (i < size)
				*dp = tab0[(*sp)];
		}
	} else if (csize == 3) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_d64 *tab2 = (mlib_d64 *)table[2];
			mlib_d64 *tab;
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;
			mlib_s32 off;
			mlib_s32 size = xsize * 3;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			if (off == 1) {
				(*dp++) = tab0[(*sp)];
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab;
				size--;
				sp++;
			} else if (off == 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				tab = tab2;
				tab2 = tab1;
				tab1 = tab0;
				tab0 = tab;
				size -= 2;
				sp += 2;
			} else if (off == 3) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				(*dp++) = tab2[sp[2]];
				size -= 3;
				sp += 3;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 4, sa++) {
				READ_U8_D64(tab0, tab1, tab2, tab0);
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab;
				s0 = sa[0];
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;

			}

			READ_U8_D64(tab0, tab1, tab2, tab0);
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;

			dp += 4;
			sp = (mlib_u8 *)sa;
			i += 4;

			if (i < size) {
				(*dp++) = tab1[(*sp)];
				i++;
				sp++;
			}

			if (i < size) {
				(*dp++) = tab2[(*sp)];
				i++;
				sp++;
			}

			if (i < size) {
				*dp = tab0[(*sp)];
			}
		}
	} else if (csize == 4) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_d64 *tab2 = (mlib_d64 *)table[2];
			mlib_d64 *tab3 = (mlib_d64 *)table[3];
			mlib_d64 *tab;
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;
			mlib_s32 off;
			mlib_s32 size = xsize * 4;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			if (off == 1) {
				(*dp++) = tab0[(*sp)];
				tab = tab0;
				tab0 = tab1;
				tab1 = tab2;
				tab2 = tab3;
				tab3 = tab;
				size--;
				sp++;
			} else if (off == 2) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				tab = tab0;
				tab0 = tab2;
				tab2 = tab;
				tab = tab1;
				tab1 = tab3;
				tab3 = tab;
				size -= 2;
				sp += 2;
			} else if (off == 3) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[1]];
				(*dp++) = tab2[sp[2]];
				tab = tab3;
				tab3 = tab2;
				tab2 = tab1;
				tab1 = tab0;
				tab0 = tab;
				size -= 3;
				sp += 3;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 4, sa++) {
				READ_U8_D64(tab0, tab1, tab2, tab3);
				s0 = sa[0];
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;

			}

			READ_U8_D64(tab0, tab1, tab2, tab3);
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;

			dp += 4;
			sp = (mlib_u8 *)sa;
			i += 4;

			if (i < size) {
				(*dp++) = tab0[(*sp)];
				i++;
				sp++;
			}

			if (i < size) {
				(*dp++) = tab1[(*sp)];
				i++;
				sp++;
			}

			if (i < size) {
				*dp = tab2[(*sp)];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUp_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
		table_base[c] = &table[c][32768];
	}

	MLIB_C_IMAGELOOKUP(mlib_d64,
	    mlib_s16,
	    table_base);
}

/* *********************************************************** */

void
mlib_ImageLookUp_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
		table_base[c] = &table[c][0];
	}

	MLIB_C_IMAGELOOKUP(mlib_d64,
	    mlib_u16,
	    table_base);
}

/* *********************************************************** */

void
mlib_ImageLookUp_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
#ifdef _MSC_VER
		table_base[c] = &table[c][0];
#else		   /* _MSC_VER */
		table_base[c] = &table[c][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */
	}

	MLIB_C_IMAGELOOKUP(mlib_d64,
	    mlib_s32,
	    table_base);
}

/* *********************************************************** */

void
mlib_ImageLookUpSI_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	if (xsize < 7) {
		MLIB_C_IMAGELOOKUPSI(mlib_d64,
		    mlib_u8,
		    table);
	} else if (csize == 2) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;
			mlib_s32 off;
			mlib_s32 size = xsize;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			for (i = 0; i < off; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
				size--;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 8, sa++) {
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				s0 = sa[0];
				*((mlib_s64 *) &dp[4]) = t0;
				*((mlib_s64 *) &dp[5]) = t1;
				*((mlib_s64 *) &dp[6]) = t2;
				*((mlib_s64 *) &dp[7]) = t3;
			}

#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;
#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[4]) = t0;
			*((mlib_s64 *) &dp[5]) = t1;
			*((mlib_s64 *) &dp[6]) = t2;
			*((mlib_s64 *) &dp[7]) = t3;

			dp += 8;
			sp = (mlib_u8 *)sa;
			i += 4;

			for (; i < size; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
			}
		}
	} else if (csize == 3) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_d64 *tab2 = (mlib_d64 *)table[2];
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3, t4, t5;
			mlib_s32 off;
			mlib_s32 size = xsize;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			for (i = 0; i < off; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
				(*dp++) = tab2[sp[0]];
				size--;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 12, sa++) {
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 << 3) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
				t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 21) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
				t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;
				*((mlib_s64 *) &dp[4]) = t4;
				*((mlib_s64 *) &dp[5]) = t5;
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 13) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
				t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 5) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
				t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				s0 = sa[0];
				*((mlib_s64 *) &dp[6]) = t0;
				*((mlib_s64 *) &dp[7]) = t1;
				*((mlib_s64 *) &dp[8]) = t2;
				*((mlib_s64 *) &dp[9]) = t3;
				*((mlib_s64 *) &dp[10]) = t4;
				*((mlib_s64 *) &dp[11]) = t5;
			}

#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
			t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 21) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
			t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;
			*((mlib_s64 *) &dp[4]) = t4;
			*((mlib_s64 *) &dp[5]) = t5;
#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 13) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
			t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t4 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
			t5 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[6]) = t0;
			*((mlib_s64 *) &dp[7]) = t1;
			*((mlib_s64 *) &dp[8]) = t2;
			*((mlib_s64 *) &dp[9]) = t3;
			*((mlib_s64 *) &dp[10]) = t4;
			*((mlib_s64 *) &dp[11]) = t5;
			dp += 12;
			sp = (mlib_u8 *)sa;
			i += 4;

			for (; i < size; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
				(*dp++) = tab2[sp[0]];
			}
		}
	} else if (csize == 4) {
		mlib_s32 i, j;

		for (j = 0; j < ysize; j++, dst += dlb, src += slb) {
			mlib_u32 *sa;
			mlib_d64 *tab0 = (mlib_d64 *)table[0];
			mlib_d64 *tab1 = (mlib_d64 *)table[1];
			mlib_d64 *tab2 = (mlib_d64 *)table[2];
			mlib_d64 *tab3 = (mlib_d64 *)table[3];
			mlib_u32 s0;
			mlib_s64 t0, t1, t2, t3;
			mlib_s32 off;
			mlib_s32 size = xsize;
			mlib_d64 *dp = (mlib_d64 *)dst;
			mlib_u8 *sp = (void *)src;

			off = (mlib_s32)((4 - ((mlib_addr)src & 3)) & 3);

			for (i = 0; i < off; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
				(*dp++) = tab2[sp[0]];
				(*dp++) = tab3[sp[0]];
				size--;
			}

			sa = (mlib_u32 *)sp;

			s0 = sa[0];
			sa++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size - 7; i += 4, dp += 16, sa++) {
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 << 3) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 << 3) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 21) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 21) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp[0]) = t0;
				*((mlib_s64 *) &dp[1]) = t1;
				*((mlib_s64 *) &dp[2]) = t2;
				*((mlib_s64 *) &dp[3]) = t3;
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 5) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 13) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp[4]) = t0;
				*((mlib_s64 *) &dp[5]) = t1;
				*((mlib_s64 *) &dp[6]) = t2;
				*((mlib_s64 *) &dp[7]) = t3;
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 13) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 13) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 13) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 13) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 5) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 5) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 5) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 5) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				*((mlib_s64 *) &dp[8]) = t0;
				*((mlib_s64 *) &dp[9]) = t1;
				*((mlib_s64 *) &dp[10]) = t2;
				*((mlib_s64 *) &dp[11]) = t3;
#ifdef _LITTLE_ENDIAN
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 >> 21) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 >> 21) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 >> 21) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
				t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
				    ((s0 << 3) & 0x7F8));
				t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
				    ((s0 << 3) & 0x7F8));
				t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
				    ((s0 << 3) & 0x7F8));
				t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
				    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
				s0 = sa[0];
				*((mlib_s64 *) &dp[12]) = t0;
				*((mlib_s64 *) &dp[13]) = t1;
				*((mlib_s64 *) &dp[14]) = t2;
				*((mlib_s64 *) &dp[15]) = t3;
			}

#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 << 3) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 21) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 21) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[0]) = t0;
			*((mlib_s64 *) &dp[1]) = t1;
			*((mlib_s64 *) &dp[2]) = t2;
			*((mlib_s64 *) &dp[3]) = t3;
#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 5) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 13) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 13) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[4]) = t0;
			*((mlib_s64 *) &dp[5]) = t1;
			*((mlib_s64 *) &dp[6]) = t2;
			*((mlib_s64 *) &dp[7]) = t3;
#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 13) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 13) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 13) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 13) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 5) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 5) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 5) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 5) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[8]) = t0;
			*((mlib_s64 *) &dp[9]) = t1;
			*((mlib_s64 *) &dp[10]) = t2;
			*((mlib_s64 *) &dp[11]) = t3;
#ifdef _LITTLE_ENDIAN
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 >> 21) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 >> 21) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 >> 21) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 >> 21) & 0x7F8));
#else /* _LITTLE_ENDIAN */
			t0 = *(mlib_s64 *)((mlib_u8 *)tab0 +
			    ((s0 << 3) & 0x7F8));
			t1 = *(mlib_s64 *)((mlib_u8 *)tab1 +
			    ((s0 << 3) & 0x7F8));
			t2 = *(mlib_s64 *)((mlib_u8 *)tab2 +
			    ((s0 << 3) & 0x7F8));
			t3 = *(mlib_s64 *)((mlib_u8 *)tab3 +
			    ((s0 << 3) & 0x7F8));
#endif /* _LITTLE_ENDIAN */
			*((mlib_s64 *) &dp[12]) = t0;
			*((mlib_s64 *) &dp[13]) = t1;
			*((mlib_s64 *) &dp[14]) = t2;
			*((mlib_s64 *) &dp[15]) = t3;
			dp += 16;
			sp = (mlib_u8 *)sa;
			i += 4;

			for (; i < size; i++, sp++) {
				(*dp++) = tab0[sp[0]];
				(*dp++) = tab1[sp[0]];
				(*dp++) = tab2[sp[0]];
				(*dp++) = tab3[sp[0]];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageLookUpSI_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
		table_base[c] = &table[c][32768];
	}

	MLIB_C_IMAGELOOKUPSI(mlib_d64,
	    mlib_s16,
	    table_base);
}

/* *********************************************************** */

void
mlib_ImageLookUpSI_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
		table_base[c] = &table[c][0];
	}

	MLIB_C_IMAGELOOKUPSI(mlib_d64,
	    mlib_u16,
	    table_base);
}

/* *********************************************************** */

void
mlib_ImageLookUpSI_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table)
{
	const mlib_d64 *table_base[4];
	mlib_s32 c;

	for (c = 0; c < csize; c++) {
#ifdef _MSC_VER
		table_base[c] = &table[c][0];
#else		   /* _MSC_VER */
		table_base[c] = &table[c][TABLE_SHIFT_S32];
#endif		   /* _MSC_VER */
	}

	MLIB_C_IMAGELOOKUPSI(mlib_d64,
	    mlib_s32,
	    table_base);
}

/* *********************************************************** */
