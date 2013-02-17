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

#pragma ident	"@(#)mlib_c_GraphicsFillArc_AB.c	9.4	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillArc_AB_[8 | 32] - fill arc
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFillArc_AB_8 (mlib_image *buffer,
 *                                            mlib_s16   x,
 *                                            mlib_s16   y,
 *                                            mlib_s32   r,
 *                                            mlib_f32   t1,
 *                                            mlib_f32   t2,
 *                                            mlib_s32   c,
 *                                            mlib_s32   a)
 *
 *      mlib_status mlib_GraphicsFillArc_AB_32(mlib_image *buffer,
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
 *      a       alpha
 *
 * DESCRIPTION
 *      Function draws antialiased circle arc with radius r in image pointed
 *      by buffer with color c and center in point x, y.
 *      If abs(t1 - t2) >= 2 * PI, result is circle.
 *      If t1 > t2 t2 = t2 + 2 * PI is used.
 */

#include <mlib_SysMath.h>
#include <mlib_graphics.h>
#include <mlib_c_GraphicsTabAlias.h>
#include <mlib_ImageDivTables.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillArc_AB_32 = __mlib_GraphicsFillArc_AB_32
#pragma weak mlib_GraphicsFillArc_AB_8 = __mlib_GraphicsFillArc_AB_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillArc_AB_32) mlib_GraphicsFillArc_AB_32
    __attribute__((weak, alias("__mlib_GraphicsFillArc_AB_32")));
__typeof__(__mlib_GraphicsFillArc_AB_8) mlib_GraphicsFillArc_AB_8
    __attribute__((weak, alias("__mlib_GraphicsFillArc_AB_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct tag_pinfo_line
{
	mlib_s32 cx;
	mlib_s32 cy;
	mlib_s32 acx;
	mlib_s32 acy;
	mlib_s32 c;
	mlib_s32 ac;
	mlib_u16 notused;
} pinfo_line;

/* *********************************************************** */

typedef struct tag_pinfo
{
	mlib_s32 cy;
	mlib_s32 m;
	mlib_u16 c[3];
	mlib_u16 notused;
} pinfo;

/* *********************************************************** */

#define	GET_BUF_LINE(stepsign)  { 		\
	buf_line->cx = xx;			\
	buf_line->cy = yy;			\
	BLENDALIAS(yy * stride + xx, 		\
	    (atable[offset + 256]) << 8,	\
	    buf_line->c);  			\
	if (stepsign) {				\
	    buf_line->acx = ux;			\
	    buf_line->acy = uy;			\
	    if (~ux && ~uy)			\
		BLENDALIAS(uy * stride + ux,    \
		    (atable[offset ^ 0xFF]	\
		    << 8), buf_line->ac);  	\
	} else					\
	{					\
	    buf_line->acx = dx;			\
	    buf_line->acy = dy;			\
	    if (~dx && ~dy)			\
		BLENDALIAS(dy * stride + dx, 	\
		    (atable[offset] << 8),	\
		    buf_line->ac);  		\
	}					\
	buf_line++;				\
}

/* *********************************************************** */

#define	GET_BUF_LINE_SIMPLE(stepsign)  { 	\
	buf_line->cx = xx;			\
	buf_line->cy = yy;			\
	BLENDP(data[yy * stride + xx], c, 	\
	    buf_line->c); 			\
	if (stepsign) {				\
	    buf_line->acx = ux;			\
	    buf_line->acy = uy;			\
	    if (~ux && ~uy)			\
		BLENDALIAS_SIMPLE(uy * stride	\
		    + ux, divt, buf_line->ac); 	\
	} else {				\
	    buf_line->acx = dx;			\
	    buf_line->acy = dy;			\
	    if (~dx && ~dy)			\
		BLENDALIAS_SIMPLE(dy * stride 	\
		    + dx, divt, buf_line->ac); 	\
	}					\
	buf_line++;				\
}

/* ********************************************************* */

#define	COMPAA8(OFFSET) {			\
	divt = mlib_GraphicsDivTab_u8 +		\
	    OFFSET; 				\
	alpha0 = divt[c];			\
}

/* *********************************************************** */

#define	COMPAA32(OFFSET) {			\
	mlib_u8 c0 = (c >> 16) & 0xff;		\
	mlib_u8 c1 = (c >> 8) & 0xff;		\
	mlib_u8 c2 = (c >> 0) & 0xff;		\
	divt  = mlib_GraphicsDivTab_u8 +	\
	    OFFSET; 				\
	alpha0 = divt[c0];			\
	alpha1 = divt[c1];			\
	alpha2 = divt[c2];			\
}

