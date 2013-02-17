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

#pragma ident	"@(#)mlib_ImageFlipY_f.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFlipY - flip image on Y axis
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipY(mlib_image       *dst,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     pointer to an dst image
 *      src     pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipY.h>

/* *********************************************************** */

#ifdef i386	/* do not perform the copying by mlib_d64 data type for x86 */

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

/* *********************************************************** */

#define	PREPAREVARS(data_type, num_ch)                                     \
	mlib_s32 sstride =                                                 \
		mlib_ImageGetStride(src) / (mlib_s32)sizeof (data_type);   \
	mlib_s32 dstride =                                                 \
		mlib_ImageGetStride(dst) / (mlib_s32)sizeof (data_type);   \
	mlib_s32 ws = mlib_ImageGetWidth(src);                             \
	mlib_s32 hs = mlib_ImageGetHeight(src);                            \
	mlib_s32 wd = mlib_ImageGetWidth(dst);                             \
	mlib_s32 hd = mlib_ImageGetHeight(dst);                            \
	mlib_s32 sw = (ws < wd) ? ws : wd;                                 \
	mlib_s32 sh = (hs < hd) ? hs : hd;                                 \
	mlib_s32 dx = ((mlib_s32)ws - (mlib_s32)wd);                       \
	mlib_s32 dy = ((mlib_s32)hs - (mlib_s32)hd);                       \
	mlib_s32 Q = (sstride >= 0) ? 1 : 0;                               \
	                                                                   \
/*                                                                         \
 * for Rotate180                                                           \
 */                                                                        \
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);                     \
	mlib_s32 dxd = ((dx > 0) ? 0 : (1 - dx) >> 1);                     \
	mlib_s32 dys = ((dy > 0) ? (dy + Q) >> 1 : 0);                     \
	mlib_s32 dyd = ((dy > 0) ? 0 : (1 - Q - dy) >> 1);                 \
	data_type *sdata =                                                 \
		(data_type *) mlib_ImageGetData(src) + dxs * num_ch +      \
		sstride * dys;                                             \
	data_type *ddata =                                                 \
		(data_type *) mlib_ImageGetData(dst) + dxd * num_ch +      \
		dstride * dyd

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, data_type)                                \
	{                                                               \
	    mlib_s32 i0, j0, _w = (w), _h = (h);                        \
	    data_type *_ps = (ps), *_pd = (pd);                         \
	    data_type s0, s1;                                           \
	                                                                \
	    for (i0 = 0; i0 < _h; i0++) {                               \
		if (j0 = _w & 1)                                        \
		    _pd[i0 * dstride + (_w - 1)] = _ps[i0 * sstride];   \
		for (; j0 < _w; j0 += 2) {                              \
		    s0 = _ps[i0 * sstride + j0];                        \
		    s1 = _ps[i0 * sstride + j0 + 1];                    \
		    _pd[i0 * dstride + (_w - j0 - 1)] = s0;             \
		    _pd[i0 * dstride + (_w - j0 - 2)] = s1;             \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u8, 1);

	if (sw < 16) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + sw - 1, *pdst_row =
		    ddata + i * dstride;

		if (!(((mlib_addr)(psrc_row + 1) ^ (4 -
		    (mlib_addr)pdst_row)) & 3)) {
			for (j = 0;
			    j < (mlib_s32)((4 - (mlib_addr)pdst_row) & 3);
			    j++) {
				pdst_row[j] = psrc_row[-j];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (sw - 4); j += 4) {
				mlib_u32 dst1 =
				    *((mlib_u32 *)(psrc_row - j - 3)), dst0;

				dst0 = (dst1 >> 16) | (dst1 << 16);
				dst1 = ((dst0 << 8) & 0xff00ff00);
				*((mlib_u32 *)(pdst_row + j)) =
				    dst1 | ((dst0 >> 8) & 0xff00ff);
			}
		} else {
			if (!(((mlib_addr)(psrc_row +
			    1) ^ (mlib_addr)pdst_row) & 1)) {
				if (j = (mlib_s32)(((mlib_addr)pdst_row) & 1)) {
					pdst_row[0] = psrc_row[0];
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (sw - 4); j += 4) {
					mlib_u16 src0 =
					    *((mlib_u16 *)(psrc_row - j - 1));
					mlib_u16 src1 =
					    *((mlib_u16 *)(psrc_row - j - 3));

					*((mlib_u16 *)(pdst_row + j)) =
					    (src0 >> 8) | (src0 << 8);
					*((mlib_u16 *)(pdst_row + j + 2)) =
					    (src1 >> 8) | (src1 << 8);
				}
			} else {
				if (j = (mlib_s32)(((mlib_addr)pdst_row) & 1)) {
					pdst_row[0] = psrc_row[0];
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (sw - 4); j += 4) {
					mlib_u16 src0 =
					    *((mlib_u8 *)(psrc_row - j)),
					    src1 =
					    *((mlib_u8 *)(psrc_row - j - 1)),
					    src2 =
					    *((mlib_u8 *)(psrc_row - j - 2)),
					    src3 =
					    *((mlib_u8 *)(psrc_row - j - 3));

#ifdef _LITTLE_ENDIAN
					*((mlib_u16 *)(pdst_row + j)) =
					    (src1 << 8) | src0;
					*((mlib_u16 *)(pdst_row + j + 2)) =
					    (src3 << 8) | src2;
#else /* _LITTLE_ENDIAN */
					*((mlib_u16 *)(pdst_row + j)) =
					    (src0 << 8) | src1;
					*((mlib_u16 *)(pdst_row + j + 2)) =
					    (src2 << 8) | src3;
#endif /* _LITTLE_ENDIAN */
				}
			}
		}

		for (; j < sw; j++)
			pdst_row[j] = psrc_row[-j];
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u16, 1);

	if (sw < 8) {
		STRIP_1(ddata, sdata, sw, sh, mlib_u16);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row = sdata + i * sstride + sw - 1, *pdst_row =
		    ddata + i * dstride;

		if (!(((mlib_addr)(psrc_row + 1) ^ (mlib_addr)pdst_row) & 3)) {
			if (j = (mlib_s32)(((mlib_addr)pdst_row) & 2)) {
				pdst_row[0] = psrc_row[0];
				j--;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (sw - 4); j += 4) {
				mlib_u32 src0 =
				    *((mlib_u32 *)(psrc_row - j - 1));
				mlib_u32 src1 =
				    *((mlib_u32 *)(psrc_row - j - 3));

				*((mlib_u32 *)(pdst_row + j)) =
				    (src0 >> 16) | (src0 << 16);
				*((mlib_u32 *)(pdst_row + j + 2)) =
				    (src1 >> 16) | (src1 << 16);
			}
		} else {
			if (j = (mlib_s32)(((mlib_addr)pdst_row) & 2)) {
				pdst_row[0] = psrc_row[0];
				j--;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (sw - 4); j += 4) {
				mlib_u32 src0 = *((mlib_u16 *)(psrc_row - j)),
				    src1 = *((mlib_u16 *)(psrc_row - j - 1)),
				    src2 = *((mlib_u16 *)(psrc_row - j - 2)),
				    src3 = *((mlib_u16 *)(psrc_row - j - 3));

#ifdef _LITTLE_ENDIAN
				*((mlib_u32 *)(pdst_row + j)) =
				    (src1 << 16) | src0;
				*((mlib_u32 *)(pdst_row + j + 2)) =
				    (src3 << 16) | src2;
#else /* _LITTLE_ENDIAN */
				*((mlib_u32 *)(pdst_row + j)) =
				    (src0 << 16) | src1;
				*((mlib_u32 *)(pdst_row + j + 2)) =
				    (src2 << 16) | src3;
#endif /* _LITTLE_ENDIAN */
			}
		}

		for (; j < sw; j++)
			pdst_row[j] = psrc_row[-j];
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 1);

	STRIP_1(ddata, sdata, sw, sh, mlib_u32);
}

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1;                                       \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 2 * j];                  \
		    s1 = _ps[i * sstride + 2 * j + 1];              \
		    _pd[i * dstride + 2 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 2 * (_w - j - 1) + 1] = s1;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_U8_2(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u8, 2);

	if ((((mlib_addr)sdata | sstride | (mlib_addr)ddata | dstride) & 1) ==
	    0) {
		mlib_ImageFlipY_S16_1(dst, src);
		return;
	}

	if (sw < 16) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + (sw - 1) * 2,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 1)) {

			if (!(((mlib_addr)(psrc_row + 2) ^ (4 -
			    (mlib_addr)pdst_row)) & 3)) {
				if (j = (mlib_s32)(((mlib_addr)pdst_row & 2)
				    >> 1)) {
					*(mlib_u16 *)(pdst_row) =
					    *(mlib_u16 *)(psrc_row);
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (sw - 2); j += 2) {
					mlib_u32 dst1 =
					    *((mlib_u32 *)(psrc_row - j * 2 -
					    2));

					*((mlib_u32 *)(pdst_row + j * 2)) =
					    (dst1 << 16) | (dst1 >> 16);
				}

				psrc_row++;
			} else {
				if (!((mlib_addr)psrc_row & 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j <= (sw - 2); j += 2) {
						mlib_u16 dst1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 2)),
						    dst0 =
						    *((mlib_u16 *)(psrc_row -
						    j * 2 - 2));

						*((mlib_u16 *)(pdst_row +
						    j * 2)) = dst1;
						*((mlib_u16 *)(pdst_row +
						    j * 2 + 2)) = dst0;
					}

					psrc_row++;
				} else {
					j = 0;
					psrc_row++;
				}
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < sw; j++) {
				mlib_u16 dst1 =
				    *((mlib_u8 *)(psrc_row - j * 2)),
				    dst0 =
				    *((mlib_u8 *)(psrc_row - j * 2 - 1));

#ifdef _LITTLE_ENDIAN
				*((mlib_u16 *)(pdst_row + j * 2)) =
				    (dst1 << 8) | dst0;
#else /* _LITTLE_ENDIAN */
				*((mlib_u16 *)(pdst_row + j * 2)) =
				    (dst0 << 8) | dst1;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			psrc_row = psrc_row + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				mlib_u8 s1 = psrc_row[-j * 2], s0 =
				    psrc_row[-j * 2 - 1];

				pdst_row[j * 2] = s0;
				pdst_row[j * 2 + 1] = s1;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S16_2(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u16, 2);

	if ((((mlib_addr)sdata | sstride | (mlib_addr)ddata | dstride) & 3) ==
	    0) {
		mlib_ImageFlipY_S32_1(dst, src);
		return;
	}

	if (sw < 8) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u16);

		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row = sdata + i * sstride + (sw - 1) * 2,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 3)) {
			if (!((mlib_addr)psrc_row & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (sw - 2); j += 2) {
					mlib_u32 dst1 =
					    *((mlib_u32 *)(psrc_row - j * 2)),
					    dst0 =
					    *((mlib_u32 *)(psrc_row - j * 2 -
					    2));

					*((mlib_u32 *)(pdst_row + j * 2)) =
					    dst1;
					*((mlib_u32 *)(pdst_row + j * 2 + 2)) =
					    dst0;
				}

				psrc_row++;
			} else {
				j = 0;
				psrc_row++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < sw; j++) {
				mlib_u32 dst1 =
				    *((mlib_u16 *)(psrc_row - j * 2)),
				    dst0 =
				    *((mlib_u16 *)(psrc_row - j * 2 - 1));

#ifdef _LITTLE_ENDIAN
				*((mlib_u32 *)(pdst_row + j * 2)) =
				    (dst1 << 16) | dst0;
#else /* _LITTLE_ENDIAN */
				*((mlib_u32 *)(pdst_row + j * 2)) =
				    (dst0 << 16) | dst1;
#endif /* _LITTLE_ENDIAN */
			}
		} else {
			psrc_row = psrc_row + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				mlib_u16 s1 = psrc_row[-j * 2],
				    s0 = psrc_row[-j * 2 - 1];

				pdst_row[j * 2] = s0;
				pdst_row[j * 2 + 1] = s1;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S32_2(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u32, 2);

	if (sw < 4) {
		STRIP_2(ddata, sdata, sw, sh, mlib_u32);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u32 *psrc_row = sdata + i * sstride + (sw - 1) * 2,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 7) &&
		    !((mlib_addr)(psrc_row + 2) & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				TYPE_64BIT dsrc0 =
				    *((TYPE_64BIT *) (psrc_row - j * 2));

				*((TYPE_64BIT *) (pdst_row + j * 2)) = dsrc0;
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				mlib_u32 src0 =
				    *((mlib_u32 *)(psrc_row - j * 2)),
				    src1 =
				    *((mlib_u32 *)(psrc_row - j * 2 + 1));

				*((mlib_u32 *)(pdst_row + j * 2)) = src0;
				*((mlib_u32 *)(pdst_row + j * 2 + 1)) = src1;
			}
		}
	}
}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1, s2;                                   \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 3 * j];                  \
		    s1 = _ps[i * sstride + 3 * j + 1];              \
		    s2 = _ps[i * sstride + 3 * j + 2];              \
		    _pd[i * dstride + 3 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 3 * (_w - j - 1) + 1] = s1;   \
		    _pd[i * dstride + 3 * (_w - j - 1) + 2] = s2;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u8, 3);

	if (sw < 8) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + (sw - 1) * 3,
		    *pdst_row = ddata + i * dstride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < sw; j++) {
			mlib_u8 s0 = psrc_row[-j * 3],
			    s1 =
			    psrc_row[-j * 3 + 1], s2 = psrc_row[-j * 3 + 2];
			pdst_row[j * 3] = s0;
			pdst_row[j * 3 + 1] = s1;
			pdst_row[j * 3 + 2] = s2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u16, 3);

	if (sw < 8) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u16);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row = sdata + i * sstride + (sw - 1) * 3,
		    *pdst_row = ddata + i * dstride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < sw; j++) {
			mlib_u16 s0 = psrc_row[-j * 3],
			    s1 = psrc_row[-j * 3 + 1],
			    s2 = psrc_row[-j * 3 + 2];
			pdst_row[j * 3] = s0;
			pdst_row[j * 3 + 1] = s1;
			pdst_row[j * 3 + 2] = s2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u32, 3);

	if (sw < 8) {
		STRIP_3(ddata, sdata, sw, sh, mlib_u32);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u32 *psrc_row = sdata + i * sstride + (sw - 1) * 3,
		    *pdst_row = ddata + i * dstride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < sw; j++) {
			mlib_u32 s0 = psrc_row[-j * 3],
			    s1 = psrc_row[-j * 3 + 1],
			    s2 = psrc_row[-j * 3 + 2];
			pdst_row[j * 3] = s0;
			pdst_row[j * 3 + 1] = s1;
			pdst_row[j * 3 + 2] = s2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_d64, 3);

	for (i = 0; i < sh; i++) {
		mlib_d64 *psrc_row = sdata + i * sstride + (sw - 1) * 3,
		    *pdst_row = ddata + i * dstride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < sw; j++) {
			mlib_d64 s0 = psrc_row[-j * 3],
			    s1 = psrc_row[-j * 3 + 1],
			    s2 = psrc_row[-j * 3 + 2];
			pdst_row[j * 3] = s0;
			pdst_row[j * 3 + 1] = s1;
			pdst_row[j * 3 + 2] = s2;
		}
	}
}

