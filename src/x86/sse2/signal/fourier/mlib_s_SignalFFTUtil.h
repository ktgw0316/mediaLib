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

#ifndef _MLIB_S_SIGNALFFTUTIL_H
#define	_MLIB_S_SIGNALFFTUTIL_H

#pragma ident	"@(#)mlib_s_SignalFFTUtil.h	9.4	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
/* cache sizes, layers strategies etc... */
#include <mlib_s_SignalFFTSettings.h>

/* *********************************************************** */
/* preface for func prototypes common in C and VIS */
/* *********************************************************** */

/* macros, typedefs */
/* type for input-output data in *D64* functions */
typedef mlib_d64 data_t;

/* textually separates 'my' consts from 'normal' ones */
/* const */
#define	CONST

/* boolean semantics support */
typedef mlib_s32 BOOL;

#define	FALSE	0
#define	TRUE	(!FALSE)

/* controls ( TRUE or FALSE ) whether deal_t is float */
#define	DEAL_F32	!TRUE

/* type to perform ALL intermediate processing */

#if DEAL_F32

/* parametrize it! */
typedef mlib_f32 deal_t;

#else

/* parametrize it! */
typedef mlib_d64 deal_t;

#endif /* DEAL_F32 */

/* complex type for deal_t */
typedef struct dtCmplxTag
{
	deal_t r, i;
} dtCmplx;

/* type returned by kernel(s) */
typedef mlib_s32 kernel_ret_t;

/* *********************************************************** */
/* function prototypes */