/* *********************************************************** */

#define	BLENDALIAS8(OFF, OFFSET, VAL)				\
{								\
	mlib_u8 bg = data[OFF];					\
	mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);	\
	mlib_u8 cbg = bg + (_dtable[c] - _dtable[bg]);		\
	BLEND8P(bg, cbg, VAL);					\
}

/* *********************************************************** */

#define	BLENDALIAS_SIMPLE8(OFF, DIVT, VAL)			\
{								\
	mlib_u8 bg  = data[OFF];				\
	mlib_u8 cbg = bg + (alpha0 - DIVT[ bg]);		\
	BLEND8P(bg, cbg, VAL);					\
}

/* *********************************************************** */

#define	BLENDALIAS_SIMPLE32(OFF, DIVT,  VAL)			\
{								\
	mlib_u8  bg1, bg2, bg0;					\
	mlib_u8  c1, c2, c0;					\
	mlib_s32 bg  = data[OFF];				\
								\
	bg0 = ((bg >> 16) & 0xff);				\
	bg1 = ((bg >> 8) & 0xff);				\
	bg2 = ((bg >> 0) & 0xff);				\
								\
	c0 = bg0 + (alpha0 - DIVT[bg0]);			\
	c1 = bg1 + (alpha1 - DIVT[bg1]);			\
	c2 = bg2 + (alpha2 - DIVT[bg2]);			\
								\
	BLEND8(bg0, c0);    					\
	BLEND8(bg1, c1);					\
	BLEND8(bg2, c2);    					\
								\
	VAL = 0xff000000 | (bg0 << 16) | (bg1 << 8) | bg2;	\
}

/* *********************************************************** */

#define	BLENDALIAS32(OFF, OFFSET, VAL)				\
{								\
	mlib_u8  bg1, bg2, bg0;					\
	mlib_s32 bg = data[OFF];				\
	mlib_u8 *_dtable = mlib_GraphicsDivTab_u8 + (OFFSET);	\
								\
	mlib_u8 c0 = (c >> 16) & 0xff;				\
	mlib_u8 c1 = (c >> 8) & 0xff;				\
	mlib_u8 c2 = (c >> 0) & 0xff;				\
								\
	bg0 = ((bg >> 16) & 0xff);				\
	bg1 = ((bg >> 8) & 0xff);				\
	bg2 = ((bg >> 0)& 0xff);				\
								\
	c0 = bg0 + (_dtable[c0] - _dtable[bg0]);		\
	c1 = bg1 + (_dtable[c1] - _dtable[bg1]);		\
	c2 = bg2 + (_dtable[c2] - _dtable[bg2]);		\
								\
	BLEND8(bg0, c0);					\
	BLEND8(bg1, c1);					\
	BLEND8(bg2, c2);					\
								\
	bg = 0xff000000 | (bg0 << 16) | (bg1 << 8) | bg2;	\
	VAL = bg;						\
}

/* *********************************************************** */

#define	GET_XY_Y {				\
	offset =				\
	(t >> (MLIB_SHIFT - 8)) & 0xFF;		\
	yy = t >> MLIB_SHIFT;			\
	t += dt;				\
						\
	ux = ((yy - 1) >> 31) | xx;		\
	dx = ((lheight - yy) >> 31) | xx;	\
	uy = yy - 1;				\
	dy = yy + 1;				\
}

/* *********************************************************** */

#define	GET_XY_Y_SIMPLE	{			\
	ux = ((yy - 1) >> 31) | xx;		\
	dx = ((lheight - yy) >> 31) | xx;	\
	uy = yy - 1;				\
	dy = yy + 1;				\
}

/* *********************************************************** */

#define	GET_XY_X {				\
	offset = (t >> (MLIB_SHIFT -		\
	8)) & 0xFF;				\
						\
	xx = t >> MLIB_SHIFT;			\
	t += dt;				\
	ux = xx - 1;				\
	dx = (lwidth - xx) >> 31;		\
	dx = (xx + 1) | dx;			\
						\
	uy = yy | ((xx-1) >> 31);		\
	dy = yy |((lwidth - xx) >> 31);		\
}

/* *********************************************************** */

