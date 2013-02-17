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

#pragma ident	"@(#)mlib_AlgebraUtil.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_atan2i - computes the inverse tangent (arctan) of yy / xx.
 *
 * SYNOPSIS
 *      mlib_d64 mlib_atan2i (mlib_s32 yy, mlib_s32 xx);
 *
 * DESCRIPTION
 *      mlib_atan2i return a value in radians, in the range of -pi to pi.
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

const mlib_d64 mlib_SU82D64[] = {
	-128.0, -127.0, -126.0, -125.0, -124.0, -123.0, -122.0, -121.0,
	-120.0, -119.0, -118.0, -117.0, -116.0, -115.0, -114.0, -113.0,
	-112.0, -111.0, -110.0, -109.0, -108.0, -107.0, -106.0, -105.0,
	-104.0, -103.0, -102.0, -101.0, -100.0, -99.0, -98.0, -97.0,
	-96.0, -95.0, -94.0, -93.0, -92.0, -91.0, -90.0, -89.0,
	-88.0, -87.0, -86.0, -85.0, -84.0, -83.0, -82.0, -81.0,
	-80.0, -79.0, -78.0, -77.0, -76.0, -75.0, -74.0, -73.0,
	-72.0, -71.0, -70.0, -69.0, -68.0, -67.0, -66.0, -65.0,
	-64.0, -63.0, -62.0, -61.0, -60.0, -59.0, -58.0, -57.0,
	-56.0, -55.0, -54.0, -53.0, -52.0, -51.0, -50.0, -49.0,
	-48.0, -47.0, -46.0, -45.0, -44.0, -43.0, -42.0, -41.0,
	-40.0, -39.0, -38.0, -37.0, -36.0, -35.0, -34.0, -33.0,
	-32.0, -31.0, -30.0, -29.0, -28.0, -27.0, -26.0, -25.0,
	-24.0, -23.0, -22.0, -21.0, -20.0, -19.0, -18.0, -17.0,
	-16.0, -15.0, -14.0, -13.0, -12.0, -11.0, -10.0, -9.0,
	-8.0, -7.0, -6.0, -5.0, -4.0, -3.0, -2.0, -1.0,
	0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0,
	8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0,
	16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0,
	24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0,
	32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
	40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0,
	48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0,
	56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0,
	64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0,
	72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0, 79.0,
	80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0,
	88.0, 89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0,
	96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0,
	104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0,
	112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0,
	120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0,
	128.0, 129.0, 130.0, 131.0, 132.0, 133.0, 134.0, 135.0,
	136.0, 137.0, 138.0, 139.0, 140.0, 141.0, 142.0, 143.0,
	144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0,
	152.0, 153.0, 154.0, 155.0, 156.0, 157.0, 158.0, 159.0,
	160.0, 161.0, 162.0, 163.0, 164.0, 165.0, 166.0, 167.0,
	168.0, 169.0, 170.0, 171.0, 172.0, 173.0, 174.0, 175.0,
	176.0, 177.0, 178.0, 179.0, 180.0, 181.0, 182.0, 183.0,
	184.0, 185.0, 186.0, 187.0, 188.0, 189.0, 190.0, 191.0,
	192.0, 193.0, 194.0, 195.0, 196.0, 197.0, 198.0, 199.0,
	200.0, 201.0, 202.0, 203.0, 204.0, 205.0, 206.0, 207.0,
	208.0, 209.0, 210.0, 211.0, 212.0, 213.0, 214.0, 215.0,
	216.0, 217.0, 218.0, 219.0, 220.0, 221.0, 222.0, 223.0,
	224.0, 225.0, 226.0, 227.0, 228.0, 229.0, 230.0, 231.0,
	232.0, 233.0, 234.0, 235.0, 236.0, 237.0, 238.0, 239.0,
	240.0, 241.0, 242.0, 243.0, 244.0, 245.0, 246.0, 247.0,
	248.0, 249.0, 250.0, 251.0, 252.0, 253.0, 254.0, 255.0
};

/* *********************************************************** */

const mlib_d64 mlib_S82D64[] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87,
	88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103,
	104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127,

	128, 127, 126, 125, 124, 123, 122, 121,
	120, 119, 118, 117, 116, 115, 114, 113,
	112, 111, 110, 109, 108, 107, 106, 105,
	104, 103, 102, 101, 100, 99, 98, 97,
	96, 95, 94, 93, 92, 91, 90, 89,
	88, 87, 86, 85, 84, 83, 82, 81,
	80, 79, 78, 77, 76, 75, 74, 73,
	72, 71, 70, 69, 68, 67, 66, 65,
	64, 63, 62, 61, 60, 59, 58, 57,
	56, 55, 54, 53, 52, 51, 50, 49,
	48, 47, 46, 45, 44, 43, 42, 41,
	40, 39, 38, 37, 36, 35, 34, 33,
	32, 31, 30, 29, 28, 27, 26, 25,
	24, 23, 22, 21, 20, 19, 18, 17,
	16, 15, 14, 13, 12, 11, 10, 9,
	8, 7, 6, 5, 4, 3, 2, 1,

};

