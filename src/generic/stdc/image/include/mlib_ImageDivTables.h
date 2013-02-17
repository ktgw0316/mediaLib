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

#ifndef _MLIB_IMAGEDIVTABLES_H
#define	_MLIB_IMAGEDIVTABLES_H

#pragma ident	"@(#)mlib_ImageDivTables.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

#if defined(__SUNPRO_C) && defined(__DIV_TABLE_DEFINED)

#pragma align 64(mlib_div6_tab)
#pragma align 64(mlib_div1_tab)
#pragma align 64(mlib_HSL2RGB_L2)
#pragma align 64(mlib_HSL2RGB_F)
#pragma align 64(mlib_U82F32)
#pragma align 64(mlib_FlipAndFixRotateTable)

#endif		   /* defined(__SUNPRO_C) && defined(__DIV_TABLE_DEFINED) */

extern const mlib_u16 mlib_div6_tab[];
extern const mlib_u16 mlib_div1_tab[];
extern const mlib_f32 mlib_HSL2RGB_L2[];
extern const mlib_f32 mlib_HSL2RGB_F[];
extern const mlib_f32 mlib_U82F32[];
extern const mlib_d64 mlib_SQ_U8[];
extern const mlib_d64 mlib_U82D64[];
extern const mlib_u32 mlib_FlipAndFixRotateTable[];

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEDIVTABLES_H */
