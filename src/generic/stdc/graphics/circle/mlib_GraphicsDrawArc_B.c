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

#pragma ident	"@(#)mlib_GraphicsDrawArc_B.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *    mlib_GraphicsDrawArc_B_[8 | 32] - draw arc with alpha blending
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsDrawArc_B_8 (mlib_image *buffer,
 *                                          mlib_s16   x,
 *                                          mlib_s16   y,
 *                                          mlib_s32   r,
 *                                          mlib_f32   t1,
 *                                          mlib_f32   t2,
 *                                          mlib_s32   c,
 *                                          mlib_s32   a)
 *
 *    mlib_status mlib_GraphicsDrawArc_B_32(mlib_image *buffer,
 *                                          mlib_s16   x,
 *                                          mlib_s16   y,
 *                                          mlib_s32   r,
 *                                          mlib_f32   t1,
 *                                          mlib_f32   t2,
 *                                          mlib_s32   c,
 *                                          mlib_s32   a)
 *
 * ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the arc center.
 *    y       Y coordinate of the arc center.
 *    r       Arc radius.
 *    t1, t2  Start and end angles of the arc in radians.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 * DESCRIPTION
 *    Function draws a circle arc of radius r and center (x, y)
 *    in image pointed by buffer, by alpha blending with color c,
 *    alpha value a.
 *    If abs(t1 - t2) >= 2 * PI, result is circle.
 *    If t1 > t2 t2 = t2 + 2*PI is used.
 *
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image.
 *
 *    Pixels falling outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_SysMath.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawArc_B_32 = __mlib_GraphicsDrawArc_B_32
#pragma weak mlib_GraphicsDrawArc_B_8 = __mlib_GraphicsDrawArc_B_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawArc_B_32) mlib_GraphicsDrawArc_B_32
    __attribute__((weak, alias("__mlib_GraphicsDrawArc_B_32")));
__typeof__(__mlib_GraphicsDrawArc_B_8) mlib_GraphicsDrawArc_B_8
    __attribute__((weak, alias("__mlib_GraphicsDrawArc_B_8")));

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

#define	CHECK_INTERSECTION                                               \
	if (x - r > width || x + r < 0 || y - r > height || y + r < 0)   \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	FILL_BUF                                                \
	cy = r;                                                 \
	cx = 1;                                                 \
	del = 1 - r;                                            \
	mask = 0;                                               \
	buf[0][0] = r;                                          \
	buf[0][1] = 0;                                          \
	while (cx < cy) {                                       \
	    buf[cx][0] = cy;                                    \
	    buf[cx][1] = mask;                                  \
	    del += (2 * cx + 1) - ((2 * cy) & mask);            \
	    mask = ~(del >> 31);                                \
	    cy += mask;                                         \
	    ++cx;                                               \
	}                                                       \
	                                                        \
	if (cx == cy && mask) {                                 \
	    buf[cx][0] = cy;                                    \
	    buf[cx][1] = mask;                                  \
	    count = cx;                                         \
	} else {                                                \
	    buf[cx][0] = cx - 1;                                \
	    buf[cx][1] = -1;                                    \
	    count = cx - 1;                                     \
	}                                                       \
	                                                        \
	scount = cx - 1

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
		sum = buf[sin1][0] + sin1;                      \
	    else                                                \
		sum = buf[cos1][0] + cos1;                      \
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
		sum = buf[sin2][0] + sin2;                      \
	    else                                                \
		sum = buf[cos2][0] + cos2;                      \
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
	if (y - r < 0) {                                        \
	    mask = (y - scount) >> 31;                          \
	    flagd &= ~(mask & 0x06);                            \
	    flag1 = (mask & 0x09);                              \
	    flag1 |= (~mask & 0x06);                            \
	    mask = (y - 1) >> 31;                               \
	    flagd &= ~(mask & 0x09);                            \
	    flag1 |= (mask & 0x90);                             \
	    mask = (y + scount) >> 31;                          \
	    flagd &= ~(mask & 0x90);                            \
	    flag1 |= (mask & 0x60);                             \
	}                                                       \
	if (y + r > height) {                                   \
	    mask = (height - (y + scount)) >> 31;               \
	    flagd &= ~(mask & 0x60);                            \
	    flag2 = (mask & 0x90);                              \
	    flag2 |= (~mask & 0x60);                            \
	    mask = (height - y) >> 31;                          \
	    flagd &= ~(mask & 0x90);                            \
	    flag2 |= (mask & 0x09);                             \
	    mask = (height - (y - scount)) >> 31;               \
	    flagd &= ~(mask & 0x09);                            \
	    flag2 |= (mask & 0x06);                             \
	}                                                       \
	if (x - r < 0) {                                        \
	    mask = (x - count) >> 31;                           \
	    flagd &= ~(mask & 0x18);                            \
	    flag3 = (mask & 0x24);                              \
	    flag3 |= (~mask & 0x18);                            \
	    mask = (x - 0) >> 31;                               \
	    flagd &= ~(mask & 0x24);                            \
	    flag3 |= (mask & 0x42);                             \
	    mask = (x + count) >> 31;                           \
	    flagd &= ~(mask & 0x42);                            \
	    flag3 |= (mask & 0x81);                             \
	}                                                       \
	if (x + r > width) {                                    \
	    mask = (width - (x + count)) >> 31;                 \
	    flagd &= ~(mask & 0x81);                            \
	    flag4 = (mask & 0x42);                              \
	    flag4 |= (~mask & 0x81);                            \
	    mask = (width - (x - 0)) >> 31;                     \
	    flagd &= ~(mask & 0x42);                            \
	    flag4 |= (mask & 0x24);                             \
	    mask = (width - (x - count)) >> 31;                 \
	    flagd &= ~(mask & 0x24);                            \
	    flag4 |= (mask & 0x18);                             \
	}                                                       \
	flagclip = flag1 | flag2 | flag3 | flag4

