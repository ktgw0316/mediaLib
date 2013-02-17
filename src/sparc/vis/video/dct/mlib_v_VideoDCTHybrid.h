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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_V_VIDEODCTHYBRID_H
#define	_MLIB_V_VIDEODCTHYBRID_H

#pragma ident	"@(#)mlib_v_VideoDCTHybrid.h	9.4	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif
/*
 *
 * Hybrid VIS IDCT with integer tranpose operations.
 *
 */
/*
 *
 * Stores from Int RF implemented using inline templates
 *
 */
#ifdef __sparcv9
extern void intStoreV9_0_0(
	mlib_u64,
	void *);
extern void intStoreV9_0_1(
	mlib_u64,
	void *);
extern void intStoreV9_0_2(
	mlib_u64,
	void *);
extern void intStoreV9_0_3(
	mlib_u64,
	void *);
extern void intStoreV9_1_0(
	mlib_u64,
	void *);
extern void intStoreV9_1_1(
	mlib_u64,
	void *);
extern void intStoreV9_1_2(
	mlib_u64,
	void *);
extern void intStoreV9_1_3(
	mlib_u64,
	void *);
extern void intStoreV9_2_0(
	mlib_u64,
	void *);
extern void intStoreV9_2_1(
	mlib_u64,
	void *);
extern void intStoreV9_2_2(
	mlib_u64,
	void *);
extern void intStoreV9_2_3(
	mlib_u64,
	void *);
extern void intStoreV9_3_0(
	mlib_u64,
	void *);
extern void intStoreV9_3_1(
	mlib_u64,
	void *);
extern void intStoreV9_3_2(
	mlib_u64,
	void *);
extern void intStoreV9_3_3(
	mlib_u64,
	void *);

