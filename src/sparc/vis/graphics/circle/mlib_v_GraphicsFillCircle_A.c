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

#pragma ident	"@(#)mlib_v_GraphicsFillCircle_A.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillCircle_A_[8 | 32] - draw filled circle
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFillCircle_A_8 (mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c)
 *      mlib_status mlib_GraphicsFillCircle_A_32(mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c)
 *
 * ARGUMENTS
 *      buffer  pointer to a dst image
 *      x, y    start point coords
 *      r       radius
 *      c       color
 *
 * DESCRIPTION
 *      Function fills circle with radius r in image pointed by buffer
 *      with color c and center in point x, y in antialiasing mode.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsFillRow.h>
#include <mlib_v_GraphicsTabAlias.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillCircle_A_32 = __mlib_GraphicsFillCircle_A_32
#pragma weak mlib_GraphicsFillCircle_A_8 = __mlib_GraphicsFillCircle_A_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillCircle_A_32) mlib_GraphicsFillCircle_A_32
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_A_32")));
__typeof__(__mlib_GraphicsFillCircle_A_8) mlib_GraphicsFillCircle_A_8
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_A_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK_INTERSECTION                                             \
	if (x - r > width + 1 || x + r < -1 || y - r > height + 1 ||   \
	    y + r < -1)                                                \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	INSIDEX(X)	((X) <= width && (X) >= 0)
#define	INSIDEY(Y)	((Y) <= height && (Y) >= 0)
#define	INSIDE(X, Y)	(INSIDEX(X) && INSIDEY(Y))

#define	LDCOEF(ind)	((mlib_d64 *)mlib_v_TabAlias)[ind]

/* *********************************************************** */

#define	CALC_CCOEF(coef)                                        \
	vis_fmul8x16(cf, coef);                                 \
	coef = vis_fpsub16(d_one, coef)

#define	PREP_IC(coef)	c##coef = CALC_CCOEF(coef)

/* *********************************************************** */

#define	PPROC	PPROC8
#define	PROC_OPTI	PROC_OPTI8

#define	MLIB_FILL_ROW	MLIB_FILL_ROW_8

/* *********************************************************** */

