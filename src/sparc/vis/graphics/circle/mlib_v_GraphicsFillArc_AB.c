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

#pragma ident	"@(#)mlib_v_GraphicsFillArc_AB.c	9.4	07/11/05 SMI"

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

#include <vis_proto.h>
#include <mlib_SysMath.h>
#include <mlib_graphics.h>
#include <mlib_v_GraphicsTabAlias.h>
#include <mlib_GraphicsMacro.h>
#include <mlib_GraphicsDrawSet.h>

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

#define	BLENDALIAS_SIMPLE8(stepsign)  { 		\
	mlib_u8 tmp;  					\
	buf_line->cx = xx;				\
	buf_line->cy = yy;				\
							\
	w_bg =  vis_ld_u8_i(data, xx + yy * stride);	\
	w_bg = BLE8C(w_bg, d_c); 			\
	vis_st_u8(w_bg, &(tmp)); 			\
	buf_line->c = tmp; 				\
							\
	if (stepsign) {					\
		buf_line->acx = ux; 			\
		buf_line->acy = uy;			\
		if ((~ux) && (~uy)) {			\
		w_ubg = vis_ld_u8_i(data,		\
		    ux + uy * stride); 			\
		w_uresbg = vis_fmul8x16al(		\
		    vis_read_lo(w_ubg), 		\
		    vis_read_lo(w_udiv));		\
		w_uresbg = vis_fpadd16(			\
		    w_uresc, w_uresbg);			\
		w_uresbg = BLE8C(w_ubg, w_uresbg);	\
		vis_st_u8(w_uresbg, &(tmp)); 		\
		buf_line->ac = tmp;			\
		}					\
	}						\
	else {						\
		buf_line->acx = dx;			\
		buf_line->acy = dy;			\
		if (~dx && ~dy) {			\
		    w_dbg = vis_ld_u8_i(		\
			data, dx + dy * stride);	\
		    w_dresbg = vis_fmul8x16al(		\
			vis_read_lo(w_dbg),		\
			vis_read_lo(w_udiv));		\
		    w_dresbg = vis_fpadd16(		\
			w_uresc, w_dresbg);		\
		    w_dresbg = BLE8C(w_dbg, w_dresbg);	\
		    vis_st_u8(w_dresbg, &(tmp)); 	\
		    buf_line->ac = tmp;			\
		}					\
	}						\
	buf_line++;					\
}

/* *********************************************************** */

#define	BLENDALIAS_SIMPLE32(stepsign)				\
{								\
	mlib_f32 tmp, v_ubg, v_dbg;				\
	v_bg = *(mlib_f32 *)(data + xx + yy * stride);		\
	*(mlib_f32 *)(&(buf_line->c)) =				\
			BLE32C(v_bg, cf);			\
	buf_line->cx = xx;					\
	buf_line->cy = yy;					\
								\
	if (stepsign) 						\
	{							\
		buf_line->acx = ux;				\
		buf_line->acy = uy;				\
		if ((~ux) && (~uy))				\
		{						\
		    v_ubg = *(mlib_f32 *)(data + ux +		\
			uy * stride);				\
		    w_uresbg = vis_fmul8x16al(v_ubg,		\
			vis_read_lo(w_udiv));			\
		    w_uresbg = vis_fpadd16(w_ac, w_uresbg);	\
		    w_uresbg = vis_fpadd16(w_uresc, w_uresbg);  \
		    vis_write_gsr(7 << 3);			\
		    tmp = vis_fpack16(w_uresbg);		\
		    vis_write_gsr(1 << 3);			\
		    *(mlib_f32 *)(&(buf_line->ac)) =		\
			BLE32C(v_ubg, tmp);			\
		}						\
	}							\
	else {							\
		buf_line->acx = dx;				\
		buf_line->acy = dy;				\
								\
		if ((~dx) && (~dy)) {				\
			v_dbg = *(mlib_f32 *)(data + dx +	\
			    dy * stride);			\
			w_dresbg = vis_fmul8x16al(v_dbg,	\
			    vis_read_lo(w_udiv));		\
			w_dresbg = vis_fpadd16(w_ac,		\
			    w_dresbg);				\
			w_dresbg = vis_fpadd16(w_uresc, 	\
			    w_dresbg);				\
			vis_write_gsr(7 << 3);			\
			tmp = vis_fpack16(w_dresbg);		\
			vis_write_gsr(1 << 3);			\
			*(mlib_f32 *)(&(buf_line->ac)) =	\
			    BLE32C(v_dbg, tmp);			\
		}						\
	}							\
	buf_line++;						\
}


