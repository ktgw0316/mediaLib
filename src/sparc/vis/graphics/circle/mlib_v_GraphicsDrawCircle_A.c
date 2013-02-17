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

#pragma ident	"@(#)mlib_v_GraphicsDrawCircle_A.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsDrawCircle_A_[8 | 32] - draw circle
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsDrawCircle_A_8 (mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c)
 *      mlib_status mlib_GraphicsDrawCircle_A_32(mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c)
 *
 * ARGUMENTS
 *      buffer  pointer to an dst image
 *      x, y     start point coords
 *      r       radius
 *      c       color
 *
 * DESCRIPTION
 *      Function draws circle with radius r in image pointed by buffer
 *      with color c and center in point x, y in antialiasing mode.
 */

#include <mlib_graphics.h>
#include <vis_proto.h>
#include <mlib_v_GraphicsTabAlias.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawCircle_A_32 = __mlib_GraphicsDrawCircle_A_32
#pragma weak mlib_GraphicsDrawCircle_A_8 = __mlib_GraphicsDrawCircle_A_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawCircle_A_32) mlib_GraphicsDrawCircle_A_32
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_A_32")));
__typeof__(__mlib_GraphicsDrawCircle_A_8) mlib_GraphicsDrawCircle_A_8
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_A_8")));

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
#define	LDCOEFD(ind)	((mlib_d64 *)mlib_v_TabAlias)[ind]
#define	PREP_IC(coef)	c##coef = CALC_CCOEF(coef)

/* *********************************************************** */

#define	CALC_CCOEF(coef)                                        \
	vis_fmul8x16(cf, coef);                                 \
	coef = vis_fpsub16(d_one, coef)

/* *********************************************************** */

#define	IPPROC8(ptr, ind)                                       \
	{                                                       \
	    mlib_d64 v = vis_ld_u8((ptr));                      \
	                                                        \
	    ((mlib_f32 *)&v)[1] = PCALC8(v, ind);               \
	    vis_st_u8(v, (ptr));                                \
	}

/* *********************************************************** */

