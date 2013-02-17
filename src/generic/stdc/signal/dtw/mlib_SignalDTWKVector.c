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

#pragma ident	"@(#)mlib_SignalDTWKVector.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalDTWKVector_S16  - dynamic time warping
 *            for K-best paths
 *            of vector data
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalDTWKVectorInit_S16(void           **state,
 *                                               const mlib_s16 **dref,
 *                                               mlib_s32       lref,
 *                                               mlib_s32       ndata,
 *                                               mlib_s32       kbest,
 *                                               mlib_s32       dtype,
 *                                               mlib_s32       sref,
 *                                               mlib_s32       delta,
 *                                               mlib_s32       local,
 *                                               mlib_s32       slope);
 *     mlib_status mlib_SignalDTWKVector_S16(mlib_d64       *dist,
 *                                           const mlib_s16 **dobs,
 *                                           mlib_s32       lobs,
 *                                           mlib_s32       sobs,
 *                                          void            *state);
 *     mlib_status mlib_SignalDTWKVectorPath_S16(mlib_s32 *path,
 *                                               mlib_s32 *lpath,
 *                                               mlib_s32 kpath,
 *                                               void     *state);
 *     void mlib_SignalDTWKVectorFree_S16(void *state);
 *
 * ARGUMENTS
 *      dist    the distances of the K-best paths. length = kbest.
 *              the values are in sorted order, where dist[0] holds
 *              the distance of the best path.
 *      dobs    the observed data array.
 *      lobs    the length of the observed data array.
 *      sobs    the scaling factor of the observed data array,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *              Available scaling factor range: -111 - +126
 *      dref    the reference data array.
 *      lref    the length of the reference data array.
 *      ndata   the length of each data vector.
 *      kbest   the number of the best paths evaluated.
 *      dtype   the type of distance metric between data vectors.
 *              MLIB_DTW_L1NORM for L1 norm of difference
 *              (sum of absolute difference).
 *              MLIB_DTW_L2NORM for L2 norm of difference
 *              (Euclidean distance).
 *      sref    the scaling factor of the reference data array,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *              Available scaling factor range: -111 - +126
 *      delta   the delta in the endpoint constraints.
 *      local   the type of the local continuity constraints.
 *              MLIB_DTW_ITAKURA for Itakura type constraints.
 *      slope   the type of the slope weighting.
 *              MLIB_DTW_NONE for no slope weighting.
 *      path    the chain code array of the K-th best path.
 *      path[0] is the dref index for dobs[0], the actual
 *              chain code is from path[1] to path[lpath-1].
 *      lpath   the length of the chain code array.
 *      kpath   the path index, 0 <= kpath < kbest.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the function is completed successfully,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *  The init functions should only perform internal structure
 *  allocation and global initialization. Per function call
 *  initialization should be done in function, so the same
 *  internal structure can be reused for multiple function
 *  calls. mlib_SignalDTWKVectorPath_S16() should only be called
 *  after mlib_SignalDTWKVector_S16(), otherwise MLIB_FAILURE
 *  should be returned.
 *
 * DESCRIPTION
 *  Assume the reference data are
 *
 *    dr(y), y = 1, 2,..., N
 *
 *  and the observed data are
 *
 *    do(x), x = 1, 2,..., M
 *
 *  the dynamic time warping is to find a mapping function
 *  (a path)
 *
 *    p(i) = {px(i), py(i)}, i = 1, 2,..., Q
 *
 *  with the minimum distance.
 *
 *  In K-best paths case, K paths with the K minimum distances
 *  are searched.
 *
 *  The distance of a path is defined as
 *
 *            Q
 *    dist = SUM d(dr(py(i)), do(px(i))) * m(px(i), py(i))
 *           i = 1
 *
 *  where d(r, o) is the dissimilarity between data point/vector
 *  r and data point/vector o; m(x, y) is the path weighting
 *  coefficient associated with path point (x, y); N is the length
 *  of the reference data; M is the length of the observed data;
 *  Q is the length of the path.
 *
 *  Using L1 norm (sum of absolute differences)
 *
 *                   L-1
 *    d(r, o) = SUM |r(i) - o(i)|
 *             i = 0
 *
 *  Using L2 norm (Euclidean distance)
 *
 *                    L-1              2
 *    d(r, o) = SQRT { SUM (r(i) - o(i)) }
 *                    i = 0
 *
 *  where L is the length of each data vector.
 *
 *  To scalar data where L = 1, the two norms are the same.
 *
 *    d(r, o) = |r - o|
 *
 *  The constraints of dynamic time warping are:
 *
 *  A. Endpoint constraints
 *
 *    px(1) = 1
 *    1 <= py(1) <= 1 + delta
 *     and
 *    px(Q) = M
 *    N-delta <= py(Q) <= N
 *
 *  B. Monotonicity Conditions
 *
 *    px(i) <= px(i+1)
 *    py(i) <= py(i+1)
 *
 *  C. Local Continuity Constraints
 *
 *     See Table 4.5 on page 211 in Rabiner and Juang's book.
 *
 *     Itakura Type:
 *
 *      py
 *
 *      |
 *      *----*----*
 *      |p4  |p1  |p0
 *      |    |    |
 *      *----*----*
 *      |    |p2  |
 *      |    |    |
 *      *----*----*-- px
 *            p3
 *
 *    Allowable paths are
 *
 *      p1->p0  (1, 0)
 *      p2->p0  (1, 1)
 *      p3->p0  (1, 2)
 *
 *    Consecutive (1, 0)(1, 0) is disallowed.
 *    So path p4->p1->p0 is disallowed.
 *
 *  D. Global Path Constraints
 *
 *     Due to local continuity constraints, certain portions
 *     of the (px, py) plane are excluded from the region the
 *     optimal warping path can traverse. This forms global
 *     path constraints.
 *
 *  E. Slope Weighting
 *
 *     See Equation 4.150-3 on page 216 in Rabiner and Juang's
 *     book.
 *
 *  A path in (px, py) plane can be represented in chain code.
 *  The value of the chain code is defined as following.
 *
 *    ============================
 *    shift ( x , y ) | chain code
 *    ----------------------------
 *        ( 1 , 0 )   |     0
 *        ( 0 , 1 )   |     1
 *        ( 1 , 1 )   |     2
 *        ( 2 , 1 )   |     3
 *        ( 1 , 2 )   |     4
 *        ( 3 , 1 )   |     5
 *        ( 3 , 2 )   |     6
 *        ( 1 , 3 )   |     7
 *        ( 2 , 3 )   |     8
 *    ============================
 *
 *      py
 *
 *      |
 *      *  8  7  *
 *      |
 *      *  4  *  6
 *      |
 *      1  2  3  5
 *      |
 *      x--0--*--*-- px
 *
 *  where x marks the start point of a path segment, the numbers
 *  are the values of the chain code for the segment that ends at
 *  the point.
 *
 *  In following example, the observed data with 11 data points
 *  are mapped into the reference data with 9 data points
 *
 *      py
 *
 *      |
 *   9  | * * * * * * * * * *-*
 *      |                  /
 *      | * * * * * * * *-* * *
 *      |              /
 *      | * * * * * * * * * * *
 *      |            /
 *      | * * * * *-* * * * * *
 *      |        /
 *      | * * * * * * * * * * *
 *      |       |
 *      | * * * * * * * * * * *
 *      |      /
 *      | * * * * * * * * * * *
 *      |    /
 *      | * * * * * * * * * * *
 *      |  /
 *   1  | * * * * * * * * * * *
 *      |
 *      +------------------------ px
 *        1                   11
 *
 *  The chain code that represents the path is
 *
 *    (2 2 2 1 2 0 2 2 0 2 0)
 *
 *  See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *  and Biing-Hwang Juang, Prentice Hall, 1993.
 */