/* *********************************************************** */

#define	LINE_LOAD_ALPHA8					\
	w_div = vis_ld_u8_i(atable, offset + 256);		\
	w_udiv = vis_ld_u8_i(atable, offset ^ 0xFF);		\
	w_ddiv = vis_ld_u8_i(atable, offset);			\
	w_div = vis_fpadd16(w_div, w_div);			\
	w_div = vis_fpsub16(w_div, w_01);			\
	w_udiv = vis_fpadd16(w_udiv, w_udiv);			\
	w_ddiv = vis_fpadd16(w_ddiv, w_ddiv);			\
	w_resc = vis_fmul8x16al(vis_read_lo(w_c),		\
		vis_read_lo(w_div));				\
	w_uresc = vis_fmul8x16al(vis_read_lo(w_c),		\
		vis_read_lo(w_udiv));				\
	w_dresc = vis_fmul8x16al(vis_read_lo(w_c),		\
		vis_read_lo(w_ddiv));				\
	w_div = vis_fpsub16(w_neg, w_div);			\
	w_udiv = vis_fpsub16(w_neg, w_udiv);			\
	w_ddiv = vis_fpsub16(w_neg, w_ddiv);

/* *********************************************************** */

#define	COMPAA8(OFFSET) {				\
	w_udiv = OFFSET;				\
	w_udiv = vis_fpadd16(w_udiv, w_udiv);   	\
	w_uresc = vis_fmul8x16al(vis_read_lo(w_c),	\
		vis_read_lo(w_udiv));			\
	w_udiv = vis_fpsub16(w_neg, w_udiv);		\
}

/* *********************************************************** */

#define	COMPAA32(OFFSET) {				\
	w_udiv = OFFSET;				\
	w_udiv = vis_fpadd16(w_udiv, w_udiv);		\
	w_uresc = vis_fmul8x16al(vis_read_lo(w_c),	\
		vis_read_lo(w_udiv));			\
	w_udiv = vis_fpsub16(w_neg, w_udiv);		\
	w_resc = vis_fpadd16(w_ac, w_resc);		\
}


/* *********************************************************** */

#define	BLENDALIAS32(stepsign)					\
{								\
	mlib_f32 tmp, v_ubg, v_dbg;				\
	v_bg = *(mlib_f32 *)(data + xx + yy * stride);		\
	w_resbg = vis_fmul8x16al(v_bg, vis_read_lo(w_div));	\
	w_resbg = vis_fpadd16(w_ac, w_resbg);			\
	w_resbg = vis_fpadd16(w_resc, w_resbg);			\
	vis_write_gsr(7 << 3);					\
	tmp = vis_fpack16(w_resbg);				\
	vis_write_gsr(1 << 3);					\
	*(mlib_f32 *)(&(buf_line->c)) =				\
		BLE32C(v_bg, tmp);				\
	buf_line->cx = xx;					\
	buf_line->cy = yy;					\
								\
	if (stepsign) {						\
	    buf_line->acx = ux;					\
	    buf_line->acy = uy;					\
	    if (~ux && ~uy)					\
	    {							\
		v_ubg = *(mlib_f32 *)(data + ux			\
			+ uy * stride);				\
		w_uresbg = vis_fmul8x16al(v_ubg, 		\
			vis_read_lo(w_udiv));			\
		w_uresbg = vis_fpadd16(w_ac, w_uresbg);		\
		w_uresbg = vis_fpadd16(w_uresc, w_uresbg);	\
		vis_write_gsr(7 << 3);				\
		tmp = vis_fpack16(w_uresbg);			\
		vis_write_gsr(1 << 3);				\
		*(mlib_f32 *)(&(buf_line->ac)) =		\
			BLE32C(v_ubg, tmp);			\
	    }							\
	}							\
	else {							\
	    buf_line->acx = dx;					\
	    buf_line->acy = dy;					\
								\
	    if ((~dx) && (~dy)) {				\
		v_dbg = *(mlib_f32 *)(data + dx			\
			+ dy * stride);				\
		w_dresbg = vis_fmul8x16al(v_dbg,		\
			vis_read_lo(w_ddiv));			\
		w_dresbg = vis_fpadd16(w_ac, w_dresbg);		\
		w_dresbg = vis_fpadd16(w_dresc, w_dresbg);	\
		vis_write_gsr(7 << 3);				\
		tmp = vis_fpack16(w_dresbg);			\
		vis_write_gsr(1 << 3);				\
		*(mlib_f32 *)(&(buf_line->ac)) =		\
			BLE32C(v_dbg, tmp);			\
	    }							\
	}							\
	buf_line++;						\
}

