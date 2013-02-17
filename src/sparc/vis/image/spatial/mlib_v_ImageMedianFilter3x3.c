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

#pragma ident	"@(#)mlib_v_ImageMedianFilter3x3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter3x3    - median filtering with 3x3 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter3x3(mlib_image       *dst,
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
					((void *)(sp), (void *)(dp), n)
#define	DST_COPY_u8(sp, dp, n)	mlib_v_ImageDataTypeConvert_S16_U8_D1 \
					((void *)(sp), (void *)(dp), n)

/* *********************************************************** */

#define	SRC_COPY_s16(sp, dp, n)	mlib_ImageCopy_na((void *)(sp), (void *)(dp), n)
#define	DST_COPY_s16(sp, dp, n)	mlib_ImageCopy_na((void *)(sp), (void *)(dp), n)

/* *********************************************************** */

#define	SRC_COPY_u16(sp, dp, n)	mlib_ImageLineXor8000((void *)(sp), \
					(void *)(dp), n)
#define	DST_COPY_u16(sp, dp, n)	mlib_ImageLineXor8000((void *)(sp), \
					(void *)(dp), n)

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

#define	FUNC_MEDIAN_RECT(img_type)                                \
	static void                                               \
	mlib_v_median_row_3x3rect_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state * fs)                   \
	{                                                         \
	    void **buff_arr = fs->buff_row;                       \
	    mlib_d64 *buff0, *buff1, *buff2, *buff3;              \
	    mlib_d64 *buff4, *buff5, *buff6, *buff7, *buff8;      \
	    mlib_s32 nchan = fs->nchan, size = fs->size;          \
	    mlib_s32 xsize = nchan * size;                        \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);    \
	    mlib_s32 bline = (fs->buff_size) << 2;                \
	    mlib_##img_type *sp;                                  \
	                                                          \
	    if (((fs->buff_ind) & 1) == 0) {                      \
		mlib_##img_type *sp0 = psl[1];                    \
		mlib_##img_type *sp1 = psl[2];                    \
	                                                          \
		buff0 = buff_arr[0];                              \
		buff1 = buff_arr[1];                              \
		buff2 = buff_arr[2];                              \
		buff3 = buff_arr[3];                              \
		buff4 = buff_arr[4];                              \
		buff5 = buff_arr[5];                              \
		SRC_COPY_##img_type(sp0 - nchan, buff0, dsize);   \
		SRC_COPY_##img_type(sp0, buff1, dsize);           \
		SRC_COPY_##img_type(sp0 + nchan, buff2, dsize);   \
		SRC_COPY_##img_type(sp1 - nchan, buff3, dsize);   \
		SRC_COPY_##img_type(sp1, buff4, dsize);           \
		SRC_COPY_##img_type(sp1 + nchan, buff5, dsize);   \
		LINE_SORT(buff0, buff3);                          \
		LINE_SORT(buff1, buff4);                          \
		LINE_SORT(buff2, buff5);                          \
		LINE_SORT(buff0, buff1);                          \
		LINE_MAX_(buff0, buff2);                          \
		LINE_SORT(buff3, buff5);                          \
		LINE_MIN_(buff4, buff5);                          \
		LINE_SORT(buff1, buff2);                          \
		LINE_SORT(buff3, buff4);                          \
		SRC_COPY_s16(buff1, buff_arr[7], 4 * bline);      \
		sp = psl[0];                                      \
	    } else {                                              \
		sp = psl[2];                                      \
/*                                                                \
 * use buffers copied on previous line                            \
 */                                                               \
		buff1 = buff_arr[7];                              \
		buff2 = buff_arr[8];                              \
		buff3 = buff_arr[9];                              \
		buff4 = buff_arr[10];                             \
	    }                                                     \
	    buff6 = buff_arr[0];                                  \
	    buff7 = buff_arr[5];                                  \
	    buff8 = buff_arr[6];                                  \
	    SRC_COPY_##img_type(sp - nchan, buff6, dsize);        \
	    SRC_COPY_##img_type(sp, buff7, dsize);                \
	    SRC_COPY_##img_type(sp + nchan, buff8, dsize);        \
	    LINE_SORT(buff1, buff6);                              \
	    LINE_MAX_(buff1, buff3);                              \
	    LINE_SORT(buff2, buff6);                              \
	    LINE_MIN_(buff4, buff6);                              \
	    LINE_SORT(buff2, buff3);                              \
	    LINE_SORT(buff4, buff7);                              \
	    LINE_MAX_(buff2, buff4);                              \
	    LINE_MIN_(buff3, buff7);                              \
	    LINE_SORT(buff3, buff4);                              \
	    LINE_MAX_(buff3, buff8);                              \
	    LINE_MIN_(buff4, buff8);                              \
	    DST_COPY_##img_type(buff4, dl, dsize);                \
	    (fs->buff_ind) ^= 1;                                  \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_PLUS(img_type)                                \
	static void                                               \
	mlib_v_median_row_3x3plus_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state * fs)                   \
	{                                                         \
	    void **buff_arr = fs->buff_row;                       \
	    mlib_d64 *buff0, *buff1, *buff2, *buff3, *buff4;      \
	    mlib_s32 nchan = fs->nchan, size = fs->size;          \
	    mlib_s32 xsize = nchan * size;                        \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);    \
	    mlib_##img_type *sp0 = psl[0];                        \
	    mlib_##img_type *sp1 = psl[1];                        \
	    mlib_##img_type *sp2 = psl[2];                        \
	                                                          \
	    buff0 = buff_arr[0];                                  \
	    buff1 = buff_arr[1];                                  \
	    buff2 = buff_arr[2];                                  \
	    buff3 = buff_arr[3];                                  \
	    buff4 = buff_arr[4];                                  \
	    if (((fs->buff_ind) & 1) == 0) {                      \
		SRC_COPY_##img_type(sp1, buff0, dsize);           \
		SRC_COPY_##img_type(sp2, buff1, dsize);           \
		LINE_SORT(buff0, buff1);                          \
		SRC_COPY_##img_type(sp0, buff2, dsize);           \
		SRC_COPY_##img_type(sp1 - nchan, buff3, dsize);   \
		SRC_COPY_##img_type(sp1 + nchan, buff4, dsize);   \
	    } else {                                              \
	                                                          \
/*                                                                \
 * buff0 & buff1 loaded and sorted on previous line               \
 */                                                               \
		SRC_COPY_##img_type(sp2, buff2, dsize);           \
		SRC_COPY_##img_type(sp1 - nchan, buff3, dsize);   \
		SRC_COPY_##img_type(sp1 + nchan, buff4, dsize);   \
	    }                                                     \
	    LINE_SORT(buff2, buff3);                              \
	    LINE_SORT(buff2, buff4);                              \
	    LINE_SORT(buff3, buff4);                              \
	    LINE_MAX_(buff0, buff2);                              \
	    LINE_MIN_(buff4, buff1);                              \
	    LINE_SORT(buff4, buff2);                              \
	    LINE_MAX_(buff4, buff3);                              \
	    LINE_MIN_(buff2, buff3);                              \
	    DST_COPY_##img_type(buff2, dl, dsize);                \
	    (fs->buff_ind) ^= 1;                                  \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_XMSK(img_type)                               \
	static void                                              \
	mlib_v_median_row_3x3xmsk_##img_type(mlib_u8 *dl,        \
	    void **psl, mlib_filter_state * fs)                  \
	{                                                        \
	    void **buff_arr = fs->buff_row;                      \
	    mlib_d64 *buff0, *buff1, *buff2, *buff3, *buff4;     \
	    mlib_s32 nchan = fs->nchan, size = fs->size;         \
	    mlib_s32 xsize = nchan * size;                       \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);   \
	    mlib_##img_type *sp0 = psl[0];                       \
	    mlib_##img_type *sp1 = psl[1];                       \
	    mlib_##img_type *sp2 = psl[2];                       \
	                                                         \
	    buff0 = buff_arr[0];                                 \
	    buff1 = buff_arr[1];                                 \
	    buff2 = buff_arr[2];                                 \
	    buff3 = buff_arr[3];                                 \
	    buff4 = buff_arr[4];                                 \
	    SRC_COPY_##img_type(sp0 - nchan, buff0, dsize);      \
	    SRC_COPY_##img_type(sp0 + nchan, buff1, dsize);      \
	    SRC_COPY_##img_type(sp1, buff2, dsize);              \
	    SRC_COPY_##img_type(sp2 - nchan, buff3, dsize);      \
	    SRC_COPY_##img_type(sp2 + nchan, buff4, dsize);      \
	    LINE_SORT(buff0, buff1);                             \
	    LINE_SORT(buff2, buff3);                             \
	    LINE_SORT(buff2, buff4);                             \
	    LINE_SORT(buff3, buff4);                             \
	    LINE_MAX_(buff0, buff2);                             \
	    LINE_MIN_(buff1, buff4);                             \
	    LINE_SORT(buff1, buff2);                             \
	    LINE_MAX_(buff1, buff3);                             \
	    LINE_MIN_(buff2, buff3);                             \
	    DST_COPY_##img_type(buff2, dl, dsize);               \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_RSEP(img_type)                                        \
	static void                                                       \
	mlib_v_median_initrow_3x3rsep_##img_type(mlib_u8 *dl,             \
	    void **psl, mlib_filter_state * fs)                           \
	{                                                                 \
	    mlib_d64 *buff3, *buff4, *buff5;                              \
	    mlib_s32 nchan = fs->nchan, size = fs->size;                  \
	    mlib_s32 xsize = nchan * size;                                \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);            \
	    mlib_##img_type *sp = psl[0];                                 \
	                                                                  \
	    buff3 = (fs->buff_row)[fs->buff_ind];                         \
	    buff4 = (fs->buff_row)[6];                                    \
	    buff5 = (fs->buff_row)[7];                                    \
	    SRC_COPY_##img_type(sp - nchan, buff3, dsize);                \
	    SRC_COPY_##img_type(sp, buff4, dsize);                        \
	    SRC_COPY_##img_type(sp + nchan, buff5, dsize);                \
	    mlib_ImageLineSortMin((void *)buff3, (void *)buff4, xsize);   \
	    mlib_ImageLineMax((void *)buff3, (void *)buff5, xsize);       \
	    mlib_ImageLineMin((void *)buff3, (void *)buff4, xsize);       \
	    if (++(fs->buff_ind) >= 5)                                    \
		(fs->buff_ind) -= 3;                                      \
	}                                                                 \
	static void                                                       \
	mlib_v_median_row_3x3rsep_##img_type(mlib_u8 *dl,                 \
	    void **psl, mlib_filter_state * fs)                           \
	{                                                                 \
	    mlib_d64 *buff1, *buff2, *buff3, *buff4, *buff5, *buff0;      \
	    mlib_s32 nchan = fs->nchan, size = fs->size;                  \
	    mlib_s32 xsize = nchan * size;                                \
	    mlib_s32 dsize = xsize * sizeof (mlib_##img_type);            \
	    mlib_##img_type *sp = psl[2];                                 \
	                                                                  \
	    buff1 = (fs->buff_row)[fs->buff_ind - 2];                     \
	    buff2 = (fs->buff_row)[fs->buff_ind - 1];                     \
	    buff3 = (fs->buff_row)[fs->buff_ind];                         \
	    buff4 = (fs->buff_row)[6];                                    \
	    buff5 = (fs->buff_row)[7];                                    \
	    buff0 = (fs->buff_row)[8];                                    \
	    SRC_COPY_##img_type(sp - nchan, buff3, dsize);                \
	    SRC_COPY_##img_type(sp, buff4, dsize);                        \
	    SRC_COPY_##img_type(sp + nchan, buff5, dsize);                \
	    mlib_ImageLineSortMin((void *)buff3, (void *)buff4, xsize);   \
	    mlib_ImageLineMax((void *)buff3, (void *)buff5, xsize);       \
	    mlib_ImageLineMin((void *)buff3, (void *)buff4, xsize);       \
	    if (((fs->cmask) & 0x10) == 0) {                              \
		SRC_COPY_s16(buff3, buff0, xsize * sizeof (mlib_s16));    \
		mlib_ImageLineSortMin((void *)buff2, (void *)buff0,       \
		    xsize);                                               \
		mlib_ImageLineMax((void *)buff1, (void *)buff2, xsize);   \
		mlib_ImageLineMin((void *)buff1, (void *)buff0, xsize);   \
		DST_COPY_##img_type(buff1, dl, dsize);                    \
	    } else {                                                      \
	                                                                  \
/*                                                                        \
 * buff1 & buff0 sorted on previous line (was buff2 & buff0)              \
 */                                                                       \
		SRC_COPY_s16(buff3, buff4, xsize * sizeof (mlib_s16));    \
		mlib_ImageLineMax((void *)buff4, (void *)buff1, xsize);   \
		mlib_ImageLineMin((void *)buff4, (void *)buff0, xsize);   \
		DST_COPY_##img_type(buff4, dl, dsize);                    \
	    }                                                             \
	    if (++(fs->buff_ind) >= 5)                                    \
		(fs->buff_ind) -= 3;                                      \
	    (fs->cmask) ^= 0x10;                                          \
	}

/* *********************************************************** */

FUNC_MEDIAN_RECT(u8)
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
mlib_median_fun_type mlib_v_median_row_3x3_funs[] = {
	mlib_median_row_3x3rect_u8,
	mlib_v_median_row_3x3plus_u8,
	mlib_v_median_row_3x3xmsk_u8,
	mlib_v_median_row_3x3rsep_u8,
	mlib_v_median_row_3x3rect_s16,
	mlib_v_median_row_3x3plus_s16,
	mlib_v_median_row_3x3xmsk_s16,
	mlib_v_median_row_3x3rsep_s16,
	mlib_v_median_row_3x3rect_u16,
	mlib_v_median_row_3x3plus_u16,
	mlib_v_median_row_3x3xmsk_u16,
	mlib_v_median_row_3x3rsep_u16
};

/* *********************************************************** */

mlib_median_fun_type mlib_v_median_init_3x3_funs[] = {
	mlib_v_median_initrow_3x3rsep_u8,
	mlib_v_median_initrow_3x3rsep_s16,
	mlib_v_median_initrow_3x3rsep_u16
};

/* *********************************************************** */