/*
 * FUNCTIONS
 *     mlib_SignalDTWKVector_F32  - dynamic time warping
 *            for K-best paths
 *            of vector data
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalDTWKVectorInit_F32(void     **state,
 *                                               mlib_f32 **dref,
 *                                               mlib_s32 lref,
 *                                               mlib_s32 ndata,
 *                                               mlib_s32 kbest,
 *                                               mlib_s32 dtype,
 *                                               mlib_s32 delta,
 *                                               mlib_s32 local,
 *                                               mlib_s32 slope);
 *     mlib_status mlib_SignalDTWKVector_F32(mlib_d64       *dist,
 *                                           const mlib_f32 **dobs,
 *                                           mlib_s32       lobs,
 *                                           void           *state);
 *     mlib_status mlib_SignalDTWKVectorPath_F32(mlib_s32 *path,
 *                                               mlib_s32 *lpath,
 *                                               mlib_s32 kpath,
 *                                               void     *state);
 *     void        mlib_SignalDTWKVectorFree_F32(void *state);
 *
 * ARGUMENTS
 *      dist    the distances of the K-best paths. length = kbest.
 *              the values are in sorted order, where dist[0] holds
 *              the distance of the best path.
 *      dobs    the observed data array.
 *      lobs    the length of the observed data array.
 *      dref    the reference data array.
 *      lref    the length of the reference data array.
 *      ndata   the length of each data vector.
 *      kbest   the number of the best paths evaluated.
 *      dtype   the type of distance metric between data vectors.
 *              MLIB_DTW_L1NORM for L1 norm of difference
 *              (sum of absolute difference).
 *              MLIB_DTW_L2NORM for L2 norm of difference
 *              (Euclidean distance).
 *      delta   the delta in the endpoint constraints.
 *      local   the type of the local continuity constraints.
 *              MLIB_DTW_ITAKURA for Itakura type constraints.
 *      slope   the type of the slope weighting.
 *              MLIB_DTW_NONE for no slope weighting.
 *      path    the chain code array of the K-th best path.
 *      path[0] is the dref index for dobs[0], the actual
 *              chain code is from path[1] to path[lpath-1].
 *      lpath   the length of the chain code array.
 *      kpath   the path index, 0 <= kpath < kbest.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the function is completed successfully,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *  The init functions should only perform internal structure
 *  allocation and global initialization. Per function call
 *  initialization should be done in function, so the same
 *  internal structure can be reused for multiple function
 *  calls. mlib_SignalDTWKVectorPath_F32() should only be called
 *  after mlib_SignalDTWKVector_F32(), otherwise MLIB_FAILURE
 *  should be returned.
 *
 * DESCRIPTION
 *  Assume the reference data are
 *
 *    dr(y), y = 1, 2,..., N
 *
 *  and the observed data are
 *
 *    do(x), x = 1, 2,..., M
 *
 *  the dynamic time warping is to find a mapping function
 *  (a path)
 *
 *    p(i) = {px(i), py(i)}, i = 1, 2,..., Q
 *
 *  with the minimum distance.
 *
 *  In K-best paths case, K paths with the K minimum distances
 *  are searched.
 *
 *  The distance of a path is defined as
 *
 *            Q
 *    dist = SUM d(dr(py(i)), do(px(i))) * m(px(i), py(i))
 *           i = 1
 *
 *  where d(r, o) is the dissimilarity between data point/vector
 *  r and data point/vector o; m(x, y) is the path weighting
 *  coefficient associated with path point (x, y); N is the length
 *  of the reference data; M is the length of the observed data;
 *  Q is the length of the path.
 *
 *  Using L1 norm (sum of absolute differences)
 *
 *              L-1
 *    d(r, o) = SUM |r(i) - o(i)|
 *             i = 0
 *
 *  Using L2 norm (Euclidean distance)
 *
 *                    L-1              2
 *    d(r, o) = SQRT { SUM (r(i) - o(i)) }
 *                    i = 0
 *
 *  where L is the length of each data vector.
 *
 *  To scalar data where L = 1, the two norms are the same.
 *
 *    d(r, o) = |r - o|
 *
 *  The constraints of dynamic time warping are:
 *
 *  A. Endpoint constraints
 *
 *    px(1) = 1
 *    1 <= py(1) <= 1 + delta
 *     and
 *    px(Q) = M
 *    N-delta <= py(Q) <= N
 *
 *  B. Monotonicity Conditions
 *
 *    px(i) <= px(i+1)
 *    py(i) <= py(i+1)
 *
 *  C. Local Continuity Constraints
 *
 *     See Table 4.5 on page 211 in Rabiner and Juang's book.
 *
 *     Itakura Type:
 *
 *      py
 *
 *      |
 *      *----*----*
 *      |p4  |p1  |p0
 *      |    |    |
 *      *----*----*
 *      |    |p2  |
 *      |    |    |
 *      *----*----*-- px
 *            p3
 *
 *    Allowable paths are
 *
 *      p1->p0  (1, 0)
 *      p2->p0  (1, 1)
 *      p3->p0  (1, 2)
 *
 *    Consecutive (1, 0)(1, 0) is disallowed.
 *    So path p4->p1->p0 is disallowed.
 *
 *  D. Global Path Constraints
 *
 *     Due to local continuity constraints, certain portions
 *     of the (px, py) plane are excluded from the region the
 *     optimal warping path can traverse. This forms global
 *     path constraints.
 *
 *  E. Slope Weighting
 *
 *     See Equation 4.150-3 on page 216 in Rabiner and Juang's
 *     book.
 *
 *  A path in (px, py) plane can be represented in chain code.
 *  The value of the chain code is defined as following.
 *
 *    ============================
 *    shift ( x , y ) | chain code
 *    ----------------------------
 *        ( 1 , 0 )   |     0
 *        ( 0 , 1 )   |     1
 *        ( 1 , 1 )   |     2
 *        ( 2 , 1 )   |     3
 *        ( 1 , 2 )   |     4
 *        ( 3 , 1 )   |     5
 *        ( 3 , 2 )   |     6
 *        ( 1 , 3 )   |     7
 *        ( 2 , 3 )   |     8
 *    ============================
 *
 *      py
 *
 *      |
 *      *  8  7  *
 *      |
 *      *  4  *  6
 *      |
 *      1  2  3  5
 *      |
 *      x--0--*--*-- px
 *
 *  where x marks the start point of a path segment, the numbers
 *  are the values of the chain code for the segment that ends at
 *  the point.
 *
 *  In following example, the observed data with 11 data points
 *  are mapped into the reference data with 9 data points
 *
 *      py
 *
 *      |
 *   9  | * * * * * * * * * *-*
 *      |                  /
 *      | * * * * * * * *-* * *
 *      |              /
 *      | * * * * * * * * * * *
 *      |            /
 *      | * * * * *-* * * * * *
 *      |        /
 *      | * * * * * * * * * * *
 *      |       |
 *      | * * * * * * * * * * *
 *      |      /
 *      | * * * * * * * * * * *
 *      |    /
 *      | * * * * * * * * * * *
 *      |  /
 *   1  | * * * * * * * * * * *
 *      |
 *      +------------------------ px
 *        1                   11
 *
 *  The chain code that represents the path is
 *
 *    (2 2 2 1 2 0 2 2 0 2 0)
 *
 *  See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *  and Biing-Hwang Juang, Prentice Hall, 1993.
 */

