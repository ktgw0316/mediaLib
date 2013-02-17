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

#pragma ident	"@(#)mlib_SignalDTWVector.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalDTWVector_S16 - dynamic time warping
 *                                of vector data
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalDTWVectorInit_S16(void           **state,
 *                                              const mlib_s16 **dref,
 *                                              mlib_s32       lref,
 *                                              mlib_s32       ndata,
 *                                              mlib_s32       dtype,
 *                                              mlib_s32       sref,
 *                                              mlib_s32       delta,
 *                                              mlib_s32       local,
 *                                              mlib_s32       slope);
 *     mlib_status mlib_SignalDTWVector_S16(mlib_d64       *dist,
 *                                          const mlib_s16 **dobs,
 *                                          mlib_s32       lobs,
 *                                          mlib_s32       sobs,
 *                                          void           *state);
 *     mlib_status mlib_SignalDTWVectorPath_S16(mlib_d64       *dist,
 *                                              mlib_s32       *path,
 *                                              mlib_s32       *lpath,
 *                                              const mlib_s16 **dobs,
 *                                              mlib_s32       lobs,
 *                                              mlib_s32       sobs,
 *                                              void           *state);
 *     mlib_status mlib_SignalDTWVectorFree_S16(void *state);
 *
 * ARGUMENTS
 *      dist    the distance of the optimal path.
 *      dobs    the observed data array.
 *      lobs    the length of the observed data array.
 *      sobs    the scaling factor of the observed data array,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *      dref    the reference data array.
 *      lref    the length of the reference data array.
 *      ndata   the length of each data vector.
 *      dtype   the type of distance metric between data vectors.
 *              MLIB_DTW_L1NORM for L1 norm of difference
 *              (sum of absolute difference).
 *              MLIB_DTW_L2NORM for L2 norm of difference
 *              (Euclidean distance).
 *      sref    the scaling factor of the reference data array,
 *              where actual_data = input_data * 2^(-scaling_factor).
 *      delta   the delta in the endpoint constraints.
 *      local   the type of the local continuity constraints.
 *              MLIB_DTW_ITAKURA for Itakura type constraints.
 *      slope   the type of the slope weighting.
 *              MLIB_DTW_NONE for no slope weighting.
 *      path    the chain code array of the optimal path.
 *              path[0] is the dref index for dobs[0], the actual
 *              chain code is from path[1] to path[lpath-1].
 *      lpath   the length of the chain code array.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the function is completed successfully,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per function call
 *      initialization should be done in function, so the same
 *      internal structure can be reused for multiple function
 *      calls.
 *
 * DESCRIPTION
 *      Assume the reference data are
 *
 *              dr(y), y = 1, 2,..., N
 *
 *      and the observed data are
 *
 *              do(x), x = 1, 2,..., M
 *
 *      the dynamic time warping is to find a mapping function
 *      (a path)
 *
 *              p(i) = {px(i), py(i)}, i = 1, 2,..., Q
 *
 *      with the minimum distance.
 *
 *      In K-best paths case, K paths with the K minimum distances
 *      are searched.
 *
 *      The distance of a path is defined as
 *
 *                      Q
 *              dist = SUM d(dr(py(i)), do(px(i))) * m(px(i), py(i))
 *                     i = 1
 *
 *      where d(r, o) is the dissimilarity between data point/vector
 *      r and data point/vector o; m(x, y) is the path weighting
 *      coefficient associated with path point (x, y); N is the length
 *      of the reference data; M is the length of the observed data;
 *      Q is the length of the path.
 *
 *      Using L1 norm (sum of absolute differences)
 *
 *                       L-1
 *              d(r, o) = SUM |r(i) - o(i)|
 *                       i = 0
 *
 *      Using L2 norm (Euclidean distance)
 *
 *                              L-1              2
 *              d(r, o) = SQRT { SUM (r(i) - o(i)) }
 *                              i = 0
 *
 *      where L is the length of each data vector.
 *
 *      To scalar data where L = 1, the two norms are the same.
 *
 *              d(r, o) = |r - o|
 *
 *      The constraints of dynamic time warping are:
 *
 *      A. Endpoint constraints
 *
 *              px(1) = 1
 *              1 <= py(1) <= 1 + delta
 *         and
 *              px(Q) = M
 *              N-delta <= py(Q) <= N
 *
 *      B. Monotonicity Conditions
 *
 *              px(i) <= px(i+1)
 *              py(i) <= py(i+1)
 *
 *      C. Local Continuity Constraints
 *
 *         See Table 4.5 on page 211 in Rabiner and Juang's book.
 *
 *         Itakura Type:
 *
 *                      py
 *
 *                      |
 *                      *----*----*
 *                      |p4  |p1  |p0
 *                      |    |    |
 *                      *----*----*
 *                      |    |p2  |
 *                      |    |    |
 *                      *----*----*-- px
 *                            p3
 *
 *              Allowable paths are
 *
 *                      p1->p0  (1, 0)
 *                      p2->p0  (1, 1)
 *                      p3->p0  (1, 2)
 *
 *              Consecutive (1, 0)(1, 0) is disallowed.
 *              So path p4->p1->p0 is disallowed.
 *
 *      D. Global Path Constraints
 *
 *         Due to local continuity constraints, certain portions
 *         of the (px, py) plane are excluded from the region the
 *         optimal warping path can traverse. This forms global
 *         path constraints.
 *
 *      E. Slope Weighting
 *
 *         See Equation 4.150-3 on page 216 in Rabiner and Juang's
 *         book.
 *
 *      A path in (px, py) plane can be represented in chain code.
 *      The value of the chain code is defined as following.
 *
 *              ============================
 *              shift ( x , y ) | chain code
 *              ----------------------------
 *                  ( 1 , 0 )   |     0
 *                  ( 0 , 1 )   |     1
 *                  ( 1 , 1 )   |     2
 *                  ( 2 , 1 )   |     3
 *                  ( 1 , 2 )   |     4
 *                  ( 3 , 1 )   |     5
 *                  ( 3 , 2 )   |     6
 *                  ( 1 , 3 )   |     7
 *                  ( 2 , 3 )   |     8
 *              ============================
 *
 *                      py
 *
 *                      |
 *                      *  8  7  *
 *                      |
 *                      *  4  *  6
 *                      |
 *                      1  2  3  5
 *                      |
 *                      x--0--*--*-- px
 *
 *      where x marks the start point of a path segment, the numbers
 *      are the values of the chain code for the segment that ends at
 *      the point.
 *
 *      In following example, the observed data with 11 data points
 *      are mapped into the reference data with 9 data points
 *
 *                      py
 *
 *                      |
 *                    9 | * * * * * * * * * *-*
 *                      |                  /
 *                      | * * * * * * * *-* * *
 *                      |              /
 *                      | * * * * * * * * * * *
 *                      |            /
 *                      | * * * * *-* * * * * *
 *                      |        /
 *                      | * * * * * * * * * * *
 *                      |       |
 *                      | * * * * * * * * * * *
 *                      |      /
 *                      | * * * * * * * * * * *
 *                      |    /
 *                      | * * * * * * * * * * *
 *                      |  /
 *                    1 | * * * * * * * * * * *
 *                      |
 *                      +------------------------ px
 *                        1                   11
 *
 *      The chain code that represents the path is
 *
 *              (2 2 2 1 2 0 2 2 0 2 0)
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 */