#else		   /* __sparcv9 */
extern void intStoreV8_0_0(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_0_1(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_0_2(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_0_3(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_1_0(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_1_1(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_1_2(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_1_3(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_2_0(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_2_1(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_2_2(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_2_3(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_3_0(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_3_1(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_3_2(
	mlib_u32,
	mlib_u32,
	void *);
extern void intStoreV8_3_3(
	mlib_u32,
	mlib_u32,
	void *);

#endif /* __sparcv9 */

/*
 *
 * Integer 4x4 Transpose
 *
 */

#define	I_TRANSPOSE0(x0, x1, mask)	(((x1 >> 16) & ~mask) | (x0 & mask))
#define	I_TRANSPOSE1(x0, x1, mask)	(((x0 << 16) & mask) | (x1 & ~mask))

#define	INT_TRANSPOSE(A0, A1, A2, A3, B0, B1, B2, B3)           \
	B0 = I_TRANSPOSE0(A0, A1, MASK);                        \
	B1 = I_TRANSPOSE0(A2, A3, MASK);                        \
	B2 = I_TRANSPOSE1(A0, A1, MASK);                        \
	B3 = I_TRANSPOSE1(A2, A3, MASK);

/*
 *
 * Load data to Int RF
 *
 */
#define	MUL_S16_S32A(dx, dy)                                           \
	{                                                              \
	    mlib_d64 dr, dr1, dr2, dr3;                                \
	                                                               \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr = vis_fpadd32(dr, dr1);                                 \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr1 = vis_fpadd32(dr2, dr3);                               \
	    dx = vis_fpackfix_pair(dr, dr1);			       \
	}

#define	LOAD_INT_INT0_DEQUANT					\
	LOAD_INT_INT_0;						\
	d0 = ((mlib_u64 *)dPtr1)[8];                            \
	MUL_S16_S32A(I0, d0);					\
	d1 = ((mlib_u64 *)dPtr1)[10];                           \
	MUL_S16_S32A(I1, d1);					\
	d2 = ((mlib_u64 *)dPtr1)[12];                           \
	MUL_S16_S32A(I2, d2);					\
	d3 = ((mlib_u64 *)dPtr1)[14];				\
	MUL_S16_S32A(I3, d3);

#define	LOAD_INT_INT1_DEQUANT					\
	LOAD_INT_INT_1;						\
	d4 = ((mlib_u64 *)dPtr1)[9];                            \
	MUL_S16_S32A(I4, d4);					\
	d5 = ((mlib_u64 *)dPtr1)[11];                           \
	MUL_S16_S32A(I5, d5);					\
	d6 = ((mlib_u64 *)dPtr1)[13];                           \
	MUL_S16_S32A(I6, d6);					\
	d7 = ((mlib_u64 *)dPtr1)[15];				\
	MUL_S16_S32A(I7, d7);

#define	LOAD_INT_INT_0                                          \
	I0 = ((mlib_u64 *)dPtr)[8];                             \
	I1 = ((mlib_u64 *)dPtr)[10];                            \
	I2 = ((mlib_u64 *)dPtr)[12];                            \
	I3 = ((mlib_u64 *)dPtr)[14];

#define	LOAD_INT_INT_1                                          \
	I4 = ((mlib_u64 *)dPtr)[9];                             \
	I5 = ((mlib_u64 *)dPtr)[11];                            \
	I6 = ((mlib_u64 *)dPtr)[13];                            \
	I7 = ((mlib_u64 *)dPtr)[15];

#define	LOAD_FP_INT_0                                           \
	I0 = ((mlib_u64 *)int_FP_transfer)[0];                  \
	I1 = ((mlib_u64 *)int_FP_transfer)[2];                  \
	I2 = ((mlib_u64 *)int_FP_transfer)[4];                  \
	I3 = ((mlib_u64 *)int_FP_transfer)[6];                  \
	I4 = ((mlib_u64 *)int_FP_transfer)[8];                  \
	I5 = ((mlib_u64 *)int_FP_transfer)[10];                 \
	I6 = ((mlib_u64 *)int_FP_transfer)[12];                 \
	I7 = ((mlib_u64 *)int_FP_transfer)[14];

/*
 *
 * Load transposed data back into FP RF
 *
 */

#define	LOAD_INT_FP_VAR						\
	I0 = *((mlib_u64 *)&p40);				\
	I4 = *((mlib_u64 *)&p41);				\
	I1 = *((mlib_u64 *)&p50);				\
	I5 = *((mlib_u64 *)&p51);				\
	I2 = *((mlib_u64 *)&p60);				\
	I6 = *((mlib_u64 *)&p61);				\
	I3 = *((mlib_u64 *)&p70);				\
	I7 = *((mlib_u64 *)&p71);

#define	LOAD_INT_FP_0                                           \
	d0 = int_FP_transfer[1];                                \
	d1 = int_FP_transfer[3];                                \
	d2 = int_FP_transfer[5];                                \
	d3 = int_FP_transfer[7];                                \
	d4 = int_FP_transfer[9];                                \
	d5 = int_FP_transfer[11];                               \
	d6 = int_FP_transfer[13];                               \
	d7 = int_FP_transfer[15];

#define	LOAD_INT_FP_1                                           \
	d0 = int_FP_transfer[0];                                \
	d1 = int_FP_transfer[2];                                \
	d2 = int_FP_transfer[4];                                \
	d3 = int_FP_transfer[6];

#define	LOAD_INT_FP_2                                           \
	d0 = int_FP_transfer[1];                                \
	d1 = int_FP_transfer[3];                                \
	d2 = int_FP_transfer[5];                                \
	d3 = int_FP_transfer[7];

/*
 *
 * Store transposed data from Int RF
 *
 */

#ifdef __sparcv9

#define	INT_STORE_0                                             \
	intStoreV9_0_0(T0, (void *)int_FP_transfer);            \
	intStoreV9_0_1(T1, (void *)int_FP_transfer);            \
	intStoreV9_0_2(T2, (void *)int_FP_transfer);            \
	intStoreV9_0_3(T3, (void *)int_FP_transfer);

#define	INT_STORE_1                                             \
	intStoreV9_1_0(T4, (void *)int_FP_transfer);            \
	intStoreV9_1_1(T5, (void *)int_FP_transfer);            \
	intStoreV9_1_2(T6, (void *)int_FP_transfer);            \
	intStoreV9_1_3(T7, (void *)int_FP_transfer);

#define	INT_STORE_2                                             \
	intStoreV9_2_0(T0, (void *)int_FP_transfer);            \
	intStoreV9_2_1(T1, (void *)int_FP_transfer);            \
	intStoreV9_2_2(T2, (void *)int_FP_transfer);            \
	intStoreV9_2_3(T3, (void *)int_FP_transfer);

#define	INT_STORE_3                                             \
	intStoreV9_3_0(T4, (void *)int_FP_transfer);            \
	intStoreV9_3_1(T5, (void *)int_FP_transfer);            \
	intStoreV9_3_2(T6, (void *)int_FP_transfer);            \
	intStoreV9_3_3(T7, (void *)int_FP_transfer);

#else /* __sparcv9 */

#define	INT_STORE_0                                             \
	intStoreV8_0_0((mlib_s32)(T0), (mlib_s32)(T0 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_0_1((mlib_s32)(T1), (mlib_s32)(T1 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_0_2((mlib_s32)(T2), (mlib_s32)(T2 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_0_3((mlib_s32)(T3), (mlib_s32)(T3 >> 32),    \
		(void *)int_FP_transfer);

#define	INT_STORE_1                                             \
	intStoreV8_1_0((mlib_s32)(T4), (mlib_s32)(T4 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_1_1((mlib_s32)(T5), (mlib_s32)(T5 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_1_2((mlib_s32)(T6), (mlib_s32)(T6 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_1_3((mlib_s32)(T7), (mlib_s32)(T7 >> 32),    \
		(void *)int_FP_transfer);

#define	INT_STORE_2                                             \
	intStoreV8_2_0((mlib_s32)(T0), (mlib_s32)(T0 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_2_1((mlib_s32)(T1), (mlib_s32)(T1 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_2_2((mlib_s32)(T2), (mlib_s32)(T2 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_2_3((mlib_s32)(T3), (mlib_s32)(T3 >> 32),    \
		(void *)int_FP_transfer);

#define	INT_STORE_3                                             \
	intStoreV8_3_0((mlib_s32)(T4), (mlib_s32)(T4 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_3_1((mlib_s32)(T5), (mlib_s32)(T5 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_3_2((mlib_s32)(T6), (mlib_s32)(T6 >> 32),    \
		(void *)int_FP_transfer);                       \
	intStoreV8_3_3((mlib_s32)(T7), (mlib_s32)(T7 >> 32),    \
		(void *)int_FP_transfer);

#endif /* __sparcv9 */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_VIDEODCTHYBRID_H */