#include <mlib_algebra.h>
#include <mlib_SignalDTWK_Kernel.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalDTWKVectorFree_F32 = __mlib_SignalDTWKVectorFree_F32
#pragma weak mlib_SignalDTWKVectorFree_S16 = __mlib_SignalDTWKVectorFree_S16
#pragma weak mlib_SignalDTWKVectorInit_F32 = __mlib_SignalDTWKVectorInit_F32
#pragma weak mlib_SignalDTWKVectorInit_S16 = __mlib_SignalDTWKVectorInit_S16
#pragma weak mlib_SignalDTWKVectorPath_F32 = __mlib_SignalDTWKVectorPath_F32
#pragma weak mlib_SignalDTWKVectorPath_S16 = __mlib_SignalDTWKVectorPath_S16
#pragma weak mlib_SignalDTWKVector_F32 = __mlib_SignalDTWKVector_F32
#pragma weak mlib_SignalDTWKVector_S16 = __mlib_SignalDTWKVector_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalDTWKVectorFree_F32) mlib_SignalDTWKVectorFree_F32
    __attribute__((weak, alias("__mlib_SignalDTWKVectorFree_F32")));
__typeof__(__mlib_SignalDTWKVectorFree_S16) mlib_SignalDTWKVectorFree_S16
    __attribute__((weak, alias("__mlib_SignalDTWKVectorFree_S16")));
__typeof__(__mlib_SignalDTWKVectorInit_F32) mlib_SignalDTWKVectorInit_F32
    __attribute__((weak, alias("__mlib_SignalDTWKVectorInit_F32")));
__typeof__(__mlib_SignalDTWKVectorInit_S16) mlib_SignalDTWKVectorInit_S16
    __attribute__((weak, alias("__mlib_SignalDTWKVectorInit_S16")));
__typeof__(__mlib_SignalDTWKVectorPath_F32) mlib_SignalDTWKVectorPath_F32
    __attribute__((weak, alias("__mlib_SignalDTWKVectorPath_F32")));
__typeof__(__mlib_SignalDTWKVectorPath_S16) mlib_SignalDTWKVectorPath_S16
    __attribute__((weak, alias("__mlib_SignalDTWKVectorPath_S16")));
__typeof__(__mlib_SignalDTWKVector_F32) mlib_SignalDTWKVector_F32
    __attribute__((weak, alias("__mlib_SignalDTWKVector_F32")));
__typeof__(__mlib_SignalDTWKVector_S16) mlib_SignalDTWKVector_S16
    __attribute__((weak, alias("__mlib_SignalDTWKVector_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SQRT(a)	mlib_sqrtf((a))

/* *********************************************************** */

/* S16_TO_F32_LIB = 1 : Call __mlib_SignalConvertShift_F32_S16 */
#define	S16_TO_F32_LIB	1
/* S16_TO_F32_LIB = 1 : Call __mlib_VectorCopy_S32 */
#define	COPY_LIB	1

/* *********************************************************** */

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
    mlib_s32 i1)
{
	mlib_s32 RO, RDO, y_min, y_max, y_min1, y_max1;
	mlib_s32 i_x, i_y, i_n, index_y, index_x;
	mlib_f32 x1_f32, x2_f32, x3_f32, x4_f32, x5_f32, x6_f32;

	RO = (lref - 1) - ((lobs - 1) >> 1);
	RDO = (lref - 1) - ((lobs - 1) << 1) - delta_y;
	y_min = 0;
	y_max = MIN(lref, (delta_y + STD_Y_OFFSET));

	if (ndata == 1) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++) {
				COST(ptr_step_cost, i_y, 0) =
					ABS(pref[i_y] - x1_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		for (i_x = i0; i_x < (i1 - 1); i_x += 2) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[i_x];
			x2_f32 = pobs[i_x + 1];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < (y_max - 1); i_y += 2) {
				mlib_f32 y1_f32, y2_f32;

				y1_f32 = pref[i_y];
				y2_f32 = pref[i_y + 1];
				ptr_step_f32[0] = ABS(y1_f32 - x1_f32);
				ptr_step_f32[lref] = ABS(y1_f32 - x2_f32);
				ptr_step_f32 += 1;
				ptr_step_f32[0] = ABS(y2_f32 - x1_f32);
				ptr_step_f32[lref] = ABS(y2_f32 - x2_f32);
				ptr_step_f32 += 1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i_y < y_max; i_y++) {
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(pref[i_y] - x1_f32);
				COST(ptr_step_cost, i_y, i_x + 1) =
				    ABS(pref[i_y] - x2_f32);
			}
		}

		for (; i_x < i1; i_x++) {
			x1_f32 = pobs[i_x];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < lref; i_y++) {
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(pref[i_y] - x1_f32);
			}
		}

	}

/*
 * end ndata = 1
 */