/*
 * FUNCTIONS
 *     mlib_SignalDTWVector_F32 - dynamic time warping
 *                                of vector data
 *
 * SYNOPSIS
 *     mlib_status mlib_SignalDTWVectorInit_F32(void           **state,
 *                                              const mlib_f32 **dref,
 *                                              mlib_s32       lref,
 *                                              mlib_s32       ndata,
 *                                              mlib_s32       dtype,
 *                                              mlib_s32       delta,
 *                                              mlib_s32       local,
 *                                              mlib_s32       slope);
 *     mlib_status mlib_SignalDTWVector_F32(mlib_d64       *dist,
 *                                          const mlib_f32 **dobs,
 *                                          mlib_s32       lobs,
 *                                          void           *state);
 *     mlib_status mlib_SignalDTWVectorPath_F32(mlib_d64       *dist,
 *                                              mlib_s32       *path,
 *                                              mlib_s32       *lpath,
 *                                              const mlib_f32 **dobs,
 *                                              mlib_s32       lobs,
 *                                              void           *state);
 *     mlib_status mlib_SignalDTWVectorFree_F32(void *state);
 *
 * ARGUMENTS
 *      dist    the distance of the optimal path.
 *      dobs    the observed data array.
 *      lobs    the length of the observed data array.
 *      dref    the reference data array.
 *      lref    the length of the reference data array.
 *      ndata   the length of each data vector.
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
 *      path    the chain code array of the optimal path.
 *              path[0] is the dref index for dobs[0], the actual
 *              chain code is from path[1] to path[lpath-1].
 *      lpath   the length of the chain code array.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the function is completed successfully,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per function call
 *      initialization should be done in function, so the same
 *      internal structure can be reused for multiple function
 *      calls.
 *
 * DESCRIPTION
 *      Assume the reference data are
 *
 *              dr(y), y = 1, 2,..., N
 *
 *      and the observed data are
 *
 *              do(x), x = 1, 2,..., M
 *
 *      the dynamic time warping is to find a mapping function
 *      (a path)
 *
 *              p(i) = {px(i), py(i)}, i = 1, 2,..., Q
 *
 *      with the minimum distance.
 *
 *      In K-best paths case, K paths with the K minimum distances
 *      are searched.
 *
 *      The distance of a path is defined as
 *
 *                      Q
 *              dist = SUM d(dr(py(i)), do(px(i))) * m(px(i), py(i))
 *                     i = 1
 *
 *      where d(r, o) is the dissimilarity between data point/vector
 *      r and data point/vector o; m(x, y) is the path weighting
 *      coefficient associated with path point (x, y); N is the length
 *      of the reference data; M is the length of the observed data;
 *      Q is the length of the path.
 *
 *      Using L1 norm (sum of absolute differences)
 *
 *                       L-1
 *              d(r, o) = SUM |r(i) - o(i)|
 *                       i = 0
 *
 *      Using L2 norm (Euclidean distance)
 *
 *                              L-1              2
 *              d(r, o) = SQRT { SUM (r(i) - o(i)) }
 *                              i = 0
 *
 *      where L is the length of each data vector.
 *
 *      To scalar data where L = 1, the two norms are the same.
 *
 *              d(r, o) = |r - o|
 *
 *      The constraints of dynamic time warping are:
 *
 *      A. Endpoint constraints
 *
 *              px(1) = 1
 *              1 <= py(1) <= 1 + delta
 *         and
 *              px(Q) = M
 *              N-delta <= py(Q) <= N
 *
 *      B. Monotonicity Conditions
 *
 *              px(i) <= px(i+1)
 *              py(i) <= py(i+1)
 *
 *      C. Local Continuity Constraints
 *
 *         See Table 4.5 on page 211 in Rabiner and Juang's book.
 *
 *         Itakura Type:
 *
 *                      py
 *
 *                      |
 *                      *----*----*
 *                      |p4  |p1  |p0
 *                      |    |    |
 *                      *----*----*
 *                      |    |p2  |
 *                      |    |    |
 *                      *----*----*-- px
 *                            p3
 *
 *              Allowable paths are
 *
 *                      p1->p0  (1, 0)
 *                      p2->p0  (1, 1)
 *                      p3->p0  (1, 2)
 *
 *              Consecutive (1, 0)(1, 0) is disallowed.
 *              So path p4->p1->p0 is disallowed.
 *
 *      D. Global Path Constraints
 *
 *         Due to local continuity constraints, certain portions
 *         of the (px, py) plane are excluded from the region the
 *         optimal warping path can traverse. This forms global
 *         path constraints.
 *
 *      E. Slope Weighting
 *
 *         See Equation 4.150-3 on page 216 in Rabiner and Juang's
 *         book.
 *
 *      A path in (px, py) plane can be represented in chain code.
 *      The value of the chain code is defined as following.
 *
 *              ============================
 *              shift ( x , y ) | chain code
 *              ----------------------------
 *                  ( 1 , 0 )   |     0
 *                  ( 0 , 1 )   |     1
 *                  ( 1 , 1 )   |     2
 *                  ( 2 , 1 )   |     3
 *                  ( 1 , 2 )   |     4
 *                  ( 3 , 1 )   |     5
 *                  ( 3 , 2 )   |     6
 *                  ( 1 , 3 )   |     7
 *                  ( 2 , 3 )   |     8
 *              ============================
 *
 *                      py
 *
 *                      |
 *                      *  8  7  *
 *                      |
 *                      *  4  *  6
 *                      |
 *                      1  2  3  5
 *                      |
 *                      x--0--*--*-- px
 *
 *      where x marks the start point of a path segment, the numbers
 *      are the values of the chain code for the segment that ends at
 *      the point.
 *
 *      In following example, the observed data with 11 data points
 *      are mapped into the reference data with 9 data points
 *
 *                      py
 *
 *                      |
 *                    9 | * * * * * * * * * *-*
 *                      |                  /
 *                      | * * * * * * * *-* * *
 *                      |              /
 *                      | * * * * * * * * * * *
 *                      |            /
 *                      | * * * * *-* * * * * *
 *                      |        /
 *                      | * * * * * * * * * * *
 *                      |       |
 *                      | * * * * * * * * * * *
 *                      |      /
 *                      | * * * * * * * * * * *
 *                      |    /
 *                      | * * * * * * * * * * *
 *                      |  /
 *                    1 | * * * * * * * * * * *
 *                      |
 *                      +------------------------ px
 *                        1                   11
 *
 *      The chain code that represents the path is
 *
 *              (2 2 2 1 2 0 2 2 0 2 0)
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <mlib_SysMath.h>
#include <mlib_SignalDTW.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalDTWVectorFree_F32 = __mlib_SignalDTWVectorFree_F32
#pragma weak mlib_SignalDTWVectorFree_S16 = __mlib_SignalDTWVectorFree_S16
#pragma weak mlib_SignalDTWVectorInit_F32 = __mlib_SignalDTWVectorInit_F32
#pragma weak mlib_SignalDTWVectorInit_S16 = __mlib_SignalDTWVectorInit_S16
#pragma weak mlib_SignalDTWVectorPath_F32 = __mlib_SignalDTWVectorPath_F32
#pragma weak mlib_SignalDTWVectorPath_S16 = __mlib_SignalDTWVectorPath_S16
#pragma weak mlib_SignalDTWVector_F32 = __mlib_SignalDTWVector_F32
#pragma weak mlib_SignalDTWVector_S16 = __mlib_SignalDTWVector_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalDTWVectorFree_F32) mlib_SignalDTWVectorFree_F32
    __attribute__((weak, alias("__mlib_SignalDTWVectorFree_F32")));
__typeof__(__mlib_SignalDTWVectorFree_S16) mlib_SignalDTWVectorFree_S16
    __attribute__((weak, alias("__mlib_SignalDTWVectorFree_S16")));
__typeof__(__mlib_SignalDTWVectorInit_F32) mlib_SignalDTWVectorInit_F32
    __attribute__((weak, alias("__mlib_SignalDTWVectorInit_F32")));
__typeof__(__mlib_SignalDTWVectorInit_S16) mlib_SignalDTWVectorInit_S16
    __attribute__((weak, alias("__mlib_SignalDTWVectorInit_S16")));
__typeof__(__mlib_SignalDTWVectorPath_F32) mlib_SignalDTWVectorPath_F32
    __attribute__((weak, alias("__mlib_SignalDTWVectorPath_F32")));
__typeof__(__mlib_SignalDTWVectorPath_S16) mlib_SignalDTWVectorPath_S16
    __attribute__((weak, alias("__mlib_SignalDTWVectorPath_S16")));
__typeof__(__mlib_SignalDTWVector_F32) mlib_SignalDTWVector_F32
    __attribute__((weak, alias("__mlib_SignalDTWVector_F32")));
__typeof__(__mlib_SignalDTWVector_S16) mlib_SignalDTWVector_S16
    __attribute__((weak, alias("__mlib_SignalDTWVector_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SWAP(v0, v1, type)	{ type tmp; tmp = v0; v0 = v1; v1 = tmp; }

/* *********************************************************** */

