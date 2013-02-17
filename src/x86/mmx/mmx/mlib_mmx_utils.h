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

#ifndef _MLIB_MMX_UTILS_H
#define	_MLIB_MMX_UTILS_H

#pragma ident	"@(#)mlib_mmx_utils.h	9.10	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

/* *********************************************************** */

/* fpu control word */
#ifdef __ICL

#include <float.h>

#define	MLIB_SETCW(cw, cw_mask, old_cw)                         \
	old_cw = _control87(cw, cw_mask)

#define	MLIB_RC_MASK	_MCW_RC
#define	MLIB_RC_NEAR	_RC_NEAR
#define	MLIB_RC_DOWN	_RC_DOWN
#define	MLIB_RC_UP	_RC_UP
#define	MLIB_RC_ZERO	_RC_CHOP

#elif __INTEL_COMPILER

#define	MLIB_SETCW(cw, cw_mask, old_cw)

/*
 * #include <fpu_control.h>
 *
 * #define MLIB_SETCW(cw, cw_mask, old_cw) {                      \
 *  mlib_u16 _old_cw, _new_cw;                                    \
 *  _FPU_GETCW(_old_cw);                                          \
 *  _new_cw = ((_old_cw) &~ (cw_mask)) | ((cw) & (cw_mask));      \
 *  _FPU_SETCW(_new_cw);                                          \
 *  old_cw = _old_cw;                                             \
 * }
 *
 * #define MLIB_RC_MASK  _FPU_RC_ZERO
 * #define MLIB_RC_NEAR  _FPU_RC_NEAREST
 * #define MLIB_RC_DOWN  _FPU_RC_DOWN
 * #define MLIB_RC_UP    _FPU_RC_UP
 * #define MLIB_RC_ZERO  _FPU_RC_ZERO
 *
 */

#if !defined(__MEDIALIB_OLD_NAME)
#define	mlib_malloc	__mlib_malloc
#define	mlib_free	__mlib_free
#endif

#elif __SUNPRO_C >= 0x580

#define	MLIB_SETCW(cw, cw_mask, old_cw)

#if !defined(__MEDIALIB_OLD_NAME)
#define	mlib_malloc	__mlib_malloc
#define	mlib_free	__mlib_free
#endif

#else
#error "unknown x86 compiler"
#endif

/* *********************************************************** */

#ifdef	__SUNPRO_C

#pragma align 16(mlib_mask128_arr)
#pragma align 8(mlib_mask64_arr)
#pragma align 8(mlib_pst_32)
#pragma align 8(mlib_pst_16)
#pragma align 8(mlib_pst_8)

#pragma align 4(ltab_8)
#pragma align 4(rtab_8)
#pragma align 4(ltab_16)
#pragma align 4(rtab_16)
#pragma align 4(ltab_32)
#pragma align 4(rtab_32)

#endif /* __SUNPRO_C */

/* *********************************************************** */

extern const __m64 mlib_mask128_arr[];
extern const __m64 mlib_mask64_arr[];

/* *********************************************************** */

extern const __m64 mask_pst_32[];
extern const __m64 mask_pst_16[];
extern const __m64 mask_pst_8[];

extern const int ltab_8[];
extern const int rtab_8[];

extern const int ltab_16[];
extern const int rtab_16[];

extern const int ltab_32[];
extern const int rtab_32[];

/* *********************************************************** */

#define	_m_zero	_mm_setzero_si64()
#define	_m_ones _mm_set1_pi8(0xff)

/* *********************************************************** */

#define	_mm_mlib_setr_pi8(s7, s6, s5, s4, s3, s2, s1, s0)              \
	_mm_unpacklo_pi32(_mm_unpacklo_pi16(_mm_unpacklo_pi8           \
	    (_mm_cvtsi32_si64(s7), _mm_cvtsi32_si64(s6)),              \
	    _mm_unpacklo_pi8(_mm_cvtsi32_si64(s5),                     \
	    _mm_cvtsi32_si64(s4))),                                    \
	    _mm_unpacklo_pi16(_mm_unpacklo_pi8(_mm_cvtsi32_si64(s3),   \
	    _mm_cvtsi32_si64(s2)),                                     \
	    _mm_unpacklo_pi8(_mm_cvtsi32_si64(s1),                     \
	    _mm_cvtsi32_si64(s0))));