#define	GET_XY_X_SIMPLE	{			\
						\
	ux = xx - 1;				\
	dx = (lwidth - xx) >> 31;		\
	dx = (xx + 1) | dx;			\
						\
	uy = yy | ((xx - 1) >> 31);		\
	dy = yy |((lwidth - xx) >> 31);		\
}

#define	PROC_LINEDATA_Y				\
for (j = 0; j <= n; j++, xx++) {		\
	GET_XY_Y;				\
	GET_BUF_LINE(stepsigny);		\
}

/* ******************************************** */

#define	PROC_LINEDATA_Y_SIMPLE				\
	for (j = 0; j <= n; j++, xx++) {		\
	    GET_XY_Y_SIMPLE;				\
	    GET_BUF_LINE_SIMPLE(stepsigny);		\
}


/* *********************************************************** */

#define	PROC_LINEDATA_X					\
	for (j = 0; j <= n; j++, yy++) {		\
	GET_XY_X;					\
	GET_BUF_LINE(stepsignx);			\
}

/* *********************************************************** */

#define	PROC_LINEDATA_X_SIMPLE					\
	for (j = 0; j <= n; j++, yy++, xx += dt) {		\
	    GET_XY_X_SIMPLE;					\
	    GET_BUF_LINE_SIMPLE(stepsignx);			\
}


/* *********************************************************** */

#define	SPAMLINE_SET_VAR(is)			\
	mlib_s32 stepsignx, stepsigny; 	\
	mlib_s32 j, n, t, dt, offset, zz; 	\
	mlib_u8 *atable, *divt; 		\
						\
	stepsignx = stepsignx##is;		\
	stepsigny = stepsigny##is;		\
						\
	n = coords[MLIB_GDS_LENGTHLINES];	\
	yy = coords[MLIB_GDS_STARTY];		\
	xx = coords[MLIB_GDS_STARTX];		\
	dt = coords[MLIB_GDS_STEPLINES];	\
						\
	zz = dt >> 31;				\
	zz = (dt ^ zz) - zz; 			\
	buf_line = buf_line##is;		\
						\
	atable = mlib_GraphicsAliasTab_u8 +	\
	    512 * mlib_GraphicsSqrtTab_u8[zz 	\
	    >> (MLIB_SHIFT - 7)]; 		\


/* *********************************************************** */

#define	SPAMLINESET_AB(is) 					\
{								\
	SPAMLINE_SET_VAR(is);					\
								\
	if (n >= 0) {						\
		if (yy < 0) {					\
			t = ~yy;				\
			yy = (t >> MLIB_SHIFT);			\
			if (dt) {				\
				PROC_LINEDATA_Y;		\
			} 					\
			else {					\
				COMPAA((MLIB_BG_COEFF << 8));	\
				PROC_LINEDATA_Y_SIMPLE;		\
			}					\
		} 						\
		else {						\
			t = ~xx;				\
			xx = t >> MLIB_SHIFT;			\
								\
			if (dt & (MLIB_ONE - 1)) {		\
				PROC_LINEDATA_X;		\
			} 					\
			else {					\
				dt = dt >> MLIB_SHIFT;		\
				COMPAA((atable[128]<< 8)); 	\
				PROC_LINEDATA_X_SIMPLE;		\
			}					\
		}						\
	}							\
	lineindex##is = n + 1;					\
}


/* *********************************************************** */

#define	ARC_FILL_LINEBUF(is) 					\
{								\
	mlib_s32 coords[MLIB_GDS_NEXT];				\
	mlib_s16 x_crd[2], y_crd[2];				\
								\
	x_crd[0] = x;  x_crd[1] = x+cs##is;			\
	y_crd[0] = y;  y_crd[1] = y-sn##is;			\
								\
	mlib_GraphicsClipLine(coords, width, height,		\
	    x_crd, y_crd); 					\
	SPAMLINESET_AB(is);					\
}

/* *********************************************************** */

#define	BLEND8(s, c)						\
	s = (mlib_U82D64[(s)] * A0) + (mlib_U82D64[(c)] * A1)

/* *********************************************************** */

#define	BLEND8P(s, c, val)					\
	val = (mlib_U82D64[(s)] * A0) + (mlib_U82D64[(c)] * A1)

/* *********************************************************** */

