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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_I_SIGNALWINTAB_H
#define	_MLIB_I_SIGNALWINTAB_H

#pragma ident	"@(#)mlib_i_SignalWinTab.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

extern const mlib_s16 mlib_i_Hanning_S16[];
extern const mlib_s16 mlib_i_Hamming_S16[];
extern const mlib_s16 mlib_i_Blackman_S16[];
extern const mlib_s16 mlib_i_Kaiser_S16[];

/* *********************************************************** */

#define	MLIB_MULWIN2P_S16S(WIN) {				\
	mlib_s32 i;						\
	mlib_s16 w0;						\
	for (i = i0 * 2; i < i1 * 2; i++) {			\
		w0 = WIN[i];					\
		dst[2 * i] = (mlib_s32)(src[2 * i] * 		\
			w0) >> 15;				\
		dst[2 * i + 1] = (mlib_s32)(src[2 * i + 1] * 	\
			w0) >> 15;				\
		dst_[-(2 * i)] = (mlib_s32)(src_[-(2 * i)] * 	\
			w0) >> 15;				\
		dst_[-(2 * i + 1)] = (mlib_s32)(		\
			src_[-(2 * i + 1)] *  w0) >> 15;	\
	}							\
	if ((i1 == n / 4) && (i < n / 2)) {			\
		w0 = WIN[i];					\
		dst[2 * i] = (mlib_s32)(src[2 * i] * 		\
			w0) >> 15;				\
		dst[2 * i + 1] = (mlib_s32)(src[2 * i + 1] * 	\
			w0) >> 15;				\
		dst_[-(2 * i)] = (mlib_s32)(src_[-(2 * i)] * 	\
			w0) >> 15;				\
		dst_[-(2 * i + 1)] = (mlib_s32)(		\
			src_[-(2 * i + 1)] *  w0) >> 15;	\
	}							\
}

/* *********************************************************** */

#define	MLIB_MULWIN2P_S16(WIN) {				\
	mlib_s32 i;						\
	mlib_s16 w0, w1;					\
	for (i = i0; i < i1; i++) {				\
		w0 = WIN[2 * i];				\
		w1 = WIN[2 * i + 1];				\
		dst[2 * i] = (mlib_s32)(src[2 * i] * 		\
			w0) >> 15;				\
		dst[2 * i + 1] = (mlib_s32)(src[2 * i + 1] * 	\
			w1) >> 15;				\
		dst_[-(2 * i)] = (mlib_s32)(src_[-(2 * i)] * 	\
			w0) >> 15;				\
		dst_[-(2 * i + 1)] = (mlib_s32)(		\
			src_[-(2 * i + 1)] *  w1) >> 15;	\
	}							\
	if ((n == 2) && ~i1) {					\
		w0 = WIN[0];					\
		dst[0] = (mlib_s32)(src[0] * w0) >> 15;		\
		dst[1] = (mlib_s32)(src[1] * w0) >> 15;		\
	}							\
}

/* *********************************************************** */

#define	MLIB_MULKAISER2P_S16S(WIN) {				\
	mlib_s32 i;						\
	mlib_s16 w0 = WIN[0];					\
	if (istart == 1) {					\
		dst[0] = (mlib_s32)(src[0] * 			\
				w0) >> 15;			\
		dst[1] = (mlib_s32)(src[1] * 			\
				w0) >> 15;			\
		dst_[0] = (mlib_s32)(src_[0] *			\
				w0) >> 15;			\
		dst_[-1] = (mlib_s32)(src_[-1] * 		\
				w0) >> 15;			\
	}							\
	for (i = istart; i <= iend; i++) {			\
		w0 = WIN[i];					\
		dst[2 * i] = (mlib_s32)(src[2 * i] * 		\
			w0) >> 15;				\
		dst[2 * i + 1] = (mlib_s32)(src[2 * i + 1] 	\
			*  w0) >> 15;				\
		dst_[-(2 * i)] = (mlib_s32)(src_[-(2 * i)] 	\
			*  w0) >> 15;				\
		dst_[-(2 * i + 1)] = (mlib_s32)(		\
			src_[-(2 * i + 1)] *  w0) >> 15;	\
	}							\
}

/* *********************************************************** */

#define	MLIB_MULKAISER2P_S16(WIN) {				\
	mlib_s32 i;						\
	mlib_s16 w0 = WIN[0];					\
	if (istart == 1) {					\
		dst[0] = (mlib_s32)(src[0] * 			\
				w0) >> 15;			\
		dst_[0] = (mlib_s32)(src_[0] *			\
				w0) >> 15;			\
	}							\
	for (i = istart; i <= iend; i++) {			\
		w0 = WIN[i];					\
		dst[i] = (mlib_s32)(src[i] * 			\
			w0) >> 15;				\
		dst_[-i] = (mlib_s32)(src_[-i] * 		\
			w0) >> 15;				\
	}							\
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_SIGNALWINTAB_H */