/* *********************************************************** */

#define	PROC_CL_OCT2                                            \
	if (flagd & (0x001 << 2)) {                             \
	    start = 0;                                          \
	    end = count;                                        \
	    if (flagclip & (0x001 << 2)) {                      \
		if (flag3 & (0x001 << 2))                       \
		    end = x;                                    \
		if (flag4 & (0x001 << 2))                       \
		    start = x - width;                          \
		if (flag1 & (0x001 << 2))                       \
		    while (buf[start][0] > y)                   \
			++start;                                \
		if (flag2 & (0x001 << 2))                       \
		    while (buf[end][0] < y - height)            \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 2))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cx = (cos1 < start) ? start : cos1;         \
		    cy = buf[cx][0];                            \
		    line = line0 - (cy) * stride;               \
		    for (; cx <= end; ++cx) {                   \
			mask = buf[cx + 1][1];                  \
			BLEND(line - cx)                        \
		    }                                           \
		    if (cos2 < end)                             \
			end = cos2;                             \
		} else {                                        \
		    if (oct1 == 2 && start < cos1)              \
			start = cos1;                           \
		    if ((oct2 & 7) == 2 && end > cos2)          \
			end = cos2;                             \
		}                                               \
	    }                                                   \
	    line = line0 - buf[start][0] * stride;              \
	    for (; start <= end; start++) {                     \
		mask = buf[start + 1][1];                       \
		BLEND(line - start)                             \
		line += (stride & mask);                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT1                                            \
	if (flagd & (0x001 << 1)) {                             \
	    start = 1;                                          \
	    end = count;                                        \
	    if (flagclip & (0x001 << 1)) {                      \
		if (flag3 & (0x001 << 1))                       \
		    start = -x;                                 \
		if (flag4 & (0x001 << 1))                       \
		    end = width - x;                            \
		if (flag1 & (0x001 << 1))                       \
		    while (buf[start][0] > y)                   \
			++start;                                \
		if (flag2 & (0x001 << 1))                       \
		    while (buf[end][0] < y - height)            \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 1))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cx = (cos2 < start) ? start : cos2;         \
		    cy = buf[cx][0];                            \
		    line = line0 - (cy) * stride;               \
		    for (; cx <= end; ++cx) {                   \
			mask = buf[cx + 1][1];                  \
			BLEND(line + cx)                        \
			line += (stride & mask);                \
		    }                                           \
		    if (cos1 < end)                             \
			end = cos1;                             \
		} else {                                        \
		    if (oct1 == 1 && end > cos1)                \
			end = cos1;                             \
		    if ((oct2 & 7) == 1 && start < cos2)        \
			start = cos2;                           \
		}                                               \
	    }                                                   \
	    line = line0 - buf[start][0] * stride;              \
	    for (; start <= end; start++) {                     \
		mask = buf[start + 1][1];                       \
		BLEND(line + start)                             \
		line += (stride & mask);                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT3                                            \
	if (flagd & (0x001 << 3)) {                             \
	    start = 1;                                          \
	    end = scount;                                       \
	    if (flagclip & (0x001 << 3)) {                      \
		if (flag1 & (0x001 << 3))                       \
		    end = y;                                    \
		if (flag2 & (0x001 << 3))                       \
		    start = y - height;                         \
		if (flag3 & (0x001 << 3))                       \
		    while (buf[start][0] > x)                   \
			++start;                                \
		if (flag4 & (0x001 << 3))                       \
		    while (buf[end][0] < x - width)             \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 3))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cy = (sin2 < start) ? start : sin2;         \
		    cx = buf[cy][0];                            \
		    line = line0 - (cy) * stride;               \
		    for (; cy <= end; ++cy) {                   \
			BLEND(line - cx)                        \
			cx = buf[cy + 1][0];                    \
			line -= stride;                         \
		    }                                           \
		    if (sin1 < end)                             \
			end = sin1;                             \
		} else {                                        \
		    if (oct1 == 3 && end > sin1)                \
			end = sin1;                             \
		    if ((oct2 & 7) == 3 && start < sin2)        \
			start = sin2;                           \
		}                                               \
	    }                                                   \
	    line = line0 - start * stride;                      \
	    cx = buf[start][0];                                 \
	    for (; start <= end; start++) {                     \
		BLEND(line - cx)                                \
		cx = buf[start + 1][0];                         \
		line -= stride;                                 \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT0                                            \
	if (flagd & (0x001 << 0)) {                             \
	    start = 1;                                          \
	    end = scount;                                       \
	    if (flagclip & (0x001 << 0)) {                      \
		if (flag1 & (0x001 << 0))                       \
		    end = y;                                    \
		if (flag2 & (0x001 << 0))                       \
		    start = y - height;                         \
		if (flag4 & (0x001 << 0))                       \
		    while (buf[start][0] > width - x)           \
			++start;                                \
		if (flag3 & (0x001 << 0))                       \
		    while (buf[end][0] < -x)                    \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 0))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cy = (sin1 < start) ? start : sin1;         \
		    cx = buf[cy][0];                            \
		    line = line0 - (cy) * stride;               \
		    for (; cy <= end; ++cy) {                   \
			BLEND(line + cx)                        \
			cx = buf[cy + 1][0];                    \
			line -= stride;                         \
		    }                                           \
		    if (sin2 < end)                             \
			end = sin2;                             \
		} else {                                        \
		    if (oct1 == 0 && start < sin1)              \
			start = sin1;                           \
		    if ((oct2 & 7) == 0 && end > sin2)          \
			end = sin2;                             \
		}                                               \
	    }                                                   \
	    line = line0 - start * stride;                      \
	    cx = buf[start][0];                                 \
	    for (; start <= end; start++) {                     \
		BLEND(line + cx)                                \
		cx = buf[start + 1][0];                         \
		line -= stride;                                 \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT4                                            \
	if (flagd & (0x001 << 4)) {                             \
	    start = 0;                                          \
	    end = scount;                                       \
	    if (flagclip & (0x001 << 4)) {                      \
		if (flag1 & (0x001 << 4))                       \
		    start = -y;                                 \
		if (flag2 & (0x001 << 4))                       \
		    end = height - y;                           \
		if (flag3 & (0x001 << 4))                       \
		    while (buf[start][0] > x)                   \
			++start;                                \
		if (flag4 & (0x001 << 4))                       \
		    while (buf[end][0] < x - width)             \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 4))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cy = (sin1 < start) ? start : sin1;         \
		    cx = buf[cy][0];                            \
		    line = line0 + (cy) * stride;               \
		    for (; cy <= end; ++cy) {                   \
			BLEND(line - cx)                        \
			cx = buf[cy + 1][0];                    \
			line += stride;                         \
		    }                                           \
		    if (sin2 < end)                             \
			end = sin2;                             \
		} else {                                        \
		    if (oct1 == 4 && start < sin1)              \
			start = sin1;                           \
		    if ((oct2 & 7) == 4 && end > sin2)          \
			end = sin2;                             \
		}                                               \
	    }                                                   \
	    line = line0 + start * stride;                      \
	    cx = buf[start][0];                                 \
	    for (; start <= end; start++) {                     \
		BLEND(line - cx)                                \
		cx = buf[start + 1][0];                         \
		line += stride;                                 \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT7                                            \
	if (flagd & (0x001 << 7)) {                             \
	    start = 0;                                          \
	    end = scount;                                       \
	    if (flagclip & (0x001 << 7)) {                      \
		if (flag1 & (0x001 << 7))                       \
		    start = -y;                                 \
		if (flag2 & (0x001 << 7))                       \
		    end = height - y;                           \
		if (flag4 & (0x001 << 7))                       \
		    while (buf[start][0] > width - x)           \
			++start;                                \
		if (flag3 & (0x001 << 7))                       \
		    while (buf[end][0] < -x)                    \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 7))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cy = (sin2 < start) ? start : sin2;         \
		    cx = buf[cy][0];                            \
		    line = line0 + (cy) * stride;               \
		    for (; cy <= end; ++cy) {                   \
			BLEND(line + cx)                        \
			cx = buf[cy + 1][0];                    \
			line += stride;                         \
		    }                                           \
		    if (sin1 < end)                             \
			end = sin1;                             \
		} else {                                        \
		    if (oct1 == 7 && end > sin1)                \
			end = sin1;                             \
		    if ((oct2 & 7) == 7 && start < sin2)        \
			start = sin2;                           \
		}                                               \
	    }                                                   \
	    line = line0 + start * stride;                      \
	    cx = buf[start][0];                                 \
	    for (; start <= end; start++) {                     \
		BLEND(line + cx)                                \
		cx = buf[start + 1][0];                         \
		line += stride;                                 \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT5                                            \
	if (flagd & (0x001 << 5)) {                             \
	    start = 0;                                          \
	    end = count;                                        \
	    if (flagclip & (0x001 << 5)) {                      \
		if (flag3 & (0x001 << 5))                       \
		    end = x;                                    \
		if (flag4 & (0x001 << 5))                       \
		    start = x - width;                          \
		if (flag2 & (0x001 << 5))                       \
		    while (buf[start][0] > height - y)          \
			++start;                                \
		if (flag1 & (0x001 << 5))                       \
		    while (buf[end][0] < -y)                    \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 5))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cx = (cos2 < start) ? start : cos2;         \
		    cy = buf[cx][0];                            \
		    line = line0 + (cy) * stride;               \
		    for (; cx <= end; ++cx) {                   \
			mask = buf[cx + 1][1];                  \
			BLEND(line - cx)                        \
			line -= (stride & mask);                \
		    }                                           \
		    if (cos1 < end)                             \
			end = cos1;                             \
		} else {                                        \
		    if (oct1 == 5 && end > cos1)                \
			end = cos1;                             \
		    if ((oct2 & 7) == 5 && start < cos2)        \
			start = cos2;                           \
		}                                               \
	    }                                                   \
	    line = line0 + buf[start][0] * stride;              \
	    for (; start <= end; start++) {                     \
		mask = buf[start + 1][1];                       \
		BLEND(line - start)                             \
		line -= (stride & mask);                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	PROC_CL_OCT6                                            \
	if (flagd & (0x001 << 6)) {                             \
	    start = 1;                                          \
	    end = count;                                        \
	    if (flagclip & (0x001 << 6)) {                      \
		if (flag3 & (0x001 << 6))                       \
		    start = -x;                                 \
		if (flag4 & (0x001 << 6))                       \
		    end = width - x;                            \
		if (flag2 & (0x001 << 6))                       \
		    while (buf[start][0] > height - y)          \
			++start;                                \
		if (flag1 & (0x001 << 6))                       \
		    while (buf[end][0] < -y)                    \
			--end;                                  \
	    }                                                   \
	    if (!(flagc & (0x001 << 6))) {                      \
		if (oct1 + 8 == oct2) {                         \
		    cx = (cos1 < start) ? start : cos1;         \
		    cy = buf[cx][0];                            \
		    line = line0 + (cy) * stride;               \
		    for (; cx <= end; ++cx) {                   \
			mask = buf[cx + 1][1];                  \
			BLEND(line + cx)                        \
			line -= (stride & mask);                \
		    }                                           \
		    if (cos2 < end)                             \
			end = cos2;                             \
		} else {                                        \
		    if (oct1 == 6 && start < cos1)              \
			start = cos1;                           \
		    if ((oct2 & 7) == 6 && end > cos2)          \
			end = cos2;                             \
		}                                               \
	    }                                                   \
	    line = line0 + buf[start][0] * stride;              \
	    for (; start <= end; start++) {                     \
		mask = buf[start + 1][1];                       \
		BLEND(line + start)                             \
		line -= (stride & mask);                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	BLEND	MLIB_GRAPHICS_BLEND_8

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawArc_B_8(
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

	MLIB_GRAPHICS_DECL_VAR_B_8

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagc, flagd;
	mlib_s32 buf0[BUFSIZE][2], (
	    *buf)[2],
	    count,
	    scount;
	mlib_s32 start, end;
	mlib_s32 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flagclip;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		mlib_s32 s = (data + (stride * y + x))[0];
		a &= 0xff;
		c &= 0xff;
		da = a * (1./255.);
		(data + (stride * y + x))[0] = s + da * (c - s);
		return (MLIB_SUCCESS);
	}

	MLIB_GRAPHICS_SET_VAR_B_8(c, a)

	if (mlib_fabs(t1 - t2) >= PIx2)
		return (__mlib_GraphicsDrawCircle_B_8(buffer, x, y, r, c, a));
	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	line0 = data + stride * y + x;

	if (r > RADMAX) {
		buf = __mlib_malloc(sizeof (mlib_s32) * 2 * r);

		if (!buf)
			return (MLIB_FAILURE);
	} else
		buf = buf0;

	FILL_BUF;

	GET_BORDERS;

	FILL_FLAGS;

	FILL_CL_FLAGS;

	PROC_CL_OCT2;
	PROC_CL_OCT1;
	PROC_CL_OCT3;
	PROC_CL_OCT0;
	PROC_CL_OCT4;
	PROC_CL_OCT7;
	PROC_CL_OCT5;
	PROC_CL_OCT6;

	if (buf != buf0)
		__mlib_free(buf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef BLEND
#define	BLEND	MLIB_GRAPHICS_BLEND_32

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawArc_B_32(
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
	mlib_s32 buf0[BUFSIZE][2], (
	    *buf)[2],
	    count,
	    scount;
	mlib_s32 start, end;
	mlib_s32 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flagclip;

	MLIB_GRAPHICS_DECL_VAR_B_32

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	CHECK_INTERSECTION;

	if (r == 0) {
		mlib_s32 cb1, cb2, cb3;
		mlib_s32 sb1, sb2, sb3, s;
		mlib_s32 r0, r1, r2, r3;

		a &= 0xff;
		da = (mlib_d64)a * (1./255.);
		cb1 = (c >> 16) & 0xff;
		cb2 = (c >> 8) & 0xff;
		cb3 = c & 0xff;

		s = (data + (stride * y + x))[0];
		sb1 = (s >> 16) & 0xff;
		sb2 = (s >> 8) & 0xff;
		sb3 = s & 0xff;

		r0 = (mlib_s32)0xff000000;
		r1 = (mlib_s32)(sb1 + da * (cb1 - sb1));
		r2 = (mlib_s32)(sb2 + da * (cb2 - sb2));
		r3 = (mlib_s32)(sb3 + da * (cb3 - sb3));
		r0 |= (r1 << 16) | (r2 << 8) | r3;

		(data + (stride * y + x))[0] = r0;
		return (MLIB_SUCCESS);
	}

	MLIB_GRAPHICS_SET_VAR_B_32(c, a)

	if (mlib_fabs(t1 - t2) >= PIx2)
		return (__mlib_GraphicsDrawCircle_B_32(buffer, x, y, r, c, a));
	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	line0 = data + stride * y + x;

	if (r > RADMAX) {
		buf = __mlib_malloc(sizeof (mlib_s32) * 2 * r);

		if (!buf)
			return (MLIB_FAILURE);
	} else
		buf = buf0;

	FILL_BUF;

	GET_BORDERS;

	FILL_FLAGS;

	FILL_CL_FLAGS;

	PROC_CL_OCT2;
	PROC_CL_OCT1;
	PROC_CL_OCT3;
	PROC_CL_OCT0;
	PROC_CL_OCT4;
	PROC_CL_OCT7;
	PROC_CL_OCT5;
	PROC_CL_OCT6;

	if (buf != buf0)
		__mlib_free(buf);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
