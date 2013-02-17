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

#pragma ident	"@(#)mlib_SignalDTWK_Kernel.c	9.3	07/10/09 SMI"

#include <mlib_SignalDTWK_Kernel.h>

/* *********************************************************** */

static void
mlib_Path_BackTrace(
    mlib_s32 start_x,
    mlib_f32 *ptr_path_cost,
    mlib_f32 *ptr_alongpath_cost,
    mlib_s32 *ptr_chain_code,
    mlib_s32 *ptr_path_points_y,
    mlib_f32 k_best_path_cost,
    mlib_s32 lref)
{
	mlib_s32 i;
	mlib_s32 tmp_y, y_shift_tmp;
	mlib_f32 chain_end, prev_y_shift, y_shift;
	mlib_f32 tmp3_real;
	mlib_uni64 uni1;

	tmp_y = ptr_path_points_y[start_x + 1];
	prev_y_shift = ptr_chain_code[start_x + 2];

/* backtrace for path and costs */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = start_x; ((i >= 0) && (tmp_y > 1)); i--) {

		mlib_f32 *ptr_path_f32 =
		    (mlib_f32 *)PATH_COST_PTR(ptr_path_cost, (tmp_y), i);

		chain_end = (prev_y_shift > 0.) ? ptr_path_f32[1] : EMPTY_CELL;
		y_shift = 0.;

		uni1.d64 = (((mlib_d64 *)ptr_path_f32) - 1)[0];
		tmp3_real = MIN(uni1.f32.f1, uni1.f32.f0);

		if (tmp3_real < chain_end)
			y_shift = 1.;
		if (tmp3_real < chain_end)
			chain_end = tmp3_real;

		uni1.d64 = (((mlib_d64 *)ptr_path_f32) - 2)[0];
		tmp3_real = MIN(uni1.f32.f1, uni1.f32.f0);

		if (tmp3_real < chain_end)
			y_shift = 2.;
		if (tmp3_real < chain_end)
			chain_end = tmp3_real;

		prev_y_shift = y_shift;
		y_shift_tmp = (mlib_s32)prev_y_shift;
		tmp_y -= y_shift_tmp;

		ptr_chain_code[i + 1] = y_shift_tmp;
		ptr_path_points_y[i] = tmp_y;
		ptr_alongpath_cost[i] = k_best_path_cost - chain_end;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i >= 0; i--) {

		chain_end = EMPTY_CELL;

		if (prev_y_shift) {

			chain_end = PATH_COST(ptr_path_cost, (tmp_y), i, 1);
			y_shift = 0.;
		}

		if ((tmp_y - 1) >= 0) {

			uni1.d64 =
			    *(PATH_COST_PTR(ptr_path_cost, (tmp_y - 1), i));
			tmp3_real = MIN(uni1.f32.f1, uni1.f32.f0);

			if (tmp3_real < chain_end) {
				y_shift = 1.;
				chain_end = tmp3_real;
			}

			if ((tmp_y - 2) >= 0) {

				uni1.d64 =
				    *(PATH_COST_PTR(ptr_path_cost, (tmp_y - 2),
				    i));
				tmp3_real = MIN(uni1.f32.f1, uni1.f32.f0);

				if (tmp3_real < chain_end) {
					y_shift = 2.;
					chain_end = tmp3_real;
				}
			}
		}

		if (chain_end >= EMPTY_CELL) {
			break;
		}

		prev_y_shift = y_shift;
		tmp_y -= prev_y_shift;

		ptr_chain_code[i + 1] = y_shift;
		ptr_path_points_y[i] = tmp_y;
		ptr_alongpath_cost[i] = k_best_path_cost - chain_end;
	}

/* save starting point */
	ptr_chain_code[0] = tmp_y;
}

/* *********************************************************** */