/* *********************************************************** */

#define	_mm_mlib_set_pi8(s7, s6, s5, s4, s3, s2, s1, s0)               \
	_mm_unpacklo_pi32(_mm_unpacklo_pi16(_mm_unpacklo_pi8           \
	    (_mm_cvtsi32_si64(s0), _mm_cvtsi32_si64(s1)),              \
	    _mm_unpacklo_pi8(_mm_cvtsi32_si64(s2),                     \
	    _mm_cvtsi32_si64(s3))),                                    \
	    _mm_unpacklo_pi16(_mm_unpacklo_pi8(_mm_cvtsi32_si64(s4),   \
	    _mm_cvtsi32_si64(s5)),                                     \
	    _mm_unpacklo_pi8(_mm_cvtsi32_si64(s6),                     \
	    _mm_cvtsi32_si64(s7))));

/* *********************************************************** */

#define	_mm_mlib_setr_pi16(s3, s2, s1, s0)                          \
	_mm_unpacklo_pi32(_mm_unpacklo_pi16(_mm_cvtsi32_si64(s3),   \
	    _mm_cvtsi32_si64(s2)),                                  \
	    _mm_unpacklo_pi16(_mm_cvtsi32_si64(s1),                 \
	    _mm_cvtsi32_si64(s0)));

/* *********************************************************** */

#define	_mm_mlib_set_pi16(s3, s2, s1, s0)                           \
	_mm_unpacklo_pi32(_mm_unpacklo_pi16(_mm_cvtsi32_si64(s0),   \
	    _mm_cvtsi32_si64(s1)),                                  \
	    _mm_unpacklo_pi16(_mm_cvtsi32_si64(s2),                 \
	    _mm_cvtsi32_si64(s0)));

/* *********************************************************** */

#define	_mm_mlib_setr_pi32(s1, s0)                                       \
	_mm_unpacklo_pi32(_mm_cvtsi32_si64(s1), _mm_cvtsi32_si64(s0));

/* *********************************************************** */

#define	_mm_mlib_set_pi32(s1, s0)                                        \
	_mm_unpacklo_pi32(_mm_cvtsi32_si64(s0), _mm_cvtsi32_si64(s1));

/* *********************************************************** */

#define	_m_pnot(s)	_mm_xor_si64(s, _m_ones)
#define	_mm_not_si64	_m_pnot

/* *********************************************************** */

/* Rewrite the mmx_pst_* macro to fix the data racing in MT mlib */

#define	mmx_pst_32(src, dst, mask)                                      \
	{                                                               \
	    __m64 __mask64 = mask_pst_32[mask & 0x3];                   \
	    mmx_pst_32m64(src, dst, __mask64)				\
	}

/* *********************************************************** */

#define	mmx_pst_32m64(src, dst, mask64)                               	\
	{								\
	    if (mmx_read_hi(mask64))					\
		((mlib_s32 *)(dst))[0] = mmx_read_hi(src);		\
	    if (mmx_read_lo(mask64))					\
		((mlib_s32 *)(dst))[1] = mmx_read_lo(src);		\
	}

/* *********************************************************** */

#define	mmx_pst_16(src, dst, mask)                                      \
	{								\
	    mlib_s32 tmask = mask & 0xF;				\
	    if (tmask) {						\
		if (tmask == 0xF) {					\
			*(__m64 *) (dst) = (src);              		\
		} else {						\
			__m64 __mask64 = mask_pst_16[tmask];          	\
			mmx_pst_16m64(src, dst, __mask64);		\
		}							\
	    }								\
	}

/* *********************************************************** */

#define	mmx_read_16(x)	((mlib_s16)((_m_to_int(x)) & 0xFFFF))

