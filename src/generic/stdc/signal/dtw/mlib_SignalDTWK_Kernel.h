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

#ifndef _MLIB_SIGNALDTWK_KERNEL_H
#define	_MLIB_SIGNALDTWK_KERNEL_H

#pragma ident	"@(#)mlib_SignalDTWK_Kernel.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_signal.h>
#include <mlib_SysMath.h>

#define	MLIB_DTWK_L1NORM	MLIB_DTW_L1NORM
#define	MLIB_DTWK_L2NORM	MLIB_DTW_L2NORM
#define	MIN_DTWK_S16_SHIFT	-111
#define	MAX_DTWK_S16_SHIFT	126
/* *********************************************************** */
    typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0, i1;
	} s32;
	struct
	{
		float f0, f1;
	} f32;
} mlib_uni64;

/* *********************************************************** */
#define	index_common_path	2
#define	index_branch	1
#define	index_chain_code	0
#define	TAIL_INFO_SIZE	3
#define	NUM_SHIFTS	3
#define	STD_Y_OFFSET	3

/* *********************************************************** */
#define	EMPTY_CELL	MLIB_F32_MAX

/* *********************************************************** */
#define	ABS(value)	mlib_fabsf((value))

/* *********************************************************** */
#define	COST(ptr_cost, y, x)	ptr_cost[(y) + (x)*lref]
#define	COST_PTR(ptr_cost, y, x)	(ptr_cost+(y) + (x)*lref)

/* *********************************************************** */
#define	NUM_TYPES	2
/*
 * type = 0  - "bad" path income ( chain code == 0 )
 * type = 1  - "good" path income ( chain code > 0 )
 */
#define	PATH_COST(ptr_cost, y, x, type)	ptr_cost[(y)*NUM_TYPES+NUM_TYPES*(x)* \
					lref+(type)]
#define	PATH_COST_PTR(ptr_cost, y, x)	((mlib_d64*)(ptr_cost+(y)*      \
					NUM_TYPES+NUM_TYPES*(x)*lref))

/* *********************************************************** */
#define	MIN(a, b)	((a) > (b) ? (b) : (a))

/* *********************************************************** */
#define	MAX(a, b)	((a) > (b) ? (a) : (b))

/* *********************************************************** */
#define	MIN_MAX_Y(cur_x)                                             \
	{                                                            \
	    mlib_s32 y_up1, y_up2, y_dn1, y_dn2;                     \
	                                                             \
	    y_dn1 = (cur_x) >> 1;                                    \
	    y_dn2 = (cur_x) << 1;                                    \
	    y_up1 = y_dn1 + RO + ((lobs & 1) ? ((cur_x) & 1) : 0);   \
	    y_up2 = y_dn2 + delta_y;                                 \
	    y_dn2 += RDO;                                            \
	    y_max = MIN(y_up1, y_up2);                               \
	    y_min = MAX(y_dn1, y_dn2);                               \
	}

/* *********************************************************** */
#define	MIN_MAX_Y_1(cur_x)                                           \
	{                                                            \
	    mlib_s32 y_up1, y_up2, y_dn1, y_dn2;                     \
	                                                             \
	    y_dn1 = (cur_x) >> 1;                                    \
	    y_dn2 = (cur_x) << 1;                                    \
	    y_up1 = y_dn1 + RO + ((lobs & 1) ? ((cur_x) & 1) : 0);   \
	    y_up2 = y_dn2 + delta_y;                                 \
	    y_dn2 += RDO;                                            \
	    y_max1 = MIN(y_up1, y_up2);                              \
	    y_min1 = MAX(y_dn1, y_dn2);                              \
	}

/* *********************************************************** */
/* cur_x should be even: cur_x % 2 == 0 */
#define	MIN_MAX_TWO_STEP(cur_x)                                            \
	{                                                                  \
	    mlib_s32 y_up1, y_up2, y_dn1, y_dn2;                           \
	                                                                   \
	    y_dn1 = (cur_x) >> 1;                                          \
	    y_dn2 = (cur_x) << 1;                                          \
	    y_up1 = y_dn1 + RO;                                            \
	    y_up2 = y_dn2 + delta_y;                                       \
	    y_dn2 += RDO;                                                  \
	    y_max1 =                                                       \
		MAX(MIN(y_up1, y_up2), MIN(y_up1 + ((lobs & 1) ? 1 : 0),   \
		y_up2 + 2));                                               \
	    y_min1 = MIN(MAX(y_dn1, y_dn2), MAX(y_dn1, y_dn2 + 2));        \
	    y_max1 = MIN(lref, (y_max1 + STD_Y_OFFSET));                   \
	    y_min1 = MAX((y_min1 - STD_Y_OFFSET), 0);                      \
	}