#define	BLEND32(p, c, a) {			\
	mlib_u8 c0, c1, c2;			\
	mlib_u8 r0, r1, r2;			\
	mlib_s32 bg = p;			\
	r0 = ((bg >> 16) & 0xff);		\
	r1 = ((bg >> 8) & 0xff);		\
	r2 = ((bg >> 0) & 0xff);		\
	c0 = ((c >> 16) & 0xff);		\
	c1 = ((c >> 8) & 0xff);			\
	c2 = ((c >> 0) & 0xff);			\
	BLEND8(r0, c0);				\
	BLEND8(r1, c1);				\
	BLEND8(r2, c2);				\
	p = 0xff000000 | (r0 << 16) |		\
	    (r1 << 8) | r2;			\
}

/* *********************************************************** */

#define	BLEND32P(p, c, val) {			\
	mlib_u8 c0, c1, c2;			\
	mlib_u8 r0, r1, r2;			\
	mlib_s32 bg = p;			\
	r0 = ((bg >> 16) & 0xff);		\
	r1 = ((bg >> 8) & 0xff);		\
	r2 = ((bg >> 0) & 0xff);		\
	c0 = ((c >> 16) & 0xff);		\
	c1 = ((c >> 8) & 0xff);			\
	c2 = ((c >> 0) & 0xff);			\
						\
	BLEND8(r0, c0);				\
	BLEND8(r1, c1);				\
	BLEND8(r2, c2);				\
						\
	val = 0xff000000 | (r0 << 16) |		\
	    (r1 << 8) | r2;			\
}

/* *********************************************************** */

#define	PIx2	6.28318530717958647692f
#define	PI_4	0.78539816339744830962f

/* *********************************************************** */

#define	BUFSIZE	256
#define	RADMAX	350

/* *********************************************************** */

#define	INSIDEX(X)	((X) <= width && (X) >= 0)
#define	INSIDEY(Y)	((Y) <= height && (Y) >= 0)
#define	INSIDE(X, Y)	(INSIDEX(X) && INSIDEY(Y))

/* *********************************************************** */

#define	CHECK_INTERSECTION                                      \
	if (x - r > width + 1 || 				\
	    x + r < -1 || 					\
	    y - r > height + 1 ||   				\
	    y + r < -1)                                         \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	FILL_BUF                                                \
	buf[0].cy = r;                                          \
	buf[0].m = 0;                                           \
	buf[0].c[0] = mlib_c_TabAlias_S16[0];                   \
	buf[0].c[1] = mlib_c_TabAlias_S16[255];                 \
	c2 = mlib_c_TabAlias_S16[255];                          \
	buf[0].c[2] = (r == 1) ? (c2 / 4.0f) : c2;              \
	cy = r;                                                 \
	cx = 1;                                                 \
	del = 1 - r;                                            \
	mask = 0;                                               \
	while (cx < cy - 1) {                                   \
	    mdel = (del ^ ~mask) - ~mask;                       \
	    ind0 = cy - mdel;                                   \
	    ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);        \
	    ind1 = cy + del + ((cy * 2) & ~mask);               \
	    ind2 = cy - del + ((cy * 2) & mask);                \
	    ind0 = (ind0 * k) >> 13;                            \
	    ind1 = (ind1 * k) >> 13;                            \
	    ind2 = (ind2 * k) >> 13;                            \
	    buf[cx].cy = cy;                                    \
	    buf[cx].m = mask;                                   \
	    c0 = mlib_c_TabAlias_S16[ind0];                     \
	    c1 = mlib_c_TabAlias_S16[ind1];                     \
	    c2 = mlib_c_TabAlias_S16[ind2];                     \
	    buf[cx].c[0] = c0;                                  \
	    buf[cx].c[1] = c1;                                  \
	    buf[cx].c[2] = c2;                                  \
	    del += (2 * cx + 1) - ((2 * cy) & mask);            \
	    mask = ~(del >> 31);                                \
	    cy += mask;                                         \
	    ++cx;                                               \
	}                                                       \
	                                                        \
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
	buf[cx + 1].m = -1;                                     \
	c0 = mlib_c_TabAlias_S16[ind0];                         \
	c1 = mlib_c_TabAlias_S16[ind1];                         \
	c2 = mlib_c_TabAlias_S16[ind2];                         \
	buf[cx].c[0] = c0;                                      \
	buf[cx].c[1] = c1;                                      \
	buf[cx].c[2] = c2;                                      \
	scount = (cx < cy) ? cx : cx - 1;                       \
	count = cx;                                             \
	buf[cx + 1].cy = 0

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
		sn1 = (fs1 > 0) ? sin1 : -sin1;                 \
		cs1 = (fc1 > 0) ? cos1 : -cos1;   		\
		sn2 = (fs2 > 0) ? sin2 : -sin2;                 \
		cs2 = (fc2 > 0) ? cos2 : -cos2;                 \
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