#define	mmx_pst_16m64(src, dst, mask64)              	                \
	{								\
	    mlib_s16 mtmp, stmp;					\
	    mlib_s32 iidx;						\
	    for (iidx = 0; iidx < 4; iidx++) {				\
		mtmp = mmx_read_16(mask64);				\
		if (mtmp) {						\
		    stmp = mmx_read_16(src);				\
		    ((mlib_s16 *)(dst))[iidx] = stmp;			\
		}							\
		mask64 = _mm_srli_si64(mask64, 16);			\
		src = _mm_srli_si64(src, 16);				\
	    }								\
	}

/* *********************************************************** */

#define	mmx_pst_8(src, dst, mask)                                       \
	{								\
	    mlib_s32 tmask = (mask) & 0xFF;				\
	    if (tmask) {						\
		if (tmask == 0xFF) {					\
			*(__m64 *) (dst) = (src);              		\
		} else {						\
			__m64 __mask64 = mask_pst_8[tmask];		\
			mmx_pst_8m64(src, dst, __mask64);		\
		}							\
	    }								\
	}

/* *********************************************************** */

#define	mmx_read_8(x)	((mlib_u8)((_m_to_int(x)) & 0xFF))

#define	mmx_pst_8m64(src, dst, mask64)              	                \
	{								\
	    mlib_u8 mtmp, stmp;						\
	    mlib_s32 iidx;						\
	    for (iidx = 0; iidx < 8; iidx++) {				\
		mtmp = mmx_read_8(mask64);				\
		if (mtmp) {						\
		    stmp = mmx_read_8(src);				\
		    ((mlib_u8 *)(dst))[iidx] = stmp;			\
		}							\
		mask64 = _mm_srli_si64(mask64, 8);			\
		src = _mm_srli_si64(src, 8);				\
	    }								\
	}

/* *********************************************************** */
/* read 64 bits from memory, then write to __m64 variable */

#define	mmx_write_64(x)							\
	_mm_set_pi32((int)((x) >> 32), (int)(x))

/* *********************************************************** */

#if 0
#define	mmx_pst_32(src, dst, mask)                                      \
	{                                                               \
	    __m64 __mask64 = mask_pst_32[mask & 0x3];                   \
	    __m64 __s0 = _mm_and_si64(__mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(__mask64, *(__m64 *) (dst));   \
	                                                                \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);                 \
	}


#define	mmx_pst_32m64(src, dst, mask64)                               \
	{                                                             \
	    __m64 __s0 = _mm_and_si64(mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(mask64, *(__m64 *) (dst));   \
	                                                              \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);               \
	}


#define	mmx_pst_16(src, dst, mask)                                      \
	{                                                               \
	    __m64 __mask64 = mask_pst_16[mask & 0xF];                   \
	    __m64 __s0 = _mm_and_si64(__mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(__mask64, *(__m64 *) (dst));   \
	                                                                \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);                 \
	}


#define	mmx_pst_16m64(src, dst, mask64)                               \
	{                                                             \
	    __m64 __s0 = _mm_and_si64(mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(mask64, *(__m64 *) (dst));   \
	                                                              \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);               \
	}


#define	mmx_pst_8(src, dst, mask)                                       \
	{                                                               \
	    __m64 __mask64 = mask_pst_8[mask & 0xFF];                   \
	    __m64 __s0 = _mm_and_si64(__mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(__mask64, *(__m64 *) (dst));   \
	                                                                \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);                 \
	}


#define	mmx_pst_8m64(src, dst, mask64)                                \
	{                                                             \
	    __m64 __s0 = _mm_and_si64(mask64, (src));                 \
	    __m64 __d0 = _mm_andnot_si64(mask64, *(__m64 *) (dst));   \
	                                                              \
	    *(__m64 *) (dst) = _mm_or_si64(__d0, __s0);               \
	}
#endif

/* *********************************************************** */

#define	mmx_edge8(s1, s2)                                                  \
	(((((unsigned int)s1) & ~0x7) ==                                   \
	    (((unsigned int)s2) & ~0x7))) ? ltab_8[(((unsigned int)s1) &   \
	    0x7)] & rtab_8[(((unsigned int)s2) & 0x7)] :                   \
	    ltab_8[(((unsigned int)s1) & 0x7)];