/* *********************************************************** */

/*
 *        Table of constants for mlib_atan2i
 */

const mlib_d64 mlib_constarctgf[] = {
/* atanf (0x3e800000) */
	2.4497866312686414347e-01,
/* atanf (0x3e820000) */
	2.4865174119051325352e-01,
/* atanf (0x3e840000) */
	2.5231798088642720668e-01,
/* atanf (0x3e860000) */
	2.5597730301300553046e-01,
/* atanf (0x3e880000) */
	2.5962962940825751179e-01,
/* atanf (0x3e8a0000) */
	2.6327488295528239659e-01,
/* atanf (0x3e8c0000) */
	2.6691298758740045205e-01,
/* atanf (0x3e8e0000) */
	2.7054386829293658456e-01,
/* atanf (0x3e900000) */
	2.7416745111965878934e-01,
/* atanf (0x3e920000) */
	2.7778366317887326353e-01,
/* atanf (0x3e940000) */
	2.8139243264917845888e-01,
/* atanf (0x3e960000) */
	2.8499368877988129345e-01,
/* atanf (0x3e980000) */
	2.8858736189407740991e-01,
/* atanf (0x3e9a0000) */
	2.9217338339139875547e-01,
/* atanf (0x3e9c0000) */
	2.9575168575043153663e-01,
/* atanf (0x3e9e0000) */
	2.9932220253080743522e-01,
/* atanf (0x3ea00000) */
	3.0288486837497141657e-01,
/* atanf (0x3ea20000) */
	3.0643961900963012646e-01,
/* atanf (0x3ea40000) */
	3.0998639124688343038e-01,
/* atanf (0x3ea60000) */
	3.1352512298504392474e-01,
/* atanf (0x3ea80000) */
	3.1705575320914702875e-01,
/* atanf (0x3eaa0000) */
	3.2057822199115698636e-01,
/* atanf (0x3eac0000) */
	3.2409247048987172013e-01,
/* atanf (0x3eae0000) */
	3.2759844095053086699e-01,
/* atanf (0x3eb00000) */
	3.3109607670413210290e-01,
/* atanf (0x3eb20000) */
	3.3458532216645897606e-01,
/* atanf (0x3eb40000) */
	3.3806612283682546671e-01,
/* atanf (0x3eb60000) */
	3.4153842529654171445e-01,
/* atanf (0x3eb80000) */
	3.4500217720710513181e-01,
/* atanf (0x3eba0000) */
	3.4845732730812206679e-01,
/* atanf (0x3ebc0000) */
	3.5190382541496478819e-01,
/* atanf (0x3ebe0000) */
	3.5534162241616834566e-01,
/* atanf (0x3ec00000) */
	3.5877067027057224502e-01,
/* atanf (0x3ec20000) */
	3.6219092200421221239e-01,
/* atanf (0x3ec40000) */
	3.6560233170696687655e-01,
/* atanf (0x3ec60000) */
	3.6900485452896442107e-01,
/* atanf (0x3ec80000) */
	3.7239844667675420231e-01,
/* atanf (0x3eca0000) */
	3.7578306540924893975e-01,
/* atanf (0x3ecc0000) */
	3.7915866903344180860e-01,
/* atanf (0x3ece0000) */
	3.8252521689990515164e-01,
/* atanf (0x3ed00000) */
	3.8588266939807375211e-01,
/* atanf (0x3ed20000) */
	3.8923098795132071714e-01,
/* atanf (0x3ed40000) */
	3.9257013501182858040e-01,
/* atanf (0x3ed60000) */
	3.9590007405526289608e-01,
/* atanf (0x3ed80000) */
	3.9922076957525254315e-01,
/* atanf (0x3eda0000) */
	4.0253218707768251283e-01,
/* atanf (0x3edc0000) */
	4.0583429307480411996e-01,
/* atanf (0x3ede0000) */
	4.0912705507916830028e-01,
/* atanf (0x3ee00000) */
	4.1241044159738732278e-01,
/* atanf (0x3ee20000) */
	4.1568442212372941347e-01,
/* atanf (0x3ee40000) */
	4.1894896713355284090e-01,
/* atanf (0x3ee60000) */
	4.2220404807658362678e-01,
/* atanf (0x3ee80000) */
	4.2544963737004226623e-01,
/* atanf (0x3eea0000) */
	4.2868570839162578601e-01,
/* atanf (0x3eec0000) */
	4.3191223547234819380e-01,
/* atanf (0x3eee0000) */
	4.3512919388924686803e-01,
/* atanf (0x3ef00000) */
	4.3833655985795783039e-01,
/* atanf (0x3ef20000) */
	4.4153431052516672883e-01,
/* atanf (0x3ef40000) */
	4.4472242396093936145e-01,
/* atanf (0x3ef60000) */
	4.4790087915093729221e-01,
/* atanf (0x3ef80000) */
	4.5106965598852349908e-01,
/* atanf (0x3efa0000) */
	4.5422873526676249556e-01,
/* atanf (0x3efc0000) */
	4.5737809867032080957e-01,
/* atanf (0x3efe0000) */
	4.6051772876727109507e-01,
/* atanf (0x3f000000) */
	4.6364760900080609352e-01,
/* atanf (0x3f020000) */
	4.6987805797568693578e-01,
/* atanf (0x3f040000) */
	4.7606933032276121942e-01,
/* atanf (0x3f060000) */
	4.8222132422785374262e-01,
/* atanf (0x3f080000) */
	4.8833395105640553524e-01,
/* atanf (0x3f0a0000) */
	4.9440713507127537207e-01,
/* atanf (0x3f0c0000) */
	5.0044081314729416121e-01,
/* atanf (0x3f0e0000) */
	5.0643493448309673255e-01,
/* atanf (0x3f100000) */
	5.1238946031073773213e-01,
/* atanf (0x3f120000) */
	5.1830436360357801107e-01,
/* atanf (0x3f140000) */
	5.2417962878291324280e-01,
/* atanf (0x3f160000) */
	5.3001525142379313227e-01,
/* atanf (0x3f180000) */
	5.3581123796046370433e-01,
/* atanf (0x3f1a0000) */
	5.4156760539184500836e-01,
/* atanf (0x3f1c0000) */
	5.4728438098743692475e-01,
/* atanf (0x3f1e0000) */
	5.5296160199402832891e-01,
/* atanf (0x3f200000) */
	5.5859931534356244143e-01,
/* atanf (0x3f220000) */
	5.6419757736249764868e-01,
/* atanf (0x3f240000) */
	5.6975645348297843107e-01,
/* atanf (0x3f260000) */
	5.7527601795611782443e-01,
/* atanf (0x3f280000) */
	5.8075635356767041362e-01,
/* atanf (0x3f2a0000) */
	5.8619755135636064658e-01,
/* atanf (0x3f2c0000) */
	5.9159971033511138394e-01,
/* atanf (0x3f2e0000) */
	5.9696293721540150123e-01,
/* atanf (0x3f300000) */
	6.0228734613496415218e-01,
/* atanf (0x3f320000) */
	6.0757305838902242456e-01,
/* atanf (0x3f340000) */
	6.1282020216524135670e-01,
/* atanf (0x3f360000) */
	6.1802891228256184863e-01,
/* atanf (0x3f380000) */
	6.2319932993406590427e-01,
/* atanf (0x3f3a0000) */
	6.2833160243400965062e-01,
/* atanf (0x3f3c0000) */
	6.3342588296914459356e-01,
/* atanf (0x3f3e0000) */
	6.3848233035443757721e-01,
/* atanf (0x3f400000) */
	6.4350110879328437097e-01,
/* atanf (0x3f420000) */
	6.4848238764230059505e-01,
/* atanf (0x3f440000) */
	6.5342634118076192706e-01,
/* atanf (0x3f460000) */
	6.5833314838475598396e-01,
/* atanf (0x3f480000) */
	6.6320299270609328612e-01,
/* atanf (0x3f4a0000) */
	6.6803606185602015799e-01,
/* atanf (0x3f4c0000) */
	6.7283254759376320830e-01,
/* atanf (0x3f4e0000) */
	6.7759264551992515191e-01,
/* atanf (0x3f500000) */
	6.8231655487474807131e-01,
/* atanf (0x3f520000) */
	6.8700447834124500623e-01,
/* atanf (0x3f540000) */
	6.9165662185319987110e-01,
/* atanf (0x3f560000) */
	6.9627319440802359907e-01,
/* atanf (0x3f580000) */
	7.0085440788445019233e-01,
/* atanf (0x3f5a0000) */
	7.0540047686504903091e-01,
/* atanf (0x3f5c0000) */
	7.0991161846352490716e-01,
/* atanf (0x3f5e0000) */
	7.1438805215676903781e-01,
/* atanf (0x3f600000) */
	7.1882999962162452690e-01,
/* atanf (0x3f620000) */
	7.2323768457631787410e-01,
/* atanf (0x3f640000) */
	7.2761133262651067621e-01,
/* atanf (0x3f660000) */
	7.3195117111591656567e-01,
/* atanf (0x3f680000) */
	7.3625742898142809700e-01,
/* atanf (0x3f6a0000) */
	7.4053033661269274113e-01,
/* atanf (0x3f6c0000) */
	7.4477012571607514868e-01,
/* atanf (0x3f6e0000) */
	7.4897702918294140062e-01,
/* atanf (0x3f700000) */
	7.5315128096219441378e-01,
/* atanf (0x3f720000) */
	7.5729311593699244476e-01,
/* atanf (0x3f740000) */
	7.6140276980557841657e-01,
/* atanf (0x3f760000) */
	7.6548047896614446195e-01,
/* atanf (0x3f780000) */
	7.6952648040565829746e-01,
/* atanf (0x3f7a0000) */
	7.7354101159257349085e-01,
/* atanf (0x3f7c0000) */
	7.7752431037334779340e-01,
/* atanf (0x3f7e0000) */
	7.8147661487268826885e-01,
/* atanf (0x3f800000) */
	7.8539816339744827900e-01
};