#define	FILL_FLAGS                                              \
	flagc = ((-2) << oct1) & ~(((-1) << oct2));             \
	flagd = ((-1) << oct1) & ~((-2) << oct2);               \
	flagc |= (flagc >> 8);                                  \
	flagd |= (flagd >> 8);                                  \
	if (sin1 == sin2 && cos1 == cos2 && (oct1 + 8 == oct2 ||\
	    (oct1 + 7 <= oct2 && (sin1 == 0 || cos1 == 0))))    \
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

#define	PCALC8(p0, i0, bi)                                      \
	{                                                       \
	    mlib_s32 v0, c0;                                    \
	                                                        \
	    c0 = buf[bi].c[i0];                                 \
	    v0 = (p0)[0];                                       \
	    BLEND8((p0)[0], PCOMP8(v0, c0));                    \
	}

/* *********************************************************** */

#define	PCALC3_8(p1, p0, p2, i1, i0, i2, bi)                    \
	{                                                       \
	    mlib_s32 v1, v0, v2, c1, c0, c2;                    \
	                                                        \
	    v1 = *(p1);                                         \
	    c1 = buf[bi].c[i1];                                 \
	    v0 = *(p0);                                         \
	    c0 = buf[bi].c[i0];                                 \
	    v2 = *(p2);                                         \
	    c2 = buf[bi].c[i2];                                 \
	    BLEND8(*(p1), PCOMP8(v1, c1));                      \
	    BLEND8(*(p0), PCOMP8(v0, c0));                      \
	    BLEND8(*(p2), PCOMP8(v2, c2));                      \
	}
/* *********************************************************** */

#define	PCALC2_8(p1, p0,  i1, i0,  bi)                    	\
	{                                                       \
	    mlib_s32 v1, v0, v2, c1, c0, c2;                    \
	                                                        \
	    v1 = *(p1);                                         \
	    c1 = buf[bi].c[i1];                                 \
	    v0 = *(p0);                                         \
	    c0 = buf[bi].c[i0];                                 \
	    BLEND8(*(p1), PCOMP8(v1, c1));                      \
	    BLEND8(*(p0), PCOMP8(v0, c0));                      \
	}

/* *********************************************************** */

#define	PCOMP32(v, coef, cfval)	((v) + (((cfval - (v)) * coef) >> 14))
#define	PCOMP8(v, cf)	((v) + (((c - (v)) * cf) >> 14))

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	PCALC32(ptr, i0, bi)                                    \
	{                                                       \
	    mlib_u8 *p = (mlib_u8 *)(ptr);                      \
	    mlib_s32 v2 = p[2], v1 = p[1], v0 = p[0];           \
	    mlib_s32 c0 = buf[bi].c[i0];                        \
	                                                        \
	    p[3] = cfalpha;                                     \
	    BLEND8(p[2], PCOMP32(v2, c0, cf2));                 \
	    BLEND8(p[1], PCOMP32(v1, c0, cf1));                 \
	    BLEND8(p[0], PCOMP32(v0, c0, cf0));                 \
	}

#else