/*
 * ------------------
 */
	else if (ndata == 2) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
			x2_f32 = pobs[1];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++, index_y += 2) {
				mlib_f32 y1_f32, y2_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				COST(ptr_step_cost, i_y, 0) =
				    ABS(y1_f32 - x1_f32) +
				    ABS(y2_f32 - x2_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		index_x = i0 << 1;
		for (i_x = i0; i_x < (i1 - 1); i_x += 2, index_x += 4) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[index_x];
			x2_f32 = pobs[index_x + 1];

			x3_f32 = pobs[index_x + 2];
			x4_f32 = pobs[index_x + 3];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

			index_y = 2 * y_min;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < (y_max - 1);
			    i_y += 2, index_y += 4) {
				mlib_f32 y1_f32, y2_f32, y3_f32, y4_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				y4_f32 = pref[index_y + 3];

				ptr_step_f32[0] =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 - x2_f32);
				ptr_step_f32[lref] =
				    ABS(y1_f32 - x3_f32) + ABS(y2_f32 - x4_f32);
				ptr_step_f32 += 1;
				ptr_step_f32[0] =
				    ABS(y3_f32 - x1_f32) + ABS(y4_f32 - x2_f32);
				ptr_step_f32[lref] =
				    ABS(y3_f32 - x3_f32) + ABS(y4_f32 - x4_f32);
				ptr_step_f32 += 1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i_y < y_max; i_y++, index_y += 2) {
				mlib_f32 y1_f32, y2_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 - x2_f32);
				COST(ptr_step_cost, i_y, i_x + 1) =
				    ABS(y1_f32 - x3_f32) + ABS(y2_f32 - x4_f32);
			}
		}

		for (; i_x < i1; i_x++) {
			x1_f32 = pobs[2 * i_x];
			x2_f32 = pobs[2 * i_x + 1];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < lref; i_y++, index_y += 2) {
				mlib_f32 y1_f32, y2_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 - x2_f32);
			}
		}
	}

/* end ndata = 2 */

/* ------------------ */
	else if (ndata == 3) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
			x2_f32 = pobs[1];
			x3_f32 = pobs[2];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++, index_y += 3) {
				mlib_f32 y1_f32, y2_f32, y3_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				COST(ptr_step_cost, i_y, 0) =
				    ABS(y1_f32 - x1_f32) +
				    ABS(y2_f32 - x2_f32) +
				    ABS(y3_f32 - x3_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		index_x = i0 * 3;
		for (i_x = i0; i_x < (i1 - 1); i_x += 2, index_x += 6) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[index_x];
			x2_f32 = pobs[index_x + 1];
			x3_f32 = pobs[index_x + 2];

			x4_f32 = pobs[index_x + 3];
			x5_f32 = pobs[index_x + 4];
			x6_f32 = pobs[index_x + 5];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

			index_y = 3 * y_min;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < (y_max - 1);
			    i_y += 2, index_y += 6) {
				mlib_f32 y1_f32, y2_f32, y3_f32, y4_f32, y5_f32,
				    y6_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				y4_f32 = pref[index_y + 3];
				y5_f32 = pref[index_y + 4];
				y6_f32 = pref[index_y + 5];

				ptr_step_f32[0] =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 -
				    x2_f32) + ABS(y3_f32 - x3_f32);
				ptr_step_f32[lref] =
				    ABS(y1_f32 - x4_f32) + ABS(y2_f32 -
				    x5_f32) + ABS(y3_f32 - x6_f32);
				ptr_step_f32 += 1;
				ptr_step_f32[0] =
				    ABS(y4_f32 - x1_f32) + ABS(y5_f32 -
				    x2_f32) + ABS(y6_f32 - x3_f32);
				ptr_step_f32[lref] =
				    ABS(y4_f32 - x4_f32) + ABS(y5_f32 -
				    x5_f32) + ABS(y6_f32 - x6_f32);
				ptr_step_f32 += 1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i_y < y_max; i_y++, index_y += 3) {
				mlib_f32 y1_f32, y2_f32, y3_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 -
				    x2_f32) + ABS(y3_f32 - x3_f32);
				COST(ptr_step_cost, i_y, i_x + 1) =
				    ABS(y1_f32 - x4_f32) + ABS(y2_f32 -
				    x5_f32) + ABS(y3_f32 - x6_f32);
			}
		}

		for (; i_x < i1; i_x++) {
			x1_f32 = pobs[3 * i_x];
			x2_f32 = pobs[3 * i_x + 1];
			x3_f32 = pobs[3 * i_x + 2];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < lref; i_y++, index_y += 3) {
				mlib_f32 y1_f32, y2_f32, y3_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 -
				    x2_f32) + ABS(y3_f32 - x3_f32);
			}
		}
	}

/* end ndata = 3 */

/* ------------------ */
	else if (ndata == 4) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
			x2_f32 = pobs[1];
			x3_f32 = pobs[2];
			x4_f32 = pobs[3];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++, index_y += 4) {
				mlib_f32 y1_f32, y2_f32, y3_f32, y4_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				y4_f32 = pref[index_y + 3];

				COST(ptr_step_cost, i_y, 0) =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 - x2_f32)
				    + ABS(y3_f32 - x3_f32) +
				    ABS(y4_f32 - x4_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		index_x = i0 << 2;
		for (i_x = i0; i_x < i1; i_x++, index_x += 4) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[index_x];
			x2_f32 = pobs[index_x + 1];
			x3_f32 = pobs[index_x + 2];
			x4_f32 = pobs[index_x + 3];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

			index_y = 4 * y_min;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < y_max; i_y++, index_y += 4) {
				mlib_f32 y1_f32, y2_f32, y3_f32, y4_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				y4_f32 = pref[index_y + 3];

				ptr_step_f32[0] =
				    ABS(y1_f32 - x1_f32) + ABS(y2_f32 - x2_f32)
				    + ABS(y3_f32 - x3_f32) + ABS(y4_f32 -
				    x4_f32);
				ptr_step_f32 += 1;
			}
		}
	}

/* end ndata = 4 */

/* ------------------ */
	else {
		mlib_f32 *tmp_ptr_ref, *tmp_ptr_obs;

		tmp_ptr_obs = (void *)pobs;
		tmp_ptr_ref = (void *)pref;

		if (i0 == 0) {
			for (i_y = 0; i_y < y_max; i_y++) {
				x1_f32 = 0;
				x2_f32 = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i_n = 0; i_n < (ndata - 1); i_n += 2) {
					x1_f32 += ABS(tmp_ptr_obs[i_n] -
						tmp_ptr_ref[i_n]);
					x2_f32 += ABS(tmp_ptr_obs[i_n + 1] -
						tmp_ptr_ref[i_n + 1]);
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i_n < ndata; i_n++) {
					x1_f32 += ABS(tmp_ptr_obs[i_n] -
						tmp_ptr_ref[i_n]);
				}

				COST(ptr_step_cost, i_y, 0) = x1_f32 + x2_f32;
				tmp_ptr_ref += ndata;
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		tmp_ptr_obs += ndata * i0;

		for (i_x = i0; i_x < i1; i_x++) {
			mlib_f32 *ptr_step_f32;

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);
			tmp_ptr_ref = (void *)(pref + y_min * ndata);

			for (i_y = y_min; i_y < y_max; i_y++) {

				x1_f32 = 0;
				x2_f32 = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i_n = 0; i_n < (ndata - 1); i_n += 2) {
					x1_f32 +=
					    ABS(tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]);
					x2_f32 +=
					    ABS(tmp_ptr_obs[i_n + 1] -
					    tmp_ptr_ref[i_n + 1]);
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i_n < ndata; i_n++) {
					x1_f32 +=
					    ABS(tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]);
				}

				ptr_step_f32[0] = x1_f32 + x2_f32;
				ptr_step_f32 += 1;
				tmp_ptr_ref += ndata;
			}

			tmp_ptr_obs += ndata;
		}
	}