/* *********************************************************** */
typedef struct
{

	mlib_f32 best_value;
	mlib_s32 best_step;
	mlib_s32 best_chain_code;
	mlib_s32 tail_k;
	mlib_s32 prev_data;
	mlib_s32 next_data;

} MLIB_DTWK_BEST_PATH;

/* *********************************************************** */
typedef struct
{

	mlib_s32 first_index;
	mlib_s32 last_index;
	mlib_s32 number_of_items;
	mlib_s32 cur_index;
	mlib_s32 kpath;

} MLIB_DTWK_CURRENT_DATA;

/* *********************************************************** */
typedef struct
{
	mlib_f32 *dref;
	mlib_s32 lref;
	mlib_f32 *dobs;
	mlib_s32 lobs;

	mlib_s32 kbest;
	mlib_s32 k_found;
	mlib_s32 delta_y;

	mlib_s32 ndata;
	mlib_s32 dtype;

	mlib_f32 *ptr_step_cost;
	mlib_f32 *ptr_path_cost;
	mlib_f32 *ptr_alongpath_cost;
	mlib_f32 *ptr_best_cost_stack;
	mlib_s32 *ptr_path_points_y;
	mlib_s32 *ptr_chain_code;
	mlib_s32 *ptr_common_tails;
	mlib_s32 *ptr_common_paths;

	MLIB_DTWK_BEST_PATH *ptr_search_data;
} MLIB_DTWK_DATA;

/* *********************************************************** */
mlib_status mlib_SignalDWTK_Kernel(
    MLIB_DTWK_DATA * p_state,
    mlib_d64 *dist);
void
mlib_SignalDTWKScalar_Cost(
    mlib_s32 lobs,
    void *state,
    mlib_s32 i0,
    mlib_s32 i1);
void
mlib_VectorCostMatrix_L1(
    mlib_f32 *ptr_step_cost,
    const mlib_f32 *pref,
    const mlib_f32 *pobs,
    mlib_s32 lref,
    mlib_s32 lobs,
    mlib_s32 ndata,
    mlib_s32 delta_y,
    mlib_s32 i0,
    mlib_s32 i1);
void
mlib_VectorCostMatrix_L2(
    mlib_f32 *ptr_step_cost,
    const mlib_f32 *pref,
    const mlib_f32 *pobs,
    mlib_s32 lref,
    mlib_s32 lobs,
    mlib_s32 ndata,
    mlib_s32 delta_y,
    mlib_s32 i0,
    mlib_s32 i1);

/* *********************************************************** */
#define	DTWK_SCALAR_FREE_MEMORIES()                             \
	{                                                       \
	    if (ptr_common_tails)                               \
		__mlib_free(ptr_common_tails);                  \
	    if (ptr_common_paths)                               \
		__mlib_free(ptr_common_paths);                  \
	    if (ptr_chain_code)                                 \
		__mlib_free(ptr_chain_code);                    \
	    if (ptr_alongpath_cost)                             \
		__mlib_free(ptr_alongpath_cost);                \
	    if (ptr_path_points_y)                              \
		__mlib_free(ptr_path_points_y);                 \
	    if (ptr_path_cost)                                  \
		__mlib_free(ptr_path_cost);                     \
	    if (ptr_best_cost_stack)                            \
		__mlib_free(ptr_best_cost_stack);               \
	    if (ptr_step_cost)                                  \
		__mlib_free(ptr_step_cost);                     \
	    if (pref)                                           \
		__mlib_free(pref);                              \
	    if (pobs)                                           \
		__mlib_free(pobs);                              \
	    if (ptr_search_data)                                \
		__mlib_free(ptr_search_data);                   \
	}

/* *********************************************************** */
/* scale = 2 ** (-shift) */
#define	CALC_SCALE(scale, shift)                                \
	{                                                       \
	    mlib_uni64 uni1;                                    \
	                                                        \
	    uni1.s32.i0 = (1023 - (shift)) << 20;               \
	    uni1.s32.i1 = 0;                                    \
	    scale = (mlib_f32)uni1.d64;                         \
	}

/* *********************************************************** */
#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALDTWK_KERNEL_H */
