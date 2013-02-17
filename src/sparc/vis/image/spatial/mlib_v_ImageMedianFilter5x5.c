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

#pragma ident	"@(#)mlib_v_ImageMedianFilter5x5.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter5x5    - median filtering with 5x5 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter5x5(mlib_image       *dst,
 *                                            const mlib_image *src,
 *                                            mlib_median_mask mmask,
 *                                            mlib_s32         cmask,
 *                                            mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      mmask   Shape of the mask to be used for median filtering.
 *      cmask   Channel mask to indicate the channels to be filtered.
 *              Each bit of which represents a channel in the image. The
 *              channels corresponded to 1 bits are those to be processed.
 *      edge    Type of edge condition.
 *
 * DESCRIPTION
 *      Median filtering.
 *
 *      There are several shapes possible for the mask: rectangle mask,
 *      plus mask, and X mask.
 *
 *      The separable rectangle median is defined as the median of the
 *      medians of each row.
 *
 *      The unselected channels are not overwritten. If both src and dst have
 *      just one channel, cmask is ignored.
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *      The src and the dst must be images of the same type.
 *      The src and the dst can be images of MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT type.
 *      The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *      The mask shape can be one of the following:
 *              MLIB_MEDIAN_MASK_RECT   (default)
 *              MLIB_MEDIAN_MASK_PLUS
 *              MLIB_MEDIAN_MASK_X
 *              MLIB_MEDIAN_MASK_RECT_SEPARABLE
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageMedianFilter.h>

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

#define	SRC_COPY_u8(sp, dp, n)	mlib_v_ImageDataTypeConvert_U8_S16_D1 \
					((void*)(sp), (void*)(dp), n)
#define	DST_COPY_u8(sp, dp, n)	mlib_v_ImageDataTypeConvert_S16_U8_D1 \
					((void*)(sp), (void*)(dp), n)

/* *********************************************************** */

#define	SRC_COPY_s16(sp, dp, n)	mlib_ImageCopy_na((void*)(sp), (void*)(dp), n)
#define	DST_COPY_s16(sp, dp, n)	mlib_ImageCopy_na((void*)(sp), (void*)(dp), n)

/* *********************************************************** */

#define	SRC_COPY_u16(sp, dp, n)	mlib_ImageLineXor8000((void*)(sp), \
					(void*)(dp), n)
#define	DST_COPY_u16(sp, dp, n)	mlib_ImageLineXor8000((void*)(sp), \
					(void*)(dp), n)

/* *********************************************************** */

#define	LINE_SORT(buff0, buff1)                                      \
	mlib_ImageLineSortMin((void *)buff0, (void *)buff1, xsize)

/* *********************************************************** */

#define	LINE_MAX_(buff0, buff1)                                  \
	mlib_ImageLineMax((void *)buff1, (void *)buff0, xsize)

/* *********************************************************** */

#define	LINE_MIN_(buff0, buff1)                                  \
	mlib_ImageLineMin((void *)buff0, (void *)buff1, xsize)

/* *********************************************************** */

#define	SRC_COPY5(img_type, sp, N)                                \
	SRC_COPY_##img_type((mlib_##img_type *) sp - 2 * nchan,   \
	    buff_arr[N], dsize);                                  \
	SRC_COPY_##img_type((mlib_##img_type *) sp - nchan,       \
	    buff_arr[N + 1], dsize);                              \
	SRC_COPY_##img_type((mlib_##img_type *) sp,               \
	    buff_arr[N + 2], dsize);                              \
	SRC_COPY_##img_type((mlib_##img_type *) sp + nchan,       \
	    buff_arr[N + 3], dsize);                              \
	SRC_COPY_##img_type((mlib_##img_type *) sp + 2 * nchan,   \
	    buff_arr[N + 4], dsize)

/* *********************************************************** */