static void
UpdateBestData(
    mlib_f32 best_value,
    mlib_s32 best_step,
    mlib_s32 best_chain_code,
    mlib_s32 tail_k,
    MLIB_DTWK_CURRENT_DATA * p_data,
    MLIB_DTWK_BEST_PATH * ptr_search_data)
{
	mlib_s32 index = p_data->cur_index;
	mlib_s32 tmp_s32;

/* kpath should be >= 2!!! */
	while (index >= 0) {

		if (ptr_search_data[index].best_value > best_value)
			break;
		index = ptr_search_data[index].next_data;
	}

/* incoming new best_value is not less then all others */
	if (index >= 0) {
/* is any free positions? */

		if (p_data->number_of_items < p_data->kpath) {
			tmp_s32 = p_data->number_of_items;

			ptr_search_data[tmp_s32].next_data = index;
			ptr_search_data[tmp_s32].prev_data =
			    ptr_search_data[index].prev_data;
			ptr_search_data[index].prev_data = tmp_s32;

			if (index == p_data->cur_index)
				p_data->cur_index = tmp_s32;

			if (ptr_search_data[tmp_s32].prev_data >= 0) {
				ptr_search_data[ptr_search_data[tmp_s32].
				    prev_data].next_data = tmp_s32;
			} else {
				p_data->first_index = tmp_s32;
			}

			p_data->number_of_items++;
		} else {
/* pushing out last best element from data array */
			tmp_s32 = p_data->last_index;

			if (index != tmp_s32) {
/* index is not last */
				p_data->last_index =
				    ptr_search_data[tmp_s32].prev_data;
				ptr_search_data[p_data->last_index].next_data =
				    -1;

				ptr_search_data[tmp_s32].next_data = index;
				ptr_search_data[tmp_s32].prev_data =
				    ptr_search_data[index].prev_data;
				ptr_search_data[index].prev_data = tmp_s32;

				if (index == p_data->cur_index)
					p_data->cur_index = tmp_s32;

				if (ptr_search_data[tmp_s32].prev_data >= 0) {
					ptr_search_data[ptr_search_data
					    [tmp_s32].prev_data].next_data =
					    tmp_s32;
				} else {
					p_data->first_index = tmp_s32;
				}
			}

/* else - index is last, so indices not changed */
		}

		ptr_search_data[tmp_s32].best_value = best_value;
		ptr_search_data[tmp_s32].best_step = best_step;
		ptr_search_data[tmp_s32].best_chain_code = best_chain_code;
		ptr_search_data[tmp_s32].tail_k = tail_k;
	} else {
/* incoming new best_value is less then all others - adding to end */
/* is any free positions? */

		if (p_data->number_of_items < p_data->kpath) {
			tmp_s32 = p_data->number_of_items;
			index = p_data->last_index;

			ptr_search_data[tmp_s32].next_data = -1;
			ptr_search_data[tmp_s32].prev_data = index;
			ptr_search_data[index].next_data = tmp_s32;

			ptr_search_data[tmp_s32].best_value = best_value;
			ptr_search_data[tmp_s32].best_step = best_step;
			ptr_search_data[tmp_s32].best_chain_code =
			    best_chain_code;
			ptr_search_data[tmp_s32].tail_k = tail_k;
			p_data->last_index = tmp_s32;
			p_data->number_of_items++;
		}
	}
}

/* *********************************************************** */

#define	UPDATE_COMMON_PATH(cur_x)                                            \
	{                                                                    \
	    mlib_s32 cur_path = ptr_common_paths[1];                         \
	                                                                     \
	    for (k = 1; k <= path_count; ) {                                 \
		if (tmp_y != ptr_path_points_y[cur_path * lobs + cur_x]) {   \
		    cur_path = ptr_common_paths[path_count];                 \
		    ptr_common_paths[k] = cur_path;                          \
		    path_count--;                                            \
		} else {                                                     \
		    k++;                                                     \
		    cur_path = ptr_common_paths[k];                          \
		}                                                            \
	    }                                                                \
	}