/* *********************************************************** */

#define	mmx_edge16(s1, s2)                                                \
	(((((unsigned int)s1) & ~0x7) ==                                  \
	    (((unsigned int)s2) & ~0x7))) ? ltab_16[(((unsigned int)s1)   \
	    & 0x7)] & rtab_16[(((unsigned int)s2) & 0x7)] :               \
	    ltab_16[(((unsigned int)s1) & 0x7)];

/* *********************************************************** */

#define	mmx_edge32(s1, s2)                                                \
	(((((unsigned int)s1) & ~0x7) ==                                  \
	    (((unsigned int)s2) & ~0x7))) ? ltab_32[(((unsigned int)s1)   \
	    & 0x7)] & rtab_32[(((unsigned int)s2) & 0x7)] :               \
	    ltab_32[(((unsigned int)s1) & 0x7)];

/* *********************************************************** */

#define	MLIB_REVERT_ENDIAN_8(x)                                            \
	(((unsigned int)(x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) &   \
	    0x0000FF00) << 8) | ((x) << 24))

#define	MLIB_REVERT_ENDIAN_16(x)                                \
	((((unsigned int)(x) >> 16) & 0xFFFF) | (((x) & 0xFFFF) << 16))

/* *********************************************************** */

#define	MMX_SCALE	(((MMX_GSR_VALUE) >> 3) & 15)
#define	MMX_ALIGN	(((MMX_GSR_VALUE) & 7) << 3)

/* *********************************************************** */

#define	mmx_fpadd16s(x, y)                                        \
	_m_to_int(_mm_add_pi16(_m_from_int(x), _m_from_int(y)))

#define	mmx_read_hi(x)	_m_to_int(x)

#define	mmx_read_lo(x)	_m_to_int(_mm_srli_si64(x, 32))

#define	mmx_freg_pair(x, y)                                     \
	_mm_unpacklo_pi32(_m_from_int(x), _m_from_int(y))

/*
 * #define mmx_freg_pair(x, y) \
 * _mm_or_si64(_mm_slli_si64(_m_from_int(y), 32), _m_from_int(x))
 */

#define	mmx_write_hi(x, y)                                        \
	_mm_unpacklo_pi32(_m_from_int(y), _mm_srli_si64(x, 32))

#define	mmx_write_lo(x, y)	_mm_unpacklo_pi32(x, _m_from_int(y))

#define	mmx_fpmerge(x, y)                                       \
	_mm_unpacklo_pi8(_m_from_int(x), _m_from_int(y))

#define	mmx_mul_f15(x, y)	_mm_slli_pi16(_mm_mulhi_pi16(x, y), 1)

#ifdef MMX_USE_MODIF_8x16

#ifdef MMX_USE_MEM_ZERO

#define	mmx_fmul8x16hi(x, y)                                             \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(x, _m_mzero[0]),   \
	    7), y)

#define	mmx_fmul8x16lo(x, y)                                             \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpackhi_pi8(x, _m_mzero[0]),   \
	    7), y)

#else

#define	mmx_fmul8x16hi(x, y)                                             \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(x, _m_zero), 7),   \
	    y)

#define	mmx_fmul8x16lo(x, y)                                             \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpackhi_pi8(x, _m_zero), 7),   \
	    y)

#endif /* MMX_USE_MEM_ZERO */

#else

#define	mmx_fmul8x16hi(x, y)                                             \
	mmx_mul_f15(_mm_slli_pi16(_mm_unpacklo_pi8(x, _m_zero), 7), y)

#define	mmx_fmul8x16lo(x, y)                                             \
	mmx_mul_f15(_mm_slli_pi16(_mm_unpackhi_pi8(x, _m_zero), 7), y)

#endif /* MMX_USE_MODIF_8x16 */

#define	mmx_fmul8x16(x, y)	mmx_fmul8x16hi(_m_from_int(x), y)