#define	PCALC32(ptr, i0, bi)                                    \
	{                                                       \
	    mlib_u8 *p = (mlib_u8 *)(ptr);                      \
	    mlib_s32 v2 = p[1], v1 = p[2], v0 = p[3];           \
	    mlib_s32 c0 = buf[bi].c[i0];                        \
	                                                        \
	    p[0] = cfalpha;                                     \
	    BLEND8(p[1], PCOMP32(v2, c0, cf2));                 \
	    BLEND8(p[2], PCOMP32(v1, c0, cf1));                 \
	    BLEND8(p[3], PCOMP32(v0, c0, cf0));                 \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	PCALC3_32(p1, p0, p2, i1, i0, i2, bi)                   \
	{                                                       \
	    PCALC32(p1, i1, bi);                                \
	    PCALC32(p0, i0, bi);                                \
	    PCALC32(p2, i2, bi);                                \
	}

/* *********************************************************** */

#define	PCALC2_32(p1, p0, i1, i0,  bi)                   \
	{                                                       \
	    PCALC32(p1, i1, bi);                                \
	    PCALC32(p0, i0, bi);                                \
	}

/* *********************************************************** */

#define	PROC_3C(max, min, cval, of1, of2, bi)                   \
	if (cval + 1 >= min && cval - 1 <= max) {               \
	    if (cval + 1 <= max)                                \
		PCALC(line + (of1), 1, bi)                      \
	    if (cval <= max && cval >= min)                     \
		PCALC(line, 0, bi)                              \
	    if (cval - 1 >= min)                                \
		PCALC(line + (of2), 2, bi)                      \
	}

/* *********************************************************** */

#define	PROC_3N(of1, of2, bi)                                   \
	PCALC3(line + (of1), line, line + (of2), 1, 0, 2, bi)

/* *********************************************************** */

#define	PROC_2C(max, min, cval, of1, bi)                   \
	if (cval + 1 >= min && cval - 1 <= max) {               \
	    if (cval + 1 <= max)                                \
		PCALC(line + (of1), 1, bi)                      \
	    if (cval <= max && cval >= min)                     \
		PCALC(line, 0, bi)                              \
	}

/* *********************************************************** */

#define	PROC_2N(of1, bi)                                   \
	PCALC2(line + (of1), line, 1, 0, bi)


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
			    PROC_2C(sy, ey, cy, -(sty),                     \
				cx) line += ((sty) & mask) + (stx);         \
			    cy = buf[cx + 1].cy;                            \
			}                                                   \
			if ((noct + 1) & 2)                                 \
			    if (!skiplast) {                                \
				cy = buf[count].cy;                         \
				if (cx == count) {                          \
				    if (cy + 1 <= sy && cy + 1 >= ey)       \
					PCALC(line - (sty), 1, cx)          \
				    if (cy <= sy && cy >= ey) {             \
					PCALC(line, 0, cx);                 \
					if (cx < (mend))                    \
						PCALC(line + (stx), 1, cx)  \
				    }                                       \
				} else                                      \
				    if ((cx == count + 1) && 		    \
					(cy <= sy) && (cy >= ey) && 	    \
					(cx < mend)) {            	    \
					line =                              \
					    line0 - (sty) * cy +            \
					    (stx) * count;                  \
					PCALC(line + (stx), 1, count)       \
				    }                                       \
			    }                                               \
			astart = vb##is;                                    \
		    } else                                                  \
			if (end > vb##ie)                                   \
			    end = vb##ie;                                   \
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
			PROC_2N(-(sty),  start) line +=               \
			    ((sty) & mask) + (stx);                         \
		    }                                                       \
		} else {                                                    \
		    cy = buf[start].cy;                                     \
		    for (; start <= end; start++) {                         \
			mask = buf[start + 1].m;                            \
			PROC_2C(sy, ey, cy, -(sty),                   \
			    start) line += ((sty) & mask) + (stx);          \
			cy = buf[start + 1].cy;                             \
		    }                                                       \
		}                                                           \
		if ((noct + 1) & 2)                                         \
		    if (!skiplast) {                                        \
			cy = buf[count].cy;                                 \
			if (start == count) {                               \
			    if (cy + 1 <= sy && cy + 1 >= ey) {             \
				PCALC(line - (sty), 1, start);              \
			    }                                               \
			    if (cy <= sy && cy >= ey) {                     \
				PCALC(line, 0, start);                      \
				if (start < (mend))                         \
					PCALC(line + (stx), 1, start)       \
			    }                                               \
			} else                                              \
			    if ((start == count + 1) && 		    \
				(astart <= count) &&    		    \
				(cy <= sy) && (cy >= ey) && 		    \
				(start < mend)) {     			    \
				line =                                      \
				    line0 - (sty) * cy + (stx) * count;     \
				PCALC(line + (stx), 1, count)               \
			    }                                               \
		    }                                                       \
	    }                                                               \
	}

/* *********************************************************** */

#define	SET_STEPSIGN	{						\
	mlib_s32 flag_oct1 = 1 << (oct1 & 7);				\
	mlib_s32 flag_oct2 = 1 << (oct2 & 7);				\
	stepsignx1 = (flag_oct1) & 0x0f ? 0 : -1;			\
	stepsignx2 = (flag_oct2) & 0x0f ? -1 : 0;			\
	stepsigny1 = (flag_oct1) & 0xc3 ? 0 :  -1;			\
	stepsigny2 = (flag_oct2) & 0xc3 ? -1 : 0;			\
	ifoverlap = 0;							\
	if (t2 - t1 > PI_4 * 6.1) ifoverlap = 1;			\
}