/* end ndata > 4 */
}

/* *********************************************************** */

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
    mlib_s32 i1)
{
	mlib_s32 RO, RDO, y_min, y_max, y_min1, y_max1;
	mlib_s32 i_x, i_y, i_n, index_y, index_x;
	mlib_f32 x1_f32, x2_f32, x3_f32, x4_f32;

	RO = (lref - 1) - ((lobs - 1) >> 1);
	RDO = (lref - 1) - ((lobs - 1) << 1) - delta_y;
	y_min = 0;
	y_max = MIN(lref, (delta_y + STD_Y_OFFSET));

	if (ndata == 1) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++) {
				COST(ptr_step_cost, i_y, 0) =
					ABS(pref[i_y] - x1_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		for (i_x = i0; i_x < (i1 - 1); i_x += 2) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[i_x];
			x2_f32 = pobs[i_x + 1];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < (y_max - 1); i_y += 2) {
				mlib_f32 y1_f32, y2_f32;

				y1_f32 = pref[i_y];
				y2_f32 = pref[i_y + 1];
				ptr_step_f32[0] = ABS(y1_f32 - x1_f32);
				ptr_step_f32[lref] = ABS(y1_f32 - x2_f32);
				ptr_step_f32 += 1;
				ptr_step_f32[0] = ABS(y2_f32 - x1_f32);
				ptr_step_f32[lref] = ABS(y2_f32 - x2_f32);
				ptr_step_f32 += 1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i_y < y_max; i_y++) {
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(pref[i_y] - x1_f32);
				COST(ptr_step_cost, i_y, i_x + 1) =
				    ABS(pref[i_y] - x2_f32);
			}
		}

		for (; i_x < i1; i_x++) {
			x1_f32 = pobs[i_x];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < lref; i_y++) {
				COST(ptr_step_cost, i_y, i_x) =
				    ABS(pref[i_y] - x1_f32);
			}
		}
	}

/* end ndata = 1 */
/* ------------------ */
	else if (ndata == 2) {

		if (i0 == 0) {
			x1_f32 = pobs[0];
			x2_f32 = pobs[1];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < y_max; i_y++, index_y += 2) {
				mlib_f32 y1_f32, y2_f32;
				mlib_f32 res1_f32, res2_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				res1_f32 = (y1_f32 - x1_f32) *
					(y1_f32 - x1_f32);
				res2_f32 = (y2_f32 - x2_f32) *
					(y2_f32 - x2_f32);
				COST(ptr_step_cost, i_y, 0) = SQRT(res1_f32 +
					res2_f32);
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		index_x = i0 << 1;

		for (i_x = i0; i_x < (i1 - 1); i_x += 2, index_x += 4) {
			mlib_f32 *ptr_step_f32;

			x1_f32 = pobs[index_x];
			x2_f32 = pobs[index_x + 1];

			x3_f32 = pobs[index_x + 2];
			x4_f32 = pobs[index_x + 3];

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);

			index_y = 2 * y_min;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = y_min; i_y < (y_max - 1);
			    i_y += 2, index_y += 4) {
				mlib_f32 y1_f32, y2_f32, y3_f32, y4_f32;
				mlib_f32 res1_f32, res2_f32, res3_f32, res4_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];
				y3_f32 = pref[index_y + 2];
				y4_f32 = pref[index_y + 3];

				res1_f32 =
				    (y1_f32 - x1_f32) * (y1_f32 - x1_f32);
				res2_f32 =
				    (y2_f32 - x2_f32) * (y2_f32 - x2_f32);
				ptr_step_f32[0] = SQRT(res1_f32 + res2_f32);

				res3_f32 =
				    (y1_f32 - x3_f32) * (y1_f32 - x3_f32);
				res4_f32 =
				    (y2_f32 - x4_f32) * (y2_f32 - x4_f32);
				ptr_step_f32[lref] = SQRT(res3_f32 + res4_f32);

				ptr_step_f32 += 1;

				res1_f32 =
				    (y3_f32 - x1_f32) * (y3_f32 - x1_f32);
				res2_f32 =
				    (y4_f32 - x2_f32) * (y4_f32 - x2_f32);
				ptr_step_f32[0] = SQRT(res1_f32 + res2_f32);

				res3_f32 =
				    (y3_f32 - x3_f32) * (y3_f32 - x3_f32);
				res4_f32 =
				    (y4_f32 - x4_f32) * (y4_f32 - x4_f32);
				ptr_step_f32[lref] = SQRT(res3_f32 + res4_f32);

				ptr_step_f32 += 1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i_y < y_max; i_y++) {
				mlib_f32 y1_f32, y2_f32;
				mlib_f32 res1_f32, res2_f32, res3_f32, res4_f32;

				y1_f32 = pref[2 * i_y];
				y2_f32 = pref[2 * i_y + 1];

				res1_f32 =
				    (y1_f32 - x1_f32) * (y1_f32 - x1_f32);
				res2_f32 =
				    (y2_f32 - x2_f32) * (y2_f32 - x2_f32);
				COST(ptr_step_cost, i_y, i_x) =
				    SQRT(res1_f32 + res2_f32);

				res3_f32 =
				    (y1_f32 - x3_f32) * (y1_f32 - x3_f32);
				res4_f32 =
				    (y2_f32 - x4_f32) * (y2_f32 - x4_f32);
				COST(ptr_step_cost, i_y, i_x + 1) =
				    SQRT(res3_f32 + res4_f32);
			}
		}

		for (; i_x < i1; i_x++) {
			x1_f32 = pobs[2 * i_x];
			x2_f32 = pobs[2 * i_x + 1];

			index_y = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i_y = 0; i_y < lref; i_y++, index_y += 2) {
				mlib_f32 y1_f32, y2_f32;
				mlib_f32 res1_f32, res2_f32;

				y1_f32 = pref[index_y];
				y2_f32 = pref[index_y + 1];

				res1_f32 =
				    (y1_f32 - x1_f32) * (y1_f32 - x1_f32);
				res2_f32 =
				    (y2_f32 - x2_f32) * (y2_f32 - x2_f32);
				COST(ptr_step_cost, i_y, i_x) =
				    SQRT(res1_f32 + res2_f32);
			}
		}
	}