/* *********************************************************** */

#define	STRIP_4(pd, ps, w, h, data_type)                            \
	{                                                           \
	    mlib_s32 i, j, _w = (w), _h = (h);                      \
	    data_type *_ps = (ps), *_pd = (pd);                     \
	    data_type s0, s1, s2, s3;                               \
	                                                            \
	    for (i = 0; i < _h; i++) {                              \
		for (j = 0; j < _w; j++) {                          \
		    s0 = _ps[i * sstride + 4 * j];                  \
		    s1 = _ps[i * sstride + 4 * j + 1];              \
		    s2 = _ps[i * sstride + 4 * j + 2];              \
		    s3 = _ps[i * sstride + 4 * j + 3];              \
		    _pd[i * dstride + 4 * (_w - j - 1)] = s0;       \
		    _pd[i * dstride + 4 * (_w - j - 1) + 1] = s1;   \
		    _pd[i * dstride + 4 * (_w - j - 1) + 2] = s2;   \
		    _pd[i * dstride + 4 * (_w - j - 1) + 3] = s3;   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_ImageFlipY_U8_4(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u8, 4);

	if (sw < 8) {
		STRIP_4(ddata, sdata, sw, sh, mlib_u8);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u8 *psrc_row = sdata + i * sstride + (sw - 1) * 4,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 3)) {
			if (!((mlib_addr)psrc_row & 3)) {
				if (sw & 1) {
					*((mlib_u32 *)(pdst_row + (sw -
					    1) * 4)) =
					    *((mlib_u32 *)(psrc_row + (1 -
					    sw) * 4));
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (sw - 2); j += 2) {
					mlib_u32 src0 =
					    *((mlib_u32 *)(psrc_row - j * 4));
					mlib_u32 src1 =
					    *((mlib_u32 *)(psrc_row - j * 4 -
					    4));

					*((mlib_u32 *)(pdst_row + j * 4)) =
					    src0;
					*((mlib_u32 *)(pdst_row + j * 4 + 4)) =
					    src1;
				}
			} else {
				if (!((mlib_addr)psrc_row & 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 2));

#ifdef _LITTLE_ENDIAN
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src1 << 16) | src0;
#else /* _LITTLE_ENDIAN */
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src0 << 16) | src1;
#endif /* _LITTLE_ENDIAN */
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u8 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 1));
						mlib_u32 src2 =
						    *((mlib_u8 *)(psrc_row -
						    j * 4 + 3));