/* *********************************************************** */

#define	BLENDALIAS8(stepsign)					\
{								\
	mlib_u8 tmp; 						\
	w_bg = vis_ld_u8_i(data, xx + yy * stride);		\
	w_resbg = vis_fmul8x16al(vis_read_lo(w_bg),		\
		vis_read_lo(w_div));				\
	w_resbg = vis_fpadd16(w_resc, w_resbg);			\
	w_resbg = BLE8C(w_bg, w_resbg);				\
	vis_st_u8(w_resbg, &tmp);				\
	buf_line->c = tmp;					\
	buf_line->cx = xx;					\
	buf_line->cy = yy;					\
								\
	if (stepsign) {						\
	    if (~ux && ~uy) {					\
		w_ubg = vis_ld_u8_i(data, ux +			\
		    uy * stride);				\
		w_uresbg =					\
		    vis_fmul8x16al(vis_read_lo(w_ubg),		\
		    vis_read_lo(w_udiv));			\
		w_uresbg = vis_fpadd16(w_uresc, w_uresbg);	\
		w_uresbg = BLE8C(w_ubg, w_uresbg);		\
		vis_st_u8(w_uresbg, &(tmp));			\
		buf_line->ac = tmp;				\
	    }							\
								\
	buf_line->acx = ux;					\
	buf_line->acy = uy;					\
	}							\
	else {							\
	if (~dx && ~dy) {					\
	    w_dbg = vis_ld_u8_i(data, dx + dy * stride);	\
	    w_dresbg =						\
		vis_fmul8x16al(vis_read_lo(w_dbg), 		\
			vis_read_lo(w_ddiv));			\
	    w_dresbg = vis_fpadd16(w_dresc, w_dresbg);		\
	    w_dresbg = BLE8C(w_dbg, w_dresbg);			\
	    vis_st_u8(w_dresbg, &(tmp));			\
	    buf_line->ac = tmp;					\
	}							\
								\
	    buf_line->acx = dx;					\
	    buf_line->acy = dy;					\
	}							\
	buf_line++;						\
}

/* *********************************************************** */

#define	GET_XY_Y	{			\
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

#define	GET_XY_X	{			\
	offset = (t >> (MLIB_SHIFT -		\
	8)) & 0xFF;				\
						\
	xx = t >> MLIB_SHIFT;			\
	t += dt;				\
	ux = xx - 1;				\
	dx = (lwidth - xx) >> 31;		\
	dx = (xx + 1) | dx;			\
						\
	uy = yy | ((xx - 1) >> 31);		\
	dy = yy | ((lwidth - xx) >> 31);	\
}

/* *********************************************************** */

#define	GET_XY_X_SIMPLE	{			\
						\
	ux = xx - 1;				\
	dx = (lwidth - xx) >> 31;		\
	dx = (xx + 1) | dx;			\
						\
	uy = yy | ((xx - 1) >> 31);		\
	dy = yy | ((lwidth - xx) >> 31);	\
}

/* *********************************************************** */

