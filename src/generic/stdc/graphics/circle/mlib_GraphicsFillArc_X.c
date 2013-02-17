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

#pragma ident	"@(#)mlib_GraphicsFillArc_X.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillArc_X_[8 | 32] - draw filled arc with XOR.
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFillArc_X_8   (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   r,
 *                                              mlib_f32   t1,
 *                                              mlib_f32   t2,
 *                                              mlib_s32   c,
 *                                              mlib_s32   c2)
 *
 *      mlib_status mlib_GraphicsFillArc_X_32  (mlib_image *buffer,
 *                                              mlib_s16   x,
 *                                              mlib_s16   y,
 *                                              mlib_s32   r,
 *                                              mlib_f32   t1,
 *                                              mlib_f32   t2,
 *                                              mlib_s32   c,
 *                                              mlib_s32   c2)
 *
 * ARGUMENTS
 *      buffer   Pointer to the image which the function is drawing into.
 *      x        X coordinate of the center.
 *      y        Y coordinate of the center.
 *      r        Radius of the arc.
 *      t1, t2   Start and end angles of the arc in radians.
 *      c, c2    Color used in the drawing.
 *
 * DESCRIPTION
 *      Function fills circle arc with radius 'r' in image pointed by buffer
 *      by XORing with color 'c' and center in point 'x', 'y'.
 *
 */

#include <mlib_graphics.h>
#include <mlib_SysMath.h>
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillArc_X_32 = __mlib_GraphicsFillArc_X_32
#pragma weak mlib_GraphicsFillArc_X_8 = __mlib_GraphicsFillArc_X_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillArc_X_32) mlib_GraphicsFillArc_X_32
    __attribute__((weak, alias("__mlib_GraphicsFillArc_X_32")));
