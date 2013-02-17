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

#pragma ident	"@(#)mlib_SignalWindowUtil.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_exp - computes the exponential of x.
 *
 * SYNOPSIS
 *      mlib_d64 mlib_exp (mlib_d64 x, mlib_s32 *shift);
 *
 * DESCRIPTION
 *      The argument must be x >= 0.
 *      The result is exp(x) = mlib_exp(x, &shift) * 2^shift.
 */

/*
 * FUNCTION
 *      mlib_i0 - computes the modified zero-order Bessel
 *                function of the first kind.
 *
 * SYNOPSIS
 *      mlib_d64 mlib_i0 (mlib_d64 x, mlib_s32 *shift);
 *
 * DESCRIPTION
 *      The argument must be x >= 0.
 *      The result is I0(x) = mlib_i0(x, &shift) * 2^shift.
 */

/*
 * FUNCTION
 *      mlib_sincospi - computes the sine and cosine functions together.
 *
 * SYNOPSIS
 *      mlib_d64 mlib_sincospi (mlib_d64 *x, mlib_d64 *CO);
 *
 * ARGUMENTS
 *      x    argument must be in terms of pi (i.e. sincos(x * pi)).
 *      CO   pointer to the output cosine value
 *
 * DESCRIPTION
 *      function mlib_sincospi return a value of sine for "x" argument.
 *      The argument must be in terms of pi and in the range of 0
 *      to 2**31.
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindowUtil.h>

/* *********************************************************** */

/*
 *        Table of constants for mlib_exp
 */

