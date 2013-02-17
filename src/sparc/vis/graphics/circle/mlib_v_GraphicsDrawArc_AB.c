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

#pragma ident	"@(#)mlib_v_GraphicsDrawArc_AB.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsDrawArc_AB_[8 | 32] - draw arc
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsDrawArc_AB_8 (mlib_image *buffer,
 *                                            mlib_s16   x,
 *                                            mlib_s16   y,
 *                                            mlib_s32   r,
 *                                            mlib_f32   t1,
 *                                            mlib_f32   t2,
 *                                            mlib_s32   c,
 *                                            mlib_s32   a)
 *      mlib_status mlib_GraphicsDrawArc_AB_32(mlib_image *buffer,
 *                                            mlib_s16   x,
 *                                            mlib_s16   y,
 *                                            mlib_s32   r,
 *                                            mlib_f32   t1,
 *                                            mlib_f32   t2,
 *                                            mlib_s32   c,
 *                                            mlib_s32   a)
 *
 * ARGUMENTS
 *      buffer  pointer to an dst image
 *      x, y     center point coords
 *      r       radius
 *      t1, t2  start and end angles in radians
 *      c       color
 *      c       alpha
 *
 * DESCRIPTION
 *      Function draws antialiased circle arc with radius r in image pointed
 *      by buffer with color c and center in point x, y.
 *      If abs(t1 - t2) >= 2 * PI, result is circle.
 *      If t1 > t2 t2 = t2 + 2 * PI is used.
 */

#include <vis_proto.h>
#include <mlib_SysMath.h>
#include <mlib_graphics.h>
#include <mlib_v_GraphicsTabAlias.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawArc_AB_32 = __mlib_GraphicsDrawArc_AB_32
#pragma weak mlib_GraphicsDrawArc_AB_8 = __mlib_GraphicsDrawArc_AB_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawArc_AB_32) mlib_GraphicsDrawArc_AB_32
    __attribute__((weak, alias("__mlib_GraphicsDrawArc_AB_32")));