#ifdef _LITTLE_ENDIAN
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src2 << 24) | (src1 << 8) |
						    (src0);
#else /* _LITTLE_ENDIAN */
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src0 << 24) | (src1 << 8) |
						    (src2);
#endif /* _LITTLE_ENDIAN */
					}
				}
			}
		} else {
			if (!((mlib_addr)pdst_row & 1)) {
				if (!((mlib_addr)psrc_row & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u32 *)(psrc_row -
						    j * 4));

#ifdef _LITTLE_ENDIAN
						*((mlib_u16 *)(pdst_row +
						    j * 4)) = src0 & 0xffff;
						*((mlib_u16 *)(pdst_row +
						    j * 4 + 2)) = src0 >> 16;
#else /* _LITTLE_ENDIAN */
						*((mlib_u16 *)(pdst_row +
						    j * 4)) = src0 >> 16;
						*((mlib_u16 *)(pdst_row +
						    j * 4 + 2)) = src0 & 0xffff;
#endif /* _LITTLE_ENDIAN */
					}
				} else {
					if (!((mlib_addr)psrc_row & 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j < sw; j++) {
							mlib_u16 src0 =
							    *((mlib_u16
							    *)(psrc_row -
							    j * 4));
							mlib_u16 src1 =
							    *((mlib_u16
							    *)(psrc_row -
							    j * 4 + 2));

							*((mlib_u16 *)(pdst_row
							    + j * 4)) = src0;
							*((mlib_u16 *)(pdst_row
							    + j * 4 + 2)) =
							    src1;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (j = 0; j < sw; j++) {
							mlib_u16 src0 =
							    *((mlib_u8
							    *)(psrc_row -
							    j * 4));
							mlib_u16 src1 =
							    *((mlib_u8
							    *)(psrc_row -
							    j * 4 + 1));
							mlib_u16 src2 =
							    *((mlib_u8
							    *)(psrc_row -
							    j * 4 + 2));
							mlib_u16 src3 =
							    *((mlib_u8
							    *)(psrc_row -
							    j * 4 + 3));

#ifdef _LITTLE_ENDIAN
							*((mlib_u16 *)(pdst_row
							    + j * 4)) =
							    (src1 << 8) | src0;
							*((mlib_u16 *)(pdst_row
							    + j * 4 + 2)) =
							    (src3 << 8) | src2;
#else /* _LITTLE_ENDIAN */
							*((mlib_u16 *)(pdst_row
							    + j * 4)) =
							    (src0 << 8) | src1;
							*((mlib_u16 *)(pdst_row
							    + j * 4 + 2)) =
							    (src2 << 8) | src3;
#endif /* _LITTLE_ENDIAN */
						}
					}
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < sw; j++) {
					mlib_u8 s0 = psrc_row[-j * 4],
					    s1 = psrc_row[1 - j * 4],
					    s2 = psrc_row[2 - j * 4],
					    s3 = psrc_row[3 - j * 4];

					pdst_row[j * 4] = s0;
					pdst_row[j * 4 + 1] = s1;
					pdst_row[j * 4 + 2] = s2;
					pdst_row[j * 4 + 3] = s3;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S16_4(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u16, 4);

	if ((((mlib_addr)sdata | sstride | (mlib_addr)ddata | dstride) & 7) ==
	    0) {
		mlib_ImageFlipY_S32_2(dst, src);
		return;
	}

	if (sw < 8) {
		STRIP_4(ddata, sdata, sw, sh, mlib_u16);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u16 *psrc_row = sdata + i * sstride + (sw - 1) * 4,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 7)) {
			if (!((mlib_addr)psrc_row & 7)) {
				if (sw & 1) {
					*((TYPE_64BIT *) (pdst_row + (sw -
					    1) * 4)) =
					    *((TYPE_64BIT *) (psrc_row + (1 -
					    sw) * 4));
				}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (sw - 2); j += 2) {
					TYPE_64BIT src0 =
					    *((TYPE_64BIT *) (psrc_row -
					    j * 4));
					TYPE_64BIT src1 =
					    *((TYPE_64BIT *) (psrc_row - j * 4 -
					    4));

					*((TYPE_64BIT *) (pdst_row + j * 4)) =
					    src0;
					*((TYPE_64BIT *) (pdst_row + j * 4 +
					    4)) = src1;
				}
			} else {
				if (!((mlib_addr)psrc_row & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u32 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u32 *)(psrc_row -
						    j * 4 + 2));

						*((mlib_u32 *)(pdst_row +
						    j * 4)) = src0;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) = src1;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 1));
						mlib_u32 src2 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 2));
						mlib_u32 src3 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 3));