/* *********************************************************** */

#define	WHAT_SHIFT_POSSIBLE(curren_shift)                              \
	{                                                              \
	    mlib_s32 i;                                                \
	                                                               \
	    for (i = 0; i < NUM_SHIFTS; i++) {                         \
		IsShiftPossible[i] = 1;                                \
	    }                                                          \
	    IsShiftPossible[curren_shift] = 0;                         \
	    for (k = 1; k <= path_count; k++) {                        \
		mlib_s32 cur_path = ptr_common_paths[k];               \
	                                                               \
		i = ptr_chain_code[cur_path * (lobs + 1) + i_x + 1];   \
		IsShiftPossible[i] = 0;                                \
	    }                                                          \
	}

/* *********************************************************** */

static void
mlib_SortSearch_Best_NextK(
    MLIB_DTWK_DATA * p_state,
    MLIB_DTWK_CURRENT_DATA * p_data,
    mlib_s32 cur_k)
{
	mlib_s32 i_x;
	mlib_s32 tmp_y, tmp_x, prev_y_shift, y_shift, k, path_count;
	mlib_f32 tmp1_real, tmp2_real;
	mlib_f32 tail_end, chain_end;
	mlib_s32 *tmp_ptr_path_points_y, *tmp_ptr_chain_code;
	mlib_f32 *tmp_ptr_alongpath_cost;
	mlib_s32 start_x, cur_shift;

	mlib_s32 IsShiftPossible[NUM_SHIFTS];

	mlib_s32 lobs = p_state->lobs;
	mlib_s32 lref = p_state->lref;
	mlib_f32 *ptr_step_cost = p_state->ptr_step_cost;
	mlib_f32 *ptr_path_cost = p_state->ptr_path_cost;
	mlib_f32 *ptr_alongpath_cost = p_state->ptr_alongpath_cost;
	mlib_s32 *ptr_path_points_y = p_state->ptr_path_points_y;
	mlib_s32 *ptr_chain_code = p_state->ptr_chain_code;
	mlib_s32 *ptr_common_tails = p_state->ptr_common_tails;
	mlib_s32 *ptr_common_paths = p_state->ptr_common_paths;

/* MLIB_DTWK_BEST_PATH *ptr_search_data = p_state -> ptr_search_data; */

/* update MLIB_DTWK_CURRENT_DATA - increment cur_index */
	k = p_data->cur_index;

	if ((k >= 0) && (k < p_state->kbest)) {
		p_data->cur_index = p_state->ptr_search_data[k].next_data;
/* ? is need insert new empty best-data element */

		if (p_data->cur_index < 0) {
			mlib_s32 item_index = p_data->number_of_items - 1;

			p_state->ptr_search_data[k].next_data = item_index;
			p_state->ptr_search_data[item_index].best_value =
			    EMPTY_CELL;
			p_state->ptr_search_data[item_index].next_data = -1;
			p_state->ptr_search_data[item_index].prev_data = k;
			p_state->ptr_search_data[item_index].best_step =
			    lobs - 1;
			p_state->ptr_search_data[item_index].best_chain_code =
			    0;
			p_state->ptr_search_data[item_index].tail_k = 0;
			p_data->cur_index = item_index;
			p_data->number_of_items++;
			p_data->last_index = p_data->cur_index;
		}
	}

	tmp_ptr_path_points_y = ptr_path_points_y + (cur_k - 1) * lobs;
	tmp_ptr_chain_code = ptr_chain_code + (cur_k - 1) * (lobs + 1);
	tmp_ptr_alongpath_cost = ptr_alongpath_cost + (cur_k - 1) * lobs;

/* initialize common-path stack */
	path_count = 0;
	tmp_y = tmp_ptr_path_points_y[lobs - 1];
	for (k = 0; k < cur_k - 1; k++) {
		if (tmp_y == ptr_path_points_y[k * lobs + lobs - 1]) {
			ptr_common_paths[path_count + 1] = k;
			path_count++;
		}
	}

/* observing (cur_k-1)-best for tail */

	if (ptr_common_tails[(cur_k - 1) * TAIL_INFO_SIZE + index_branch] ==
	    (lobs - 1)) {
		start_x = lobs - 2;
		prev_y_shift = 1;
	} else {
		start_x =
		    ptr_common_tails[(cur_k - 1) * TAIL_INFO_SIZE +
		    index_branch] - 1;
		prev_y_shift = tmp_ptr_chain_code[start_x + 2];
	}

/* update common-path stack */
	for (i_x = (lobs - 2); i_x > (start_x + 1); i_x--) {
		tmp_y = tmp_ptr_path_points_y[i_x];
		UPDATE_COMMON_PATH(i_x);
	}

	tmp_y = tmp_ptr_path_points_y[start_x + 1];
	i_x = start_x;
	for (; ((i_x >= 0) && (path_count > 0)); i_x--) {

		chain_end = EMPTY_CELL;
		tmp_x = i_x + 1;
		y_shift = tmp_ptr_chain_code[tmp_x];
		tail_end = tmp_ptr_alongpath_cost[tmp_x];

		UPDATE_COMMON_PATH(tmp_x);
		WHAT_SHIFT_POSSIBLE(y_shift);

		cur_shift = 0;

		if ((IsShiftPossible[0]) && (prev_y_shift)) {

/* equation 4.183 */
			chain_end = PATH_COST(ptr_path_cost, tmp_y, i_x, 1);
			chain_end += COST(ptr_step_cost, tmp_y, tmp_x);
			chain_end += tail_end;

			if (chain_end < EMPTY_CELL) {
				UpdateBestData(chain_end, i_x,
				    0, cur_k - 1,
				    p_data, p_state->ptr_search_data);
			}
		}

		for (cur_shift = 1; cur_shift < NUM_SHIFTS; cur_shift++) {
			if ((IsShiftPossible[cur_shift]) &&
			    (tmp_y - cur_shift >= 0)) {
				tmp1_real =
				    PATH_COST(ptr_path_cost,
				    (tmp_y - cur_shift), i_x, 1);
				tmp2_real =
				    PATH_COST(ptr_path_cost,
				    (tmp_y - cur_shift), i_x, 0);

/* equation 4.183 */
				chain_end = MIN(tmp1_real, tmp2_real);
				chain_end += COST(ptr_step_cost, tmp_y, tmp_x);
				chain_end += tail_end;

				if (chain_end < EMPTY_CELL) {
					UpdateBestData(chain_end, i_x,
					    cur_shift, cur_k - 1,
					    p_data, p_state->ptr_search_data);
				}
			}
		}

		prev_y_shift = y_shift;
		tmp_y = tmp_ptr_path_points_y[i_x];
	}

	for (; (i_x >= 0); i_x--) {

		chain_end = EMPTY_CELL;
		tmp_x = i_x + 1;
		y_shift = tmp_ptr_chain_code[tmp_x];
		tail_end = tmp_ptr_alongpath_cost[tmp_x];

		IsShiftPossible[0] = prev_y_shift;
		IsShiftPossible[1] = 1;
		IsShiftPossible[2] = 1;
		IsShiftPossible[y_shift] = 0;

		if (IsShiftPossible[0]) {
/* equation 4.183 */
			chain_end = PATH_COST(ptr_path_cost, tmp_y, i_x, 1);
			chain_end += COST(ptr_step_cost, tmp_y, tmp_x);
			chain_end += tail_end;

			if (chain_end < EMPTY_CELL) {
				UpdateBestData(chain_end, i_x,
				    0, cur_k - 1,
				    p_data, p_state->ptr_search_data);
			}
		}

		for (cur_shift = 1; cur_shift < NUM_SHIFTS; cur_shift++) {

			if ((IsShiftPossible[cur_shift]) &&
			    ((tmp_y - cur_shift) >= 0)) {
				tmp1_real =
				    PATH_COST(ptr_path_cost,
				    (tmp_y - cur_shift), i_x, 1);
				tmp2_real =
				    PATH_COST(ptr_path_cost,
				    (tmp_y - cur_shift), i_x, 0);
				chain_end = MIN(tmp1_real, tmp2_real);
/* equation 4.183 */
				chain_end += COST(ptr_step_cost, tmp_y, tmp_x);
				chain_end += tail_end;

				if (chain_end < EMPTY_CELL) {
					UpdateBestData(chain_end, i_x,
					    cur_shift, cur_k - 1,
					    p_data, p_state->ptr_search_data);
				}
			}
		}

		prev_y_shift = y_shift;
		tmp_y = tmp_ptr_path_points_y[i_x];
	}
}

