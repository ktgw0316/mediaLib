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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoom_BC_Fp_D64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoom on MLIB_BICUBIC filter
 */

#include <mlib_image.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageFilters.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	DTYPE	mlib_d64
#define	TTYPE	DTYPE
#define	STYPE	__m128d
#define	SNUM	2

#define	S_LD    _mm_loadu_pd
#define	S_STO   _mm_storeu_pd
#define	S_SET   _mm_set1_pd
#define	S_MUL   _mm_mul_pd
#define	S_ADD   _mm_add_pd

#define	FUNC_NAME(CC)	mlib_ImageZoomBicubic_D64_##CC

/* *********************************************************** */

#include <mlib_s_ImageZoom_BC.h>

/* *********************************************************** */
