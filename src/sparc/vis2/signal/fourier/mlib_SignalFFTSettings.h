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

#ifndef _MLIB_SIGNALFFTSETTINGS_H
#define	_MLIB_SIGNALFFTSETTINGS_H

#pragma ident	"@(#)mlib_SignalFFTSettings.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MLIB_VIS
#error this include file can be used with MLIB_VIS >= 2.0
#elif MLIB_VIS < 0x200
#error this include file can be used with MLIB_VIS >= 2.0
#endif

/* ************************************************* */
/* ************************************************* */
/*  cache sizes, layers strategies etc...  */

/* ************************************************* */

/*  cache sizes controlling values  */
/*  14 and 18 for UII, 16 and 23 for UIII  */
/*  ( 1 << this ) == bytes in L1 cache  */
#ifndef CACHE1_ORD_BYTES
#define	CACHE1_ORD_BYTES	16
#endif /* CACHE1_ORD_BYTES */
/*  ( 1 << this ) == bytes in L2 cache  */
#ifndef CACHE2_ORD_BYTES
#define	CACHE2_ORD_BYTES	23
#endif /* CACHE2_ORD_BYTES */

/*  MORE_3LAYERS = worse calculations, better loads-stores (eg for UIII)  */
#ifndef MORE_3LAYERS
#define	MORE_3LAYERS	1
#endif /* MORE_3LAYERS */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALFFTSETTINGS_H */
