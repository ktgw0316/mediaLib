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

#ifndef _MLIB_SYSMATH_H
#define	_MLIB_SYSMATH_H

#pragma ident	"@(#)mlib_SysMath.h	9.4	07/10/29 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <math.h>
#ifdef _MSC_VER
#define	M_PI	3.14159265358979323846
#define	M_1_PI	0.31830988618379067154
#endif /* _MSC_VER */

#if defined(__INTEL_COMPILER) && defined(_USE_FOOMATH)
double __foo_acos(double);
double __foo_cos(double);
double __foo_sin(double);

#define	mlib_acos	__foo_acos
#define	mlib_sin	__foo_sin
#define	mlib_cos	__foo_cos
#else /* (__INTEL_COMPILER) && (_USE_FOOMATH) */
#define	mlib_acos	acos
#define	mlib_sin	sin
#define	mlib_cos	cos
#endif /* (__INTEL_COMPILER) && (_USE_FOOMATH) */

#define	mlib_fabs	fabs
#define	mlib_ceil	ceil
#define	mlib_abs	abs
#define	mlib_llabs	llabs

#ifdef MLIB_LIBCAFEMATH

#include <stdlib.h>

#define	mlib_sqrt	mlib_sqrt_cafe
#define	mlib_sinf	sinf
#define	mlib_cosf	cosf
void mlib_sincosf(
    float x,
    float *s,
    float *c);

#define	mlib_sqrtf	mlib_sqrtf_cafe
#define	mlib_fabsf	fabsf

double mlib_sqrt_cafe(
    double x);
float mlib_sqrtf_cafe(
    float x);

#else

#define	mlib_sqrt	sqrt

#ifdef MLIB_NO_LIBSUNMATH

#define	mlib_sinf	(float)sin
#define	mlib_cosf	(float)cos
void mlib_sincosf(
    float x,
    float *s,
    float *c);

#define	mlib_sqrtf	(float)sqrt
#define	mlib_fabsf	(float)fabs

#else

#include <sunmath.h>

float mlib_sinf(
    float x);
float mlib_cosf(
    float x);
float mlib_logf(
    float x);
float mlib_expf(
    float x);

void
mlib_sincosf(
    float x,
    float *s,
    float *c);

float
mlib_sqrtf(
    float x);

float
mlib_fabsf(
    float x);

#endif /* MLIB_NO_LIBSUNMATH */

#endif /* MLIB_LIBCAFEMATH */

/* internal mathematical functions */

double mlib_sincospi(
    double x,
    double *co);
double mlib_atan2i(
    int y,
    int x);
int mlib_ilogb(
    double x);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SYSMATH_H */