/* *********************************************************** */

mlib_status
mlib_SignalDWTK_Kernel(
    MLIB_DTWK_DATA * p_state,
    mlib_d64 *dist)
{
	mlib_s32 i_y, i_x, i, k, last_k;
	mlib_f32 *tmp_ptr_alongpath_cost, *tail_ptr_alongpath_cost;

/* mlib_f32  prev_chain_end; */
	mlib_f32 tmp1_32, tmp2_32, tmp3_32, tmp4_32, tmp5_32, tmp6_32,
	    best_value;

	mlib_s32 *tmp_ptr_path_points_y, *tmp_ptr_chain_code;
	mlib_s32 *tail_ptr_path_points_y, *tail_ptr_chain_code;

	mlib_s32 tmp_y, /* tmp_x, */ best_step, best_chain_code, tail_k,
	    cur_index;

	mlib_s32 RO, RDO, y_min, y_max, y_min1, y_max1;

	mlib_s32 lref = p_state->lref;
	mlib_s32 lobs = p_state->lobs;
	mlib_s32 kbest = p_state->kbest;
	mlib_s32 delta_y = p_state->delta_y;

	mlib_f32 *ptr_step_cost = p_state->ptr_step_cost;
	mlib_f32 *ptr_path_cost = p_state->ptr_path_cost;
	mlib_f32 *ptr_alongpath_cost = p_state->ptr_alongpath_cost;
	mlib_f32 *ptr_best_cost_stack = p_state->ptr_best_cost_stack;
	mlib_s32 *ptr_path_points_y = p_state->ptr_path_points_y;
	mlib_s32 *ptr_chain_code = p_state->ptr_chain_code;
	mlib_s32 *ptr_common_tails = p_state->ptr_common_tails;
	MLIB_DTWK_BEST_PATH *ptr_search_data = p_state->ptr_search_data;
	MLIB_DTWK_CURRENT_DATA cur_data;
	mlib_uni64 uni1;

	RO = (lref - 1) - ((lobs - 1) >> 1);
	RDO = (lref - 1) - ((lobs - 1) << 1) - delta_y;

	uni1.f32.f0 = EMPTY_CELL;
	uni1.f32.f1 = EMPTY_CELL;
/* setting uninitialized */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_y = delta_y; i_y < MIN(lref, delta_y + STD_Y_OFFSET); i_y++) {
		*(PATH_COST_PTR(ptr_path_cost, i_y, 0)) = uni1.d64;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_y = MAX(lref - 1 - delta_y - STD_Y_OFFSET, 0);
	    i_y < lref; i_y++) {
		*(PATH_COST_PTR(ptr_path_cost, i_y, lobs - 1)) = uni1.d64;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_y = 0; i_y < 1 + delta_y; i_y++) {
		uni1.f32.f0 = COST(ptr_step_cost, i_y, 0);
		uni1.f32.f1 = uni1.f32.f0;
		*(PATH_COST_PTR(ptr_path_cost, i_y, 0)) = uni1.d64;
	}

	y_min = 0;
	y_max = lref - 1;