__typeof__(__mlib_GraphicsDrawArc_AB_8) mlib_GraphicsDrawArc_AB_8
    __attribute__((weak, alias("__mlib_GraphicsDrawArc_AB_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PIx2	6.28318530717958647692f
#define	PI_4	0.78539816339744830962f
#define	BUFSIZE	256
#define	RADMAX	350
#define	INSIDEX(X)	((X) <= width && (X) >= 0)
#define	INSIDEY(Y)	((Y) <= height && (Y) >= 0)
#define	INSIDE(X, Y)	(INSIDEX(X) && INSIDEY(Y))

/* *********************************************************** */

#define	CHECK_INTERSECTION                                             \
	if (x - r > width + 1 || x + r < -1 || y - r > height + 1 ||   \
	    y + r < -1)                                                \
	    return MLIB_SUCCESS

/* *********************************************************** */

typedef struct tag_pinfo
{
	mlib_s32 cy;
	mlib_s32 m;
	mlib_d64 c[3];
} pinfo;

/* *********************************************************** */

#define	FILL_BUF                                                    \
	buf[0].cy = r;                                              \
	buf[0].m = 0;                                               \
	buf[0].c[0] = ((mlib_d64 *)mlib_v_TabAlias)[0];             \
	buf[0].c[1] = ((mlib_d64 *)mlib_v_TabAlias)[255];           \
	buf[0].c[2] =                                               \
	    ((mlib_d64 *)mlib_v_TabAlias)[(r == 1) ? 370 : 255];    \
	cy = r;                                                     \
	cx = 1;                                                     \
	del = 1 - r;                                                \
	mask = 0;                                                   \
	while (cx < cy - 1) {                                       \
	    mdel = (del ^ ~mask) - ~mask;                           \
	    ind0 = cy - mdel;                                       \
	    ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);            \
	    ind1 = cy + del + ((cy * 2) & ~mask);                   \
	    ind2 = cy - del + ((cy * 2) & mask);                    \
	    ind0 = (ind0 * k) >> 13;                                \
	    ind1 = (ind1 * k) >> 13;                                \
	    ind2 = (ind2 * k) >> 13;                                \
	    buf[cx].cy = cy;                                        \
	    buf[cx].m = mask;                                       \
	    c0 = ((mlib_d64 *)mlib_v_TabAlias)[ind0];               \
	    c1 = ((mlib_d64 *)mlib_v_TabAlias)[ind1];               \
	    c2 = ((mlib_d64 *)mlib_v_TabAlias)[ind2];               \
	    buf[cx].c[0] = c0;                                      \
	    buf[cx].c[1] = c1;                                      \
	    buf[cx].c[2] = c2;                                      \
	    del += (2 * cx + 1) - ((2 * cy) & mask);                \
	    mask = ~(del >> 31);                                    \
	    cy += mask;                                             \
	    ++cx;                                                   \
	}                                                           \
	                                                            \
	mdel = (del ^ ~mask) - ~mask;                               \
	ind0 = cy - mdel;                                           \
	ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);                \
	ind1 = cy + del + ((cy * 2) & ~mask);                       \
	ind2 = cy - del + ((cy * 2) & mask);                        \
	ind0 = (ind0 * k) >> 13;                                    \
	ind1 = (ind1 * k) >> 13;                                    \
	ind2 = (ind2 * k) >> 13;                                    \
	buf[cx].cy = cy;                                            \
	buf[cx].m = mask;                                           \
	c0 = ((mlib_d64 *)mlib_v_TabAlias)[ind0];                   \
	c1 = ((mlib_d64 *)mlib_v_TabAlias)[ind1];                   \
	c2 = ((mlib_d64 *)mlib_v_TabAlias)[ind2];                   \
	buf[cx].c[0] = c0;                                          \
	buf[cx].c[1] = c1;                                          \
	buf[cx].c[2] = c2;                                          \
	scount = (cx < cy) ? cx : cx - 1;                           \
	count = cx

/* *********************************************************** */

#define	GET_BORDERS                                             \
	{                                                       \
	    mlib_f32 fs1, fc1, fs2, fc2, fr;                    \
	    mlib_s32 sum;                                       \
	                                                        \
	    fr = (mlib_f32)r;                                   \
	    mlib_sincosf(t1, &fs1, &fc1);                       \
	    mlib_sincosf(t2, &fs2, &fc2);                       \
	    sin1 = mlib_fabs(fr * fs1) + 0.5f;                  \
	    cos1 = mlib_fabs(fr * fc1) + 0.5f;                  \
	    sin2 = mlib_fabs(fr * fs2) + 0.5f;                  \
	    cos2 = mlib_fabs(fr * fc2) + 0.5f;                  \
	    oct1 = t1 / (PI_4);                                 \
	    oct2 = t2 / (PI_4);                                 \
	    if (t1 < 0) {                                       \
		oct1 -= 1;                                      \
		if (t2 < 0)                                     \
		    oct2 -= 1;                                  \
	    }                                                   \
	    oct2 -= (oct1 & ~7);                                \
	    oct1 &= 7;                                          \
	    if (cos1 == sin1 && cos1 != count)                  \
		oct1 = (oct1 & ~1) + 1;                         \
	    if (cos1 > sin1)                                    \
		sum = buf[sin1].cy + sin1;                      \
	    else                                                \
		sum = buf[cos1].cy + cos1;                      \
	    if (cos1 + sin1 > sum) {                            \
		if (oct1 & 2)                                   \
		    sin1 -= 1;                                  \
		else                                            \
		    cos1 -= 1;                                  \
	    } else if (cos1 + sin1 < sum) {                     \
		if (oct1 & 2)                                   \
		    cos1 += 1;                                  \
		else                                            \
		    sin1 += 1;                                  \
	    }                                                   \
	    if (cos2 == sin2 && cos2 != count)                  \
		oct2 = (oct2 & ~1);                             \
	    if (cos2 > sin2)                                    \
		sum = buf[sin2].cy + sin2;                      \
	    else                                                \
		sum = buf[cos2].cy + cos2;                      \
	    if (cos2 + sin2 > sum) {                            \
		if (oct2 & 2)                                   \
		    cos2 -= 1;                                  \
		else                                            \
		    sin2 -= 1;                                  \
	    } else if (cos2 + sin2 < sum) {                     \
		if (oct2 & 2)                                   \
		    sin2 += 1;                                  \
		else                                            \
		    cos2 += 1;                                  \
	    }                                                   \
	}                                                       \
	                                                        \
	if (sin1 == 0) {                                        \
	    if (oct1 == 0) {                                    \
		oct1 = 7;                                       \
		oct2 += 8;                                      \
	    } else if (oct1 == 3) {                             \
		oct1 = 4;                                       \
	    }                                                   \
	} else if (cos1 == 0) {                                 \
	    if (oct1 == 1) {                                    \
		oct1 = 2;                                       \
	    } else if (oct1 == 6) {                             \
		oct1 = 5;                                       \
	    }                                                   \
	}                                                       \
	if (sin2 == 0) {                                        \
	    if (oct2 == 0) {                                    \
		oct2 = 7;                                       \
		oct1 += 8;                                      \
	    } else if (oct2 == 8) {                             \
		oct2 = 7;                                       \
	    } else if ((oct2 & 7) == 3) {                       \
		oct2 += 1;                                      \
	    }                                                   \
	} else if (cos2 == 0) {                                 \
	    if ((oct2 & 7) == 1) {                              \
		oct2 += 1;                                      \
	    } else if ((oct2 & 7) == 6) {                       \
		oct2 -= 1;                                      \
	    }                                                   \
	}                                                       \
	if (scount < count) {                                   \
	    if (cos1 == sin1) {                                 \
		if (oct1 == 0)                                  \
		    oct1 = 1;                                   \
		else if (oct1 == 3)                             \
		    oct1 = 2;                                   \
		else if (oct1 == 4)                             \
		    oct1 = 5;                                   \
		else if (oct1 == 7)                             \
		    oct1 = 6;                                   \
	    }                                                   \
	    if (cos2 == sin2) {                                 \
		if ((oct2 & 7) == 0)                            \
		    oct2 += 1;                                  \
		else if ((oct2 & 7) == 3)                       \
		    oct2 -= 1;                                  \
		else if ((oct2 & 7) == 4)                       \
		    oct2 += 1;                                  \
		else if ((oct2 & 7) == 7)                       \
		    oct2 -= 1;                                  \
	    }                                                   \
	}

/* *********************************************************** */

#define	FILL_FLAGS                                                 \
	flagc = ((-2) << oct1) & ~(((-1) << oct2));                \
	flagd = ((-1) << oct1) & ~((-2) << oct2);                  \
	flagc |= (flagc >> 8);                                     \
	flagd |= (flagd >> 8);                                     \
	if (sin1 == sin2 && cos1 == cos2 && (oct1 + 8 == oct2 ||   \
	    (oct1 + 7 <= oct2 && (sin1 == 0 || cos1 == 0))))       \
	    flagc = 0xff

/* *********************************************************** */

#define	FILL_CL_FLAGS                                           \
	if (y < r) {                                            \
	    if (y < count - 1)                                  \
		flagd &= ~0x06;                                 \
	    if (y < 0)                                          \
		flagd &= ~0x09;                                 \
	    if (y < -count)                                     \
		flagd &= ~0x90;                                 \
	}                                                       \
	if (height - y < r) {                                   \
	    if (height - y < count - 1)                         \
		flagd &= ~0x60;                                 \
	    if (height - y < 0)                                 \
		flagd &= ~0x90;                                 \
	    if (height - y < -count)                            \
		flagd &= ~0x09;                                 \
	}                                                       \
	if (x < r) {                                            \
	    if (x < count)                                      \
		flagd &= ~0x18;                                 \
	    if (x < 0)                                          \
		flagd &= ~0x24;                                 \
	    if (x < -count - 1)                                 \
		flagd &= ~0x42;                                 \
	}                                                       \
	if (width - x < r) {                                    \
	    if (width - x < count)                              \
		flagd &= ~0x81;                                 \
	    if (width - x < 0)                                  \
		flagd &= ~0x42;                                 \
	    if (width - x < -count - 1)                         \
		flagd &= ~0x24;                                 \
	}

/* *********************************************************** */

#define	CALC_CCOEF(coef)                                        \
	vis_fmul8x16(cf, coef);                                 \
	coef = vis_fpsub16(d_one, coef)

/* *********************************************************** */

#define	PRF32(X)	((mlib_s32 *)&X)

/* *********************************************************** */

#define	BLE8(DATA, X)                                                    \
	{                                                                \
	    mlib_d64 d0 =                                                \
		vis_fpadd16(vis_fmul8x16(vis_read_lo(vis_ld_u8_i(DATA,   \
		X)), da1), vis_fmul8x16(fa, dc));                        \
	    vis_st_u8_i(d0, DATA, X);                                    \
	}

/* *********************************************************** */

#define	BLE32(DATA, X)                                          \
	{                                                       \
	    mlib_f32 f_bg = *(mlib_f32 *)(DATA + X);            \
	    mlib_d64 m0 = vis_fmul8x16(f_bg, da1);              \
	    mlib_d64 m1 = vis_fmul8x16(fa, dc);                 \
	    mlib_d64 m2 = vis_fpadd16(m0, m1);                  \
	    mlib_f32 m3 = vis_fors(falpha, vis_fpack16(m2));    \
	                                                        \
	    *(mlib_f32 *)(DATA + X) = m3;                       \
	}

/* *********************************************************** */

#define	PCOMP8(v, cof, ac)                                      \
	vis_fpack16_to_lo(v, vis_fpadd16(ac,                    \
	    vis_fmul8x16(vis_read_lo(v), cof)))

/* *********************************************************** */

#define	BLE8C(d_bg, d_c0)                                       \
	vis_fpadd16(vis_fmul8x16(vis_read_lo(d_bg), da1),       \
	    vis_fmul8x16(fa, d_c0))

/* *********************************************************** */

#define	PCALC8(p0, i0, bi)                                      \
	{                                                       \
	    mlib_d64 v0, c0, adc;                               \
	                                                        \
	    c0 = buf[bi].c[i0];                                 \
	    adc = CALC_CCOEF(c0);                               \
	    v0 = vis_ld_u8(p0);                                 \
	    vis_st_u8(BLE8C(v0, PCOMP8(v0, c0, adc)), p0);      \
	}

/* *********************************************************** */

#define	PCALC3_8(p1, p0, p2, i1, i0, i2, bi)                    \
	{                                                       \
	    mlib_d64 v1, v0, v2, c1, c0, c2, a1, a0, a2;        \
	                                                        \
	    c1 = buf[bi].c[i1];                                 \
	    a1 = CALC_CCOEF(c1);                                \
	    c0 = buf[bi].c[i0];                                 \
	    a0 = CALC_CCOEF(c0);                                \
	    c2 = buf[bi].c[i2];                                 \
	    a2 = CALC_CCOEF(c2);                                \
	    v1 = vis_ld_u8(p1);                                 \
	    v0 = vis_ld_u8(p0);                                 \
	    v2 = vis_ld_u8(p2);                                 \
	    vis_st_u8(BLE8C(v1, PCOMP8(v1, c1, a1)), p1);       \
	    vis_st_u8(BLE8C(v0, PCOMP8(v0, c0, a0)), p0);       \
	    vis_st_u8(BLE8C(v2, PCOMP8(v2, c2, a2)), p2);       \
	}

/* *********************************************************** */

#define	BLE32C(f_bg, f_c0)                                            \
	vis_fors(falpha, vis_fpack16(vis_fpadd16(vis_fmul8x16(f_bg,   \
	    da1), vis_fmul8x16(f_c0, da))))

/* *********************************************************** */

#define	PCOMP32(v, cof, ac)                                             \
	BLE32C(v, vis_fpack16(vis_fpadd16(ac, vis_fmul8x16(v, cof))))

/* *********************************************************** */

#define	PCALC32(ptr, i0, bi)                                    \
	{                                                       \
	    mlib_f32 v0, *p = (mlib_f32 *)(ptr);                \
	    mlib_d64 a0, c0 = buf[bi].c[i0];                    \
	                                                        \
	    a0 = CALC_CCOEF(c0);                                \
	    v0 = p[0];                                          \
	    p[0] = PCOMP32(v0, c0, a0);                         \
	}

/* *********************************************************** */

#define	PCALC3_32(p1, p0, p2, i1, i0, i2, bi)                   \
	{                                                       \
	    mlib_d64 c1, c0, c2, a1, a0, a2;                    \
	    mlib_f32 v1, v0, v2;                                \
	                                                        \
	    c1 = buf[bi].c[i1];                                 \
	    a1 = CALC_CCOEF(c1);                                \
	    c0 = buf[bi].c[i0];                                 \
	    a0 = CALC_CCOEF(c0);                                \
	    c2 = buf[bi].c[i2];                                 \
	    a2 = CALC_CCOEF(c2);                                \
	    v1 = *(mlib_f32 *)(p1);                             \
	    v0 = *(mlib_f32 *)(p0);                             \
	    v2 = *(mlib_f32 *)(p2);                             \
	    *(mlib_f32 *)(p1) = PCOMP32(v1, c1, a1);            \
	    *(mlib_f32 *)(p0) = PCOMP32(v0, c0, a0);            \
	    *(mlib_f32 *)(p2) = PCOMP32(v2, c2, a2);            \
	}

/* *********************************************************** */

#define	PROC_3C(max, min, cval, of1, of2, bi)                   \
	if (cval + 1 >= min && cval - 1 <= max) {               \
	    if (cval + 1 <= max)                                \
		PCALC(line + (of1), 1, bi);                     \
	    if (cval <= max && cval >= min)                     \
		PCALC(line, 0, bi);                             \
	    if (cval - 1 >= min)                                \
		PCALC(line + (of2), 2, bi);                     \
	}

/* *********************************************************** */

#define	PROC_3N(of1, of2, bi)                                   \
	PCALC3(line + (of1), line, line + (of2), 1, 0, 2, bi)

/* *********************************************************** */

#define	PROC_OCT(sy, ey, sx, ex, noct, vb, is, ie, sty, stx)                \
	if (flagd & (0x1 << noct)) {                                        \
	    mlib_s32 astart = start, mend = (ex) & ~((ex) >> 31);           \
	                                                                    \
	    if (start < (sx))                                               \
		start = (sx);                                               \
	    if (end > mend)                                                 \
		end = mend;                                                 \
	    if (!(flagc & (0x1 << noct))) {                                 \
		if ((oct##ie & ~8) == noct) {                               \
		    if ((oct##ie ^ 8) == (oct##is)) {                       \
			mlib_s32 cxe =                                      \
			    (end < vb##ie) ? end : vb##ie;                  \
			mlib_s32 skiplast = count - cxe;                    \
	                                                                    \
			if ((noct + 1) & 2)                                 \
			    if (!skiplast)                                  \
				--cxe;                                      \
			line =                                              \
			    line0 - (sty) * buf[start].cy +                 \
			    (stx) * start;                                  \
			cy = buf[start].cy;                                 \
			for (cx = start; cx <= cxe; cx++) {                 \
			    mask = buf[cx + 1].m;                           \
			    PROC_3C(sy, ey, cy, -(sty), (sty), cx);         \
			    line += ((sty) & mask) + (stx);                 \
			    cy = buf[cx + 1].cy;                            \
			}                                                   \
			if ((noct + 1) & 2)                                 \
			    if (!skiplast) {                                \
				cy = buf[count].cy;                         \
				if (cx == count) {                          \
				    if (cy + 1 <= sy && cy + 1 >= ey)       \
					PCALC(line - (sty), 1, cx);         \
				    if (cy <= sy && cy >= ey) {             \
					PCALC(line, 0, cx);                 \
					if (cx < (mend))                    \
					    PCALC(line + (stx), 1, cx);     \
				    }                                       \
				} else if (cx == count + 1 && cy <= sy &&   \
				    cy >= ey && cx < mend) {                \
				    line =                                  \
					line0 - (sty) * cy +                \
					(stx) * count;                      \
				    PCALC(line + (stx), 1, count);          \
				}                                           \
			    }                                               \
			astart = vb##is;                                    \
		    } else if (end > vb##ie)                                \
			end = vb##ie;                                       \
		}                                                           \
		if ((oct##is & ~8) == noct) {                               \
		    astart = vb##is;                                        \
		    if (start < astart)                                     \
			start = astart;                                     \
		}                                                           \
	    }                                                               \
	    {                                                               \
		mlib_s32 skiplast = (count - end);                          \
	                                                                    \
		if ((noct + 1) & 2)                                         \
		    if (!skiplast)                                          \
			--end;                                              \
		line = line0 - (sty) * buf[start].cy + (stx) * start;       \
		if (buf[start].cy + 1 < (sy) && buf[end].cy - 1 > (ey)) {   \
		    for (; start <= end; start++) {                         \
			mask = buf[start + 1].m;                            \
			PROC_3N(-(sty), (sty), start);                      \
			line += ((sty) & mask) + (stx);                     \
		    }                                                       \
		} else {                                                    \
		    cy = buf[start].cy;                                     \
		    for (; start <= end; start++) {                         \
			mask = buf[start + 1].m;                            \
			PROC_3C(sy, ey, cy, -(sty), (sty), start);          \
			line += ((sty) & mask) + (stx);                     \
			cy = buf[start + 1].cy;                             \
		    }                                                       \
		}                                                           \
		if ((noct + 1) & 2)                                         \
		    if (!skiplast) {                                        \
			cy = buf[count].cy;                                 \
			if (start == count) {                               \
			    if (cy + 1 <= sy && cy + 1 >= ey)               \
				PCALC(line - (sty), 1, start);              \
			    if (cy <= sy && cy >= ey) {                     \
				PCALC(line, 0, start);                      \
				if (start < (mend))                         \
				    PCALC(line + (stx), 1, start);          \
			    }                                               \
			} else if (start == count + 1 && astart <= count && \
			    cy <= sy && cy >= ey && start < mend) {         \
			    line = line0 - (sty) * cy + (stx) * count;      \
			    PCALC(line + (stx), 1, count);                  \
			}                                                   \
		    }                                                       \
	    }                                                               \
	}

/* *********************************************************** */

#define	PCALC	PCALC8
#define	PCALC3	PCALC3_8

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawArc_AB_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_f32 t1,
    mlib_f32 t2,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer) - 1;
	mlib_s32 height = mlib_ImageGetHeight(buffer) - 1;
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0, *line;
	mlib_s32 cx, cy, del, mask;

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	pinfo buf0[BUFSIZE], *buf = 0;
	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, mdel, k;

	mlib_d64 c0, c1, c2;
	mlib_f32 cf = vis_to_float(c);
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];

	mlib_s32 a1;
	mlib_f32 fa;
	mlib_d64 da, da1, dc;

	c &= 0xff;
	a &= 0xff;
	a1 = ~a & 0xff;

	fa = vis_to_float((a << 24) | (a << 16) | (a << 8) | a);
	da = vis_to_double(a, a);
	da1 = vis_to_double(a1, a1);
	dc = vis_to_double(c, c);
	vis_write_gsr((1 << 3) + 0);

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		if (INSIDE(x, y)) {
			mlib_s32 s = (data + (stride * y + x))[0];

			(data + (stride * y + x))[0] =
			    ((255. - a) * s + c * a) / 255.;
		}
		return (MLIB_SUCCESS);
	}

	if (mlib_fabs(t1 - t2) >= PIx2)
		return (__mlib_GraphicsDrawCircle_AB_8(buffer, x, y, r, c, a));
	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	line0 = data + stride * y + x;

	if (r > RADMAX) {
		buf = (pinfo *) __mlib_malloc(sizeof (pinfo) * r);

		if (!buf)
			return (MLIB_FAILURE);
	} else
		buf = buf0;

	k = (0x100000 / r);

	FILL_BUF;

	GET_BORDERS;

	FILL_FLAGS;

	FILL_CL_FLAGS;

	start = 0;
	end = count;
	PROC_OCT(y, y - height, x - width, x, 2, cos, 1, 2, stride, -1);
	start = 1;
	end = count;
	PROC_OCT(y, y - height, -x, width - x, 1, cos, 2, 1, stride, 1);
	start = 0;
	end = count;
	PROC_OCT(height - y, -y, x - width, x, 5, cos, 2, 1, -stride, -1);
	start = 1;
	end = count;
	PROC_OCT(height - y, -y, -x, width - x, 6, cos, 1, 2, -stride, 1);

	start = 1;
	end = scount;
	PROC_OCT(x, x - width, y - height, y, 3, sin, 2, 1, 1, -stride);
	start = 0;
	end = scount;
	PROC_OCT(x, x - width, -y, height - y, 4, sin, 1, 2, 1, stride);
	start = 1;
	end = scount;
	PROC_OCT(width - x, -x, y - height, y, 0, sin, 1, 2, -1, -stride);
	start = 0;
	end = scount;
	PROC_OCT(width - x, -x, -y, height - y, 7, sin, 2, 1, -1, stride);

	if (buf != buf0)
		__mlib_free(buf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  PCALC
#undef  PCALC3

/* *********************************************************** */

#define	PCALC	PCALC32
#define	PCALC3	PCALC3_32

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawArc_AB_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_f32 t1,
    mlib_f32 t2,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (mlib_s32);
	mlib_s32 width = mlib_ImageGetWidth(buffer) - 1;
	mlib_s32 height = mlib_ImageGetHeight(buffer) - 1;
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0, *line;
	mlib_s32 cx, cy, del, mask;

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	pinfo buf0[BUFSIZE], *buf = 0;
	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, mdel, k;
	mlib_d64 c0, c1, c2;
	mlib_f32 cf;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];

	mlib_s32 a1;
	mlib_f32 fa;
	mlib_d64 da, da1, dc;
	mlib_f32 falpha = vis_to_float(0xFF000000);

	a &= 0xff;
	a1 = ~a & 0xff;

	fa = vis_to_float((a << 16) | (a << 8) | a);
	da = vis_to_double((a << 6), (a << 22) | (a << 6));
	da1 = vis_to_double((a1 << 6), (a1 << 22) | (a1 << 6));
	dc = vis_fmul8x16al(vis_to_float(c), vis_to_float(0x4000));
	vis_write_gsr((1 << 3) + 0);

	c |= 0xFF000000;
	cf = vis_to_float(c);
	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		if (INSIDE(x, y))
			BLE32((data + (stride * y + x)), 0);
		return (MLIB_SUCCESS);
	}

	if (mlib_fabs(t1 - t2) >= PIx2)
		return (__mlib_GraphicsDrawCircle_AB_32(buffer, x, y, r, c, a));
	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	line0 = data + stride * y + x;

	if (r > RADMAX) {
		buf = (pinfo *) __mlib_malloc(sizeof (pinfo) * r);

		if (!buf)
			return (MLIB_FAILURE);
	} else
		buf = buf0;

	k = (0x100000 / r);

	FILL_BUF;

	GET_BORDERS;

	FILL_FLAGS;

	FILL_CL_FLAGS;

	start = 0;
	end = count;
	PROC_OCT(y, y - height, x - width, x, 2, cos, 1, 2, stride, -1);
	start = 1;
	end = count;
	PROC_OCT(y, y - height, -x, width - x, 1, cos, 2, 1, stride, 1);
	start = 0;
	end = count;
	PROC_OCT(height - y, -y, x - width, x, 5, cos, 2, 1, -stride, -1);
	start = 1;
	end = count;
	PROC_OCT(height - y, -y, -x, width - x, 6, cos, 1, 2, -stride, 1);

	start = 1;
	end = scount;
	PROC_OCT(x, x - width, y - height, y, 3, sin, 2, 1, 1, -stride);
	start = 0;
	end = scount;
	PROC_OCT(x, x - width, -y, height - y, 4, sin, 1, 2, 1, stride);
	start = 1;
	end = scount;
	PROC_OCT(width - x, -x, y - height, y, 0, sin, 1, 2, -1, -stride);
	start = 0;
	end = scount;
	PROC_OCT(width - x, -x, -y, height - y, 7, sin, 2, 1, -1, stride);

	if (buf != buf0)
		__mlib_free(buf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
