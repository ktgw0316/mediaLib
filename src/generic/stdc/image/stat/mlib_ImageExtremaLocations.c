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

#pragma ident	"@(#)mlib_ImageExtremaLocations.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtremaLocations     - finds the image-wise minimum and
 *                                       maximum pixel values for each channel,
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageExtremaLocations(mlib_s32         *min,
 *                                             mlib_s32         *max,
 *                                             const mlib_image *img,
 *                                             mlib_s32         xStart,
 *                                             mlib_s32         yStart,
 *                                             mlib_s32         xPeriod,
 *                                             mlib_s32         yPeriod,
 *                                             mlib_s32         saveLocations,
 *                                             mlib_s32         maxRuns,
 *                                             mlib_s32         *minCounts,
 *                                             mlib_s32         *maxCounts,
 *                                             mlib_s32         **minLocations,
 *                                             mlib_s32         **maxLocations,
 *                                             mlib_s32         len);
 *
 * ARGUMENT
 *      min            The pointer to the minimum values.
 *      max            The pointer to the maximum values.
 *      img            The pointer to the input image.
 *      xStart         The initial X sample coordinate.
 *      yStart         The initial Y sample coordinate.
 *      xPeriod        The X sampling rate, xPeriod >= 1.
 *      yPeriod        The Y sampling rate, yPeriod >= 1.
 *      saveLocations  If true (i.e., != 0), find the extrema locations;
 *                     otherwise only find the extrema.
 *      maxRuns        The number of runs of the minimum/maximum the caller
 *                     expects for each channel.  maxRuns >= 1.
 *                     If it is MLIB_S32_MAX, all the minimum/maximum locations
 *                     should be recorded.
 *      minCounts      The pointer to the numbers of runs of the minimum
 *                     recorded in minLocations.
 *      maxCounts      The pointer to the numbers of runs of the maximum
 *                     recorded in maxLocations.
 *      minLocations   The pointer to the minimum locations in a format of
 *                     run-length coding.
 *      maxLocations   The pointer to the maximum locations in a format of
 *                     run-length coding.
 *      len            The length of the buffers for the minimum/maximum
 *                     locations in each channel.
 *
 * DESCRIPTION
 *
 *      The mlib_ImageExtremaLocations[_Fp] function scans an image, finds the
 *      minimum and maximum pixel values for each channel, and finds the
 *      locations of those pixels with the minimum or maximum values.
 *
 *      The user provides initial minimum/maximum values through the arguments
 *      min and max.  This function will update them based on findings.
 *
 *      The set of pixels scanned may furthermore be reduced by specifying
 *      "xPeriod" and "yPeriod" parameters that specify the sampling rate
 *      along each axis.
 *
 *      The set of pixels to be scanned may be obtained by intersecting
 *      the grid (xStart + i*xPeriod, yStart + j*yPeriod); i, j >= 0 with
 *      the bounding rectangle of the image.
 *
 *      The locations of the minimum/maximum, if asked, are recorded in a
 *      format of run-length coding.  Each run-length code, or simply called
 *      a run, has a format of (xStart, yStart, length).  Here length is
 *      defined on the low-resolution image (with downsampling factors of
 *      1/xPeriod, 1/yPeriod) and does not cross rows.  So the run-length code
 *      (xStart, yStart, length) means that the pixels at (xStart, yStart),
 *      (xStart + xPeriod, yStart), ..., (xStart + (length - 1)*xPeriod, yStart)
 *      of the original image have a value of the minimum/maximum.
 *
 *      The buffers for minLocations and maxLocations are organized in the
 *      following format for each channel c:
 *
 *          minLocations[c][0] = xStart0;       // the 1st run
 *          minLocations[c][1] = yStart0;
 *          minLocations[c][2] = length0;
 *          minLocations[c][3] = xStart1;       // the 2nd run
 *          minLocations[c][4] = yStart1;
 *          minLocations[c][5] = length1;
 *              ......                          // more runs
 *          minLocations[c][len-1] = ...;
 *
 *      It is the user's responsibility of allocating enough memory for the
 *      buffers for minLocations and maxLocations.  This function should
 *      return MLIB_OUTOFRANGE, if any of the buffers is not big enough.
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.  The arguments min, max,
 *      minCounts, maxCounts, minLocations, maxLocations must support at
 *      least img->channels channels.
 *      For mlib_ImageExtremaLocations, the image can be of MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <stdlib.h>