/* simply the best path search */
	for (i_x = 1; i_x < lobs; i_x++) {
		mlib_f32 *ptr_step_f32, *ptr_path_f32;

/* setting uninitialized */
		MIN_MAX_Y(i_x - 1);
		MIN_MAX_Y_1(i_x);
		y_max1 = MAX(y_max, y_max1);
		y_min1 = MIN(y_min, y_min1);

		tmp_y = MAX(0, y_min1 - STD_Y_OFFSET);
		y_min1 = MIN(y_min1 + STD_Y_OFFSET, lref);
		uni1.f32.f0 = EMPTY_CELL;
		uni1.f32.f1 = EMPTY_CELL;

		ptr_path_f32 =
		    (mlib_f32 *)PATH_COST_PTR(ptr_path_cost, tmp_y, i_x);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i_y = tmp_y; i_y < y_min1; i_y++) {
			((mlib_d64 *)ptr_path_f32)[0] = uni1.d64;
			ptr_path_f32 += 2;
		}

		tmp_y = MIN(y_max1 + STD_Y_OFFSET, lref);
		y_max1 = MAX(y_max1 - STD_Y_OFFSET, 0);

		ptr_path_f32 =
		    (mlib_f32 *)PATH_COST_PTR(ptr_path_cost, y_max1, i_x);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i_y = y_max1; i_y < tmp_y; i_y++) {
			((mlib_d64 *)ptr_path_f32)[0] = uni1.d64;
			ptr_path_f32 += 2;
		}

		y_min = MAX(0, y_min);
		tmp_y = MIN(y_max, lref - 3);

		tmp3_32 = EMPTY_CELL;

		if ((tmp_y >= y_min) && (y_min >= 0)) {
			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);
			tmp4_32 = ptr_step_f32[0];
			ptr_step_f32 += 1;
			tmp5_32 = ptr_step_f32[0];
			ptr_step_f32 += 1;
			ptr_path_f32 =
			    (mlib_f32 *)PATH_COST_PTR(ptr_path_cost, y_min,
			    i_x);
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i_y = y_min; i_y <= tmp_y; i_y++) {
			uni1.d64 =
			    ((mlib_d64 *)(ptr_path_f32 - (lref << 1)))[0];
			tmp6_32 = ptr_step_f32[0];
			ptr_step_f32 += 1;
/* chain code = 0 */
			ptr_path_f32[0] = uni1.f32.f1 + tmp4_32;
			tmp1_32 = MIN(uni1.f32.f1, uni1.f32.f0);
/* chain code = 2 */
			ptr_path_f32 += 2;
			tmp2_32 = tmp5_32 + tmp1_32;
			ptr_path_f32[1] = MIN(tmp2_32, tmp3_32);
/* chain code = 4 */
			tmp3_32 = tmp6_32 + tmp1_32;
			tmp4_32 = tmp5_32;
			tmp5_32 = tmp6_32;
		}