#define	mmx_unpackhi4_pi16(y)                                    \
	_mm_unpacklo_pi16(_mm_unpacklo_pi16(_m_from_int(y),      \
	    _m_from_int(y)), _mm_unpacklo_pi16(_m_from_int(y),   \
	    _m_from_int(y)))

#define	mmx_unpacklo4_pi16(y)                                    \
	_mm_unpackhi_pi16(_mm_unpacklo_pi16(_m_from_int(y),      \
	    _m_from_int(y)), _mm_unpacklo_pi16(_m_from_int(y),   \
	    _m_from_int(y)))

#define	mmx_fmul8x16au(x, y)	mmx_fmul8x16(x, mmx_unpackhi4_pi16(y))

#define	mmx_fmul8x16al(x, y)	mmx_fmul8x16(x, mmx_unpacklo4_pi16(y))

#define	mmx_fmul8x16au_hi(x, y)                                 \
	mmx_fmul8x16hi(x, mmx_unpackhi4_pi16(y))

#define	mmx_fmul8x16al_hi(x, y)                                 \
	mmx_fmul8x16hi(x, mmx_unpacklo4_pi16(y))

#define	mmx_muld16x16_hi(x, y)                                          \
	_mm_unpacklo_pi16(_mm_mullo_pi16(x, y), _mm_mulhi_pi16(x, y))

#define	mmx_muld16x16_lo(x, y)                                          \
	_mm_unpackhi_pi16(_mm_mullo_pi16(x, y), _mm_mulhi_pi16(x, y))

#define	mmx_muld16x16(x, y)                                     \
	mmx_muld16x16_hi(_m_from_int(x), _m_from_int(y))

#define	mmx_fmuld8ulx16(x, y)                                           \
	mmx_muld16x16_hi(_m_from_int((x) & 0xFF00FF), _m_from_int(y))

#define	mmx_fmuld8sux16(x, y)                                            \
	mmx_muld16x16_hi(_m_from_int((x) & ~0xFF00FF), _m_from_int(y))

#define	mmx_mul_16x16(r, x, y)                                  \
	tmpa = _mm_mullo_pi16(x, y);                            \
	tmpb = _mm_mulhi_pi16(x, y);                            \
	r##h = _mm_unpacklo_pi16(tmpa, tmpb);                   \
	r##l = _mm_unpackhi_pi16(tmpa, tmpb)

#define	mmx_add_d32(r, x)                                       \
	r##h = _mm_add_pi32(r##h, x##h);                        \
	r##l = _mm_add_pi32(r##l, x##l)

#define	mmx_srai_d32(r, x)                                      \
	r##h = _mm_srai_pi32(r##h, x);                          \
	r##l = _mm_srai_pi32(r##l, x)

#define	mmx_madd_16x16(r, x, y)                                 \
	mmx_mul_16x16(tmp, x, y);                               \
	mmx_add_d32(r, tmp)

/* *********************************************************** */

#ifdef MMX_USE_MODIF_8x16

#ifdef MMX_USE_MEM_ZERO

#define	mmx_fpack16(x)                                              \
	_m_to_int(_mm_packs_pu16(_mm_srai_pi16(x, 6 - MMX_SCALE),   \
	    _m_mzero[0]))

#else

#define	mmx_fpack16(x)                                              \
	_m_to_int(_mm_packs_pu16(_mm_srai_pi16(x, 6 - MMX_SCALE),   \
	    _m_zero))

#endif /* MMX_USE_MEM_ZERO */

#define	mmx_fpack16_pair(x, y)                                             \
	_mm_packs_pu16(_mm_srai_pi16(x, 6 - MMX_SCALE), _mm_srai_pi16(y,   \
	    6 - MMX_SCALE))

#else

#define	mmx_fpack16(x)                                              \
	_m_to_int(_mm_packs_pu16(_mm_srai_pi16(x, 7 - MMX_SCALE),   \
	    _m_zero))

#define	mmx_fpack16_pair(x, y)                                             \
	_mm_packs_pu16(_mm_srai_pi16(x, 7 - MMX_SCALE), _mm_srai_pi16(y,   \
	    7 - MMX_SCALE))

