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

#pragma ident	"@(#)mlib_c_ImageZoomIn2X_NN.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2X -  image 2X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2X(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *              MLIB_NEAREST
 *              MLIB_BILINEAR
 *              MLIB_BICUBIC
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_OP_NEAREST
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                              \
	MLIB_TYPE	*pdst,                                        \
	const MLIB_TYPE	*psrc,                                        \
	mlib_s32	src_width,                                    \
	mlib_s32	src_height,                                   \
	mlib_s32	dst_stride,                                   \
	mlib_s32	src_stride,                                   \
	mlib_s32	src_w_beg,                                    \
	mlib_s32	src_h_beg,                                    \
	mlib_s32	channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                             \
	(MLIB_TYPE *)pdst, (MLIB_TYPE *)psrc,                         \
	src_width,  src_height, dst_stride,  src_stride,              \
	src_w_beg, src_h_beg, channels

/* *********************************************************** */

static void mlib_c_ImageZoomIn2X_U8_NNA(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));

static void mlib_c_ImageZoomIn2X_U8_NNG(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));

static void mlib_c_ImageZoomIn2X_S16_NNA(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));

/* *********************************************************** */

void
mlib_c_ImageZoomIn2X_U8_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i = 0, j, k;

	if ((((mlib_s32)psrc | (mlib_s32)pdst | src_stride | dst_stride |
	    channels) & 1) == 0) {
		dst_stride /= 2;
		src_stride /= 2;
		channels /= 2;
		mlib_c_ImageZoomIn2X_S16_NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));

		return;
	}

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

	if ((((mlib_s32)pdst | dst_stride) & 3) == 0)
		mlib_c_ImageZoomIn2X_U8_NNA(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));

	else
		mlib_c_ImageZoomIn2X_U8_NNG(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));

	i = src_height / 2;
	j = src_width / 2;

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

void
mlib_c_ImageZoomIn2X_U8_NNA(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j;

	switch (channels) {
	case 1: {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					psrc[i * src_stride +
					j] | (psrc[i * src_stride + j] << 8);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(psrc[i * src_stride +
					j] << 8) | psrc[i * src_stride + j];
#endif /* _LITTLE_ENDIAN */
				    ((mlib_u16 *)pdst)[2 * i *
					(dst_stride >> 1) + j] =
					((mlib_u16 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + j] = a0;
			    }
		    }
	    }

		break;
	case 2: {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					psrc[i * src_stride +
					2 * j] | (psrc[i * src_stride + 2 * j +
					1] << 8);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(psrc[i * src_stride +
					2 * j] << 8) | psrc[i * src_stride +
					2 * j + 1];
#endif /* _LITTLE_ENDIAN */
				    a0 |= a0 << 16;
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 2) + j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 2) + j] = a0;
			    }
		    }
	    }

		break;
	case 3: {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a1 =
					psrc[i * src_stride + 3 * j] << 8;
				    mlib_s32 a0 =
					psrc[i * src_stride + 3 * j + 1] << 8;
				    mlib_s32 a2 =
					psrc[i * src_stride + 3 * j + 2] << 8;
				    a0 |= psrc[i * src_stride + 3 * j];
				    a2 |= psrc[i * src_stride + 3 * j + 1];
				    a1 |= psrc[i * src_stride + 3 * j + 2];
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a1 = psrc[i * src_stride + 3 * j];
				    mlib_s32 a0 =
					psrc[i * src_stride + 3 * j + 1];
				    mlib_s32 a2 =
					psrc[i * src_stride + 3 * j + 2];
				    a0 |= (psrc[i * src_stride + 3 * j] << 8);
				    a2 |=
					(psrc[i * src_stride + 3 * j + 1] << 8);
				    a1 |=
					(psrc[i * src_stride + 3 * j + 2] << 8);
#endif /* _LITTLE_ENDIAN */
				    ((mlib_u16 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j] =
					((mlib_u16 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j] = a0;
				    ((mlib_u16 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j + 2] =
					((mlib_u16 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j + 2] =
					a2;
				    ((mlib_u16 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j + 1] =
					((mlib_u16 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j + 1] =
					a1;
			    }
		    }
	    }

		break;
	case 4: {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					psrc[i * src_stride + 4 * j] |
					(psrc[i * src_stride + 4 * j +
					1] << 8) | (psrc[i * src_stride +
					4 * j +
					2] << 16) | (psrc[i * src_stride +
					4 * j + 3] << 24);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(psrc[i * src_stride + 4 * j] << 24) |
					(psrc[i * src_stride + 4 * j +
					1] << 16) | (psrc[i * src_stride +
					4 * j + 2] << 8) | psrc[i * src_stride +
					4 * j + 3];
#endif /* _LITTLE_ENDIAN */
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 2) + 2 * j] =
					((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 2) + 2 * j + 1] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 2) + 2 * j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 2) + 2 * j + 1] =
					a0;
			    }
		    }

		    break;
	    }
	}
}

/* *********************************************************** */

