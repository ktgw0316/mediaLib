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

#ifndef _MLIB_C_VIDEOCOLORYUV_H
#define	_MLIB_C_VIDEOCOLORYUV_H

#pragma ident	"@(#)mlib_c_VideoColorYUV.h	9.2	07/10/09 SMI"

/*
 * DESCRIPTION:
 *
 * mlib_u8  mlib_tClip[288*2 + 544*2] U8 Saturation table with zero in 288*2
 * mlib_u16 mlib_tYY[i] = (int)(i*1.1644*2 + 0.49999999f)
 * mlib_u16 mlib_tRV[i] = (int)(i*1.5966*2 + 0.49999999f)
 * mlib_u16 mlib_tGU[i] = (int)(135.6352*2 - i*0.3920*2 + 0.49999999f)
 * mlib_u16 mlib_tGV[i] = (int)(i*0.8132*2 + 0.49999999f)
 * mlib_u16 mlib_tBU[i] = (int)(i*2.0184*2 + 0.49999999f)
 *
 * REFERENCING:
 *
 *     mlib_c_VideoColorColorYUV2ABGR411
 *     mlib_c_VideoColorColorYUV2ABGR420
 *     mlib_c_VideoColorColorYUV2ABGR422
 *     mlib_c_VideoColorColorYUV2ABGR444
 *     mlib_c_VideoColorColorYUV2ARGB411
 *     mlib_c_VideoColorColorYUV2ARGB420
 *     mlib_c_VideoColorColorYUV2ARGB422
 *     mlib_c_VideoColorColorYUV2ARGB444
 *     mlib_c_VideoColorColorYUV2RGB411
 *     mlib_c_VideoColorColorYUV2RGB420
 *     mlib_c_VideoColorColorYUV2RGB422
 *     mlib_c_VideoColorColorYUV2RGB444
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_video.h>

extern const mlib_u8 mlib_tClip[288 * 2 + 544 * 2];

/* tables for YUV -> RGB conversions without sampling */
extern const mlib_u8 mlib_video_pClipOff256[800];
extern const mlib_s32 mlib_t_Yuv2rgb[256];
extern const mlib_s32 mlib_t_yUv2rgb[256];
extern const mlib_s32 mlib_t_yuV2rgb[256];

/* tables for YUV -> RGB conversions with 422, 420 and 411 sampling */
extern const mlib_u16 mlib_tYY[256];
extern const mlib_u16 mlib_tRV[256];
extern const mlib_u16 mlib_tGU[256];
extern const mlib_u16 mlib_tGV[256];
extern const mlib_u16 mlib_tBU[256];

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_VIDEOCOLORYUV_H */