mlib_status mlib_c_fftDispatcherW_1(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_c_fftDispatcherW_2(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_c_fftDispatcherW_3(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_c_fftDispatcherW_4(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scalef,
    mlib_s32 CONST data_info_flags);

mlib_s32 mlib_c_fftKernel_4(
    deal_t *CONST pal64data,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

mlib_status mlib_c_fftDispatcherW_S16C_S16(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_c_fftDispatcherW_4_S16C_S16(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scale,
    mlib_s32 CONST data_info_flags);

/* *********************************************************** */
/* macros, typedefs */

/* dispatchers without windowing */
#define	mlib_c_fftDispatcher_1(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_c_fftDispatcherW_1(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_c_fftDispatcher_2(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_c_fftDispatcherW_2(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_c_fftDispatcher_3(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_c_fftDispatcherW_3(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_c_fftDispatcher_4(dst_main, dst_aux, src_main,             \
	src_aux, order, scalef, data_info_flags)                        \
	mlib_c_fftDispatcherW_4(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, scalef, data_info_flags)

/* *********************************************************** */

typedef void (*fft_func_order) (
    mlib_d64 *data_r,
    deal_t *data_i,
    mlib_s32 order);

typedef void (*fft_func_layer) (
    mlib_d64 *data_r,
    deal_t *data_i,
    mlib_s32 order,
    mlib_s32 layer);

typedef struct
{
	mlib_s32 isign;
	mlib_s32 ri_flag;
	fft_func_order func_order4;
	fft_func_order func_L2_First;
	fft_func_order func_L3_First;
	fft_func_layer func_L2;
	fft_func_layer func_L3;
} fft_param;

/* *********************************************************** */

extern fft_param mlib_fft_func_I[];
extern fft_param mlib_fft_func_F[];
extern fft_param mlib_fft_func_RI_I[];
extern fft_param mlib_fft_func_RI_F[];

/* *********************************************************** */

void mlib_c_fftKernel1(
    void *data_r,
    void *data_i,
    mlib_s32 order,
    mlib_s32 layer_min,
    mlib_s32 layer_max,
    fft_param *prm,
    mlib_s32 inp_flag);

void mlib_c_fftKernel(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign);

void mlib_c_fftKernelInplace(
    deal_t *CONST pal64data,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

void mlib_c_fftKernel_RI(
    deal_t *CONST pal64data_r,
    deal_t *CONST pal64data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

void mlib_c_fftKernelInplace_RI(
    deal_t *CONST pal64data_r,
    deal_t *CONST pal64data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

/* *********************************************************** */

void mlib_SignalFFT_first3_blk_nbr(
    mlib_d64 *dst_src,
    mlib_s32 order,
    mlib_s32 order_main);

void mlib_SignalIFFT_first3_blk_nbr(
    mlib_d64 *dst_src,
    mlib_s32 order,
    mlib_s32 order_main);

void mlib_SignalFFT_first3_blk(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main);

void mlib_SignalIFFT_first3_blk(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main);

void mlib_SignalFFT_first3_blk_r(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main);

void mlib_SignalIFFT_first3_blk_r(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main);

/* *********************************************************** */
/* things specific in VIS */

#ifdef MLIB_VIS

/* *********************************************************** */

#define	FFT_VIS_TBL_ORDER	10
#define	ONE_LAYER_DIV	0
#define	BOTH_LAYERS_DIV	1

/* *********************************************************** */
/* vis deal type */
typedef mlib_s16 deal_v_t;

/* *********************************************************** */
/* function prototypes */

mlib_status mlib_v_fftDispatcherW(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags,
    mlib_s32 FFTtype);

#define	mlib_v_fftDispatcherW_1(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_v_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 1)

#define	mlib_v_fftDispatcherW_2(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_v_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 2)

#define	mlib_v_fftDispatcherW_3(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_v_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 3)

mlib_status mlib_v_fftDispatcherW_4(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scalef,
    mlib_s32 CONST data_info_flags);

void mlib_v_fftKernelDivNone(
    mlib_s16 *CONST pal64data_r,
    mlib_s16 *CONST pal64data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

void mlib_v_fftKernelDivAll(
    mlib_s16 *CONST pal64data_r,
    mlib_s16 *CONST pal64data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

void mlib_v_fftKernelDivOdds(
    mlib_s16 *CONST pal64data_r,
    mlib_s16 *CONST pal64data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign);

/* *********************************************************** */
/* macros, typedefs */

/* vis dispatchers without windowing */
#define	mlib_v_fftDispatcher_1(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_v_fftDispatcherW_1(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_v_fftDispatcher_2(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_v_fftDispatcherW_2(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_v_fftDispatcher_3(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_v_fftDispatcherW_3(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_v_fftDispatcher_4(dst_main, dst_aux, src_main,             \
	src_aux, order, scalef, data_info_flags)                        \
	mlib_v_fftDispatcherW_4(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, scalef, data_info_flags)

#endif /* MLIB_VIS */
/* *********************************************************** */


/* things specific in SSE2 Begins */

/* *********************************************************** */

#define	FFT_VIS_TBL_ORDER	10
#define	ONE_LAYER_DIV	0
#define	BOTH_LAYERS_DIV	1

/* *********************************************************** */
/* vis deal type */
#ifndef	__sparc
typedef mlib_s16 deal_v_t;
#endif

/* *********************************************************** */
/* function prototypes */

mlib_status mlib_s_fftDispatcherW(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags,
    mlib_s32 FFTtype);

#define	mlib_s_fftDispatcherW_1(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_s_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 1)

#define	mlib_s_fftDispatcherW_2(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_s_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 2)

#define	mlib_s_fftDispatcherW_3(dst_main, dst_aux, src_main,          \
	src_aux, window, order, data_info_flags)                      \
	mlib_s_fftDispatcherW(dst_main, dst_aux, src_main, src_aux,   \
	    window, order, data_info_flags, 3)

mlib_s32 mlib_s_fftKernel_4(
    deal_v_t *CONST pal128data_r,
    deal_v_t *CONST pal128data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST scaleGet,
    mlib_s32 CONST isign);

/* *********************************************************** */
/* macros, typedefs */

/* vis dispatchers without windowing */
#define	mlib_s_fftDispatcher_1(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_s_fftDispatcherW_1(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_s_fftDispatcher_2(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_s_fftDispatcherW_2(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_s_fftDispatcher_3(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                \
	mlib_s_fftDispatcherW_3(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

/* End of things in SSE2 */


/* *********************************************************** */

/* *********************************************************** */
/* things common in C and VIS */

/* *********************************************************** */
/* function prototypes */

#define	DISPATCHER_PROTO(NAME)            \
mlib_status NAME(                         \
    data_t *CONST dst_main,               \
    data_t *CONST dst_aux,                \
    data_t *CONST src_main,               \
    data_t *CONST src_aux,                \
    mlib_s32 CONST order,                 \
    mlib_s32 CONST shiftscale,            \
    mlib_s32 CONST isign);

#define	DISPATCHER_PROTO_ALL(SUFF)                \
DISPATCHER_PROTO(mlib_fftDispatcher_##SUFF)       \
DISPATCHER_PROTO(__mlib_fftDispatcher_##SUFF)     \
DISPATCHER_PROTO(__mlib_fftDispatcher_8_##SUFF)   \
DISPATCHER_PROTO(__mlib_fftDispatcher_16_##SUFF)  \
DISPATCHER_PROTO(__mlib_fftDispatcher_24_##SUFF)  \
DISPATCHER_PROTO(__mlib_fftDispatcher_32_##SUFF)

DISPATCHER_PROTO_ALL(D64C_D64C)
DISPATCHER_PROTO_ALL(D64_D64)
DISPATCHER_PROTO_ALL(D64C_D64)
DISPATCHER_PROTO_ALL(D64_D64C)
DISPATCHER_PROTO_ALL(D64)
DISPATCHER_PROTO_ALL(D64C)

mlib_status
__mlib_fftDispatcherW_F32_Common(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags);
mlib_status
mlib_fftDispatcherW_F32_Common(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags);
mlib_status
__mlib_fftDispatcherW_F32C_F32(
	mlib_f32 *CONST dst_main,
	mlib_f32 *CONST dst_aux,
	mlib_f32 *CONST src_main,
	mlib_f32 *CONST src_aux,
	mlib_f32 *CONST window,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale,
	mlib_s32 CONST data_info_flags);
mlib_status
mlib_fftDispatcherW_F32C_F32(
	mlib_f32 *CONST dst_main,
	mlib_f32 *CONST dst_aux,
	mlib_f32 *CONST src_main,
	mlib_f32 *CONST src_aux,
	mlib_f32 *CONST window,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale,
	mlib_s32 CONST data_info_flags);

mlib_s32 mlib_fftK_DealChunks(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign);

/* *********************************************************** */
/* macros, typedefs */

/*
 *    bit flags providing src/dst and direct/inverse data
 *    processing in dispatcher. They will be or'ed in calls to it
 */
typedef enum
{
	eSrcCmplx = 1,
	eSrcReIm = 2,
	eSrcRe = 4,
	eSrcMask = 7,
	eDstCmplx = 8,
	eDstReIm = 16,
	eDstRe = 32,
	eDstMask = 56,
	eInversFFT = 64
} eDataInfoFlags;

/* controls direct / inverse fft */
#define	DIRECT_FFT	(-1)
#define	INVERS_FFT	(-DIRECT_FFT)

/* pointer alignments */
#define	ALIGNMENT_64(ptr)	(((mlib_s32)(ptr)) & 7)
#define	PTR_IS_ALIGNED64(ptr)	(!ALIGNMENT_64(ptr))

/*
 *    controls internal self-diagnostics and assertions
 *    disabling them when set to TRUE
 */
#define	RELEASE_ON	TRUE

/*
 *    takes sines from table. Presupposes sines are stored
 *    for Pi, Pi/2, Pi/4, ... ,Pi/0x10000000 sequentially
 */
#define	SINUS(table_index)	(mlib_fftK_SinesTable[ table_index ])

#if DEAL_F32

typedef deal_t precise_t;

#else

typedef mlib_d64 precise_t;

#endif /* DEAL_F32 */

/* *********************************************************** */
/* sines table */
extern const precise_t mlib_fftK_SinesTable[];

/* *********************************************************** */
/* tricks used in bit reversion */

/* bit reversion indexes */
extern const mlib_u8 mlib_fftK_Rev8[256];

/* getting 8 and 16-bit indexes reversed images */
#define	REV_INDX_U(indx)	((mlib_u32)(mlib_fftK_Rev8[indx]))

#define	REV_INDX_8(indx, shift)	(REV_INDX_U(indx) >> shift)

#define	REV_INDX_16(indx, shift)                                          \
	((REV_INDX_U((indx) >> 8) | (REV_INDX_U(((indx) & 0xFF)) << 8))   \
	    >> (shift))

#define	REV_INDX_24(indx, shift)                                    \
	((mlib_s32)(((REV_INDX_U(((indx) & 0x0000ff)) << 16) |      \
	    (REV_INDX_U(((indx) & 0x00ff00) >> 8) << 8) |           \
	    (REV_INDX_U(((indx) & 0xff0000) >> 16))) >> (shift)))

#define	REV_INDX_32(indx, shift)                                      \
	((mlib_s32)(((REV_INDX_U(((indx) & 0x000000ff)) << 24) |      \
	    (REV_INDX_U(((indx) & 0x0000ff00) >> 8) << 16) |          \
	    (REV_INDX_U(((indx) & 0x00ff0000) >> 16) << 8) |          \
	    (REV_INDX_U(((indx) & 0xff000000) >> 24))) >> (shift)))

/* determine ( 1 << DTCMPLX_SZ_ORD ) == 2 * sizeof ( deal_t ) */

#if DEAL_F32

#define	DTCMPLX_SZ_ORD	3

#else

#define	DTCMPLX_SZ_ORD	4

#endif /* DEAL_F32 */

/* caches tuned to deal_t */
#define	CACHE1_ORD	(CACHE1_ORD_BYTES - DTCMPLX_SZ_ORD)
#define	CACHE2_ORD	(CACHE2_ORD_BYTES - DTCMPLX_SZ_ORD)

#if CACHE1_ORD < 1 || CACHE1_ORD > CACHE2_ORD
#error wrong cache order(s)
#endif /* CACHE1_ORD < 1 || CACHE1_ORD > CACHE2_ORD */

/* used to branch direct/inverse FFT processings */
#define	MORE_ZERO_ISIGN	(DIRECT_FFT > 0 ? DIRECT_FFT : INVERS_FFT)

/* tag to mark 'speed-important' variables */
#define	REGISTER

/* values ( kill ALL 'magic digits'! ) */
#define	ZERO	(precise_t)0.0
#define	ONE	(precise_t)1.0
#define	MINUS_ONE	(precise_t)-1.0
#define	TWO	(precise_t)2.0
#define	SIN_PI_BY4                                                      \
	(precise_t)0.70710678118654752440084436210484903928483593768846
#define	SIN_PI_BY8	(precise_t)0.3826834323650897717284599840304
#define	SIN_PI_BY16	(precise_t)0.1950903220161282678482848684770
#define	COS_PI_BY8	(precise_t)(1 - TWO * SIN_PI_BY16 * SIN_PI_BY16)

/* to switch between -( value ) and -1.*( value ) */
#define	MINUS(value)	((MINUS_ONE) * (value))
/* to switch between MINUS( value ) and ( value ) */
#define	AS_IS(value)	(value)

/* declare & calc various step_values */
#define	DCL_CALC_STEPS1234(var_tag, ord_lay)                     \
	CONST mlib_s32 var_tag = 1 << ord_lay, two_##var_tag =   \
	    var_tag * 2, three_##var_tag =                       \
	    var_tag * 3, four_##var_tag = var_tag * 4

/* declare & calc cos-sin of incr angles */
#define	DCL_CALC_INCR(var_tag, ord_lay, tag)                     \
	CONST precise_t sin_half_delta_##tag =                   \
	    isign * SINUS(ord_lay + 1), var_tag##_r =            \
	    ONE -                                                \
	    TWO * sin_half_delta_##tag * sin_half_delta_##tag,   \
	    var_tag##_i = isign * SINUS(ord_lay)

/* declare & init cos-sin of zero angle ( rotators ) */
#define	DCL_INIT_ROT(var_tag, incr_tag)                         \
	precise_t var_tag##_r = incr_tag##_r, var_tag##_i =     \
	    incr_tag##_i

/* declare/define multiples of rots components */
#define	DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh)              \
	precise_t CONST r_rh##_r =                              \
	    rot##_r * rot_half##_r -                            \
	    rot##_i * rot_half##_i, r_rh##_i =                  \
	    rot##_r * rot_half##_i +                            \
	    rot##_i * rot_half##_r

/*
 *    find sin and cos of angles incremented by delta.
 *    See also 'C Num Recipes' ( 5.5.6 )
 */
#define	ROTATE_CMPLX_TMP(dest, incr, tmp)                       \
	{                                                       \
	    dest##_r = ((tmp =                                  \
		dest##_r) * incr##_r -                          \
		dest##_i * incr##_i);                           \
	    dest##_i =                                          \
		tmp * incr##_i + dest##_i * incr##_r;           \
	}

/*
 *    make a butterfly on two-reals A and B as CONST src's
 *    and C and D as dests, i.e.
 *    { C = A + B; D = A - B; }  ( optimize? )
 *    Maybe all( some of these ) can be rewritten when DEAL_F32?
 */
#define	BUTTERFLY_CMPLX_CMPLX(dest1, dest2, src1, src2)         \
	{                                                       \
	    dest1->r = src1##_r + src2##_r;                     \
	    dest1->i = src1##_i + src2##_i;                     \
	    dest2->r = src1##_r - src2##_r;                     \
	    dest2->i = src1##_i - src2##_i;                     \
	}

/*
 *    declares and inits FOUR ptrs to dtCmplx
 *    from p_beg shifted by step
 */
#define	DCL_4_CMPLX_PTRS(tag, ptr_beg, tagstp)                  \
	dtCmplx *CONST tag##_0 = ptr_beg, *CONST tag##_1 =      \
	    tag##_0 + tagstp, *CONST tag##_2 =                  \
	    tag##_0 + two_##tagstp, *CONST tag##_3 =            \
	    tag##_0 + three_##tagstp

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT(dst, src, iSIGNed)                 \
	CONST deal_t dst##_0_pls_1_r =                          \
	    src##_0->r + src##_1->r, dst##_0_pls_1_i =          \
	    src##_0->i + src##_1->i, dst##_0_mns_1_r =          \
	    src##_0->r - src##_1->r, dst##_0_mns_1_i =          \
	    src##_0->i - src##_1->i, dst##_2_pls_3_r =          \
	    src##_2->r + src##_3->r, dst##_2_pls_3_i =          \
	    src##_2->i + src##_3->i, dst##_2_mns_3_r =          \
	iSIGNed(src##_3->i - src##_2->i), dst##_2_mns_3_i =     \
	iSIGNed(src##_2->r - src##_3->r)

/* perform & store simple butterflies */
#define	STORE_SIMPLE_BUTTERFLIES(dst, src)                      \
	BUTTERFLY_CMPLX_CMPLX(dst##_0, dst##_2,                 \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX(dst##_1, dst##_3,                 \
	    src##_0_mns_1, src##_2_mns_3);

/* declare/define components for normal butterflies */
#define	DCL_CALC_NORMAL_BUTT(src, tag, rot, rot_half, r_rh,     \
	iSIGNed)                                                \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag = src##_0->r + (x##tag =        \
	    (src##_1->r * rot##_r) -                            \
	    (src##_1->i * rot##_i)), aaa1_##tag =               \
	    src##_0->r - x##tag;                                \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag = src##_0->i + (y##tag =        \
	    (src##_1->i * rot##_r) +                            \
	    (src##_1->r * rot##_i)), aaa3_##tag =               \
	    src##_0->i - y##tag;                                \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag = (a##tag =                     \
	    (src##_2->r * rot_half##_r) -                       \
	    (src##_2->i * rot_half##_i)) + (b##tag =            \
	    (src##_3->r * (r_rh##_r)) -                         \
	    (src##_3->i * (r_rh##_i))), bbb1_##tag =            \
	iSIGNed(b##tag - a##tag);                               \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag = (c##tag =                     \
	    (src##_2->i * rot_half##_r) +                       \
	    (src##_2->r * rot_half##_i)) + (d##tag =            \
	    (src##_3->i * (r_rh##_r)) +                         \
	    (src##_3->r * (r_rh##_i))), bbb3_##tag =            \
	iSIGNed(c##tag - d##tag)

/* declare/define components for mirror butterflies */
/*
 *    NOTE: 'mirrored' section taken from Mayer gives these benefits:
 *      1.Loop becomes shorter, its body longer
 *      2.Decrease of trigonometric mults/adds in rots.
 *      3.Last layers loops unrolling
 */
#define	DCL_CALC_MIRROR_BUTT(src, tag, rot, rot_half, r_rh,     \
	iSIGNed)                                                \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag = src##_0->r - (x##tag =        \
	    (src##_1->r * rot##_r) +                            \
	    (src##_1->i * rot##_i)), aaa1_##tag =               \
	    src##_0->r + x##tag;                                \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag = src##_0->i - (y##tag =        \
	    (src##_1->i * rot##_r) -                            \
	    (src##_1->r * rot##_i)), aaa3_##tag =               \
	    src##_0->i + y##tag;                                \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag = iSIGNed((a##tag =             \
	    (src##_2->r * rot_half##_i) -                       \
	    (src##_2->i * rot_half##_r)) + (b##tag =            \
	    (src##_3->i * (r_rh##_r)) -                         \
	    (src##_3->r * (r_rh##_i)))), bbb1_##tag =           \
	    (b##tag - a##tag);                                  \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag = iSIGNed((c##tag =             \
	    (src##_2->i * rot_half##_i) +                       \
	    (src##_2->r * rot_half##_r)) + (d##tag =            \
	    (src##_3->r * (-r_rh##_r)) -                        \
	    (src##_3->i * (r_rh##_i)))), bbb3_##tag =           \
	    (c##tag - d##tag)

/* declare/define components for half/way butterflies */
#define	DCL_CALC_HALFWAY_BUTT(src, tag, iSIGNed)                \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag = src##_0->r - (x##tag =        \
	    iSIGNed(src##_1->i)), aaa1_##tag =                  \
	    src##_0->r + x##tag;                                \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag = src##_0->i + (y##tag =        \
	    iSIGNed(src##_1->r)), aaa3_##tag =                  \
	    src##_0->i - y##tag;                                \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag =                               \
	    (iSIGNed(SIN_PI_BY4)) * ((a##tag =                  \
	    iSIGNed(src##_2->r) - src##_2->i) - (b##tag =       \
	    src##_3->i + iSIGNed(src##_3->r))), bbb1_##tag =    \
	    -SIN_PI_BY4 * (b##tag + a##tag);                    \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag =                               \
	    (iSIGNed(SIN_PI_BY4)) * ((c##tag =                  \
	    iSIGNed(src##_2->i) + src##_2->r) + (d##tag =       \
	    src##_3->r - iSIGNed(src##_3->i))), bbb3_##tag =    \
	    SIN_PI_BY4 * (c##tag - d##tag)

/* perform & store usual butterflies */
#define	STORE_USUAL_BUTTERFLIES(dst, tag)                       \
	dst##_0->r = aaa0_##tag + bbb0_##tag;                   \
	dst##_0->i = aaa2_##tag + bbb2_##tag;                   \
	dst##_1->r = aaa1_##tag - bbb3_##tag;                   \
	dst##_1->i = aaa3_##tag - bbb1_##tag;                   \
	dst##_2->r = aaa0_##tag - bbb0_##tag;                   \
	dst##_2->i = aaa2_##tag - bbb2_##tag;                   \
	dst##_3->r = aaa1_##tag + bbb3_##tag;                   \
	dst##_3->i = aaa3_##tag + bbb1_##tag;

/* trick to DEAL_* with step == 1 */
enum HetTricks
{
	two_1 = 2, three_1 = 3, four_1 = 4, two_four_1 = 8, three_four_1 = 12,
	two_4 = 8, three_4 = 12, four_4 = 16
};

/* sequence to perform simple butt on 4 complex elems */
#define	DEAL_4_SIMPLE(ptr_beg, tagstep, iSIGNed)                \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                 \
	DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES(d0, c0);

/* sequence to perform usual butt on 4+4 complex elems mirrored */
#define	DEAL_8_USUAL_MIRR(ptr_beg, ptr_mirr, tagstep, rot,           \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS(d1, ptr_mirr, tagstep);                     \
	DCL_CALC_NORMAL_BUTT(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES(d1, 1);

/* sequence to perform half-way butt on 4 complex elems */
#define	DEAL_4_HALFWAY(ptr_beg, tagstep, iSIGNed)               \
	DCL_4_CMPLX_PTRS(d2, ptr_beg, tagstep);                 \
	DCL_CALC_HALFWAY_BUTT(d2, 2, iSIGNed);                  \
	STORE_USUAL_BUTTERFLIES(d2, 2);

/* perform all ( i.e last 2 layers ) for order 4 */
#define	DEAL_ALL_ORD4(isign, ptr_beg, iSIGNed)                           \
	{                                                                \
	    DEAL_4_SIMPLE(ptr_beg, 4, iSIGNed);                          \
	}                                                                \
	                                                                 \
	{                                                                \
	    DEAL_4_HALFWAY(ptr_beg + 2, 4, iSIGNed);                     \
	}                                                                \
	                                                                 \
	{                                                                \
	    CONST precise_t rot_r = SIN_PI_BY4, rot_i =                  \
		iSIGNed(SIN_PI_BY4), rot_half_r =                        \
		COS_PI_BY8, rot_half_i = iSIGNed(SIN_PI_BY8), r_rh_r =   \
		SIN_PI_BY8, r_rh_i = iSIGNed(COS_PI_BY8);                \
	    DEAL_8_USUAL_MIRR(ptr_beg + 1, ptr_beg + 3, 4, rot,          \
		rot_half, r_rh, iSIGNed);                                \
	}

/* loop performing mlib_fftK_2Layers on chunks */
#define	LOOP_2LAYS(dat_first, isign, ord_l, ord_ch, l0, mult)     \
	for (ord_l = l0; mult * (1 << ord_l) <= (1 << ord_ch);    \
	    ord_l += 2) {                                         \
	    mlib_fftK_2Layers(dat_first, ord_ch, isign, ord_l);   \
	}

/*
 *    make a butterfly on two-reals A and B as CONST src's
 *    and C and D as dests, i.e.
 *    { C = A + B; D = A - B; }  ( optimize? )
 */
#define	BUTTERFLY_CMPLX_CMPLX_RI(dst1, dst2, src1, src2)        \
	{                                                       \
	    (*(dst1##__r)) = src1##_r + src2##_r;               \
	    (*(dst1##__i)) = src1##_i + src2##_i;               \
	    (*(dst2##__r)) = src1##_r - src2##_r;               \
	    (*(dst2##__i)) = src1##_i - src2##_i;               \
	}

/*
 *    declares and inits FOUR ptrs to real and imag
 *    from p_beg shifted by step
 */
#define	DCL_4_CMPLX_PTRS_RI(tag, ptr_beg, tagstp)               \
	deal_t *CONST tag##_0__r =                              \
	    ptr_beg##_r, *CONST tag##_1__r =                    \
	    tag##_0__r + tagstp, *CONST tag##_2__r =            \
	    tag##_0__r + two_##tagstp, *CONST tag##_3__r =      \
	    tag##_0__r + three_##tagstp, *CONST tag##_0__i =    \
	    ptr_beg##_i, *CONST tag##_1__i =                    \
	    tag##_0__i + tagstp, *CONST tag##_2__i =            \
	    tag##_0__i + two_##tagstp, *CONST tag##_3__i =      \
	    tag##_0__i + three_##tagstp

#define	DCL_4_NEXT_PTRS_RI(tag, ptr_beg, shift, tagstp)         \
	deal_t *CONST tag##next4_r =                            \
	    ptr_beg##_r + shift, *CONST tag##next4_i =          \
	    ptr_beg##_i + shift;                                \
	DCL_4_CMPLX_PTRS_RI(tag, tag##next4, tagstp)

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT_RI(dst, src, iSIGNed)              \
	CONST deal_t dst##_0_pls_1_r =                          \
	    (*(src##_0##__r)) + (*(src##_1##__r)),              \
	    dst##_0_pls_1_i =                                   \
	    (*(src##_0##__i)) + (*(src##_1##__i)),              \
	    dst##_0_mns_1_r =                                   \
	    (*(src##_0##__r)) - (*(src##_1##__r)),              \
	    dst##_0_mns_1_i =                                   \
	    (*(src##_0##__i)) - (*(src##_1##__i)),              \
	    dst##_2_pls_3_r =                                   \
	    (*(src##_2##__r)) + (*(src##_3##__r)),              \
	    dst##_2_pls_3_i =                                   \
	    (*(src##_2##__i)) + (*(src##_3##__i)),              \
	    dst##_2_mns_3_r =                                   \
	iSIGNed((*(src##_3##__i)) - (*(src##_2##__i))),         \
	    dst##_2_mns_3_i =                                   \
	iSIGNed((*(src##_2##__r)) - (*(src##_3##__r)))

/* perform & store simple butterflies */
#define	STORE_SIMPLE_BUTTERFLIES_RI(dst, src)                   \
	BUTTERFLY_CMPLX_CMPLX_RI(dst##_0, dst##_2,              \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_RI(dst##_1, dst##_3,              \
	    src##_0_mns_1, src##_2_mns_3);

/* declare/define components for normal butterflies */
#define	DCL_CALC_NORMAL_BUTT_RI(src, tag, rot, rot_half,        \
	r_rh, iSIGNed)                                          \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag =                               \
	    (*(src##_0##__r)) + (x##tag =                       \
	    ((*(src##_1##__r)) * rot##_r) -                     \
	    ((*(src##_1##__i)) * rot##_i)), aaa1_##tag =        \
	    (*(src##_0##__r)) - x##tag;                         \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag =                               \
	    (*(src##_0##__i)) + (y##tag =                       \
	    ((*(src##_1##__i)) * rot##_r) +                     \
	    ((*(src##_1##__r)) * rot##_i)), aaa3_##tag =        \
	    (*(src##_0##__i)) - y##tag;                         \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag = (a##tag =                     \
	    ((*(src##_2##__r)) * rot_half##_r) -                \
	    ((*(src##_2##__i)) * rot_half##_i)) +               \
	    (b##tag =                                           \
	    ((*(src##_3##__r)) * (r_rh##_r)) -                  \
	    ((*(src##_3##__i)) * (r_rh##_i))),                  \
	    bbb1_##tag = iSIGNed(b##tag - a##tag);              \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag = (c##tag =                     \
	    ((*(src##_2##__i)) * rot_half##_r) +                \
	    ((*(src##_2##__r)) * rot_half##_i)) +               \
	    (d##tag =                                           \
	    ((*(src##_3##__i)) * (r_rh##_r)) +                  \
	    ((*(src##_3##__r)) * (r_rh##_i))),                  \
	    bbb3_##tag = iSIGNed(c##tag - d##tag)

/* declare/define components for mirror butterflies */
/*
 *    NOTE: 'mirrored' section taken from Mayer gives these benefits:
 *      1.Loop becomes shorter, its body longer
 *      2.Decrease of trigonometric mults/adds in rots.
 *      3.Last layers loops unrolling
 */
#define	DCL_CALC_MIRROR_BUTT_RI(src, tag, rot, rot_half,        \
	r_rh, iSIGNed)                                          \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag =                               \
	    (*(src##_0##__r)) - (x##tag =                       \
	    ((*(src##_1##__r)) * rot##_r) +                     \
	    ((*(src##_1##__i)) * rot##_i)), aaa1_##tag =        \
	    (*(src##_0##__r)) + x##tag;                         \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag =                               \
	    (*(src##_0##__i)) - (y##tag =                       \
	    ((*(src##_1##__i)) * rot##_r) -                     \
	    ((*(src##_1##__r)) * rot##_i)), aaa3_##tag =        \
	    (*(src##_0##__i)) + y##tag;                         \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag = iSIGNed((a##tag =             \
	    ((*(src##_2##__r)) * rot_half##_i) -                \
	    ((*(src##_2##__i)) * rot_half##_r)) +               \
	    (b##tag =                                           \
	    ((*(src##_3##__i)) * (r_rh##_r)) -                  \
	    ((*(src##_3##__r)) * (r_rh##_i)))),                 \
	    bbb1_##tag = (b##tag - a##tag);                     \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag = iSIGNed((c##tag =             \
	    ((*(src##_2##__i)) * rot_half##_i) +                \
	    ((*(src##_2##__r)) * rot_half##_r)) +               \
	    (d##tag =                                           \
	    ((*(src##_3##__r)) * (-r_rh##_r)) -                 \
	    ((*(src##_3##__i)) * (r_rh##_i)))),                 \
	    bbb3_##tag = (c##tag - d##tag)

/* declare/define components for half/way butterflies */
#define	DCL_CALC_HALFWAY_BUTT_RI(src, tag, iSIGNed)             \
	REGISTER deal_t x##tag;                                 \
	CONST deal_t aaa0_##tag =                               \
	    (*(src##_0##__r)) - (x##tag =                       \
	    iSIGNed((*(src##_1##__i)))), aaa1_##tag =           \
	    (*(src##_0##__r)) + x##tag;                         \
	REGISTER deal_t y##tag;                                 \
	CONST deal_t aaa2_##tag =                               \
	    (*(src##_0##__i)) + (y##tag =                       \
	    iSIGNed((*(src##_1##__r)))), aaa3_##tag =           \
	    (*(src##_0##__i)) - y##tag;                         \
	REGISTER deal_t a##tag, b##tag;                         \
	CONST deal_t bbb0_##tag =                               \
	    (iSIGNed(SIN_PI_BY4)) * ((a##tag =                  \
	    iSIGNed((*(src##_2##__r))) -                        \
	    (*(src##_2##__i))) - (b##tag =                      \
	    (*(src##_3##__i)) +                                 \
	    iSIGNed((*(src##_3##__r))))), bbb1_##tag =          \
	    -SIN_PI_BY4 * (b##tag + a##tag);                    \
	REGISTER deal_t c##tag, d##tag;                         \
	CONST deal_t bbb2_##tag =                               \
	    (iSIGNed(SIN_PI_BY4)) * ((c##tag =                  \
	    iSIGNed((*(src##_2##__i))) +                        \
	    (*(src##_2##__r))) + (d##tag =                      \
	    (*(src##_3##__r)) -                                 \
	    iSIGNed((*(src##_3##__i))))), bbb3_##tag =          \
	    SIN_PI_BY4 * (c##tag - d##tag)

/* perform & store usual butterflies */
#define	STORE_USUAL_BUTTERFLIES_RI(dst, tag)                    \
	(*(dst##_0##__r)) = aaa0_##tag + bbb0_##tag;            \
	(*(dst##_0##__i)) = aaa2_##tag + bbb2_##tag;            \
	(*(dst##_1##__r)) = aaa1_##tag - bbb3_##tag;            \
	(*(dst##_1##__i)) = aaa3_##tag - bbb1_##tag;            \
	(*(dst##_2##__r)) = aaa0_##tag - bbb0_##tag;            \
	(*(dst##_2##__i)) = aaa2_##tag - bbb2_##tag;            \
	(*(dst##_3##__r)) = aaa1_##tag + bbb3_##tag;            \
	(*(dst##_3##__i)) = aaa3_##tag + bbb1_##tag;

/* sequence to perform simple butt on 4 complex elems */
#define	DEAL_4_SIMPLE_RI(ptr_beg, tagstep, iSIGNed)             \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);              \
	DCL_CALC_SIMPLE_BUTT_RI(c0, d0, iSIGNed);               \
	STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);

/* sequence to perform usual butt on 4+4 complex elems mirrored */
#define	DEAL_8_USUAL_MIRR_RI(ptr_beg, ptr_mirr, tagstep, rot,           \
	rot_half, r_rh, iSIGNed)                                        \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS_RI(d1, ptr_mirr, tagstep);                     \
	DCL_CALC_NORMAL_BUTT_RI(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_MIRROR_BUTT_RI(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_RI(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_RI(d1, 1);

/* sequence to perform half-way butt on 4 complex elems */
#define	DEAL_4_HALFWAY_RI(ptr_beg, tagstep, iSIGNed)            \
	DCL_4_CMPLX_PTRS_RI(d2, ptr_beg, tagstep);              \
	DCL_CALC_HALFWAY_BUTT_RI(d2, 2, iSIGNed);               \
	STORE_USUAL_BUTTERFLIES_RI(d2, 2);

/* perform all ( i.e last 2 layers ) for order 4 */
#define	DEAL_ALL_ORD4_RI(isign, ptr_beg, iSIGNed)                          \
	{                                                                  \
	    DEAL_4_SIMPLE_RI(ptr_beg, 4, iSIGNed);                         \
	}                                                                  \
	                                                                   \
	{                                                                  \
	    deal_t *ptr_beg_2_r = ptr_beg##_r + 2, *ptr_beg_2_i =          \
		ptr_beg##_i + 2;                                           \
	    DEAL_4_HALFWAY_RI(ptr_beg_2, 4, iSIGNed);                      \
	}                                                                  \
	                                                                   \
	{                                                                  \
	    CONST precise_t rot_r = SIN_PI_BY4, rot_i =                    \
		iSIGNed(SIN_PI_BY4), rot_half_r =                          \
		COS_PI_BY8, rot_half_i = iSIGNed(SIN_PI_BY8), r_rh_r =     \
		SIN_PI_BY8, r_rh_i = iSIGNed(COS_PI_BY8);                  \
	    deal_t *ptr_beg_1_r = ptr_beg##_r + 1, *ptr_beg_1_i =          \
		ptr_beg##_i + 1, *ptr_beg_3_r =                            \
		ptr_beg##_r + 3, *ptr_beg_3_i = ptr_beg##_i + 3;           \
	    DEAL_8_USUAL_MIRR_RI(ptr_beg_1, ptr_beg_3, 4, rot, rot_half,   \
		r_rh, iSIGNed);                                            \
	}

/* loop performing mlib_fftK_2Layers on chunks */
/*
 * #define LOOP_2LAYS_RI( dat_first, isign, ord_l, ord_ch, l0, mult )   \
 *  for( ord_l = l0; mult * ( 1 << ord_l ) <=                           \
 *       ( 1 << ord_ch ); ord_l += 2 ) {                                \
 *    mlib_fftK_2Layers_RI                                              \
 *       ( dat_first##_r, dat_first##_i, ord_ch, isign, ord_l );        \
 *  }
 *
 */

/* *********************************************************** */
/* things for *F32* functions */

/* parametrize it! */
typedef mlib_f32 deal_f32_t;

#define	DT_F32CMPLX_SZ_ORD	3

mlib_status mlib_fftDispatcherW_F32_1(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32_2(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32_3(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32_1(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32_2(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32_3(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

mlib_status mlib_fftDispatcherW_F32C_F32(
    mlib_f32 *CONST dst_main,
    mlib_f32 *CONST dst_aux,
    mlib_f32 *CONST src_main,
    mlib_f32 *CONST src_aux,
    mlib_f32 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags);

/* *********************************************************** */

#define	PROTO_3LayersBlk(DIR)  \
void mlib_fft_3LayersBlk_##DIR(\
	mlib_d64 *dat,         \
	mlib_d64 *data_i,      \
	mlib_s32 n_ord_lay,    \
	mlib_s32 i0,           \
	mlib_s32 i1,           \
	mlib_d64 r1__r,        \
	mlib_d64 r1__i);

PROTO_3LayersBlk(F)
PROTO_3LayersBlk(I)
PROTO_3LayersBlk(RI_F)
PROTO_3LayersBlk(RI_I)

#define	PROTO_BIT_REV(BITS)                 \
void                                        \
__mlib_fftDispatcher_##BITS##_BitRev(       \
	data_t *pal64,                      \
	data_t *src_main,                   \
	mlib_s32 CONST order,               \
	mlib_s32 CONST scl,                 \
	mlib_s32 CONST shiftscale,          \
	mlib_s32 CONST isign,               \
	mlib_s32 CONST _iii0,               \
	mlib_s32 CONST _iii1);              \
void                                        \
__mlib_fftDispatcher_##BITS##_BitRev_RI(    \
	data_t *pal64_r,                    \
	data_t *pal64_i,                    \
	data_t *src_main,                   \
	data_t *src_aux,                    \
	mlib_s32 CONST order,               \
	mlib_s32 CONST shiftscale,          \
	mlib_s32 CONST isign,               \
	mlib_s32 CONST _iii0,               \
	mlib_s32 CONST _iii1);              \
void                                        \
__mlib_fftDispatcher_##BITS##_BitRev_CRI(   \
	data_t *pal64_r,                    \
	data_t *pal64_i,                    \
	data_t *src_main,                   \
	data_t *src_aux,                    \
	mlib_s32 CONST order,               \
	mlib_s32 CONST shiftscale,          \
	mlib_s32 CONST isign,               \
	mlib_s32 CONST _iii0,               \
	mlib_s32 CONST _iii1);              \
void                                        \
__mlib_fftDispatcher_##BITS##_BitRev_Inp(   \
	data_t *dst_main,                   \
	mlib_s32 CONST order,               \
	mlib_s32 CONST shiftscale);         \
                                            \
void                                        \
__mlib_fftDispatcher_##BITS##_BitRev_RI_Inp(\
	data_t *pal64_r,                    \
	data_t *pal64_i,                    \
	mlib_s32 CONST order,               \
	mlib_s32 CONST shiftscale);

PROTO_BIT_REV(8)
PROTO_BIT_REV(16)
PROTO_BIT_REV(24)
PROTO_BIT_REV(32)

void
mlib_fft_D64C_D64_LL(
	data_t *pal64,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i);
void
mlib_fft_F32C_F32_LL(
	mlib_f32 *dst_main,
	mlib_d64 *pal64,
	mlib_d64 mult,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i);

void
mlib_fft_S16C_S16_LL(
	mlib_s16 *dst_main,
	mlib_d64 *pal64,
	mlib_d64 mult,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i);

mlib_s32
mlib_fft_S16C_S16_4_LL(
	mlib_d64 *pal64,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i);

void
mlib_fft_S16C_S16_4_WR(
	mlib_s16 *dst_main,
	mlib_d64 *pal64,
	mlib_d64 mult,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1);

mlib_s32
mlib_fftK_2LayersVeryLast_0(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign);

mlib_s32
mlib_fftK_2LayersVeryLast_i(
    dtCmplx * CONST dat,
    mlib_s32 CONST n_ord_arr,
    mlib_s32 CONST isign,
    mlib_s32 iii0,
    mlib_s32 iii1,
    mlib_d64 rot_r,
    mlib_d64 rot_i,
    mlib_d64 rot_half_r,
    mlib_d64 rot_half_i);

void
mlib_fftDW_PutData_S16(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST data_info_flags,
    mlib_s32 CONST bool_is_windowed,
    mlib_d64 scalemult,
    mlib_s32 i0,
    mlib_s32 i1);

mlib_s32
mlib_SignalFFT_CACHE1_ORD();

#define	PROTO_GetData(BITS)                                                \
	void                                                               \
	mlib_fftDW_GetData1_##BITS(deal_t *CONST dst,                      \
	    mlib_f32 *CONST src_main, mlib_f32 *CONST src_aux,             \
	    mlib_f32 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST scale, mlib_s32 CONST data_info_flags,          \
	    mlib_s32 _iii0, mlib_s32 _iii1);                               \
	void                                                               \
	mlib_c_fftDW_GetData1_##BITS##_4(deal_t *CONST dst,                \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST data_info_flags, mlib_s32 iii0, mlib_s32 iii1); \
	void                                                               \
	mlib_c_fftDW_GetData1_##BITS(deal_t *CONST dst,                    \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST scale, mlib_s32 CONST data_info_flags,          \
		mlib_s32 iii0, mlib_s32 iii1);

PROTO_GetData(8)
PROTO_GetData(16)
PROTO_GetData(24)
PROTO_GetData(32)

/* *********************************************************** */
/* macros, typedefs */

/* dispatchers without windowing */
#define	mlib_fftDispatcher_F32_1(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                  \
	mlib_fftDispatcherW_F32_1(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_fftDispatcher_F32_2(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                  \
	mlib_fftDispatcherW_F32_2(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

#define	mlib_fftDispatcher_F32_3(dst_main, dst_aux, src_main,             \
	src_aux, order, data_info_flags)                                  \
	mlib_fftDispatcherW_F32_3(dst_main, dst_aux, src_main, src_aux,   \
	    NULL, order, data_info_flags)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_SIGNALFFTUTIL_H */
