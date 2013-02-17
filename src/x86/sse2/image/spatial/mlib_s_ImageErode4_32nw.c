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

#pragma ident   "@(#)mlib_s_ImageErode4_32nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDilate4_U8.c -
 *      OR:
 *      mlib_ImageErode4_U8.c -
 *              Does a 3x3 erode 4 or dilate 4 filter on the
 *              8-bit input image. Edge policy = no write.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDilate4_S32(void     *dst,
 *                                        void     *src,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 slb,
 *                                        mlib_s32 wid,
 *                                        mlib_s32 hgt)
 *      OR
 *      mlib_status mlib_ImageErode4_S32(void     *dst,
 *                                       void     *src,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 slb,
 *                                       mlib_s32 wid,
 *                                       mlib_s32 hgt)
 *
 * ARGUMENTS
 *      dst     Pointer to the first byte for first pixel of destination
 *      src     Pointer to the first byte for first pixel of source
 *      dlb     Bytes count of the destination row
 *      slb     Bytes count of the source row
 *      w       number of elements in a irow of source
 *      h       number of rows of source
 *
 * DESCRIPTION
 *        Does a 3x3 erode 4 or dilate 4 filter on the 8-bit input image.
 *      Edge = no write. Filtered values are returned in the destination
 *      image.
 *        The compiler switch DILATE_FILTER controls the macro C_COMP,
 *      #defining it as max() if true, or as min() if false. It also
 *      controls the function name. (Comments and #defined macro names
 *      usually refer to "max" instead of "min".)
 *        Uses the dilate4 neighborhood (not the dilate8):
 *              Dilate4 neighborhood:           Dilate8 neighborhood:
 *                        o                             o o o
 *                      o o o                           o o o
 *                        o                             o o o
 *
 * NOTE TO MAINTENANCE PROGRAMMERS !!!!!!!!!
 *
 *      If you change this erosion or dilation filter, just change one,
 *      then copy it over the other one, changing only the true/false (1/0)
 *      value of the #defined switch DILATE_FILTER. For library purposes,
 *      two files are probably necessary, but keep them in sync so that
 *      their only difference is the value of the DILATE_FILTER switch.
 *
 * ***************************************************************************
 */

#include <mlib_image.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	DILATE_FILTER	0

#define	IMG_TYPE    4

#include <mlib_s_ImageDilateErode4.h>

/* *********************************************************** */