/* *********************************************************** */

#define	SET_PIX(lx, ly, lc) 					\
	if ((~lx) && (~ly))					\
	    data[lx+ ly * stride] = lc;


/* *********************************************************** */

#define	CHECKING_OVERLAP(is, io)					\
	overlap = cs##io * (liney - y) +  sn##io * (linex - x);		\
	overlap = overlap ^ (cs##io >> 31);				\
	if (cs##io == 0) overlap = ~overlap;				\
	overlap = ~((overlap >> 31) ^ (stepsigny##io));

/* *********************************************************** */

#define	DRAW_LINE_AB(is, io)  {						\
	mlib_s32 linex, liney, ll, overlap = -1;			\
	for (ll = 0; ll < lineindex##is; ll++)				\
	{								\
		linex = buf_line##is[ll].cx;				\
		liney = buf_line##is[ll].cy;				\
		SET_PIX(linex, liney, buf_line##is[ll].c);		\
		linex = buf_line##is[ll].acx;				\
		liney = buf_line##is[ll].acy;				\
									\
		if (ifoverlap) { 					\
			CHECKING_OVERLAP(is, io);			\
		}							\
		if (overlap) 						\
		SET_PIX(linex, liney, buf_line##is[ll].ac);		\
									\
	}								\
}

/* *********************************************************** */

#define	FREE_MEM 	{						\
	if (buf != buf0)						\
		__mlib_free(buf);					\
									\
	if (buf_line1 != buf0_line1)					\
		__mlib_free(buf_line1);					\
									\
	if (buf_line2 != buf0_line2)					\
		__mlib_free(buf_line2);					\
}


/* *********************************************************** */

#define	PCALC	PCALC8
#define	PCALC3	PCALC3_8
#define	PCALC2	PCALC2_8

/* *********************************************************** */

#define	COMPAA  		COMPAA8
#define	BLENDALIAS 		BLENDALIAS8
#define	BLENDALIAS_SIMPLE 	BLENDALIAS_SIMPLE8
#define	BLENDP			BLEND8P

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillArc_AB_8(
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
	mlib_s32 lheight = height - 1;
	mlib_s32 lwidth = width -1;
	mlib_u8 *line0, *line;
	mlib_s32 cx, cy, del, mask;
	mlib_status rez;

	mlib_s32 stepsignx1, stepsignx2, stepsigny1, stepsigny2;
	mlib_s32 lineindex1 = 0, lineindex2 = 0;
	mlib_s32 ifoverlap = 0;

	mlib_f32 tt1 = t1, tt2 = t2;
	mlib_s32 sn1, cs1, sn2, cs2;
	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	pinfo buf0[BUFSIZE], *buf = 0;

	pinfo_line buf0_line1[RADMAX + 1], *buf_line1 = 0;
	pinfo_line buf0_line2[RADMAX + 1], *buf_line2 = 0;
	pinfo_line *buf_line;

	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, c0, c1, c2, mdel, k;
	mlib_s32 xx, yy, ux, uy, dx, dy;
	mlib_s32 alpha0;
	mlib_d64 A0, A1;

	c &= 0xff;
	a &= 0xff;
	A1 = a / 255.;
	A0 = 1. - A1;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		if (INSIDE(x, y))
			(data + (stride * y + x))[0] =
				BLEND8((data + (stride * y + x))[0], c);
		return (MLIB_SUCCESS);
	}

	if (mlib_fabs(t1 - t2) >= PIx2) {
		return (__mlib_GraphicsFillCircle_AB_8(buffer, x, y, r, c, a));
	}

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

		buf_line1 =
		    (pinfo_line *) __mlib_malloc(sizeof (pinfo_line)
		    * (r + 1));

		if (!buf_line1) {
			__mlib_free(buf);
			return (MLIB_FAILURE);
		}

		buf_line2 =
		    (pinfo_line *) __mlib_malloc(sizeof (pinfo_line)
		    * (r + 1));

		if (!buf_line2) {
			__mlib_free(buf);
			__mlib_free(buf_line1);
			return (MLIB_FAILURE);
		}

	} else 	{
		buf = buf0;
		buf_line1 = buf0_line1;
		buf_line2 = buf0_line2;
	}


	k = (0x100000 / r);

	FILL_BUF;

	GET_BORDERS;

	if (t1 == t2) {
		FREE_MEM;
		return (__mlib_GraphicsDrawLine_AB_8(buffer, x, y, x + cs1,
			y - sn1, c, a));
	}

	FILL_FLAGS;

	FILL_CL_FLAGS;

	SET_STEPSIGN;

	ARC_FILL_LINEBUF(1);
	ARC_FILL_LINEBUF(2);

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

	rez = __mlib_GraphicsFillArc_B_8(buffer, x, y,
	    r - 1, tt1, tt2, c, a);

	if (rez != MLIB_SUCCESS) {
		FREE_MEM;
		return (rez);
	}
	DRAW_LINE_AB(1, 2)
	DRAW_LINE_AB(2, 1)

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	PCALC
#undef	PCALC3
#undef	PCALC2

/* *********************************************************** */

#undef	COMPAA
#undef	BLENDALIAS
#undef	BLENDALIAS_SIMPLE
#undef	BLENDP

/* *********************************************************** */

#define	PCALC	PCALC32
#define	PCALC3	PCALC3_32
#define	PCALC2	PCALC2_32

/* *********************************************************** */

#define	COMPAA  		COMPAA32
#define	BLENDALIAS 		BLENDALIAS32
#define	BLENDALIAS_SIMPLE 	BLENDALIAS_SIMPLE32
#define	BLENDP			BLEND32P

/* *********************************************************** */


mlib_status
__mlib_GraphicsFillArc_AB_32(
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
	mlib_s32 lwidth = width - 1;
	mlib_s32 lheight = height - 1;
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0, *line;
	mlib_s32 cx, cy, del, mask;
	mlib_status rez;

	mlib_s32 stepsignx1, stepsignx2, stepsigny1, stepsigny2;
	mlib_s32 lineindex1 = 0, lineindex2 = 0;
	mlib_s32 ifoverlap = 0;

	mlib_f32 tt1 = t1, tt2 = t2;
	mlib_s32 sn1, cs1, sn2, cs2;
	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	mlib_s32 xx, yy, ux, uy, dx, dy;

	pinfo buf0[BUFSIZE], *buf = 0;
	pinfo_line buf0_line1[RADMAX+1], *buf_line1 = 0;
	pinfo_line buf0_line2[RADMAX+1], *buf_line2 = 0;
	pinfo_line *buf_line;

	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, c0, c1, c2, mdel, k;
	mlib_s32 alpha0, alpha1, alpha2;
	mlib_s32 cf0 = c & 0xff, cf1 = (c & 0xff00) >> 8, cf2 =
	    (c & 0xff0000) >> 16;
	mlib_u8 cfalpha = 0xFF;
	mlib_d64 A0, A1;

	a &= 0xff;
	A1 = a / 255.;
	A0 = 1. - A1;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		if (INSIDE(x, y)) {
			BLEND32((data + (stride * y + x))[0], c, a);
		}
		return (MLIB_SUCCESS);
	}

	if (mlib_fabs(t1 - t2) >= PIx2) {
		return (__mlib_GraphicsFillCircle_AB_32(buffer, x, y, r, c, a));
	}
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

		buf_line1 = (pinfo_line *) __mlib_malloc(sizeof (pinfo_line)
			* (r + 1));

		if (!buf_line1) {
			__mlib_free(buf);
			return (MLIB_FAILURE);
		}
		buf_line2 = (pinfo_line *) __mlib_malloc(sizeof (pinfo_line)
			* (r + 1));
		if (!buf_line2) {
			__mlib_free(buf);
			__mlib_free(buf_line1);
			return (MLIB_FAILURE);
		}
	} else {
		buf = buf0;
		buf_line1 = buf0_line1;
		buf_line2 = buf0_line2;
	}


	k = (0x100000 / r);

	FILL_BUF;

	GET_BORDERS;

	if (t1 == t2) {
		FREE_MEM;
		return (__mlib_GraphicsDrawLine_AB_32(buffer, x, y,
		    x + cs1, y - sn1, c, a));
	}

	FILL_FLAGS;

	FILL_CL_FLAGS;

	SET_STEPSIGN;

	ARC_FILL_LINEBUF(1);
	ARC_FILL_LINEBUF(2);

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

	rez = __mlib_GraphicsFillArc_B_32(buffer, x, y, r - 1, tt1, tt2, c, a);
	if (rez != MLIB_SUCCESS) {
		FREE_MEM;
		return (rez);
	}

	DRAW_LINE_AB(1, 2);
	DRAW_LINE_AB(2, 1);

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