#define	MIN(v1, v2)	(((v1) < (v2)) ? (v1) : (v2))
#define	MAX(v1, v2)	(((v1) > (v2)) ? (v1) : (v2))

/* *********************************************************** */

static void
mlib_SignalDTWVectorNorm(
    mlib_f32 *dst,
    const mlib_f32 *src1,
    const mlib_f32 *src2,
    mlib_s32 ndata,
    mlib_s32 n,
    mlib_s32 dtype)
{
	mlib_s32 i, k;

	switch (ndata) {
	case 1:
	    {

		    if (dtype == MLIB_DTW_L1NORM)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] = mlib_fabsf(src2[0] - src1[i]);
		    else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_sqrtf((src2[0] -
					src1[i]) * (src2[0] - src1[i]));
		    break;
	    }

	case 2: {

		    if (dtype == MLIB_DTW_L1NORM)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_fabsf(src2[0] - src1[i * 2]) +
					mlib_fabsf(src2[1] - src1[i * 2 + 1]);
		    else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_sqrtf((src2[0] -
					src1[i * 2]) * (src2[0] - src1[i * 2]) +
					(src2[1] - src1[i * 2 + 1]) * (src2[1] -
					src1[i * 2 + 1]));
		    break;
	    }

	case 3: {

		    if (dtype == MLIB_DTW_L1NORM)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_fabsf(src2[0] - src1[i * 3]) +
					mlib_fabsf(src2[1] - src1[i * 3 + 1]) +
					mlib_fabsf(src2[2] - src1[i * 3 + 2]);
		    else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_sqrtf((src2[0] -
					src1[i * 3]) * (src2[0] - src1[i * 3]) +
					(src2[1] - src1[i * 3 + 1]) * (src2[1] -
					src1[i * 3 + 1]) + (src2[2] -
					src1[i * 3 + 2]) * (src2[2] -
					src1[i * 3 + 2]));
		    break;
	    }

	case 4: {

		    if (dtype == MLIB_DTW_L1NORM)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_fabsf(src2[0] - src1[i * 4]) +
					mlib_fabsf(src2[1] - src1[i * 4 + 1]) +
					mlib_fabsf(src2[2] - src1[i * 4 + 2]) +
					mlib_fabsf(src2[3] - src1[i * 4 + 3]);
		    else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < n; i++)
				    dst[i] =
					mlib_sqrtf((src2[0] -
					src1[i * 4]) * (src2[0] - src1[i * 4]) +
					(src2[1] - src1[i * 4 + 1]) * (src2[1] -
					src1[i * 4 + 1]) + (src2[2] -
					src1[i * 4 + 2]) * (src2[2] -
					src1[i * 4 + 2]) + (src2[3] -
					src1[i * 4 + 3]) * (src2[3] -
					src1[i * 4 + 3]));
		    break;
	    }

	default: {

		    if (dtype == MLIB_DTW_L1NORM)
			    for (i = 0; i < n; i++) {
				    mlib_f32 l_d = 0.f, l_d1 = 0.f;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < ndata - 1; k += 2) {
					    mlib_f32 t0, t1;

					    t0 = src2[k] - src1[ndata * i + k];
					    t1 = src2[k + 1] - src1[ndata * i +
						k + 1];
					    l_d += mlib_fabsf(t0);
					    l_d1 += mlib_fabsf(t1);
				    }

				    for (; k < ndata; k++)
					    l_d +=
						mlib_fabsf(src2[k] -
						src1[ndata * i + k]);
				    dst[i] = l_d + l_d1;
		    } else
			    for (i = 0; i < n; i++) {
				    mlib_f32 l_d = 0.f, l_d1 = 0.f;
				    mlib_f32 t, t1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < ndata - 1; k += 2) {
					    t = src2[k] - src1[ndata * i + k];
					    t1 = src2[k + 1] - src1[ndata * i +
						k + 1];
					    l_d += t * t;
					    l_d1 += t1 * t1;
				    }

				    for (; k < ndata; k++) {
					    t = src2[k] - src1[ndata * i + k];
					    l_d += t * t;
				    }

				    dst[i] = mlib_sqrtf(l_d + l_d1);
			    }
	    }
	}
}