__typeof__(__mlib_GraphicsFillArc_X_8) mlib_GraphicsFillArc_X_8
    __attribute__((weak, alias("__mlib_GraphicsFillArc_X_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	MLIB_GRAPHICS_COLOR_8(C)                                \
	C &= 0xFF;                                              \
	C |= (C << 8);                                          \
	C |= (C << 16)

#define	MLIB_GRAPHICS_COLOR_32(C) C |= 0xFF000000;
#define	MLIB_GRAPHICS_TO_DOUBLE(D_C, C)                         \
	((mlib_s32 *)&D_C)[0] = C;                              \
	((mlib_s32 *)&D_C)[1] = C

#define	MLIB_SIGNSHIFT	31

/* *********************************************************** */

#define	PIx2	6.28318530717958647692f
#define	PI_4	0.78539816339744830962f

/* *********************************************************** */

#define	BUFSIZE	256
#define	RADMAX	350

/* *********************************************************** */

#define	CHECK_INTERSECTION(UP, DOWN, LEFT, RIGHT)                          \
	if ((x + LEFT < width) && (x + RIGHT >= 0) && (y + UP < height) && \
	    (y + DOWN >= 0))

/* *********************************************************** */

#define	CHECK_CLIPPING(UP, DOWN, LEFT, RIGHT)                             \
	if (x + RIGHT >= width || x + LEFT < 0 || y + DOWN >= height ||   \
	    y + UP < 0)                                                   \
	    clip = 1; 							  \
	else clip = 0;

/* *********************************************************** */

#define	FILL_LINE_POINT(x_end, y_end, sign)                                \
	{                                                                  \
	    mlib_s32 eps, epsDx, epsDy, stepDx, stepDy;                    \
	    mlib_s32 i;                                                    \
	    mlib_s32 ax, ay;                                               \
	    mlib_s32 flag_line;                                            \
	                                                                   \
	    ax = (x_end >= 0) ? x_end : -x_end;                            \
	    ay = (y_end >= 0) ? y_end : -y_end;                            \
	    if (y_end == 0) {                                              \
		*point_line = (x_end > 0) ? r : -r;                        \
	    } else {                                                       \
		if (ax == ay) {                                            \
		    point_line += ay;                                      \
		    if (x_end > 0) {                                       \
			for (i = ay; i >= 0; i--) {                        \
			    *point_line = i;                               \
			    point_line--;                                  \
			}                                                  \
		    } else {                                               \
			for (i = -ay; i <= 0; i++) {                       \
			    *point_line = i;                               \
			    point_line--;                                  \
			}                                                  \
		    }                                                      \
		} else {                                                   \
		    if (!x_end) {                                          \
			point_line += ay;                                  \
			for (i = ay + 1; i > 0; i--) {                     \
			    *point_line = 0;                               \
			    point_line--;                                  \
			}                                                  \
		    } else {                                               \
			if (ay > ax) {                                     \
			    epsDx = 2 * ax;                                \
			    epsDy = -2 * ay;                               \
			    eps = 2 * ax - ay;                             \
			    stepDx = (y_end < 0) ? -stride : stride;       \
			    stepDy = (x_end < 0) ? -1 : 1;                 \
			    i = 0;                                         \
			    for (end = ay + 1; end > 0; end--) {           \
				*point_line = i;                           \
				point_line++;                              \
				flag_line = ~(eps >> MLIB_SIGNSHIFT);      \
				i += (stepDy & flag_line);                 \
				eps += (epsDx + (epsDy & flag_line));      \
			    }                                              \
			} else {                                           \
			    if (ax > ay) {                                 \
				epsDx = 2 * ay;                            \
				epsDy = -2 * ax;                           \
				eps = 2 * ay - ax;                         \
				stepDx = (x_end < 0) ? -1 : 1;             \
				stepDy = (y_end < 0) ? -stride : stride;   \
				i = 0;                                     \
				if (sign & ((x_end > 0 && y_end > 0) ||    \
				    (x_end < 0 && y_end < 0)) ||           \
				    ~sign & ((x_end > 0 && y_end < 0) ||   \
				    (x_end < 0 && y_end > 0))) {           \
				    flag_line = 1;                         \
				    for (end = ax + 1; end > 0; end--) {   \
					if (flag_line) {                   \
					    *point_line = i;               \
					    point_line++;                  \
					}                                  \
					flag_line =                        \
					    ~(eps >> MLIB_SIGNSHIFT);      \
					eps +=                             \
					    (epsDx +                       \
					    (epsDy & flag_line));          \
					i += stepDx;                       \
				    }                                      \
				} else {                                   \
				    for (end = ax; end > 0; end--) {       \
					flag_line =                        \
					    ~(eps >> MLIB_SIGNSHIFT);      \
					eps +=                             \
					    (epsDx +                       \
					    (epsDy & flag_line));          \
					if (flag_line) {                   \
					    *point_line = i;               \
					    point_line++;                  \
					}                                  \
					i += stepDx;                       \
				    }                                      \
				    *point_line = i;                       \
				}                                          \
			    }                                              \
			}                                                  \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	FILL_BUF                                                \
	cy = r;                                                 \
	cx = 1;                                                 \
	del = 1 - r;                                            \
	mask = 0;                                               \
	buf[0] = r;                                             \
	while (cx < cy) {                                       \
	    buf[cx] = cy;                                       \
	    del += (2 * cx + 1) - ((2 * cy) & mask);            \
	    mask = ~(del >> 31);                                \
	    buf[cy] = cx & mask;                                \
	    cy += mask;                                         \
	    ++cx;                                               \
	}                                                       \
	                                                        \
	if (cx == cy && mask) {                                 \
	    buf[cx] = cy;                                       \
	    count_arc = cx;                                     \
	} else {                                                \
	    buf[cx] = cx - 1;                                   \
	    count_arc = cx - 1;                                 \
	}

/* *********************************************************** */

#define	GET_X_Y_COORDINATE                                      \
	{                                                       \
	    mlib_f32 fs1, fc1, fs2, fc2, fr;                    \
	    mlib_s32 sum;                                       \
	                                                        \
	    fr = (mlib_f32)r;                                   \
	    mlib_sincosf(t1, &fs1, &fc1);                       \
	    mlib_sincosf(t2, &fs2, &fc2);                       \
	    oct1 = t1 / (PI_4);                                 \
	    oct2 = t2 / (PI_4);                                 \
	    sin1 = mlib_fabs(fr * fs1) + 0.5f;                  \
	    cos1 = mlib_fabs(fr * fc1) + 0.5f;                  \
	    if (cos1 == sin1 && (cos1 != count_arc ||           \
		sin1 != buf[count_arc]))                        \
		oct1 = (oct1 & ~1) + 1;                         \
	    if (cos1 > sin1)                                    \
		sum = buf[sin1] + sin1;                         \
	    else                                                \
		sum = buf[cos1] + cos1;                         \
	    if (cos1 + sin1 > sum) {                            \
		if (oct1 & 2)                                   \
		    sin1 -= 1;                                  \
		else                                            \
		    cos1 -= 1;                                  \
	    } else {                                            \
		if (cos1 + sin1 < sum) {                        \
		    if (oct1 & 2) {                             \
			cos1 += 1;                              \
		    } else {                                    \
			sin1 += 1;                              \
		    }                                           \
		}                                               \
	    }                                                   \
	    sn1 = (fs1 > 0) ? sin1 : -sin1;                     \
	    cs1 = (fc1 > 0) ? cos1 : -cos1;                     \
	    sin2 = mlib_fabs(fr * fs2) + 0.5f;                  \
	    cos2 = mlib_fabs(fr * fc2) + 0.5f;                  \
	    if (cos2 == sin2 && (cos2 != count_arc ||           \
		sin2 != buf[count_arc]))                        \
		oct2 = (oct2 & ~1);                             \
	    if (cos2 > sin2)                                    \
		sum = buf[sin2] + sin2;                         \
	    else                                                \
		sum = buf[cos2] + cos2;                         \
	    if (cos2 + sin2 > sum) {                            \
		if (oct2 & 2)                                   \
		    cos2 -= 1;                                  \
		else                                            \
		    sin2 -= 1;                                  \
	    } else {                                            \
		if (cos2 + sin2 < sum) {                        \
		    if (oct2 & 2)                               \
			sin2 += 1;                              \
		    else                                        \
			cos2 += 1;                              \
		}                                               \
	    }                                                   \
	    sn2 = (fs2 > 0) ? sin2 : -sin2;                     \
	    cs2 = (fc2 > 0) ? cos2 : -cos2;                     \
	}

/* *********************************************************** */

#define	FILL_FLAGD                                              \
	flagc = ((-2) << oct1) & ~(((-1) << oct2));             \
	flagc |= (flagc >> 8);                                  \
	flagd = ((-1) << oct1) & ~((-2) << oct2);               \
	flagd |= (flagd >> 8)

/* *********************************************************** */

#define	LEFT(src, dst)	dst = (src) & ~((src) >> 31)

/* *********************************************************** */

#define	RIGHT(src, dst)                                         \
	{                                                       \
	    mlib_s32 dl = (src) - (width - 1);                  \
	                                                        \
	    dst = (dl & (dl >> 31)) + (width - 1);              \
	}

/* *********************************************************** */

#define	GET_BORDERS(FLAGC, OCT_1, OCT_2, OCT_FLAG_1,            \
	OCT_FLAG_2, AL, AR, START, END)                         \
	{                                                       \
	    if ((flagc & (FLAGC)) == (FLAGC)) {                 \
		start = (START);                                \
		start_1 = (END) - 1;                            \
		start_2 = start_1 + 1;                          \
		side = 0;                                       \
		xl = -buf[END];                                 \
		xr = buf[END];                                  \
	    } else if (OCT_1 == OCT_FLAG_1) {                   \
		if (OCT_2 == OCT_FLAG_2) {                      \
		    start = (START);                            \
		    if (sin1 > sin2) {                          \
			start_1 = sin1;                         \
			start_2 = sin2;                         \
			side = 1;                               \
		    } else if (sin1 < sin2) {                   \
			start_1 = sin2;                         \
			start_2 = sin1;                         \
			side = 2;                               \
		    } else {                                    \
			start_1 = sin1;                         \
			start_2 = sin2;                         \
			side = 0;                               \
		    }                                           \
		    xl = line_fill_##AL[sin##AL];               \
		    xr = line_fill_##AR[sin##AR];               \
		    sin1 = sin2 = END - 1;                      \
		} else if (OCT_2 == OCT_FLAG_1) {               \
		    start = sin##AL;                            \
		    start_1 = start;                            \
		    start_2 = sin##AR;                          \
		    side = AL;                                  \
		    xl = line_fill_##AL[END];                   \
		    xr = line_fill_##AR[sin##AR];               \
		    sin1 = sin2 = END - 1;                      \
		} else {                                        \
		    start = (START);                            \
		    start_1 = sin##AR;                          \
		    start_2 = (END) - 1;                        \
		    side = AR;                                  \
		    xl = -buf[END];                             \
		    xr = line_fill_##AR[sin##AR];               \
		    sin##AR = END - 1;                          \
		}                                               \
	    } else if (OCT_1 == OCT_FLAG_2) {                   \
		if (OCT_2 == OCT_FLAG_2) {                      \
		    start = sin##AR;                            \
		    start_1 = start;                            \
		    start_2 = sin##AL;                          \
		    side = AR;                                  \
		    xl = line_fill_##AL[sin##AL];               \
		    xr = line_fill_##AR[END];                   \
		    sin1 = sin2 = END - 1;                      \
		} else {                                        \
		    start = sin##AR;                            \
		    start_1 = start;                            \
		    start_2 = (END) - 1;                        \
		    side = AR;                                  \
		    xl = -buf[END];                             \
		    xr = line_fill_##AR[END];                   \
		    sin##AR = END - 1;                          \
		}                                               \
	    } else if (OCT_2 == OCT_FLAG_1) {                   \
		start = sin##AL;                                \
		start_1 = start;                                \
		start_2 = (END) - 1;                            \
		side = AL;                                      \
		xl = line_fill_##AL[END];                       \
		xr = buf[END];                                  \
		sin##AL = END - 1;                              \
	    } else if (OCT_2 == OCT_FLAG_2) {                   \
		start = (START);                                \
		start_1 = sin##AL;                              \
		start_2 = (END) - 1;                            \
		side = AL;                                      \
		xl = line_fill_##AL[sin##AL];                   \
		xr = buf[END];                                  \
		sin##AL = END - 1;                              \
	    }                                                   \
	}

/* *********************************************************** */

#define	DRAW_LINE(VAR, ARRAYleft, ARRAYright, SIGN, FORM)       \
	{                                                       \
	    MLIB_FILL_ROW_X_##FORM(line, ARRAYleft[VAR],        \
		ARRAYright[VAR], cxor, dc);                     \
	    line = line SIGN stride;                            \
	}

/* *********************************************************** */

#define	DRAW_LINE_CLIP(VAR, ARRAYleft, ARRAYright, SIGN, FORM)          \
	{                                                               \
	    if (((y - SIGN VAR) >= 0) && ((y - SIGN VAR < height))) {   \
		LEFT(x ARRAYleft[VAR], xb);                             \
		RIGHT(x ARRAYright[VAR], xe);                           \
		MLIB_FILL_ROW_X_##FORM(line - x, xb, xe, cxor, dc);     \
	    }                                                           \
	    line = line SIGN stride;                                    \
	}

/* *********************************************************** */

#define	DRAW_PART_ARC(END, SIGN_STEP, B_1, B_2, B_3, B_4,            \
	B_5, B_6, FORM)                                              \
	{                                                            \
	    line = line0 - (SIGN_STEP stride * start);               \
	    for (; start > start_1; start--) {                       \
		DRAW_LINE(start, -buf, +buf, SIGN_STEP, FORM);       \
	    }                                                        \
	    if (side == 1) {                                         \
		for (; start_1 > start_2; start_1--) {               \
		    DRAW_LINE(start_1, B_1, B_2, SIGN_STEP, FORM);   \
		}                                                    \
	    } else if (side == 2) {                                  \
		for (; start_1 > start_2; start_1--) {               \
		    DRAW_LINE(start_1, B_3, B_4, SIGN_STEP, FORM);   \
		}                                                    \
	    }                                                        \
	    for (; start_1 >= (END); start_1--) {                    \
		DRAW_LINE(start_1, B_5, B_6, SIGN_STEP, FORM);       \
	    }                                                        \
	}

/* *********************************************************** */

#define	DRAW_PART_ARC_CLIP(END, SIGN_STEP, B_1, B_2, B_3,                 \
	B_4, B_5, B_6, FORM)                                              \
	{                                                                 \
	    line = line0 - (SIGN_STEP stride * start);                    \
	    for (; start > start_1; start--) {                            \
		DRAW_LINE_CLIP(start, -buf, +buf, SIGN_STEP, FORM);       \
	    }                                                             \
	    if (side == 1) {                                              \
		for (; start_1 > start_2; start_1--) {                    \
		    DRAW_LINE_CLIP(start_1, B_1, B_2, SIGN_STEP, FORM);   \
		}                                                         \
	    } else if (side == 2) {                                       \
		for (; start_1 > start_2; start_1--) {                    \
		    DRAW_LINE_CLIP(start_1, B_3, B_4, SIGN_STEP, FORM);   \
		}                                                         \
	    }                                                             \
	    for (; start_1 >= (END); start_1--) {                         \
		DRAW_LINE_CLIP(start_1, B_5, B_6, SIGN_STEP, FORM);       \
	    }                                                             \
	}

/* *********************************************************** */

#define	FINISH_STEP                                             \
	if (count_repeat == 0) {                                \
	    repeat = 1;                                         \
	    t2 = help_t2;                                       \
	    t1 = 0.0;                                           \
	}                                                       \
	continue

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillArc_X_8(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_f32 t1,
    mlib_f32 t2,
    mlib_s32 c0,
    mlib_s32 c2)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0 = NULL, *line = NULL;
	mlib_s32 cx, cy, del, mask;

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagd, flagc;
	mlib_s32 sn1, cs1, sn2, cs2, xl, xr;
	mlib_s32 buf0[BUFSIZE], *buf = NULL, count_arc;
	mlib_s32 line_fill_1_0[RADMAX], line_fill_2_0[RADMAX];
	mlib_s32 *line_fill_1 = NULL, *line_fill_2 = NULL, *point_line = NULL;
	mlib_s32 start, start_1, start_2, end, help;
	mlib_d64 dc;

	mlib_s32 xb, xe, clip, side, count_repeat, repeat;
	mlib_f32 help_t2, start_sin1;
	mlib_s32 x = xx, y = yy, cxor = c0 ^ c2, xb_last, xe_last;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	if (r == 0) {
		if (y < height && y >= 0 && x < width && x >= 0)
			*(data + (stride * y + x)) ^= cxor;
		return (MLIB_SUCCESS);
	}

	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)
		return (MLIB_SUCCESS);

	if (mlib_fabs(t1 - t2) >= PIx2)
		return (__mlib_GraphicsFillCircle_X_8
		    (buffer, xx, yy, r, c0, c2));

	xb_last = width;
	xe_last = -1;
	count_repeat = 1;
	repeat = 0;

	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	{
		mlib_s32 n2p = t1 / PIx2;
		mlib_f32 sh = PIx2 * (mlib_f32)n2p;

		if (t1 < 0.0f)
			sh -= PIx2;
		t1 -= sh;
		t2 -= sh;
	}

	line0 = data + stride * y + x;

	MLIB_GRAPHICS_COLOR_8(cxor);
	MLIB_GRAPHICS_TO_DOUBLE(dc, cxor);

	if (r >= RADMAX) {
		buf = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!buf)
			return (MLIB_FAILURE);
		line_fill_1 = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!line_fill_1) {
			__mlib_free(buf);
			return (MLIB_FAILURE);
		}

		line_fill_2 = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!line_fill_2) {
			__mlib_free(buf);
			__mlib_free(line_fill_1);
			return (MLIB_FAILURE);
		}
	} else {
		buf = buf0;
		line_fill_1 = line_fill_1_0;
		line_fill_2 = line_fill_2_0;
	}

	FILL_BUF;

	if (t2 > PIx2) {
		help_t2 = t2 - PIx2;
		t2 = PIx2;
		count_repeat = 0;
	}

	for (; count_repeat < 2; count_repeat++) {
		GET_X_Y_COORDINATE;

		if (oct2 < oct1) {
			mask =
			    __mlib_GraphicsDrawLine_X_8(buffer, xx, yy,
			    xx + cs1, yy + sn1, c0, c2);

			if (mask == MLIB_SUCCESS) {
				FINISH_STEP;
			} else
				return (mask);
		}

		if (oct2 == 8)
			oct2 = 7;

		if ((sn1 == 0) && (sn2 == 0) && (cs1 >= 0) && (cs2 >= 0) &&
		    (oct2 - oct1 > 4))
			return __mlib_GraphicsFillCircle_X_8(buffer, xx, yy, r,
			    c0, c2);

		if (count_repeat == 0)
			start_sin1 = sn1;

		point_line = &line_fill_1[0];
		FILL_LINE_POINT(cs1, sn1, 0);

		point_line = &line_fill_2[0];
		FILL_LINE_POINT(cs2, sn2, -1);

		FILL_FLAGD;

		if ((y | (height - 1 - y)) >= 0) {
			if (sn1 | sn2) {
				if (sn1 >= 0) {
					if (sn2 > 0) {
						LEFT(x + line_fill_2[0], xb);
						RIGHT(x + line_fill_1[0], xe);
					} else {
						LEFT(x - buf[0], xb);

						if (line_fill_1[0] >=
						    line_fill_2[0])
							RIGHT(x +
							    line_fill_1[0], xe)
							    else
							RIGHT(x +
							    line_fill_2[0], xe);
					}
				} else {
					LEFT(x + line_fill_1[0], xb);
					RIGHT(x + line_fill_2[0], xe);
				}
			} else if ((cs1 | cs2) > 0) {
				LEFT(x, xb);
				RIGHT(x + r, xe);

				if (xb_last > xb)
					xb_last = xb;

				if (xe_last < xe)
					xe_last = xe;
				FINISH_STEP;
			} else if ((cs2 & cs1) < 0) {
				LEFT(x - r, xb);
				RIGHT(x, xe);

				if (xb_last > xb)
					xb_last = xb;

				if (xe_last < xe)
					xe_last = xe;
				FINISH_STEP;
			} else {
				LEFT(x - r, xb);
				RIGHT(x + r, xe);
			}

			if (xb < xb_last)
				xb_last = xb;

			if (xe > xe_last)
				xe_last = xe;
		}

		oct2 = oct2 & 7;
		help = buf[count_arc] + 1;

		if (flagd & 0x006) {
			GET_BORDERS(0x006, oct1, oct2, 1, 2, 2, 1, r, help);
			CHECK_INTERSECTION(-start, -help, xl, xr) {
				CHECK_CLIPPING(-start, -help, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(help, +, -buf,
					    +line_fill_1, +line_fill_2, +buf,
					    +line_fill_2, +line_fill_1, 8);
				} else {
					DRAW_PART_ARC(help, +, -buf,
					    line_fill_1, line_fill_2, buf,
					    line_fill_2, line_fill_1, 8);
				}
			}
		}

		if ((flagd & 0x009) || (flagd & 0x006)) {
			help = buf[count_arc];
			GET_BORDERS(0x009, (oct1 >> 1), (oct2 >> 1), 0, 1, 2, 1,
			    help, 1);
			CHECK_INTERSECTION(-start, 1, xl, xr) {
				CHECK_CLIPPING(-start, 1, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(1, +, -buf,
					    +line_fill_1, +line_fill_2, +buf,
					    +line_fill_2, +line_fill_1, 8);
				} else {
					DRAW_PART_ARC(1, +, -buf, line_fill_1,
					    line_fill_2, buf, line_fill_2,
					    line_fill_1, 8);
				}
			}
		}

		help = buf[count_arc] + 1;

		if (flagd & 0x060) {
			GET_BORDERS(0x060, oct2, oct1, 6, 5, 1, 2, r, help);
			CHECK_INTERSECTION(help, start, xl, xr) {
				CHECK_CLIPPING(help, start, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(help, -,
					    +line_fill_1, +buf, -buf,
					    +line_fill_2, +line_fill_1,
					    +line_fill_2, 8);
				} else {
					DRAW_PART_ARC(help, -, line_fill_1, buf,
					    -buf, line_fill_2, line_fill_1,
					    line_fill_2, 8);
				}
			}
		}

		if ((flagd & 0x090) || (flagd & 0x060)) {
			help = buf[count_arc];
			GET_BORDERS(0x090, (oct2 >> 1), (oct1 >> 1), 3, 2, 1, 2,
			    help, 1);
			CHECK_INTERSECTION(1, start, xl, xr) {
				CHECK_CLIPPING(1, start, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(1, -, +line_fill_1,
					    +buf, -buf, +line_fill_2,
					    +line_fill_1, +line_fill_2, 8);
				} else {
					DRAW_PART_ARC(1, -, line_fill_1, buf,
					    -buf, line_fill_2, line_fill_1,
					    line_fill_2, 8);
				}
			}
		}

		if (count_repeat == 0) {
			repeat = 1;
			t2 = help_t2;
			t1 = 0.0;
		}
	}

	if (repeat == 1) {
		if (start_sin1 > 0 && sn2 > 0) {
			start = (start_sin1 > sn2) ? sn2 : start_sin1;
			end = 1;
			line = line0 - stride * start;
			for (; (start >= end); start--) {
				DRAW_LINE_CLIP(start, +line_fill_2,
				    +line_fill_1, +, 8);
			}
		} else if (start_sin1 < 0 && sn2 < 0) {
			start = (start_sin1 > sn2) ? -start_sin1 : -sn2;
			end = 1;
			line = line0 + stride * start;
			for (; (start >= end); start--) {
				DRAW_LINE_CLIP(start, +line_fill_1,
				    +line_fill_2, -, 8);
			}
		}
	}

	if ((y | (height - 1 - y)) >= 0) {
		MLIB_FILL_ROW_X_8(line0 - x, xb_last, xe_last, cxor, dc);
	}

	if (r >= RADMAX) {
		__mlib_free(buf);
		__mlib_free(line_fill_1);
		__mlib_free(line_fill_2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillArc_X_32(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_f32 t1,
    mlib_f32 t2,
    mlib_s32 c0,
    mlib_s32 c2)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (mlib_s32);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0 = NULL, *line = NULL;
	mlib_s32 cx, cy, del, mask;

	mlib_s32 sin1, cos1, sin2, cos2, oct1, oct2, flagd, flagc;
	mlib_s32 sn1, cs1, sn2, cs2, xl, xr;
	mlib_s32 buf0[BUFSIZE], *buf = NULL, count_arc;
	mlib_s32 line_fill_1_0[RADMAX], line_fill_2_0[RADMAX];
	mlib_s32 *line_fill_1 = NULL, *line_fill_2 = NULL, *point_line = NULL;
	mlib_s32 start, start_1, start_2, end, help;
	mlib_d64 dc;

	mlib_s32 xb, xe, clip, side, count_repeat, repeat;
	mlib_f32 help_t2, start_sin1;
	mlib_s32 x = xx, y = yy, cxor, xb_last, xe_last;

	MLIB_GRAPHICS_COLOR_32(c0);
	MLIB_GRAPHICS_COLOR_32(c2);
	cxor = c0 ^ c2;

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);

	if (r == 0) {
		if (y < height && y >= 0 && x < width && x >= 0)
			*(data + (stride * y + x)) ^= cxor;
		return (MLIB_SUCCESS);
	}

	MLIB_GRAPHICS_TO_DOUBLE(dc, cxor);

	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)
		return (MLIB_SUCCESS);

	if (mlib_fabs(t1 - t2) >= PIx2)
		return __mlib_GraphicsFillCircle_X_32(buffer, xx, yy, r, c0,
		    c2);

	xb_last = width;
	xe_last = -1;
	count_repeat = 1;
	repeat = 0;

	{
		mlib_f32 tt = t1;

		t1 = -t2;
		t2 = -tt;
	}

	if (t1 > t2)
		t2 += PIx2;

	{
		mlib_s32 n2p = t1 / PIx2;
		mlib_f32 sh = PIx2 * (mlib_f32)n2p;

		if (t1 < 0.0f)
			sh -= PIx2;
		t1 -= sh;
		t2 -= sh;
	}

	line0 = data + stride * y + x;

	if (r >= RADMAX) {
		buf = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!buf)
			return (MLIB_FAILURE);
		line_fill_1 = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!line_fill_1) {
			__mlib_free(buf);
			return (MLIB_FAILURE);
		}

		line_fill_2 = __mlib_malloc(sizeof (mlib_s32) * (r + 1));

		if (!line_fill_2) {
			__mlib_free(buf);
			__mlib_free(line_fill_1);
			return (MLIB_FAILURE);
		}
	} else {
		buf = buf0;
		line_fill_1 = line_fill_1_0;
		line_fill_2 = line_fill_2_0;
	}

	FILL_BUF;

	if (t2 > PIx2) {
		help_t2 = t2 - PIx2;
		t2 = PIx2;
		count_repeat = 0;
	}

	for (; count_repeat < 2; count_repeat++) {
		GET_X_Y_COORDINATE;

		if (oct2 < oct1) {
			mask =
			    __mlib_GraphicsDrawLine_X_32(buffer, xx, yy,
			    xx + cs1, yy + sn1, c0, c2);

			if (mask == MLIB_SUCCESS) {
				FINISH_STEP;
			} else
				return (mask);
		}

		if (oct2 == 8)
			oct2 = 7;

		if ((sn1 == 0) && (sn2 == 0) && (cs1 >= 0) && (cs2 >= 0) &&
		    (oct2 - oct1 > 4))
			return __mlib_GraphicsFillCircle_X_32(buffer, xx, yy, r,
			    c0, c2);

		if (count_repeat == 0)
			start_sin1 = sn1;

		point_line = &line_fill_1[0];
		FILL_LINE_POINT(cs1, sn1, 0);

		point_line = &line_fill_2[0];
		FILL_LINE_POINT(cs2, sn2, -1);

		FILL_FLAGD;

		if ((y | (height - 1 - y)) >= 0) {
			if (sn1 | sn2) {
				if (sn1 >= 0) {
					if (sn2 > 0) {
						LEFT(x + line_fill_2[0], xb);
						RIGHT(x + line_fill_1[0], xe);
					} else {
						LEFT(x - buf[0], xb);

						if (line_fill_1[0] >=
						    line_fill_2[0])
							RIGHT(x +
							    line_fill_1[0], xe)
							    else
							RIGHT(x +
							    line_fill_2[0], xe);
					}
				} else {
					LEFT(x + line_fill_1[0], xb);
					RIGHT(x + line_fill_2[0], xe);
				}
			} else if ((cs1 | cs2) > 0) {
				LEFT(x, xb);
				RIGHT(x + r, xe);

				if (xb_last > xb)
					xb_last = xb;

				if (xe_last < xe)
					xe_last = xe;
				FINISH_STEP;
			} else if ((cs2 & cs1) < 0) {
				LEFT(x - r, xb);
				RIGHT(x, xe);

				if (xb_last > xb)
					xb_last = xb;

				if (xe_last < xe)
					xe_last = xe;
				FINISH_STEP;
			} else {
				LEFT(x - r, xb);
				RIGHT(x + r, xe);
			}

			if (xb < xb_last)
				xb_last = xb;

			if (xe > xe_last)
				xe_last = xe;
		}

		oct2 = oct2 & 7;
		help = buf[count_arc] + 1;

		if (flagd & 0x006) {
			GET_BORDERS(0x006, oct1, oct2, 1, 2, 2, 1, r, help);
			CHECK_INTERSECTION(-start, -help, xl, xr) {
				CHECK_CLIPPING(-start, -help, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(help, +, -buf,
					    +line_fill_1, +line_fill_2, +buf,
					    +line_fill_2, +line_fill_1, 32);
				} else {
					DRAW_PART_ARC(help, +, -buf,
					    line_fill_1, line_fill_2, buf,
					    line_fill_2, line_fill_1, 32);
				}
			}
		}

		if ((flagd & 0x009) || (flagd & 0x006)) {
			help = buf[count_arc];
			GET_BORDERS(0x009, (oct1 >> 1), (oct2 >> 1), 0, 1, 2, 1,
			    help, 1);
			CHECK_INTERSECTION(-start, 1, xl, xr) {
				CHECK_CLIPPING(-start, 1, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(1, +, -buf,
					    +line_fill_1, +line_fill_2, +buf,
					    +line_fill_2, +line_fill_1, 32);
				} else {
					DRAW_PART_ARC(1, +, -buf, line_fill_1,
					    line_fill_2, buf, line_fill_2,
					    line_fill_1, 32);
				}
			}
		}

		help = buf[count_arc] + 1;

		if (flagd & 0x060) {
			GET_BORDERS(0x060, oct2, oct1, 6, 5, 1, 2, r, help);
			CHECK_INTERSECTION(help, start, xl, xr) {
				CHECK_CLIPPING(help, start, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(help, -,
					    +line_fill_1, +buf, -buf,
					    +line_fill_2, +line_fill_1,
					    +line_fill_2, 32);
				} else {
					DRAW_PART_ARC(help, -, line_fill_1, buf,
					    -buf, line_fill_2, line_fill_1,
					    line_fill_2, 32);
				}
			}
		}

		if ((flagd & 0x090) || (flagd & 0x060)) {
			help = buf[count_arc];
			GET_BORDERS(0x090, (oct2 >> 1), (oct1 >> 1), 3, 2, 1, 2,
			    help, 1);
			CHECK_INTERSECTION(1, start, xl, xr) {
				CHECK_CLIPPING(1, start, xl, xr);

				if (clip) {
					DRAW_PART_ARC_CLIP(1, -, +line_fill_1,
					    +buf, -buf, +line_fill_2,
					    +line_fill_1, +line_fill_2, 32);
				} else {
					DRAW_PART_ARC(1, -, line_fill_1, buf,
					    -buf, line_fill_2, line_fill_1,
					    line_fill_2, 32);
				}
			}
		}

		if (count_repeat == 0) {
			repeat = 1;
			t2 = help_t2;
			t1 = 0.0;
		}
	}

	if (repeat == 1) {
		if (start_sin1 > 0 && sn2 > 0) {
			start = (start_sin1 > sn2) ? sn2 : start_sin1;
			end = 1;
			line = line0 - stride * start;
			for (; (start >= end); start--) {
				DRAW_LINE_CLIP(start, +line_fill_2,
				    +line_fill_1, +, 32);
			}
		} else if (start_sin1 < 0 && sn2 < 0) {
			start = (start_sin1 > sn2) ? -start_sin1 : -sn2;
			end = 1;
			line = line0 + stride * start;
			for (; (start >= end); start--) {
				DRAW_LINE_CLIP(start, +line_fill_1,
				    +line_fill_2, -, 32);
			}
		}
	}

	if ((y | (height - 1 - y)) >= 0) {
		MLIB_FILL_ROW_X_32(line0 - x, xb_last, xe_last, cxor, dc);
	}

	if (r >= RADMAX) {
		__mlib_free(buf);
		__mlib_free(line_fill_1);
		__mlib_free(line_fill_2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