#define	PCALC8(val, ind)                                        \
	vis_fpack16(vis_fpadd16(ccoef##ind,                     \
	    vis_fmul8x16(vis_read_lo(val), coef##ind)))

/* *********************************************************** */

#define	PPROC8(ptr, ind)                                        \
	{                                                       \
	    mlib_d64 v = vis_ld_u8((ptr));                      \
	                                                        \
	    ((mlib_f32 *)&v)[1] = PCALC8(v, ind);               \
	    vis_st_u8(v, (ptr));                                \
	}

/* *********************************************************** */

#define	PCALC32(val, ind)                                         \
	vis_fpack16(vis_fpadd16(ccoef##ind, vis_fmul8x16((val),   \
	    coef##ind)))

/* *********************************************************** */

#define	PPROC32(ptr, cind)                                      \
	{                                                       \
	    mlib_f32 p = *(mlib_f32 *)(ptr);                    \
	                                                        \
	    p = PCALC32(p, cind);                               \
	    *(mlib_f32 *)(ptr) = p;                             \
	}

/* *********************************************************** */

#define	PUT_CLIP_R0                                             \
	if (r == 0) {                                           \
	    if (INSIDE(x, y))                                   \
		line0[0] = c;                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	PUT_CLIP_R1                                             \
	if (r == 1) {                                           \
	    mlib_d64 coef2 = LDCOEF(148);                       \
	                                                        \
	    coef1 = LDCOEF(276);                                \
	    PREP_IC(coef2);                                     \
	    PREP_IC(coef1);                                     \
	    if (INSIDEY(y)) {                                   \
		if (INSIDEX(x - 2))                             \
		    PPROC(line1 - 2, 1);                        \
		if (INSIDEX(x - 1))                             \
		    line1[-1] = c;                              \
		if (INSIDEX(x))                                 \
		    line1[0] = c;                               \
		if (INSIDEX(x + 1))                             \
		    line1[+1] = c;                              \
		if (INSIDEX(x + 2))                             \
		    PPROC(line1 + 2, 1);                        \
	    }                                                   \
	    if (INSIDEY(y - 1)) {                               \
		if (INSIDEX(x - 1))                             \
		    PPROC(line0 - 1, 2);                        \
		if (INSIDEX(x))                                 \
		    line0[0] = c;                               \
		if (INSIDEX(x + 1))                             \
		    PPROC(line0 + 1, 2);                        \
	    }                                                   \
	    if (INSIDEY(y + 1)) {                               \
		if (INSIDEX(x - 1))                             \
		    PPROC(line3 - 1, 2);                        \
		if (INSIDEX(x))                                 \
		    line3[0] = c;                               \
		if (INSIDEX(x + 1))                             \
		    PPROC(line3 + 1, 2);                        \
	    }                                                   \
	    if (INSIDEX(x)) {                                   \
		if (INSIDEY(y - 2))                             \
		    PPROC(line0 - stride, 1);                   \
		if (INSIDEY(y + 2))                             \
		    PPROC(line3 + stride, 1);                   \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	PUT_CLIP_TB                                             \
	if (INSIDEX(x - cx)) {                                  \
	    if (INSIDEY(y - (cy + 1)))                          \
		PPROC(line0 - cx - stride, 1);                  \
	    if (INSIDEY(y - (cy + 0)))                          \
		PPROC(line0 - cx, 0);                           \
	    if (INSIDEY(y + (cy + 1)))                          \
		PPROC(line3 - cx + stride, 1);                  \
	    if (INSIDEY(y + (cy + 0)))                          \
		PPROC(line3 - cx, 0);                           \
	}                                                       \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - (cy + 1)))                          \
		PPROC(line0 + cx - stride, 1);                  \
	    if (INSIDEY(y - (cy + 0)))                          \
		PPROC(line0 + cx, 0);                           \
	    if (INSIDEY(y + (cy + 1)))                          \
		PPROC(line3 + cx + stride, 1);                  \
	    if (INSIDEY(y + (cy + 0)))                          \
		PPROC(line3 + cx, 0);                           \
	}

/* *********************************************************** */

#define	PUT_CLIP_LR                                             \
	if (INSIDEY(y - (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		PPROC(line1 - (cy + 1), 1);                     \
	    if (INSIDEX(x - (cy + 0)))                          \
		PPROC(line1 - cy, 0);                           \
	    if (INSIDEX(x + (cy + 1)))                          \
		PPROC(line1 + (cy + 1), 1);                     \
	    if (INSIDEX(x + (cy + 0)))                          \
		PPROC(line1 + cy, 0);                           \
	}                                                       \
	if (INSIDEY(y + (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		PPROC(line2 - (cy + 1), 1);                     \
	    if (INSIDEX(x - (cy + 0)))                          \
		PPROC(line2 - cy, 0);                           \
	    if (INSIDEX(x + (cy + 1)))                          \
		PPROC(line2 + (cy + 1), 1);                     \
	    if (INSIDEX(x + (cy + 0)))                          \
		PPROC(line2 + cy, 0);                           \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_FIRST                                        \
	coef1 = LDCOEF(255);                                    \
	PREP_IC(coef1);                                         \
	line0[0] = c;                                           \
	PPROC(line0 - stride, 1);                               \
	PPROC(line0 + stride, 1);                               \
	PPROC(line1 - r - 1, 1);                                \
	PPROC(line1 + r + 1, 1);                                \
	line3[0] = c;                                           \
	PPROC(line3 + stride, 1);                               \
	PPROC(line3 - stride, 1);                               \
	MLIB_FILL_ROW(line1, -r, r, c, dcolor)

/* *********************************************************** */

#define	PUT_CLIP_FIRST                                          \
	coef1 = LDCOEF(255);                                    \
	PREP_IC(coef1);                                         \
	if (INSIDEX(x)) {                                       \
	    if (INSIDEY(y - (r + 1)))                           \
		PPROC(line0 - stride, 1);                       \
	    if (INSIDEY(y - (r + 0)))                           \
		line0[0] = c;                                   \
	    if (INSIDEY(y - (r - 1)))                           \
		PPROC(line0 + stride, 1);                       \
	    if (INSIDEY(y + (r + 1)))                           \
		PPROC(line3 + stride, 1);                       \
	    if (INSIDEY(y + (r + 0)))                           \
		line3[0] = c;                                   \
	    if (INSIDEY(y + (r - 1)))                           \
		PPROC(line3 - stride, 1);                       \
	}                                                       \
	if (INSIDEY(y)) {                                       \
	    mlib_s32 beg = (r > x) ? -x : -r;                   \
	    mlib_s32 end = (width - x < r) ? width - x : r;     \
	                                                        \
	    MLIB_FILL_ROW(line1, beg, end, c, dcolor);          \
	    if (INSIDEX(x - (r + 1)))                           \
		PPROC(line1 - (r + 1), 1);                      \
	    if (INSIDEX(x + (r + 1)))                           \
		PPROC(line1 + (r + 1), 1);                      \
	}

/* *********************************************************** */

#define	PUT_CLIP_LAST                                            \
	{                                                        \
	    mlib_s32 beg = ((cx - 1) > x) ? -x : -(cx - 1);      \
	    mlib_s32 end =                                       \
		(width - x < (cx - 1)) ? width - x : (cx - 1);   \
	    if (INSIDEY(y - (cy + 1))) {                         \
		if (INSIDEX(x - (cx + 0)))                       \
		    PPROC(line0 - stride - cx, 1);               \
		if (INSIDEX(x + (cx + 0)))                       \
		    PPROC(line0 - stride + cx, 1);               \
	    }                                                    \
	    if (INSIDEY(y - (cy + 0))) {                         \
		MLIB_FILL_ROW(line0, beg, end, c, dcolor);       \
		if (INSIDEX(x - (cx + 1)))                       \
		    PPROC(line0 - 1 - cx, 1);                    \
		if (INSIDEX(x - (cx + 0)))                       \
		    PPROC(line0 - 0 - cx, 0);                    \
		if (INSIDEX(x + (cx + 0)))                       \
		    PPROC(line0 - 0 + cx, 0);                    \
		if (INSIDEX(x + (cx + 1)))                       \
		    PPROC(line0 + 1 + cx, 1);                    \
	    }                                                    \
	    if (INSIDEY(y + (cy + 0))) {                         \
		MLIB_FILL_ROW(line3, beg, end, c, dcolor);       \
		if (INSIDEX(x - (cx + 1)))                       \
		    PPROC(line3 - 1 - cx, 1);                    \
		if (INSIDEX(x - (cx + 0)))                       \
		    PPROC(line3 - 0 - cx, 0);                    \
		if (INSIDEX(x + (cx + 0)))                       \
		    PPROC(line3 - 0 + cx, 0);                    \
		if (INSIDEX(x + (cx + 1)))                       \
		    PPROC(line3 + 1 + cx, 1);                    \
	    }                                                    \
	    if (INSIDEY(y + (cy + 1))) {                         \
		if (INSIDEX(x - (cx + 0)))                       \
		    PPROC(line3 + stride - cx, 1);               \
		if (INSIDEX(x + (cx + 0)))                       \
		    PPROC(line3 + stride + cx, 1);               \
	    }                                                    \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_TB                                                     \
	PROC_OPTI(line3 - cx + stride, line3 - cx, line3 + cx + stride,   \
	    line3 + cx, line0 - cx - stride, line0 - cx,                  \
	    line0 + cx - stride, line0 + cx, 1, 0)

/* *********************************************************** */

#define	PUT_NOCLIP_LR                                                 \
	PROC_OPTI(line1 - cy - 1, line1 - cy, line1 + cy + 1,         \
	    line1 + cy, line2 - cy - 1, line2 - cy, line2 + cy + 1,   \
	    line2 + cy, 1, 0)

/* *********************************************************** */

#define	PUT_NOCLIP_LAST                                               \
	PROC_OPTI(line3 - cx - 1, line3 - cx, line3 + cx + 1,         \
	    line3 + cx, line0 - cx - 1, line0 - cx, line0 + cx + 1,   \
	    line0 + cx, 1, 0);                                        \
	PPROC(line0 - stride - cx, 1);                                \
	PPROC(line0 - stride + cx, 1);                                \
	MLIB_FILL_ROW(line0, 1 - cx, cx - 1, c, dcolor);              \
	PPROC(line3 + stride - cx, 1);                                \
	PPROC(line3 + stride + cx, 1);                                \
	MLIB_FILL_ROW(line3, 1 - cx, cx - 1, c, dcolor)

/* *********************************************************** */

#define	PROC_OPTI8(p01, p00, p11, p10, p21, p20, p31, p30,          \
	i1, i0)                                                     \
	{                                                           \
	    mlib_d64 v01, v00, v11, v10, v21, v20, v31, v30;        \
	                                                            \
	    v01 = vis_ld_u8(p01);                                   \
	    v00 = vis_ld_u8(p00);                                   \
	    v10 = vis_ld_u8(p10);                                   \
	    v11 = vis_ld_u8(p11);                                   \
	    v21 = vis_ld_u8(p21);                                   \
	    v20 = vis_ld_u8(p20);                                   \
	    v30 = vis_ld_u8(p30);                                   \
	    v31 = vis_ld_u8(p31);                                   \
	    vis_st_u8(vis_write_lo(v01, PCALC8(v01, i1)), (p01));   \
	    vis_st_u8(vis_write_lo(v00, PCALC8(v00, i0)), (p00));   \
	    vis_st_u8(vis_write_lo(v10, PCALC8(v10, i0)), (p10));   \
	    vis_st_u8(vis_write_lo(v11, PCALC8(v11, i1)), (p11));   \
	    vis_st_u8(vis_write_lo(v21, PCALC8(v21, i1)), (p21));   \
	    vis_st_u8(vis_write_lo(v20, PCALC8(v20, i0)), (p20));   \
	    vis_st_u8(vis_write_lo(v30, PCALC8(v30, i0)), (p30));   \
	    vis_st_u8(vis_write_lo(v31, PCALC8(v31, i1)), (p31));   \
	}

/* *********************************************************** */

#define	PROC_OPTI32(p01, p00, p11, p10, p21, p20, p31, p30,     \
	i1, i0)                                                 \
	{                                                       \
	    mlib_f32 v01, v00, v11, v10, v21, v20, v31, v30;    \
	                                                        \
	    v01 = *(mlib_f32 *)(p01);                           \
	    v00 = *(mlib_f32 *)(p00);                           \
	    v10 = *(mlib_f32 *)(p10);                           \
	    v11 = *(mlib_f32 *)(p11);                           \
	    v21 = *(mlib_f32 *)(p21);                           \
	    v20 = *(mlib_f32 *)(p20);                           \
	    v30 = *(mlib_f32 *)(p30);                           \
	    v31 = *(mlib_f32 *)(p31);                           \
	    *(mlib_f32 *)(p01) = PCALC32(v01, i1);              \
	    *(mlib_f32 *)(p00) = PCALC32(v00, i0);              \
	    *(mlib_f32 *)(p10) = PCALC32(v10, i0);              \
	    *(mlib_f32 *)(p11) = PCALC32(v11, i1);              \
	    *(mlib_f32 *)(p21) = PCALC32(v21, i1);              \
	    *(mlib_f32 *)(p20) = PCALC32(v20, i0);              \
	    *(mlib_f32 *)(p30) = PCALC32(v30, i0);              \
	    *(mlib_f32 *)(p31) = PCALC32(v31, i1);              \
	}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillCircle_A_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer) - 1;
	mlib_s32 height = mlib_ImageGetHeight(buffer) - 1;
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

	mlib_s32 ind0, ind1, k;
	mlib_d64 coef0, coef1;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;

	mlib_d64 dcolor;
	mlib_s32 color;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r <= 0 || y + r >= height || y - r <= 0)
		doclip = 1;

	vis_write_gsr((1 << 3));
	cf = vis_to_float(c);

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	PUT_CLIP_R0;
	PUT_CLIP_R1;

	color = c & 0xff;
	color |= (color << 8);
	color |= (color << 16);
	DOUBLE_FROM_INT(dcolor, color);

	k = (0x100000 / r);
	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line2 += stride;

	ind0 = (k) >> 13;
	ind1 = ind0 + 255;

	if (doclip) {

		PUT_CLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);

			PUT_CLIP_TB;
			PUT_CLIP_LR;

			{
				mlib_s32 beg = ((cy - 1) > x) ? -x : -(cy - 1);
				mlib_s32 end =
				    (width - x <
				    (cy - 1)) ? width - x : (cy - 1);

				if (INSIDEY(y - cx))
					MLIB_FILL_ROW(line1, beg, end, c,
					    dcolor);

				if (INSIDEY(y + cx))
					MLIB_FILL_ROW(line2, beg, end, c,
					    dcolor);
			}

			del += (2 * cx + 1);

			if (!mask) {
				mlib_s32 beg = ((cx - 1) > x) ? -x : -(cx - 1);
				mlib_s32 end =
				    (width - x <
				    (cx - 1)) ? width - x : (cx - 1);

				if (INSIDEY(y - cy))
					MLIB_FILL_ROW(line0, beg, end, c,
					    dcolor);

				if (INSIDEY(y + cy))
					MLIB_FILL_ROW(line3, beg, end, c,
					    dcolor);
				del -= (2 * cy);
			}

			mask = del >> 31;
			cy += ~mask;

			ind0 = ((cy + del) & mask) & ~((cy + del) >> 31);
			ind0 += (del - cy) & ~((del - cy) >> 31);
			ind1 = cy + del + ((cy * 2) & mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);

		PUT_CLIP_LAST;

		if (cx < cy) {
			mlib_s32 beg = ((cy - 1) > x) ? -x : -(cy - 1);
			mlib_s32 end =
			    (width - x < (cy - 1)) ? width - x : (cy - 1);

			if (INSIDEY(y - cx))
				MLIB_FILL_ROW(line1, beg, end, c, dcolor);

			if (INSIDEY(y + cx))
				MLIB_FILL_ROW(line2, beg, end, c, dcolor);
			PUT_CLIP_LR;
		}
	} else {
		PUT_NOCLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);

			PUT_NOCLIP_TB;
			PUT_NOCLIP_LR;

			MLIB_FILL_ROW(line1, -cy + 1, cy - 1, c, dcolor);
			MLIB_FILL_ROW(line2, -cy + 1, cy - 1, c, dcolor);

			if (!mask) {
				MLIB_FILL_ROW(line0, -cx + 1, cx - 1, c,
				    dcolor);
				MLIB_FILL_ROW(line3, -cx + 1, cx - 1, c,
				    dcolor);
			}

			del += (2 * cx + 1) - ((2 * cy) & ~mask);
			mask = del >> 31;
			cy += ~mask;

			ind0 = ((cy + del) & mask) & ~((cy + del) >> 31);
			ind0 += (del - cy) & ~((del - cy) >> 31);
			ind1 = cy + del + ((cy * 2) & mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);

		PUT_NOCLIP_LAST;

		if (cx < cy) {
			MLIB_FILL_ROW(line1, -cy + 1, cy - 1, c, dcolor);
			MLIB_FILL_ROW(line2, -cy + 1, cy - 1, c, dcolor);
			PUT_NOCLIP_LR;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  PPROC
#undef  PROC_OPTI
#undef  MLIB_FILL_ROW

/* *********************************************************** */

#define	PPROC	PPROC32
#define	PROC_OPTI	PROC_OPTI32
#define	MLIB_FILL_ROW	MLIB_FILL_ROW_32

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillCircle_A_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (mlib_s32);
	mlib_s32 width = mlib_ImageGetWidth(buffer) - 1;
	mlib_s32 height = mlib_ImageGetHeight(buffer) - 1;
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

	mlib_s32 ind0, ind1, k;
	mlib_d64 coef0, coef1;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;
	mlib_d64 dcolor;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r <= 0 || y + r >= height || y - r <= 0)
		doclip = 1;

	vis_write_gsr((1 << 3));
	c  |= 0xFF000000;
	cf  = vis_to_float(c);

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	PUT_CLIP_R0;
	PUT_CLIP_R1;

	DOUBLE_FROM_INT(dcolor, c);

	k = (0x100000 / r);
	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line2 += stride;

	ind0 = (k) >> 13;
	ind1 = ind0 + 255;

	if (doclip) {

		PUT_CLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);

			PUT_CLIP_TB;
			PUT_CLIP_LR;

			{
				mlib_s32 beg = ((cy - 1) > x) ? -x : -(cy - 1);
				mlib_s32 end =
				    (width - x <
				    (cy - 1)) ? width - x : (cy - 1);

				if (INSIDEY(y - cx))
					MLIB_FILL_ROW(line1, beg, end, c,
					    dcolor);

				if (INSIDEY(y + cx))
					MLIB_FILL_ROW(line2, beg, end, c,
					    dcolor);
			}

			del += (2 * cx + 1);

			if (!mask) {
				mlib_s32 beg = ((cx - 1) > x) ? -x : -(cx - 1);
				mlib_s32 end =
				    (width - x <
				    (cx - 1)) ? width - x : (cx - 1);

				if (INSIDEY(y - cy))
					MLIB_FILL_ROW(line0, beg, end, c,
					    dcolor);

				if (INSIDEY(y + cy))
					MLIB_FILL_ROW(line3, beg, end, c,
					    dcolor);
				del -= (2 * cy);
			}

			mask = del >> 31;
			cy += ~mask;

			ind0 = ((cy + del) & mask) & ~((cy + del) >> 31);
			ind0 += (del - cy) & ~((del - cy) >> 31);
			ind1 = cy + del + ((cy * 2) & mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);

		PUT_CLIP_LAST;

		if (cx < cy) {
			mlib_s32 beg = ((cy - 1) > x) ? -x : -(cy - 1);
			mlib_s32 end =
			    (width - x < (cy - 1)) ? width - x : (cy - 1);

			if (INSIDEY(y - cx))
				MLIB_FILL_ROW(line1, beg, end, c, dcolor);

			if (INSIDEY(y + cx))
				MLIB_FILL_ROW(line2, beg, end, c, dcolor);
			PUT_CLIP_LR;
		}
	} else {
		PUT_NOCLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);

			PUT_NOCLIP_TB;
			PUT_NOCLIP_LR;

			MLIB_FILL_ROW(line1, -cy + 1, cy - 1, c, dcolor);
			MLIB_FILL_ROW(line2, -cy + 1, cy - 1, c, dcolor);

			del += (2 * cx + 1);

			if (!mask) {
				MLIB_FILL_ROW(line0, -cx + 1, cx - 1, c,
				    dcolor);
				MLIB_FILL_ROW(line3, -cx + 1, cx - 1, c,
				    dcolor);
				del -= (2 * cy);
			}

			mask = del >> 31;
			cy += ~mask;

			ind0 = ((cy + del) & mask) & ~((cy + del) >> 31);
			ind0 += (del - cy) & ~((del - cy) >> 31);
			ind1 = cy + del + ((cy * 2) & mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);

		PUT_NOCLIP_LAST;

		if (cx < cy) {
			MLIB_FILL_ROW(line1, -cy + 1, cy - 1, c, dcolor);
			MLIB_FILL_ROW(line2, -cy + 1, cy - 1, c, dcolor);
			PUT_NOCLIP_LR;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