/* *********************************************************** */

#define	PI	3.1415926535897931160e+00
#define	PION2	1.57079632679489655800e+00
#define	KA9	1.00523913835726148843e-01
#define	KA7	-1.42452689179072812273e-01
#define	KA5	1.99994988952939798604e-01
#define	KA3	-3.33333323562239806408e-01
#define	KB3	-3.33327229950751257532e-01

/* *********************************************************** */

mlib_d64
mlib_atan2i(
	mlib_s32 yy,
	mlib_s32 xx)
{
	type_union_mlib_d64 arg1, arg2, darg;
	mlib_d64 y, yv2;
	mlib_s32 ind;

	arg1.db = mlib_fabs((mlib_d64)xx);
	arg2.db = mlib_fabs((mlib_d64)yy);

	if (arg2.db < arg1.db)
		darg.db = arg2.db / arg1.db;
	else {
		if (yy == 0)
			return (0.0);
		darg.db = arg1.db / arg2.db;
	}

	if (darg.db <= 0.25) {
		yv2 = darg.db * darg.db;
		y = darg.db + KA5 * darg.db * yv2 * (KA3 / KA5 + yv2) +
			KA9 * darg.db * yv2 * (KA7 / KA9 + yv2) * (yv2 * yv2);
	} else {
		y = darg.db;
		darg.two_int.int0 &= 0x3fffc000;
		darg.two_int.int1 = 0;
		y = (y - darg.db) / (1.0 + y * darg.db);
		ind = (darg.two_int.int0 - 0x3fd00000) >> 14;
		y = y + mlib_constarctgf[ind] + KB3 * y * (y * y);
	}

	if (arg2.db >= arg1.db)
		y = (xx < 0) ? PION2 + y : PION2 - y;
	else
		y = (xx < 0) ? PI - y : y;
	return ((yy < 0) ? -y : y);
}