#ifdef _LITTLE_ENDIAN
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src1 << 16) | src0;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) =
						    (src3 << 16) | src2;
#else /* _LITTLE_ENDIAN */
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src0 << 16) | src1;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) =
						    (src2 << 16) | src3;
#endif /* _LITTLE_ENDIAN */
					}
				}
			}
		} else {
			if (!((mlib_addr)pdst_row & 3)) {
				if (!((mlib_addr)psrc_row & 3)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u32 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u32 *)(psrc_row -
						    j * 4 + 2));

						*((mlib_u32 *)(pdst_row +
						    j * 4)) = src0;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) = src1;
					}
				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < sw; j++) {
						mlib_u32 src0 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4));
						mlib_u32 src1 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 1));
						mlib_u32 src2 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 2));
						mlib_u32 src3 =
						    *((mlib_u16 *)(psrc_row -
						    j * 4 + 3));

#ifdef _LITTLE_ENDIAN
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src1 << 16) | src0;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) =
						    (src3 << 16) | src2;
#else /* _LITTLE_ENDIAN */
						*((mlib_u32 *)(pdst_row +
						    j * 4)) =
						    (src0 << 16) | src1;
						*((mlib_u32 *)(pdst_row +
						    j * 4 + 2)) =
						    (src2 << 16) | src3;
