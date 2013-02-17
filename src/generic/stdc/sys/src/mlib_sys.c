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

#pragma ident	"@(#)mlib_sys.c	9.5	07/10/29 SMI"

#include <stdlib.h>
#if defined __INTEL_COMPILER
/*
 * Fixed warning for icc compiler.
 */
void *memset(void *s1, int c, size_t n);
void *memcpy(void *s1, const void *s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
#else
#include <string.h>
#endif
#include <mlib_types.h>
#include <mlib_sys_proto.h>
#include <mlib_SysMath.h>

#if defined(__ICL)
#include <xmmintrin.h>
#endif /* !(defined(__ICL)) */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_memmove = __mlib_memmove
#pragma weak mlib_malloc = __mlib_malloc
#pragma weak mlib_realloc = __mlib_realloc
#pragma weak mlib_free = __mlib_free
#pragma weak mlib_memset = __mlib_memset
#pragma weak mlib_memcpy = __mlib_memcpy

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_memmove) mlib_memmove
    __attribute__((weak, alias("__mlib_memmove")));
__typeof__(__mlib_malloc) mlib_malloc
    __attribute__((weak, alias("__mlib_malloc")));
__typeof__(__mlib_realloc) mlib_realloc
    __attribute__((weak, alias("__mlib_realloc")));
__typeof__(__mlib_free) mlib_free __attribute__((weak, alias("__mlib_free")));
__typeof__(__mlib_memset) mlib_memset
    __attribute__((weak, alias("__mlib_memset")));
__typeof__(__mlib_memcpy) mlib_memcpy
    __attribute__((weak, alias("__mlib_memcpy")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#if defined(__SUNPRO_C) && defined(MLIB_USE_MEMORY_WRAPPER)

void *__special_mlib_malloc(size_t size);
void *__special_mlib_realloc(void *ptr, size_t size);
void __special_mlib_free(void *ptr);

#pragma weak __special_mlib_malloc
#pragma weak __special_mlib_free
#pragma weak __special_mlib_realloc

#endif

void *
__mlib_malloc(
    size_t size)
{
#ifdef _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment. -- from stdlib.h of MS VC++5.0.
 */
#if defined(__ICL)
	return ((void *)_mm_malloc(size, 16));
#else
	return ((void *)malloc(size));

#endif /* !(defined(__ICL) */

#else /* _MSC_VER */

#if defined(__SUNPRO_C) && defined(MLIB_USE_MEMORY_WRAPPER)
	if (&__special_mlib_malloc)
		return (__special_mlib_malloc(size));
#endif
	return ((void *)memalign(16, size));
#endif /* _MSC_VER */
}

void *
__mlib_realloc(
    void *ptr,
    size_t size)
{
#if defined(__SUNPRO_C) && defined(MLIB_USE_MEMORY_WRAPPER)
	if (&__special_mlib_realloc)
		return (__special_mlib_realloc(ptr, size));
#endif
	return ((void *)realloc(ptr, size));
}

void
__mlib_free(
    void *ptr)
{
#if defined(__SUNPRO_C) && defined(MLIB_USE_MEMORY_WRAPPER)
	if (&__special_mlib_free)
		__special_mlib_free(ptr);
	else
#endif

#if defined(__ICL)
	_mm_free(ptr);
#else
	free(ptr);
#endif /* !(defined(__INTEL_COMPILER)) */

}

void *
__mlib_memset(
    void *s,
    mlib_s32 c,
    size_t n)
{
	return ((void *)memset(s, c, n));
}

void *
__mlib_memcpy(
    void *s1,
    const void *s2,
    size_t n)
{
	return ((void *)memcpy(s1, s2, n));
}

void *
__mlib_memmove(
    void *s1,
    const void *s2,
    size_t n)
{
	return ((void *)memmove(s1, s2, n));
}

#ifdef MLIB_NO_LIBSUNMATH

void
mlib_sincosf(
    mlib_f32 x,
    mlib_f32 *s,
    mlib_f32 *c)
{
	*s = (mlib_f32)sin(x);
	*c = (mlib_f32)cos(x);
}

#undef mlib_sinf
mlib_f32
mlib_sinf(mlib_f32 x)
{
	return ((mlib_f32)sin(x));
}

#undef mlib_cosf
mlib_f32
mlib_cosf(mlib_f32 x)
{
	return ((mlib_f32)cos(x));
}

#undef mlib_expf
mlib_f32
mlib_expf(mlib_f32 x)
{
	return ((mlib_f32)exp(x));
}

#undef mlib_logf
mlib_f32
mlib_logf(mlib_f32 x)
{
	return ((mlib_f32)log(x));
}

#undef mlib_sqrtf
mlib_f32
mlib_sqrtf(mlib_f32 x)
{
	return ((mlib_f32)sqrt(x));
}

#undef mlib_fabsf
mlib_f32
mlib_fabsf(mlib_f32 x)
{
	return ((mlib_f32)fabs(x));
}

#else	/* MLIB_NO_LIBSUNMATH */

#include <math.h>

void
mlib_sincosf(
    mlib_f32 x,
    mlib_f32 *s,
    mlib_f32 *c)
{
	sincosf(x, s, c);
}

mlib_f32
mlib_sinf(mlib_f32 x)
{
	return (sinf(x));
}

mlib_f32
mlib_cosf(mlib_f32 x)
{
	return (cosf(x));
}

mlib_f32
mlib_expf(mlib_f32 x)
{
	return (expf(x));
}

mlib_f32
mlib_logf(mlib_f32 x)
{
	return (logf(x));
}

mlib_f32
mlib_sqrtf(mlib_f32 x)
{
	return (sqrtf(x));
}

mlib_f32
mlib_fabsf(mlib_f32 x)
{
	return (fabsf(x));
}

#endif	/* MLIB_NO_LIBSUNMATH */

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 int1, int0;
#else		   /* _LITTLE_ENDIAN */
		mlib_s32 int0, int1;
#endif		   /* _LITTLE_ENDIAN */
	} two_int;
} type_union_mlib_d64;

#define	DVAIN52	4.503599627370496e15

/* *********************************************************** */

mlib_s32
mlib_ilogb(
    mlib_d64 X)
{
	type_union_mlib_d64 arg;
	mlib_s32 n;

	if (X == 0.0)
		return (-MLIB_S32_MAX);
	arg.db = X;
	n = arg.two_int.int0 & 0x7ff00000;

	if (n)
		n = (n < 0x7ff00000) ? (n >> 20) - 1023 : MLIB_S32_MAX;
	else {
		arg.db = X * DVAIN52;
		n = ((arg.two_int.int0 & 0x7ff00000) >> 20) - 1075;
	}

	return (n);
}

/* *********************************************************** */