#define	PROC_LINEDATA_Y				\
for (j = 0; j <= n; j++, xx++) {		\
	GET_XY_Y;				\
	LINE_LOAD_ALPHA8;			\
	BLENDALIAS(stepsigny);			\
}

/* *********************************************************** */

#define	PROC_LINEDATA_Y_SIMPLE				\
	for (j = 0; j <= n; j++, xx++) {		\
	    GET_XY_Y_SIMPLE;				\
	    BLENDALIAS_SIMPLE(stepsigny);		\
}

/* *********************************************************** */

#define	PROC_LINEDATA_X					\
	for (j = 0; j <= n; j++, yy++) {		\
	    GET_XY_X;					\
	    LINE_LOAD_ALPHA8;				\
	    BLENDALIAS(stepsignx);			\
}

/* *********************************************************** */

#define	PROC_LINEDATA_X_SIMPLE					\
	for (j = 0; j <= n; j++, yy++, xx += dt) {		\
	    GET_XY_X_SIMPLE;					\
	    BLENDALIAS_SIMPLE(stepsignx);			\
}

/* *********************************************************** */

#define	SPAMLINE_DEL_VAR 						\
	mlib_s32 stepsignx, stepsigny;					\
	mlib_s32 j, xx, yy, n, t, dt, offset, zz, ux, dx, uy, dy;	\
	mlib_u8 *atable, *divt;						\
	pinfo_line * buf_line;						\
	mlib_d64 w_div = 0., w_udiv = 0., w_ddiv = 0.;			\
	mlib_d64 w_bg = 0., w_ubg = 0., w_dbg = 0.;			\
	mlib_d64 w_c = dbc, w_neg = vis_to_double_dup(255);		\
	mlib_d64 w_01 = vis_to_double_dup(0x01);			\
	mlib_d64 w_resc, w_uresc, w_dresc, w_resbg, w_uresbg, w_dresbg; \
	mlib_f32  v_bg;							\
	mlib_d64  d_c = dc, w_ac;					\

/* *********************************************************** */

#define	SPAMLINE_SET_VAR(is)			\
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
	zz = (dt ^ zz) - zz;			\
	buf_line = buf_line##is;		\
	w_ac = vis_to_double(0xFF0000, 0);	\
						\
	atable = mlib_GraphicsAliasTab_u8 +	\
	512 * mlib_GraphicsSqrtTab_u8[zz >>	\
	(MLIB_SHIFT - 7)];

/* *********************************************************** */

#define	SPAMLINESET_AB(is) 				\
{							\
	SPAMLINE_DEL_VAR;				\
	SPAMLINE_SET_VAR(is);				\
							\
	if (n >= 0) {					\
		if (yy < 0) {				\
			t = ~yy;			\
			yy = (t >> MLIB_SHIFT);		\
			if (dt) {			\
				PROC_LINEDATA_Y;	\
			} 				\
			else {				\
				COMPAA( 		\
				vis_to_double_dup(	\
				MLIB_BG_COEFF));	\
				PROC_LINEDATA_Y_SIMPLE;	\
			}				\
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
			COMPAA(				\
			vis_ld_u8_i(atable, 128)); 	\
			PROC_LINEDATA_X_SIMPLE;		\
		}					\
	}						\
	}						\
	lineindex##is = n + 1;				\
}

/* *********************************************************** */

#define	ARC_LINE_AB(is)					\
{	mlib_s32 coords[MLIB_GDS_NEXT];			\
	mlib_s16 x_coord[2], y_coord[2];		\
	x_coord[0] = x;  x_coord[1] = x+cs##is;		\
	y_coord[0] = y;  y_coord[1] = y-sn##is;		\
	mlib_GraphicsClipLine(coords, width,		\
		height, x_coord, y_coord);		\
	SPAMLINESET_AB(is);				\
}

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