const mlib_d64 mlib_constexp[] = {
/* 2 ** (  0/128) */
	1.0000000000000000000e+00,
/* 2 ** (  1/128) */
	1.0054299011128027264e+00,
/* 2 ** (  2/128) */
	1.0108892860517004753e+00,
/* 2 ** (  3/128) */
	1.0163783149109530957e+00,
/* 2 ** (  4/128) */
	1.0218971486541166271e+00,
/* 2 ** (  5/128) */
	1.0274459491187637461e+00,
/* 2 ** (  6/128) */
	1.0330248790212284149e+00,
/* 2 ** (  7/128) */
	1.0386341019613787306e+00,
/* 2 ** (  8/128) */
	1.0442737824274137548e+00,
/* 2 ** (  9/128) */
	1.0499440858006872102e+00,
/* 2 ** ( 10/128) */
	1.0556451783605571570e+00,
/* 2 ** ( 11/128) */
	1.0613772272892620929e+00,
/* 2 ** ( 12/128) */
	1.0671404006768236972e+00,
/* 2 ** ( 13/128) */
	1.0729348675259755552e+00,
/* 2 ** ( 14/128) */
	1.0787607977571198603e+00,
/* 2 ** ( 15/128) */
	1.0846183622133092062e+00,
/* 2 ** ( 16/128) */
	1.0905077326652576897e+00,
/* 2 ** ( 17/128) */
	1.0964290818163768826e+00,
/* 2 ** ( 18/128) */
	1.1023825833078408909e+00,
/* 2 ** ( 19/128) */
	1.1083684117236787259e+00,
/* 2 ** ( 20/128) */
	1.1143867425958924322e+00,
/* 2 ** ( 21/128) */
	1.1204377524096067464e+00,
/* 2 ** ( 22/128) */
	1.1265216186082418481e+00,
/* 2 ** ( 23/128) */
	1.1326385195987191956e+00,
/* 2 ** ( 24/128) */
	1.1387886347566915646e+00,
/* 2 ** ( 25/128) */
	1.1449721444318041730e+00,
/* 2 ** ( 26/128) */
	1.1511892299529826733e+00,
/* 2 ** ( 27/128) */
	1.1574400736337511209e+00,
/* 2 ** ( 28/128) */
	1.1637248587775774755e+00,
/* 2 ** ( 29/128) */
	1.1700437696832501899e+00,
/* 2 ** ( 30/128) */
	1.1763969916502812207e+00,
/* 2 ** ( 31/128) */
	1.1827847109843410145e+00,
/* 2 ** ( 32/128) */
	1.1892071150027210269e+00,
/* 2 ** ( 33/128) */
	1.1956643920398273284e+00,
/* 2 ** ( 34/128) */
	1.2021567314527030756e+00,
/* 2 ** ( 35/128) */
	1.2086843236265816248e+00,
/* 2 ** ( 36/128) */
	1.2152473599804689552e+00,
/* 2 ** ( 37/128) */
	1.2218460329727574010e+00,
/* 2 ** ( 38/128) */
	1.2284805361068700247e+00,
/* 2 ** ( 39/128) */
	1.2351510639369331912e+00,
/* 2 ** ( 40/128) */
	1.2418578120734840020e+00,
/* 2 ** ( 41/128) */
	1.2486009771892048192e+00,
/* 2 ** ( 42/128) */
	1.2553807570246910963e+00,
/* 2 ** ( 43/128) */
	1.2621973503942507389e+00,
/* 2 ** ( 44/128) */
	1.2690509571917332199e+00,
/* 2 ** ( 45/128) */
	1.2759417783963922233e+00,
/* 2 ** ( 46/128) */
	1.2828700160787782636e+00,
/* 2 ** ( 47/128) */
	1.2898358734066657227e+00,
/* 2 ** ( 48/128) */
	1.2968395546510096406e+00,
/* 2 ** ( 49/128) */
	1.3038812651919360341e+00,
/* 2 ** ( 50/128) */
	1.3109612115247644137e+00,
/* 2 ** ( 51/128) */
	1.3180796012660640493e+00,
/* 2 ** ( 52/128) */
	1.3252366431597413232e+00,
/* 2 ** ( 53/128) */
	1.3324325470831615004e+00,
/* 2 ** ( 54/128) */
	1.3396675240533029161e+00,
/* 2 ** ( 55/128) */
	1.3469417862329458035e+00,
/* 2 ** ( 56/128) */
	1.3542555469368926513e+00,
/* 2 ** ( 57/128) */
	1.3616090206382247541e+00,
/* 2 ** ( 58/128) */
	1.3690024229745905160e+00,
/* 2 ** ( 59/128) */
	1.3764359707545299472e+00,
/* 2 ** ( 60/128) */
	1.3839098819638318005e+00,
/* 2 ** ( 61/128) */
	1.3914243757719262362e+00,
/* 2 ** ( 62/128) */
	1.3989796725383110143e+00,
/* 2 ** ( 63/128) */
	1.4065759938190154354e+00,
/* 2 ** ( 64/128) */
	1.4142135623730949234e+00,
/* 2 ** ( 65/128) */
	1.4218926021691655759e+00,
/* 2 ** ( 66/128) */
	1.4296133383919700233e+00,
/* 2 ** ( 67/128) */
	1.4373759974489823676e+00,
/* 2 ** ( 68/128) */
	1.4451808069770466503e+00,
/* 2 ** ( 69/128) */
	1.4530279958490526226e+00,
/* 2 ** ( 70/128) */
	1.4609177941806468226e+00,
/* 2 ** ( 71/128) */
	1.4688504333369820642e+00,
/* 2 ** ( 72/128) */
	1.4768261459394993462e+00,
/* 2 ** ( 73/128) */
	1.4848451658727523927e+00,
/* 2 ** ( 74/128) */
	1.4929077282912648350e+00,
/* 2 ** ( 75/128) */
	1.5010140696264255844e+00,
/* 2 ** ( 76/128) */
	1.5091644275934228414e+00,
/* 2 ** ( 77/128) */
	1.5173590411982147419e+00,
/* 2 ** ( 78/128) */
	1.5255981507445381951e+00,
/* 2 ** ( 79/128) */
	1.5338819978409561351e+00,
/* 2 ** ( 80/128) */
	1.5422108254079409662e+00,
/* 2 ** ( 81/128) */
	1.5505848776849999737e+00,
/* 2 ** ( 82/128) */
	1.5590044002378369292e+00,
/* 2 ** ( 83/128) */
	1.5674696399655529966e+00,
/* 2 ** ( 84/128) */
	1.5759808451078864966e+00,
/* 2 ** ( 85/128) */
	1.5845382652524937495e+00,
/* 2 ** ( 86/128) */
	1.5931421513422669989e+00,
/* 2 ** ( 87/128) */
	1.6017927556826934143e+00,
/* 2 ** ( 88/128) */
	1.6104903319492542835e+00,
/* 2 ** ( 89/128) */
	1.6192351351948637284e+00,
/* 2 ** ( 90/128) */
	1.6280274218573478340e+00,
/* 2 ** ( 91/128) */
	1.6368674497669646328e+00,
/* 2 ** ( 92/128) */
	1.6457554781539647237e+00,
/* 2 ** ( 93/128) */
	1.6546917676561943011e+00,
/* 2 ** ( 94/128) */
	1.6636765803267363761e+00,
/* 2 ** ( 95/128) */
	1.6727101796415966284e+00,
/* 2 ** ( 96/128) */
	1.6817928305074292261e+00,
/* 2 ** ( 97/128) */
	1.6909247992693052787e+00,
/* 2 ** ( 98/128) */
	1.7001063537185234775e+00,
/* 2 ** ( 99/128) */
	1.7093377631004629258e+00,
/* 2 ** (100/128) */
	1.7186192981224779341e+00,
/* 2 ** (101/128) */
	1.7279512309618376698e+00,
/* 2 ** (102/128) */
	1.7373338352737062174e+00,
/* 2 ** (103/128) */
	1.7467673861991688256e+00,
/* 2 ** (104/128) */
	1.7562521603732994535e+00,
/* 2 ** (105/128) */
	1.7657884359332727264e+00,
/* 2 ** (106/128) */
	1.7753764925265211883e+00,
/* 2 ** (107/128) */
	1.7850166113189349648e+00,
/* 2 ** (108/128) */
	1.7947090750031071682e+00,
/* 2 ** (109/128) */
	1.8044541678066239321e+00,
/* 2 ** (110/128) */
	1.8142521755003988559e+00,
/* 2 ** (111/128) */
	1.8241033854070534126e+00,
/* 2 ** (112/128) */
	1.8340080864093424307e+00,
/* 2 ** (113/128) */
	1.8439665689586259845e+00,
/* 2 ** (114/128) */
	1.8539791250833856928e+00,
/* 2 ** (115/128) */
	1.8640460483977889794e+00,
/* 2 ** (116/128) */
	1.8741676341102999626e+00,
/* 2 ** (117/128) */
	1.8843441790323345320e+00,
/* 2 ** (118/128) */
	1.8945759815869656073e+00,
/* 2 ** (119/128) */
	1.9048633418176741383e+00,
/* 2 ** (120/128) */
	1.9152065613971474001e+00,
/* 2 ** (121/128) */
	1.9256059436361250281e+00,
/* 2 ** (122/128) */
	1.9360617934922945693e+00,
/* 2 ** (123/128) */
	1.9465744175792332182e+00,
/* 2 ** (124/128) */
	1.9571441241754001794e+00,
/* 2 ** (125/128) */
	1.9677712232331758813e+00,
/* 2 ** (126/128) */
	1.9784560263879509279e+00,
	1.9891988469672663431e+00	/* 2 ** (127/128) */
};