/* *********************************************************** */

void
mlib_type_to_D64(
	mlib_s32 type,
	const void *x,
	mlib_s32 n,
	mlib_d64 *dx)
{
	mlib_s32 i;

	switch (type) {
	    case type_U8:
		    for (i = 0; i < n; i++)
			    dx[i] = (mlib_d64)((mlib_u8 *)x)[i];
		    return;
	    case type_S8:
		    for (i = 0; i < n; i++)
			    dx[i] = (mlib_d64)((mlib_s8 *)x)[i];
		    return;
	    case type_S16:
		    for (i = 0; i < n; i++)
			    dx[i] = (mlib_d64)((mlib_s16 *)x)[i];
		    return;
	    case type_S32:
		    for (i = 0; i < n; i++)
			    dx[i] = (mlib_d64)((mlib_s32 *)x)[i];
		    return;
	    case type_U8C:
		    for (i = 0; i < n; i++) {
			    dx[2 * i] = (mlib_d64)((mlib_u8 *)x)[2 * i];
			    dx[2 * i + 1] = (mlib_d64)((mlib_u8 *)x)[2 * i + 1];
		    }

		    return;
	    case type_S8C:
		    for (i = 0; i < n; i++) {
			    dx[2 * i] = (mlib_d64)((mlib_s8 *)x)[2 * i];
			    dx[2 * i + 1] = (mlib_d64)((mlib_s8 *)x)[2 * i + 1];
		    }

		    return;
	    case type_S16C:
		    for (i = 0; i < n; i++) {
			    dx[2 * i] = (mlib_d64)((mlib_s16 *)x)[2 * i];
			    dx[2 * i + 1] =
				    (mlib_d64)((mlib_s16 *)x)[2 * i + 1];
		    }

		    return;
	    case type_S32C:
		    for (i = 0; i < n; i++) {
			    dx[2 * i] = (mlib_d64)((mlib_s32 *)x)[2 * i];
			    dx[2 * i + 1] =
				    (mlib_d64)((mlib_s32 *)x)[2 * i + 1];
		    }
	}
}

