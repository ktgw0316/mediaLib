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

#pragma ident	"@(#)mlib_ImageInterpTable.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageInterpTableCreate     - create an interpolation table
 *      mlib_ImageInterpTableDelete     - delete an interpolation table
 *
 * SYNOPSIS
 *      void *mlib_ImageInterpTableCreate(mlib_type  type,
 *                                        mlib_s32   width,
 *                                        mlib_s32   height,
 *                                        mlib_s32   leftPadding,
 *                                        mlib_s32   topPadding,
 *                                        mlib_s32   subsampleBitsH,
 *                                        mlib_s32   subsampleBitsV,
 *                                        mlib_s32   precisionBits,
 *                                        const void *dataH,
 *                                        const void *dataV)
 *
 *      void mlib_ImageInterpTableDelete(void *table)
 *
 * ARGUMENTS
 *        type               Data type of the coefficient
 *        width              The width of the interpolation kernel in pixels.
 *        height             The height of the interpolation kernel in pixels.
 *        leftPadding        The number of pixels lying to the left
 *                           of the interpolation kernel key position.
 *        topPadding         The number of pixels lying above the
 *                           interpolation kernel key position.
 *        subsampleBitsH     The numbers of bits used for the
 *                           horizontal subsample position.
 *        subsampleBitsV     The numbers of bits used for the
 *                           vertical subsample position.
 *        precisionBits      The number of fractional bits used to
 *                           describe the coefficients.
 *        dataH              The horizontal coefficient data.
 *        dataV              The vertical coefficient data.
 *        table              Pointer to interpolation table structure
 *
 * DESCRIPTION
 *      mlib_ImageInterpTableCreate() creates an internal data structure, an
 *      interpolation table, which can be used by some image geometric
 *      functions for implementing a table-driven interpolation algorithm.
 *
 *      The parameter type defines the type of dataH/dataV input arrays and
 *      can be MLIB_BYTE, MLIB_SHORT, MLIB_INT, MLIB_FLOAT, or MLIB_DOUBLE.
 *
 *      The dataH array should have at least width*2^subsampleBitsH entries.
 *      dataH[i*2^subsampleBitsH] holds the coefficient for the leftmost
 *      neighboring pixel, dataH[i*2^subsampleBitsH+1] holds the the
 *      coefficient for the second neighboring pixel from left, ..., and
 *      dataH[i*2^subsampleBitsH+width-1] holds the coefficient for the
 *      rightmost neighboring pixel, where i = 0, 1, 2, ..., 2^subsampleBitsH-1.
 *
 *      The dataV array should have at least height*2^subsampleBitsV entries
 *      or should be NULL.  If dataV is NULL, then dataH is used in its place,
 *      and in this case the parameters topPadding, height, and subsampleBitsV
 *      are ignored.
 *
 *      mlib_ImageInterpTableDelete() deletes the interpolation table
 *      structure.
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageInterpTableCreate = __mlib_ImageInterpTableCreate
#pragma weak mlib_ImageInterpTableDelete = __mlib_ImageInterpTableDelete
#pragma weak mlib_ImageInitInterpTable_S16 = __mlib_ImageInitInterpTable_S16
#pragma weak mlib_ImageInitInterpTableAffine_S16 = \
	__mlib_ImageInitInterpTableAffine_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageInterpTableCreate) mlib_ImageInterpTableCreate
    __attribute__((weak, alias("__mlib_ImageInterpTableCreate")));
__typeof__(__mlib_ImageInterpTableDelete) mlib_ImageInterpTableDelete
    __attribute__((weak, alias("__mlib_ImageInterpTableDelete")));
__typeof__(__mlib_ImageInitInterpTable_S16) mlib_ImageInitInterpTable_S16
    __attribute__((weak, alias("__mlib_ImageInitInterpTable_S16")));