/* chain code = 4 */

		if ((tmp_y + 2 >= 0) && (tmp_y + 2 < lref))
			PATH_COST(ptr_path_cost, tmp_y + 2, i_x, 1) = tmp3_32;

		tmp_y = MIN(y_max, lref - 2);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i_y <= tmp_y; i_y++) {
			tmp1_32 = PATH_COST(ptr_path_cost, i_y, i_x - 1, 1);
			tmp2_32 = PATH_COST(ptr_path_cost, i_y, i_x - 1, 0);
/* chain code = 0 */
			PATH_COST(ptr_path_cost, i_y, i_x, 0) =
			    tmp1_32 + COST(ptr_step_cost, i_y, i_x);
			tmp1_32 = MIN(tmp1_32, tmp2_32);
/* chain code = 2 */
			tmp3_32 = PATH_COST(ptr_path_cost, i_y + 1, i_x, 1);
			tmp4_32 = COST(ptr_step_cost, i_y + 1, i_x) + tmp1_32;
			PATH_COST(ptr_path_cost, i_y + 1, i_x, 1) =
			    MIN(tmp3_32, tmp4_32);
		}	/* end slice loop */

		tmp_y = MIN(y_max, lref - 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i_y <= tmp_y; i_y++) {
			tmp1_32 = PATH_COST(ptr_path_cost, i_y, i_x - 1, 1);
/* chain code = 0 */
			PATH_COST(ptr_path_cost, i_y, i_x, 0) =
			    tmp1_32 + COST(ptr_step_cost, i_y, i_x);
		}	/* end slice loop */

	}	/* end best path loop */

	ptr_path_points_y[lobs - 1] = lref - 1;
	ptr_alongpath_cost[lobs - 1] = 0;
	ptr_chain_code[lobs] = 1;
	best_value = EMPTY_CELL;

	cur_data.first_index = 0;
	cur_data.last_index = 0;
	cur_data.number_of_items = 1;
	cur_data.cur_index = 0;
	cur_data.kpath = kbest;

	ptr_search_data[0].best_value = EMPTY_CELL;
	ptr_search_data[0].best_step = 0;
	ptr_search_data[0].best_chain_code = 0;
	ptr_search_data[0].tail_k = 0;
	ptr_search_data[0].prev_data = -1;
	ptr_search_data[0].next_data = -1;

