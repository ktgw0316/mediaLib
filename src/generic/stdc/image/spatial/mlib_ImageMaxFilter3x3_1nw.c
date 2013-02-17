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

#pragma ident	"@(#)mlib_ImageMaxFilter3x3_1nw.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      Internal functions for processing of BIT images in Dilate8/Erode8
 *      and MaxFIlter3x3/MinFilter3x3.
 */

#include <mlib_image.h>
#include <mlib_ImageMaxFilter.h>

/* *********************************************************** */

#define	MAX_FILTER	1

#define	BUFF_SIZE	128

/* *********************************************************** */

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

/* *********************************************************** */

#define	TTYPE	mlib_u32
#define	TSIZE	32

#define	CONST1	((mlib_s32)0x80000000)

#define	CONSTF	((TTYPE)0xFFFFFFFF)

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* *********************************************************** */

#define	TTYPE	mlib_u64
#define	TSIZE	64

#define	CONST1	((mlib_s64)0x8000000000000000)

#define	CONSTF	((TTYPE)0xFFFFFFFFFFFFFFFF)

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* *********************************************************** */

#define	TMASK	(TSIZE - 1)
#define	BMASK	(TMASK >> 3)

#ifdef _LITTLE_ENDIAN

static const TTYPE mask_arr[] = { 0, 0xFF, 0xFFFF, 0xFFFFFF };
static const TTYPE lbits[] = {
	0x00000000, 0x80808080, 0xC0C0C0C0, 0xE0E0E0E0,
	0xF0F0F0F0, 0xF8F8F8F8, 0xFCFCFCFC, 0xFEFEFEFE, 0xFFFFFFFF
};

/* *********************************************************** */

#define	SBITS	0x01010101
#define	LBITS	0x80808080

#define	LEFT(a0, a1)	(((a0 & SBITS) >> 17) | ((a1 & SBITS) << 15) | \
				((a1 &~ SBITS) >> 1))

#define	RIGHT(a0, a1)	(((a1 & LBITS) << 17) | ((a0 & LBITS) >> 15) | \
				((a0 &~ LBITS) << 1))

/* *********************************************************** */

#define	SHIFTR(s0, sh)                                                    \
	((((s0 >> (sh & ~7)) & lbits[sh & 7]) >> (16 -                    \
	    (sh & 7))) | (((s0 >> (sh & ~7)) & ~lbits[sh & 7]) << (sh &   \
	    7)))

/* *********************************************************** */

#define	SHIFTL(s0, sh)                                          \
	((((s0 << (sh & ~7)) & ~lbits[8 - (sh & 7)]) << (16 -   \
	    (sh & 7))) | (((s0 << (sh & ~7)) & lbits[8 -        \
	    (sh & 7)]) >> (sh & 7)))

/* *********************************************************** */

#define	ALIGN(s0, s1, sh)	(SHIFTR(s0, sh) | SHIFTL(s1, (TSIZE - sh)))
#define	ALIGND(s0, s1, sh)	(SHIFTR(s0, (TSIZE - sh)) | SHIFTL(s1, sh))

/* *********************************************************** */

#define	GET_MASK(sh)                                            \
	(((TTYPE) (CONST1 >> ((sh) & 7)) >> (TSIZE - 8 -        \
	    ((sh) & ~7))) | mask_arr[(sh) >> 3])

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	LEFT(a0, a1)	((a0 << TMASK) | (a1 >> 1))
#define	RIGHT(a0, a1)	((a0 << 1) | (a1 >> TMASK))

#define	ALIGN(s0, s1, sh)	((s0 << sh) | (s1 >> (TSIZE - sh)))

#define	ALIGND(s0, s1, sh)	((s0 << (TSIZE - sh)) | (s1 >> sh))

#define	GET_MASK(sh)	((TTYPE)(CONST1 >> (sh)))

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#if MAX_FILTER
#define	C_COMP(a, b)	(a | b)
#else /* MAX_FILTER */

#define	C_COMP(a, b)	(a & b)
#endif /* MAX_FILTER */

/* *********************************************************** */

void mlib_ImageCopy_bit_al(mlib_u8 * sa, mlib_u8 * da, mlib_s32 size,
			    mlib_s32 s_offset);

void mlib_ImageCopy_bit_na(mlib_u8 * sa, mlib_u8 * da, mlib_s32 size,
			    mlib_s32 s_offset, mlib_s32 d_offset);