typedef struct tag_pinfo_line {
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
		sn1 = (fs1 > 0) ? sin1 : -sin1;                 \
		cs1 = (fc1 > 0) ? cos1 : -cos1;                 \
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

#define	PCALC2_8(p1, p0, i1, i0, bi)                            \
	{                                                       \
	    mlib_d64 v1, v0,  c1, c0,  a1, a0;                  \
	                                                        \
	    c1 = buf[bi].c[i1];                                 \
	    a1 = CALC_CCOEF(c1);                                \
	    c0 = buf[bi].c[i0];                                 \
	    a0 = CALC_CCOEF(c0);                                \
	    v1 = vis_ld_u8(p1);                                 \
	    v0 = vis_ld_u8(p0);                                 \
	    vis_st_u8(BLE8C(v1, PCOMP8(v1, c1, a1)), p1);       \
	    vis_st_u8(BLE8C(v0, PCOMP8(v0, c0, a0)), p0);       \
	}

/* *********************************************************** */


#define	BLE32C(f_bg, f_c0)                                            \
	vis_fors(falpha, vis_fpack16(vis_fpadd16(vis_fmul8x16(f_bg,   \
	    da1), vis_fmul8x16(f_c0, da))))

/* *********************************************************** */

#define	PCOMP32(v, cof, ac)                                           \
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

#define	PCALC2_32(p1, p0, i1, i0, bi)                           \
	{                                                       \
	    mlib_d64 c1, c0, a1, a0;                            \
	    mlib_f32 v1, v0;                                    \
	                                                        \
	    c1 = buf[bi].c[i1];                                 \
	    a1 = CALC_CCOEF(c1);                                \
	    c0 = buf[bi].c[i0];                                 \
	    a0 = CALC_CCOEF(c0);                                \
	    v1 = *(mlib_f32 *)(p1);                             \
	    v0 = *(mlib_f32 *)(p0);                             \
	    *(mlib_f32 *)(p1) = PCOMP32(v1, c1, a1);            \
	    *(mlib_f32 *)(p0) = PCOMP32(v0, c0, a0);            \
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

#define	PROC_2C(max, min, cval, of1,  bi)                       \
	if (cval + 1 >= min && cval - 1 <= max) {               \
	    if (cval + 1 <= max)                                \
		PCALC(line + (of1), 1, bi);                     \
	    if (cval <= max && cval >= min)                     \
		PCALC(line, 0, bi);                             \
	}

/* *********************************************************** */

#define	PROC_2N(of1,  bi)                                       \
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
			    PROC_2C(sy, ey, cy, -(sty), cx);         \
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
			PROC_2N(-(sty),  start);			    \
			line += ((sty) & mask) + (stx);                     \
		    }                                                       \
		} else {                                                    \
		    cy = buf[start].cy;                                     \
		    for (; start <= end; start++) {                         \
			mask = buf[start + 1].m;                            \
			PROC_2C(sy, ey, cy, -(sty), start);          	    \
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

#define	SET_STEPSIGN    {						\
	mlib_s32 flag_oct1 = 1 << (oct1 & 7);				\
	mlib_s32 flag_oct2 = 1 << (oct2 & 7);				\
	stepsignx1 = (flag_oct1) & 0x0f ? 0 : -1;			\
	stepsignx2 = (flag_oct2) & 0x0f ? -1 : 0;			\
	stepsigny1 = (flag_oct1) & 0xc3 ? 0 : -1;			\
	stepsigny2 = (flag_oct2) & 0xc3 ? -1 : 0;			\
	ifoverlap = 0;							\
	if (t2 - t1 > PI_4 * 6.1) ifoverlap = 1;			\
}

/* *********************************************************** */

#define	SET_PIX(lx, ly, lc)					\
	if ((~lx) && (~ly)) {					\
	    data[lx+ ly * stride] = lc;				\
	}


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
	    SET_PIX(linex, liney, buf_line##is[ll].c);			\
	    linex = buf_line##is[ll].acx;				\
	    liney = buf_line##is[ll].acy;				\
	    if (ifoverlap) {						\
		CHECKING_OVERLAP(is, io);				\
	    }								\
	    if (overlap) {						\
		SET_PIX(linex, liney, buf_line##is[ll].ac);		\
	    }								\
	}								\
}

/* *********************************************************** */

#define	FREE_MEM	{				\
	if (buf != buf0)				\
	    __mlib_free(buf);				\
	if (buf_line1 != buf0_line1)			\
	    __mlib_free(buf_line1);			\
	if (buf_line2 != buf0_line2)			\
	    __mlib_free(buf_line2);			\
}