#endif /* _LITTLE_ENDIAN */
					}
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < sw; j++) {
					mlib_u16 s0 = psrc_row[-j * 4],
					    s1 = psrc_row[1 - j * 4],
					    s2 = psrc_row[2 - j * 4],
					    s3 = psrc_row[3 - j * 4];

					pdst_row[j * 4] = s0;
					pdst_row[j * 4 + 1] = s1;
					pdst_row[j * 4 + 2] = s2;
					pdst_row[j * 4 + 3] = s3;
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_S32_4(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_u32, 4);

	if (sw < 8) {
		STRIP_4(ddata, sdata, sw, sh, mlib_u32);
		return;
	}

	for (i = 0; i < sh; i++) {
		mlib_u32 *psrc_row = sdata + i * sstride + (sw - 1) * 4,
		    *pdst_row = ddata + i * dstride;

		if (!((mlib_addr)pdst_row & 7) && !((mlib_addr)psrc_row & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				TYPE_64BIT src0 =
				    *((TYPE_64BIT *) (psrc_row - j * 4));
				TYPE_64BIT src1 =
				    *((TYPE_64BIT *) (psrc_row - j * 4 + 2));

				*((TYPE_64BIT *) (pdst_row + j * 4)) = src0;
				*((TYPE_64BIT *) (pdst_row + j * 4 + 2)) = src1;
			}
		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < sw; j++) {
				mlib_u32 src0 =
				    *((mlib_u32 *)(psrc_row - j * 4));
				mlib_u32 src1 =
				    *((mlib_u32 *)(psrc_row - j * 4 + 1));
				mlib_u32 src2 =
				    *((mlib_u32 *)(psrc_row - j * 4 + 2));
				mlib_u32 src3 =
				    *((mlib_u32 *)(psrc_row - j * 4 + 3));

				*((mlib_u32 *)(pdst_row + j * 4)) = src0;
				*((mlib_u32 *)(pdst_row + j * 4 + 1)) = src1;
				*((mlib_u32 *)(pdst_row + j * 4 + 2)) = src2;
				*((mlib_u32 *)(pdst_row + j * 4 + 3)) = src3;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFlipY_D64_4(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	PREPAREVARS(mlib_d64, 4);

	for (i = 0; i < sh; i++) {
		mlib_d64 *psrc_row = sdata + i * sstride + (sw - 1) * 4,
		    *pdst_row = ddata + i * dstride;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < sw; j++) {
			pdst_row[j * 4] = psrc_row[-j * 4];
			pdst_row[j * 4 + 1] = psrc_row[-j * 4 + 1];
			pdst_row[j * 4 + 2] = psrc_row[-j * 4 + 2];
			pdst_row[j * 4 + 3] = psrc_row[-j * 4 + 3];
		}
	}
}

/* *********************************************************** */