/* *********************************************************** */

mlib_s32
mlib_D64_to_S32_Mod(
	mlib_d64 x)
{
	type_union_mlib_d64 dd;
	mlib_s32 exponent;

	dd.db = mlib_fabs(x);

	if (dd.db <= MLIB_S32_MAX)
		return ((mlib_s32)x);
	exponent = 0x433 - (dd.two_int.int0 >> 20);

	if (exponent > 0) {
		dd.two_int.int0 |= 0x100000;

#ifdef _NO_LONGLONG

		dd.two_int.int1 = ((mlib_u32)dd.two_int.int1) >> exponent;
		dd.two_int.int1 |= (dd.two_int.int0) << (32 - exponent);

#else

		dd.ll >>= exponent;

#endif /* _NO_LONGLONG */
	} else
		dd.two_int.int1 <<= -exponent;
	return ((x < 0) ? -dd.two_int.int1 : dd.two_int.int1);
}

/* *********************************************************** */

void
mlib_VectorD64_to_type(
	mlib_s32 type,
	mlib_s32 mode,
	const mlib_d64 *dx,
	mlib_s32 n,
	void *x)
{
	mlib_s32 i;

	switch (type) {
	    case type_U8C:
		    type = type_U8;
		    n += n;
		    break;

	    case type_S8C:
		    type = type_S8;
		    n += n;
		    break;

	    case type_S16C:
		    type = type_S16;
		    n += n;
		    break;

	    case type_S32C:
		    type = type_S32;
		    n += n;
	}

	if (mode == mode_Sat) {
		switch (type) {
		    case type_U8:
			    for (i = 0; i < n; i++) {
				    ((mlib_u8 *)x)[i] =
					    (dx[i] >=
					    MLIB_U8_MAX) ? MLIB_U8_MAX : ((dx[i]
					    < 0) ? 0 : (mlib_u8)dx[i]);
			    }

			    return;

		    case type_S8:
			    for (i = 0; i < n; i++) {
				    ((mlib_s8 *)x)[i] =
					    (mlib_fabs(dx[i]) >
					    MLIB_S8_MAX) ? ((dx[i] >
					    0) ? MLIB_S8_MAX : MLIB_S8_MIN)
					    : (mlib_s8)dx[i];
			    }

			    return;

		    case type_S16:
			    for (i = 0; i < n; i++) {
				    ((mlib_s16 *)x)[i] =
					    (mlib_fabs(dx[i]) >
					    MLIB_S16_MAX) ? ((dx[i] >
					    0) ? MLIB_S16_MAX : MLIB_S16_MIN)
					    : (mlib_s16)dx[i];
			    }

			    return;

		    case type_S32:
			    for (i = 0; i < n; i++) {
				    ((mlib_s32 *)x)[i] =
#ifndef MLIB_USE_FTOI_CLAMPING
					    (mlib_fabs(dx[i]) >
					    MLIB_S32_MAX) ? ((dx[i] >
					    0) ? MLIB_S32_MAX : MLIB_S32_MIN)
					    : (mlib_s32)dx[i];
#else
					    (mlib_s32)dx[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
			    }

			    return;
		}
	} else {
		switch (type) {
		    case type_U8:
			    for (i = 0; i < n; i++) {
				    ((mlib_u8 *)x)[i] =
					    (mlib_s8)mlib_D64_to_S32_Mod(dx[i]);
			    }

			    return;

		    case type_S8:
			    for (i = 0; i < n; i++) {
				    ((mlib_s8 *)x)[i] =
					    (mlib_s8)mlib_D64_to_S32_Mod(dx[i]);
			    }

			    return;

		    case type_S16:
			    for (i = 0; i < n; i++) {
				    ((mlib_s16 *)x)[i] =
					    (mlib_s16)
					    mlib_D64_to_S32_Mod(dx[i]);
			    }

			    return;

		    case type_S32:
			    for (i = 0; i < n; i++) {
				    ((mlib_s32 *)x)[i] =
					    (mlib_s32)
					    mlib_D64_to_S32_Mod(dx[i]);
			    }
		}
	}
}

/* *********************************************************** */

void
mlib_S32_to_type_Sat(
	mlib_s32 in,
	mlib_s32 type_out,
	void *out,
	mlib_s32 index)
{
	switch (type_out) {

	    case type_U8:
	    case type_U8C:
		    in = (in >= MLIB_U8_MAX) ? MLIB_U8_MAX : ((in <
			    0) ? 0 : in);
		    ((mlib_u8 *)out)[index] = in;
		    return;

	    case type_S8:
	    case type_S8C:
		    in = (in >= MLIB_S8_MAX) ? MLIB_S8_MAX : ((in <=
			    MLIB_S8_MIN) ? MLIB_S8_MIN : in);
		    ((mlib_s8 *)out)[index] = in;
		    return;

	    case type_S16:
	    case type_S16C:
		    in = (in >= MLIB_S16_MAX) ? MLIB_S16_MAX : ((in <=
			    MLIB_S16_MIN) ? MLIB_S16_MIN : in);
		    ((mlib_s16 *)out)[index] = in;
		    return;

	    case type_S32:
	    case type_S32C:
		    ((mlib_s32 *)out)[index] = in;
	}
}

/* *********************************************************** */

void
mlib_S32_to_type_Mod(
	mlib_s32 in,
	mlib_s32 type_out,
	void *out,
	mlib_s32 index)
{
	switch (type_out) {

	    case type_U8:
	    case type_U8C:
		    ((mlib_u8 *)out)[index] = in;
		    return;

	    case type_S8:
	    case type_S8C:
		    ((mlib_s8 *)out)[index] = in;
		    return;

	    case type_S16:
	    case type_S16C:
		    ((mlib_s16 *)out)[index] = in;
		    return;

	    case type_S32:
	    case type_S32C:
		    ((mlib_s32 *)out)[index] = in;
	}
}

/* *********************************************************** */

mlib_status
mlib_MatrixMul_type(
	mlib_s32 type_in,
	mlib_s32 type_out,
	mlib_s32 mode,
	const void *x,
	const void *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n,
	mlib_s32 ns,
	void *z)
{
	mlib_s32 i, j, k;

	if (mode == mode_Sat) {

		switch (type_in) {

		    case type_U8:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_u8 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_u8
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Sat(mul, type_out,
						    z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S8:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s8 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_s8
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Sat(mul, type_out,
						    z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s16 *)x)[j *
							    l +
							    k] *
							    (mlib_d64)((mlib_s16
							    *)y)[k * ns + i];
					    }

#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul > MLIB_S32_MAX)
						    mul = MLIB_S32_MAX;
					    else if (mul < MLIB_S32_MIN)
						    mul = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S32:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul = 0, mul_mid =
						    0, mul_lo = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s32 xi =
							    ((mlib_s32 *)x)[j *
							    l + k], yi =
							    ((mlib_s32 *)y)[k *
							    ns + i];
						    mlib_d64 dx_lo =
							    xi & MLIB_U16_MAX;
						    mlib_d64 dx_hi =
							    xi - dx_lo, dy_lo =
							    yi & MLIB_U16_MAX;
						    mlib_d64 dy_hi = yi - dy_lo;

						    mul += dx_hi * dy_hi;
						    mul_lo += dx_lo * dy_lo;
						    mul_mid +=
							    dx_hi * dy_lo +
							    dx_lo * dy_hi;
					    }

					    mul += mul_lo + mul_mid;
#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul > MLIB_S32_MAX)
						    mul = MLIB_S32_MAX;
					    else if (mul < MLIB_S32_MIN)
						    mul = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_U8C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_u8 *px =
							    (mlib_u8 *)x, *py =
							    (mlib_u8 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i)]
							    - px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i)]
							    + px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
					    }

					    mlib_S32_to_type_Sat(mul_r,
						    type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Sat(mul_i,
						    type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S8C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s8 *px =
							    (mlib_s8 *)x, *py =
							    (mlib_s8 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i)]
							    - px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i)]
							    + px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
					    }

					    mlib_S32_to_type_Sat(mul_r,
						    type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Sat(mul_i,
						    type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s16 *px =
							    (mlib_s16 *)x, *py =
							    (mlib_s16 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_d64)py[2 *
							    (k * ns + i)]
							    - px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_d64)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_d64)py[2 *
							    (k * ns + i)]
							    + px[2 * (j * l +
							    k)] *
							    (mlib_d64)py[2 *
							    (k * ns + i) + 1];
					    }