/* initializing path-cost stack */

	if (kbest == 1) {
		mlib_s32 tmp_tail_k = lobs - 1;

		for (i = lref - 1; i >= lref - 1 - delta_y; i--) {
			tmp1_32 =
			    MIN(PATH_COST(ptr_path_cost, i, lobs - 1, 1),
			    PATH_COST(ptr_path_cost, i, lobs - 1, 0));

			if (tmp1_32 < best_value) {
				best_value = tmp1_32;
				tmp_tail_k = i;
			}
		}

		ptr_search_data[0].best_value = best_value;
		ptr_search_data[0].best_step = lobs - 1;
		ptr_search_data[0].best_chain_code = 0;
		ptr_search_data[0].tail_k = tmp_tail_k;
	} else {
		for (i = lref - 1; i >= lref - 1 - delta_y; i--) {
			tmp1_32 =
			    MIN(PATH_COST(ptr_path_cost, i, lobs - 1, 1),
			    PATH_COST(ptr_path_cost, i, lobs - 1, 0));

			if (tmp1_32 < EMPTY_CELL) {
				UpdateBestData(tmp1_32, lobs - 1, 0, i,
				    &cur_data, ptr_search_data);
			}
		}

		best_value = ptr_search_data[cur_data.first_index].best_value;
	}

	if (best_value >= EMPTY_CELL) {
		for (k = 0; k < kbest; k++) {
			dist[k] = MLIB_D64_MAX;
		}

		p_state->k_found = 0;
		return (MLIB_FAILURE);
	} else {
		ptr_path_points_y[lobs - 1] =
		    ptr_search_data[cur_data.first_index].tail_k;
	}

	mlib_Path_BackTrace(lobs - 2,
	    ptr_path_cost,
	    ptr_alongpath_cost,
	    ptr_chain_code, ptr_path_points_y, best_value, lref);

/* backsearch starting point */
	ptr_chain_code[lobs] = lobs - 2;
	ptr_best_cost_stack[0] = best_value;
	ptr_common_tails[index_branch] = lobs - 1;
	ptr_common_tails[index_chain_code] = ptr_chain_code[lobs - 1];
	ptr_common_tails[index_common_path] = ptr_path_points_y[lobs - 1];

	tmp_ptr_path_points_y = ptr_path_points_y + lobs;
	tmp_ptr_chain_code = ptr_chain_code + (lobs + 1);
	tmp_ptr_alongpath_cost = ptr_alongpath_cost + lobs;