__typeof__(__mlib_ImageInitInterpTableAffine_S16)
    mlib_ImageInitInterpTableAffine_S16
    __attribute__((weak, alias("__mlib_ImageInitInterpTableAffine_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#else

#define	__mlib_ImageInitInterpTable_S16 \
	mlib_ImageInitInterpTable_S16
#define	__mlib_ImageInitInterpTableAffine_S16 \
	mlib_ImageInitInterpTableAffine_S16

#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_ImageInitInterpTable(
    mlib_interp_table *table,
    mlib_type dtype);

/* *********************************************************** */

#define	COPY_DATA(DTYPE, STYPE, tbl_size, ksize, kbits, data)            \
	tbl = __mlib_malloc(tbl_size * (1 << kbits) * sizeof (DTYPE));   \
	if (tbl != NULL) {                                               \
	    for (j = 0; j < ksize; j++) {                                \
		for (i = 0; i < tbl_size; i++) {                         \
		    ((DTYPE *) tbl)[j + (i << kbits)] =                  \
			((STYPE *) data)[j + i * ksize] * scale;         \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	INIT_TABLE(TBL, DTYPE, tbl_size, ksize, kbits, data_d64)        \
	if (TBL == NULL) {                                              \
	    TBL =                                                       \
		__mlib_malloc(num_copy * tbl_size * (1 << kbits) *      \
		sizeof (DTYPE));                                        \
	    if (TBL == NULL)                                            \
		return (MLIB_FAILURE);                                  \
	    for (j = 0; j < ksize; j++) {                               \
		DTYPE *tbl = (DTYPE *) (TBL) + j * num_copy;            \
	                                                                \
		for (i = 0; i < tbl_size; i++) {                        \
		    mlib_d64 v = data_d64[j + (i << kbits)] * dscale;   \
	                                                                \
		    for (c = 0; c < num_copy; c++) {                    \
			tbl[num_copy * (i << kbits) + c] = (DTYPE) v;   \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	INIT_TABLE_16(TBL, tbl_size, ksize, kbits, kbits_vis,           \
	data_d64)                                                       \
	if ((ksize & 1) == 0) {                                         \
	    for (j = 0; j < ksize; j++) {                               \
		mlib_s16 *tbl = (mlib_s16 *)(TBL) + j * num_copy;       \
	                                                                \
		for (i = 0; i < tbl_size; i++) {                        \
		    mlib_d64 v = data_d64[j + (i << kbits)] * dscale;   \
	                                                                \
		    for (c = 0; c < num_copy; c++) {                    \
			tbl[num_copy * (i << kbits_vis) + c] =          \
			    (mlib_s16)v;                                \
		    }                                                   \
		}                                                       \
	    }                                                           \
	} else {                                                        \
	    mlib_s32 ksize_tmp = ((ksize - 2) & ~3) + 3;                \
	    mlib_s16 *tbl;                                              \
	                                                                \
	    for (j = 0; j < ksize_tmp; j++) {                           \
		tbl = (mlib_s16 *)(TBL) + j * num_copy;                 \
		for (i = 0; i < tbl_size; i++) {                        \
		    mlib_d64 v = data_d64[j + (i << kbits)] * dscale;   \
	                                                                \
		    for (c = 0; c < num_copy; c++) {                    \
			tbl[num_copy * (i << kbits_vis) + c] =          \
			    (mlib_s16)v;                                \
		    }                                                   \
		}                                                       \
	    }                                                           \
	    tbl = (mlib_s16 *)(TBL) + j * num_copy;                     \
	    for (i = 0; i < tbl_size; i++) {                            \
		for (c = 0; c < num_copy; c++) {                        \
		    tbl[num_copy * (i << kbits_vis) + c] = 0;           \
		}                                                       \
	    }                                                           \
	    for (; j < ksize; j++) {                                    \
		tbl = (mlib_s16 *)(TBL) + (j + 1) * num_copy;           \
		for (i = 0; i < tbl_size; i++) {                        \
		    mlib_d64 v = data_d64[j + (i << kbits)] * dscale;   \
	                                                                \
		    for (c = 0; c < num_copy; c++) {                    \
			tbl[num_copy * (i << kbits_vis) + c] =          \
			    (mlib_s16)v;                                \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	CALC_MAX_SUM(max_s, tbl_size, ksize, kbits, data_d64)   \
	max_s = 0;                                              \
	for (i = 0; i < tbl_size; i++) {                        \
	    mlib_d64 s = 0;                                     \
	                                                        \
	    for (j = 0; j < ksize; j++) {                       \
		s += data_d64[j + (i << kbits)];                \
	    }                                                   \
	    if (s > max_s)                                      \
		max_s = s;                                      \
	}

/* *********************************************************** */

#define	CALC_BITS(max_bits, tbl_size, ksize, kbits, data_d64)    \
	{                                                        \
	    mlib_d64 max_s = 0;                                  \
	    mlib_d64 max = 0;                                    \
	    mlib_d64 sum = 0;                                    \
	    mlib_d64 d;                                          \
	                                                         \
	    for (i = 0; i < tbl_size; i++) {                     \
		mlib_d64 s;                                      \
		mlib_d64 sum_pos = 0;                            \
		mlib_d64 sum_neg = 0;                            \
	                                                         \
		for (j = 0; j < ksize; j++) {                    \
		    d = mlib_fabs(data_d64[j + (i << kbits)]);   \
		    if (data_d64[j + (i << kbits)] > 0) {        \
			sum_pos += d;                            \
		    } else                                       \
			sum_neg += d;                            \
		    max = (max > d) ? max : d;                   \
		}                                                \
		s = sum_pos + sum_neg;                           \
		max_s = (max_s > s) ? max_s : s;                 \
		sum = (sum > sum_pos) ? sum : sum_pos;           \
		sum = (sum > sum_neg) ? sum : sum_neg;           \
	    }                                                    \
	    max = (max > sum) ? max : sum;                       \
	    max *= 2;                                            \
	    max = (max > max_s) ? max : max_s;                   \
	    max_bits = mlib_ilogb(max);                          \
	}

/* *********************************************************** */

#define	CALC_MAX_SUM_X(max_s, tbl_size, ksize, kbits, data_d64)   \
	{                                                         \
	    mlib_d64 d;                                           \
	                                                          \
	    max_s = 0;                                            \
	    for (i = 0; i < tbl_size; i++) {                      \
		mlib_d64 sum_pos = 0;                             \
		mlib_d64 sum_neg = 0;                             \
	                                                          \
		for (j = 0; j < ksize; j++) {                     \
		    d = mlib_fabs(data_d64[j + (i << kbits)]);    \
		    if (data_d64[j + (i << kbits)] > 0) {         \
			sum_pos += d;                             \
		    } else                                        \
			sum_neg += d;                             \
		}                                                 \
		max_s = (max_s > sum_neg) ? max_s : sum_neg;      \
		max_s = (max_s > sum_pos) ? max_s : sum_pos;      \
	    }                                                     \
	    max_s *= 2;                                           \
	}

/* *********************************************************** */

#define	CALC_MAX_SUM_Y(max_s, tbl_size, ksize, kbits, data_d64)   \
	{                                                         \
	    mlib_d64 max = 0;                                     \
	    mlib_d64 d;                                           \
	                                                          \
	    max_s = 0;                                            \
	    for (i = 0; i < tbl_size; i++) {                      \
		mlib_d64 s = 0;                                   \
	                                                          \
		for (j = 0; j < ksize; j++) {                     \
		    d = mlib_fabs(data_d64[j + (i << kbits)]);    \
		    s += d;                                       \
		    max = (max > d) ? max : d;                    \
		}                                                 \
		max_s = (max_s > s) ? max_s : s;                  \
	    }                                                     \
	    max *= 2;                                             \
	    max_s = (max > max_s) ? max : max_s;                  \
	}

/* *********************************************************** */

static void *
mlib_CopyData(
    mlib_type stype,
    mlib_s32 ss_bits,
    mlib_s32 ksize,
    mlib_s32 kbits,
    mlib_s32 precisionBits,
    const void *data)
{
	void *tbl = NULL;
	mlib_s32 tbl_size = (1 << ss_bits), i, j;
	mlib_d64 scale = 1.0 / (1u << precisionBits);

	if (!data)
		return (NULL);

	switch (stype) {
	case MLIB_BYTE:
		COPY_DATA(mlib_d64,
		    mlib_u8,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	case MLIB_SHORT:
		COPY_DATA(mlib_d64,
		    mlib_s16,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	case MLIB_USHORT:
		COPY_DATA(mlib_d64,
		    mlib_u16,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	case MLIB_INT:
		COPY_DATA(mlib_d64,
		    mlib_s32,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	case MLIB_FLOAT:
		COPY_DATA(mlib_d64,
		    mlib_f32,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	case MLIB_DOUBLE:
		COPY_DATA(mlib_d64,
		    mlib_d64,
		    tbl_size,
		    ksize,
		    kbits,
		    data);

		break;
	default:
		return (NULL);
	}

	return (tbl);
}

/* *********************************************************** */

mlib_status
mlib_ImageInitInterpTable(
    mlib_interp_table *table,
    mlib_type dtype)
{
	mlib_s32 width, height, width_bits, height_bits;
	mlib_s32 subsampleBitsH, subsampleBitsV;
	mlib_s32 i, j, c, num_copy = 1;
	mlib_d64 dscale = 1.0, *dataH, *dataV;

	if (!table)
		return (MLIB_FAILURE);

	dataH = mlib_ImageGetInterpDoubleDataH(table);
	dataV = mlib_ImageGetInterpDoubleDataV(table);

	if (!dataH || !dataV)
		return (MLIB_FAILURE);

	width = mlib_ImageGetInterpWidth(table);
	height = mlib_ImageGetInterpHeight(table);
	width_bits = mlib_ImageGetInterpWidthBits(table);
	height_bits = mlib_ImageGetInterpHeightBits(table);
	subsampleBitsH = mlib_ImageGetInterpSubsampleBitsH(table);
	subsampleBitsV = mlib_ImageGetInterpSubsampleBitsV(table);

	switch (dtype) {
	case MLIB_FLOAT:
		INIT_TABLE(table->dataH_f32, mlib_f32,
		    (1 << subsampleBitsH),
		    width,
		    width_bits,
		    dataH);

		if (dataV == dataH) {
			table->dataV_f32 = mlib_ImageGetInterpFloatDataH(table);
		} else {
			INIT_TABLE(table->dataV_f32, mlib_f32,
			    (1 << subsampleBitsV),
			    height,
			    height_bits,
			    dataV);
		}

		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageInitInterpTableAffine_S16(
    mlib_interp_table *table,
    mlib_s32 nchan)
{
	mlib_s32 width, height, width_bits, height_bits, vis_width_bits,
	    vis_height_bits;
	mlib_s32 subsampleBitsH, subsampleBitsV;
	mlib_s32 i, j, c, max_xbits, max_ybits, shift, num_copy, num_copy_old;
	mlib_d64 dscale, *dataH, *dataV;
	mlib_d64 **ptr_tablex, *tablex, *tablex_old, *tabley;

	if (!table)
		return (MLIB_FAILURE);

	if (table->shift_vis_affine < 0)
		return (MLIB_FAILURE);

	if (nchan == 1 || nchan == 2) {
		num_copy = 1;
		ptr_tablex = &(table->dataH_s16_1);
	} else if (nchan == 3) {
		num_copy = 3;
		ptr_tablex = &(table->dataH_s16_3);
	} else if (nchan == 4) {
		num_copy = 4;
		ptr_tablex = &(table->dataH_s16_4);
	} else
		return (MLIB_FAILURE);

	if (*ptr_tablex != NULL && table->dataV_s16_1 != NULL)
		return (MLIB_SUCCESS);

	dataH = mlib_ImageGetInterpDoubleDataH(table);
	dataV = mlib_ImageGetInterpDoubleDataV(table);

	if (!dataH || !dataV)
		return (MLIB_FAILURE);

	width = mlib_ImageGetInterpWidth(table);
	height = mlib_ImageGetInterpHeight(table);
	width_bits = mlib_ImageGetInterpWidthBits(table);
	height_bits = mlib_ImageGetInterpHeightBits(table);
	vis_width_bits = table->vis_width_bits;
	vis_height_bits = table->vis_height_bits;
	subsampleBitsH = mlib_ImageGetInterpSubsampleBitsH(table);
	subsampleBitsV = mlib_ImageGetInterpSubsampleBitsV(table);

	if (table->dataV_s16_1 != NULL) {
		if (table->dataH_s16_1 != NULL) {
			tablex_old = table->dataH_s16_1;
			num_copy_old = 1;
		} else if (table->dataH_s16_3 != NULL) {
			tablex_old = table->dataH_s16_3;
			num_copy_old = 3;
		} else {
			tablex_old = table->dataH_s16_4;
			num_copy_old = 4;
		}

		tablex =
		    __mlib_malloc(num_copy * (1 << subsampleBitsH) *
		    (1 << vis_width_bits) * sizeof (mlib_s16));

		if (tablex == NULL)
			return (MLIB_FAILURE);

		for (j = 0; j < ((width + 1) & ~1); j++) {
			mlib_s16 *tbl = (mlib_s16 *)tablex + j * num_copy;
			mlib_s16 *tbl_old =
			    (mlib_s16 *)tablex_old + j * num_copy_old;
			for (i = 0; i < (1 << subsampleBitsH); i++) {
				mlib_s16 v =
				    tbl_old[num_copy_old *
				    (i << vis_width_bits)];
				for (c = 0; c < num_copy; c++) {
					tbl[num_copy * (i << vis_width_bits) +
					    c] = v;
				}
			}
		}

		*ptr_tablex = tablex;
		return (MLIB_SUCCESS);
	}

	CALC_BITS(max_xbits, (1 << subsampleBitsH), width, width_bits, dataH);

	if (dataH != dataV) {
		CALC_BITS(max_ybits, (1 << subsampleBitsV), height, height_bits,
		    dataV);
	} else
		max_ybits = max_xbits;

	shift = ((15 - max_xbits) - 8) + ((15 - max_ybits) - 16);

	if (shift < 0) {
		table->shift_vis_affine = shift;
		return (MLIB_FAILURE);
	}

	if (shift > 7) {
		max_xbits += ((shift - 6) >> 1);
		max_ybits = -1 - max_xbits;
		shift = 7;
	}

/* X table */

	if (max_xbits < 16) {
		dscale = (1 << (15 - max_xbits));
	} else {
		dscale = 1.0 / (1 << (max_xbits - 15));
	}

	tablex =
	    __mlib_malloc(num_copy * (1 << subsampleBitsH) *
	    (1 << vis_width_bits) * sizeof (mlib_s16));

	if (tablex == NULL)
		return (MLIB_FAILURE);

	INIT_TABLE_16(tablex, (1 << subsampleBitsH), width, width_bits,
	    vis_width_bits, dataH);

	if ((dataH == dataV) && num_copy == 1)
		tabley = tablex;
	else {
/* Y table */

		if (max_ybits < 16) {
			dscale = (1 << (15 - max_ybits));
		} else {
			dscale = 1.0 / (1 << (max_ybits - 15));
		}

		num_copy = 1;

		tabley =
		    __mlib_malloc(num_copy * (1 << subsampleBitsV) *
		    (1 << vis_height_bits) * sizeof (mlib_s16));

		if (tabley == NULL) {
			__mlib_free(tablex);
			return (MLIB_FAILURE);
		}

		INIT_TABLE_16(tabley, (1 << subsampleBitsV), height,
		    height_bits, vis_height_bits, dataV);
	}

	*ptr_tablex = tablex;
	table->dataV_s16_1 = tabley;

/* Store shift */
	table->shift_vis_affine = shift;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageInitInterpTable_S16(
    mlib_interp_table *table)
{
	mlib_s32 width, height, width_bits, height_bits;
	mlib_s32 subsampleBitsH, subsampleBitsV;
	mlib_s32 i, j, c, max_xbits, max_ybits, shift, num_copy;
	mlib_d64 dscale, max_xf, max_yf, *dataH, *dataV;

	if (!table)
		return (MLIB_FAILURE);

	if (table->dataH_s16 != NULL && table->dataV_s16 != NULL)
		return (MLIB_SUCCESS);

	dataH = mlib_ImageGetInterpDoubleDataH(table);
	dataV = mlib_ImageGetInterpDoubleDataV(table);

	if (!dataH || !dataV)
		return (MLIB_FAILURE);

	width = mlib_ImageGetInterpWidth(table);
	height = mlib_ImageGetInterpHeight(table);
	width_bits = mlib_ImageGetInterpWidthBits(table);
	height_bits = mlib_ImageGetInterpHeightBits(table);
	subsampleBitsH = mlib_ImageGetInterpSubsampleBitsH(table);
	subsampleBitsV = mlib_ImageGetInterpSubsampleBitsV(table);

	if (dataV == NULL)
		dataV = dataH;
	CALC_MAX_SUM_X(max_xf, (1 << subsampleBitsH), width, width_bits, dataH);
	CALC_MAX_SUM_Y(max_yf, (1 << subsampleBitsV), height, height_bits,
	    dataV);

	max_xbits = mlib_ilogb(max_xf);
	max_ybits = mlib_ilogb(max_yf);

	shift = ((15 - max_xbits) - 8) + ((15 - max_ybits) - 16);

	if (shift < 0) {
		table->shift_vis = shift;
		return (MLIB_FAILURE);
	}

	if (shift > 7) {
		max_xbits += ((shift - 6) >> 1);
		max_ybits = -1 - max_xbits;
		shift = 7;
	}

/* X table */

	if (max_xbits < 16) {
		dscale = (1 << (15 - max_xbits));
	} else {
		dscale = 1.0 / (1 << (max_xbits - 15));
	}

	num_copy = 1;
	INIT_TABLE(table->dataH_s16, mlib_s16,
	    (1 << subsampleBitsH),
	    width,
	    width_bits,
	    dataH);

/* Y table */

	if (max_ybits < 16) {
		dscale = (1 << (15 - max_ybits));
	} else {
		dscale = 1.0 / (1 << (max_ybits - 15));
	}

	num_copy = 4;
	INIT_TABLE(table->dataV_s16, mlib_s16,
	    (1 << subsampleBitsV),
	    height,
	    height_bits,
	    dataV);

/* Store shift */
	table->shift_vis = shift;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void *
__mlib_ImageInterpTableCreate(
    mlib_type type,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 leftPadding,
    mlib_s32 topPadding,
    mlib_s32 subsampleBitsH,
    mlib_s32 subsampleBitsV,
    mlib_s32 precisionBits,
    const void *dataH,
    const void *dataV)
{
	mlib_interp_table *table;
	mlib_s32 width_bits = 0, height_bits = 0, a;
	mlib_status stat;

	if (leftPadding < 0 || leftPadding > width - 2 ||
	    topPadding < 0 || topPadding > height - 2 ||
	    precisionBits < 0 || precisionBits >= 31) {
		return (NULL);
	}

	if (type == MLIB_FLOAT || type == MLIB_DOUBLE)
		precisionBits = 0;
	else if (type != MLIB_BYTE && type != MLIB_SHORT &&
		type != MLIB_USHORT && type != MLIB_INT)
		return (NULL);

	for (a = width - 1; a; a >>= 1)
		width_bits++;

	table = __mlib_malloc(sizeof (mlib_interp_table));

	if (table == NULL)
		return (NULL);

	table->width = width;
	table->width_bits = width_bits;
	table->vis_width_bits = width_bits;

	if (width_bits == 1)
		(table->vis_width_bits)++;
	table->leftPadding = leftPadding;
	table->subsampleBitsH = subsampleBitsH;
	table->precisionBits = precisionBits;

	table->dataH_d64 =
	    mlib_CopyData(type, subsampleBitsH, width, width_bits,
	    precisionBits, dataH);

	if (table->dataH_d64 == NULL) {
		__mlib_ImageInterpTableDelete(table);
		return (NULL);
	}

	if (dataV == NULL) {
		table->dataV_d64 = table->dataH_d64;
		table->height = width;
		table->height_bits = width_bits;
		table->vis_height_bits = table->vis_width_bits;
		table->topPadding = leftPadding;
		table->subsampleBitsV = subsampleBitsH;
	} else {
		for (a = height - 1; a; a >>= 1)
			height_bits++;

		table->height = height;
		table->height_bits = height_bits;
		table->vis_height_bits = height_bits;
		table->topPadding = topPadding;
		table->subsampleBitsV = subsampleBitsV;
		table->dataV_d64 =
		    mlib_CopyData(type, subsampleBitsV, height, height_bits,
		    precisionBits, dataV);

		if (table->dataV_d64 == NULL) {
			__mlib_ImageInterpTableDelete(table);
			return (NULL);
		}
	}

	table->dataH_f32 = NULL;
	table->dataV_f32 = NULL;
	table->dataH_s16 = NULL;
	table->dataV_s16 = NULL;
	table->dataH_s16_1 = NULL;
	table->dataV_s16_1 = NULL;
	table->dataH_s16_3 = NULL;
	table->dataH_s16_4 = NULL;

/* calc float table */
	stat = mlib_ImageInitInterpTable(table, MLIB_FLOAT);

	if (stat == MLIB_FAILURE) {
		__mlib_ImageInterpTableDelete(table);
		return (NULL);
	}

/* table for VIS */
	table->shift_vis = -1;
	table->shift_vis_affine = 0;
/* mlib_ImageInitInterpTable_S16(table); */

	return (table);
}

/* *********************************************************** */

void
__mlib_ImageInterpTableDelete(
    void *table)
{
	mlib_interp_table *tbl = (mlib_interp_table *) table;

	if (tbl == NULL)
		return;

	if (tbl->dataH_s16_1 != NULL) {
		__mlib_free(tbl->dataH_s16_1);
	}

	if (tbl->dataV_s16_1 != NULL && tbl->dataV_s16_1 != tbl->dataH_s16_1) {
		__mlib_free(tbl->dataV_s16_1);
	}

	if (tbl->dataH_s16_3 != NULL) {
		__mlib_free(tbl->dataH_s16_3);
	}

	if (tbl->dataH_s16_4 != NULL) {
		__mlib_free(tbl->dataH_s16_4);
	}

	if (tbl->dataV_s16 != NULL && tbl->dataV_s16 != tbl->dataH_s16) {
		__mlib_free(tbl->dataV_s16);
	}

	if (tbl->dataH_s16 != NULL) {
		__mlib_free(tbl->dataH_s16);
	}

	if (tbl->dataV_f32 != NULL && tbl->dataV_f32 != tbl->dataH_f32) {
		__mlib_free(tbl->dataV_f32);
	}

	if (tbl->dataH_f32 != NULL) {
		__mlib_free(tbl->dataH_f32);
	}

	if (tbl->dataV_d64 != NULL && tbl->dataV_d64 != tbl->dataH_d64) {
		__mlib_free(tbl->dataV_d64);
	}

	if (tbl->dataH_d64 != NULL) {
		__mlib_free(tbl->dataH_d64);
	}

	__mlib_free(tbl);
}

/* *********************************************************** */