void
mlib_SignalDTWVectorCalcDists(void *state,
	const mlib_f32 *dobs,
	mlib_s32 lobs,
	mlib_s32 i0,
	mlib_s32 i1)
{
	DTWVector_state *pstate = state;
	mlib_s32 i;
	mlib_s32 y_min, y_max;
	mlib_f32 *dref = pstate->dref;
	mlib_s32 lref = pstate->lref;
	mlib_f32 *dists = pstate->dists;
	mlib_s32 delta = pstate->delta;
	mlib_s32 ndata = pstate->ndata;
	mlib_s32 dtype = pstate->dtype;

	for (i = i0; i < i1; i++) {
		CALC_Y_MIN_MAX(i);
		y_max++;

		mlib_SignalDTWVectorNorm(dists + i * lref + y_min,
			dref + y_min * ndata,
			dobs + i * ndata, ndata, y_max - y_min, dtype);
	}
}

/* *********************************************************** */

static void
mlib_SignalDTWVectorDist(
    mlib_d64 *dist,
    const mlib_f32 *dobs,
    mlib_s32 lobs,
    void *state)
{
	DTWVector_state *pstate = state;
	mlib_s32 i, j;
	mlib_f32 *lo0, *lo1, *mi0, *mi1, *preabs;
	mlib_f32 *dref;
	mlib_s32 lref;
	mlib_s32 delta, ndata, dtype;
	mlib_s32 y_min, y_max;
	mlib_s32 y_min_prev, y_max_prev;
	mlib_f32 min_val;

	dref = pstate->dref;
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;
	dtype = pstate->dtype;

	preabs = pstate->preabs;
	lo0 = pstate->lo0 + 2;
	lo1 = pstate->lo1 + 2;
	mi0 = pstate->mi0 + 2;
	mi1 = pstate->mi1 + 2;

	CALC_Y_MIN_MAX(0);
	y_min_prev = y_min;
	y_max_prev = y_max + 1;

	mlib_SignalDTWVectorNorm(lo0, dref + y_min_prev * ndata, dobs, ndata,
	    y_max_prev - y_min_prev, dtype);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < y_max_prev - y_min_prev; j++) {
		mi0[j] = lo0[j];
	}

	for (i = 1; i < lobs; i++) {
		mlib_f32 *mi0_, *lo0_;

		CALC_Y_MIN_MAX(i);
		y_max++;

		mi0_ = mi0 + y_min - y_min_prev;
		lo0_ = lo0 + y_min - y_min_prev;

		lo0[y_max_prev - 1 + 1 - y_min_prev] = PSEUDO_INF;
		lo0[y_max_prev - 1 + 2 - y_min_prev] = PSEUDO_INF;
		mi0[y_max_prev - 1 + 1 - y_min_prev] = PSEUDO_INF;
		mi0[y_max_prev - 1 + 2 - y_min_prev] = PSEUDO_INF;

		mlib_SignalDTWVectorNorm(preabs, dref + y_min * ndata,
		    dobs + i * ndata, ndata, y_max - y_min, dtype);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < y_max - y_min - 1; j += 2) {
			mlib_f32 t0, l_d0;
			mlib_f32 t1, l_d1;
			mlib_f32 mi0_jm10 = mi0_[j - 1];
			mlib_f32 mi0_jm20 = mi0_[j - 2];
			mlib_f32 mi0_jm11 = mi0_[j];
			mlib_f32 mi0_jm21 = mi0_jm10;
			mlib_f32 lo0_j0 = lo0_[j];
			mlib_f32 lo0_j1 = lo0_[j + 1];

			l_d0 = preabs[j];
			l_d1 = preabs[j + 1];
			t0 = l_d0 + MIN(mi0_jm10, mi0_jm20);
			t1 = l_d1 + MIN(mi0_jm11, mi0_jm21);
			lo1[j] = t0;
			lo1[j + 1] = t1;
			mi1[j] = MIN(t0, lo0_j0 + l_d0);
			mi1[j + 1] = MIN(t1, lo0_j1 + l_d1);
		}

		for (; j < y_max - y_min; j++) {
			mlib_f32 t, l_d;
			mlib_f32 mi0_jm1 = mi0_[j - 1];
			mlib_f32 mi0_jm2 = mi0_[j - 2];
			mlib_f32 lo0_j = lo0_[j];

			l_d = preabs[j];
			t = l_d + MIN(mi0_jm1, mi0_jm2);
			lo1[j] = t;
			mi1[j] = MIN(t, lo0_j + l_d);
		}

