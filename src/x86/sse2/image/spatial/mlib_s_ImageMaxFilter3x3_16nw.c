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

#pragma ident   "@(#)mlib_s_ImageMaxFilter3x3_16nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *              Internal functions for 3x3 maximum filter or minimum filter
 *              on the input image. Edge = no write.
 *              The compiler switch MAX_FILTER controls the macro
 *              C_COMP, #defining it as max() if true, or
 *              as min() if false. It also controls the function
 *              name. (Comments and #defined macro names always refer
 *              to "max" instead of "min".)
 *
 * ARGUMENT
 *     dst    Pointer to the first byte for first pixel of destination
 *     src    Pointer to the first byte for first pixel of source
 *     slb    Bytes count of the source row
 *     dlb    Bytes count of the destination row
 *     wid    number of elements in a row of source
 *     hgt    number of rows of source
 *
 * DESCRIPTION
 *      Does a 3x3 maximum filter on the input image. The filtered values are
 *      returned in the destination image.
 *
 * NOTE TO MAINTENANCE PROGRAMMERS !!!!!!!!!
 *
 *      If you change this min or max filter, just change one, then copy it
 *      over the other one, changing only the true/false (1/0) value of
 *      the #defined switch MAX_FILTER. For library purposes, two files are
 *      probably necessary, but keep them in sync so that their only
 *      difference is the value of the MAX_FILTER switch.
 *
 */

#include <mlib_image.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MAX_FILTER	1

#define	IMG_TYPE    2

#include <mlib_s_ImageMaxMinFilter3x3.h>

/* *********************************************************** */
