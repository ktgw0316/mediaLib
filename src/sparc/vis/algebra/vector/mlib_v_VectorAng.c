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

#pragma ident	"@(#)mlib_v_VectorAng.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAng_U8C - phase of unsigned 8-bit format complex vector
 *      mlib_VectorAng_S8C - phase of signed 8-bit format complex vector
 *      mlib_VectorAng_S16C - phase of 16-bit format complex vector
 *      mlib_VectorAng_S32C - phase of 32-bit format complex vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorAng_U8C(mlib_d64      * a,
 *                                     const mlib_u8 * x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorAng_S8C(mlib_d64      *a,
 *                                     const mlib_s8 *x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorAng_S16C(mlib_d64       *a,
 *                                      const mlib_s16 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorAng_S32C(mlib_d64       *a,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      a    pointer to the output phase vector
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the x vector
 *
 * DESCRIPTION
 *      a = atan2(x_imag, x_real)
 *      function atan2 return a value in radians, in the range of -pi to pi.
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAng_S16C = __mlib_VectorAng_S16C
#pragma weak mlib_VectorAng_S8C = __mlib_VectorAng_S8C
#pragma weak mlib_VectorAng_U8C = __mlib_VectorAng_U8C
#pragma weak mlib_VectorAng_S32C = __mlib_VectorAng_S32C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorAng_S16C) mlib_VectorAng_S16C
	__attribute__((weak, alias("__mlib_VectorAng_S16C")));
__typeof__(__mlib_VectorAng_S8C) mlib_VectorAng_S8C
	__attribute__((weak, alias("__mlib_VectorAng_S8C")));
__typeof__(__mlib_VectorAng_U8C) mlib_VectorAng_U8C
	__attribute__((weak, alias("__mlib_VectorAng_U8C")));
__typeof__(__mlib_VectorAng_S32C) mlib_VectorAng_S32C
	__attribute__((weak, alias("__mlib_VectorAng_S32C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 *        Table of constants for mlib_atan2i
 */

/* *********************************************************** */

#define	PI	3.1415926535897931160e+00
#define	PION2	1.57079632679489655800e+00
#define	KA9	1.00523913835726148843e-01
#define	KA7	-1.42452689179072812273e-01
#define	KA5	1.99994988952939798604e-01
#define	KA3	-3.33333323562239806408e-01
#define	KB3	-3.33327229950751257532e-01

/* *********************************************************** */

/*  Table for S8/U8 : x[i] = (mlib_d64)(i)   -128 <= i <= 255  */

static const mlib_d64 itod[] = {
	128, 127, 126, 125, 124, 123, 122, 121, 120, 119,
	118, 117, 116, 115, 114, 113, 112, 111, 110, 109,
	108, 107, 106, 105, 104, 103, 102, 101, 100, 99,
	98, 97, 96, 95, 94, 93, 92, 91, 90, 89,
	88, 87, 86, 85, 84, 83, 82, 81, 80, 79,
	78, 77, 76, 75, 74, 73, 72, 71, 70, 69,
	68, 67, 66, 65, 64, 63, 62, 61, 60, 59,
	58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
	48, 47, 46, 45, 44, 43, 42, 41, 40, 39,
	38, 37, 36, 35, 34, 33, 32, 31, 30, 29,
	28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
	18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
	8, 7, 6, 5, 4, 3, 2, 1, 0, 1,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
	42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
	62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
	82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
	92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
	102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
	122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
	132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
	142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
	152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
	162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
	172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
	182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
	192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
	202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
	212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
	222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
	232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
	242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
	252, 253, 254, 255
};

/* *********************************************************** */

#define	MLIB_VECTOR_ANG_ATAN2I                                         \
	if (arg2 < arg1) {                                             \
	    darg = arg2 / arg1;                                        \
	    if (darg <= 0.25) {                                        \
		yv2 = darg * darg;                                     \
		y = darg + yv2 * darg * (KA5 * (div1 + yv2) +          \
			KA9 * (div2 + yv2) * (yv2 * yv2));             \
	    } else {                                                   \
		y = darg;                                              \
		darg = vis_fand(darg, vis_to_double(0x3fffc000, 0));   \
		y = (y - darg) / (1.0 + y * darg);                     \
		farg = vis_fpsub32s(vis_read_hi(darg),                 \
			vis_to_float(0x3fd00000));                     \
		ind = *((mlib_s32 *)&farg) >> 14;                      \
		y = y + mlib_constarctgf[ind] + KB3 * y * (y * y);     \
	    }                                                          \
	    y = (xx < 0) ? PI - y : y;                                 \
	    a[i] = (yy < 0) ? -y : y;                                  \
	} else {                                                       \
	    if (arg2 == 0.) {                                          \
		a[i] = 0.;                                             \
		continue;                                              \
	    }                                                          \
	    darg = arg1 / arg2;                                        \
	    if (darg <= 0.25) {                                        \
		yv2 = darg * darg;                                     \
		y = darg + yv2 * darg * (KA5 * (div1 + yv2) +          \
			KA9 * (div2 + yv2) * (yv2 * yv2));             \
	    } else {                                                   \
		y = darg;                                              \
		darg = vis_fand(darg, vis_to_double(0x3fffc000, 0));   \
		y = (y - darg) / (1.0 + y * darg);                     \
		farg = vis_fpsub32s(vis_read_hi(darg),                 \
			vis_to_float(0x3fd00000));                     \
		ind = *((mlib_s32 *)&farg) >> 14;                      \
		y = y + mlib_constarctgf[ind] + KB3 * y * (y * y);     \
	    }                                                          \
	    y = (xx < 0) ? PION2 + y : PION2 - y;                      \
	    a[i] = (yy < 0) ? -y : y;                                  \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_U8C(
	mlib_d64 *a,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 y, yv2;
	mlib_s32 ind, xx, yy;
	const mlib_d64 *ptab = itod + 128;
	mlib_f32 farg;
	mlib_d64 arg1, arg2, darg;
	mlib_d64 div1 = KA3 / KA5, div2 = KA7 / KA9;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		xx = x[2 * i];
		yy = x[2 * i + 1];

/*  fabs  */
		arg1 = ptab[xx];
		arg2 = ptab[yy];

	MLIB_VECTOR_ANG_ATAN2I}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S8C(
	mlib_d64 *a,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 y, yv2;
	mlib_s32 ind, xx, yy;
	const mlib_d64 *ptab = itod + 128;
	mlib_f32 farg;
	mlib_d64 arg1, arg2, darg;
	mlib_d64 div1 = KA3 / KA5, div2 = KA7 / KA9;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		xx = x[2 * i];
		yy = x[2 * i + 1];

/*  fabs  */
		arg1 = ptab[xx];
		arg2 = ptab[yy];

	MLIB_VECTOR_ANG_ATAN2I}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S16C(
	mlib_d64 *a,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i, ind;
	mlib_d64 y, yv2;
	mlib_d64 xx, yy, xx0, yy0, xx1, yy1;
	mlib_f32 farg;
	mlib_d64 arg1, arg2, darg;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_d64 div1 = KA3 / KA5, div2 = KA7 / KA9;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		xx0 = vis_ld_u16(px + 2 * i);
		yy0 = vis_ld_u16(px + 2 * i + 1);
		xx1 = vis_fmuld8ulx16(fone, vis_read_lo(xx0));
		yy1 = vis_fmuld8ulx16(fone, vis_read_lo(yy0));
		xx = *((mlib_s32 *)&xx1 + 1);
		yy = *((mlib_s32 *)&yy1 + 1);

		arg1 = mlib_fabs(xx);
		arg2 = mlib_fabs(yy);

	MLIB_VECTOR_ANG_ATAN2I}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S32C(
	mlib_d64 *a,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i, ind;
	mlib_d64 y, yv2;
	mlib_d64 xx, yy;
	mlib_f32 farg;
	mlib_d64 arg1, arg2, darg;
	mlib_d64 div1 = KA3 / KA5, div2 = KA7 / KA9;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		xx = x[2 * i];
		yy = x[2 * i + 1];

		arg1 = mlib_fabs(xx);
		arg2 = mlib_fabs(yy);

	MLIB_VECTOR_ANG_ATAN2I}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