/* searching for next bests */
	for (k = 1; k < kbest; k++) {

/* calling for main search algorithm */
		mlib_SortSearch_Best_NextK(p_state, &cur_data, k);

		cur_index = cur_data.cur_index;
		best_value = ptr_search_data[cur_index].best_value;
		best_step = ptr_search_data[cur_index].best_step;
		tail_k = ptr_search_data[cur_index].tail_k;
		best_chain_code = ptr_search_data[cur_index].best_chain_code;

		if (best_value >= EMPTY_CELL) {
/* no more paths! */
			break;
		}

		ptr_best_cost_stack[k] = best_value;
		ptr_common_tails[TAIL_INFO_SIZE * k + index_branch] = best_step;
		ptr_common_tails[TAIL_INFO_SIZE * k + index_chain_code] =
		    best_chain_code;
		ptr_common_tails[TAIL_INFO_SIZE * k + index_common_path] =
		    tail_k;

		if (best_step == lobs - 1) {

			tmp_ptr_path_points_y[lobs - 1] = tail_k;
			tmp_ptr_alongpath_cost[lobs - 1] = 0;
/* prev_chain_end = best_value; */
			tmp_ptr_chain_code[lobs] = 1;
		} else {

			tail_ptr_path_points_y =
			    ptr_path_points_y + tail_k * lobs;
			tail_ptr_chain_code =
			    ptr_chain_code + tail_k * (lobs + 1);
			tail_ptr_alongpath_cost =
			    ptr_alongpath_cost + tail_k * lobs;
			tail_ptr_path_points_y =
			    ptr_path_points_y + tail_k * lobs;

			tmp_ptr_path_points_y[lobs - 1] =
			    tail_ptr_path_points_y[lobs - 1];
			tmp_ptr_alongpath_cost[lobs - 1] = 0;

/* copying overlapping tail */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = lobs - 2; i > best_step; i--) {
				tmp_ptr_path_points_y[i] =
				    tail_ptr_path_points_y[i];
				tmp_ptr_chain_code[i + 1] =
				    tail_ptr_chain_code[i + 1];
				tmp1_32 = tail_ptr_alongpath_cost[i];
				tmp_ptr_alongpath_cost[i] = tmp1_32;
/* prev_chain_end = best_value - tmp1_32; */
			}

/* tmp_x = best_step; */
			tmp_y =
			    tmp_ptr_path_points_y[best_step + 1] -
			    best_chain_code;
			tmp_ptr_path_points_y[best_step] = tmp_y;
			tmp_ptr_chain_code[best_step + 1] = best_chain_code;

			if (best_chain_code) {
				tmp1_32 =
				    PATH_COST(ptr_path_cost, tmp_y, best_step,
				    1);
				tmp2_32 =
				    PATH_COST(ptr_path_cost, tmp_y, best_step,
				    0);
				tmp3_32 = MIN(tmp1_32, tmp2_32);
			} else {
				tmp3_32 =
				    PATH_COST(ptr_path_cost, tmp_y, best_step,
				    1);
			}

			if (tmp3_32 >= EMPTY_CELL) {
/* path step estimation error.Aborting search! */
				break;
			}

			tmp_ptr_alongpath_cost[best_step] =
			    best_value - tmp3_32;
		}

		mlib_Path_BackTrace(best_step - 1,
		    ptr_path_cost,
		    tmp_ptr_alongpath_cost,
		    tmp_ptr_chain_code,
		    tmp_ptr_path_points_y, best_value, lref);

		tmp_ptr_alongpath_cost += lobs;
		tmp_ptr_chain_code += (lobs + 1);
		tmp_ptr_path_points_y += lobs;
	}

/* last found path */
	last_k = k;

/* saving results */
	for (k = 0; k < last_k; k++) {
		dist[k] = ptr_best_cost_stack[k];
	}

/* saving empty path's */
	for (; k < kbest; k++) {
		dist[k] = MLIB_D64_MAX;
	}

	p_state->k_found = last_k;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
