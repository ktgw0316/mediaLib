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

#ifndef _MLIB_SIGNALWINDOWUTIL_H
#define	_MLIB_SIGNALWINDOWUTIL_H

#pragma ident	"@(#)mlib_SignalWindowUtil.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

mlib_d64 mlib_exp(
    mlib_d64 x,
    mlib_s32 *shift);
mlib_d64 mlib_i0(
    mlib_d64 x,
    mlib_s32 *shift);

/*
 * mlib_d64 mlib_sincospi (mlib_d64 x, mlib_d64 *CO);
 */

extern const mlib_d64 mlib_constexp[];

/* *********************************************************** */
typedef union
{
	mlib_d64 value;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_s32 int1, int0;
#else
		mlib_s32 int0, int1;
#endif		   /* _LITTLE_ENDIAN */
	} two_int;
} type_union_mlib_d64;

/* *********************************************************** */
/* constants for mlib_exp function */

#define	KE2	1.46887601897442005360e-05
#define	KE1	5.41520903533885414090e-03
#define	KE1_2	3.68663451876611926242e+02
#define	K128ONLN2	184.664965233787313537

/* constants for mlib_i0 function */

#define	KA15	3.01994344786611594760e-33
#define	KA14	-2.05956528054275859961e-30
#define	KA13	1.77951753584904639219e-27
#define	KA12	-2.27419788500891516854e-25
#define	KA11	2.66812640346831872737e-22
#define	KA10	5.22744157378584507964e-20
#define	KA9	3.14881298909946669588e-17
#define	KA8	9.15610281123052177158e-15
#define	KA7	2.41800423176749703465e-12
#define	KA6	4.70235620035171105777e-10
#define	KA5	6.78399989665250597199e-08
#define	KA4	6.78119250827281897757e-06
#define	KA3	4.34034074279533363692e-04
#define	KA2	1.56249577093511678657e-02
#define	KA1	2.50000112128937745191e-01
#define	KA0	9.99999950708564499693e-01
#define	KP1	-4.57221293076591814741e-01
#define	KP0	3.98942276319758936876e-01
#define	KQ2	8.88157626449593940489e-02
#define	KQ1	-1.27108747030136657937e+00
#define	KA1_KA3	5.75991902349880464149e+02
#define	KA2_KA3	3.59993803142933427748e+01
#define	KA4_KA7	2.80445849481245403123e+06
#define	KA5_KA7	2.80561953015838263115e+04
#define	KA6_KA7	1.94472620790841761765e+02
#define	KA8_KA11	3.43166005903184756272e+07
#define	KA9_KA11	1.18015885042264101644e+05
#define	KA10_KA11	1.95921811162718985580e+02
#define	KA12_KA15	-7.53059758988486151537e+07
#define	KA13_KA15	5.89255251487059724953e+05
#define	KA14_KA15	-6.81988029278509165201e+02

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SIGNALWINDOWUTIL_H */