/* end ndata = 2 */
/* ------------------ */
	else {
		mlib_f32 *tmp_ptr_ref, *tmp_ptr_obs;

		tmp_ptr_obs = (void *)pobs;
		tmp_ptr_ref = (void *)pref;

		if (i0 == 0) {
			for (i_y = 0; i_y < y_max; i_y++) {
				x1_f32 = 0;
				x2_f32 = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i_n = 0; i_n < (ndata - 1); i_n += 2) {
					x1_f32 +=
						(tmp_ptr_obs[i_n] -
						tmp_ptr_ref[i_n]) *
						(tmp_ptr_obs[i_n] -
						tmp_ptr_ref[i_n]);
					x2_f32 +=
						(tmp_ptr_obs[i_n + 1] -
						tmp_ptr_ref[i_n + 1]) *
						(tmp_ptr_obs[i_n + 1] -
						tmp_ptr_ref[i_n + 1]);
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i_n < ndata; i_n++) {
					x1_f32 +=
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]) *
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]);
				}

				COST(ptr_step_cost, i_y, 0) =
					SQRT(x1_f32 + x2_f32);
				tmp_ptr_ref += ndata;
			}
			i0 ++;
		}

		y_min = 0;
		y_max = lref;
		tmp_ptr_obs += i0 * ndata;

		for (i_x = i0; i_x < i1; i_x++) {
			mlib_f32 *ptr_step_f32;

			MIN_MAX_Y(i_x);
			MIN_MAX_Y_1(i_x + 1);
			y_max = MAX(y_max, y_max1);
			y_min = MIN(y_min, y_min1);
			y_max = MIN(lref, (y_max + STD_Y_OFFSET));
			y_min = MAX((y_min - STD_Y_OFFSET), 0);

			ptr_step_f32 =
			    (mlib_f32 *)COST_PTR(ptr_step_cost, y_min, i_x);
			tmp_ptr_ref = (void *)(pref + y_min * ndata);

			for (i_y = y_min; i_y < y_max; i_y++) {

				x1_f32 = 0;
				x2_f32 = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i_n = 0; i_n < (ndata - 1); i_n += 2) {
					x1_f32 +=
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]) *
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]);
					x2_f32 +=
					    (tmp_ptr_obs[i_n + 1] -
					    tmp_ptr_ref[i_n +
					    1]) * (tmp_ptr_obs[i_n + 1] -
					    tmp_ptr_ref[i_n + 1]);
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i_n < ndata; i_n++) {
					x1_f32 +=
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]) *
					    (tmp_ptr_obs[i_n] -
					    tmp_ptr_ref[i_n]);
				}

				ptr_step_f32[0] = SQRT(x1_f32 + x2_f32);
				ptr_step_f32 += 1;
				tmp_ptr_ref += ndata;
			}

			tmp_ptr_obs += ndata;
		}
	}

/* end ndata > 4 */
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVectorInit_F32(
    void **state,
    const mlib_f32 **dref,
    mlib_s32 lref,
    mlib_s32 ndata,
    mlib_s32 kbest,
    mlib_s32 dtype,
    mlib_s32 delta,
    mlib_s32 local,
    mlib_s32 slope)
{
	mlib_s32 i;

	mlib_f32 *ptr_path_cost, *ptr_alongpath_cost;
	mlib_f32 *ptr_step_cost, *ptr_best_cost_stack;
	mlib_s32 *ptr_path_points_y, *ptr_chain_code;
	mlib_s32 *ptr_common_tails, *ptr_common_paths;
	mlib_s32 lobs;
	mlib_f32 *pref, *pobs;
	mlib_f32 *pref_tmp;
	MLIB_DTWK_BEST_PATH *ptr_search_data;
	MLIB_DTWK_DATA *p_state;

/*
 * testing inputs
 */

	if (delta < 0)
		return (MLIB_FAILURE);

	if (lref < 1)
		return (MLIB_FAILURE);

	if (kbest < 1)
		return (MLIB_FAILURE);

	if (local != MLIB_DTW_ITAKURA)
		return (MLIB_FAILURE);

	if (slope != MLIB_DTW_NONE)
		return (MLIB_FAILURE);

	if (ndata < 1)
		return (MLIB_FAILURE);

	if ((dtype != MLIB_DTW_L1NORM) && (dtype != MLIB_DTW_L2NORM))
		return (MLIB_FAILURE);

	if (!dref)
		return (MLIB_FAILURE);

/*
 * initializing memories
 */

	*state = __mlib_malloc(sizeof (MLIB_DTWK_DATA));

	if (!(*state))
		return (MLIB_FAILURE);
	p_state = (MLIB_DTWK_DATA *) (*state);

/* upper bound */
	lobs = (lref << 1) + 2;

	ptr_step_cost = __mlib_malloc(sizeof (mlib_f32) * lref * lobs);
	ptr_path_cost = __mlib_malloc(sizeof (mlib_f32) * (lref * lobs * 2));
	ptr_alongpath_cost = __mlib_malloc(sizeof (mlib_f32) * lobs * kbest);
	ptr_path_points_y = __mlib_malloc(sizeof (mlib_s32) * lobs * kbest);
	ptr_chain_code = __mlib_malloc(sizeof (mlib_s32) * (lobs + 1) * kbest);
	ptr_common_tails =
	    __mlib_malloc(sizeof (mlib_s32) * (TAIL_INFO_SIZE) * kbest);
	ptr_common_paths = __mlib_malloc(sizeof (mlib_s32) * (kbest + 1));
	ptr_best_cost_stack = __mlib_malloc(sizeof (mlib_f32) * kbest);
	pref = __mlib_malloc(sizeof (mlib_f32) * lref * ndata);
	pobs = __mlib_malloc(sizeof (mlib_f32) * lobs * ndata);
	ptr_search_data =
	    (MLIB_DTWK_BEST_PATH *) __mlib_malloc(sizeof (MLIB_DTWK_BEST_PATH) *
	    kbest);

	if ((!ptr_path_points_y) || (!ptr_search_data) || (!ptr_step_cost) ||
	    (!ptr_path_cost) || (!ptr_alongpath_cost) || (!ptr_chain_code) ||
	    (!ptr_common_tails) || (!pref) || (!pobs) || (!ptr_common_paths) ||
	    (!ptr_best_cost_stack)) {

		DTWK_SCALAR_FREE_MEMORIES();
		__mlib_free(*state);
		return (MLIB_FAILURE);
	}

/*
 * Fix : <rtc> Read from uninitialized (rui)
 */
	for (i = 0; i < kbest + 1; i++) {
		ptr_common_paths[i] = 0;
	}

/*
 * storing data to internal struct
 */
#if COPY_LIB

	pref_tmp = pref;
	for (i = 0; i < lref; i++) {
		__mlib_VectorCopy_S32((void *)pref_tmp, (void *)(dref[i]),
		    ndata);
		pref_tmp += ndata;
	}

#else /* COPY_LIB */

	pref_tmp = pref;
	for (i = 0; i < lref; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ndata; j++) {
			pref_tmp[j] = dref[i][j];
		}

		pref_tmp += ndata;
	}