#endif /* MMX_USE_MODIF_8x16 */

#define	mmx_fpack16_to_hi(x, y)	mmx_write_hi(x, mmx_fpack16(y))

#define	mmx_fpack16_to_lo(x, y)	mmx_write_lo(x, mmx_fpack16(y))

#define	mmx_fpackfix_pair(x, y)                                 \
	_mm_packs_pi32(_mm_srai_pi32(x, 16 - MMX_SCALE),        \
	    _mm_srai_pi32(y, 16 - MMX_SCALE))

#define	mmx_faligndata(x, y)                                        \
	_mm_or_si64(_mm_srli_si64(x, MMX_ALIGN), _mm_slli_si64(y,   \
	    64 - MMX_ALIGN))

/*
 * #define mmx_to_double(x, y) \
 * mmx_freg_pair(MLIB_REVERT_ENDIAN_16(x), MLIB_REVERT_ENDIAN_16(y))
 */

#define	mmx_to_double(x, y)                                                \
	_mm_or_si64(_mm_slli_si64(_m_from_int(MLIB_REVERT_ENDIAN_16(y)),   \
	    32), _m_from_int(MLIB_REVERT_ENDIAN_16(x)))

#define	mmx_from_int_dup(x)                                     \
	_mm_unpacklo_pi32(_m_from_int(x), _m_from_int(x))

#define	mmx_to_double_dup(x)	mmx_to_double(x, x)

#ifdef MMX_TO_FLOAT_U8
#define	mmx_to_float(x)	MLIB_REVERT_ENDIAN_8(x)
#else
#define	mmx_to_float(x)	MLIB_REVERT_ENDIAN_16(x)
#endif /* MMX_TO_FLOAT_U8 */

#define	mmx_alignaddr(ptr, i)                                   \
	(void *)((unsigned char *)ptr + i - (MMX_ALIGN =        \
	    8 * ((int)((unsigned char *)ptr + i) & 7)) / 8)

#define	mmx_ld_u8(ptr)	*(__m64*)((unsigned char *)(ptr) - 7)

#define	mmx_ld_u16(ptr)	*(__m64*)((unsigned char *)(ptr) - 6)

#define	mmx_ld_u8_i(ptr, ind)                                   \
	* (__m64 *) ((unsigned char *)(ptr) + (ind) - 7)

#define	mmx_ld_u16_i(ptr, ind)                                  \
	* (__m64 *) ((unsigned char *)(ptr) + (ind) - 6)

#define	mmx_st_u8(val, ptr)                                     \
	* (unsigned char *)(ptr) = *(unsigned char *)&val

#define	mmx_st_u16(val, ptr)                                    \
	* (unsigned short *)(ptr) = *(unsigned short *)&val

/* *********************************************************** */

#define	ARG0(x, y)	(x)