#if 0
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < y_max - y_min; j++) {
			mlib_f32 t, l_d;

			l_d = preabs[j];
			t = l_d + MIN(mi0_[j - 1], mi0_[j - 2]);
			lo1[j] = t;
			mi1[j] = MIN(t, lo0_[j] + l_d);
		}

#endif /* 0 */

		SWAP(lo0, lo1, mlib_f32 *);
		SWAP(mi0, mi1, mlib_f32 *);

		y_min_prev = y_min;
		y_max_prev = y_max;
	}

	min_val = PSEUDO_INF;
	for (i = MAX(y_min_prev, lref - 1 - delta); i < MIN(y_max_prev, lref);
	    i++) {
		mlib_f32 t = mi0[i - y_min_prev];

		if (t < min_val) {
			min_val = t;
		}
	}

	*dist = min_val;
}

/* *********************************************************** */

static void
mlib_SignalDTWVectorPath(
    mlib_d64 *dist,
    mlib_s32 *path,
    mlib_s32 *lpath,
    const mlib_f32 *dobs,
    mlib_s32 lobs,
    void *state)
{
	DTWVector_state *pstate = state;

	mlib_s32 i, j;
	mlib_f32 *lo0, *lo1, *mi0, *mi1, *preabs;
	mlib_f32 *dref;
	mlib_s32 lref;
	mlib_s32 delta, ndata, dtype;
	mlib_s32 y_min, y_max;
	mlib_s32 y_min_prev, y_max_prev;
	mlib_f32 min_val;
	mlib_f32 *path_buf;
	mlib_s32 fwd_check;
	mlib_s32 min_elem;

	path_buf = pstate->path_buf;
	dref = pstate->dref;
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;
	dtype = pstate->dtype;

	preabs = pstate->preabs;
	lo0 = pstate->lo0 + 2;
	lo1 = pstate->lo1 + 2;
	mi0 = pstate->mi0 + 2;
	mi1 = pstate->mi1 + 2;

	CALC_Y_MIN_MAX(0);
	y_min_prev = 0;
	y_max_prev = y_max + 1;

	mlib_SignalDTWVectorNorm(lo0, dref + y_min_prev * ndata, dobs, ndata,
	    y_max_prev - y_min_prev, dtype);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < y_max_prev - y_min_prev; j++) {
		mi0[j] = lo0[j];
	}

	for (i = 1; i < lobs; i++) {
		mlib_f32 *mi0_, *lo0_;

		CALC_Y_MIN_MAX(i);
		y_max++;

		mi0_ = mi0 + y_min - y_min_prev;
		lo0_ = lo0 + y_min - y_min_prev;

		lo0[y_max_prev - 1 + 1 - y_min_prev] = PSEUDO_INF;
		lo0[y_max_prev - 1 + 2 - y_min_prev] = PSEUDO_INF;

		mi0[y_max_prev - 1 + 1 - y_min_prev] = PSEUDO_INF;
		mi0[y_max_prev - 1 + 2 - y_min_prev] = PSEUDO_INF;

		mlib_SignalDTWVectorNorm(preabs, dref + y_min * ndata,
		    dobs + i * ndata, ndata, y_max - y_min, dtype);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < y_max - y_min; j++) {
			mlib_f32 sub0, sub1;
			mlib_f32 t0, t1, l_d;
			mlib_f32 mi0_jm1 = mi0_[j - 1];
			mlib_f32 mi0_jm2 = mi0_[j - 2];
			mlib_f32 lo0_j = lo0_[j];

			l_d = preabs[j];
			t0 = MIN(mi0_jm1, mi0_jm2);
			sub0 = mi0_jm1 - mi0_jm2;
			lo1[j] = l_d + t0;

			t1 = MIN(t0, lo0_j);
			sub1 = t0 - lo0_j;
			mi1[j] = l_d + t1;

			path_buf[2 * i * lref + 2 * j + 2 * y_min] = sub0;
			path_buf[2 * i * lref + 2 * j + 2 * y_min + 1] = sub1;
		}

		SWAP(lo0, lo1, mlib_f32 *);
		SWAP(mi0, mi1, mlib_f32 *);

		y_min_prev = y_min;
		y_max_prev = y_max;
	}

	min_val = PSEUDO_INF;
	min_elem = -1;
	for (i = MAX(y_min_prev, lref - 1 - delta); i < MIN(y_max_prev, lref);
	    i++) {
		mlib_f32 t = mi0[i - y_min_prev];

		if (t < min_val) {
			min_val = t;
			min_elem = i;
		}
	}

	*lpath = lobs;
	*dist = min_val;
	fwd_check = 1;
	for (i = lobs - 1; i >= 1; i--) {
		mlib_f32 sub0 = path_buf[2 * i * lref + 2 * min_elem];
		mlib_f32 sub1 = path_buf[2 * i * lref + 2 * min_elem + 1];
		mlib_s32 res;

		if ((sub1 >= 0.0f) && fwd_check) {
			res = 0;
			fwd_check = 0;
		} else {
			if (sub0 < 0.0f)
				res = 1;
			else
				res = 2;
			fwd_check = 1;
			min_elem -= res;
			res <<= 1;
		}

		path[i] = res;
	}

	path[0] = min_elem;
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVectorInit_S16(
    void **state,
    const mlib_s16 **dref,
    mlib_s32 lref,
    mlib_s32 ndata,
    mlib_s32 dtype,
    mlib_s32 sref,
    mlib_s32 delta,
    mlib_s32 local,
    mlib_s32 slope)
{
	DTWVector_state *pstate;
	mlib_s32 i;
	mlib_f32 *ldref;

	if (local != MLIB_DTW_ITAKURA || slope != MLIB_DTW_NONE || lref < 0 ||
	    ndata < 0 || delta < 0 || delta > lref - 1)
		return (MLIB_FAILURE);
	pstate = (DTWVector_state *) __mlib_malloc(sizeof (DTWVector_state));

	if (!pstate)
		return (MLIB_FAILURE);
	pstate->dref =
	    (mlib_f32 *)__mlib_malloc(lref * ndata * sizeof (mlib_f32));
	pstate->lo0 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->lo1 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->mi0 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->mi1 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->preabs =
	    (mlib_f32 *)__mlib_malloc(((lref + 1) & ~1) * sizeof (mlib_f32));

	if (!pstate->dref || !pstate->lo0 || !pstate->lo1 || !pstate->mi0 ||
	    !pstate->mi1 || !pstate->preabs) {

		if (pstate->dref)
			__mlib_free(pstate->dref);

		if (pstate->lo0)
			__mlib_free(pstate->lo0);

		if (pstate->lo1)
			__mlib_free(pstate->lo1);

		if (pstate->mi0)
			__mlib_free(pstate->mi0);

		if (pstate->mi1)
			__mlib_free(pstate->mi1);

		if (pstate->preabs)
			__mlib_free(pstate->preabs);
		__mlib_free(pstate);
		return (MLIB_FAILURE);
	}

	(pstate->lo0)[0] = (pstate->lo0)[1] = PSEUDO_INF;
	(pstate->lo1)[0] = (pstate->lo1)[1] = PSEUDO_INF;
	(pstate->mi0)[0] = (pstate->mi0)[1] = PSEUDO_INF;
	(pstate->mi1)[0] = (pstate->mi1)[1] = PSEUDO_INF;
	pstate->delta = delta;
	pstate->ndata = ndata;
	pstate->dtype = dtype;
	pstate->dobs = 0;
	pstate->lobs = 0;
	pstate->lbuf = 0;
	pstate->path_buf = 0;
	pstate->lref = lref;

	ldref = pstate->dref;

	for (i = 0; i < lref; i++) {
		if (__mlib_SignalConvertShift_F32_S16(ldref + ndata * i,
		    dref[i], 15 - sref, ndata) != MLIB_SUCCESS) {
			return (MLIB_FAILURE);
		}
	}

	*state = pstate;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVector_S16(
    mlib_d64 *dist,
    const mlib_s16 **dobs,
    mlib_s32 lobs,
    mlib_s32 sobs,
    void *state)
{
	DTWVector_state *pstate = state;
	mlib_s32 i;
	mlib_s32 lref;
	mlib_f32 *ldobs;
	mlib_s32 delta, ndata;
	mlib_s32 y_min, y_max;

	if (!state || lobs < 0)
		return (MLIB_FAILURE);
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;

/* check path existence */
	CALC_Y_MIN_MAX(0);

	if (y_min > lref - 1 || y_max < 0 || y_min > y_max)
		return (MLIB_FAILURE);

	if (!pstate->dobs || pstate->lobs < lobs) {
		if (pstate->dobs)
			__mlib_free(pstate->dobs);
		pstate->dobs =
		    (mlib_f32 *)__mlib_malloc(lobs * ndata * sizeof (mlib_f32));

		if (!pstate->dobs)
			return (MLIB_FAILURE);
		pstate->lobs = lobs;
	}

	ldobs = pstate->dobs;

	for (i = 0; i < lobs; i++) {
		if (__mlib_SignalConvertShift_F32_S16(ldobs + ndata * i,
		    dobs[i], 15 - sobs, ndata) != MLIB_SUCCESS) {
			return (MLIB_FAILURE);
		}
	}

	mlib_SignalDTWVectorDist(dist, ldobs, lobs, state);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVectorPath_S16(
    mlib_d64 *dist,
    mlib_s32 *path,
    mlib_s32 *lpath,
    const mlib_s16 **dobs,
    mlib_s32 lobs,
    mlib_s32 sobs,
    void *state)
{
	DTWVector_state *pstate = state;
	mlib_s32 i;
	mlib_s32 lref;
	mlib_f32 *ldobs;
	mlib_s32 delta, ndata;
	mlib_s32 y_min, y_max;

	if (!state || lobs < 0)
		return (MLIB_FAILURE);
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;

/* check path existence */
	CALC_Y_MIN_MAX(0);

	if (y_min > lref - 1 || y_max < 0 || y_min > y_max)
		return (MLIB_FAILURE);

	if (!pstate->dobs || pstate->lobs < lobs) {
		if (pstate->dobs)
			__mlib_free(pstate->dobs);
		pstate->dobs =
		    (mlib_f32 *)__mlib_malloc(lobs * ndata * sizeof (mlib_f32));

		if (!pstate->dobs)
			return (MLIB_FAILURE);
		pstate->lobs = lobs;
	}

	if (!pstate->path_buf || pstate->lbuf < lobs) {
		if (pstate->path_buf)
			__mlib_free(pstate->path_buf);
		pstate->path_buf =
		    (mlib_f32 *)__mlib_malloc(2 * sizeof (mlib_f32) * lobs *
		    pstate->lref);

		if (!pstate->path_buf)
			return (MLIB_FAILURE);
		pstate->lbuf = lobs;
	}

	ldobs = pstate->dobs;
	for (i = 0; i < lobs; i++) {
		if (__mlib_SignalConvertShift_F32_S16(ldobs + ndata * i,
		    dobs[i], 15 - sobs, ndata) != MLIB_SUCCESS) {
			return (MLIB_FAILURE);
		}
	}

	mlib_SignalDTWVectorPath(dist, path, lpath, ldobs, lobs, state);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalDTWVectorFree_S16(
    void *state)
{
	DTWVector_state *pstate = state;

	__mlib_free(pstate->lo0);
	__mlib_free(pstate->lo1);
	__mlib_free(pstate->mi0);
	__mlib_free(pstate->mi1);
	__mlib_free(pstate->preabs);

	if (pstate->dobs)
		__mlib_free(pstate->dobs);
	__mlib_free(pstate->dref);

	if (pstate->path_buf)
		__mlib_free(pstate->path_buf);
	__mlib_free(pstate);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVectorInit_F32(
    void **state,
    const mlib_f32 **dref,
    mlib_s32 lref,
    mlib_s32 ndata,
    mlib_s32 dtype,
    mlib_s32 delta,
    mlib_s32 local,
    mlib_s32 slope)
{
	DTWVector_state *pstate;
	mlib_s32 i;
	mlib_f32 *ldref;

	if (local != MLIB_DTW_ITAKURA || slope != MLIB_DTW_NONE || lref < 0 ||
	    ndata < 0 || delta < 0 || delta > lref - 1)
		return (MLIB_FAILURE);
	pstate = (DTWVector_state *) __mlib_malloc(sizeof (DTWVector_state));

	if (!pstate)
		return (MLIB_FAILURE);
	pstate->dref =
	    (mlib_f32 *)__mlib_malloc(lref * ndata * sizeof (mlib_f32));
	pstate->lo0 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->lo1 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->mi0 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->mi1 = (mlib_f32 *)__mlib_malloc((lref + 4) * sizeof (mlib_f32));
	pstate->preabs =
	    (mlib_f32 *)__mlib_malloc(((lref + 1) & ~1) * sizeof (mlib_f32));

	if (!pstate->dref || !pstate->lo0 || !pstate->lo1 || !pstate->mi0 ||
	    !pstate->mi1 || !pstate->preabs) {

		if (pstate->dref)
			__mlib_free(pstate->dref);

		if (pstate->lo0)
			__mlib_free(pstate->lo0);

		if (pstate->lo1)
			__mlib_free(pstate->lo1);

		if (pstate->mi0)
			__mlib_free(pstate->mi0);

		if (pstate->mi1)
			__mlib_free(pstate->mi1);

		if (pstate->preabs)
			__mlib_free(pstate->preabs);
		__mlib_free(pstate);
		return (MLIB_FAILURE);
	}

	(pstate->lo0)[0] = (pstate->lo0)[1] = PSEUDO_INF;
	(pstate->lo1)[0] = (pstate->lo1)[1] = PSEUDO_INF;
	(pstate->mi0)[0] = (pstate->mi0)[1] = PSEUDO_INF;
	(pstate->mi1)[0] = (pstate->mi1)[1] = PSEUDO_INF;
	pstate->delta = delta;
	pstate->ndata = ndata;
	pstate->dtype = dtype;
	pstate->dobs = 0;
	pstate->lobs = 0;
	pstate->lbuf = 0;
	pstate->path_buf = 0;
	pstate->lref = lref;

	ldref = pstate->dref;

	for (i = 0; i < lref; i++)
		__mlib_VectorCopy_S32((void *)(ldref + ndata * i),
		    (void *)(dref[i]), ndata);

	*state = pstate;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVector_F32(
    mlib_d64 *dist,
    const mlib_f32 **dobs,
    mlib_s32 lobs,
    void *state)
{
	DTWVector_state *pstate = state;
	mlib_s32 i;
	mlib_s32 lref;
	mlib_f32 *ldobs;
	mlib_s32 delta, ndata;
	mlib_s32 y_min, y_max;

	if (!state || lobs < 0)
		return (MLIB_FAILURE);
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;

/* check path existence */
	CALC_Y_MIN_MAX(0);

	if (y_min > lref - 1 || y_max < 0 || y_min > y_max)
		return (MLIB_FAILURE);

	if (!pstate->dobs || pstate->lobs < lobs) {
		if (pstate->dobs)
			__mlib_free(pstate->dobs);
		pstate->dobs =
		    (mlib_f32 *)__mlib_malloc(lobs * ndata * sizeof (mlib_f32));

		if (!pstate->dobs)
			return (MLIB_FAILURE);
		pstate->lobs = lobs;
	}

	ldobs = pstate->dobs;

	for (i = 0; i < lobs; i++)
		__mlib_VectorCopy_S32((void *)(ldobs + ndata * i),
		    (void *)(dobs[i]), ndata);

	mlib_SignalDTWVectorDist(dist, ldobs, lobs, state);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDTWVectorPath_F32(
    mlib_d64 *dist,
    mlib_s32 *path,
    mlib_s32 *lpath,
    const mlib_f32 **dobs,
    mlib_s32 lobs,
    void *state)
{
	DTWVector_state *pstate = state;
	mlib_s32 i;
	mlib_s32 lref;
	mlib_f32 *ldobs;
	mlib_s32 delta, ndata;
	mlib_s32 y_min, y_max;

	if (!state || lobs < 0)
		return (MLIB_FAILURE);
	lref = pstate->lref;
	delta = pstate->delta;
	ndata = pstate->ndata;

/* check path existence */
	CALC_Y_MIN_MAX(0);

	if (y_min > lref - 1 || y_max < 0 || y_min > y_max)
		return (MLIB_FAILURE);

	if (!pstate->dobs || pstate->lobs < lobs) {
		if (pstate->dobs)
			__mlib_free(pstate->dobs);
		pstate->dobs =
		    (mlib_f32 *)__mlib_malloc(lobs * ndata * sizeof (mlib_f32));

		if (!pstate->dobs)
			return (MLIB_FAILURE);
		pstate->lobs = lobs;
	}

	if (!pstate->path_buf || pstate->lbuf < lobs) {
		if (pstate->path_buf)
			__mlib_free(pstate->path_buf);
		pstate->path_buf =
		    (mlib_f32 *)__mlib_malloc(2 * sizeof (mlib_f32) * lobs *
		    pstate->lref);

		if (!pstate->path_buf)
			return (MLIB_FAILURE);
		pstate->lbuf = lobs;
	}

	ldobs = pstate->dobs;

	for (i = 0; i < lobs; i++)
		__mlib_VectorCopy_S32((void *)(ldobs + ndata * i),
		    (void *)(dobs[i]), ndata);

	mlib_SignalDTWVectorPath(dist, path, lpath, ldobs, lobs, state);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalDTWVectorFree_F32(
    void *state)
{
	DTWVector_state *pstate = state;

	__mlib_free(pstate->lo0);
	__mlib_free(pstate->lo1);
	__mlib_free(pstate->mi0);
	__mlib_free(pstate->mi1);
	__mlib_free(pstate->preabs);

	if (pstate->dobs)
		__mlib_free(pstate->dobs);
	__mlib_free(pstate->dref);

	if (pstate->path_buf)
		__mlib_free(pstate->path_buf);
	__mlib_free(pstate);
}

/* *********************************************************** */
