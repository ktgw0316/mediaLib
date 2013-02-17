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

#pragma ident	"@(#)mlib_ImageColorTrue2IndexInit.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorTrue2IndexInit - initialize the colormap structure
 *      mlib_ImageColorTrue2IndexFree - free the colormap structure
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorTrue2IndexInit(void       **colormap,
 *                                                mlib_s32   bits,
 *                                                mlib_type  intype,
 *                                                mlib_type  outtype,
 *                                                mlib_s32   channels,
 *                                                mlib_s32   entries,
 *                                                mlib_s32   offset,
 *                                                const void **table)
 *
 *      void mlib_ImageColorTrue2IndexFree(void *colormap)
 *
 * ARGUMENTS
 *      colormap  Internal data structure for inverse color mapping.
 *      bits      Number of bits per color component used in the colorcube
 *                of the colormap structure. (If bits == 0, no colorcube will
 *                be created. But the inverse color mapping might be done by
 *                using the original lookup table.)
 *      intype    Data type of the source color image and the lookup table
 *      outtype   Data type of the destination indexed image
 *      channels  Number of channels of the lookup table.
 *      entries   Number of entries of the lookup table.
 *      offset    The first entry offset of the lookup table.
 *      table     The lookup table (lut).
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *
 * DESCRIPTION
 *      Convert a true color image to a pseudo color image with the method
 *      of finding the nearest matched lut entry for each pixel.
 *
 *      The src can be an MLIB_BYTE or MLIB_SHORT image with 3 or 4 channels.
 *      The dst must be a 1-channel MLIB_BYTE or MLIB_SHORT image.
 *
 *      The lut might have either 3 or 4 channels. The type of the lut can be
 *      one of the following:
 *              MLIB_BYTE in, MLIB_BYTE out (i.e., BYTE-to-BYTE)
 *              MLIB_BYTE in, MLIB_SHORT out (i.e., BYTE-to-SHORT)
 *              MLIB_SHORT in, MLIB_SHORT out (i.e., SHORT-to-SHORT)
 *              MLIB_SHORT in, MLIB_BYTE out (i.e., SHORT-to-BYTE)
 *
 *      The src image and the lut must have same number of channels.
 */

/*
 * FUNCTION
 *      mlib_ImageColorDitherInit - initialize the colormap structure for
 *                                  dithering
 *      mlib_ImageColorDitherFree - free the colormap structure for dithering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorDitherInit(void      **colormap,
 *                                            mlib_s32  *dimensions,
 *                                            mlib_type intype,
 *                                            mlib_type outtype,
 *                                            mlib_s32  channels,
 *                                            mlib_s32  entries,
 *                                            mlib_s32  offset,
 *                                            void      **lut)
 *
 *      void mlib_ImageColorDitherFree(void *colormap)
 *
 * ARGUMENTS
 *      colormap   Internal data structure for image dithering.
 *      dimensions Dimensions of the colorcube in the colormap structure.
 *      intype     Data type of the source image and the lookup table.
 *      outtype    Data type of the destination indexed image.
 *      channels   Number of channels of the lookup table and source image.
 *      entries    Number of entries of the lookup table.
 *      offset     The index offset of the lookup table.
 *      lut        The lookup table.
 *
 * DESCRIPTION
 *      Create/delete an internal data structure for image dithering.
 *
 *      If dimensions == NULL, then no colorcube will be created.
 *      In this case, the user-provided lookup table, lut, will be used for
 *      dithering.
 *
 *      If dimensions != NULL, then a colorcube is created from scratch
 *      in a way shown in the following example.
 *
 *      To dither an RGB image of type MLIB_BYTE to a color-indexed image of
 *      type MLIB_BYTE, we can use the following parameters:
 *
 *          mlib_s32  dimensions[] = {2, 3, 4};
 *          mlib_type intype = MLIB_BYTE;
 *          mlib_type outtype = MLIB_BYTE;
 *          mlib_s32  channels = 3;
 *          mlib_s32  offset = 6;
 *
 *      These values would lead to the creation of a colorcube that would
 *      dither red values in the source image to one of 2 red levels, green
 *      values to one of 3 green levels, and blue values to one of 4 blue
 *      levels.  You could picture this colorcube as a cube with dimensions
 *      of 2, 3, and 4.  The index values assigned to the elements in that
 *      cube can be described by the following lookup table:
 *
 *          Indexes    Red values    Green values    Blue values
 *          ----------------------------------------------------
 *             0
 *            ...
 *             5
 *          ----------------------------------------------------
 *             6            0              0               0
 *             7          255              0               0
 *             8            0            128               0
 *             9          255            128               0
 *            10            0            255               0
 *            11          255            255               0
 *            12            0              0              85
 *            13          255              0              85
 *            14            0            128              85
 *            15          255            128              85
 *            16            0            255              85
 *            17          255            255              85
 *            18            0              0             170
 *            19          255              0             170
 *            20            0            128             170
 *            21          255            128             170
 *            22            0            255             170
 *            23          255            255             170
 *            24            0              0             255
 *            25          255              0             255
 *            26            0            128             255
 *            27          255            128             255
 *            28            0            255             255
 *            29          255            255             255
 *          ----------------------------------------------------
 *            ...
 *          ----------------------------------------------------
 *
 *      The distance between level changes in each channel of the lookup
 *      table is determined by the following formulas:
 *
 *          multipliers[0] = signof(dimensions[0])*1;
 *          multipliers[i] = signof(dimensions[i])*
 *                           abs(multipliers[i-1]*dimension[i-1]);
 *
 *      A negative dimensions[i], so as to a negative multipliers[i],
 *      indicates that the values in a color ramp for channel i should
 *      appear in decreasing as opposed to increasing order.
 *
 *      For each channel i, the values of the levels are determined by the
 *      following formulas:
 *
 *          double delta = (dataMax - dataMin)/(abs(dimensions[i]) - 1);
 *          int levels[j] = (int)(j*delta + 0.5);
 *
 *      where dataMax and dataMin are the maximum and minimum values,
 *      respectively, for data type intype.
 *
 *      Whenever a colorcube is created, if lut != NULL, the lookup table
 *      will be filled according to the colorcube and supplied parameters
 *      like offset.  For the example shown above, the lookup table will
 *      start from line 6.  In this case, it's the user's responsibility
 *      to allocate memory for the lookup table.
 *
 * RESTRICTIONS
 *      The lut might have either 1, 3 or 4 channels. The type of the lut
 *      can be one of the following:
 *              MLIB_BYTE in,  MLIB_BYTE out  (i.e., BYTE-to-BYTE)
 *              MLIB_BYTE in,  MLIB_SHORT out (i.e., BYTE-to-SHORT)
 *              MLIB_SHORT in, MLIB_BYTE out  (i.e., SHORT-to-BYTE)
 *              MLIB_SHORT in, MLIB_SHORT out (i.e., SHORT-to-SHORT)
 *              MLIB_BIT in,   MLIB_BYTE out  (i.e., BIT-to-BYTE)
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorDitherFree = __mlib_ImageColorDitherFree
#pragma weak mlib_ImageColorDitherInit = __mlib_ImageColorDitherInit
#pragma weak mlib_ImageColorTrue2IndexFree = __mlib_ImageColorTrue2IndexFree
#pragma weak mlib_ImageColorTrue2IndexInit = __mlib_ImageColorTrue2IndexInit

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorDitherFree) mlib_ImageColorDitherFree
    __attribute__((weak, alias("__mlib_ImageColorDitherFree")));
__typeof__(__mlib_ImageColorDitherInit) mlib_ImageColorDitherInit
    __attribute__((weak, alias("__mlib_ImageColorDitherInit")));
__typeof__(__mlib_ImageColorTrue2IndexFree) mlib_ImageColorTrue2IndexFree
    __attribute__((weak, alias("__mlib_ImageColorTrue2IndexFree")));
__typeof__(__mlib_ImageColorTrue2IndexInit) mlib_ImageColorTrue2IndexInit
    __attribute__((weak, alias("__mlib_ImageColorTrue2IndexInit")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LUT_BYTE_COLORS_3CHANNELS	700
#define	LUT_BYTE_COLORS_4CHANNELS	1500

#define	LUT_SHORT_COLORS_3CHANNELS	1000

#define	LUT_SHORT_COLORS_4CHANNELS	1000

/* *********************************************************** */

static void *mlib_color_cube_init_U8_3(
    mlib_colormap * state);
static void *mlib_color_cube_init_U8_4(
    mlib_colormap * state);

static void *mlib_color_cube_init_S16_3(
    mlib_colormap * state);
static void *mlib_color_cube_init_S16_4(
    mlib_colormap * state);

static void *mlib_binary_tree_init_3(
    mlib_colormap * state);
static void *mlib_binary_tree_init_4(
    mlib_colormap * state);

static void __mlib_free_binary_tree_3(
    void *lut);
static void __mlib_free_binary_tree_4(
    void *lut);

/* *********************************************************** */