#define	MMX_MERGE4_U8(a, b, c, d)                                   \
	_m_from_int((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#define	MMX_MERGE4_U16(a, b, c, d)                              \
	mmx_freg_pair((a) | ((b) << 16), (c) | ((d) << 16))

#define	MMX_ALIGNDATA(x, y, al)                                  \
	_mm_or_si64(_mm_srli_si64(x, 8 * al), _mm_slli_si64(y,   \
	    64 - 8 * al))

/* *********************************************************** */

#if 0

/*
 *
 * Utility Inlines
 *
 */

#define	vis_start()	unsigned int gsr_register = 0
#define	vis_end	_m_empty

#define	vis_write_gsr(gsr_val /* GSR */)	gsr_register = gsr_val;

#define	vis_read_gsr()	gsr_register

#define	vis_read_hi(x)	_m_to_int(x)

#define	vis_read_lo(x)	_m_to_int(_mm_srli_si64(x, 32))

#define	vis_freg_pair(x, y)	_mm_unpacklo_pi32(_m_from_int(x), \
	_m_from_int(y))

#define	_MLIB_REVERT_ENDIAN_8(x)                                           \
	(((unsigned int)(x) >> 24) | (((unsigned int)(x) & 0x00FF0000)     \
	    >> 8) | (((unsigned int)(x) & 0x0000FF00) << 8) | ((unsigned   \
	    int)(x) << 24))

#define	_MLIB_REVERT_ENDIAN_16(x)                                   \
	(((unsigned long)(x) >> 16) | ((unsigned long)(x) << 16))

#define	vis_to_float(x)	_MLIB_REVERT_ENDIAN_16(x)

#define	vis_to_double(x, y)                                            \
	_mm_or_si64(_mm_slli_si64(_m_from_int(_MLIB_REVERT_ENDIAN_16   \
	    (y)), 32), _m_from_int(_MLIB_REVERT_ENDIAN_16(x)))

/*
 *
 * VIS Logical Instructions
 *
 */

#define	vis_fzero	_mm_setzero_si64

/*
 *
 * Arithmetic Instructions
 *
 */

#define	vis_fpadd16(x, y)	_mm_add_pi16(x, y)

#define	vis_fpsub16(x, y)	_mm_sub_pi16(x, y)

#define	_mmx_mul_f15(x, y)	_mm_slli_pi16(_mm_mulhi_pi16(x, y), 1)

#define	_mmx_fmul8x16hi(x, y)                                   \
	_mmx_mul_f15(_mm_slli_pi16(_mm_unpacklo_pi8(x,          \
	    _mm_setzero_si64()), 7), y)

#define	_mmx_fmul8x16lo(x, y)                                   \
	_mmx_mul_f15(_mm_slli_pi16(_mm_unpackhi_pi8(x,          \
	    _mm_setzero_si64()), 7), y)

#define	vis_fmul8x16(x, y)	_mmx_fmul8x16hi(_m_from_int(x), y)

#define	_mmx_unpackhi4_pi16(y)                                   \
	_mm_unpacklo_pi16(_mm_unpacklo_pi16(_m_from_int(y),      \
	    _m_from_int(y)), _mm_unpacklo_pi16(_m_from_int(y),   \
	    _m_from_int(y)))

#define	_mmx_unpacklo4_pi16(y)                                   \
	_mm_unpackhi_pi16(_mm_unpacklo_pi16(_m_from_int(y),      \
	    _m_from_int(y)), _mm_unpacklo_pi16(_m_from_int(y),   \
	    _m_from_int(y)))

#define	vis_fmul8x16au(x, y)	vis_fmul8x16(x, _mmx_unpackhi4_pi16(y))

#define	vis_fmul8x16al(x, y)	vis_fmul8x16(x, _mmx_unpacklo4_pi16(y))

#define	_mmx_fmul8x16au_hi(x, y)                                \
	_mmx_fmul8x16hi(x, _mmx_unpackhi4_pi16(y))

#define	_mmx_fmul8x16al_hi(x, y)                                \
	_mmx_fmul8x16hi(x, _mmx_unpacklo4_pi16(y))

#define	_mmx_muld16x16_hi(x, y)                                         \
	_mm_unpacklo_pi16(_mm_mullo_pi16(x, y), _mm_mulhi_pi16(x, y))

#define	_mmx_muld16x16_lo(x, y)                                         \
	_mm_unpackhi_pi16(_mm_mullo_pi16(x, y), _mm_mulhi_pi16(x, y))

#define	_mmx_muld16x16(x, y)                                      \
	_mmx_muld16x16_hi(_m_from_int(x), _m_from_int(y))

#define	vis_fmuld8ulx16(x, y)                                           \
	_mmx_muld16x16_hi(_m_from_int((x) & 0xFF00FF), _m_from_int(y))

#define	vis_fmuld8sux16(x, y)                                           \
	_mmx_muld16x16_hi(_m_from_int((x) & ~0xFF00FF), _m_from_int(y))

/*
 *
 * Pixel Formatting Instructions
 *
 */

#define	SCALE_VALUE	((vis_read_gsr() >> 3) & 15)

#define	vis_fpack16(x)	_m_to_int(_mm_packs_pu16(_mm_srai_pi16(x, \
	(7 - SCALE_VALUE)), vis_fzero()))

