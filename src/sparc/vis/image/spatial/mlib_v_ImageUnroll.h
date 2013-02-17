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

#ifndef _MLIB_V_IMAGEUNROLL_H
#define	_MLIB_V_IMAGEUNROLL_H

#pragma ident	"@(#)mlib_v_ImageUnroll.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DESCRIPTION
 *      Preprocessor defines for loop unrolling in assembler
 */

/* *********************************************************** */

#ifndef MLIB_IMAGE_UNROLL_H
#define	MLIB_IMAGE_UNROLL_H

#define	I0(x)	x##0
#define	I1(x)	x##2
#define	I2(x)	x##4
#define	I3(x)	x##6
#define	I4(x)	x##8
#define	I5(x)	x##A

#define	f1A	f2
#define	f2A	f4
#define	f3A	f6
#define	f4A	f8
#define	f5A	f0

#endif /* MLIB_IMAGE_UNROLL_H */

/* *********************************************************** */

#undef CALC
#undef LAST
#undef STOR

#undef I05
#undef I04
#undef I03
#undef I02
#undef I01
#undef I00
#undef I15
#undef I14
#undef I13
#undef I12
#undef I11
#undef I10
#undef I25
#undef I24
#undef I23
#undef I22
#undef I21
#undef I20
#undef I35
#undef I34
#undef I33
#undef I32
#undef I31
#undef I30
#undef I45
#undef I44
#undef I43
#undef I42
#undef I41
#undef I40

/* *********************************************************** */

#if UNROLL == 3

#define	I02	I0
#define	I01	I1
#define	I00	I2

#define	I12	I1
#define	I11	I2
#define	I10	I0

#define	I22	I2
#define	I21	I0
#define	I20	I1

#elif UNROLL == 4	/* UNROLL == 3 */

#define	I03	I0
#define	I02	I1
#define	I01	I2
#define	I00	I3

#define	I13	I1
#define	I12	I2
#define	I11	I3
#define	I10	I0

#define	I23	I2
#define	I22	I3
#define	I21	I0
#define	I20	I1

#elif UNROLL == 5	/* UNROLL == 3 */

#define	I04	I0
#define	I03	I1
#define	I02	I2
#define	I01	I3
#define	I00	I4

#define	I14	I1
#define	I13	I2
#define	I12	I3
#define	I11	I4
#define	I10	I0

#define	I24	I2
#define	I23	I3
#define	I22	I4
#define	I21	I0
#define	I20	I1

#define	I34	I3
#define	I33	I4
#define	I32	I0
#define	I31	I1
#define	I30	I2

#define	I44	I4
#define	I43	I0
#define	I42	I1
#define	I41	I2
#define	I40	I3

#elif UNROLL == 6	/* UNROLL == 3 */

#define	I05	I0
#define	I04	I1
#define	I03	I2
#define	I02	I3
#define	I01	I4
#define	I00	I5

#define	I15	I1
#define	I14	I2
#define	I13	I3
#define	I12	I4
#define	I11	I5
#define	I10	I0

#define	I25	I2
#define	I24	I3
#define	I23	I4
#define	I22	I5
#define	I21	I0
#define	I20	I1

#define	I35	I3
#define	I34	I4
#define	I33	I5
#define	I32	I0
#define	I31	I1
#define	I30	I2

#define	I45	I4
#define	I44	I5
#define	I43	I0
#define	I42	I1
#define	I41	I2
#define	I40	I3

#define	I55	I5
#define	I54	I0
#define	I53	I1
#define	I52	I2
#define	I51	I3
#define	I50	I4

#endif /* UNROLL == 3 */

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEUNROLL_H */