#define	COPY_SRC(src, pbuff, bit_off, wid)                                \
	if (bit_off == 0) {                                               \
	    mlib_ImageCopy_bit_al((void*)src, (void*)pbuff, wid + 1, 0);  \
	} else {                                                          \
	    mlib_ImageCopy_bit_na((void*)src, (void*)pbuff, wid + 1,      \
		bit_off, 0);                                              \
	}                                                                 \
	src = pbuff

/* *********************************************************** */

#define	COPY_DST(dl, dp, al, wid)                                      \
	{                                                              \
	    if (dbit_off == 7) {                                       \
		mlib_ImageCopy_bit_na((mlib_u8*)dp, (mlib_u8*)dl + 1,  \
			wid - 1, 1, 0);                                \
	    } else                                                     \
	    if (al) {                                                  \
		TTYPE *da = (TTYPE *) ((mlib_addr)dl & ~BMASK);        \
		mlib_s32 last;                                         \
	                                                               \
		s0 = ALIGN(0, da[0], al);                              \
		for (i = 0; i <= ((wid + al) / TSIZE) - 2; i += 2) {   \
		    s1 = dp[i];                                        \
		    da[i] = ALIGND(s0, s1, al);                        \
		    s0 = dp[i + 1];                                    \
		    da[i + 1] = ALIGND(s1, s0, al);                    \
		}                                                      \
		s1 = dp[i];                                            \
		last = (wid + al) - TSIZE * i;                         \
		if (last) {                                            \
		    mlib_s32 v, b;                                     \
	                                                               \
		    b = (mlib_u8 *)dl + (dbit_off + 1) / 8 -           \
			(mlib_u8 *)(da + i);                           \
		    if (b < 0)                                         \
			b = 0;                                         \
		    if (last <= TSIZE) {                               \
			TTYPE mask = GET_MASK(last - 1); {             \
			    TTYPE res0 =                               \
				(da[i] & ~mask) | (ALIGND(s0, s1,      \
				al) & mask);                           \
			    for (v = b; v < (last + 7) / 8; v++)       \
				((mlib_u8 *)(da + i))[v] =             \
				    ((mlib_u8 *)&res0)[v];             \
			}                                              \
		    } else {                                           \
			TTYPE mask = GET_MASK(last - (1 + TSIZE));     \
			TTYPE res0, res1;                              \
	                                                               \
			res0 = ALIGND(s0, s1, al);                     \
			s0 = dp[i + 1];                                \
			res1 =                                         \
			    (da[i + 1] & ~mask) | (ALIGND(s1, s0,      \
			    al) & mask);                               \
			for (v = b; v < (TSIZE >> 3); v++)             \
			    ((mlib_u8 *)(da + i))[v] =                 \
				((mlib_u8 *)&res0)[v];                 \
			for (v = 0; v < (last - TSIZE + 7) / 8; v++)   \
			    ((mlib_u8 *)(da + i + 1))[v] =             \
				((mlib_u8 *)&res1)[v];                 \
		    }                                                  \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

#if MAX_FILTER
mlib_status
mlib_ImageMaxFilter3x3_BIT(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 dbit_off,
    mlib_s32 sbit_off)
#else /* MAX_FILTER */
mlib_status
mlib_ImageMinFilter3x3_BIT(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 dbit_off,
    mlib_s32 sbit_off)
#endif		   /* MAX_FILTER */
{
	mlib_u8 *sl = (void *)src, *dl0 = (mlib_u8 *)dst + dlb;
	TTYPE *sl0, *sl1, *sl2, *sl3, *tl0, *tl1, *tl2, *tl3, *dp0, *dp1;
	TTYPE a0, a1, a2, b0, b1, b2;
	TTYPE e1, e2, e3, f1, f2, f3, g1, g2, g3, h1, h2, h3;
	TTYPE s0, s1, mask0 = 0, mask1 = 0, mask2 = 0;
	TTYPE buff_lcl[BUFF_SIZE], *buff = buff_lcl, *pbuff, *buff_arr[8];
	mlib_s32 i, j, k, buff_ind = 0, last = -1;
	mlib_s32 bsize = wid / TSIZE + 5;

/* including first bit, but without last bit */
	wid -= 1;
/* without first and last line */
	hgt -= 2;

	if ((6 * bsize) > BUFF_SIZE)
		buff = __mlib_malloc((6 * bsize) * sizeof (TTYPE));

	pbuff = buff;
	for (k = 0; k < 4; k++) {
		buff_arr[k] = pbuff;
		pbuff += bsize;
	}

	buff_arr[4] = buff_arr[0];
	buff_arr[5] = buff_arr[1];
	buff_arr[6] = pbuff;
	buff_arr[7] = pbuff + bsize;

	sl0 = (TTYPE *) sl;
	sl1 = (TTYPE *) ((mlib_u8 *)sl0 + slb);
	sl2 = (TTYPE *) ((mlib_u8 *)sl1 + slb);
	sl3 = (TTYPE *) ((mlib_u8 *)sl2 + slb);
	sl += 2 * slb;

	tl0 = buff_arr[0];
	tl1 = buff_arr[1];
	COPY_SRC(sl0, tl0, sbit_off, wid);
	COPY_SRC(sl1, tl1, sbit_off, wid);

	a0 = 0;
	a1 = sl0[0];
	b0 = 0;
	b1 = sl1[0];

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i = 0; i < (wid + TMASK) / TSIZE; i += 3) {
		a2 = sl0[i + 1];
		b2 = sl1[i + 1];
		g1 = C_COMP(a1, LEFT(a0, a1));
		g1 = C_COMP(g1, RIGHT(a1, a2));
		h1 = C_COMP(b1, LEFT(b0, b1));
		h1 = C_COMP(h1, RIGHT(b1, b2));

		a0 = sl0[i + 2];
		b0 = sl1[i + 2];
		g2 = C_COMP(a2, LEFT(a1, a2));
		g2 = C_COMP(g2, RIGHT(a2, a0));
		h2 = C_COMP(b2, LEFT(b1, b2));
		h2 = C_COMP(h2, RIGHT(b2, b0));

		a1 = sl0[i + 3];
		b1 = sl1[i + 3];
		g3 = C_COMP(a0, LEFT(a2, a0));
		g3 = C_COMP(g3, RIGHT(a0, a1));
		h3 = C_COMP(b0, LEFT(b2, b0));
		h3 = C_COMP(h3, RIGHT(b0, b1));

		tl0[i] = g1;
		tl0[i + 1] = g2;
		tl0[i + 2] = g3;
		tl1[i] = h1;
		tl1[i + 1] = h2;
		tl1[i + 2] = h3;
	}

	for (j = 0; j <= (hgt - 2); j += 2) {
		mlib_u8 *dl1 = dl0 + dlb;
		mlib_s32 d0_al = 8 * ((mlib_s32)dl0 & BMASK) + dbit_off;
		mlib_s32 d1_al = 8 * ((mlib_s32)dl1 & BMASK) + dbit_off;
		mlib_s32 d0_frst = 0, d1_frst = 0;

		if (dbit_off != 7) {
			d0_frst = (dl0[0] << dbit_off) & 0x80;
			d1_frst = (dl1[0] << dbit_off) & 0x80;
		}

		dp0 = (d0_al) ? buff_arr[6] : ((TTYPE *) dl0);
		dp1 = (d1_al) ? buff_arr[7] : ((TTYPE *) dl1);

		tl2 = buff_arr[buff_ind + 2];
		tl3 = buff_arr[buff_ind + 3];
		COPY_SRC(sl2, tl2, sbit_off, wid);
		COPY_SRC(sl3, tl3, sbit_off, wid);

/*
 * byte off:  -1 0  1  2  3
 *  line0:       e1 e2 e3
 *  line1:       f1 f2 f3
 *  line2:    a0 a1 a2 a0 a1
 *  line3:    b0 b1 b2 b0 b1
 */

		a0 = 0;
		a1 = sl2[0];
		b0 = 0;
		b1 = sl3[0];

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 0; i <= (wid / TSIZE - 3); i += 3) {
			e1 = tl0[i];
			e2 = tl0[i + 1];
			e3 = tl0[i + 2];
			f1 = tl1[i];
			f2 = tl1[i + 1];
			f3 = tl1[i + 2];

			a2 = sl2[i + 1];
			b2 = sl3[i + 1];
			g1 = C_COMP(a1, LEFT(a0, a1));
			g1 = C_COMP(g1, RIGHT(a1, a2));
			h1 = C_COMP(b1, LEFT(b0, b1));
			h1 = C_COMP(h1, RIGHT(b1, b2));
			f1 = C_COMP(f1, g1);

			a0 = sl2[i + 2];
			b0 = sl3[i + 2];
			g2 = C_COMP(a2, LEFT(a1, a2));
			g2 = C_COMP(g2, RIGHT(a2, a0));
			h2 = C_COMP(b2, LEFT(b1, b2));
			h2 = C_COMP(h2, RIGHT(b2, b0));
			f2 = C_COMP(f2, g2);

			a1 = sl2[i + 3];
			b1 = sl3[i + 3];
			g3 = C_COMP(a0, LEFT(a2, a0));
			g3 = C_COMP(g3, RIGHT(a0, a1));
			h3 = C_COMP(b0, LEFT(b2, b0));
			h3 = C_COMP(h3, RIGHT(b0, b1));
			f3 = C_COMP(f3, g3);

			tl2[i] = g1;
			tl2[i + 1] = g2;
			tl2[i + 2] = g3;
			tl3[i] = h1;
			tl3[i + 1] = h2;
			tl3[i + 2] = h3;

			dp0[i] = C_COMP(e1, f1);
			dp0[i + 1] = C_COMP(e2, f2);
			dp0[i + 2] = C_COMP(e3, f3);
			dp1[i] = C_COMP(f1, h1);
			dp1[i + 1] = C_COMP(f2, h2);
			dp1[i + 2] = C_COMP(f3, h3);
		}

		if (last < 0) {
			last = (wid - TSIZE * i);

			if (last <= TSIZE) {
				mask0 = GET_MASK(last - 1);
			} else {
				mask0 = CONSTF;

				if (last <= 2 * TSIZE) {
					mask1 = GET_MASK(last - (1 + TSIZE));
				} else {
					mask1 = CONSTF;
					mask2 =
					    GET_MASK(last - (1 + 2 * TSIZE));
				}
			}
		}

		if (last) {
			mlib_s32 v;
			TTYPE res0, res1;

			e1 = tl0[i];
			f1 = tl1[i];
			a2 = sl2[i + 1];
			b2 = sl3[i + 1];
			g1 = C_COMP(a1, LEFT(a0, a1));
			g1 = C_COMP(g1, RIGHT(a1, a2));
			h1 = C_COMP(b1, LEFT(b0, b1));
			h1 = C_COMP(h1, RIGHT(b1, b2));
			f1 = C_COMP(f1, g1);

			tl2[i] = g1;
			tl3[i] = h1;
			if (last < TSIZE) {
				res0 =
				    (dp0[i] & ~mask0) | (C_COMP(e1,
				    f1) & mask0);
				res1 =
				    (dp1[i] & ~mask0) | (C_COMP(f1,
				    h1) & mask0);
				for (v = 0; v < (last + 7) / 8; v++) {
					((mlib_u8 *)(dp0 + i))[v] =
					    ((mlib_u8 *)&res0)[v];
					((mlib_u8 *)(dp1 + i))[v] =
					    ((mlib_u8 *)&res1)[v];
				}
			} else {
				dp0[i] =
				    (dp0[i] & ~mask0) | (C_COMP(e1,
				    f1) & mask0);
				dp1[i] =
				    (dp1[i] & ~mask0) | (C_COMP(f1,
				    h1) & mask0);
			}

			if (last > TSIZE) {
				e2 = tl0[i + 1];
				f2 = tl1[i + 1];
				a0 = sl2[i + 2];
				b0 = sl3[i + 2];
				g2 = C_COMP(a2, LEFT(a1, a2));
				g2 = C_COMP(g2, RIGHT(a2, a0));
				h2 = C_COMP(b2, LEFT(b1, b2));
				h2 = C_COMP(h2, RIGHT(b2, b0));
				f2 = C_COMP(f2, g2);

				tl2[i + 1] = g2;
				tl3[i + 1] = h2;
				if (last < 2 * TSIZE) {
					res0 =
					    (dp0[i + 1] & ~mask1) | (C_COMP(e2,
					    f2) & mask1);
					res1 =
					    (dp1[i + 1] & ~mask1) | (C_COMP(f2,
					    h2) & mask1);
					for (v = 0; v < (last - TSIZE + 7) / 8;
					    v++) {
						((mlib_u8 *)(dp0 + i + 1))[v] =
						    ((mlib_u8 *)&res0)[v];
						((mlib_u8 *)(dp1 + i + 1))[v] =
						    ((mlib_u8 *)&res1)[v];
					}
				} else {
					dp0[i + 1] =
					    (dp0[i + 1] & ~mask1) | (C_COMP(e2,
					    f2) & mask1);
					dp1[i + 1] =
					    (dp1[i + 1] & ~mask1) | (C_COMP(f2,
					    h2) & mask1);
				}

				if (last > 2 * TSIZE) {
					e3 = tl0[i + 2];
					f3 = tl1[i + 2];
					a1 = sl2[i + 3];
					b1 = sl3[i + 3];
					g3 = C_COMP(a0, LEFT(a2, a0));
					g3 = C_COMP(g3, RIGHT(a0, a1));
					h3 = C_COMP(b0, LEFT(b2, b0));
					h3 = C_COMP(h3, RIGHT(b0, b1));
					f3 = C_COMP(f3, g3);

					tl2[i + 2] = g3;
					tl3[i + 2] = h3;
					res0 =
					    (dp0[i + 2] & ~mask2) | (C_COMP(e3,
					    f3) & mask2);
					res1 =
					    (dp1[i + 2] & ~mask2) | (C_COMP(f3,
					    h3) & mask2);

					for (v = 0;
					    v < (last - 2 * TSIZE + 7) / 8;
					    v++) {
						((mlib_u8 *)(dp0 + i + 2))[v] =
						    ((mlib_u8 *)&res0)[v];
						((mlib_u8 *)(dp1 + i + 2))[v] =
						    ((mlib_u8 *)&res1)[v];
					}
				}
			}
		}

		*(mlib_u8 *)dp0 = ((*(mlib_u8 *)dp0) & ~0x80) | d0_frst;
		COPY_DST(dl0, dp0, d0_al, wid);

		if (d1_al)
			d1_frst = (dl1[0] << dbit_off) & 0x80;
		*(mlib_u8 *)dp1 = ((*(mlib_u8 *)dp1) & ~0x80) | d1_frst;
		COPY_DST(dl1, dp1, d1_al, wid);

		tl0 = tl2;
		tl1 = tl3;
		sl2 = (TTYPE *) (sl += 2 * slb);
		sl3 = (TTYPE *) ((mlib_u8 *)sl2 + slb);
		dl0 += 2 * dlb;

		buff_ind = 2 - buff_ind;
	}

/* last line */
	for (; j < hgt; j++) {
		mlib_s32 d0_al = 8 * ((mlib_s32)dl0 & BMASK) + dbit_off;
		mlib_s32 d0_frst = (dl0[0] << dbit_off) & 0x80;

		dp0 = (d0_al) ? buff_arr[6] : ((TTYPE *) dl0);
		last = wid & (TSIZE - 1);

		COPY_SRC(sl2, buff_arr[buff_ind + 2], sbit_off, wid);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid / TSIZE; i++) {
			e1 = tl0[i];
			f1 = tl1[i];
			a0 = sl2[i - 1];
			a1 = sl2[i];
			a2 = sl2[i + 1];
			g1 = C_COMP(a1, LEFT(a0, a1));
			g1 = C_COMP(g1, RIGHT(a1, a2));

			f1 = C_COMP(f1, g1);
			f1 = C_COMP(f1, e1);

			dp0[i] = f1;
		}

		last = (wid - TSIZE * i);
		mask0 = GET_MASK(last - 1);

		if (last) {
			mlib_s32 v;
			TTYPE res;

			e1 = tl0[i];
			f1 = tl1[i];
			a0 = sl2[i - 1];
			a1 = sl2[i];
			a2 = sl2[i + 1];
			g1 = C_COMP(a1, LEFT(a0, a1));
			g1 = C_COMP(g1, RIGHT(a1, a2));

			f1 = C_COMP(f1, g1);
			f1 = C_COMP(f1, e1);

			res = (dp0[i] & ~mask0) | (f1 & mask0);
			for (v = 0; v < (last + 7) / 8; v++)
				((mlib_u8 *)(dp0 + i))[v] =
				    ((mlib_u8 *)&res)[v];
		}

		*(mlib_u8 *)dp0 = ((*(mlib_u8 *)dp0) & ~0x80) | d0_frst;

		COPY_DST(dl0, dp0, d0_al, wid);

		sl2 = (TTYPE *) (sl += slb);
		dl0 += dlb;

		buff_ind++;

		if (buff_ind >= 3)
			buff_ind = 0;
	}

	if (buff != buff_lcl)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
