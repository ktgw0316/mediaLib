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

#pragma ident	"@(#)mlib_i_ImageChannelCopy_f.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_c_ImageChannelCopy_u8
 *  mlib_c_ImageChannelCopy_s16
 *  mlib_c_ImageChannelCopy_s32
 *  mlib_c_ImageChannelCopy_d64
 *  mlib_c_ImageChannelCopy_u8_3
 *  mlib_c_ImageChannelCopy_u8_24
 *  mlib_c_ImageChannelCopy_s16_2
 *  mlib_c_ImageChannelCopy_s16_3
 *  mlib_c_ImageChannelCopy_s16_4
 *
 * DESCRIPTION
 *      Copy the selected channels of the source image into the
 *      destination image.
 *
 * NOTE
 *      These functions are separated from mlib_c_ImageChannelCopy.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_c_ImageChannelCopy.h>

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_u8(
    const mlib_u8 *src,
    mlib_s32 src_stride,
    mlib_u8 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, x, y;
	mlib_u8 *srcRow, *dstRow, *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4], cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (void *)src;
		dstRow = dst;
		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (srcRow + cc);
			pDst = (dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_s16(
    const mlib_s16 *src,
    mlib_s32 src_stride,
    mlib_s16 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_s16 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 2;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_s16 *)(srcRow + cc);
			pDst = (mlib_s16 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_s32(
    const mlib_s32 *src,
    mlib_s32 src_stride,
    mlib_s32 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_s32 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 4;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_s32 *)(srcRow + cc);
			pDst = (mlib_s32 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_d64(
    const mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_d64 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_d64 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 8;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_d64 *)(srcRow + cc);
			pDst = (mlib_d64 *)(dstRow + cc);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < src_width; x++) {
#ifdef _NO_LONGLONG
				*pDst = *pSrc;
#else
				*(mlib_s64 *)pDst = *(mlib_s64 *)pSrc;
#endif
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	RECALC_MASK_U8_24(cc)                                   \
	if (cc) {                                               \
	    mask = fm;                                          \
	    fm >>= (cc << 3);                                   \
	    mask <<= (32 - (cc << 3));                          \
	    fm |= mask;                                         \
	    mask = fm >> (nchannels << 3);                      \
	    fm |= mask;                                         \
	}

#else /* _LITTLE_ENDIAN */

#define	RECALC_MASK_U8_24(cc)                                   \
	if (cc) {                                               \
	    mask = fm;                                          \
	    fm <<= (cc << 3);                                   \
	    mask >>= (32 - (cc << 3));                          \
	    fm |= mask;                                         \
	    mask = fm << (nchannels << 3);                      \
	    fm |= mask;                                         \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	RECALC_MASK_U8_3(cc)                                    \
	{                                                       \
	    if (cc == 1) {                                      \
		mask = fm1;                                     \
		fm1 = fm2;                                      \
		fm2 = fm3;                                      \
		fm3 = mask;                                     \
	    }                                                   \
	    if (cc == 2) {                                      \
		mask = fm2;                                     \
		fm2 = fm1;                                      \
		fm1 = fm3;                                      \
		fm3 = mask;                                     \
	    }                                                   \
	}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_u8_24(
    const mlib_u8 *src,
    mlib_s32 src_stride,
    mlib_u8 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, j, cc;
	mlib_s32 src_width_nchannels = src_width * nchannels;
	mlib_u32 mask = cmask;
	mlib_u32 cfm, fm = 0;
	mlib_u32 shr, shl;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_u32 *psrc, *pdst;
	mlib_u32 s0, s1, d0, d1, z0, z1;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
	mlib_u64 *psrc, *pdst;
	mlib_u64 s0, s1, d0;
	mlib_u64 u64_fm;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

	if (src_width < 8) {
		mlib_c_ImageChannelCopy_u8(src, src_stride, dst, dst_stride,
		    nchannels, src_width, src_height, cmask);
		return;
	}

	if ((nchannels == 4) && ((cmask == 1) || (cmask == 2) || (cmask == 4) ||
	    (cmask == 8))) {
		mlib_c_ImageChannelCopy_u8(src, src_stride, dst, dst_stride,
		    nchannels, src_width, src_height, cmask);
		return;
	}

	for (i = 0; i < nchannels; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm |= 0xff000000 >> (i << 3);
#else /* _LITTLE_ENDIAN */
			fm |= 0xff << (i << 3);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

#ifdef _LITTLE_ENDIAN
	mask = fm >> (nchannels << 3);
#else /* _LITTLE_ENDIAN */
	mask = fm << (nchannels << 3);
#endif /* _LITTLE_ENDIAN */
	cfm = fm |= mask;

	for (i = 0; i < src_height; i++) {

		mlib_u8 *psrc_row = (void *)(src + i * src_stride);
		mlib_u8 *pdst_row = dst + i * dst_stride;

		fm = cfm;

		if ((!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7))) {

			for (j = 0, cc = 0, mask = fm;
			    j < (mlib_s32)((8 - (mlib_addr)psrc_row) & 7);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (mask & 0xff) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask >>= 8;
#else /* _LITTLE_ENDIAN */

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
#endif /* _LITTLE_ENDIAN */
				cc++;

				if (cc == 4) {
					mask = fm;
					cc = 0;
				}
			}

			RECALC_MASK_U8_24(cc);
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			psrc = ((mlib_u32 *)(psrc_row + j));
			pdst = ((mlib_u32 *)(pdst_row + j));

			s0 = psrc[0];
			d0 = pdst[0];
			s1 = psrc[1];
			d1 = pdst[1];
			z0 = (s0 & fm) | (d0 & ~fm);
			z1 = (s1 & fm) | (d1 & ~fm);

			s0 = psrc[2];
			d0 = pdst[2];
			s1 = psrc[3];
			d1 = pdst[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 24); j += 8) {
				pdst[0] = z0;
				pdst[1] = z1;
				z0 = (s0 & fm) | (d0 & ~fm);
				z1 = (s1 & fm) | (d1 & ~fm);
				s0 = psrc[4];
				d0 = pdst[4];
				s1 = psrc[5];
				d1 = pdst[5];
				psrc += 2;
				pdst += 2;
			}

			if (j <= (src_width_nchannels - 16)) {
				pdst[0] = z0;
				pdst[1] = z1;
				z0 = (s0 & fm) | (d0 & ~fm);
				z1 = (s1 & fm) | (d1 & ~fm);
				pdst += 2;
				j += 8;
			}

			if (j <= (src_width_nchannels - 8)) {
				pdst[0] = z0;
				pdst[1] = z1;
				j += 8;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			psrc = ((mlib_u64 *)(psrc_row + j));
			pdst = ((mlib_u64 *)(pdst_row + j));
			u64_fm = ((mlib_u64)fm << 32) | fm;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 8); j += 8) {
				s0 = psrc[0];
				d0 = pdst[0];
				pdst[0] = (s0 & u64_fm) | (d0 & ~u64_fm);
				psrc++;
				pdst++;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		} else {
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			for (j = 0, mask = fm;
			    j < (mlib_s32)((4 - (mlib_addr)pdst_row) & 3);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (mask & 0xff) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask >>= 8;
#else /* _LITTLE_ENDIAN */

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
#endif /* _LITTLE_ENDIAN */
			}

			RECALC_MASK_U8_24(j);

			if (!(((mlib_addr)psrc_row ^
				(mlib_addr)pdst_row) & 3)) {

				psrc = ((mlib_u32 *)(psrc_row + j));
				pdst = ((mlib_u32 *)(pdst_row + j));

				s0 = psrc[0];
				d0 = pdst[0];
				s1 = psrc[1];
				d1 = pdst[1];
				z0 = (s0 & fm) | (d0 & ~fm);
				z1 = (s1 & fm) | (d1 & ~fm);

				s0 = psrc[2];
				d0 = pdst[2];
				s1 = psrc[3];
				d1 = pdst[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (src_width_nchannels - 24);
					j += 8) {
					pdst[0] = z0;
					pdst[1] = z1;
					z0 = (s0 & fm) | (d0 & ~fm);
					z1 = (s1 & fm) | (d1 & ~fm);
					s0 = psrc[4];
					d0 = pdst[4];
					s1 = psrc[5];
					d1 = pdst[5];
					psrc += 2;
					pdst += 2;
				}

				if (j <= (src_width_nchannels - 16)) {
					pdst[0] = z0;
					pdst[1] = z1;
					z0 = (s0 & fm) | (d0 & ~fm);
					z1 = (s1 & fm) | (d1 & ~fm);
					pdst += 2;
					j += 8;
				}

				if (j <= (src_width_nchannels - 8)) {
					pdst[0] = z0;
					pdst[1] = z1;
					z0 = (s0 & fm) | (d0 & ~fm);
					z1 = (s1 & fm) | (d1 & ~fm);
					j += 8;
				}
			} else {
				psrc = (mlib_u32 *)(psrc_row + j);
				pdst = (mlib_u32 *)(pdst_row + j);
				shl = (mlib_addr)psrc & 3;
				psrc = (mlib_u32 *)((mlib_addr)psrc - shl);
				shl <<= 3;
				shr = 32 - shl;

				s1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (src_width_nchannels - 4); j += 4) {
					s0 = s1;
					s1 = psrc[1];
					d0 = pdst[0];
#ifdef _LITTLE_ENDIAN
					d0 = (((s0 >> shl) | (s1 << shr)) & fm)
					    | (d0 & ~fm);
#else /* _LITTLE_ENDIAN */
					d0 = (((s0 << shl) | (s1 >> shr)) & fm)
					    | (d0 & ~fm);
#endif /* _LITTLE_ENDIAN */
					pdst[0] = d0;
					psrc++;
					pdst++;
				}
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			for (j = 0, mask = fm, cc = 0;
			    j < (mlib_s32)((8 - (mlib_addr)pdst_row) & 7);
			    j++) {

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
				cc++;

				if (cc == 4) {
					mask = fm;
					cc = 0;
				}
			}

			RECALC_MASK_U8_24(cc);

			psrc = (mlib_u64 *)(psrc_row + j);
			pdst = (mlib_u64 *)(pdst_row + j);
			u64_fm = ((mlib_u64)fm << 32) | fm;
			shl = (mlib_addr)psrc & 7;
			psrc = (mlib_u64 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 64 - shl;
			s1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 8); j += 8) {
				s0 = s1;
				s1 = psrc[1];
				d0 = pdst[0];
				pdst[0] =
				    (((s0 << shl) | (s1 >> shr)) & u64_fm) | (d0
				    & ~u64_fm);
				psrc++;
				pdst++;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}

		for (cc = 0, mask = fm; j < src_width_nchannels; j++) {
#ifdef _LITTLE_ENDIAN

			if (mask & 0xff) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask >>= 8;
#else /* _LITTLE_ENDIAN */

			if (mask & 0xff000000) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask <<= 8;
#endif /* _LITTLE_ENDIAN */
			cc++;

			if (cc == 4) {
				mask = fm;
				cc = 0;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_u8_3(
    const mlib_u8 *src,
    mlib_s32 src_stride,
    mlib_u8 *dst,
    mlib_s32 dst_stride,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, j, cc;
	mlib_u32 cfm1, cfm2, cfm3;
	mlib_u32 fm1, fm2, fm3;
	mlib_s32 src_width_nchannels = src_width * 3;
	mlib_u32 mask = cmask;
	mlib_u32 shr, shl;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_u32 *psrc, *pdst;
	mlib_u32 s0, s1, s2, d0, d1, d2, z0, z1, z2;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
	mlib_u64 *psrc, *pdst;
	mlib_u64 s0, s1, s2, d0, d1, d2, z0;
	mlib_u64 u64_fm1, u64_fm2, u64_fm3;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

	if (src_width < 8) {
		mlib_c_ImageChannelCopy_u8(src, src_stride, dst, dst_stride, 3,
		    src_width, src_height, cmask);
		return;
	}

	fm1 = 0;

	for (i = 0; i < 3; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm1 |= 0xff000000 >> (i << 3);
#else /* _LITTLE_ENDIAN */
			fm1 |= 0xff << (i << 3);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

#ifdef _LITTLE_ENDIAN
	cfm1 = (fm1 << 16) | (fm1 >> 8);
	cfm2 = (cfm1 >> 8);
	cfm2 |= (cfm2 << 24);
	cfm3 = (cfm2 >> 8);
	cfm3 |= (cfm3 << 24);
#else /* _LITTLE_ENDIAN */
	cfm1 = (fm1 >> 16) | (fm1 << 8);
	cfm2 = (cfm1 << 8);
	cfm2 |= (cfm2 >> 24);
	cfm3 = (cfm2 << 8);
	cfm3 |= (cfm3 >> 24);
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < src_height; i++) {

		mlib_u8 *psrc_row = (void *)(src + i * src_stride);
		mlib_u8 *pdst_row = dst + i * dst_stride;

		fm1 = cfm1;
		fm2 = cfm2;
		fm3 = cfm3;

		if ((!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7))) {

			for (j = 0, cc = 0, mask = fm1;
			    j < (mlib_s32)((8 - (mlib_addr)psrc_row) & 7);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (mask & 0xff) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask >>= 8;
#else /* _LITTLE_ENDIAN */

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
#endif /* _LITTLE_ENDIAN */
				cc++;

				if (cc == 3) {
					mask = fm1;
					cc = 0;
				}
			}

			RECALC_MASK_U8_3(cc);
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			psrc = ((mlib_u32 *)(psrc_row + j));
			pdst = ((mlib_u32 *)(pdst_row + j));

			s0 = psrc[0];
			s1 = psrc[1];
			s2 = psrc[2];
			d0 = pdst[0];
			d1 = pdst[1];
			d2 = pdst[2];
			z0 = (s0 & fm1) | (d0 & ~fm1);
			z1 = (s1 & fm2) | (d1 & ~fm2);
			z2 = (s2 & fm3) | (d2 & ~fm3);

			s0 = psrc[3];
			s1 = psrc[4];
			s2 = psrc[5];
			d0 = pdst[3];
			d1 = pdst[4];
			d2 = pdst[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 36); j += 12) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				z0 = (s0 & fm1) | (d0 & ~fm1);
				z1 = (s1 & fm2) | (d1 & ~fm2);
				z2 = (s2 & fm3) | (d2 & ~fm3);
				s0 = psrc[6];
				s1 = psrc[7];
				s2 = psrc[8];
				d0 = pdst[6];
				d1 = pdst[7];
				d2 = pdst[8];
				psrc += 3;
				pdst += 3;
			}

			if (j <= (src_width_nchannels - 24)) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				z0 = (s0 & fm1) | (d0 & ~fm1);
				z1 = (s1 & fm2) | (d1 & ~fm2);
				z2 = (s2 & fm3) | (d2 & ~fm3);
				pdst += 3;
				j += 12;
			}

			if (j <= (src_width_nchannels - 12)) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				j += 12;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			psrc = ((mlib_u64 *)(psrc_row + j));
			pdst = ((mlib_u64 *)(pdst_row + j));
			u64_fm1 = ((mlib_u64)fm1 << 32) | fm2;
			u64_fm2 = ((mlib_u64)fm3 << 32) | fm1;
			u64_fm3 = ((mlib_u64)fm2 << 32) | fm3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 24); j += 24) {
				s0 = psrc[0];
				s1 = psrc[1];
				s2 = psrc[2];
				d0 = pdst[0];
				d1 = pdst[1];
				d2 = pdst[2];
				d0 = (s0 & u64_fm1) | (d0 & ~u64_fm1);
				d1 = (s1 & u64_fm2) | (d1 & ~u64_fm2);
				d2 = (s2 & u64_fm3) | (d2 & ~u64_fm3);
				pdst[0] = d0;
				pdst[1] = d1;
				pdst[2] = d2;
				psrc += 3;
				pdst += 3;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		} else {

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
			for (j = 0, mask = fm1;
			    j < (mlib_s32)((4 - (mlib_addr)pdst_row) & 3);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (mask & 0xff) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask >>= 8;
#else /* _LITTLE_ENDIAN */

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
#endif /* _LITTLE_ENDIAN */
			}

			RECALC_MASK_U8_3(j);

			if (!(((mlib_addr)psrc_row ^
				(mlib_addr)pdst_row) & 3)) {

				psrc = ((mlib_u32 *)(psrc_row + j));
				pdst = ((mlib_u32 *)(pdst_row + j));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <=
				    (src_width_nchannels - 12); j += 12) {
					s0 = psrc[0];
					s1 = psrc[1];
					s2 = psrc[2];
					d0 = pdst[0];
					d1 = pdst[1];
					d2 = pdst[2];
					d0 = (s0 & fm1) | (d0 & ~fm1);
					d1 = (s1 & fm2) | (d1 & ~fm2);
					d2 = (s2 & fm3) | (d2 & ~fm3);
					pdst[0] = d0;
					pdst[1] = d1;
					pdst[2] = d2;
					psrc += 3;
					pdst += 3;
				}
			} else {
				psrc = (mlib_u32 *)(psrc_row + j);
				pdst = (mlib_u32 *)(pdst_row + j);
				shl = (mlib_addr)psrc & 3;
				psrc = (mlib_u32 *)((mlib_addr)psrc - shl);
				shl <<= 3;
				shr = 32 - shl;
				z0 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <=
				    (src_width_nchannels - 12); j += 12) {
					s0 = psrc[1];
					s1 = psrc[2];
					s2 = psrc[3];
					d0 = pdst[0];
					d1 = pdst[1];
					d2 = pdst[2];
#ifdef _LITTLE_ENDIAN
					d0 = (((z0 >> shl) | (s0 << shr)) & fm1)
					    | (d0 & ~fm1);
					d1 = (((s0 >> shl) | (s1 << shr)) & fm2)
					    | (d1 & ~fm2);
					d2 = (((s1 >> shl) | (s2 << shr)) & fm3)
					    | (d2 & ~fm3);
#else /* _LITTLE_ENDIAN */
					d0 = (((z0 << shl) | (s0 >> shr)) & fm1)
					    | (d0 & ~fm1);
					d1 = (((s0 << shl) | (s1 >> shr)) & fm2)
					    | (d1 & ~fm2);
					d2 = (((s1 << shl) | (s2 >> shr)) & fm3)
					    | (d2 & ~fm3);
#endif /* _LITTLE_ENDIAN */
					pdst[0] = d0;
					pdst[1] = d1;
					pdst[2] = d2;
					z0 = s2;
					psrc += 3;
					pdst += 3;
				}
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			for (j = 0, mask = fm1, cc = 0;
			    j < (mlib_s32)((8 - (mlib_addr)pdst_row) & 7);
			    j++) {

				if (mask & 0xff000000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 8;
				cc++;

				if (cc == 3) {
					mask = fm1;
					cc = 0;
				}
			}

			RECALC_MASK_U8_3(cc);

			psrc = (mlib_u64 *)(psrc_row + j);
			pdst = (mlib_u64 *)(pdst_row + j);
			u64_fm1 = ((mlib_u64)fm1 << 32) | fm2;
			u64_fm2 = ((mlib_u64)fm3 << 32) | fm1;
			u64_fm3 = ((mlib_u64)fm2 << 32) | fm3;
			shl = (mlib_addr)psrc & 7;
			psrc = (mlib_u64 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 64 - shl;
			z0 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 24); j += 24) {
				s0 = psrc[1];
				s1 = psrc[2];
				s2 = psrc[3];
				d0 = pdst[0];
				d1 = pdst[1];
				d2 = pdst[2];
				d0 = (((z0 << shl) | (s0 >> shr)) & u64_fm1) |
				    (d0 & ~u64_fm1);
				d1 = (((s0 << shl) | (s1 >> shr)) & u64_fm2) |
				    (d1 & ~u64_fm2);
				d2 = (((s1 << shl) | (s2 >> shr)) & u64_fm3) |
				    (d2 & ~u64_fm3);
				pdst[0] = d0;
				pdst[1] = d1;
				pdst[2] = d2;
				z0 = s2;
				psrc += 3;
				pdst += 3;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}

		for (cc = 0, mask = fm1; j < src_width_nchannels; j++) {
#ifdef _LITTLE_ENDIAN

			if (mask & 0xff) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask >>= 8;
#else /* _LITTLE_ENDIAN */

			if (mask & 0xff000000) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask <<= 8;
#endif /* _LITTLE_ENDIAN */
			cc++;

			if (cc == 3) {
				mask = fm1;
				cc = 0;
			}
		}
	}
}

/* *********************************************************** */

#define	RECALC_MASK_S16_2(cc)	if (cc) { fm = ~fm; }

/* *********************************************************** */

#define	RECALC_MASK_S16_3                                       \
	{                                                       \
	    mask = fm1;                                         \
	    fm1 = fm2;                                          \
	    fm2 = fm3;                                          \
	    fm3 = mask;                                         \
	}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	RECALC_MASK_S16_4                                       \
	{                                                       \
	    s0 = fm1;                                           \
	    s1 = fm2;                                           \
	    fm1 >>= 16;                                         \
	    fm2 >>= 16;                                         \
	    s0 <<= 16;                                          \
	    s1 <<= 16;                                          \
	    fm1 |= s1;                                          \
	    fm2 |= s0;                                          \
	}

#else /* _LITTLE_ENDIAN */

#define	RECALC_MASK_S16_4                                       \
	{                                                       \
	    s0 = fm1;                                           \
	    s1 = fm2;                                           \
	    fm1 <<= 16;                                         \
	    fm2 <<= 16;                                         \
	    s0 >>= 16;                                          \
	    s1 >>= 16;                                          \
	    fm1 |= s1;                                          \
	    fm2 |= s0;                                          \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_s16_2(
    const mlib_s16 *src,
    mlib_s32 src_stride,
    mlib_s16 *dst,
    mlib_s32 dst_stride,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 2,
	    src_width, src_height, cmask);
	return;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

	mlib_s32 i, j, cc;
	mlib_u32 fm, cfm;
	mlib_s32 src_width_nchannels = src_width * 2;
	mlib_u32 mask = cmask;
	mlib_u32 shr, shl;
	mlib_u64 *psrc, *pdst;
	mlib_u64 s0, s1, d0;
	mlib_u64 u64_fm;

	if (src_width < 16) {
		mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 2,
		    src_width, src_height, cmask);
		return;
	}

	fm = 0;

	for (i = 0; i < 2; i++) {
		if (mask & 1) {
			fm |= 0xffff << (i << 4);
		}

		mask >>= 1;
	}

	cfm = fm;

	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < src_height; i++) {

		mlib_s16 *psrc_row = (void *)(src + i * src_stride);
		mlib_s16 *pdst_row = dst + i * dst_stride;

		fm = cfm;

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			for (j = 0, cc = 0, mask = fm;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)psrc_row) & 7) >> 1);
			    j++) {

				if (mask & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 16;
				cc++;

				if (cc == 2) {
					mask = fm;
					cc = 0;
				}
			}

			RECALC_MASK_S16_2(cc);

			psrc = ((mlib_u64 *)(psrc_row + j));
			pdst = ((mlib_u64 *)(pdst_row + j));
			u64_fm = ((mlib_u64)fm << 32) | fm;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 4); j += 4) {
				s0 = psrc[0];
				d0 = pdst[0];
				pdst[0] = (s0 & u64_fm) | (d0 & ~u64_fm);
				psrc++;
				pdst++;
			}
		} else {
			for (j = 0, cc = 0, mask = fm;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)pdst_row) & 7) >> 1);
			    j++) {

				if (mask & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				mask <<= 16;
				cc++;

				if (cc == 2) {
					mask = fm;
					cc = 0;
				}
			}

			RECALC_MASK_S16_2(cc);

			psrc = (mlib_u64 *)(psrc_row + j);
			pdst = (mlib_u64 *)(pdst_row + j);
			u64_fm = ((mlib_u64)fm << 32) | fm;
			shl = (mlib_addr)psrc & 7;
			psrc = (mlib_u64 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 64 - shl;
			s1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 4); j += 4) {
				s0 = s1;
				s1 = psrc[1];
				d0 = pdst[0];
				pdst[0] =
				    (((s0 << shl) | (s1 >> shr)) & u64_fm) | (d0
				    & ~u64_fm);
				psrc++;
				pdst++;
			}
		}

		for (cc = 0, mask = fm; j < src_width_nchannels; j++) {
			if (mask & 0xffff0000) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask <<= 16;
			cc++;

			if (cc == 2) {
				mask = fm;
				cc = 0;
			}
		}
	}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_s16_3(
    const mlib_s16 *src,
    mlib_s32 src_stride,
    mlib_s16 *dst,
    mlib_s32 dst_stride,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, j, cc;
	mlib_u32 fm1, fm2, fm3, cfm1, cfm2, cfm3;
	mlib_s32 src_width_nchannels = src_width * 3;
	mlib_u32 mask = cmask;
	mlib_u32 shl, shr;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_u32 *psrc, *pdst;
	mlib_u32 s0, s1, s2, s3, d0, d1, d2, z0, z1, z2;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
	mlib_u64 *psrc, *pdst;
	mlib_u64 s0, s1, s2, s3, d0, d1, d2, z0, z1, z2;
	mlib_u64 u64_fm1, u64_fm2, u64_fm3;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

	if (src_width < 12) {
		mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 3,
		    src_width, src_height, cmask);
		return;
	}

	if ((cmask == 1) || (cmask == 2) || (cmask == 4)) {
		mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 3,
		    src_width, src_height, cmask);
		return;
	}

	fm1 = fm2 = 0;

	for (i = 0; i < 2; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm1 |= 0xffff0000 >> (i << 4);
#else /* _LITTLE_ENDIAN */
			fm1 |= 0xffff << (i << 4);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

	for (i = 0; i < 2; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm2 |= 0xffff0000 >> (i << 4);
#else /* _LITTLE_ENDIAN */
			fm2 |= 0xffff << (i << 4);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

#ifdef _LITTLE_ENDIAN
	fm2 |= fm1 >> 16;
	fm3 = (fm1 << 16) | (fm2 >> 16);
#else /* _LITTLE_ENDIAN */
	fm2 |= fm1 << 16;
	fm3 = (fm1 >> 16) | (fm2 << 16);
#endif /* _LITTLE_ENDIAN */

	cfm1 = fm1;
	cfm2 = fm2;
	cfm3 = fm3;

	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < src_height; i++) {

		mlib_s16 *psrc_row = (void *)(src + i * src_stride);
		mlib_s16 *pdst_row = dst + i * dst_stride;

		fm1 = cfm1;
		fm2 = cfm2;
		fm3 = cfm3;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 3)) {
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((4 - (mlib_addr)psrc_row) & 3) >> 1);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (fm3 & 0xffff) {
#else /* _LITTLE_ENDIAN */

				if (fm3 & 0xffff0000) {
#endif /* _LITTLE_ENDIAN */
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_3;
			}

			psrc = ((mlib_u32 *)(psrc_row + j));
			pdst = ((mlib_u32 *)(pdst_row + j));

			s0 = psrc[0];
			d0 = pdst[0];
			s1 = psrc[1];
			d1 = pdst[1];
			s2 = psrc[2];
			d2 = pdst[2];
			z0 = (s0 & fm3) | (d0 & ~fm3);
			z1 = (s1 & fm2) | (d1 & ~fm2);
			z2 = (s2 & fm1) | (d2 & ~fm1);

			s0 = psrc[3];
			d0 = pdst[3];
			s1 = psrc[4];
			d1 = pdst[4];
			s2 = psrc[5];
			d2 = pdst[5];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 18); j += 6) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				z0 = (s0 & fm3) | (d0 & ~fm3);
				z1 = (s1 & fm2) | (d1 & ~fm2);
				z2 = (s2 & fm1) | (d2 & ~fm1);
				s0 = psrc[6];
				d0 = pdst[6];
				s1 = psrc[7];
				d1 = pdst[7];
				s2 = psrc[8];
				d2 = pdst[8];
				psrc += 3;
				pdst += 3;
			}

			if (j <= (src_width_nchannels - 12)) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				z0 = (s0 & fm3) | (d0 & ~fm3);
				z1 = (s1 & fm2) | (d1 & ~fm2);
				z2 = (s2 & fm1) | (d2 & ~fm1);
				pdst += 3;
				j += 6;
			}

			if (j <= (src_width_nchannels - 6)) {
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				j += 6;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)psrc_row) & 7) >> 1);
			    j++) {

				if (fm3 & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_3;
			}

			psrc = ((mlib_u64 *)(psrc_row + j));
			pdst = ((mlib_u64 *)(pdst_row + j));
			u64_fm1 = ((mlib_u64)fm2 << 32) | fm1;
			u64_fm2 = ((mlib_u64)fm1 << 32) | fm3;
			u64_fm3 = ((mlib_u64)fm3 << 32) | fm2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 12); j += 12) {
				s0 = psrc[0];
				d0 = pdst[0];
				s1 = psrc[1];
				d1 = pdst[1];
				s2 = psrc[2];
				d2 = pdst[2];
				z0 = (s0 & u64_fm3) | (d0 & ~u64_fm3);
				z1 = (s1 & u64_fm2) | (d1 & ~u64_fm2);
				z2 = (s2 & u64_fm1) | (d2 & ~u64_fm1);
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				psrc += 3;
				pdst += 3;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		} else {
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

			if (j = (((mlib_addr)pdst_row & 2) != 0)) {
#ifdef _LITTLE_ENDIAN

				if (fm3 & 0xffff) {
#else /* _LITTLE_ENDIAN */

				if (fm3 & 0xffff0000) {
#endif /* _LITTLE_ENDIAN */
					*(pdst_row) = *(psrc_row);
				}

				RECALC_MASK_S16_3;
			}

			psrc = (mlib_u32 *)(psrc_row + j);
			pdst = (mlib_u32 *)(pdst_row + j);
			shl = (mlib_addr)psrc & 3;
			psrc = (mlib_u32 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 32 - shl;
			s3 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 6); j += 6) {
				s0 = s3;
				s1 = psrc[1];
				s2 = psrc[2];
				s3 = psrc[3];
				d0 = pdst[0];
				d1 = pdst[1];
				d2 = pdst[2];
#ifdef _LITTLE_ENDIAN
				z0 = (((s0 >> shl) | (s1 << shr)) & fm3) | (d0 &
				    ~fm3);
				z1 = (((s1 >> shl) | (s2 << shr)) & fm2) | (d1 &
				    ~fm2);
				z2 = (((s2 >> shl) | (s3 << shr)) & fm1) | (d2 &
				    ~fm1);
#else /* _LITTLE_ENDIAN */
				z0 = (((s0 << shl) | (s1 >> shr)) & fm3) | (d0 &
				    ~fm3);
				z1 = (((s1 << shl) | (s2 >> shr)) & fm2) | (d1 &
				    ~fm2);
				z2 = (((s2 << shl) | (s3 >> shr)) & fm1) | (d2 &
				    ~fm1);
#endif /* _LITTLE_ENDIAN */
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				psrc += 3;
				pdst += 3;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)pdst_row) & 7) >> 1);
			    j++) {

				if (fm3 & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_3;
			}

			u64_fm1 = ((mlib_u64)fm2 << 32) | fm1;
			u64_fm2 = ((mlib_u64)fm1 << 32) | fm3;
			u64_fm3 = ((mlib_u64)fm3 << 32) | fm2;
			psrc = (mlib_u64 *)(psrc_row + j);
			pdst = (mlib_u64 *)(pdst_row + j);
			shl = (mlib_addr)psrc & 7;
			psrc = (mlib_u64 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 64 - shl;
			s3 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 12); j += 12) {
				s0 = s3;
				s1 = psrc[1];
				s2 = psrc[2];
				s3 = psrc[3];
				d0 = pdst[0];
				d1 = pdst[1];
				d2 = pdst[2];
				z0 = (((s0 << shl) | (s1 >> shr)) & u64_fm3) |
				    (d0 & ~u64_fm3);
				z1 = (((s1 << shl) | (s2 >> shr)) & u64_fm2) |
				    (d1 & ~u64_fm2);
				z2 = (((s2 << shl) | (s3 >> shr)) & u64_fm1) |
				    (d2 & ~u64_fm1);
				pdst[0] = z0;
				pdst[1] = z1;
				pdst[2] = z2;
				psrc += 3;
				pdst += 3;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}

		for (cc = 0, mask = fm3; j < src_width_nchannels; j++) {
#ifdef _LITTLE_ENDIAN

			if (mask & 0xffff) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask >>= 16;
#else /* _LITTLE_ENDIAN */

			if (mask & 0xffff0000) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask <<= 16;
#endif /* _LITTLE_ENDIAN */
			cc++;

			if (cc == 2) {
				mask = fm2;
			} else {
				if (cc == 4) {
					mask = fm1;
				}
			}

			if (cc == 6) {
				mask = fm3;
				cc = 0;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelCopy_s16_4(
    const mlib_s16 *src,
    mlib_s32 src_stride,
    mlib_s16 *dst,
    mlib_s32 dst_stride,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_s32 i, j, cc;
	mlib_u32 cfm1, cfm2, fm1, fm2;
	mlib_s32 src_width_nchannels = src_width * 4;
	mlib_u32 mask = cmask;
	mlib_u32 shl, shr;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_u32 *psrc, *pdst;
	mlib_u32 s0, s1, s2, d0, d1, z0, z1;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
	mlib_u64 *psrc, *pdst;
	mlib_u64 s0, s1, d0;
	mlib_u64 u64_fm;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

	fm1 = fm2 = 0;

	if (src_width < 16) {
		mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 4,
		    src_width, src_height, cmask);
		return;
	}

	if ((cmask == 1) || (cmask == 2) || (cmask == 4) || (cmask == 8)) {
		mlib_c_ImageChannelCopy_s16(src, src_stride, dst, dst_stride, 4,
		    src_width, src_height, cmask);
		return;
	}

	for (i = 0; i < 2; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm1 |= 0xffff0000 >> (i << 4);
#else /* _LITTLE_ENDIAN */
			fm1 |= 0xffff << (i << 4);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

	for (i = 2; i < 4; i++) {
		if (mask & 1) {
#ifdef _LITTLE_ENDIAN
			fm2 |= 0xffff0000 >> ((i - 2) << 4);
#else /* _LITTLE_ENDIAN */
			fm2 |= 0xffff << ((i - 2) << 4);
#endif /* _LITTLE_ENDIAN */
		}

		mask >>= 1;
	}

	cfm1 = fm1;
	cfm2 = fm2;

	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < src_height; i++) {

		mlib_s16 *psrc_row = (void *)(src + i * src_stride);
		mlib_s16 *pdst_row = dst + i * dst_stride;

		fm1 = cfm1;
		fm2 = cfm2;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 3)) {
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((4 - (mlib_addr)psrc_row) & 3) >> 1);
			    j++) {
#ifdef _LITTLE_ENDIAN

				if (fm2 & 0xffff) {
#else /* _LITTLE_ENDIAN */

				if (fm2 & 0xffff0000) {
#endif /* _LITTLE_ENDIAN */
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_4;
			}

			psrc = ((mlib_u32 *)(psrc_row + j));
			pdst = ((mlib_u32 *)(pdst_row + j));

			s0 = psrc[0];
			s1 = psrc[1];
			d0 = pdst[0];
			d1 = pdst[1];
			z0 = (s0 & fm2) | (d0 & ~fm2);
			z1 = (s1 & fm1) | (d1 & ~fm1);

			s0 = psrc[2];
			s1 = psrc[3];
			d0 = pdst[2];
			d1 = pdst[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 12); j += 4) {
				pdst[0] = z0;
				pdst[1] = z1;
				z0 = (s0 & fm2) | (d0 & ~fm2);
				z1 = (s1 & fm1) | (d1 & ~fm1);
				s0 = psrc[4];
				s1 = psrc[5];
				d0 = pdst[4];
				d1 = pdst[5];
				psrc += 2;
				pdst += 2;
			}

			if (j <= (src_width_nchannels - 8)) {
				pdst[0] = z0;
				pdst[1] = z1;
				z0 = (s0 & fm2) | (d0 & ~fm2);
				z1 = (s1 & fm1) | (d1 & ~fm1);
				pdst += 2;
				j += 4;
			}

			if (j <= (src_width_nchannels - 4)) {
				pdst[0] = z0;
				pdst[1] = z1;
				j += 4;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

		if (!(((mlib_addr)psrc_row ^ (mlib_addr)pdst_row) & 7)) {
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)psrc_row) & 7) >> 1);
			    j++) {

				if (fm2 & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_4;
			}

			u64_fm = ((mlib_u64)fm2 << 32) | fm1;
			psrc = ((mlib_u64 *)(psrc_row + j));
			pdst = ((mlib_u64 *)(pdst_row + j));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 4); j += 4) {
				s0 = psrc[0];
				d0 = pdst[0];
				pdst[0] = (s0 & u64_fm) | (d0 & ~u64_fm);
				pdst++;
				psrc++;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		} else {
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

			if (j = (((mlib_addr)pdst_row & 2) != 0)) {
#ifdef _LITTLE_ENDIAN

				if (fm2 & 0xffff) {
#else /* _LITTLE_ENDIAN */

				if (fm2 & 0xffff0000) {
#endif /* _LITTLE_ENDIAN */
					*(pdst_row) = *(psrc_row);
				}

				RECALC_MASK_S16_4;
			}

			psrc = (mlib_u32 *)(psrc_row + j);
			pdst = (mlib_u32 *)(pdst_row + j);
			shl = (mlib_addr)psrc & 3;
			psrc = (mlib_u32 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 32 - shl;
			s2 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 4); j += 4) {
				s0 = s2;
				s1 = psrc[1];
				s2 = psrc[2];
				d0 = pdst[0];
				d1 = pdst[1];
#ifdef _LITTLE_ENDIAN
				z0 = (((s0 >> shl) | (s1 << shr)) & fm2) | (d0 &
				    ~fm2);
				z1 = (((s1 >> shl) | (s2 << shr)) & fm1) | (d1 &
				    ~fm1);
#else /* _LITTLE_ENDIAN */
				z0 = (((s0 << shl) | (s1 >> shr)) & fm2) | (d0 &
				    ~fm2);
				z1 = (((s1 << shl) | (s2 >> shr)) & fm1) | (d1 &
				    ~fm1);
#endif /* _LITTLE_ENDIAN */
				pdst[0] = z0;
				pdst[1] = z1;
				psrc += 2;
				pdst += 2;
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			for (j = 0, cc = 0;
			    j <
			    (mlib_s32)(((8 - (mlib_addr)pdst_row) & 7) >> 1);
			    j++) {

				if (fm2 & 0xffff0000) {
					*(pdst_row + j) = *(psrc_row + j);
				}

				RECALC_MASK_S16_4;
			}

			u64_fm = ((mlib_u64)fm2 << 32) | fm1;
			psrc = (mlib_u64 *)(psrc_row + j);
			pdst = (mlib_u64 *)(pdst_row + j);
			shl = (mlib_addr)psrc & 7;
			psrc = (mlib_u64 *)((mlib_addr)psrc - shl);
			shl <<= 3;
			shr = 64 - shl;
			s1 = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (src_width_nchannels - 4); j += 4) {
				s0 = s1;
				s1 = psrc[1];
				d0 = pdst[0];
				pdst[0] =
				    (((s0 << shl) | (s1 >> shr)) & u64_fm) | (d0
				    & ~u64_fm);
				psrc++;
				pdst++;
			}

#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}

		for (cc = 0, mask = fm2; j < src_width_nchannels; j++) {
#ifdef _LITTLE_ENDIAN

			if (mask & 0xffff) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask >>= 16;
#else /* _LITTLE_ENDIAN */

			if (mask & 0xffff0000) {
				*(pdst_row + j) = *(psrc_row + j);
			}

			mask <<= 16;
#endif /* _LITTLE_ENDIAN */
			cc++;

			if (cc == 2) {
				mask = fm1;
				cc = 0;
			}
		}
	}
}

/* *********************************************************** */