/* *********************************************************** */

mlib_d64
mlib_exp(
    mlib_d64 x,
    mlib_s32 *shift)
{
	mlib_d64 y = K128ONLN2 * x;
	mlib_s32 k = (mlib_s32)y;

	y -= (mlib_d64)k;
	y = KE2 * y * (y + KE1_2) + 1;
	*shift = k >> 7;
	return (mlib_constexp[k & 0x7f] * y);
}

/* *********************************************************** */

mlib_d64
mlib_i0(
    mlib_d64 x,
    mlib_s32 *shift)
{
	mlib_d64 t1, t2, t3, t4;

	if (x < 15) {
		mlib_d64 yv2 = x * x;
		mlib_d64 yv4 = yv2 * yv2;
		mlib_d64 yv8 = yv4 * yv4;
		mlib_d64 yv16 = yv8 * yv8;

		*shift = 0;
		return KA0 + KA3 * yv2 * (KA1_KA3 + KA2_KA3 * yv2 + yv4) +
		    KA7 * yv8 * (KA4_KA7 + KA5_KA7 * yv2 + yv4 * (KA6_KA7 +
		    yv2)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * yv2 +
		    yv4 * (KA10_KA11 + yv2)) + KA15 * yv8 * yv16 * (KA12_KA15 +
		    KA13_KA15 * yv2 + yv4 * (KA14_KA15 + yv2));
	} else {
		t3 = 1. / ((x + KQ1) * x + KQ2);
		t1 = mlib_sqrt(x);
		t2 = mlib_exp(x, shift);
		t4 = (KP0 * x + KP1);
		return (t1 * t2 * t3 * t4);
	}
}

/* *********************************************************** */

#define	DVAIN51	2.251799813685248e+15
#define	KB8	2.31328784993344125851e-01
#define	KB6	-1.33504444801690092992e+00
#define	KB4	4.05870725848181560183e+00
#define	KB2	-4.93480216253264887827e+00
#define	KB0	9.99999999952489416693e-01
#define	KB9	8.10016274209163915799e-02
#define	KB7	-5.99202080860959420520e-01
#define	KB5	2.55016264722017983407e+00
#define	KB3	-5.16771276917436280000e+00
#define	KB1	3.14159265357619980718e+00
#define	KB3_KB5	-2.02642477522265462358e+00
#define	KB7_KB9	-7.39740792795765902874e+00

/* *********************************************************** */

mlib_d64
mlib_sincospi(
    mlib_d64 x,
    mlib_d64 *CO)
{
	type_union_mlib_d64 arg, psi, pco;
	mlib_d64 y, yv2, yv4;
	mlib_s32 k;

#ifdef i386
/* because compiler bug for x86 */
	k = (mlib_s32)(x * 2 + 0.5);
	y = x - k * 0.5;
	k += 2;
#else /* i386 */
	arg.value = x + DVAIN51;
	k = arg.two_int.int1 + 2;
	y = x - (arg.value - DVAIN51);
#endif /* i386 */

	yv2 = y * y;
	yv4 = yv2 * yv2;
	psi.value = -KB1 * y - KB5 * y * yv2 * (KB3_KB5 + yv2) -
	    KB9 * y * yv2 * yv4 * (KB7_KB9 + yv2);
	y = -KB0 - KB2 * yv2 - KB4 * yv4 - yv2 * yv4 * (KB6 + KB8 * yv2);

	if (k & 1) {
		pco = psi;
		psi.value = y;
	} else
		pco.value = y;
	psi.two_int.int0 += ((k & 2) << 30);
	pco.two_int.int0 += (k + (k & 1)) << 30;
	*CO = pco.value;
	return (psi.value);
}

/* *********************************************************** */