/* *********************************************************** */

#define	PCALC	PCALC8
#define	PCALC3	PCALC3_8
#define	PCALC2  PCALC2_8

/* *********************************************************** */

#define	COMPAA  		COMPAA8
#define	BLENDALIAS 		BLENDALIAS8
#define	BLENDALIAS_SIMPLE	BLENDALIAS_SIMPLE8

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
	mlib_s32 lwidth = width - 1;
	mlib_s32 lheight = height - 1;
	mlib_u8 *data = mlib_ImageGetData(buffer);
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

	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, mdel, k;

	mlib_d64 c0, c1, c2;
	mlib_f32 cf = vis_to_float(c);
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];

	mlib_s32 a1;
	mlib_f32 fa;
	mlib_d64 da, da1, dc, dbc;

	c &= 0xff;
	a &= 0xff;
	a1 = ~a & 0xff;

	fa = vis_to_float((a << 24) | (a << 16) | (a << 8) | a);
	da = vis_to_double(a, a);
	da1 = vis_to_double(a1, a1);
	dc = vis_to_double(c, c);
	dbc = dc;

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
		return (__mlib_GraphicsFillCircle_AB_8(buffer, x, y, r, c, a));
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
		buf_line1 = (pinfo_line *) __mlib_malloc(sizeof (pinfo_line) *
			(r + 1));

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
		rez = __mlib_GraphicsDrawLine_AB_8(buffer, x, y, x + cs1,
			y - sn1, c, a);
		FREE_MEM;
		return (rez);
	}

	FILL_FLAGS;

	FILL_CL_FLAGS;

	SET_STEPSIGN;

	ARC_LINE_AB(1);
	ARC_LINE_AB(2);


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

	rez = __mlib_GraphicsFillArc_B_8(buffer, x, y, r - 1, tt1, tt2, c, a);
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

#undef	COMPAA
#undef	BLENDALIAS
#undef	BLENDALIAS_SIMPLE


/* *********************************************************** */

#define	PCALC	PCALC32
#define	PCALC3	PCALC3_32
#define	PCALC2  PCALC2_32

#define	COMPAA  		COMPAA32
#define	BLENDALIAS 		BLENDALIAS32
#define	BLENDALIAS_SIMPLE	BLENDALIAS_SIMPLE32

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
	mlib_s32 stepsignx1, stepsignx2, stepsigny1, stepsigny2;
	mlib_s32 lineindex1 = 0, lineindex2 = 0;
	mlib_s32 ifoverlap = 0;

	mlib_f32 tt1 = t1, tt2 = t2;
	mlib_s32 sn1, cs1, sn2, cs2;

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	pinfo buf0[BUFSIZE], *buf = 0;

	pinfo_line buf0_line1[RADMAX+1], *buf_line1 = 0;
	pinfo_line buf0_line2[RADMAX+1], *buf_line2 = 0;

	mlib_status rez;

	mlib_s32 count, scount;
	mlib_s32 start, end;

	mlib_s32 ind0, ind1, ind2, mdel, k;
	mlib_d64 c0, c1, c2;
	mlib_f32 cf;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];

	mlib_s32 a1;
	mlib_f32 fa;
	mlib_d64 da, da1, dc, dbc;
	mlib_f32 falpha = vis_to_float(0xFF000000);

	a &= 0xff;
	a1 = ~a & 0xff;

	fa = vis_to_float((a << 16) | (a << 8) | a);
	da = vis_to_double((a << 6), (a << 22) | (a << 6));
	da1 = vis_to_double((a1 << 6), (a1 << 22) | (a1 << 6));
	dc = vis_fmul8x16al(vis_to_float(c), vis_to_float(0x4000));
	vis_write_gsr((1 << 3) + 0);

	dbc = vis_to_double(c, c);

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
		return (__mlib_GraphicsFillCircle_AB_32(buffer, x, y, r, c, a));
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

	ARC_LINE_AB(1);
	ARC_LINE_AB(2);

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