#include <mlib_ImageExtremaLocations.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExtremaLocations = __mlib_ImageExtremaLocations

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageExtremaLocations) mlib_ImageExtremaLocations
    __attribute__((weak, alias("__mlib_ImageExtremaLocations")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_SIZE	1024

/* *********************************************************** */

#define	COLLECT_MINMAX                                          \
	{                                                       \
	    i_max = -1;                                         \
	    i_min = -1;                                         \
	    for (i = xStart; i < xsize; i += xPeriod) {         \
		if (sp[0] == tmax) {                            \
		    i_max++;                                    \
		    buf1[i_max] = i;                            \
		}                                               \
		if (sp[0] == tmin) {                            \
		    i_min++;                                    \
		    buf0[i_min] = i;                            \
		}                                               \
		sp += sPeriod;                                  \
	    }                                                   \
	}

/* *********************************************************** */

#define	COLLECT_1(tmin)                                         \
	{                                                       \
	    i_min = -1;                                         \
	    for (i = xStart; i < xsize; i += xPeriod) {         \
		if (sp[0] == tmin) {                            \
		    i_min++;                                    \
		    buf0[i_min] = i;                            \
		}                                               \
		sp += sPeriod;                                  \
	    }                                                   \
	}

/* *********************************************************** */

#define	RUN_CODING(i_bound, buf, pLocations, pCounts, test_break)   \
	{                                                           \
	    mlib_s32 xStart0, len0, counts;                         \
	    mlib_s32 coord, tcoord;                                 \
	                                                            \
	    if (i_bound >= 0) {                                     \
		if (maxRuns < 1)                                    \
		    return (MLIB_OUTOFRANGE);                       \
		tcoord = xStart0 = buf[0];                          \
		len0 = 1;                                           \
		counts = *pCounts;                                  \
		if (counts >= maxRuns) {                            \
		    test_break = 1;                                 \
		} else {                                            \
		    for (i = 1; i <= i_bound; i++) {                \
			coord = buf[i];                             \
			if ((coord - tcoord) != xPeriod) {          \
			    counts++;                               \
			    if (counts >= maxRuns) {                \
				counts--;                           \
				test_break = 1;                     \
				break;                              \
			    }                                       \
			    pLocations[0] = xStart0;                \
			    pLocations[1] = yStart0;                \
			    pLocations[2] = len0;                   \
			    xStart0 = coord;                        \
			    len0 = 0;                               \
			    pLocations += 3;                        \
			}                                           \
			len0++;                                     \
			tcoord = coord;                             \
		    }                                               \
		    counts++;                                       \
		    pLocations[0] = xStart0;                        \
		    pLocations[1] = yStart0;                        \
		    pLocations[2] = len0;                           \
		    pLocations += 3;                                \
		    *pCounts = counts;                              \
		}                                                   \
		if ((test_break == 1) && (small_buffer == 1)) {     \
		    if (pbuf != NULL)                               \
			__mlib_free(pbuf);                          \
		    return (MLIB_OUTOFRANGE);                       \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMALOCATIONS(STYPE)                                \
	mlib_status                                                       \
	mlib_ImageExtremaLocations_##STYPE(mlib_s32 *min,                 \
	    mlib_s32 *max, const mlib_image *img, mlib_s32 xStart,        \
	    mlib_s32 yStart, mlib_s32 xPeriod, mlib_s32 yPeriod,          \
	    mlib_s32 maxRuns, mlib_s32 *minCounts, mlib_s32 *maxCounts,   \
	    mlib_s32 **minLocations, mlib_s32 **maxLocations,             \
	    mlib_s32 *buf_test_min, mlib_s32 *buf_test_max,               \
	    mlib_s32 len)                                                 \
	{                                                                 \
	    mlib_##STYPE *sl =                                            \
		(mlib_##STYPE *) mlib_ImageGetData(img);                  \
	    mlib_s32 *pbuf = NULL, *buf0, *buf1;                          \
	    mlib_s32 buf_0[BUF_SIZE];                                     \
	    mlib_s32 buf_1[BUF_SIZE];                                     \
	    mlib_s32 nchan = mlib_ImageGetChannels(img);                  \
	    mlib_s32 xsize = mlib_ImageGetWidth(img);                     \
	    mlib_s32 ysize = mlib_ImageGetHeight(img);                    \
	    mlib_s32 slb = mlib_ImageGetStride(img);                      \
	    mlib_s32 small_buffer, sPeriod;                               \
	    mlib_s32 i, j, c, buf_len;                                    \
	                                                                  \
	    while (xStart < 0)                                            \
		xStart += xPeriod;                                        \
	    while (yStart < 0)                                            \
		yStart += yPeriod;                                        \
	    buf0 = buf_0;                                                 \
	    buf1 = buf_1;                                                 \
	    buf_len = ((xsize - xStart) / xPeriod) + 1;                   \
	    if (buf_len > BUF_SIZE) {                                     \
		pbuf =                                                    \
		    (mlib_s32 *)__mlib_malloc(2 * buf_len *               \
		    sizeof (mlib_s32));                                   \
		if (pbuf == NULL)                                         \
		    return (MLIB_FAILURE);                                \
		buf0 = pbuf;                                              \
		buf1 = pbuf + buf_len;                                    \
	    }                                                             \
	    slb = slb / (sizeof (mlib_##STYPE));                          \
	    sl = sl + yStart * slb + xStart * nchan;                      \
	    slb = slb * yPeriod;                                          \
	    sPeriod = nchan * xPeriod;                                    \
	    len = len / 3;                                                \
	    small_buffer = 0;                                             \
	    if (maxRuns > len) {                                          \
		small_buffer = 1;                                         \
		maxRuns = len;                                            \
	    }                                                             \
	    for (c = 0; c < nchan; c++) {                                 \
		mlib_##STYPE *ssl, *sp;                                   \
		mlib_s32 *pmaxCounts, *pminCounts;                        \
		mlib_s32 *pmaxLocations, *pminLocations;                  \
		mlib_s32 tmax = max[c];                                   \
		mlib_s32 tmin = min[c];                                   \
		mlib_s32 yStart0, i_max, i_min;                           \
		mlib_s32 test_break_min = buf_test_min[c];                \
		mlib_s32 test_break_max = buf_test_max[c];                \
	                                                                  \
		pmaxCounts = maxCounts;                                   \
		pmaxLocations = maxLocations[c];                          \
		*pmaxCounts = 0;                                          \
		maxCounts++;                                              \
		pminCounts = minCounts;                                   \
		pminLocations = minLocations[c];                          \
		*pminCounts = 0;                                          \
		minCounts++;                                              \
		ssl = sl + c;                                             \
		for (j = yStart; j < ysize; j += yPeriod) {               \
		    yStart0 = j;                                          \
		    sp = ssl;                                             \
		    if (test_break_min == 0) {                            \
			if (test_break_max == 0) {                        \
			COLLECT_MINMAX RUN_CODING(i_min, buf0,            \
				pminLocations, pminCounts,                \
				test_break_min) RUN_CODING(i_max,         \
				buf1, pmaxLocations, pmaxCounts,          \
				test_break_max)} else                     \
			{                                                 \
			COLLECT_1(tmin) RUN_CODING(i_min, buf0,           \
				pminLocations, pminCounts,                \
				test_break_min)}                          \
		    } else if (test_break_max == 0) {                     \
		    COLLECT_1(tmax) RUN_CODING(i_min, buf0,               \
			    pmaxLocations, pmaxCounts,                    \
			    test_break_max)} else                         \
			break;                                            \
		    ssl += slb;                                           \
		}                                                         \
	    }                                                             \
	    if (pbuf != NULL)                                             \
		__mlib_free(pbuf);                                        \
	    return (MLIB_SUCCESS);                                        \
	}

/* *********************************************************** */

FUNC_IMAGE_EXTREMALOCATIONS(u8)
    FUNC_IMAGE_EXTREMALOCATIONS(u16)
    FUNC_IMAGE_EXTREMALOCATIONS(s16)
    FUNC_IMAGE_EXTREMALOCATIONS(s32)

/* *********************************************************** */
    mlib_status __mlib_ImageExtremaLocations(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 saveLocations,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 len)
{
	mlib_status stat;
	mlib_s32 nchan = mlib_ImageGetChannels(img);
	mlib_s32 dmin[4], dmax[4];
	mlib_s32 buf_test_min[4];
	mlib_s32 buf_test_max[4];
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i = 0; i < nchan; i++) {
		dmin[i] = min[i];
		dmax[i] = max[i];
	}

	stat =
	    __mlib_ImageExtrema2(dmin, dmax, img, xStart, yStart, xPeriod,
	    yPeriod);

	if (stat != MLIB_SUCCESS)
		return (stat);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i = 0; i < nchan; i++) {
		min[i] = (min[i] < dmin[i]) ? min[i] : dmin[i];
		max[i] = (max[i] > dmax[i]) ? max[i] : dmax[i];
	}

	if (saveLocations == 0)
		return (MLIB_SUCCESS);

	if (maxRuns < 1)
		return (MLIB_OUTOFRANGE);

	if ((minCounts == NULL) || (maxCounts == NULL))
		return (MLIB_FAILURE);

	if ((minLocations == NULL) || (maxLocations == NULL))
		return (MLIB_FAILURE);

	for (i = 0; i < nchan; i++) {
		if ((minLocations[i] == NULL) || (maxLocations[i] == NULL))
			return (MLIB_FAILURE);
	}

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i = 0; i < nchan; i++) {
		buf_test_min[i] = 0;
		buf_test_max[i] = 0;

		if (dmin[i] != min[i])
			buf_test_min[i] = 1;
		if (dmax[i] != max[i])
			buf_test_max[i] = 1;
	}

	switch (mlib_ImageGetType(img)) {

	case MLIB_BYTE:
		stat =
		    mlib_ImageExtremaLocations_u8(min, max, img, xStart, yStart,
		    xPeriod, yPeriod, maxRuns, minCounts, maxCounts,
		    minLocations, maxLocations, buf_test_min, buf_test_max,
		    len);
		break;

	case MLIB_USHORT:
		stat =
		    mlib_ImageExtremaLocations_u16(min, max, img, xStart,
		    yStart, xPeriod, yPeriod, maxRuns, minCounts, maxCounts,
		    minLocations, maxLocations, buf_test_min, buf_test_max,
		    len);
		break;

	case MLIB_SHORT:
		stat =
		    mlib_ImageExtremaLocations_s16(min, max, img, xStart,
		    yStart, xPeriod, yPeriod, maxRuns, minCounts, maxCounts,
		    minLocations, maxLocations, buf_test_min, buf_test_max,
		    len);
		break;

	case MLIB_INT:
		stat =
		    mlib_ImageExtremaLocations_s32(min, max, img, xStart,
		    yStart, xPeriod, yPeriod, maxRuns, minCounts, maxCounts,
		    minLocations, maxLocations, buf_test_min, buf_test_max,
		    len);
		break;

	default:
		stat = MLIB_FAILURE;
	}

	return (stat);
}

/* *********************************************************** */