#endif /* COPY_LIB */

	p_state->dref = pref;
	p_state->lref = lref;
	p_state->dobs = pobs;
	p_state->lobs = 0;
	p_state->kbest = kbest;
	p_state->k_found = 0;
	p_state->delta_y = (delta > (lref - 1)) ? lref - 1 : delta;
	p_state->ndata = ndata;
	p_state->dtype = dtype;
	p_state->ptr_step_cost = ptr_step_cost;
	p_state->ptr_path_cost = ptr_path_cost;
	p_state->ptr_alongpath_cost = ptr_alongpath_cost;
	p_state->ptr_path_points_y = ptr_path_points_y;
	p_state->ptr_chain_code = ptr_chain_code;
	p_state->ptr_common_tails = ptr_common_tails;
	p_state->ptr_common_paths = ptr_common_paths;
	p_state->ptr_best_cost_stack = ptr_best_cost_stack;
	p_state->ptr_search_data = ptr_search_data;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalDTWKVectorFree_F32(
    void *state)
{
	mlib_f32 *ptr_path_cost, *ptr_alongpath_cost;
	mlib_f32 *ptr_step_cost, *ptr_best_cost_stack;
	mlib_s32 *ptr_path_points_y, *ptr_chain_code;
	mlib_s32 *ptr_common_tails, *ptr_common_paths;
	mlib_f32 *pref, *pobs;
	MLIB_DTWK_BEST_PATH *ptr_search_data;
	MLIB_DTWK_DATA *p_state = (MLIB_DTWK_DATA *) state;

	if (!p_state)
		return;

	pref = p_state->dref;
	pobs = p_state->dobs;
	ptr_step_cost = p_state->ptr_step_cost;
	ptr_path_cost = p_state->ptr_path_cost;
	ptr_alongpath_cost = p_state->ptr_alongpath_cost;
	ptr_path_points_y = p_state->ptr_path_points_y;
	ptr_chain_code = p_state->ptr_chain_code;
	ptr_common_tails = p_state->ptr_common_tails;
	ptr_best_cost_stack = p_state->ptr_best_cost_stack;
	ptr_search_data = p_state->ptr_search_data;
	ptr_common_paths = p_state->ptr_common_paths;

	DTWK_SCALAR_FREE_MEMORIES();
	__mlib_free(p_state);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVectorPath_F32(
    mlib_s32 *path,
    mlib_s32 *lpath,
    mlib_s32 kpath,
    void *state)
{
	mlib_s32 i;
	MLIB_DTWK_DATA *p_state = (MLIB_DTWK_DATA *) state;
	mlib_s32 *ptr_chain_code;
	mlib_s32 length;

/* testing inputs */

	if ((!p_state) || (!path) || (!lpath))
		return (MLIB_FAILURE);

	if (kpath < 0)
		return (MLIB_FAILURE);

	if (kpath >= p_state->k_found) {
		lpath[0] = 0;
		return (MLIB_FAILURE);
	}

	length = p_state->lobs;
	ptr_chain_code = p_state->ptr_chain_code + (p_state->lobs + 1) * kpath;
	path[0] = ptr_chain_code[0];
	for (i = 1; i < length; i++) {
		path[i] = (ptr_chain_code[i] << 1);
	}

	lpath[0] = length;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVector_F32(
    mlib_d64 *dist,
    const mlib_f32 **dobs,
    mlib_s32 lobs,
    void *state)
{
	mlib_s32 i;
	mlib_s32 lref, delta_y, kbest, ndata, dtype;
	mlib_f32 *pobs;
	mlib_f32 *pobs_tmp;
	MLIB_DTWK_DATA *p_state = (MLIB_DTWK_DATA *) state;

/* testing inputs */

	if ((!p_state) || (!dist) || (!dobs))
		return (MLIB_FAILURE);

	if (lobs < 1)
		return (MLIB_FAILURE);

/* testing path existence */
	lref = p_state->lref;
	delta_y = p_state->delta_y;
	kbest = p_state->kbest;
	ndata = p_state->ndata;
	dtype = p_state->dtype;

	if ((((lobs - 1) >> 1) > (lref - 1)) ||
	    ((lref - 1 - (delta_y << 1)) > ((lobs - 1) << 1))) {

		for (i = 0; i < kbest; i++) {
			dist[i] = MLIB_D64_MAX;
		}

		return (MLIB_FAILURE);
	}

/* storing input observed data */

	pobs = p_state->dobs;
#if COPY_LIB

	pobs_tmp = pobs;
	for (i = 0; i < lobs; i++) {
		__mlib_VectorCopy_S32((void *)pobs_tmp, (void *)(dobs[i]),
		    ndata);
		pobs_tmp += ndata;
	}

#else /* COPY_LIB */

	pobs_tmp = pobs;
	for (i = 0; i < lobs; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ndata; j++) {
			pobs_tmp[j] = dobs[i][j];
		}

		pobs_tmp += ndata;
	}

#endif /* COPY_LIB */

	p_state->lobs = lobs;

/* calculating step-cost matrix */

	if (dtype == MLIB_DTW_L1NORM) {
		mlib_VectorCostMatrix_L1(p_state->ptr_step_cost, p_state->dref,
		    pobs, lref, lobs, ndata, delta_y, 0, lobs);
	} else if (dtype == MLIB_DTW_L2NORM) {
		mlib_VectorCostMatrix_L2(p_state->ptr_step_cost, p_state->dref,
		    pobs, lref, lobs, ndata, delta_y, 0, lobs);
	}

	return (mlib_SignalDWTK_Kernel(p_state, dist));
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVectorInit_S16(
    void **state,
    const mlib_s16 **dref,
    mlib_s32 lref,
    mlib_s32 ndata,
    mlib_s32 kbest,
    mlib_s32 dtype,
    mlib_s32 sref,
    mlib_s32 delta,
    mlib_s32 local,
    mlib_s32 slope)
{
	mlib_s32 i;

	mlib_f32 *ptr_path_cost, *ptr_alongpath_cost;
	mlib_f32 *ptr_step_cost, *ptr_best_cost_stack;
	mlib_s32 *ptr_path_points_y, *ptr_chain_code;
	mlib_s32 *ptr_common_tails, *ptr_common_paths;
	mlib_s32 lobs;
	mlib_f32 *pref, *pobs;
	mlib_f32 *pref_tmp;
	MLIB_DTWK_BEST_PATH *ptr_search_data;
	MLIB_DTWK_DATA *p_state;

/*
 * testing inputs
 */

	if (delta < 0)
		return (MLIB_FAILURE);

	if (lref < 1)
		return (MLIB_FAILURE);

	if (kbest < 1)
		return (MLIB_FAILURE);

	if (local != MLIB_DTW_ITAKURA)
		return (MLIB_FAILURE);

	if (slope != MLIB_DTW_NONE)
		return (MLIB_FAILURE);

	if (!dref)
		return (MLIB_FAILURE);

	if (sref > MAX_DTWK_S16_SHIFT)
		return (MLIB_FAILURE);

	if (sref < MIN_DTWK_S16_SHIFT)
		return (MLIB_FAILURE);

	if (ndata < 1)
		return (MLIB_FAILURE);

	if ((dtype != MLIB_DTW_L1NORM) && (dtype != MLIB_DTW_L2NORM))
		return (MLIB_FAILURE);

/*
 * initializing memories
 */

	*state = __mlib_malloc(sizeof (MLIB_DTWK_DATA));

	if (!(*state))
		return (MLIB_FAILURE);
	p_state = (MLIB_DTWK_DATA *) (*state);

/* upper bound */
	lobs = (lref << 1) + 2;

	ptr_step_cost = __mlib_malloc(sizeof (mlib_f32) * lref * lobs);
	ptr_path_cost = __mlib_malloc(sizeof (mlib_f32) * (lref * lobs * 2));
	ptr_alongpath_cost = __mlib_malloc(sizeof (mlib_f32) * lobs * kbest);
	ptr_path_points_y = __mlib_malloc(sizeof (mlib_s32) * lobs * kbest);
	ptr_chain_code = __mlib_malloc(sizeof (mlib_s32) * (lobs + 1) * kbest);
	ptr_common_tails =
	    __mlib_malloc(sizeof (mlib_s32) * (TAIL_INFO_SIZE) * kbest);
	ptr_common_paths = __mlib_malloc(sizeof (mlib_s32) * (kbest + 1));
	ptr_best_cost_stack = __mlib_malloc(sizeof (mlib_f32) * kbest);
	pref = __mlib_malloc(sizeof (mlib_f32) * lref * ndata);
	pobs = __mlib_malloc(sizeof (mlib_f32) * lobs * ndata);
	ptr_search_data =
	    (MLIB_DTWK_BEST_PATH *) __mlib_malloc(sizeof (MLIB_DTWK_BEST_PATH) *
	    kbest);

	if ((!ptr_path_points_y) || (!ptr_search_data) || (!ptr_step_cost) ||
	    (!ptr_path_cost) || (!ptr_alongpath_cost) || (!ptr_chain_code) ||
	    (!ptr_common_tails) || (!pref) || (!pobs) || (!ptr_common_paths) ||
	    (!ptr_best_cost_stack)) {

		DTWK_SCALAR_FREE_MEMORIES();
		__mlib_free(*state);
		return (MLIB_FAILURE);
	}

/*
 * Fix : <rtc> Read from uninitialized (rui)
 */
	for (i = 0; i < kbest + 1; i++) {
		ptr_common_paths[i] = 0;
	}

/* coverting to floats, storing to internal struct */
#if S16_TO_F32_LIB
	pref_tmp = pref;
	for (i = 0; i < lref; i++) {
		__mlib_SignalConvertShift_F32_S16(pref_tmp, dref[i], 15 - sref,
		    ndata);
		pref_tmp += ndata;
	}

#else /* S16_TO_F32_LIB */
	CALC_SCALE(scale_mult, sref);

	pref_tmp = pref;
	for (i = 0; i < lref; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ndata; j++) {
			pref_tmp[j] = dref[i][j] * scale_mult;
		}

		pref_tmp += ndata;
	}

#endif /* S16_TO_F32_LIB */

	p_state->dref = pref;
	p_state->lref = lref;
	p_state->dobs = pobs;
	p_state->lobs = 0;
	p_state->kbest = kbest;
	p_state->k_found = 0;
	p_state->delta_y = (delta > (lref - 1)) ? lref - 1 : delta;
	p_state->ndata = ndata;
	p_state->dtype = dtype;
	p_state->ptr_step_cost = ptr_step_cost;
	p_state->ptr_path_cost = ptr_path_cost;
	p_state->ptr_alongpath_cost = ptr_alongpath_cost;
	p_state->ptr_path_points_y = ptr_path_points_y;
	p_state->ptr_chain_code = ptr_chain_code;
	p_state->ptr_common_tails = ptr_common_tails;
	p_state->ptr_common_paths = ptr_common_paths;
	p_state->ptr_best_cost_stack = ptr_best_cost_stack;
	p_state->ptr_search_data = ptr_search_data;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalDTWKVectorFree_S16(
    void *state)
{
	__mlib_SignalDTWKVectorFree_F32(state);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVectorPath_S16(
    mlib_s32 *path,
    mlib_s32 *lpath,
    mlib_s32 kpath,
    void *state)
{
	return (__mlib_SignalDTWKVectorPath_F32(path, lpath, kpath, state));
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWKVector_S16(
    mlib_d64 *dist,
    const mlib_s16 **dobs,
    mlib_s32 lobs,
    mlib_s32 sobs,
    void *state)
{
	mlib_s32 i;
	mlib_s32 lref, delta_y, kbest, ndata, dtype;
	mlib_f32 *pobs;
	mlib_f32 *pobs_tmp;

#if ! S16_TO_F32_LIB
	mlib_f32 scale_mult;
#endif /* ! S16_TO_F32_LIB */

	MLIB_DTWK_DATA *p_state = (MLIB_DTWK_DATA *) state;

/* testing inputs */

	if ((!p_state) || (!dist) || (!dobs))
		return (MLIB_FAILURE);

	if (lobs < 1)
		return (MLIB_FAILURE);

	if (sobs > MAX_DTWK_S16_SHIFT)
		return (MLIB_FAILURE);

	if (sobs < MIN_DTWK_S16_SHIFT)
		return (MLIB_FAILURE);

/* testing path existence */
	lref = p_state->lref;
	delta_y = p_state->delta_y;
	kbest = p_state->kbest;
	ndata = p_state->ndata;
	dtype = p_state->dtype;

	if ((((lobs - 1) >> 1) > (lref - 1)) ||
	    ((lref - 1 - (delta_y << 1)) > ((lobs - 1) << 1))) {

		for (i = 0; i < kbest; i++) {
			dist[i] = MLIB_D64_MAX;
		}

		return (MLIB_FAILURE);
	}

/* coverting to floats, storing input observed data */
	pobs = p_state->dobs;

#if S16_TO_F32_LIB

	pobs_tmp = pobs;
	for (i = 0; i < lobs; i++) {
		__mlib_SignalConvertShift_F32_S16(pobs_tmp, dobs[i], 15 - sobs,
		    ndata);
		pobs_tmp += ndata;
	}

#else /* S16_TO_F32_LIB */
	CALC_SCALE(scale_mult, sobs);

	pobs_tmp = pobs;
	for (i = 0; i < lobs; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ndata; j++) {
			pobs_tmp[j] = dobs[i][j] * scale_mult;
		}

		pobs_tmp += ndata;
	}

#endif /* S16_TO_F32_LIB */
	p_state->lobs = lobs;

/* calculating step-cost matrix */

	if (dtype == MLIB_DTW_L1NORM) {
		mlib_VectorCostMatrix_L1(p_state->ptr_step_cost, p_state->dref,
		    pobs, lref, lobs, ndata, delta_y, 0, lobs);
	} else if (dtype == MLIB_DTW_L2NORM) {
		mlib_VectorCostMatrix_L2(p_state->ptr_step_cost, p_state->dref,
		    pobs, lref, lobs, ndata, delta_y, 0, lobs);
	}

	return (mlib_SignalDWTK_Kernel(p_state, dist));
}

/* *********************************************************** */