#define	COLOR_CUBE_3_INIT_THIRD_LOOP(POINTER_TYPE)                         \
	{                                                                  \
	    mlib_s32 detect;                                               \
	    mlib_u32 *dp;                                                  \
	    POINTER_TYPE *rgbp;                                            \
	    mlib_u32 dist2, xx2;                                           \
	    mlib_s32 count, i = cindex;                                    \
	    mlib_s32 lim;                                                  \
	    mlib_s32 thismin, thismax;                                     \
	                                                                   \
	    if (ret) {                                                     \
		here2 = center2;                                           \
		min2 = 0;                                                  \
		max2 = colormax - 1;                                       \
		inc2 = cinc2;                                              \
		prevmin2 = colormax;                                       \
		prevmax2 = 0;                                              \
		dmin2 = 0;                                                 \
		dmax2 = 0;                                                 \
	    }                                                              \
	    detect = 0;                                                    \
	    thismin = min2;                                                \
	    thismax = max2;                                                \
/*                                                                         \
 * Third loop up                                                           \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * First loop just finds first applicable cell.                            \
 */                                                                        \
	    for (count = here2, dist2 = dist1, xx2 = inc2, dp =            \
		dp1, rgbp = rgbp1, lim = max2; count <= lim;               \
		count++, dp++, rgbp++, dist2 += xx2, xx2 += txsqr) {       \
		if (*dp > dist2) {                                         \
		    if (count > here2) {                                   \
			here2 = count;                                     \
			dp1 = dp;                                          \
			rgbp1 = rgbp;                                      \
			dist1 = dist2;                                     \
			inc2 = xx2;                                        \
			thismin = here2;                                   \
		    }                                                      \
		    detect = 1;                                            \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Second loop fills in a run of closer cells.                             \
 */                                                                        \
	    for (; count <= lim;                                           \
		count++, dp++, rgbp++, dist2 += xx2, xx2 += txsqr) {       \
		if (*dp > dist2) {                                         \
		    *dp = dist2;                                           \
		    *rgbp = i + offset;                                    \
		} else {                                                   \
		    thismax = count - 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Third loop down                                                         \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * Do initializations here, since the 'find' loop                          \
 * might not get executed.                                                 \
 */                                                                        \
	    lim = min2;                                                    \
	    count = here2 - 1;                                             \
	    xx2 = inc2 - txsqr;                                            \
	    dist2 = dist1 - xx2;                                           \
	    dp = dp1 - 1;                                                  \
	    rgbp = rgbp1 - 1;                                              \
/*                                                                         \
 * The 'find' loop is executed only if we didn't already                   \
 * find something.                                                         \
 */                                                                        \
	    if (!detect)                                                   \
		for (; count >= lim;                                       \
		    count--, dp--, rgbp--, xx2 -= txsqr, dist2 -= xx2) {   \
		    if (*dp > dist2) {                                     \
			here2 = count;                                     \
			dp1 = dp;                                          \
			rgbp1 = rgbp;                                      \
			dist1 = dist2;                                     \
			inc2 = xx2;                                        \
			thismax = here2;                                   \
			detect = 1;                                        \
			break;                                             \
		    }                                                      \
		}                                                          \
	                                                                   \
/*                                                                         \
 * The 'update' loop                                                       \
 */                                                                        \
	    for (; count >= lim;                                           \
		count--, dp--, rgbp--, xx2 -= txsqr, dist2 -= xx2) {       \
		if (*dp > dist2) {                                         \
		    *dp = dist2;                                           \
		    *rgbp = i + offset;                                    \
		} else {                                                   \
		    thismin = count + 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * If we saw something, update the edge trackers.                          \
 */                                                                        \
	    if (detect) {                                                  \
	                                                                   \
/*                                                                         \
 * Predictively track. Not clear this is a win.                            \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * If there was a previous line, update the                                \
 * dmin/dmax values.                                                       \
 */                                                                        \
		if (prevmax2 >= prevmin2) {                                \
		    if (thismin > 0)                                       \
			dmin2 = thismin - prevmin2 - 1;                    \
		    else                                                   \
			dmin2 = 0;                                         \
		    if (thismax < colormax - 1)                            \
			dmax2 = thismax - prevmax2 + 1;                    \
		    else                                                   \
			dmax2 = 0;                                         \
/*                                                                         \
 * Update the min and max values by the differences.                       \
 */                                                                        \
		    max2 = thismax + dmax2;                                \
		    if (max2 >= colormax)                                  \
			max2 = colormax - 1;                               \
		    min2 = thismin + dmin2;                                \
		    if (min2 < 0)                                          \
			min2 = 0;                                          \
		}                                                          \
		prevmax2 = thismax;                                        \
		prevmin2 = thismin;                                        \
	    }                                                              \
	    ret = detect;                                                  \
	}

/* *********************************************************** */

#define	COLOR_CUBE_3_INIT_SECOND_LOOP(POINTER_TYPE)                        \
	{                                                                  \
	    mlib_s32 detect, first;                                        \
	    mlib_s32 count;                                                \
	    mlib_s32 thismin, thismax;                                     \
	                                                                   \
	    if (ret) {                                                     \
		here1 = center1;                                           \
		min1 = 0;                                                  \
		max1 = colormax - 1;                                       \
		inc1 = cinc1;                                              \
		prevmax1 = 0;                                              \
		prevmin1 = colormax;                                       \
	    }                                                              \
	    thismin = min1;                                                \
	    thismax = max1;                                                \
	    detect = 0;                                                    \
	    for (count = here1, cdist1 = dist1 = dist0, xx1 =              \
		inc1, cdp1 = dp1 = dp0, crgbp1 = rgbp1 = rgbp0, first =    \
		1; count <= max1;                                          \
		count++, dp1 += stride1, cdp1 += stride1, rgbp1 +=         \
		stride1, crgbp1 += stride1, dist1 += xx1, cdist1 +=        \
		xx1, xx1 += txsqr, first = 0) {                            \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_3_INIT_THIRD_LOOP(POINTER_TYPE);                \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			if (count > here1) {                               \
			    here1 = count;                                 \
			    dp0 = cdp1;                                    \
			    rgbp0 = crgbp1;                                \
			    dist0 = cdist1;                                \
			    inc1 = xx1;                                    \
			    thismin = here1;                               \
			}                                                  \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismax = count - 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Second loop down                                                        \
 */                                                                        \
	    for (count = here1 - 1, xx1 = inc1 - txsqr, cdist1 = dist1 =   \
		dist0 - xx1, cdp1 = dp1 = dp0 - stride1, crgbp1 =          \
		rgbp1 = rgbp0 - stride1, first = 1; count >= min1;         \
		count--, dp1 -= stride1, cdp1 -= stride1, rgbp1 -=         \
		stride1, crgbp1 -= stride1, xx1 -= txsqr, dist1 -=         \
		xx1, cdist1 -= xx1, first = 0) {                           \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_3_INIT_THIRD_LOOP(POINTER_TYPE);                \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			here1 = count;                                     \
			dp0 = cdp1;                                        \
			rgbp0 = crgbp1;                                    \
			dist0 = cdist1;                                    \
			inc1 = xx1;                                        \
			thismax = here1;                                   \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismin = count + 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	    if (detect) {                                                  \
		if (thismax < prevmax1)                                    \
		    max1 = thismax;                                        \
		prevmax1 = thismax;                                        \
		if (thismin > prevmin1)                                    \
		    min1 = thismin;                                        \
		prevmin1 = thismin;                                        \
	    }                                                              \
	    ret = detect;                                                  \
	}

/* *********************************************************** */

#define	COLOR_CUBE_3_INIT_FIRST_LOOP(POINTER_TYPE)                    \
	{                                                             \
	    mlib_s32 detect;                                          \
	    mlib_s32 count;                                           \
	    mlib_s32 first;                                           \
	                                                              \
	    detect = 0;                                               \
/*                                                                    \
 * first loop up                                                      \
 */                                                                   \
	    for (count = center0, dist0 = cdist, xx0 = cinc0, dp0 =   \
		cdp, rgbp0 = crgbp, first = 1; count < colormax;      \
		count++, dp0 += stride0, rgbp0 += stride0, dist0 +=   \
		xx0, xx0 += txsqr, first = 0) {                       \
		mlib_s32 ret = first;                                 \
	                                                              \
		COLOR_CUBE_3_INIT_SECOND_LOOP(POINTER_TYPE);          \
		if (ret)                                              \
		    detect = 1;                                       \
		else if (detect)                                      \
		    break;                                            \
	    }                                                         \
	                                                              \
/*                                                                    \
 * first loop down                                                    \
 */                                                                   \
	    for (count = center0 - 1, xx0 = cinc0 - txsqr, dist0 =    \
		cdist - xx0, dp0 = cdp - stride0, rgbp0 =             \
		crgbp - stride0, first = 1; count >= 0;               \
		count--, dp0 -= stride0, rgbp0 -= stride0, xx0 -=     \
		txsqr, dist0 -= xx0, first = 0) {                     \
		mlib_s32 ret = first;                                 \
	                                                              \
		COLOR_CUBE_3_INIT_SECOND_LOOP(POINTER_TYPE);          \
		if (ret)                                              \
		    detect = 1;                                       \
		else if (detect)                                      \
		    break;                                            \
	    }                                                         \
	}

/* *********************************************************** */

#define	COLOR_CUBE_3_INIT_MAIN_LOOP(POINTER_TYPE,                         \
	SUBTRACTION, SHIFT)                                               \
	{                                                                 \
	    POINTER_TYPE *rgbp0, *rgbp1, *crgbp, *crgbp1;                 \
	                                                                  \
	    c0 = (base0[0] - SUBTRACTION) >> SHIFT;                       \
	    c1 = (base1[0] - SUBTRACTION) >> SHIFT;                       \
	    c2 = (base2[0] - SUBTRACTION) >> SHIFT;                       \
	    dist0 = c0 - x / 2;                                           \
	    dist1 = c1 - x / 2;                                           \
	    cdist = c2 - x / 2;                                           \
	    dist0 = dist0 * dist0 + dist1 * dist1 + cdist * cdist;        \
	    if (nbits) {                                                  \
		cinc0 = 2 * (xsqr - (c0 << (nbits - SHIFT)));             \
		cinc1 = 2 * (xsqr - (c1 << (nbits - SHIFT)));             \
		cinc2 = 2 * (xsqr - (c2 << (nbits - SHIFT)));             \
	    } else {                                                      \
		cinc0 = x - 2 * c0;                                       \
		cinc1 = x - 2 * c1;                                       \
		cinc2 = x - 2 * c2;                                       \
	    }                                                             \
	    cdp = dist_buf;                                               \
	    for (c0 = 0, xx0 = cinc0; c0 < colormax;                      \
		dist0 += xx0, c0++, xx0 += txsqr)                         \
		for (c1 = 0, dist1 = dist0, xx1 = cinc1; c1 < colormax;   \
		    dist1 += xx1, c1++, xx1 += txsqr)                     \
		    for (c2 = 0, cdist = dist1, xx2 = cinc2;              \
			c2 < colormax;                                    \
			cdist += xx2, c2++, xx2 += txsqr, cdp++)          \
			*cdp = cdist;                                     \
	    for (cindex = 0; cindex < memsize / 8; cindex++)              \
		*(((mlib_d64 *)table) + cindex) = doublefirst;            \
	    for (cindex = 1; cindex < state->lutlength; cindex++) {       \
		c0 = (base0[cindex] - SUBTRACTION) >> SHIFT;              \
		c1 = (base1[cindex] - SUBTRACTION) >> SHIFT;              \
		c2 = (base2[cindex] - SUBTRACTION) >> SHIFT;              \
		if (nbits) {                                              \
		    center0 = c0 >> (nbits - SHIFT);                      \
		    center1 = c1 >> (nbits - SHIFT);                      \
		    center2 = c2 >> (nbits - SHIFT);                      \
		    cinc0 =                                               \
			xsqr - 2 * c0 * x + 2 * ((center0 << (nbits -     \
			SHIFT)) + x / 2) * x;                             \
		    cinc1 =                                               \
			xsqr - 2 * c1 * x + 2 * ((center1 << (nbits -     \
			SHIFT)) + x / 2) * x;                             \
		    cinc2 =                                               \
			xsqr - 2 * c2 * x + 2 * ((center2 << (nbits -     \
			SHIFT)) + x / 2) * x;                             \
		} else {                                                  \
		    center0 = c0;                                         \
		    center1 = c1;                                         \
		    center2 = c2;                                         \
		    cinc0 = cinc1 = cinc2 = 1;                            \
		}                                                         \
		dist0 = c0 - (center0 * x + x / 2);                       \
		dist1 = c1 - (center1 * x + x / 2);                       \
		cdist = c2 - (center2 * x + x / 2);                       \
		cdist = dist0 * dist0 + dist1 * dist1 + cdist * cdist;    \
		cdp =                                                     \
		    dist_buf + center0 * stride0 + center1 * stride1 +    \
		    center2;                                              \
		crgbp =                                                   \
		    table + center0 * stride0 + center1 * stride1 +       \
		    center2;                                              \
		COLOR_CUBE_3_INIT_FIRST_LOOP(POINTER_TYPE);               \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLOR_CUBE_4_INIT_FOURTH_LOOP(POINTER_TYPE)                        \
	{                                                                  \
	    mlib_s32 detect;                                               \
	    mlib_u32 *dp;                                                  \
	    POINTER_TYPE *rgbp;                                            \
	    mlib_u32 dist3, xx3;                                           \
	    mlib_s32 count, i = cindex;                                    \
	    mlib_s32 lim;                                                  \
	    mlib_s32 thismin, thismax;                                     \
	                                                                   \
	    if (ret) {                                                     \
		here3 = center3;                                           \
		min3 = 0;                                                  \
		max3 = colormax - 1;                                       \
		inc3 = cinc3;                                              \
		prevmin3 = colormax;                                       \
		prevmax3 = 0;                                              \
		dmin3 = 0;                                                 \
		dmax3 = 0;                                                 \
	    }                                                              \
	    detect = 0;                                                    \
	    thismin = min3;                                                \
	    thismax = max3;                                                \
/*                                                                         \
 * Third loop up                                                           \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * First loop just finds first applicable cell.                            \
 */                                                                        \
	    for (count = here3, dist3 = dist2, xx3 = inc3, dp =            \
		dp2, rgbp = rgbp2, lim = max3; count <= lim;               \
		count++, dp++, rgbp++, dist3 += xx3, xx3 += txsqr) {       \
		if (*dp > dist3) {                                         \
		    if (count > here3) {                                   \
			here3 = count;                                     \
			dp2 = dp;                                          \
			rgbp2 = rgbp;                                      \
			dist2 = dist3;                                     \
			inc3 = xx3;                                        \
			thismin = here3;                                   \
		    }                                                      \
		    detect = 1;                                            \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Second loop fills in a run of closer cells.                             \
 */                                                                        \
	    for (; count <= lim;                                           \
		count++, dp++, rgbp++, dist3 += xx3, xx3 += txsqr) {       \
		if (*dp > dist3) {                                         \
		    *dp = dist3;                                           \
		    *rgbp = i + offset;                                    \
		} else {                                                   \
		    thismax = count - 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Third loop down                                                         \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * Do initializations here, since the 'find' loop                          \
 * might not get executed.                                                 \
 */                                                                        \
	    lim = min3;                                                    \
	    count = here3 - 1;                                             \
	    xx3 = inc3 - txsqr;                                            \
	    dist3 = dist2 - xx3;                                           \
	    dp = dp2 - 1;                                                  \
	    rgbp = rgbp2 - 1;                                              \
/*                                                                         \
 * The 'find' loop is executed only if we                                  \
 * didn't already find something.                                          \
 */                                                                        \
	    if (!detect)                                                   \
		for (; count >= lim;                                       \
		    count--, dp--, rgbp--, xx3 -= txsqr, dist3 -= xx3) {   \
		    if (*dp > dist3) {                                     \
			here3 = count;                                     \
			dp2 = dp;                                          \
			rgbp2 = rgbp;                                      \
			dist2 = dist3;                                     \
			inc3 = xx3;                                        \
			thismax = here3;                                   \
			detect = 1;                                        \
			break;                                             \
		    }                                                      \
		}                                                          \
	                                                                   \
/*                                                                         \
 * The 'update' loop                                                       \
 */                                                                        \
	    for (; count >= lim;                                           \
		count--, dp--, rgbp--, xx3 -= txsqr, dist3 -= xx3) {       \
		if (*dp > dist3) {                                         \
		    *dp = dist3;                                           \
		    *rgbp = i + offset;                                    \
		} else {                                                   \
		    thismin = count + 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * If we saw something, update the edge trackers.                          \
 */                                                                        \
	    if (detect) {                                                  \
	                                                                   \
/*                                                                         \
 * Predictively track. Not clear this is a win.                            \
 * If there was a previous line, update the                                \
 * dmin/dmax values.                                                       \
 */                                                                        \
		if (prevmax3 >= prevmin3) {                                \
		    if (thismin > 0)                                       \
			dmin3 = thismin - prevmin3 - 1;                    \
		    else                                                   \
			dmin3 = 0;                                         \
		    if (thismax < colormax - 1)                            \
			dmax3 = thismax - prevmax3 + 1;                    \
		    else                                                   \
			dmax3 = 0;                                         \
/*                                                                         \
 * Update the min and max values by the                                    \
 * differences.                                                            \
 */                                                                        \
		    max3 = thismax + dmax3;                                \
		    if (max3 >= colormax)                                  \
			max3 = colormax - 1;                               \
		    min3 = thismin + dmin3;                                \
		    if (min3 < 0)                                          \
			min3 = 0;                                          \
		}                                                          \
		prevmax3 = thismax;                                        \
		prevmin3 = thismin;                                        \
	    }                                                              \
	    ret = detect;                                                  \
	}

/* *********************************************************** */

#define	COLOR_CUBE_4_INIT_THIRD_LOOP(POINTER_TYPE)                         \
	{                                                                  \
	    mlib_s32 detect, first;                                        \
	    mlib_s32 count;                                                \
	    mlib_s32 thismin, thismax;                                     \
	                                                                   \
	    if (ret) {                                                     \
		here2 = center2;                                           \
		min2 = 0;                                                  \
		max2 = colormax - 1;                                       \
		inc2 = cinc2;                                              \
		prevmax2 = 0;                                              \
		prevmin2 = colormax;                                       \
	    }                                                              \
	    thismin = min2;                                                \
	    thismax = max2;                                                \
	    detect = 0;                                                    \
	    for (count = here2, cdist2 = dist2 = dist1, xx2 =              \
		inc2, cdp2 = dp2 = dp1, crgbp2 = rgbp2 = rgbp1, first =    \
		1; count <= max2;                                          \
		count++, dp2 += stride2, cdp2 += stride2, rgbp2 +=         \
		stride2, crgbp2 += stride2, dist2 += xx2, cdist2 +=        \
		xx2, xx2 += txsqr, first = 0) {                            \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_4_INIT_FOURTH_LOOP(POINTER_TYPE);               \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			if (count > here2) {                               \
			    here2 = count;                                 \
			    dp1 = cdp2;                                    \
			    rgbp1 = crgbp2;                                \
			    dist1 = cdist2;                                \
			    inc2 = xx2;                                    \
			    thismin = here2;                               \
			}                                                  \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismax = count - 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Second loop down                                                        \
 */                                                                        \
	    for (count = here2 - 1, xx2 = inc2 - txsqr, cdist2 = dist2 =   \
		dist1 - xx2, cdp2 = dp2 = dp1 - stride2, crgbp2 =          \
		rgbp2 = rgbp1 - stride2, first = 1; count >= min2;         \
		count--, dp2 -= stride2, cdp2 -= stride2, rgbp2 -=         \
		stride2, crgbp2 -= stride2, xx2 -= txsqr, dist2 -=         \
		xx2, cdist2 -= xx2, first = 0) {                           \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_4_INIT_FOURTH_LOOP(POINTER_TYPE);               \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			here2 = count;                                     \
			dp1 = cdp2;                                        \
			rgbp1 = crgbp2;                                    \
			dist1 = cdist2;                                    \
			inc2 = xx2;                                        \
			thismax = here2;                                   \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismin = count + 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	    if (detect) {                                                  \
		if (thismax < prevmax2)                                    \
		    max2 = thismax;                                        \
		prevmax2 = thismax;                                        \
		if (thismin > prevmin2)                                    \
		    min2 = thismin;                                        \
		prevmin2 = thismin;                                        \
	    }                                                              \
	    ret = detect;                                                  \
	}

/* *********************************************************** */

#define	COLOR_CUBE_4_INIT_SECOND_LOOP(POINTER_TYPE)                        \
	{                                                                  \
	    mlib_s32 detect, first;                                        \
	    mlib_s32 count;                                                \
	    mlib_s32 thismin, thismax;                                     \
	                                                                   \
	    if (ret) {                                                     \
		here1 = center1;                                           \
		min1 = 0;                                                  \
		max1 = colormax - 1;                                       \
		inc1 = cinc1;                                              \
		prevmax1 = 0;                                              \
		prevmin1 = colormax;                                       \
	    }                                                              \
	    thismin = min1;                                                \
	    thismax = max1;                                                \
	    detect = 0;                                                    \
	    for (count = here1, cdist1 = dist1 = dist0, xx1 =              \
		inc1, cdp1 = dp1 = dp0, crgbp1 = rgbp1 = rgbp0, first =    \
		1; count <= max1;                                          \
		count++, dp1 += stride1, cdp1 += stride1, rgbp1 +=         \
		stride1, crgbp1 += stride1, dist1 += xx1, cdist1 +=        \
		xx1, xx1 += txsqr, first = 0) {                            \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_4_INIT_THIRD_LOOP(POINTER_TYPE);                \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			if (count > here1) {                               \
			    here1 = count;                                 \
			    dp0 = cdp1;                                    \
			    rgbp0 = crgbp1;                                \
			    dist0 = cdist1;                                \
			    inc1 = xx1;                                    \
			    thismin = here1;                               \
			}                                                  \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismax = count - 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	                                                                   \
/*                                                                         \
 * Second loop down                                                        \
 */                                                                        \
	    for (count = here1 - 1, xx1 = inc1 - txsqr, cdist1 = dist1 =   \
		dist0 - xx1, cdp1 = dp1 = dp0 - stride1, crgbp1 =          \
		rgbp1 = rgbp0 - stride1, first = 1; count >= min1;         \
		count--, dp1 -= stride1, cdp1 -= stride1, rgbp1 -=         \
		stride1, crgbp1 -= stride1, xx1 -= txsqr, dist1 -=         \
		xx1, cdist1 -= xx1, first = 0) {                           \
		mlib_s32 ret = first;                                      \
	                                                                   \
		COLOR_CUBE_4_INIT_THIRD_LOOP(POINTER_TYPE);                \
		if (ret) {                                                 \
		    if (!detect) {                                         \
			here1 = count;                                     \
			dp0 = cdp1;                                        \
			rgbp0 = crgbp1;                                    \
			dist0 = cdist1;                                    \
			inc1 = xx1;                                        \
			thismax = here1;                                   \
			detect = 1;                                        \
		    }                                                      \
		} else if (detect) {                                       \
		    thismin = count + 1;                                   \
		    break;                                                 \
		}                                                          \
	    }                                                              \
	    if (detect) {                                                  \
		if (thismax < prevmax1)                                    \
		    max1 = thismax;                                        \
		prevmax1 = thismax;                                        \
		if (thismin > prevmin1)                                    \
		    min1 = thismin;                                        \
		prevmin1 = thismin;                                        \
	    }                                                              \
	    ret = detect;                                                  \
	}

/* *********************************************************** */

#define	COLOR_CUBE_4_INIT_FIRST_LOOP(POINTER_TYPE)                    \
	{                                                             \
	    mlib_s32 detect;                                          \
	    mlib_s32 count;                                           \
	    mlib_s32 first;                                           \
	                                                              \
	    detect = 0;                                               \
/*                                                                    \
 * first loop up                                                      \
 */                                                                   \
	    for (count = center0, dist0 = cdist, xx0 = cinc0, dp0 =   \
		cdp, rgbp0 = crgbp, first = 1; count < colormax;      \
		count++, dp0 += stride0, rgbp0 += stride0, dist0 +=   \
		xx0, xx0 += txsqr, first = 0) {                       \
		mlib_s32 ret = first;                                 \
	                                                              \
		COLOR_CUBE_4_INIT_SECOND_LOOP(POINTER_TYPE);          \
		if (ret)                                              \
		    detect = 1;                                       \
		else if (detect)                                      \
		    break;                                            \
	    }                                                         \
	                                                              \
/*                                                                    \
 * first loop down                                                    \
 */                                                                   \
	    for (count = center0 - 1, xx0 = cinc0 - txsqr, dist0 =    \
		cdist - xx0, dp0 = cdp - stride0, rgbp0 =             \
		crgbp - stride0, first = 1; count >= 0;               \
		count--, dp0 -= stride0, rgbp0 -= stride0, xx0 -=     \
		txsqr, dist0 -= xx0, first = 0) {                     \
		mlib_s32 ret = first;                                 \
	                                                              \
		COLOR_CUBE_4_INIT_SECOND_LOOP(POINTER_TYPE);          \
		if (ret)                                              \
		    detect = 1;                                       \
		else if (detect)                                      \
		    break;                                            \
	    }                                                         \
	}

/* *********************************************************** */

#define	COLOR_CUBE_4_INIT_MAIN_LOOP(POINTER_TYPE,                          \
	SUBTRACTION, SHIFT)                                                \
	{                                                                  \
	    POINTER_TYPE *rgbp0, *rgbp1, *rgbp2, *crgbp, *crgbp1,          \
		*crgbp2;                                                   \
	    c0 = (base0[0] - SUBTRACTION) >> SHIFT;                        \
	    c1 = (base1[0] - SUBTRACTION) >> SHIFT;                        \
	    c2 = (base2[0] - SUBTRACTION) >> SHIFT;                        \
	    c3 = (base3[0] - SUBTRACTION) >> SHIFT;                        \
	    dist0 = c0 - x / 2;                                            \
	    dist1 = c1 - x / 2;                                            \
	    dist2 = c2 - x / 2;                                            \
	    cdist = c3 - x / 2;                                            \
	    dist0 =                                                        \
		dist0 * dist0 + dist1 * dist1 + dist2 * dist2 +            \
		cdist * cdist;                                             \
	    if (nbits) {                                                   \
		cinc0 = 2 * (xsqr - (c0 << (nbits - SHIFT)));              \
		cinc1 = 2 * (xsqr - (c1 << (nbits - SHIFT)));              \
		cinc2 = 2 * (xsqr - (c2 << (nbits - SHIFT)));              \
		cinc3 = 2 * (xsqr - (c3 << (nbits - SHIFT)));              \
	    } else {                                                       \
		cinc0 = x - 2 * c0;                                        \
		cinc1 = x - 2 * c1;                                        \
		cinc2 = x - 2 * c2;                                        \
		cinc3 = x - 2 * c3;                                        \
	    }                                                              \
	    cdp = dist_buf;                                                \
	    for (c0 = 0, xx0 = cinc0; c0 < colormax;                       \
		dist0 += xx0, c0++, xx0 += txsqr)                          \
		for (c1 = 0, dist1 = dist0, xx1 = cinc1; c1 < colormax;    \
		    dist1 += xx1, c1++, xx1 += txsqr)                      \
		    for (c2 = 0, dist2 = dist1, xx2 = cinc2;               \
			c2 < colormax; dist2 += xx2, c2++, xx2 += txsqr)   \
			for (c3 = 0, cdist = dist2, xx3 = cinc3;           \
			    c3 < colormax;                                 \
			    cdist += xx3, c3++, xx3 += txsqr, cdp++)       \
			    *cdp = cdist;                                  \
	    for (cindex = 0; cindex < memsize / 8; cindex++)               \
		*(((mlib_d64 *)table) + cindex) = doublefirst;             \
	    for (cindex = 1; cindex < state->lutlength; cindex++) {        \
		c0 = (base0[cindex] - SUBTRACTION) >> SHIFT;               \
		c1 = (base1[cindex] - SUBTRACTION) >> SHIFT;               \
		c2 = (base2[cindex] - SUBTRACTION) >> SHIFT;               \
		c3 = (base3[cindex] - SUBTRACTION) >> SHIFT;               \
		if (nbits) {                                               \
		    center0 = c0 >> (nbits - SHIFT);                       \
		    center1 = c1 >> (nbits - SHIFT);                       \
		    center2 = c2 >> (nbits - SHIFT);                       \
		    center3 = c3 >> (nbits - SHIFT);                       \
		    cinc0 =                                                \
			xsqr - 2 * c0 * x + 2 * ((center0 << (nbits -      \
			SHIFT)) + x / 2) * x;                              \
		    cinc1 =                                                \
			xsqr - 2 * c1 * x + 2 * ((center1 << (nbits -      \
			SHIFT)) + x / 2) * x;                              \
		    cinc2 =                                                \
			xsqr - 2 * c2 * x + 2 * ((center2 << (nbits -      \
			SHIFT)) + x / 2) * x;                              \
		    cinc3 =                                                \
			xsqr - 2 * c3 * x + 2 * ((center3 << (nbits -      \
			SHIFT)) + x / 2) * x;                              \
		} else {                                                   \
		    center0 = c0;                                          \
		    center1 = c1;                                          \
		    center2 = c2;                                          \
		    center3 = c3;                                          \
		    cinc0 = cinc1 = cinc2 = cinc3 = 1;                     \
		}                                                          \
		dist0 = c0 - (center0 * x + x / 2);                        \
		dist1 = c1 - (center1 * x + x / 2);                        \
		dist2 = c2 - (center2 * x + x / 2);                        \
		cdist = c3 - (center3 * x + x / 2);                        \
		cdist =                                                    \
		    dist0 * dist0 + dist1 * dist1 + dist2 * dist2 +        \
		    cdist * cdist;                                         \
		cdp =                                                      \
		    dist_buf + center0 * stride0 + center1 * stride1 +     \
		    center2 * stride2 + center3;                           \
		crgbp =                                                    \
		    table + center0 * stride0 + center1 * stride1 +        \
		    center2 * stride2 + center3;                           \
		COLOR_CUBE_4_INIT_FIRST_LOOP(POINTER_TYPE);                \
	    }                                                              \
	}

/* *********************************************************** */

#define	BINARY_TREE_INIT_3(POINTER_TYPE, BITS, SUBTRACTION,                \
	SHIFT)                                                             \
	{                                                                  \
	    POINTER_TYPE *base[3];                                         \
	                                                                   \
	    base[0] = state->lut[0];                                       \
	    base[1] = state->lut[1];                                       \
	    base[2] = state->lut[2];                                       \
	    table = __mlib_malloc(sizeof (struct lut_node_3));             \
	    if (!table)                                                    \
		return (NULL);                                             \
	    table->tag = 0;                                                \
	    table->contents.quadrants[0] = 0;                              \
	    table->contents.quadrants[1] = 0;                              \
	    table->contents.quadrants[2] = 0;                              \
	    table->contents.quadrants[3] = 0;                              \
	    table->contents.quadrants[4] = 0;                              \
	    table->contents.quadrants[5] = 0;                              \
	    table->contents.quadrants[6] = 0;                              \
	    table->contents.quadrants[7] = 0;                              \
	    for (i = 0; i < state->lutlength; i++) {                       \
		mlib_s32 q;                                                \
		mlib_s32 finish = 0;                                       \
		mlib_s32 pass = BITS - 1;                                  \
		mlib_u32 c0, c1, c2;                                       \
	                                                                   \
		node = table;                                              \
		c0 = base[0][i] - SUBTRACTION;                             \
		c1 = base[1][i] - SUBTRACTION;                             \
		c2 = base[2][i] - SUBTRACTION;                             \
		do {                                                       \
		    q = ((c0 >> pass) & 1) | (((c1 << 1) >> pass) & 2) |   \
			(((c2 << 2) >> pass) & 4);                         \
		    if (node->tag & (1 << q)) {                            \
	                                                                   \
/*                                                                         \
 * change this cell to a node                                              \
 */                                                                        \
			mlib_s32 identical;                                \
			mlib_s32 index = node->contents.index[q];          \
			mlib_u32 palc0, palc1, palc2;                      \
	                                                                   \
			palc0 = base[0][index] - SUBTRACTION;              \
			palc1 = base[1][index] - SUBTRACTION;              \
			palc2 = base[2][index] - SUBTRACTION;              \
			identical =                                        \
			    (palc0 - c0) | (palc1 - c1) | (palc2 - c2);    \
			if (!identical) {                                  \
	                                                                   \
/*                                                                         \
 * Nodes are the same. Do we care ? Nope                                   \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * This index will be lost, so we leave                                    \
 */                                                                        \
			    finish = 1;                                    \
			} else {                                           \
			    mlib_s32 still_in_the_same_cell = 1;           \
			    mlib_s32 oldq;                                 \
	                                                                   \
			    if (BITS - pass >= bits) {                     \
	                                                                   \
/*                                                                         \
 * We found a color already on the bottom of the                           \
 * tree. Now we need to find out, what color is better                     \
 * here                                                                    \
 */                                                                        \
				mlib_s32 mask = ~((1 << pass) - 1);        \
				mlib_u32 olddistance =                     \
				    FIND_DISTANCE_3(palc0, palc0 & mask,   \
				    palc1, palc1 & mask, palc2,            \
				    palc2 & mask, SHIFT);                  \
				mlib_u32 newdistance =                     \
				    FIND_DISTANCE_3(c0, c0 & mask, c1,     \
				    c1 & mask, c2, c2 & mask, SHIFT);      \
				if (newdistance < olddistance) {           \
	                                                                   \
/*                                                                         \
 * Change color index                                                      \
 */                                                                        \
				    node->contents.index[q] = i;           \
				}                                          \
				finish = 1;                                \
			    } else {                                       \
				do {                                       \
				    mlib_s32 prevq = q;                    \
	                                                                   \
				    pass--;                                \
/*                                                                         \
 * split this cell to 2 quadrants if we are lucky. If                      \
 * not, do this several times until we become lucky. It                    \
 * may not happen infinitely since points are different                    \
 */                                                                        \
				    q = ((c0 >> pass) & 1) | (((c1 << 1)   \
					>> pass) & 2) | (((c2 << 2) >>     \
					pass) & 4);                        \
				    oldq =                                 \
					((palc0 >> pass) & 1) | (((palc1   \
					<< 1) >> pass) & 2) | (((palc2     \
					<< 2) >> pass) & 4);               \
				    if (oldq != q ||                       \
					BITS - pass >= bits) {             \
	                                                                   \
/*                                                                         \
 * we have found the place where we place both points                      \
 * in different cells. That is what we were looking for.                   \
 * I know we are lucky :)                                                  \
 */                                                                        \
					struct lut_node_3 *new_node;       \
					new_node =                         \
					    __mlib_malloc                  \
					    (sizeof (struct lut_node_3));  \
					if (!new_node) {                   \
					    __mlib_free_binary_tree_3      \
						(table);                   \
					    return (NULL);                 \
					}                                  \
	                                                                   \
/*                                                                         \
 * Set the tag mask for these new cells                                    \
 */                                                                        \
					new_node->tag =                    \
					    (1 << q) | (1 << oldq);        \
					new_node->contents.index[0] = 0;   \
					new_node->contents.index[1] = 0;   \
					new_node->contents.index[2] = 0;   \
					new_node->contents.index[3] = 0;   \
					new_node->contents.index[4] = 0;   \
					new_node->contents.index[5] = 0;   \
					new_node->contents.index[6] = 0;   \
					new_node->contents.index[7] = 0;   \
					if (q != oldq) {                   \
					    new_node->contents.            \
						index[q] = i;              \
					    new_node->contents.            \
						index[oldq] = index;       \
					} else {                           \
					    mlib_s32 mask =                \
						~((1 << pass) - 1);        \
					    mlib_u32 olddistance =         \
						FIND_DISTANCE_3(palc0,     \
						palc0 & mask, palc1,       \
						palc1 & mask, palc2,       \
						palc2 & mask, SHIFT);      \
					    mlib_u32 newdistance =         \
						FIND_DISTANCE_3(c0,        \
						c0 & mask, c1,             \
						c1 & mask, c2,             \
						c2 & mask, SHIFT);         \
/*                                                                         \
 * Change color index                                                      \
 */                                                                        \
					    if (newdistance <              \
						olddistance)               \
						new_node->contents.        \
						    index[q] = i;          \
					    else                           \
						new_node->contents.        \
						    index[q] = index;      \
					}                                  \
	                                                                   \
/*                                                                         \
 * Here is not a color cell now. Here is a pointer                         \
 */                                                                        \
					node->tag &= ~(1 << prevq);        \
					node->contents.                    \
					    quadrants[prevq] = new_node;   \
					still_in_the_same_cell = 0;        \
					finish = 1;                        \
				    } else {                               \
	                                                                   \
/*                                                                         \
 * we have to split this cell and continue splitting                       \
 * then, because points again in the same cell                             \
 */                                                                        \
					struct lut_node_3 *new_node;       \
					new_node =                         \
					    __mlib_malloc                  \
					    (sizeof (struct lut_node_3));  \
					if (!new_node) {                   \
					    __mlib_free_binary_tree_3      \
						(table);                   \
					    return (NULL);                 \
					}                                  \
	                                                                   \
/*                                                                         \
 * copy the information about colors down the tree                         \
 */                                                                        \
					new_node->tag = 1 << q;            \
					new_node->contents.index[0] = 0;   \
					new_node->contents.index[1] = 0;   \
					new_node->contents.index[2] = 0;   \
					new_node->contents.index[3] = 0;   \
					new_node->contents.index[4] = 0;   \
					new_node->contents.index[5] = 0;   \
					new_node->contents.index[6] = 0;   \
					new_node->contents.index[7] = 0;   \
					new_node->contents.index[q] =      \
					    index;                         \
/*                                                                         \
 * attach the new node                                                     \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * Here is not a color cell now. Here is a pointer                         \
 */                                                                        \
					node->tag &= ~(1 << prevq);        \
					node->contents.                    \
					    quadrants[prevq] = new_node;   \
					node = new_node;                   \
/*                                                                         \
 * and continue                                                            \
 */                                                                        \
				    }                                      \
				}                                          \
				while (still_in_the_same_cell);            \
			    }                                              \
			}                                                  \
		    } else {                                               \
			if (node->contents.quadrants[q]) {                 \
	                                                                   \
/*                                                                         \
 * Down we go                                                              \
 */                                                                        \
			    node = node->contents.quadrants[q];            \
			} else {                                           \
	                                                                   \
/*                                                                         \
 * this quadrant is yet empty                                              \
 * let's fill it with new color                                            \
 */                                                                        \
			    node->tag |= 1 << q;                           \
			    node->contents.index[q] = i;                   \
/*                                                                         \
 * and LEAVE !                                                             \
 */                                                                        \
			    finish = 1;                                    \
			}                                                  \
		    }                                                      \
		    pass--;                                                \
		}                                                          \
		while (!finish);                                           \
	    }                                                              \
	}

/* *********************************************************** */

#define	BINARY_TREE_INIT_4(POINTER_TYPE, BITS, SUBTRACTION,                \
	SHIFT)                                                             \
	{                                                                  \
	    POINTER_TYPE *base[4];                                         \
	                                                                   \
	    base[0] = state->lut[0];                                       \
	    base[1] = state->lut[1];                                       \
	    base[2] = state->lut[2];                                       \
	    base[3] = state->lut[3];                                       \
	    table = __mlib_malloc(sizeof (struct lut_node_4));             \
	    if (!table)                                                    \
		return (NULL);                                             \
	    table->tag = 0;                                                \
	    table->contents.quadrants[0] = 0;                              \
	    table->contents.quadrants[1] = 0;                              \
	    table->contents.quadrants[2] = 0;                              \
	    table->contents.quadrants[3] = 0;                              \
	    table->contents.quadrants[4] = 0;                              \
	    table->contents.quadrants[5] = 0;                              \
	    table->contents.quadrants[6] = 0;                              \
	    table->contents.quadrants[7] = 0;                              \
	    table->contents.quadrants[8] = 0;                              \
	    table->contents.quadrants[9] = 0;                              \
	    table->contents.quadrants[10] = 0;                             \
	    table->contents.quadrants[11] = 0;                             \
	    table->contents.quadrants[12] = 0;                             \
	    table->contents.quadrants[13] = 0;                             \
	    table->contents.quadrants[14] = 0;                             \
	    table->contents.quadrants[15] = 0;                             \
	    for (i = 0; i < state->lutlength; i++) {                       \
		mlib_s32 q;                                                \
		mlib_s32 finish = 0;                                       \
		mlib_s32 pass = BITS - 1;                                  \
		mlib_u32 c0, c1, c2, c3;                                   \
	                                                                   \
		node = table;                                              \
		c0 = base[0][i] - SUBTRACTION;                             \
		c1 = base[1][i] - SUBTRACTION;                             \
		c2 = base[2][i] - SUBTRACTION;                             \
		c3 = base[3][i] - SUBTRACTION;                             \
		do {                                                       \
		    q = ((c0 >> pass) & 1) | (((c1 << 1) >> pass) & 2) |   \
			(((c2 << 2) >> pass) & 4) | (((c3 << 3) >> pass)   \
			& 8);                                              \
		    if (node->tag & (1 << q)) {                            \
	                                                                   \
/*                                                                         \
 * change this cell to a node                                              \
 */                                                                        \
			mlib_s32 identical;                                \
			mlib_s32 index = node->contents.index[q];          \
			mlib_u32 palc0, palc1, palc2, palc3;               \
	                                                                   \
			palc0 = (base[0][index] - SUBTRACTION);            \
			palc1 = (base[1][index] - SUBTRACTION);            \
			palc2 = (base[2][index] - SUBTRACTION);            \
			palc3 = (base[3][index] - SUBTRACTION);            \
			identical =                                        \
			    (palc0 - c0) | (palc1 - c1) | (palc2 -         \
			    c2) | (palc3 - c3);                            \
			if (!identical) {                                  \
	                                                                   \
/*                                                                         \
 * Nodes are the same. Do we care ? Nope                                   \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * This index will be lost, so we leave                                    \
 */                                                                        \
			    finish = 1;                                    \
			} else {                                           \
			    mlib_s32 still_in_the_same_cell = 1;           \
			    mlib_s32 oldq;                                 \
	                                                                   \
			    if (BITS - pass >= bits) {                     \
	                                                                   \
/*                                                                         \
 * We found a color already on the bottom of the                           \
 * tree. Now we need to find out, what color is better                     \
 * here                                                                    \
 */                                                                        \
				mlib_s32 mask = ~((1 << pass) - 1);        \
				mlib_u32 olddistance =                     \
				    FIND_DISTANCE_4(palc0, palc0 & mask,   \
				    palc1, palc1 & mask, palc2,            \
				    palc2 & mask, palc3, palc3 & mask,     \
				    SHIFT);                                \
				mlib_u32 newdistance =                     \
				    FIND_DISTANCE_4(c0, c0 & mask, c1,     \
				    c1 & mask, c2, c2 & mask, c3,          \
				    c3 & mask, SHIFT);                     \
				if (newdistance < olddistance) {           \
	                                                                   \
/*                                                                         \
 * Change color index                                                      \
 */                                                                        \
				    node->contents.index[q] = i;           \
				}                                          \
				finish = 1;                                \
			    } else {                                       \
				do {                                       \
				    mlib_s32 prevq = q;                    \
	                                                                   \
				    pass--;                                \
/*                                                                         \
 * split this cell to 2 quadrants if we are lucky. If                      \
 * not, do this several times until we become lucky. It                    \
 * may not happen infinitely since points are different                    \
 */                                                                        \
				    q = ((c0 >> pass) & 1) | (((c1 << 1)   \
					>> pass) & 2) | (((c2 << 2) >>     \
					pass) & 4) | (((c3 << 3) >>        \
					pass) & 8);                        \
				    oldq =                                 \
					((palc0 >> pass) & 1) | (((palc1   \
					<< 1) >> pass) & 2) | (((palc2     \
					<< 2) >> pass) & 4) | (((palc3     \
					<< 3) >> pass) & 8);               \
				    if (oldq != q ||                       \
					BITS - pass >= bits) {             \
	                                                                   \
/*                                                                         \
 * we have found the place where we place both points                      \
 * in different cells. That is what we were looking for.                   \
 */                                                                        \
					struct lut_node_4 *new_node;       \
					new_node =                         \
					    __mlib_malloc                  \
					    (sizeof (struct lut_node_4));  \
					if (!new_node) {                   \
					    __mlib_free_binary_tree_4      \
						(table);                   \
					    return (NULL);                 \
					}                                  \
	                                                                   \
/*                                                                         \
 * Set the tag mask for these new cells                                    \
 */                                                                        \
					new_node->tag =                    \
					    (1 << q) | (1 << oldq);        \
					new_node->contents.index[0] = 0;   \
					new_node->contents.index[1] = 0;   \
					new_node->contents.index[2] = 0;   \
					new_node->contents.index[3] = 0;   \
					new_node->contents.index[4] = 0;   \
					new_node->contents.index[5] = 0;   \
					new_node->contents.index[6] = 0;   \
					new_node->contents.index[7] = 0;   \
					new_node->contents.index[8] = 0;   \
					new_node->contents.index[9] = 0;   \
					new_node->contents.index[10] =     \
					    0;                             \
					new_node->contents.index[11] =     \
					    0;                             \
					new_node->contents.index[12] =     \
					    0;                             \
					new_node->contents.index[13] =     \
					    0;                             \
					new_node->contents.index[14] =     \
					    0;                             \
					new_node->contents.index[15] =     \
					    0;                             \
					if (q != oldq) {                   \
					    new_node->contents.            \
						index[q] = i;              \
					    new_node->contents.            \
						index[oldq] = index;       \
					} else {                           \
					    mlib_s32 mask =                \
						~((1 << pass) - 1);        \
					    mlib_u32 olddistance =         \
						FIND_DISTANCE_4(palc0,     \
						palc0 & mask, palc1,       \
						palc1 & mask, palc2,       \
						palc2 & mask, palc3,       \
						palc3 & mask, SHIFT);      \
					    mlib_u32 newdistance =         \
						FIND_DISTANCE_4(c0,        \
						c0 & mask, c1,             \
						c1 & mask, c2,             \
						c2 & mask, c3,             \
						c3 & mask, SHIFT);         \
/*                                                                         \
 * Change color index                                                      \
 */                                                                        \
					    if (newdistance <              \
						olddistance)               \
						new_node->contents.        \
						    index[q] = i;          \
					    else                           \
						new_node->contents.        \
						    index[q] = index;      \
					}                                  \
	                                                                   \
/*                                                                         \
 * Here is not a color cell now. Here is a pointer                         \
 */                                                                        \
					node->tag &= ~(1 << prevq);        \
					node->contents.                    \
					    quadrants[prevq] = new_node;   \
					still_in_the_same_cell = 0;        \
					finish = 1;                        \
				    } else {                               \
	                                                                   \
/*                                                                         \
 * we have to split this cell and continue splitting                       \
 * then, because points again in the same cell                             \
 */                                                                        \
					struct lut_node_4 *new_node;       \
					new_node =                         \
					    __mlib_malloc                  \
					    (sizeof (struct lut_node_4));  \
					if (!new_node) {                   \
					    __mlib_free_binary_tree_4      \
						(table);                   \
					    return (NULL);                 \
					}                                  \
	                                                                   \
/*                                                                         \
 * copy the information about colors down the tree                         \
 */                                                                        \
					new_node->tag = 1 << q;            \
					new_node->contents.index[0] = 0;   \
					new_node->contents.index[1] = 0;   \
					new_node->contents.index[2] = 0;   \
					new_node->contents.index[3] = 0;   \
					new_node->contents.index[4] = 0;   \
					new_node->contents.index[5] = 0;   \
					new_node->contents.index[6] = 0;   \
					new_node->contents.index[7] = 0;   \
					new_node->contents.index[8] = 0;   \
					new_node->contents.index[9] = 0;   \
					new_node->contents.index[10] =     \
					    0;                             \
					new_node->contents.index[11] =     \
					    0;                             \
					new_node->contents.index[12] =     \
					    0;                             \
					new_node->contents.index[13] =     \
					    0;                             \
					new_node->contents.index[14] =     \
					    0;                             \
					new_node->contents.index[15] =     \
					    0;                             \
					new_node->contents.index[q] =      \
					    index;                         \
/*                                                                         \
 * attach the new node                                                     \
 */                                                                        \
	                                                                   \
/*                                                                         \
 * Here is not a color cell now. Here is a pointer                         \
 */                                                                        \
					node->tag &= ~(1 << prevq);        \
					node->contents.                    \
					    quadrants[prevq] = new_node;   \
					node = new_node;                   \
/*                                                                         \
 * and continue                                                            \
 */                                                                        \
				    }                                      \
				}                                          \
				while (still_in_the_same_cell);            \
			    }                                              \
			}                                                  \
		    } else {                                               \
			if (node->contents.quadrants[q]) {                 \
	                                                                   \
/*                                                                         \
 * Down we go                                                              \
 */                                                                        \
			    node = node->contents.quadrants[q];            \
			} else {                                           \
	                                                                   \
/*                                                                         \
 * this quadrant is yet empty                                              \
 * let's fill it with new color                                            \
 */                                                                        \
			    node->tag |= 1 << q;                           \
			    node->contents.index[q] = i;                   \
/*                                                                         \
 * and LEAVE !                                                             \
 */                                                                        \
			    finish = 1;                                    \
			}                                                  \
		    }                                                      \
		    pass--;                                                \
		}                                                          \
		while (!finish);                                           \
	    }                                                              \
	}

/* *********************************************************** */

#define	FILL_BUFF(N)                                                   \
	if (dim##N > 1) {                                              \
	    scale = drange / (dim##N - 1);                             \
	    if (!sign##N) {                                            \
		for (i = 0; i < dim##N; i++) {                         \
		    buff##N[i] = (mlib_s32)(i * scale + 0.5) + roff;   \
		}                                                      \
	    } else {                                                   \
		for (i = 0; i < dim##N; i++) {                         \
		    buff##N[dim##N - 1 - i] =                          \
			(mlib_s32)(i * scale + 0.5) + roff;            \
		}                                                      \
	    }                                                          \
	} else {                                                       \
	    buff##N[0] = 128;                                          \
	}                                                              \
	                                                               \
	p_dim[N] = dim##N

/* *********************************************************** */

#define	FILL_LUT_3CH(TYPE, colors0, colors1, colors2)           \
	{                                                       \
	    TYPE *ptr0 = (void *)colors0;                       \
	    TYPE *ptr1 = (void *)colors1;                       \
	    TYPE *ptr2 = (void *)colors2;                       \
	                                                        \
	    for (i2 = 0; i2 < dim2; i2++) {                     \
		for (i1 = 0; i1 < dim1; i1++) {                 \
		    for (i0 = 0; i0 < dim0; i0++) {             \
			(*ptr0++) = (TYPE) buff0[i0];           \
			(*ptr1++) = (TYPE) buff1[i1];           \
			(*ptr2++) = (TYPE) buff2[i2];           \
		    }                                           \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	FILL_LUT_4CH(TYPE, colors0, colors1, colors2, colors3)   \
	{                                                        \
	    TYPE *ptr0 = (void *)colors0;                        \
	    TYPE *ptr1 = (void *)colors1;                        \
	    TYPE *ptr2 = (void *)colors2;                        \
	    TYPE *ptr3 = (void *)colors3;                        \
	                                                         \
	    for (i3 = 0; i3 < dim3; i3++) {                      \
		for (i2 = 0; i2 < dim2; i2++) {                  \
		    for (i1 = 0; i1 < dim1; i1++) {              \
			for (i0 = 0; i0 < dim0; i0++) {          \
			    (*ptr0++) = (TYPE) buff0[i0];        \
			    (*ptr1++) = (TYPE) buff1[i1];        \
			    (*ptr2++) = (TYPE) buff2[i2];        \
			    (*ptr3++) = (TYPE) buff3[i3];        \
			}                                        \
		    }                                            \
		}                                                \
	    }                                                    \
	}

/* *********************************************************** */

#define	FILL_TAB_3CH(transformed_table, TYPE, tab_size)               \
	{                                                             \
	    mlib_s32 dim01 = dim0 * dim1;                             \
	    TYPE *tab0 = (void *)transformed_table;                   \
	    TYPE *tab1 = tab0 + tab_size;                             \
	    TYPE *tab2 = tab1 + tab_size;                             \
	                                                              \
	    for (val = 0; val < tab_size; val++) {                    \
		tab0[val] =                                           \
		    (mlib_s32)(((val ^ sign0) + src_off) * scale0 +   \
		    0.5) + offset;                                    \
		tab1[val] =                                           \
		    (mlib_s32)(((val ^ sign1) + src_off) * scale1 +   \
		    0.5) * dim0;                                      \
		tab2[val] =                                           \
		    (mlib_s32)(((val ^ sign2) + src_off) * scale2 +   \
		    0.5) * dim01;                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FILL_TAB_4CH(transformed_table, TYPE, tab_size)               \
	{                                                             \
	    mlib_s32 dim01 = dim0 * dim1;                             \
	    mlib_s32 dim012 = dim01 * dim2;                           \
	    TYPE *tab0 = (void *)transformed_table;                   \
	    TYPE *tab1 = tab0 + tab_size;                             \
	    TYPE *tab2 = tab1 + tab_size;                             \
	    TYPE *tab3 = tab2 + tab_size;                             \
	                                                              \
	    for (val = 0; val < tab_size; val++) {                    \
		tab0[val] =                                           \
		    (mlib_s32)(((val ^ sign0) + src_off) * scale0 +   \
		    0.5) + offset;                                    \
		tab1[val] =                                           \
		    (mlib_s32)(((val ^ sign1) + src_off) * scale1 +   \
		    0.5) * dim0;                                      \
		tab2[val] =                                           \
		    (mlib_s32)(((val ^ sign2) + src_off) * scale2 +   \
		    0.5) * dim01;                                     \
		tab3[val] =                                           \
		    (mlib_s32)(((val ^ sign3) + src_off) * scale3 +   \
		    0.5) * dim012;                                    \
	    }                                                         \
	}

/* *********************************************************** */

#ifdef i386
#define	MAX_COLOR_CUBE_BITS	21
#else /* i386 */

#define	MAX_COLOR_CUBE_BITS	24
#endif /* i386 */

/* *********************************************************** */

#define	LUT_COLOR_BIT	10

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

mlib_status
mlib_ImageColorTrue2IndexInit2(
    void **p_colormap,
    const mlib_s32 *dimensions,
    mlib_s32 bits,
    mlib_type intype,
    mlib_type outtype,
    mlib_s32 channels,
    mlib_s32 entries,
    mlib_s32 offset,
    void **table)
{
	mlib_colormap *colormap;
	mlib_s32 src_bits, method, outsize, pal_size;
	void *transformed_table = NULL;
	static mlib_s32 switch_table[2][2] = {
/*  U8_S16_3 */
/*  U8_S16_4 */
		{LUT_BYTE_COLORS_3CHANNELS, LUT_BYTE_COLORS_4CHANNELS},
/* S16_S16_3 */
/* S16_S16_4 */
		{LUT_SHORT_COLORS_3CHANNELS, LUT_SHORT_COLORS_4CHANNELS}
	};

/* Check for obvious errors */

	if (!((channels >= 3 && channels <= 4) || (channels == 1 &&
	    outtype == MLIB_BIT))) {
		return (MLIB_OUTOFRANGE);
	}

	if (table != NULL) {
		if (table[0] == NULL) {
			return (MLIB_NULLPOINTER);
		}

		if (channels >= 3) {
			if (table[1] == NULL || table[2] == NULL) {
				return (MLIB_NULLPOINTER);
			}
		}

		if (channels == 4) {
			if (table[3] == NULL) {
				return (MLIB_NULLPOINTER);
			}
		}

	} else {

		if (dimensions == NULL) {
			return (MLIB_NULLPOINTER);
		}
	}

	if (outtype == MLIB_BIT) {
		offset = 0;
		entries = 2;
	} else
	if (dimensions != NULL) {
		entries = dimensions[0] * dimensions[1] * dimensions[2];

		if (channels == 4) {
			entries *= dimensions[3];
		}

		if (entries < 0)
			entries = -entries;
	}

	if (entries <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	if (intype == MLIB_BYTE)
		src_bits = 8;
	else if (intype == MLIB_SHORT)
		src_bits = 16;
	else
		return (MLIB_FAILURE);

	if (src_bits < bits)
		return (MLIB_FAILURE);

	if (offset >= 0 && offset + entries - 1 <= 1) {
		outsize = 0;
	} else
	if (offset >= MLIB_U8_MIN && offset + entries - 1 <= MLIB_U8_MAX) {
		outsize = 1;
	} else {
		outsize = 2;

		if (offset < MLIB_S16_MIN ||
		    offset + entries - 1 > MLIB_S16_MAX)
			return (MLIB_OUTOFRANGE);
	}

	if (outtype == MLIB_BYTE && outsize == 2) {
		return (MLIB_FAILURE);
	}

	if (outtype == MLIB_BIT && outsize > 0) {
		return (MLIB_FAILURE);
	}

	colormap = __mlib_malloc(sizeof (mlib_colormap));

	if (colormap == NULL) {
		return (MLIB_FAILURE);
	}

	if (outtype == MLIB_BIT) {
		mlib_s32 col0, col1;
		mlib_u8 **colors = (mlib_u8 **)table;

		transformed_table = __mlib_malloc(8 + sizeof (void *));
		if (transformed_table == NULL) {
			__mlib_free(colormap);
			return (MLIB_FAILURE);
		}

		if (dimensions != NULL) {
			if (abs(dimensions[0]) == 1) {
				entries = 1;
				col0 = col1 = 128;
			} else {
				entries = 2;

				if (dimensions[0] >= 0) {
					col0 = 0;
					col1 = 255;
				} else {
					col0 = 255;
					col1 = 0;
				}
			}

			if (table == NULL) {
				table =
				    (void **)((mlib_u8 *)transformed_table + 8);
				table[0] = (mlib_u8 *)transformed_table + 4;
				colors = (mlib_u8 **)table;
			}

			colors[0][0] = col0;
			colors[0][1] = col1;
		} else {
			col0 = colors[0][0];

			if (entries > 1) {
				col1 = colors[0][1];
			} else {
				col1 = col0;
			}
		}

		((mlib_u8 *)transformed_table)[0] = col0;
		((mlib_u8 *)transformed_table)[1] = col1;

		if (entries > 1) {
			((mlib_u8 *)transformed_table)[2] = (col0 + col1) >> 1;
		} else {
			((mlib_u8 *)transformed_table)[2] = 255;
		}

		method = LUT_COLOR_BIT;
	} else
/* build lut */
	if (dimensions != NULL) {
		mlib_s32 dim0 = dimensions[0];
		mlib_s32 dim1 = dimensions[1];
		mlib_s32 dim2 = dimensions[2];
		mlib_s32 dim3 = 1;
		mlib_u8 *colors0, *colors1, *colors2, *colors3 = NULL;
		mlib_s32 *p_dim;
		mlib_s16 buff_lcl[BUFF_SIZE], *buff0 =
		    buff_lcl, *buff1, *buff2, *buff3;
		mlib_d64 scale, drange;
		mlib_s32 i, i0, i1, i2, i3, val, roff;
		mlib_s32 sign0, sign1, sign2, sign3;
		mlib_s32 nchan = channels;
		mlib_s32 lut_size =
		    (outtype == MLIB_BYTE) ? 256 : 256 * sizeof (mlib_s16);
		mlib_s32 tab_size = lut_size;

		if (intype == MLIB_SHORT) {
/* 256 -> 1024 */
			tab_size *= 4;
		}

		if (nchan == 4) {
			dim3 = dimensions[3];
		}

		if (intype == MLIB_BYTE) {
			drange = 256 - 1;
			roff = MLIB_U8_MIN;
		} else {
			drange = 65536 - 1;
			roff = MLIB_S16_MIN;
		}

		if (table != NULL) {
			mlib_u8 **colors = (mlib_u8 **)table;

			colors0 = colors[0];
			colors1 = colors[1];
			colors2 = colors[2];

			if (nchan == 4)
				colors3 = colors[3];

			transformed_table =
			    __mlib_malloc(nchan * tab_size +
			    4 * sizeof (mlib_s32));
			if (transformed_table == NULL) {
				__mlib_free(colormap);
				return (MLIB_FAILURE);
			}

		} else {
			transformed_table =
			    __mlib_malloc(nchan * tab_size +
			    4 * sizeof (mlib_s32) + nchan * lut_size +
			    4 * sizeof (void *));
			if (transformed_table == NULL) {
				__mlib_free(colormap);
				return (MLIB_FAILURE);
			}

			colors0 =
			    (mlib_u8 *)transformed_table + nchan * tab_size +
			    4 * sizeof (mlib_s32);
			colors1 = colors0 + lut_size;
			colors2 = colors1 + lut_size;
			colors3 = colors2 + lut_size;
			table = (void **)(colors0 + nchan * lut_size);
			table[0] = colors0;
			table[1] = colors1;
			table[2] = colors2;
			table[3] = colors3;
		}

		p_dim =
		    (mlib_s32 *)((mlib_u8 *)transformed_table +
		    nchan * tab_size);

		sign0 = dim0 >> 31;
		if (dim0 < 0)
			dim0 = -dim0;
		sign1 = dim1 >> 31;
		if (dim1 < 0)
			dim1 = -dim1;
		sign2 = dim2 >> 31;
		if (dim2 < 0)
			dim2 = -dim2;
		sign3 = dim3 >> 31;
		if (dim3 < 0)
			dim3 = -dim3;

		if (dim0 + dim1 + dim2 + dim3 > BUFF_SIZE) {
			buff0 =
			    __mlib_malloc((dim0 + dim1 + dim2 +
			    dim3) * sizeof (mlib_s16));

			if (buff0 == NULL) {
				__mlib_free(colormap);
				return (MLIB_FAILURE);
			}
		}

		buff1 = buff0 + dim0;
		buff2 = buff1 + dim1;
		buff3 = buff2 + dim2;

		FILL_BUFF(0);
		FILL_BUFF(1);
		FILL_BUFF(2);

		if (nchan == 3) {
			if (intype == MLIB_BYTE) {
				FILL_LUT_3CH(mlib_u8,
				    colors0,
				    colors1,
				    colors2)
			} else
			{
				FILL_LUT_3CH(mlib_s16,
				    colors0,
				    colors1,
				    colors2)
			}

		} else
		{
			FILL_BUFF(3);

			if (intype == MLIB_BYTE) {
				FILL_LUT_4CH(mlib_u8,
				    colors0,
				    colors1,
				    colors2,
				    colors3)
			} else
			{
				FILL_LUT_4CH(mlib_s16,
				    colors0,
				    colors1,
				    colors2,
				    colors3)
			}
		}

		tab_size = (intype == MLIB_BYTE) ? 256 : 1024;

		if (intype == MLIB_BYTE) {
			sign0 &= 0xFF;
			sign1 &= 0xFF;
			sign2 &= 0xFF;
			sign3 &= 0xFF;
		} else {
			sign0 = (sign0 & 0x3FF) ^ 0x200;
			sign1 = (sign1 & 0x3FF) ^ 0x200;
			sign2 = (sign2 & 0x3FF) ^ 0x200;
			sign3 = (sign3 & 0x3FF) ^ 0x200;
		}

		bits = 0;
		method = LUT_COLOR_DIMENSIONS;
		{
/* build inverse tables */
			mlib_d64 src_off = (intype == MLIB_BYTE) ? 0 : 0.5;
			mlib_d64 scale =
			    (intype ==
			    MLIB_BYTE) ? 1.0 / 255 : 65536.0 / (tab_size *
			    65535.0);
			mlib_d64 scale0 = (dim0 - 1) * scale;
			mlib_d64 scale1 = (dim1 - 1) * scale;
			mlib_d64 scale2 = (dim2 - 1) * scale;
			mlib_d64 scale3 = (dim3 - 1) * scale;

			if (nchan == 3) {
				if (outtype == MLIB_BYTE) {
					FILL_TAB_3CH(transformed_table, mlib_u8,
					    tab_size);
				} else {
					FILL_TAB_3CH(transformed_table,
					    mlib_s16,
					    tab_size);
				}

			} else {

				if (outtype == MLIB_BYTE) {
					FILL_TAB_4CH(transformed_table, mlib_u8,
					    tab_size);
				} else {
					FILL_TAB_4CH(transformed_table,
					    mlib_s16,
					    tab_size);
				}
			}
		}

		if (buff0 != buff_lcl) {
			__mlib_free(buff0);
		}

	} else
	if (channels * bits <= MAX_COLOR_CUBE_BITS && bits > 0) {
		method = LUT_COLOR_CUBE_SEARCH;
	} else {

		if (entries <= switch_table[src_bits / 8 - 1][channels - 3])
			method = LUT_STUPID_SEARCH;
		else
			method = LUT_BINARY_TREE_SEARCH;

		if (bits <= 0)
			bits = src_bits;
	}

	colormap->lut = table;
	colormap->intype = intype;
	colormap->outtype = outtype;
	colormap->indexsize = outsize;
	colormap->bits = bits;
	colormap->method = method;
	colormap->channels = channels;
	colormap->lutlength = entries;
	colormap->offset = offset;

	pal_size = entries * 4;

	if (intype == MLIB_SHORT) {
		pal_size *= 2;
	}

	if (outtype != MLIB_BIT) {
		mlib_s32 j;
		mlib_s32 *normal_table;
		mlib_d64 *double_lut;

		normal_table = __mlib_malloc(pal_size);

		if (normal_table == NULL) {
			__mlib_free(colormap);
			return (MLIB_FAILURE);
		}

		colormap->normal_table = (mlib_u8 *)normal_table;

		double_lut =
		    __mlib_malloc((entries + 1) * channels * sizeof (mlib_d64));

		if (double_lut == NULL) {
			__mlib_free(colormap->normal_table);
			__mlib_free(colormap);
			return (MLIB_FAILURE);
		}

		colormap->double_lut = (mlib_d64 *)double_lut;

		if (intype == MLIB_BYTE) {
			mlib_u8 **colors = (mlib_u8 **)table;

			if (channels == 3) {
				for (j = 0; j < entries; j++) {
#ifdef _LITTLE_ENDIAN
					normal_table[j] = (colors[0][j] << 8) |
					    (colors[1][j] << 16) | (colors[2][j]
					    << 24);
#else /* _LITTLE_ENDIAN */
					normal_table[j] =
					    (colors[0][j] << 16) | (colors[1][j]
					    << 8) | colors[2][j];
#endif /* _LITTLE_ENDIAN */
					double_lut[3 * j] = colors[0][j];
					double_lut[3 * j + 1] = colors[1][j];
					double_lut[3 * j + 2] = colors[2][j];
				}
			} else {
/* channels == 4 */
				for (j = 0; j < entries; j++) {
#ifdef _LITTLE_ENDIAN
					normal_table[j] = colors[0][j] |
					    (colors[1][j] << 8) | (colors[2][j]
					    << 16) | (colors[3][j] << 24);
#else /* _LITTLE_ENDIAN */
					normal_table[j] = (colors[0][j] << 24) |
					    (colors[1][j] << 16) | (colors[2][j]
					    << 8) | colors[3][j];
#endif /* _LITTLE_ENDIAN */
					double_lut[4 * j] = colors[0][j];
					double_lut[4 * j + 1] = colors[1][j];
					double_lut[4 * j + 2] = colors[2][j];
					double_lut[4 * j + 3] = colors[3][j];
				}
			}
		} else {
/* intype == MLIB_SHORT */
			mlib_u16 **colors = (mlib_u16 **)table;

			if (channels == 3) {
				for (j = 0; j < entries; j++) {
#ifdef _LITTLE_ENDIAN
					normal_table[2 * j] =
					    colors[0][j] << 16;
					normal_table[2 * j + 1] =
					    colors[1][j] | (colors[2][j] << 16);
#else /* _LITTLE_ENDIAN */
					normal_table[2 * j] = colors[0][j];
					normal_table[2 * j + 1] =
					    (colors[1][j] << 16) | colors[2][j];
#endif /* _LITTLE_ENDIAN */
					double_lut[3 * j] =
					    ((mlib_s16 **)colors)[0][j];
					double_lut[3 * j + 1] =
					    ((mlib_s16 **)colors)[1][j];
					double_lut[3 * j + 2] =
					    ((mlib_s16 **)colors)[2][j];
				}
			} else {
/* channels == 4 */
				for (j = 0; j < entries; j++) {
#ifdef _LITTLE_ENDIAN
					normal_table[2 * j] =
					    colors[0][j] | (colors[1][j] << 16);
					normal_table[2 * j + 1] =
					    colors[2][j] | (colors[3][j] << 16);
#else /* _LITTLE_ENDIAN */
					normal_table[2 * j] =
					    (colors[0][j] << 16) | colors[1][j];
					normal_table[2 * j + 1] =
					    (colors[2][j] << 16) | colors[3][j];
#endif /* _LITTLE_ENDIAN */
					double_lut[4 * j] =
					    ((mlib_s16 **)colors)[0][j];
					double_lut[4 * j + 1] =
					    ((mlib_s16 **)colors)[1][j];
					double_lut[4 * j + 2] =
					    ((mlib_s16 **)colors)[2][j];
					double_lut[4 * j + 3] =
					    ((mlib_s16 **)colors)[3][j];
				}
			}
		}

	} else {
		colormap->normal_table = NULL;
		colormap->double_lut = NULL;
	}

	switch (method) {
	case LUT_COLOR_CUBE_SEARCH:
	    {
		    switch (intype) {
		    case MLIB_BYTE:
			{
				switch (channels) {
				case 3:
					transformed_table =
					    mlib_color_cube_init_U8_3(colormap);
					break;
				case 4:
					transformed_table =
					    mlib_color_cube_init_U8_4(colormap);
					break;
				default:
					return (MLIB_FAILURE);
				}

				break;
			}

		    case MLIB_SHORT:
			{
				switch (channels) {
				case 3:
					transformed_table =
					    mlib_color_cube_init_S16_3
					    (colormap);
					break;
				case 4:
					transformed_table =
					    mlib_color_cube_init_S16_4
					    (colormap);
					break;
				default:
					return (MLIB_FAILURE);
				}

				break;
			}

		    default:
			    return (MLIB_FAILURE);
		    }

		    break;
	    }

	case LUT_BINARY_TREE_SEARCH:
	    {
		    switch (channels) {
		    case 3:
			    transformed_table =
				mlib_binary_tree_init_3(colormap);
			    break;
		    case 4:
			    transformed_table =
				mlib_binary_tree_init_4(colormap);
			    break;
		    default:
			    return (MLIB_FAILURE);
		    }

		    break;
	    }
	}

	if (transformed_table == NULL && method != LUT_STUPID_SEARCH) {
/*
 * __mlib_free( ( ( mlib_colormap * )colormap )->normal_table );
 * __mlib_free( colormap );
 * return (MLIB_FAILURE);
 */

/*
 * Better to run linear search if we don't have enough memory for
 * anything faster
 */
		colormap->method = LUT_STUPID_SEARCH;
	}

	colormap->table = transformed_table;

	*p_colormap = colormap;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorTrue2IndexInit(
    void **colormap,
    mlib_s32 bits,
    mlib_type intype,
    mlib_type outtype,
    mlib_s32 channels,
    mlib_s32 entries,
    mlib_s32 offset,
    const void **table)
{
	return mlib_ImageColorTrue2IndexInit2(colormap, NULL, bits, intype,
	    outtype, channels, entries, offset, (void **)table);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorDitherInit(
    void **colormap,
    const mlib_s32 *dimensions,
    mlib_type intype,
    mlib_type outtype,
    mlib_s32 channels,
    mlib_s32 entries,
    mlib_s32 offset,
    void **table)
{
	return mlib_ImageColorTrue2IndexInit2(colormap, dimensions, 0, intype,
	    outtype, channels, entries, offset, table);
}

/* *********************************************************** */

void
__mlib_ImageColorTrue2IndexFree(
    void *colormap)
{
	mlib_colormap *s = colormap;
	void *table;
	mlib_s32 method;

	if (colormap == NULL) {
		return;
	}

	table = s->table;
	method = s->method;

	if (method != LUT_BINARY_TREE_SEARCH) {
		__mlib_free(table);
	} else {
		switch (s->channels) {
		case 3:
			__mlib_free_binary_tree_3(table);
			break;

		case 4:
			__mlib_free_binary_tree_4(table);
			break;
		}
	}

	if (s->normal_table != NULL)
		__mlib_free(s->normal_table);
	if (s->double_lut != NULL)
		__mlib_free(s->double_lut);
	__mlib_free(colormap);
}

/* *********************************************************** */

void
__mlib_ImageColorDitherFree(
    void *colormap)
{
	__mlib_ImageColorTrue2IndexFree(colormap);
}

/* *********************************************************** */

void *
mlib_color_cube_init_U8_3(
    mlib_colormap * state)
{
	mlib_s32 offset = state->offset;
	mlib_s32 bits = state->bits;
	mlib_s32 nbits = 8 - bits;
	mlib_u32 *dist_buf;
	mlib_s32 size;
	mlib_u8 *base0, *base1, *base2;
	mlib_s32 cindex;

	mlib_s32 center0, center1, center2;
	mlib_u32 dist0, dist1, cdist;
	mlib_s32 cinc0, cinc1, cinc2;
	mlib_u32 *dp0, *dp1, *cdp, *cdp1;
	mlib_s32 stride0, stride1;
	mlib_s32 x, xsqr, colormax;
	mlib_s32 xx0, xx1, xx2;
	mlib_s32 inc1 = 0, inc2 = 0, cdist1;
	mlib_s32 here1 = 0, min1 = 0, max1 = 0, prevmin1 = 0, prevmax1 = 0;
	mlib_s32 here2 = 0, min2 = 0, max2 = 0, prevmin2 = 0, prevmax2 = 0;
	mlib_s32 dmin2, dmax2;
	mlib_s32 txsqr;
	mlib_s32 c0, c1, c2;
	mlib_d64 doublefirst;
	mlib_s32 memsize;

	size = (1 << bits) * (1 << bits) * (1 << bits);
	memsize = size * state->indexsize;

	dist_buf = __mlib_malloc(size * 4);

	if (!dist_buf)
		return (NULL);

	base0 = state->lut[0];
	base1 = state->lut[1];
	base2 = state->lut[2];

	colormax = 1 << bits;
	x = 1 << nbits;
	xsqr = 1 << (2 * nbits);
	txsqr = xsqr + xsqr;
	stride0 = colormax * colormax;
	stride1 = colormax;

	switch (state->indexsize) {
	case 1:
	    {
		    mlib_u8 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_u8 *)&doublefirst)[0] = offset;
		    ((mlib_u8 *)&doublefirst)[1] = offset;
		    ((mlib_u8 *)&doublefirst)[2] = offset;
		    ((mlib_u8 *)&doublefirst)[3] = offset;
		    ((mlib_u8 *)&doublefirst)[4] = offset;
		    ((mlib_u8 *)&doublefirst)[5] = offset;
		    ((mlib_u8 *)&doublefirst)[6] = offset;
		    ((mlib_u8 *)&doublefirst)[7] = offset;

		    COLOR_CUBE_3_INIT_MAIN_LOOP(mlib_u8,
			0,
			0);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	case 2:
	    {
		    mlib_s16 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_s16 *)&doublefirst)[0] = offset;
		    ((mlib_s16 *)&doublefirst)[1] = offset;
		    ((mlib_s16 *)&doublefirst)[2] = offset;
		    ((mlib_s16 *)&doublefirst)[3] = offset;

		    COLOR_CUBE_3_INIT_MAIN_LOOP(mlib_s16,
			0,
			0);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	default:
		return (NULL);
	}
}

/* *********************************************************** */

void *
mlib_color_cube_init_U8_4(
    mlib_colormap * state)
{
	mlib_s32 offset = state->offset;
	mlib_s32 bits = state->bits;
	mlib_s32 nbits = 8 - bits;
	mlib_u32 *dist_buf;
	mlib_s32 size;
	mlib_u8 *base0, *base1, *base2, *base3;
	mlib_s32 cindex;

	mlib_s32 center0, center1, center2, center3;
	mlib_u32 dist0, dist1, dist2, cdist;
	mlib_s32 cinc0, cinc1, cinc2, cinc3;
	mlib_u32 *dp0, *dp1, *dp2, *cdp, *cdp1, *cdp2;
	mlib_s32 stride0, stride1, stride2;
	mlib_s32 x, xsqr, colormax;
	mlib_s32 xx0, xx1, xx2, xx3;
	mlib_s32 inc1 = 0, inc2 = 0, inc3 = 0, cdist1, cdist2;
	mlib_s32 here1 = 0, min1 = 0, max1 = 0, prevmin1 = 0, prevmax1 = 0;
	mlib_s32 here2 = 0, min2 = 0, max2 = 0, prevmin2 = 0, prevmax2 = 0;
	mlib_s32 here3 = 0, min3 = 0, max3 = 0, prevmin3 = 0, prevmax3 = 0;
	mlib_s32 dmin3, dmax3;
	mlib_s32 txsqr;
	mlib_s32 c0, c1, c2, c3;
	mlib_d64 doublefirst;
	mlib_s32 memsize;

	size = (1 << bits) * (1 << bits) * (1 << bits) * (1 << bits);
	memsize = size * state->indexsize;

	dist_buf = __mlib_malloc(size * 4);

	if (!dist_buf)
		return (NULL);

	base0 = state->lut[0];
	base1 = state->lut[1];
	base2 = state->lut[2];
	base3 = state->lut[3];

	colormax = 1 << bits;
	x = 1 << nbits;
	xsqr = 1 << (2 * nbits);
	txsqr = xsqr + xsqr;
	stride0 = colormax * colormax * colormax;
	stride1 = colormax * colormax;
	stride2 = colormax;

	switch (state->indexsize) {
	case 1:
	    {
		    mlib_u8 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_u8 *)&doublefirst)[0] = offset;
		    ((mlib_u8 *)&doublefirst)[1] = offset;
		    ((mlib_u8 *)&doublefirst)[2] = offset;
		    ((mlib_u8 *)&doublefirst)[3] = offset;
		    ((mlib_u8 *)&doublefirst)[4] = offset;
		    ((mlib_u8 *)&doublefirst)[5] = offset;
		    ((mlib_u8 *)&doublefirst)[6] = offset;
		    ((mlib_u8 *)&doublefirst)[7] = offset;

		    COLOR_CUBE_4_INIT_MAIN_LOOP(mlib_u8,
			0,
			0);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	case 2:
	    {
		    mlib_s16 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_s16 *)&doublefirst)[0] = offset;
		    ((mlib_s16 *)&doublefirst)[1] = offset;
		    ((mlib_s16 *)&doublefirst)[2] = offset;
		    ((mlib_s16 *)&doublefirst)[3] = offset;

		    COLOR_CUBE_4_INIT_MAIN_LOOP(mlib_s16,
			0,
			0);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	default:
		return (NULL);
	}
}

/* *********************************************************** */

void *
mlib_color_cube_init_S16_3(
    mlib_colormap * state)
{
	mlib_s32 offset = state->offset;
	mlib_s32 bits = state->bits;
	mlib_s32 nbits = 16 - bits;
	mlib_u32 *dist_buf;
	mlib_s32 size;
	mlib_s16 *base0, *base1, *base2;
	mlib_s32 cindex;

	mlib_s32 center0, center1, center2;
	mlib_u32 dist0, dist1, cdist;
	mlib_s32 cinc0, cinc1, cinc2;
	mlib_u32 *dp0, *dp1, *cdp, *cdp1;
	mlib_s32 stride0, stride1;
	mlib_s32 x, xsqr, colormax;
	mlib_s32 xx0, xx1, xx2;
	mlib_s32 inc1 = 0, inc2 = 0, cdist1;
	mlib_s32 here1 = 0, min1 = 0, max1 = 0, prevmin1 = 0, prevmax1 = 0;
	mlib_s32 here2 = 0, min2 = 0, max2 = 0, prevmin2 = 0, prevmax2 = 0;
	mlib_s32 dmin2, dmax2;
	mlib_s32 txsqr;
	mlib_s32 c0, c1, c2;
	mlib_d64 doublefirst;
	mlib_s32 memsize;

	size = (1 << bits) * (1 << bits) * (1 << bits);
	memsize = size * state->indexsize;

	dist_buf = __mlib_malloc(size * 4);

	if (!dist_buf)
		return (NULL);

	base0 = state->lut[0];
	base1 = state->lut[1];
	base2 = state->lut[2];

	colormax = 1 << bits;
	x = 1 << (nbits - 1);
	xsqr = 1 << (2 * nbits - 2);
	txsqr = xsqr + xsqr;
	stride0 = colormax * colormax;
	stride1 = colormax;

	switch (state->indexsize) {
	case 1:
	    {
		    mlib_u8 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_u8 *)&doublefirst)[0] = offset;
		    ((mlib_u8 *)&doublefirst)[1] = offset;
		    ((mlib_u8 *)&doublefirst)[2] = offset;
		    ((mlib_u8 *)&doublefirst)[3] = offset;
		    ((mlib_u8 *)&doublefirst)[4] = offset;
		    ((mlib_u8 *)&doublefirst)[5] = offset;
		    ((mlib_u8 *)&doublefirst)[6] = offset;
		    ((mlib_u8 *)&doublefirst)[7] = offset;

		    COLOR_CUBE_3_INIT_MAIN_LOOP(mlib_u8,
			MLIB_S16_MIN,
			1);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	case 2:
	    {
		    mlib_s16 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_s16 *)&doublefirst)[0] = offset;
		    ((mlib_s16 *)&doublefirst)[1] = offset;
		    ((mlib_s16 *)&doublefirst)[2] = offset;
		    ((mlib_s16 *)&doublefirst)[3] = offset;

		    COLOR_CUBE_3_INIT_MAIN_LOOP(mlib_s16,
			MLIB_S16_MIN,
			1);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	default:
		return (NULL);
	}
}

/* *********************************************************** */

void *
mlib_color_cube_init_S16_4(
    mlib_colormap * state)
{
	mlib_s32 offset = state->offset;
	mlib_s32 bits = state->bits;
	mlib_s32 nbits = 16 - bits;
	mlib_u32 *dist_buf;
	mlib_s32 size;
	mlib_s16 *base0, *base1, *base2, *base3;
	mlib_s32 cindex;

	mlib_s32 center0, center1, center2, center3;
	mlib_u32 dist0, dist1, dist2, cdist;
	mlib_s32 cinc0, cinc1, cinc2, cinc3;
	mlib_u32 *dp0, *dp1, *dp2, *cdp, *cdp1, *cdp2;
	mlib_s32 stride0, stride1, stride2;
	mlib_s32 x, xsqr, colormax;
	mlib_s32 xx0, xx1, xx2, xx3;
	mlib_s32 inc1 = 0, inc2 = 0, inc3 = 0, cdist1, cdist2;
	mlib_s32 here1 = 0, min1 = 0, max1 = 0, prevmin1 = 0, prevmax1 = 0;
	mlib_s32 here2 = 0, min2 = 0, max2 = 0, prevmin2 = 0, prevmax2 = 0;
	mlib_s32 here3 = 0, min3 = 0, max3 = 0, prevmin3 = 0, prevmax3 = 0;
	mlib_s32 dmin3, dmax3;
	mlib_s32 txsqr;
	mlib_s32 c0, c1, c2, c3;
	mlib_d64 doublefirst;
	mlib_s32 memsize;

	size = (1 << bits) * (1 << bits) * (1 << bits) * (1 << bits);
	memsize = size * state->indexsize;

	dist_buf = __mlib_malloc(size * 4);

	if (!dist_buf)
		return (NULL);

	base0 = state->lut[0];
	base1 = state->lut[1];
	base2 = state->lut[2];
	base3 = state->lut[3];

	colormax = 1 << bits;
	x = 1 << (nbits - 1);
	xsqr = 1 << (2 * nbits - 2);
	txsqr = xsqr + xsqr;
	stride0 = colormax * colormax * colormax;
	stride1 = colormax * colormax;
	stride2 = colormax;

	switch (state->indexsize) {
	case 1:
	    {
		    mlib_u8 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_u8 *)&doublefirst)[0] = offset;
		    ((mlib_u8 *)&doublefirst)[1] = offset;
		    ((mlib_u8 *)&doublefirst)[2] = offset;
		    ((mlib_u8 *)&doublefirst)[3] = offset;
		    ((mlib_u8 *)&doublefirst)[4] = offset;
		    ((mlib_u8 *)&doublefirst)[5] = offset;
		    ((mlib_u8 *)&doublefirst)[6] = offset;
		    ((mlib_u8 *)&doublefirst)[7] = offset;

		    COLOR_CUBE_4_INIT_MAIN_LOOP(mlib_u8,
			MLIB_S16_MIN,
			1);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	case 2:
	    {
		    mlib_s16 *table;

		    table = __mlib_malloc(memsize);

		    if (!table) {
			    __mlib_free(dist_buf);
			    return (NULL);
		    }

		    ((mlib_s16 *)&doublefirst)[0] = offset;
		    ((mlib_s16 *)&doublefirst)[1] = offset;
		    ((mlib_s16 *)&doublefirst)[2] = offset;
		    ((mlib_s16 *)&doublefirst)[3] = offset;

		    COLOR_CUBE_4_INIT_MAIN_LOOP(mlib_s16,
			MLIB_S16_MIN,
			1);

		    __mlib_free(dist_buf);
		    return (table);
	    }

	default:
		return (NULL);
	}
}

/* *********************************************************** */

void *
mlib_binary_tree_init_3(
    mlib_colormap * state)
{
	mlib_s32 bits = state->bits;
	struct lut_node_3 *table, *node;
	mlib_s32 i;

	switch (state->intype) {
	case MLIB_BYTE:
	    {
		    BINARY_TREE_INIT_3(mlib_u8,
			8,
			0,
			0);

		    break;
	    }

	case MLIB_SHORT:
	    {
		    BINARY_TREE_INIT_3(mlib_s16,
			16,
			MLIB_S16_MIN,
			1);

		    break;
	    }
	default:
	    return (NULL);
	}

	return (table);
}

/* *********************************************************** */

void *
mlib_binary_tree_init_4(
    mlib_colormap * state)
{
	mlib_s32 bits = state->bits;
	struct lut_node_4 *table, *node;
	mlib_s32 i;

	switch (state->intype) {
	case MLIB_BYTE:
		BINARY_TREE_INIT_4(mlib_u8,
		    8,
		    0,
		    0);

		break;
	case MLIB_SHORT:
		BINARY_TREE_INIT_4(mlib_s16,
		    16,
		    MLIB_S16_MIN,
		    1);

		break;
	default:
		return (NULL);
	}

	return (table);
}

/* *********************************************************** */

void
__mlib_free_binary_tree_3(
    void *table)
{
	struct lut_node_3 *node;

	node = table;

	if (node) {
		mlib_s32 j;
		mlib_s32 mask = 1;

		for (j = 0; j < 8; j++) {

			if (!(node->tag & mask) && node->contents.quadrants[j])
				__mlib_free_binary_tree_3(node->contents.
				    quadrants[j]);
			mask <<= 1;
		}

		__mlib_free(node);
	}
}

/* *********************************************************** */

void
__mlib_free_binary_tree_4(
    void *table)
{
	struct lut_node_4 *node;

	node = table;

	if (node) {
		mlib_s32 j;
		mlib_s32 mask = 1;

		for (j = 0; j < 16; j++) {

			if (!(node->tag & mask) && node->contents.quadrants[j])
				__mlib_free_binary_tree_4(node->contents.
				    quadrants[j]);
			mask <<= 1;
		}

		__mlib_free(node);
	}
}

/* *********************************************************** */