#define	FUNC_MEDIAN_RECT(img_type)                                     \
	static void                                                    \
	mlib_v_median_row_5x5rect_##img_type(mlib_u8 *dl,              \
	    void **psl, mlib_filter_state * fs)                        \
	{                                                              \
	    void **buff_arr = fs->buff_row, **buff_ptr;                \
	    mlib_s32 nchan = fs->nchan, size = fs->size;               \
	    mlib_s32 xsize = nchan * size;                             \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);         \
	    mlib_s32 bline = (fs->buff_size) << 2;                     \
	    mlib_##img_type *sp;                                       \
	                                                               \
	    if (((fs->buff_ind) & 1) == 0) {                           \
		SRC_COPY5(img_type, psl[1], 0);                        \
		SRC_COPY5(img_type, psl[2], 5);                        \
		SRC_COPY5(img_type, psl[3], 10);                       \
		SRC_COPY5(img_type, psl[4], 15);                       \
		LINE_SORT(buff_arr[0], buff_arr[1]);                   \
		LINE_SORT(buff_arr[3], buff_arr[4]);                   \
		LINE_SORT(buff_arr[2], buff_arr[4]);                   \
		LINE_SORT(buff_arr[2], buff_arr[3]);                   \
		LINE_SORT(buff_arr[6], buff_arr[7]);                   \
		LINE_SORT(buff_arr[5], buff_arr[7]);                   \
		LINE_SORT(buff_arr[5], buff_arr[6]);                   \
		LINE_SORT(buff_arr[9], buff_arr[10]);                  \
		LINE_SORT(buff_arr[8], buff_arr[10]);                  \
		LINE_SORT(buff_arr[8], buff_arr[9]);                   \
		LINE_SORT(buff_arr[12], buff_arr[13]);                 \
		LINE_SORT(buff_arr[11], buff_arr[13]);                 \
		LINE_SORT(buff_arr[11], buff_arr[12]);                 \
		LINE_SORT(buff_arr[15], buff_arr[16]);                 \
		LINE_SORT(buff_arr[14], buff_arr[16]);                 \
		LINE_SORT(buff_arr[14], buff_arr[15]);                 \
		LINE_SORT(buff_arr[18], buff_arr[19]);                 \
		LINE_SORT(buff_arr[17], buff_arr[19]);                 \
		LINE_SORT(buff_arr[17], buff_arr[18]);                 \
		LINE_SORT(buff_arr[2], buff_arr[5]);                   \
		LINE_SORT(buff_arr[3], buff_arr[6]);                   \
		LINE_SORT(buff_arr[0], buff_arr[6]);                   \
		LINE_SORT(buff_arr[0], buff_arr[3]);                   \
		LINE_SORT(buff_arr[4], buff_arr[7]);                   \
		LINE_SORT(buff_arr[1], buff_arr[7]);                   \
		LINE_SORT(buff_arr[1], buff_arr[4]);                   \
		LINE_SORT(buff_arr[11], buff_arr[14]);                 \
		LINE_SORT(buff_arr[8], buff_arr[14]);                  \
		LINE_SORT(buff_arr[8], buff_arr[11]);                  \
		LINE_SORT(buff_arr[12], buff_arr[15]);                 \
		LINE_SORT(buff_arr[9], buff_arr[15]);                  \
		LINE_SORT(buff_arr[9], buff_arr[12]);                  \
		LINE_SORT(buff_arr[13], buff_arr[16]);                 \
		LINE_SORT(buff_arr[10], buff_arr[16]);                 \
		LINE_SORT(buff_arr[10], buff_arr[13]);                 \
		SRC_COPY_s16(buff_arr[0], buff_arr[25], 20 * bline);   \
		buff_ptr = buff_arr;                                   \
		sp = psl[0];                                           \
	    } else {                                                   \
		sp = psl[4];                                           \
/*                                                                     \
 * use buffers copied on previous line                                 \
 */                                                                    \
		buff_ptr = buff_arr + 25;                              \
	    }                                                          \
	    SRC_COPY5(img_type, sp, 20);                               \
	    LINE_SORT(buff_arr[21], buff_arr[22]);                     \
	    LINE_SORT(buff_arr[20], buff_arr[22]);                     \
	    LINE_SORT(buff_arr[20], buff_arr[21]);                     \
	    LINE_SORT(buff_arr[23], buff_arr[24]);                     \
	    LINE_SORT(buff_arr[20], buff_arr[23]);                     \
	    LINE_SORT(buff_ptr[17], buff_arr[23]);                     \
	    LINE_SORT(buff_ptr[17], buff_arr[20]);                     \
	    LINE_SORT(buff_arr[21], buff_arr[24]);                     \
	    LINE_SORT(buff_ptr[18], buff_arr[24]);                     \
	    LINE_SORT(buff_ptr[18], buff_arr[21]);                     \
	    LINE_SORT(buff_ptr[19], buff_arr[22]);                     \
	    LINE_MAX_(buff_ptr[8], buff_ptr[17]);                      \
	    LINE_SORT(buff_ptr[9], buff_ptr[18]);                      \
	    LINE_SORT(buff_ptr[0], buff_ptr[18]);                      \
	    LINE_MAX_(buff_ptr[0], buff_ptr[9]);                       \
	    LINE_SORT(buff_ptr[10], buff_ptr[19]);                     \
	    LINE_SORT(buff_ptr[1], buff_ptr[19]);                      \
	    LINE_SORT(buff_ptr[1], buff_ptr[10]);                      \
	    LINE_SORT(buff_ptr[11], buff_arr[20]);                     \
	    LINE_SORT(buff_ptr[2], buff_arr[20]);                      \
	    LINE_MAX_(buff_ptr[2], buff_ptr[11]);                      \
	    LINE_SORT(buff_ptr[12], buff_arr[21]);                     \
	    LINE_SORT(buff_ptr[3], buff_arr[21]);                      \
	    LINE_SORT(buff_ptr[3], buff_ptr[12]);                      \
	    LINE_SORT(buff_ptr[13], buff_arr[22]);                     \
	    LINE_MIN_(buff_ptr[4], buff_arr[22]);                      \
	    LINE_SORT(buff_ptr[4], buff_ptr[13]);                      \
	    LINE_SORT(buff_ptr[14], buff_arr[23]);                     \
	    LINE_SORT(buff_ptr[5], buff_arr[23]);                      \
	    LINE_SORT(buff_ptr[5], buff_ptr[14]);                      \
	    LINE_SORT(buff_ptr[15], buff_arr[24]);                     \
	    LINE_MIN_(buff_ptr[6], buff_arr[24]);                      \
	    LINE_SORT(buff_ptr[6], buff_ptr[15]);                      \
	    LINE_MIN_(buff_ptr[7], buff_ptr[16]);                      \
	    LINE_MIN_(buff_ptr[7], buff_ptr[19]);                      \
	    LINE_MIN_(buff_ptr[13], buff_arr[21]);                     \
	    LINE_MIN_(buff_ptr[15], buff_arr[23]);                     \
	    LINE_MIN_(buff_ptr[7], buff_ptr[13]);                      \
	    LINE_MIN_(buff_ptr[7], buff_ptr[15]);                      \
	    LINE_MAX_(buff_ptr[1], buff_ptr[9]);                       \
	    LINE_MAX_(buff_ptr[3], buff_ptr[11]);                      \
	    LINE_MAX_(buff_ptr[5], buff_ptr[17]);                      \
	    LINE_MAX_(buff_ptr[11], buff_ptr[17]);                     \
	    LINE_MAX_(buff_ptr[9], buff_ptr[17]);                      \
	    LINE_SORT(buff_ptr[4], buff_ptr[10]);                      \
	    LINE_SORT(buff_ptr[6], buff_ptr[12]);                      \
	    LINE_SORT(buff_ptr[7], buff_ptr[14]);                      \
	    LINE_SORT(buff_ptr[4], buff_ptr[6]);                       \
	    LINE_MAX_(buff_ptr[4], buff_ptr[7]);                       \
	    LINE_SORT(buff_ptr[12], buff_ptr[14]);                     \
	    LINE_MIN_(buff_ptr[10], buff_ptr[14]);                     \
	    LINE_SORT(buff_ptr[6], buff_ptr[7]);                       \
	    LINE_SORT(buff_ptr[10], buff_ptr[12]);                     \
	    LINE_SORT(buff_ptr[6], buff_ptr[10]);                      \
	    LINE_MAX_(buff_ptr[6], buff_ptr[17]);                      \
	    LINE_SORT(buff_ptr[12], buff_ptr[17]);                     \
	    LINE_MIN_(buff_ptr[7], buff_ptr[17]);                      \
	    LINE_SORT(buff_ptr[7], buff_ptr[10]);                      \
	    LINE_SORT(buff_ptr[12], buff_ptr[18]);                     \
	    LINE_MAX_(buff_ptr[7], buff_ptr[12]);                      \
	    LINE_MIN_(buff_ptr[10], buff_ptr[18]);                     \
	    LINE_SORT(buff_ptr[12], buff_arr[20]);                     \
	    LINE_MIN_(buff_ptr[10], buff_arr[20]);                     \
	    LINE_MAX_(buff_ptr[10], buff_ptr[12]);                     \
	    DST_COPY_##img_type(buff_ptr[12], dl, dsize);              \
	    (fs->buff_ind) ^= 1;                                       \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_PLUS(img_type)                                     \
	static void                                                    \
	mlib_v_median_row_5x5plus_##img_type(mlib_u8 *dl,              \
	    void **psl, mlib_filter_state * fs)                        \
	{                                                              \
	    void **buff_arr = fs->buff_row;                            \
	    mlib_s32 nchan = fs->nchan, size = fs->size;               \
	    mlib_s32 xsize = nchan * size;                             \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);         \
	    mlib_##img_type *sp = psl[2];                              \
	                                                               \
	    SRC_COPY_##img_type(psl[0], buff_arr[0], dsize);           \
	    SRC_COPY_##img_type(psl[1], buff_arr[1], dsize);           \
	    SRC_COPY_##img_type(psl[2], buff_arr[2], dsize);           \
	    SRC_COPY_##img_type(psl[3], buff_arr[3], dsize);           \
	    SRC_COPY_##img_type(psl[4], buff_arr[4], dsize);           \
	    SRC_COPY_##img_type(sp - 2 * nchan, buff_arr[5], dsize);   \
	    SRC_COPY_##img_type(sp - nchan, buff_arr[6], dsize);       \
	    SRC_COPY_##img_type(sp + nchan, buff_arr[7], dsize);       \
	    SRC_COPY_##img_type(sp + 2 * nchan, buff_arr[8], dsize);   \
	    LINE_SORT(buff_arr[0], buff_arr[3]);                       \
	    LINE_SORT(buff_arr[1], buff_arr[4]);                       \
	    LINE_SORT(buff_arr[2], buff_arr[5]);                       \
	    LINE_SORT(buff_arr[0], buff_arr[1]);                       \
	    LINE_MAX_(buff_arr[0], buff_arr[2]);                       \
	    LINE_SORT(buff_arr[3], buff_arr[5]);                       \
	    LINE_MIN_(buff_arr[4], buff_arr[5]);                       \
	    LINE_SORT(buff_arr[1], buff_arr[2]);                       \
	    LINE_SORT(buff_arr[3], buff_arr[4]);                       \
	    LINE_SORT(buff_arr[1], buff_arr[6]);                       \
	    LINE_MAX_(buff_arr[1], buff_arr[3]);                       \
	    LINE_SORT(buff_arr[2], buff_arr[6]);                       \
	    LINE_MIN_(buff_arr[4], buff_arr[6]);                       \
	    LINE_SORT(buff_arr[2], buff_arr[3]);                       \
	    LINE_SORT(buff_arr[4], buff_arr[7]);                       \
	    LINE_MAX_(buff_arr[2], buff_arr[4]);                       \
	    LINE_MIN_(buff_arr[3], buff_arr[7]);                       \
	    LINE_SORT(buff_arr[3], buff_arr[4]);                       \
	    LINE_MAX_(buff_arr[3], buff_arr[8]);                       \
	    LINE_MIN_(buff_arr[4], buff_arr[8]);                       \
	    DST_COPY_##img_type(buff_arr[4], dl, dsize);               \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_XMSK(img_type)                                  \
	static void                                                 \
	mlib_v_median_row_5x5xmsk_##img_type(mlib_u8 *dl,           \
	    void **psl, mlib_filter_state * fs)                     \
	{                                                           \
	    void **buff_arr = fs->buff_row;                         \
	    mlib_s32 nchan = fs->nchan, size = fs->size;            \
	    mlib_s32 xsize = nchan * size;                          \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);      \
	    mlib_##img_type *sp0 = psl[0];                          \
	    mlib_##img_type *sp1 = psl[1];                          \
	    mlib_##img_type *sp2 = psl[2];                          \
	    mlib_##img_type *sp3 = psl[3];                          \
	    mlib_##img_type *sp4 = psl[4];                          \
	                                                            \
	    SRC_COPY_##img_type(sp0 - 2 * nchan, buff_arr[0],       \
		dsize);                                             \
	    SRC_COPY_##img_type(sp0 + 2 * nchan, buff_arr[1],       \
		dsize);                                             \
	    SRC_COPY_##img_type(sp1 - nchan, buff_arr[2], dsize);   \
	    SRC_COPY_##img_type(sp1 + nchan, buff_arr[3], dsize);   \
	    SRC_COPY_##img_type(sp2, buff_arr[4], dsize);           \
	    SRC_COPY_##img_type(sp3 - nchan, buff_arr[5], dsize);   \
	    SRC_COPY_##img_type(sp3 + nchan, buff_arr[6], dsize);   \
	    SRC_COPY_##img_type(sp4 - 2 * nchan, buff_arr[7],       \
		dsize);                                             \
	    SRC_COPY_##img_type(sp4 + 2 * nchan, buff_arr[8],       \
		dsize);                                             \
	    LINE_SORT(buff_arr[0], buff_arr[3]);                    \
	    LINE_SORT(buff_arr[1], buff_arr[4]);                    \
	    LINE_SORT(buff_arr[2], buff_arr[5]);                    \
	    LINE_SORT(buff_arr[0], buff_arr[1]);                    \
	    LINE_MAX_(buff_arr[0], buff_arr[2]);                    \
	    LINE_SORT(buff_arr[3], buff_arr[5]);                    \
	    LINE_MIN_(buff_arr[4], buff_arr[5]);                    \
	    LINE_SORT(buff_arr[1], buff_arr[2]);                    \
	    LINE_SORT(buff_arr[3], buff_arr[4]);                    \
	    LINE_SORT(buff_arr[1], buff_arr[6]);                    \
	    LINE_MAX_(buff_arr[1], buff_arr[3]);                    \
	    LINE_SORT(buff_arr[2], buff_arr[6]);                    \
	    LINE_MIN_(buff_arr[4], buff_arr[6]);                    \
	    LINE_SORT(buff_arr[2], buff_arr[3]);                    \
	    LINE_SORT(buff_arr[4], buff_arr[7]);                    \
	    LINE_MAX_(buff_arr[2], buff_arr[4]);                    \
	    LINE_MIN_(buff_arr[3], buff_arr[7]);                    \
	    LINE_SORT(buff_arr[3], buff_arr[4]);                    \
	    LINE_MAX_(buff_arr[3], buff_arr[8]);                    \
	    LINE_MIN_(buff_arr[4], buff_arr[8]);                    \
	    DST_COPY_##img_type(buff_arr[4], dl, dsize);            \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_RSEP(img_type)                                    \
	static void                                                   \
	mlib_v_median_initrow_5x5rsep_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state * fs)                       \
	{                                                             \
	    void **buff_arr = fs->buff_row;                           \
	    mlib_d64 *buff0, *buff1, *buff2, *buff3, *buff4;          \
	    mlib_s32 nchan = fs->nchan, size = fs->size;              \
	    mlib_s32 xsize = nchan * size;                            \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);        \
	    mlib_##img_type *sp = psl[0];                             \
	                                                              \
	    buff2 = buff_arr[fs->buff_ind];                           \
	    buff0 = buff_arr[10];                                     \
	    buff1 = buff_arr[11];                                     \
	    buff3 = buff_arr[12];                                     \
	    buff4 = buff_arr[13];                                     \
	    SRC_COPY_##img_type(sp - 2 * nchan, buff0, dsize);        \
	    SRC_COPY_##img_type(sp - nchan, buff1, dsize);            \
	    SRC_COPY_##img_type(sp, buff2, dsize);                    \
	    SRC_COPY_##img_type(sp + nchan, buff3, dsize);            \
	    SRC_COPY_##img_type(sp + 2 * nchan, buff4, dsize);        \
	    LINE_SORT(buff0, buff1);                                  \
	    LINE_SORT(buff2, buff3);                                  \
	    LINE_SORT(buff2, buff4);                                  \
	    LINE_SORT(buff3, buff4);                                  \
	    LINE_MAX_(buff0, buff2);                                  \
	    LINE_MIN_(buff1, buff4);                                  \
	    LINE_SORT(buff1, buff2);                                  \
	    LINE_MAX_(buff1, buff3);                                  \
	    LINE_MIN_(buff2, buff3);                                  \
	    if (++(fs->buff_ind) >= 9)                                \
		(fs->buff_ind) -= 5;                                  \
	}                                                             \
	static void                                                   \
	mlib_v_median_row_5x5rsep_##img_type(mlib_u8 *dl,             \
	    void **psl, mlib_filter_state * fs)                       \
	{                                                             \
	    void **buff_arr = fs->buff_row;                           \
	    mlib_d64 *buff0, *buff1, *buff2, *buff3, *buff4;          \
	    mlib_s32 nchan = fs->nchan, size = fs->size;              \
	    mlib_s32 xsize = nchan * size;                            \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);        \
	    mlib_s32 csize = xsize * sizeof (mlib_s16);               \
	    mlib_##img_type *sp = psl[4];                             \
	                                                              \
	    buff2 = buff_arr[fs->buff_ind];                           \
	    buff0 = buff_arr[10];                                     \
	    buff1 = buff_arr[11];                                     \
	    buff3 = buff_arr[12];                                     \
	    buff4 = buff_arr[13];                                     \
	    SRC_COPY_##img_type(sp - 2 * nchan, buff0, dsize);        \
	    SRC_COPY_##img_type(sp - nchan, buff1, dsize);            \
	    SRC_COPY_##img_type(sp, buff2, dsize);                    \
	    SRC_COPY_##img_type(sp + nchan, buff3, dsize);            \
	    SRC_COPY_##img_type(sp + 2 * nchan, buff4, dsize);        \
	    LINE_SORT(buff0, buff1);                                  \
	    LINE_SORT(buff2, buff3);                                  \
	    LINE_SORT(buff2, buff4);                                  \
	    LINE_SORT(buff3, buff4);                                  \
	    LINE_MAX_(buff0, buff2);                                  \
	    LINE_MIN_(buff1, buff4);                                  \
	    LINE_SORT(buff1, buff2);                                  \
	    LINE_MAX_(buff1, buff3);                                  \
	    LINE_MIN_(buff2, buff3);                                  \
	    buff2 = buff_arr[fs->buff_ind - 4];                       \
	    SRC_COPY_s16(buff_arr[fs->buff_ind - 3], buff0, csize);   \
	    SRC_COPY_s16(buff_arr[fs->buff_ind - 2], buff1, csize);   \
	    SRC_COPY_s16(buff_arr[fs->buff_ind - 1], buff3, csize);   \
	    SRC_COPY_s16(buff_arr[fs->buff_ind], buff4, csize);       \
	    LINE_SORT(buff0, buff1);                                  \
	    LINE_SORT(buff2, buff3);                                  \
	    LINE_SORT(buff2, buff4);                                  \
	    LINE_SORT(buff3, buff4);                                  \
	    LINE_MAX_(buff0, buff2);                                  \
	    LINE_MIN_(buff1, buff4);                                  \
	    LINE_SORT(buff1, buff2);                                  \
	    LINE_MAX_(buff1, buff3);                                  \
	    LINE_MIN_(buff2, buff3);                                  \
	    DST_COPY_##img_type(buff2, dl, dsize);                    \
	    if (++(fs->buff_ind) >= 9)                                \
		(fs->buff_ind) -= 5;                                  \
	}

