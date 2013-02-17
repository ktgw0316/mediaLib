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

#pragma ident	"@(#)mlib_m_ImageInterpTable.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_m_ImageInitInterpTableAffine_S16 - create an interpolation table
 *                                              for MMX
 *
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_SysMath.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

mlib_s32 mlib_ilogb(
    mlib_d64 X);

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
			tbl[num_copy * (i << kbits_vis) + c] = v;       \
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
			tbl[num_copy * (i << kbits_vis) + c] = v;       \
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
			tbl[num_copy * (i << kbits_vis) + c] = v;       \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

mlib_status
mlib_m_ImageInitInterpTableAffine_S16(
    mlib_interp_table * table,
    mlib_s32 nchan)
{
	mlib_s32 width, height, width_bits, height_bits, vis_width_bits,
	    vis_height_bits;
	mlib_s32 subsampleBitsH, subsampleBitsV;
	mlib_s32 i, j, c, scale, num_copy, num_copy_old;
	mlib_s32 isum;
	mlib_s32 max_scale, min_scale, scaleh, scalev;
	mlib_s32 norm_scale_v, norm_scale_h;
	mlib_d64 dscale, *dataH, *dataV;
	mlib_d64 **ptr_tablex, *tablex, *tablex_old, *tabley;
	mlib_d64 max, d;
	mlib_d64 sumh, sumv, normh, normv;

	if (!table)
		return (MLIB_FAILURE);
	if (table->shift_vis_affine < 0)
		return (MLIB_FAILURE);

	if (nchan == 1) {
		num_copy = 1;
		ptr_tablex = &(table->dataH_s16_1);
	} else if (nchan == 2) {
		num_copy = 2;
		ptr_tablex = &(table->dataH_s16_3);
	} else if (nchan == 3 || nchan == 4) {
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
		    mlib_malloc(num_copy * (1 << subsampleBitsH) *
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

	sumv = 0;
	max = 0;

	for (i = 0; i < (1 << subsampleBitsV); i++) {
		mlib_d64 s = 0;
		mlib_s32 ind = (i << height_bits);

		for (j = 0; j < height; j++) {
			d = mlib_fabs(dataV[j + ind]);
			s += d;
			max = (max > d) ? max : d;
		}
		sumv = (sumv > s) ? sumv : s;
	}

/* all fhkernels = 0 */
	if (sumv == 0) {
		dscale = 0;

/* X table */

		tablex =
		    mlib_malloc(num_copy * (1 << subsampleBitsH) *
		    (1 << vis_width_bits) * sizeof (mlib_s16));
		if (tablex == NULL)
			return (MLIB_FAILURE);

		INIT_TABLE_16(tablex, (1 << subsampleBitsH), width, width_bits,
		    vis_width_bits, dataH);

		if ((dataH == dataV) && num_copy == 4)
			tabley = tablex;
		else {

			num_copy = 4;

			tabley =
			    mlib_malloc(num_copy * (1 << subsampleBitsV) *
			    (1 << vis_height_bits) * sizeof (mlib_s16));
			if (tabley == NULL) {
				mlib_free(tablex);
				return (MLIB_FAILURE);
			}

			INIT_TABLE_16(tabley, (1 << subsampleBitsV), height,
			    height_bits, vis_height_bits, dataV);

			*ptr_tablex = tablex;
			table->dataV_s16_1 = tabley;

/* Store shift */
			table->shift_vis_affine = 43;

			return (MLIB_SUCCESS);
		}
	}

	normv = 32767.0 / (32768.0 * sumv);
	scalev = mlib_ilogb(sumv * normv);
	isum = mlib_ilogb(max * normv);

/* all elements must be in the range -32768, 32767 */
	if (scalev == isum)
		norm_scale_v = 14;
/* but sumv may be in the range -65576, 65575 */
	else
		norm_scale_v = 15;

	min_scale = 25;
	max_scale = 40;

	normh = 32768.0 * sumv / 32767;

	if (dataH != dataV) {
		sumh = 0;
		max = 0;

		for (i = 0; i < (1 << subsampleBitsH); i++) {
			mlib_d64 s = 0;
			mlib_s32 ind = (i << width_bits);

			for (j = 0; j < width; j++) {
				d = mlib_fabs(dataH[j + ind]);
				s += d;
				max = (max > d) ? max : d;
			}
			sumh = (sumh > s) ? sumh : s;
		}
	} else
		sumh = sumv;

	isum = mlib_ilogb(max * normh);
	scaleh = mlib_ilogb(sumh * normh);

/* all elements must be in the range -32768, 32767 */
	if (scaleh == isum)
		norm_scale_h = 14;
/* but sumh may be in the range -65576, 65575 */
	else
		norm_scale_h = 15;

	scale = norm_scale_v + norm_scale_h - (scaleh + scalev);

	if (scale < min_scale) {
		table->shift_vis_affine = -1;
/* koeff. are so large */
		return (MLIB_FAILURE);
	}

	if (scale > max_scale) {
		scaleh += (scale - max_scale + 1) >> 1;
		scalev += (scale - max_scale) >> 1;
		scale = max_scale;
	}

	if (scaleh > norm_scale_h)
		normh = normh / (1 << (scaleh - norm_scale_h));
	else
		normh = normh * (1 << (norm_scale_h - scaleh));

	if (scalev > norm_scale_v)
		normv = normv / (1 << (scalev - norm_scale_v));
	else
		normv = normv * (1 << (norm_scale_v - scalev));

/* X table */

	dscale = normh;

	tablex =
	    mlib_malloc(num_copy * (1 << subsampleBitsH) *
	    (1 << vis_width_bits) * sizeof (mlib_s16));
	if (tablex == NULL)
		return (MLIB_FAILURE);

	INIT_TABLE_16(tablex, (1 << subsampleBitsH), width, width_bits,
	    vis_width_bits, dataH);

	if ((dataH == dataV) && (num_copy == 4) && (normh == normv))
		tabley = tablex;
	else {
/* Y table */
		dscale = normv;

		num_copy = 4;

		tabley =
		    mlib_malloc(num_copy * (1 << subsampleBitsV) *
		    (1 << vis_height_bits) * sizeof (mlib_s16));
		if (tabley == NULL) {
			mlib_free(tablex);
			return (MLIB_FAILURE);
		}

		INIT_TABLE_16(tabley, (1 << subsampleBitsV), height,
		    height_bits, vis_height_bits, dataV);
	}

	*ptr_tablex = tablex;
	table->dataV_s16_1 = tabley;

/* Store shift */
	table->shift_vis_affine = scale;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