#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul_r > MLIB_S32_MAX)
						    mul_r = MLIB_S32_MAX;
					    else if (mul_r < MLIB_S32_MIN)
						    mul_r = MLIB_S32_MIN;

					    if (mul_i > MLIB_S32_MAX)
						    mul_i = MLIB_S32_MAX;
					    else if (mul_i < MLIB_S32_MIN)
						    mul_i = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_r, type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_i, type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S32C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul_r = 0, mul_r_mid =
						    0, mul_r_lo = 0;
					    mlib_d64 mul_i = 0, mul_i_mid =
						    0, mul_i_lo = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s32 x_r =
							    ((mlib_s32 *)x)[2 *
							    (j * l + k)];
						    mlib_s32 x_i =
							    ((mlib_s32 *)x)[2 *
							    (j * l + k) + 1];
						    mlib_s32 y_r =
							    ((mlib_s32 *)y)[2 *
							    (k * ns + i)];
						    mlib_s32 y_i =
							    ((mlib_s32 *)y)[2 *
							    (k * ns + i) + 1];
						    mlib_d64 dx_r_lo =
							    x_r & MLIB_U16_MAX;
						    mlib_d64 dx_r_hi =
							    x_r - dx_r_lo,
							    dy_r_lo =
							    y_r & MLIB_U16_MAX;
						    mlib_d64 dy_r_hi =
							    y_r - dy_r_lo;
						    mlib_d64 dx_i_lo =
							    x_i & MLIB_U16_MAX;
						    mlib_d64 dx_i_hi =
							    x_i - dx_i_lo,
							    dy_i_lo =
							    y_i & MLIB_U16_MAX;
						    mlib_d64 dy_i_hi =
							    y_i - dy_i_lo;

						    mul_r += dx_r_hi * dy_r_hi -
							    dx_i_hi * dy_i_hi;
						    mul_r_lo +=
							    dx_r_lo * dy_r_lo -
							    dx_i_lo * dy_i_lo;
						    mul_r_mid +=
							    dx_r_hi * dy_r_lo +
							    dx_r_lo * dy_r_hi -
							    (dx_i_hi * dy_i_lo +
							    dx_i_lo * dy_i_hi);
						    mul_i += dx_i_hi * dy_r_hi +
							    dx_r_hi * dy_i_hi;
						    mul_i_lo +=
							    dx_i_lo * dy_r_lo +
							    dx_r_lo * dy_i_lo;
						    mul_i_mid +=
							    dx_i_hi * dy_r_lo +
							    dx_i_lo * dy_r_hi +
							    (dx_r_hi * dy_i_lo +
							    dx_r_lo * dy_i_hi);
					    }

					    mul_r += mul_r_lo + mul_r_mid;
					    mul_i += mul_i_lo + mul_i_mid;
