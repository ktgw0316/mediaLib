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

#pragma ident	"@(#)mlib_c_GraphicsDrawCircle_A.c	9.3	07/10/09 SMI"

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
 *
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
 *
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsMacro.h>
#include <mlib_c_GraphicsTabAlias.h>

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

#define	ONE	0x4000

#define	COEF_TYP	mlib_s32

#define	LDCOEF(ind)	(COEF_TYP)(mlib_c_TabAlias_S16[ind])

/* *********************************************************** */

#define	PPROC	PPROC8
#define	PPROC_3	PPROC8_3

/* *********************************************************** */

#define	PCALC8(val, cind)	((val) + (((c - (val)) * coef##cind) >> 14))
#define	PPROC8(ptr, cind)	*(ptr) = PCALC8(*(ptr), cind)

/* *********************************************************** */

#define	PPROC8_3(p0, p1, p2, i0, i1, i2)                        \
	{                                                       \
	    COEF_TYP v0 = *(p0), v1 = *(p1), v2 = *(p2);        \
	                                                        \
	    *(p0) = PCALC8(v0, i0);                             \
	    *(p1) = PCALC8(v1, i1);                             \
	    *(p2) = PCALC8(v2, i2);                             \
	}

/* *********************************************************** */

#define	PCALC32(val, cind, cfind)                               \
	((val) + (((cf##cfind - (val)) * coef##cind) >> 14))

#ifdef _LITTLE_ENDIAN

#define	PPROC32(ptr, cind)                                      \
	{                                                       \
	    mlib_u8 *p = (mlib_u8 *)(ptr);                      \
	    COEF_TYP v2 = p[2], v1 = p[1], v0 = p[0];           \
	                                                        \
	    p[3] = cfalpha;                                     \
	    p[2] = PCALC32(v2, cind, 2);                        \
	    p[1] = PCALC32(v1, cind, 1);                        \
	    p[0] = PCALC32(v0, cind, 0);                        \
	}

#else

#define	PPROC32(ptr, cind)                                      \
	{                                                       \
	    mlib_u8 *p = (mlib_u8 *)(ptr);                      \
	    COEF_TYP v2 = p[1], v1 = p[2], v0 = p[3];           \
	                                                        \
	    p[0] = cfalpha;                                     \
	    p[1] = PCALC32(v2, cind, 2);                        \
	    p[2] = PCALC32(v1, cind, 1);                        \
	    p[3] = PCALC32(v0, cind, 0);                        \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	PPROC32_3(p0, p1, p2, i0, i1, i2)                       \
	PPROC32(p0, i0);                                        \
	PPROC32(p1, i1);                                        \
	PPROC32(p2, i2)

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
	    coef1 = LDCOEF(276);                                \
	    coef2 = LDCOEF(148);                                \
	    if (INSIDEY(y)) {                                   \
		if (INSIDEX(x - 2))                             \
		    PPROC(line1 - 2, 1);                        \
		if (INSIDEX(x - 1))                             \
		    line1[-1] = c;                              \
		if (INSIDEX(x))                                 \
		    PPROC(line1, 1);                            \
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
	    if (INSIDEY(y - (cy - 1)))                          \
		PPROC(line0 - cx + stride, 2);                  \
	    if (INSIDEY(y + (cy + 1)))                          \
		PPROC(line3 - cx + stride, 1);                  \
	    if (INSIDEY(y + (cy + 0)))                          \
		PPROC(line3 - cx, 0);                           \
	    if (INSIDEY(y + (cy - 1)))                          \
		PPROC(line3 - cx - stride, 2);                  \
	}                                                       \
	if (INSIDEX(x + cx)) {                                  \
	    if (INSIDEY(y - (cy + 1)))                          \
		PPROC(line0 + cx - stride, 1);                  \
	    if (INSIDEY(y - (cy + 0)))                          \
		PPROC(line0 + cx, 0);                           \
	    if (INSIDEY(y - (cy - 1)))                          \
		PPROC(line0 + cx + stride, 2);                  \
	    if (INSIDEY(y + (cy + 1)))                          \
		PPROC(line3 + cx + stride, 1);                  \
	    if (INSIDEY(y + (cy + 0)))                          \
		PPROC(line3 + cx, 0);                           \
	    if (INSIDEY(y + (cy - 1)))                          \
		PPROC(line3 + cx - stride, 2);                  \
	}

/* *********************************************************** */

#define	PUT_CLIP_LR                                             \
	if (INSIDEY(y - (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		PPROC(line1 - (cy + 1), 1);                     \
	    if (INSIDEX(x - (cy + 0)))                          \
		PPROC(line1 - cy, 0);                           \
	    if (INSIDEX(x - (cy - 1)))                          \
		PPROC(line1 - (cy - 1), 2);                     \
	    if (INSIDEX(x + (cy + 1)))                          \
		PPROC(line1 + (cy + 1), 1);                     \
	    if (INSIDEX(x + (cy + 0)))                          \
		PPROC(line1 + cy, 0);                           \
	    if (INSIDEX(x + (cy - 1)))                          \
		PPROC(line1 + (cy - 1), 2);                     \
	}                                                       \
	if (INSIDEY(y + (cx + 0))) {                            \
	    if (INSIDEX(x - (cy + 1)))                          \
		PPROC(line2 - (cy + 1), 1);                     \
	    if (INSIDEX(x - (cy + 0)))                          \
		PPROC(line2 - cy, 0);                           \
	    if (INSIDEX(x - (cy - 1)))                          \
		PPROC(line2 - (cy - 1), 2);                     \
	    if (INSIDEX(x + (cy + 1)))                          \
		PPROC(line2 + (cy + 1), 1);                     \
	    if (INSIDEX(x + (cy + 0)))                          \
		PPROC(line2 + cy, 0);                           \
	    if (INSIDEX(x + (cy - 1)))                          \
		PPROC(line2 + (cy - 1), 2);                     \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_TB                                                      \
	PPROC_3(line3 - cx - stride, line3 - cx, line3 - cx + stride, 2,   \
	    0, 1);                                                         \
	PPROC_3(line3 + cx - stride, line3 + cx, line3 + cx + stride, 2,   \
	    0, 1);                                                         \
	PPROC_3(line0 - cx - stride, line0 - cx, line0 - cx + stride, 1,   \
	    0, 2);                                                         \
	PPROC_3(line0 + cx - stride, line0 + cx, line0 + cx + stride, 1,   \
	    0, 2)

/* *********************************************************** */

#define	PUT_NOCLIP_LR                                                   \
	PPROC_3(line1 - cy - 1, line1 - cy, line1 - cy + 1, 1, 0, 2);   \
	PPROC_3(line1 + cy - 1, line1 + cy, line1 + cy + 1, 2, 0, 1);   \
	PPROC_3(line2 - cy - 1, line2 - cy, line2 - cy + 1, 1, 0, 2);   \
	PPROC_3(line2 + cy - 1, line2 + cy, line2 + cy + 1, 2, 0, 1)

/* *********************************************************** */

#define	PUT_NOCLIP_FIRST                                        \
	coef1 = LDCOEF(255);                                    \
	line0[0] = c;                                           \
	PPROC(line0 - stride, 1);                               \
	PPROC(line0 + stride, 1);                               \
	line1[-r] = c;                                          \
	PPROC(line1 - r - 1, 1);                                \
	PPROC(line1 - r + 1, 1);                                \
	line1[r] = c;                                           \
	PPROC(line1 + r + 1, 1);                                \
	PPROC(line1 + r - 1, 1);                                \
	line3[0] = c;                                           \
	PPROC(line3 + stride, 1);                               \
	PPROC(line3 - stride, 1)

/* *********************************************************** */

#define	PUT_CLIP_FIRST                                          \
	coef1 = LDCOEF(255);                                    \
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
	    if (INSIDEX(x - (r + 1)))                           \
		PPROC(line1 - (r + 1), 1);                      \
	    if (INSIDEX(x - (r + 0)))                           \
		line1[-r] = c;                                  \
	    if (INSIDEX(x - (r - 1)))                           \
		PPROC(line1 - (r - 1), 1);                      \
	    if (INSIDEX(x + (r + 1)))                           \
		PPROC(line1 + (r + 1), 1);                      \
	    if (INSIDEX(x + (r + 0)))                           \
		line1[r] = c;                                   \
	    if (INSIDEX(x + (r - 1)))                           \
		PPROC(line1 + (r - 1), 1);                      \
	}

/* *********************************************************** */

#define	PUT_NOCLIP_LAST                                                  \
	PPROC_3(line3 - cx - 1, line3 - cx, line3 - cx + stride, 1, 0,   \
	    1);                                                          \
	PPROC_3(line3 + cx, line3 + cx + 1, line3 + cx + stride, 0, 1,   \
	    1);                                                          \
	PPROC_3(line0 - cx - stride, line0 - cx - 1, line0 - cx, 1, 1,   \
	    0);                                                          \
	PPROC_3(line0 + cx - stride, line0 + cx, line0 + cx + 1, 1, 0,   \
	    1)

/* *********************************************************** */

#define	PUT_CLIP_LAST                                           \
	if (INSIDEY(y - (cy + 1))) {                            \
	    if (INSIDEX(x - (cx + 0)))                          \
		PPROC(line0 - stride - cx, 1);                  \
	    if (INSIDEX(x + (cx + 0)))                          \
		PPROC(line0 - stride + cx, 1);                  \
	}                                                       \
	if (INSIDEY(y - (cy + 0))) {                            \
	    if (INSIDEX(x - (cx + 1)))                          \
		PPROC(line0 - 1 - cx, 1);                       \
	    if (INSIDEX(x - (cx + 0)))                          \
		PPROC(line0 - 0 - cx, 0);                       \
	    if (INSIDEX(x + (cx + 0)))                          \
		PPROC(line0 - 0 + cx, 0);                       \
	    if (INSIDEX(x + (cx + 1)))                          \
		PPROC(line0 + 1 + cx, 1);                       \
	}                                                       \
	if (INSIDEY(y + (cy + 0))) {                            \
	    if (INSIDEX(x - (cx + 1)))                          \
		PPROC(line3 - 1 - cx, 1);                       \
	    if (INSIDEX(x - (cx + 0)))                          \
		PPROC(line3 - 0 - cx, 0);                       \
	    if (INSIDEX(x + (cx + 0)))                          \
		PPROC(line3 - 0 + cx, 0);                       \
	    if (INSIDEX(x + (cx + 1)))                          \
		PPROC(line3 + 1 + cx, 1);                       \
	}                                                       \
	if (INSIDEY(y + (cy + 1))) {                            \
	    if (INSIDEX(x - (cx + 0)))                          \
		PPROC(line3 + stride - cx, 1);                  \
	    if (INSIDEX(x + (cx + 0)))                          \
		PPROC(line3 + stride + cx, 1);                  \
	}

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
	COEF_TYP coef0, coef1, coef2;

	if (!data)
		return (MLIB_NULLPOINTER);
	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r <= 0 || y + r >= height || y - r <= 0)
		doclip = 1;

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

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			coef2 = LDCOEF(ind2);

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
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		coef2 = LDCOEF(ind2);

		PUT_CLIP_LAST;

		if (cx < cy) {
			PUT_CLIP_LR;
		}
	} else {

		PUT_NOCLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			coef2 = LDCOEF(ind2);

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

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		coef2 = LDCOEF(ind2);

		PUT_NOCLIP_LAST;

		if (cx < cy) {
			PUT_NOCLIP_LR;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  PPROC

/* *********************************************************** */

#define	PPROC	PPROC32
#undef  PPROC_3

/* *********************************************************** */

#define	PPROC_3	PPROC32_3

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

/* mlib_s32 doclip = 0; */

	mlib_s32 ind0, ind1, ind2, mdel, k;
	COEF_TYP coef0, coef1, coef2;
	COEF_TYP cf0 = c & 0xff, cf1 = (c & 0xff00) >> 8, cf2 =
	    (c & 0xff0000) >> 16;
	mlib_u8 cfalpha = 0xff;

	MLIB_GRAPHICS_COLOR_32(c);
	if (!data)
		return (MLIB_NULLPOINTER);
	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

/*
 * if(x + r >= width || x - r <= 0 || y + r >= height ||
 * y - r <= 0) doclip = 1;
 */

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

/* if(doclip) */
	{
		PUT_CLIP_FIRST;
		line1 -= stride;

		while (cx + 1 < cy) {

			coef0 = LDCOEF(ind0);
			coef1 = LDCOEF(ind1);
			coef2 = LDCOEF(ind2);

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
		}

		coef0 = LDCOEF(ind0);
		coef1 = LDCOEF(ind1);
		coef2 = LDCOEF(ind2);

		PUT_CLIP_LAST;

		if (cx < cy) {
			PUT_CLIP_LR;
		}
	}

/* this part unfortunately only slows performance so it is commented */
/*
 * else {
 *  PUT_NOCLIP_FIRST;
 *  line1 -= stride;
 *
 *  while (cx + 1 < cy) {
 *
 *    coef0 = LDCOEF(ind0);
 *    coef1 = LDCOEF(ind1);
 *    coef2 = LDCOEF(ind2);
 *
 *    PUT_NOCLIP_TB;
 *    PUT_NOCLIP_LR;
 *
 *    del += (2 * cx +1) -((2 * cy) & ~mask);
 *
 *    mask = del >> 31;
 *    cy += ~mask;
 *
 *    mdel = (del ^ mask) - mask;
 *    ind0 = cy - mdel;
 *    ind0 = (ind0 ^ (ind0 >> 31)) - (ind0 >> 31);
 *    ind1 = cy + del + ((cy * 2) & mask);
 *    ind2 = cy - del + ((cy * 2) & ~mask);
 *
 *    ind0 = (ind0 * k) >> 13;
 *    ind1 = (ind1 * k) >> 13;
 *    ind2 = (ind2 * k) >> 13;
 *
 *    ++cx;
 *    line1 -= stride;
 *    line2 += stride;
 *    line0 += stride & ~mask;
 *    line3 -= stride & ~mask;
 *  }
 *
 *  coef0 = LDCOEF(ind0);
 *  coef1 = LDCOEF(ind1);
 *  coef2 = LDCOEF(ind2);
 *
 *  PUT_NOCLIP_LAST;
 *
 *  if(cx < cy) {
 *    PUT_NOCLIP_LR;
 *  }
 * }
 *
 */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