void
mlib_c_ImageZoomIn2X_U8_NNG(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k;

	for (i = 0; i < (src_height / 2); i++) {
		for (k = 0; k < channels; k++) {
			mlib_s32 l = k + channels;

			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * channels * j +
				    k] =
				    pdst[2 * i * dst_stride + 2 * channels * j +
				    l] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + l] =
				    psrc[i * src_stride + j * channels + k];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageZoomIn2X_S16_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
/* indices */
	mlib_s32 i = 0, j, k;

	if ((((((mlib_s32)psrc | (mlib_s32)pdst) >> 1) | src_stride | dst_stride
	    | channels) & 1) == 0) {
		dst_stride /= 2;
		src_stride /= 2;
		channels /= 2;
		mlib_ImageZoomIn2X_S32_NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s32));

		return;
	}

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

	if ((((mlib_s32)pdst & 3) | (dst_stride & 1)) == 0) {
		mlib_c_ImageZoomIn2X_S16_NNA(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));

		i = src_height / 2;
		j = src_width / 2;
	} else {
		for (i = 0; i < (src_height / 2); i++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 l = k + channels;

				for (j = 0; j < (src_width / 2); j++) {
					pdst[2 * i * dst_stride +
					    2 * channels * j + k] =
					    pdst[2 * i * dst_stride +
					    2 * channels * j + l] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + k] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + l] =
					    psrc[i * src_stride + j * channels +
					    k];
				}
			}
		}
	}

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

void
mlib_c_ImageZoomIn2X_S16_NNA(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
/* indices */
	mlib_s32 i, j;

	switch (channels) {
	case 1: {
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					((mlib_u16 *)psrc)[i * src_stride +
					j] | (((mlib_u16 *)psrc)[i *
					src_stride + j] << 16);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(((mlib_u16 *)psrc)[i * src_stride +
					j] << 16) | ((mlib_u16 *)psrc)[i *
					src_stride + j];
#endif /* _LITTLE_ENDIAN */
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + j] = a0;
			    }
		    }
	    }

		break;
	case 2: {
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					((mlib_u16 *)psrc)[i * src_stride +
					2 * j] | (((mlib_u16 *)psrc)[i *
					src_stride + 2 * j + 1] << 16);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(((mlib_u16 *)psrc)[i * src_stride +
					2 * j] << 16) | ((mlib_u16 *)psrc)[i *
					src_stride + 2 * j + 1];
#endif /* _LITTLE_ENDIAN */
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 2 * j] =
					((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 2 * j + 1] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 2 * j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 2 * j + 1] =
					a0;
			    }
		    }
	    }

		break;
	case 3: {
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a1 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j] << 16;
				    mlib_s32 a0 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 1] << 16;
				    mlib_s32 a2 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 2] << 16;
				    a0 |=
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j];
				    a2 |=
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 1];
				    a1 |=
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 2];
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a1 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j];
				    mlib_s32 a0 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 1];
				    mlib_s32 a2 =
					((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 2];
				    a0 |=
					(((mlib_u16 *)psrc)[i * src_stride +
					3 * j] << 16);
				    a2 |=
					(((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 1] << 16);
				    a1 |=
					(((mlib_u16 *)psrc)[i * src_stride +
					3 * j + 2] << 16);
#endif /* _LITTLE_ENDIAN */
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j] = a0;
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j + 2] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j + 2] =
					a2;
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 3 * j + 1] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 3 * j + 1] =
					a1;
			    }
		    }
	    }

		break;
	case 4: {
		    for (i = 0; i < (src_height / 2); i++) {
			    for (j = 0; j < (src_width / 2); j++) {
#ifdef _LITTLE_ENDIAN
				    mlib_s32 a0 =
					((mlib_u16 *)psrc)[i * src_stride +
					4 * j] | (((mlib_u16 *)psrc)[i *
					src_stride + 4 * j + 1] << 16), a1 =
					((mlib_u16 *)psrc)[i * src_stride +
					4 * j +
					2] | (((mlib_u16 *)psrc)[i *
					src_stride + 4 * j + 3] << 16);
#else /* _LITTLE_ENDIAN */
				    mlib_s32 a0 =
					(((mlib_u16 *)psrc)[i * src_stride +
					4 * j] << 16) | ((mlib_u16 *)psrc)[i *
					src_stride + 4 * j + 1], a1 =
					(((mlib_u16 *)psrc)[i * src_stride +
					4 * j +
					2] << 16) | ((mlib_u16 *)psrc)[i *
					src_stride + 4 * j + 3];
#endif /* _LITTLE_ENDIAN */
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 4 * j] =
					((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 4 * j + 2] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 4 * j] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 4 * j + 2] =
					a0;
				    ((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 4 * j + 1] =
					((mlib_s32 *)pdst)[2 * i *
					(dst_stride >> 1) + 4 * j + 3] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 4 * j + 1] =
					((mlib_s32 *)pdst)[(2 * i +
					1) * (dst_stride >> 1) + 4 * j + 3] =
					a1;
			    }
		    }
	    }

		break;
	}
}

/* *********************************************************** */