/* *********************************************************** */

/* FUNC_MEDIAN_RECT(u8) */
FUNC_MEDIAN_PLUS(u8)
FUNC_MEDIAN_XMSK(u8)
FUNC_MEDIAN_RSEP(u8)

FUNC_MEDIAN_RECT(s16)
FUNC_MEDIAN_PLUS(s16)
FUNC_MEDIAN_XMSK(s16)
FUNC_MEDIAN_RSEP(s16)

FUNC_MEDIAN_RECT(u16)
FUNC_MEDIAN_PLUS(u16)
FUNC_MEDIAN_XMSK(u16)
FUNC_MEDIAN_RSEP(u16)

/* *********************************************************** */

mlib_median_fun_type mlib_v_median_row_5x5_funs[] = {
/* histogram method is faster */
	mlib_median_row_5x5rect_u8,
	mlib_v_median_row_5x5plus_u8,
	mlib_v_median_row_5x5xmsk_u8,
	mlib_v_median_row_5x5rsep_u8,
	mlib_v_median_row_5x5rect_s16,
	mlib_v_median_row_5x5plus_s16,
	mlib_v_median_row_5x5xmsk_s16,
	mlib_v_median_row_5x5rsep_s16,
	mlib_v_median_row_5x5rect_u16,
	mlib_v_median_row_5x5plus_u16,
	mlib_v_median_row_5x5xmsk_u16,
	mlib_v_median_row_5x5rsep_u16
};

/* *********************************************************** */

mlib_median_fun_type mlib_v_median_init_5x5_funs[] = {
	mlib_v_median_initrow_5x5rsep_u8,
	mlib_v_median_initrow_5x5rsep_s16,
	mlib_v_median_initrow_5x5rsep_u16
};

/* *********************************************************** */
