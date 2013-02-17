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

#ifndef _MLIB_SIGNALCODEC_H
#define	_MLIB_SIGNALCODEC_H

#pragma ident	"@(#)mlib_SignalCodec.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <mlib_types.h>

/*
 * The following is the definition of the state structure
 * used by the G.721/G.723 encoder and decoder to preserve their internal
 * state between successive calls.  The meanings of the majority
 * of the state structure fields are explained in detail in the
 * CCITT Recommendation G.721.  The field names are essentially identical
 * to variable names in the bit level description of the coding algorithm
 * included in this Recommendation.
 */
    typedef struct
{
/* Locked or steady state step size multiplier. */
	mlib_s32 yl;
/* Unlocked or non-steady state step size multiplier. */
	mlib_s32 yu;
/* mlib_s16 term energy estimate. */
	mlib_s32 dms;
/* mlib_s32 term energy estimate. */
	mlib_s32 dml;
/* Linear weighting coefficient of 'yl' and 'yu'. */
	mlib_s32 ap;

/* Coefficients of pole portion of prediction filter. */
	mlib_s32 a[2];
/* Coefficients of zero portion of prediction filter. */
	mlib_s32 b[6];
/*
 * Signs of previous two samples of a partially
 * reconstructed signal.
 */
	mlib_s32 pk[2];
/*
 * Previous 6 samples of the quantized difference
 * signal represented in an internal floating point
 * format.
 */
	mlib_s32 dq[6];
/*
 * Previous 2 samples of the quantized difference
 * signal represented in an internal floating point
 * format.
 */
	mlib_s32 sr[2];
/* delayed tone detect, new in 1988 version */
	mlib_s8 td;
} mlib_G72x_state;

extern const mlib_s16 mlib_ADPCM2Bits_rsf[];
extern const mlib_s16 mlib_ADPCM3Bits_rsf[];
extern const mlib_s16 mlib_ADPCM4Bits_rsf[];
extern const mlib_s16 mlib_ADPCM5Bits_rsf[];

extern const mlib_s16 mlib_ADPCM2Bits_sfm[];
extern const mlib_s16 mlib_ADPCM3Bits_sfm[];
extern const mlib_s16 mlib_ADPCM4Bits_sfm[];
extern const mlib_s16 mlib_ADPCM5Bits_sfm[];

extern const mlib_s16 mlib_ADPCM2Bits_st[];
extern const mlib_s16 mlib_ADPCM3Bits_st[];
extern const mlib_s16 mlib_ADPCM4Bits_st[];
extern const mlib_s16 mlib_ADPCM5Bits_st[];

extern const mlib_s16 mlib_ADPCM3Bits_quant[];
extern const mlib_s16 mlib_ADPCM4Bits_quant[];
extern const mlib_s16 mlib_ADPCM5Bits_quant[];
extern const mlib_u8 mlib_ADPCM3Bits_qs3[];

extern const mlib_u8 mlib_u2a[];
extern const mlib_u8 mlib_a2u[];
extern const mlib_u16 mlib_u2l[];
extern const mlib_u16 mlib_A2l[];
extern const mlib_u8 mlib_l2u[];
extern const mlib_u8 mlib_l2A[];

extern const mlib_s8 mlib_sgn[];
extern const mlib_u8 mlib_ADPCM_mul[];

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALCODEC_H */