#define	PCALC8(val, ind)                                        \
	vis_fpack16(vis_fpadd16(ccoef##ind,                     \
	    vis_fmul8x16(vis_read_lo(val), coef##ind)))

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
	    coef1 = LDCOEFD(276);                               \
	    coef2 = LDCOEFD(148);                               \
	    PREP_IC(coef1);                                     \
	    PREP_IC(coef2);                                     \
	    if (INSIDEY(y)) {                                   \
		if (INSIDEX(x - 2))                             \
		    IPPROC(line1 - 2, 1);                       \
		if (INSIDEX(x - 1))                             \
		    line1[-1] = c;                              \
		if (INSIDEX(x))                                 \
		    IPPROC(line1, 1);                           \
		if (INSIDEX(x + 1))                             \
		    line1[+1] = c;                              \
		if (INSIDEX(x + 2))                             \
		    IPPROC(line1 + 2, 1);                       \
	    }                                                   \
	    if (INSIDEY(y - 1)) {                               \
		if (INSIDEX(x - 1))                             \
		    IPPROC(line0 - 1, 2);                       \
		if (INSIDEX(x))                                 \
		    line0[0] = c;                               \
		if (INSIDEX(x + 1))                             \
		    IPPROC(line0 + 1, 2);                       \
	    }                                                   \
	    if (INSIDEY(y + 1)) {                               \
		if (INSIDEX(x - 1))                             \
		    IPPROC(line3 - 1, 2);                       \
		if (INSIDEX(x))                                 \
		    line3[0] = c;                               \
		if (INSIDEX(x + 1))                             \
		    IPPROC(line3 + 1, 2);                       \
	    }                                                   \
	    if (INSIDEX(x)) {                                   \
		if (INSIDEY(y - 2))                             \
		    IPPROC(line0 - stride, 1);                  \
		if (INSIDEY(y + 2))                             \
		    IPPROC(line3 + stride, 1);                  \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	PUT_CLIP_TB                                             \
	if (INSIDEX(x - cx)) {                                  \
	    if (INSIDEY(y - (cy + 1)))                          \
		IPPROC(line0 - cx - stride, 1);                 \
	    if (INSIDEY(y - (cy + 0)))                          \
		IPPROC(line0 - cx, 0);                          \
	    if (INSIDEY(y - (cy - 1)))                          \
		IPPROC(line0 - cx + stride, 2);                 \
	    if (INSIDEY(y + (cy + 1)))                          \
		IPPROC(line3 - cx + stride, 1);                 \
	    if (INSIDEY(y + (cy + 0)))                          \
		IPPROC(line3 - cx, 0);                          \
	    if (INSIDEY(y + (cy - 1)))                          \
		IPPROC(line3 - cx - stride, 2);                 \
	}                                                       \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - (cy + 1)))                          \
		IPPROC(line0 + cx - stride, 1);                 \
	    if (INSIDEY(y - (cy + 0)))                          \
		IPPROC(line0 + cx, 0);                          \
	    if (INSIDEY(y - (cy - 1)))                          \
		IPPROC(line0 + cx + stride, 2);                 \
	    if (INSIDEY(y + (cy + 1)))                          \
		IPPROC(line3 + cx + stride, 1);                 \
	    if (INSIDEY(y + (cy + 0)))                          \
		IPPROC(line3 + cx, 0);                          \
	    if (INSIDEY(y + (cy - 1)))                          \
		IPPROC(line3 + cx - stride, 2);                 \
	}

/* *********************************************************** */

#define	PUT_CLIP_LR                                             \
	if (INSIDEY(y - (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		IPPROC(line1 - (cy + 1), 1);                    \
	    if (INSIDEX(x - (cy + 0)))                          \
		IPPROC(line1 - cy, 0);                          \
	    if (INSIDEX(x - (cy - 1)))                          \
		IPPROC(line1 - (cy - 1), 2);                    \
	    if (INSIDEX(x + (cy + 1)))                          \
		IPPROC(line1 + (cy + 1), 1);                    \
	    if (INSIDEX(x + (cy + 0)))                          \
		IPPROC(line1 + cy, 0);                          \
	    if (INSIDEX(x + (cy - 1)))                          \
		IPPROC(line1 + (cy - 1), 2);                    \
	}                                                       \
	if (INSIDEY(y + (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		IPPROC(line2 - (cy + 1), 1);                    \
	    if (INSIDEX(x - (cy + 0)))                          \
		IPPROC(line2 - cy, 0);                          \
	    if (INSIDEX(x - (cy - 1)))                          \
		IPPROC(line2 - (cy - 1), 2);                    \
	    if (INSIDEX(x + (cy + 1)))                          \
		IPPROC(line2 + (cy + 1), 1);                    \
	    if (INSIDEX(x + (cy + 0)))                          \
		IPPROC(line2 + cy, 0);                          \
	    if (INSIDEX(x + (cy - 1)))                          \
		IPPROC(line2 + (cy - 1), 2);                    \
	}

/* *********************************************************** */

#define	PUT_CLIP_FIRST                                          \
	{                                                       \
	    coef1 = LDCOEFD(255);                               \
	    PREP_IC(coef1);                                     \
	    if (INSIDEX(x)) {                                   \
		if (INSIDEY(y - (r + 1)))                       \
		    IPPROC(line0 - stride, 1);                  \
		if (INSIDEY(y - (r + 0)))                       \
		    line0[0] = c;                               \
		if (INSIDEY(y - (r - 1)))                       \
		    IPPROC(line0 + stride, 1);                  \
		if (INSIDEY(y + (r + 1)))                       \
		    IPPROC(line3 + stride, 1);                  \
		if (INSIDEY(y + (r + 0)))                       \
		    line3[0] = c;                               \
		if (INSIDEY(y + (r - 1)))                       \
		    IPPROC(line3 - stride, 1);                  \
	    }                                                   \
	    if (INSIDEY(y)) {                                   \
		if (INSIDEX(x - (r + 1)))                       \
		    IPPROC(line1 - (r + 1), 1);                 \
		if (INSIDEX(x - (r + 0)))                       \
		    line1[-r] = c;                              \
		if (INSIDEX(x - (r - 1)))                       \
		    IPPROC(line1 - (r - 1), 1);                 \
		if (INSIDEX(x + (r + 1)))                       \
		    IPPROC(line1 + (r + 1), 1);                 \
		if (INSIDEX(x + (r + 0)))                       \
		    line1[r] = c;                               \
		if (INSIDEX(x + (r - 1)))                       \
		    IPPROC(line1 + (r - 1), 1);                 \
	    }                                                   \
	}

/* *********************************************************** */

#define	PUT_CLIP_LAST                                           \
	if (INSIDEY(y - (cy + 1))) {                            \
	    if (INSIDEX(x - (cx + 0)))                          \
		IPPROC(line0 - stride - cx, 1);                 \
	    if (INSIDEX(x + (cx + 0)))                          \
		IPPROC(line0 - stride + cx, 1);                 \
	}                                                       \
	if (INSIDEY(y - (cy + 0))) {                            \
	    if (INSIDEX(x - (cx + 1)))                          \
		IPPROC(line0 - 1 - cx, 1);                      \
	    if (INSIDEX(x - (cx + 0)))                          \
		IPPROC(line0 - 0 - cx, 0);                      \
	    if (INSIDEX(x + (cx + 0)))                          \
		IPPROC(line0 - 0 + cx, 0);                      \
	    if (INSIDEX(x + (cx + 1)))                          \
		IPPROC(line0 + 1 + cx, 1);                      \
	}                                                       \
	if (INSIDEY(y + (cy + 0))) {                            \
	    if (INSIDEX(x - (cx + 1)))                          \
		IPPROC(line3 - 1 - cx, 1);                      \
	    if (INSIDEX(x - (cx + 0)))                          \
		IPPROC(line3 - 0 - cx, 0);                      \
	    if (INSIDEX(x + (cx + 0)))                          \
		IPPROC(line3 - 0 + cx, 0);                      \
	    if (INSIDEX(x + (cx + 1)))                          \
		IPPROC(line3 + 1 + cx, 1);                      \
	}                                                       \
	if (INSIDEY(y + (cy + 1))) {                            \
	    if (INSIDEX(x - (cx + 0)))                          \
		IPPROC(line3 + stride - cx, 1);                 \
	    if (INSIDEX(x + (cx + 0)))                          \
		IPPROC(line3 + stride + cx, 1);                 \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_FIRST                                        \
	{                                                       \
	    coef1 = LDCOEFD(255);                               \
	    PREP_IC(coef1);                                     \
	    line0[0] = c;                                       \
	    IPPROC(line0 - stride, 1);                          \
	    IPPROC(line0 + stride, 1);                          \
	    line1[-r] = c;                                      \
	    IPPROC(line1 - r - 1, 1);                           \
	    IPPROC(line1 - r + 1, 1);                           \
	    line1[r] = c;                                       \
	    IPPROC(line1 + r + 1, 1);                           \
	    IPPROC(line1 + r - 1, 1);                           \
	    line3[0] = c;                                       \
	    IPPROC(line3 + stride, 1);                          \
	    IPPROC(line3 - stride, 1);                          \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_TB                                                     \
	PROC_OPTI(line3 - cx + stride, line3 - cx, line3 - cx - stride,   \
	    line3 + cx + stride, line3 + cx, line3 + cx - stride,         \
	    line0 - cx - stride, line0 - cx, line0 - cx + stride,         \
	    line0 + cx - stride, line0 + cx, line0 + cx + stride, 1, 0,   \
	    2)

/* *********************************************************** */

#define	PUT_NOCLIP_LR                                                     \
	PROC_OPTI(line1 - cy - 1, line1 - cy, line1 - cy + 1,             \
	    line1 + cy + 1, line1 + cy, line1 + cy - 1, line2 - cy - 1,   \
	    line2 - cy, line2 - cy + 1, line2 + cy + 1, line2 + cy,       \
	    line2 + cy - 1, 1, 0, 2)

/* *********************************************************** */

#define	PUT_NOCLIP_LAST                                                 \
	PROC_OPTI(line3 - cx - 1, line3 - cx, line3 - cx + stride,      \
	    line3 + cx + 1, line3 + cx, line3 + cx + stride,            \
	    line0 - cx - stride, line0 - cx, line0 - cx - 1,            \
	    line0 + cx - stride, line0 + cx, line0 + cx + 1, 1, 0, 1)

/* *********************************************************** */

#define	PROC_OPTI8(p01, p00, p02, p11, p10, p12, p21, p20,               \
	p22, p31, p30, p32, i1, i0, i2)                                  \
	{                                                                \
	    mlib_d64 v01, v00, v02, v11, v10, v12, v21, v20, v22, v31,   \
		v30, v32;                                                \
	    v01 = vis_ld_u8(p01);                                        \
	    v00 = vis_ld_u8(p00);                                        \
	    v02 = vis_ld_u8(p02);                                        \
	    v12 = vis_ld_u8(p12);                                        \
	    v10 = vis_ld_u8(p10);                                        \
	    v11 = vis_ld_u8(p11);                                        \
	    v21 = vis_ld_u8(p21);                                        \
	    v20 = vis_ld_u8(p20);                                        \
	    v22 = vis_ld_u8(p22);                                        \
	    v32 = vis_ld_u8(p32);                                        \
	    v30 = vis_ld_u8(p30);                                        \
	    v31 = vis_ld_u8(p31);                                        \
	    vis_st_u8(vis_write_lo(v01, PCALC8(v01, i1)), (p01));        \
	    vis_st_u8(vis_write_lo(v00, PCALC8(v00, i0)), (p00));        \
	    vis_st_u8(vis_write_lo(v02, PCALC8(v02, i2)), (p02));        \
	    vis_st_u8(vis_write_lo(v12, PCALC8(v12, i2)), (p12));        \
	    vis_st_u8(vis_write_lo(v10, PCALC8(v10, i0)), (p10));        \
	    vis_st_u8(vis_write_lo(v11, PCALC8(v11, i1)), (p11));        \
	    vis_st_u8(vis_write_lo(v21, PCALC8(v21, i1)), (p21));        \
	    vis_st_u8(vis_write_lo(v20, PCALC8(v20, i0)), (p20));        \
	    vis_st_u8(vis_write_lo(v22, PCALC8(v22, i2)), (p22));        \
	    vis_st_u8(vis_write_lo(v32, PCALC8(v32, i2)), (p32));        \
	    vis_st_u8(vis_write_lo(v30, PCALC8(v30, i0)), (p30));        \
	    vis_st_u8(vis_write_lo(v31, PCALC8(v31, i1)), (p31));        \
	}

/* *********************************************************** */

#define	PROC_OPTI32(p01, p00, p02, p11, p10, p12, p21, p20,              \
	p22, p31, p30, p32, i1, i0, i2)                                  \
	{                                                                \
	    mlib_f32 v01, v00, v02, v11, v10, v12, v21, v20, v22, v31,   \
		v30, v32;                                                \
	    v01 = *(mlib_f32 *)(p01);                                    \
	    v00 = *(mlib_f32 *)(p00);                                    \
	    v02 = *(mlib_f32 *)(p02);                                    \
	    v12 = *(mlib_f32 *)(p12);                                    \
	    v10 = *(mlib_f32 *)(p10);                                    \
	    v11 = *(mlib_f32 *)(p11);                                    \
	    v21 = *(mlib_f32 *)(p21);                                    \
	    v20 = *(mlib_f32 *)(p20);                                    \
	    v22 = *(mlib_f32 *)(p22);                                    \
	    v32 = *(mlib_f32 *)(p32);                                    \
	    v30 = *(mlib_f32 *)(p30);                                    \
	    v31 = *(mlib_f32 *)(p31);                                    \
	    *(mlib_f32 *)(p01) = PCALC32(v01, i1);                       \
	    *(mlib_f32 *)(p00) = PCALC32(v00, i0);                       \
	    *(mlib_f32 *)(p02) = PCALC32(v02, i2);                       \
	    *(mlib_f32 *)(p12) = PCALC32(v12, i2);                       \
	    *(mlib_f32 *)(p10) = PCALC32(v10, i0);                       \
	    *(mlib_f32 *)(p11) = PCALC32(v11, i1);                       \
	    *(mlib_f32 *)(p21) = PCALC32(v21, i1);                       \
	    *(mlib_f32 *)(p20) = PCALC32(v20, i0);                       \
	    *(mlib_f32 *)(p22) = PCALC32(v22, i2);                       \
	    *(mlib_f32 *)(p32) = PCALC32(v32, i2);                       \
	    *(mlib_f32 *)(p30) = PCALC32(v30, i0);                       \
	    *(mlib_f32 *)(p31) = PCALC32(v31, i1);                       \
	}

/* *********************************************************** */

#define	IPPROC	IPPROC8
#define	PROC_OPTI	PROC_OPTI8

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawCircle_A_8(
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

	mlib_s32 ind0, ind1, ind2, mdel, k;
	mlib_d64 coef0, coef1, coef2;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r <= 0 || y + r >= height || y - r <= 0)
		doclip = 1;

	vis_write_gsr((1 << 3) + 0);
	cf = vis_to_float(c);

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	PUT_CLIP_R0;
	PUT_CLIP_R1;

	k = (0x100000 / r);
	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line2 += stride;

	mdel = (del ^ mask) - mask;
	ind0 = cy - mdel;
	ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
	ind1 = cy + del + ((cy * 2) & mask);
	ind2 = cy - del + ((cy * 2) & ~mask);

	ind0 = (ind0 * k) >> 13;
	ind1 = (ind1 * k) >> 13;
	ind2 = (ind2 * k) >> 13;

	if (doclip) {

		PUT_CLIP_FIRST;
		line1 -= stride;

		coef0 = LDCOEFD(ind0);
		coef1 = LDCOEFD(ind1);
		coef2 = LDCOEFD(ind2);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);
		ccoef2 = CALC_CCOEF(coef2);

		while (cx + 1 < cy) {

			PUT_CLIP_TB;
			PUT_CLIP_LR;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;

			mdel = (del ^ mask) - mask;
			ind0 = cy - mdel;
			ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
			ind1 = cy + del + ((cy * 2) & mask);
			ind2 = cy - del + ((cy * 2) & ~mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;
			ind2 = (ind2 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			coef0 = LDCOEFD(ind0);
			coef1 = LDCOEFD(ind1);
			coef2 = LDCOEFD(ind2);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);
			ccoef2 = CALC_CCOEF(coef2);
		}

		PUT_CLIP_LAST;

		if (cx < cy) {
			PUT_CLIP_LR;
		}
	} else {
		PUT_NOCLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEFD(ind0), coef1 = LDCOEFD(ind1), coef2 =
			    LDCOEFD(ind2);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);
			ccoef2 = CALC_CCOEF(coef2);

			PUT_NOCLIP_TB;
			PUT_NOCLIP_LR;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;

			mdel = (del ^ mask) - mask;
			ind0 = cy - mdel;
			ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
			ind1 = cy + del + ((cy * 2) & mask);
			ind2 = cy - del + ((cy * 2) & ~mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;
			ind2 = (ind2 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		coef0 = LDCOEFD(ind0);
		coef1 = LDCOEFD(ind1);
		coef2 = LDCOEFD(ind2);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);
		ccoef2 = CALC_CCOEF(coef2);

		PUT_NOCLIP_LAST;

		if (cx < cy) {
			PUT_NOCLIP_LR;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  PROC_OPTI
#undef  IPPROC

/* *********************************************************** */

#define	PROC_OPTI	PROC_OPTI32
#define	IPPROC	PPROC32

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawCircle_A_32(
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

	mlib_s32 ind0, ind1, ind2, mdel, k;
	mlib_d64 coef0, coef1, coef2;
	mlib_d64 ccoef0, ccoef1, ccoef2;
	mlib_d64 d_one = ((mlib_d64 *)mlib_v_TabAlias)[0];
	mlib_f32 cf;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r <= 0 || y + r >= height || y - r <= 0)
		doclip = 1;

	vis_write_gsr((1 << 3) + 0);
	c  |= 0xFF000000;
	cf  = vis_to_float(c);

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	PUT_CLIP_R0;
	PUT_CLIP_R1;

	k = (0x100000 / r);
	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line2 += stride;

	mdel = (del ^ mask) - mask;
	ind0 = cy - mdel;
	ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
	ind1 = cy + del + ((cy * 2) & mask);
	ind2 = cy - del + ((cy * 2) & ~mask);

	ind0 = (ind0 * k) >> 13;
	ind1 = (ind1 * k) >> 13;
	ind2 = (ind2 * k) >> 13;

	if (doclip) {

		PUT_CLIP_FIRST;
		line1 -= stride;

		coef0 = LDCOEFD(ind0);
		coef1 = LDCOEFD(ind1);
		coef2 = LDCOEFD(ind2);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);
		ccoef2 = CALC_CCOEF(coef2);

		while (cx + 1 < cy) {

			PUT_CLIP_TB;
			PUT_CLIP_LR;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;

			mdel = (del ^ mask) - mask;
			ind0 = cy - mdel;
			ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
			ind1 = cy + del + ((cy * 2) & mask);
			ind2 = cy - del + ((cy * 2) & ~mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;
			ind2 = (ind2 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			coef0 = LDCOEFD(ind0);
			coef1 = LDCOEFD(ind1);
			coef2 = LDCOEFD(ind2);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);
			ccoef2 = CALC_CCOEF(coef2);
		}

		PUT_CLIP_LAST;

		if (cx < cy) {
			PUT_CLIP_LR;
		}
	} else {
		PUT_NOCLIP_FIRST;
		line1 -= stride;

		coef0 = LDCOEFD(ind0);
		coef1 = LDCOEFD(ind1);
		coef2 = LDCOEFD(ind2);
		ccoef0 = CALC_CCOEF(coef0);
		ccoef1 = CALC_CCOEF(coef1);
		ccoef2 = CALC_CCOEF(coef2);

		while (cx + 1 < cy) {

			PUT_NOCLIP_TB;
			PUT_NOCLIP_LR;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;

			mdel = (del ^ mask) - mask;
			ind0 = cy - mdel;
			ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
			ind1 = cy + del + ((cy * 2) & mask);
			ind2 = cy - del + ((cy * 2) & ~mask);

			ind0 = (ind0 * k) >> 13;
			ind1 = (ind1 * k) >> 13;
			ind2 = (ind2 * k) >> 13;

			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			coef0 = LDCOEFD(ind0);
			coef1 = LDCOEFD(ind1);
			coef2 = LDCOEFD(ind2);
			ccoef0 = CALC_CCOEF(coef0);
			ccoef1 = CALC_CCOEF(coef1);
			ccoef2 = CALC_CCOEF(coef2);
		}

		PUT_NOCLIP_LAST;

		if (cx < cy) {
			PUT_NOCLIP_LR;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