#define	vis_fpack16_pair(x, y)	_mm_packs_pu16(_mm_srai_pi16(x, \
	7 - SCALE_VALUE), _mm_srai_pi16(y, 7 - SCALE_VALUE))

#define	OFFSET(ptr, offset)	(gsr_register = (gsr_register & 0x78) | \
	(((unsigned int)ptr  + offset) & 7))

#define	vis_alignaddr(ptr, offset)	(void *)((unsigned char *)ptr + \
	offset - (OFFSET(ptr, offset) & 7))

static inline __m64
_mmx_faligndata(
    __m64 frs1,
    __m64 frs2,
    int shift1,
    int shift2)
{
	frs1 = _m_psrlqi(frs1, (shift1 << 3));
	frs2 = _m_psllqi(frs2, (shift2 << 3));

	return (_m_por(frs1, frs2));
}

#define	vis_faligndata(x, y)                                    \
	_mmx_faligndata(x, y, (vis_read_gsr() & 0x7),           \
	    (8 - (vis_read_gsr() & 0x7)))

#define	vis_edge8(s1, s2)                                                  \
	(((((unsigned int)s1) & ~0x7) ==                                   \
	    (((unsigned int)s2) & ~0x7))) ? ltab_8[(((unsigned int)s1) &   \
	    0x7)] & rtab_8[(((unsigned int)s2) & 0x7)] :                   \
	    ltab_8[(((unsigned int)s1) & 0x7)]

#define	vis_edge16(s1, s2)                                                \
	(((((unsigned int)s1) & ~0x7) ==                                  \
	    (((unsigned int)s2) & ~0x7))) ? ltab_16[(((unsigned int)s1)   \
	    & 0x7)] & rtab_16[(((unsigned int)s2) & 0x7)] :               \
	    ltab_16[(((unsigned int)s1) & 0x7)]

#define	vis_edge32(s1, s2)                                                \
	(((((unsigned int)s1) & ~0x7) ==                                  \
	    (((unsigned int)s2) & ~0x7))) ? ltab_32[(((unsigned int)s1)   \
	    & 0x7)] & rtab_32[(((unsigned int)s2) & 0x7)] :               \
	    ltab_32[(((unsigned int)s1) & 0x7)]

static inline __m64
_mmx_pst(
    __m64 src,
    __m64 dst,
    __m64 mask64)
{
	src = _m_pand(mask64, src);
	dst = _m_pandn(mask64, dst);

	return (_m_por(src, dst));
}

#define	vis_pst_32(src, dst, mask)                                \
	* (__m64 *) (dst) =                                       \
	_mmx_pst(src, *(__m64 *) dst, mask_pst_32[mask & 0x3]);

#define	vis_pst_16(src, dst, mask)                                \
	* (__m64 *) (dst) =                                       \
	_mmx_pst(src, *(__m64 *) dst, mask_pst_16[mask & 0xf]);

static inline void
vis_pst_8(
    __m64 src,
    void *dst,
    int mask)
{
	*(__m64 *) (dst) =
	    _mmx_pst(src, *(__m64 *) dst, mask_pst_8[mask & 0xff]);
}

static inline __m64
vis_ld_u8(
    unsigned char *ptr)
{
	long lptr = (long)ptr;
	long ladr = lptr & ~7;

	return _m_psrlqi(_m_psllqi(*((__m64 *) ladr),
	    (((ladr + 7) - lptr) << 3)), 56);
}

static inline __m64
vis_ld_u8_i(
    unsigned char *ptr,
    int ind)
{
	long lptr = (long)ptr + ind;
	long ladr = (long)lptr & ~7;

	return _m_psllqi(_m_psrlqi(*((__m64 *) ladr), ((lptr - ladr) << 3)),
	    56);
}

static inline __m64
vis_ld_d64_nf(
    __m64 * addr)
{
	return (*(addr));
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_MMX_UTILS_H */