#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul_r > MLIB_S32_MAX)
						    mul_r = MLIB_S32_MAX;
					    else if (mul_r < MLIB_S32_MIN)
						    mul_r = MLIB_S32_MIN;

					    if (mul_i > MLIB_S32_MAX)
						    mul_i = MLIB_S32_MAX;
					    else if (mul_i < MLIB_S32_MIN)
						    mul_i = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_r, type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_i, type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);
		}
	} else {

		switch (type_in) {

		    case type_U8:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_u8 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_u8
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Mod(mul, type_out,
						    z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S8:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s8 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_s8
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Mod(mul, type_out,
						    z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s16 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_s16
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Mod((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S32:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s32 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_s32
							    *)y)[k * ns + i];
					    }

					    mlib_S32_to_type_Mod((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_U8C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_u8 *px =
							    (mlib_u8 *)x, *py =
							    (mlib_u8 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] -
							    px[2 * (j * l + k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] +
							    px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
					    }

					    mlib_S32_to_type_Mod(mul_r,
						    type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Mod(mul_i,
						    type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S8C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s8 *px =
							    (mlib_s8 *)x, *py =
							    (mlib_s8 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] -
							    px[2 * (j * l + k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] +
							    px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
					    }

					    mlib_S32_to_type_Mod(mul_r,
						    type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Mod(mul_i,
						    type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s16 *px =
							    (mlib_s16 *)x, *py =
							    (mlib_s16 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] -
							    px[2 * (j * l + k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + i)] +
							    px[2 * (j * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + i) + 1];
					    }

					    mlib_S32_to_type_Mod((mlib_s32)
						    mul_r, type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Mod((mlib_s32)
						    mul_i, type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);

		    case type_S32C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul_r = 0, mul_i = 0;

					    for (k = 0; k < l; k++) {
						    mlib_s32 *px =
							    (mlib_s32 *)x, *py =
							    (mlib_s32 *)y;

						    mul_r += px[2 * (j * l +
							    k)] * py[2 * (k *
							    ns + i)] -
							    px[2 * (j * l + k) +
							    1] * py[2 *
							    (k * ns +
							    i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] * py[2 *
							    (k * ns +
							    i)] +
							    px[2 * (j * l +
							    k)] * py[2 * (k *
							    ns + i) + 1];
					    }

					    mlib_S32_to_type_Mod((mlib_s32)
						    mul_r, type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Mod((mlib_s32)
						    mul_i, type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_MatrixMulShift_type(
	mlib_s32 type_in,
	mlib_s32 type_out,
	mlib_s32 mode,
	const void *x,
	const void *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n,
	mlib_s32 ns,
	void *z,
	mlib_s32 shift)
{
	mlib_s32 i, j, k;

	if (mode == mode_Sat) {

		switch (type_in) {

		    case type_S16:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul = 0, shift_d64 =
						    1.0 / (1 << shift);

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s16 *)x)[j *
							    l +
							    k] *
							    (mlib_d64)((mlib_s16
							    *)y)[k * ns + i];
					    }

					    mul *= shift_d64;
#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul > MLIB_S32_MAX)
						    mul = MLIB_S32_MAX;
					    else if (mul < MLIB_S32_MIN)
						    mul = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16C:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_d64 mul_r = 0, mul_i =
						    0, shift_d64 =
						    1.0 / (1 << shift);

					    for (k = 0; k < l; k++) {
						    mlib_s16 *px =
							    (mlib_s16 *)x, *py =
							    (mlib_s16 *)y;

						    mul_r += px[2 * (j * l +
							    k)] *
							    (mlib_d64)py[2 *
							    (k * ns + i)]
							    - px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_d64)py[2 *
							    (k * ns + i) + 1];
						    mul_i += px[2 * (j * l +
							    k) +
							    1] *
							    (mlib_d64)py[2 *
							    (k * ns + i)]
							    + px[2 * (j * l +
							    k)] *
							    (mlib_d64)py[2 *
							    (k * ns + i) + 1];
					    }

					    mul_r *= shift_d64;
					    mul_i *= shift_d64;
#ifndef MLIB_USE_FTOI_CLAMPING

					    if (mul_r > MLIB_S32_MAX)
						    mul_r = MLIB_S32_MAX;
					    else if (mul_r < MLIB_S32_MIN)
						    mul_r = MLIB_S32_MIN;

					    if (mul_i > MLIB_S32_MAX)
						    mul_i = MLIB_S32_MAX;
					    else if (mul_i < MLIB_S32_MIN)
						    mul_i = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_r, type_out, z,
						    2 * (j * ns + i));
					    mlib_S32_to_type_Sat((mlib_s32)
						    mul_i, type_out, z,
						    2 * (j * ns + i) + 1);
				    }

			    return (MLIB_SUCCESS);
		}
	} else {

		switch (type_in) {
		    case type_S16:
			    for (j = 0; j < m; j++)
				    for (i = 0; i < n; i++) {
					    mlib_s32 mul = 0;

					    for (k = 0; k < l; k++) {
						    mul += ((mlib_s16 *)x)[j *
							    l +
							    k] *
							    (mlib_s32)((mlib_s16
							    *)y)[k * ns + i];
					    }

					    mul >>= shift;
					    mlib_S32_to_type_Mod((mlib_s32)mul,
						    type_out, z, j * ns + i);
				    }

			    return (MLIB_SUCCESS);

		    case type_S16C:
			    for (i = 0; i < m; i++) {
				    for (j = 0; j < n; j++) {
					    mlib_s32 mul_r = 0, mul_i = 0;
					    const mlib_s16 *px = x, *py = y;

					    for (k = 0; k < l; k++) {
						    mul_r += px[2 * (i * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + j)]
							    - px[2 * (i * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + j) + 1];
						    mul_i += px[2 * (i * l +
							    k) +
							    1] *
							    (mlib_s32)py[2 *
							    (k * ns + j)]
							    + px[2 * (i * l +
							    k)] *
							    (mlib_s32)py[2 *
							    (k * ns + j) + 1];
					    }

					    ((mlib_s16 *)z)[2 * (i * ns + j)] =
						    mul_r >> shift;
					    ((mlib_s16 *)z)[2 * (i * ns + j) +
						    1] = mul_i >> shift;
				    }
			    }

			    return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
