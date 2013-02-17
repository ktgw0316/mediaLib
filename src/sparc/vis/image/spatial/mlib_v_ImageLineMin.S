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

#pragma ident	"@(#)mlib_v_ImageLineMin.S	9.2	07/11/05 SMI"

		.section        ".text",#alloc,#execinstr

		.register %g2, #scratch
		.register %g3, #scratch

!***************************************************************
!
! void mlib_ImageLineMin(double *dp, double *sp, int n);
!
! DESCRIPTION:
!
! for (i = 0; i < n; i++) {
!   ((short*)dp)[i] = Min(((short*)dp)[i], ((short*)sp)[i]);
! }
!

		.align 32
		.global mlib_ImageLineMin
mlib_ImageLineMin:
		ldd             [%o1], %f10
		sll             %o2, 1, %o2
		ldd             [%o0], %f20
		subcc           %o2, 8*3, %o2
		ldd             [%o1 + 8], %f12
		ldd             [%o0 + 8], %f22
		ble,pn          %icc, mlib_ImageLineMin_tail
		sub             %o0, 8, %o0
mlib_ImageLineMin_loop:
		fcmple16        %f10, %f20, %g1
		ldd             [%o1 + 16], %f14
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f24
		subcc           %o2, 8*3, %o2
		stda            %f10, [%o0] %g1, 0xC2

		fcmple16        %f12, %f22, %g2
		ldd             [%o1 + 16], %f10
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f20
		stda            %f12, [%o0] %g2, 0xC2

		fcmple16        %f14, %f24, %g3
		ldd             [%o1 + 16], %f12
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f22
		bg,pt           %icc, mlib_ImageLineMin_loop
		stda            %f14, [%o0] %g3, 0xC2
mlib_ImageLineMin_tail:
		add             %o2, 8*3 - 2, %o2

		fcmple16        %f10, %f20, %g1
		ldd             [%o1 + 16], %f14
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f24
		edge16          %g0, %o2, %o4
		subcc           %o2, 8, %o2
		and             %g1, %o4, %g1
		bl              %icc, mlib_ImageLineMin_exit
		stda            %f10, [%o0] %g1, 0xC2

		fcmple16        %f12, %f22, %g2
		ldd             [%o1 + 16], %f10
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f20
		edge16          %g0, %o2, %o4
		subcc           %o2, 8, %o2
		and             %g2, %o4, %g2
		bl              %icc, mlib_ImageLineMin_exit
		stda            %f12, [%o0] %g2, 0xC2

		fcmple16        %f14, %f24, %g3
		ldd             [%o1 + 16], %f12
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f22
		edge16          %g0, %o2, %o4
		and             %g3, %o4, %g3
		stda            %f14, [%o0] %g3, 0xC2
mlib_ImageLineMin_exit:
		retl
		nop
		.size mlib_ImageLineMin, (.-mlib_ImageLineMin)

!***************************************************************
!
! void mlib_ImageLineSortMin(double *dp, double *sp, int n);
!
! DESCRIPTION:
!
! for (i = 0; i < n; i++) {
!   ((short*)dp)[i] = Min(((short*)dp)[i], ((short*)sp)[i]);
!   ((short*)sp)[i] = Min(((short*)dp)[i], ((short*)sp)[i]);
! }
!

		.align 32
		.global mlib_ImageLineSortMin
mlib_ImageLineSortMin:
		ldd             [%o1], %f10
		sll             %o2, 1, %o2
		ldd             [%o0], %f20
		subcc           %o2, 8*3, %o2
		ldd             [%o1 + 8], %f12
		ldd             [%o0 + 8], %f22
		sub             %o1, 8, %o1
		ble,pn          %icc, mlib_ImageLineSortMin_tail
		sub             %o0, 8, %o0
mlib_ImageLineSortMin_loop:
		ldd             [%o1 + 24], %f14
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f24
		fcmple16        %f10, %f20, %g1

		stda            %f10, [%o0] %g1, 0xC2
		subcc           %o2, 8*3, %o2
		stda            %f20, [%o1] %g1, 0xC2

		ldd             [%o1 + 24], %f10
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f20
		fcmple16        %f12, %f22, %g2

		stda            %f12, [%o0] %g2, 0xC2
		stda            %f22, [%o1] %g2, 0xC2

		ldd             [%o1 + 24], %f12
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f22
		fcmple16        %f14, %f24, %g3

		stda            %f14, [%o0] %g3, 0xC2
		bg,pt           %icc, mlib_ImageLineSortMin_loop
		stda            %f24, [%o1] %g3, 0xC2
mlib_ImageLineSortMin_tail:
		add             %o2, 8*3 - 2, %o2

		ldd             [%o1 + 24], %f14
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f24
		fcmple16        %f10, %f20, %g1

		edge16          %g0, %o2, %o4
		subcc           %o2, 8, %o2
		and             %g1, %o4, %g1
		stda            %f10, [%o0] %g1, 0xC2
		bl              %icc, mlib_ImageLineSortMin_exit
		stda            %f20, [%o1] %g1, 0xC2

		ldd             [%o1 + 24], %f10
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f20
		fcmple16        %f12, %f22, %g2

		edge16          %g0, %o2, %o4
		subcc           %o2, 8, %o2
		and             %g2, %o4, %g2
		stda            %f12, [%o0] %g2, 0xC2
		bl              %icc, mlib_ImageLineSortMin_exit
		stda            %f22, [%o1] %g2, 0xC2

		ldd             [%o1 + 24], %f12
		add             %o1, 8, %o1
		add             %o0, 8, %o0
		ldd             [%o0 + 16], %f22
		fcmple16        %f14, %f24, %g3

		edge16          %g0, %o2, %o4
		and             %g3, %o4, %g3
		stda            %f14, [%o0] %g3, 0xC2
		stda            %f24, [%o1] %g3, 0xC2
mlib_ImageLineSortMin_exit:
		retl
		nop
		.size mlib_ImageLineSortMin, (.-mlib_ImageLineSortMin)

!***************************************************************
!
! void mlib_ImageLineMin_SrcNa2(double *dp, short *tp, short *sp, int n);
!
! DESCRIPTION:
!
! for (i = 0; i < n; i++) {
!   ((short*)dp)[i] = Min(tp[i], sp[i]);
! }
!
! 'tp' and 'sp' pointers must be same aligned
!

		.align 32
		.global mlib_ImageLineMin_SrcNa2
mlib_ImageLineMin_SrcNa2:
		alignaddr       %o2, %g0, %o2
		sll             %o3, 1, %o3
		or              %g0, 15, %o4
		alignaddr       %o1, %g0, %o1
		subcc           %o3, 8*5, %o3
		ldd             [%o2], %f10
		ldd             [%o1], %f20
		ldd             [%o2 + 8], %f12
		ldd             [%o1 + 8], %f22
		ldd             [%o2 + 16], %f14
		ldd             [%o1 + 16], %f24
		faligndata      %f10, %f12, %f30
		ldd             [%o2 + 24], %f16
		faligndata      %f20, %f22, %f40
		ldd             [%o1 + 24], %f26
		faligndata      %f12, %f14, %f32
		faligndata      %f22, %f24, %f42
		ble,pn          %icc, mlib_ImageLineMin_SrcNa2_tail
		sub             %o0, 8, %o0
mlib_ImageLineMin_SrcNa2_loop:
		fcmple16        %f30, %f40, %g1
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f28
		faligndata      %f24, %f26, %f44
		add             %o1, 8, %o1
		stda            %f40, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f18
		faligndata      %f14, %f16, %f34
		add             %o2, 8, %o2
		subcc           %o3, 8*5, %o3
		stda            %f30, [%o0] %g1, 0xC2

		fcmple16        %f32, %f42, %g2
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f20
		faligndata      %f26, %f28, %f46
		add             %o1, 8, %o1
		stda            %f42, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f10
		faligndata      %f16, %f18, %f36
		add             %o2, 8, %o2
		stda            %f32, [%o0] %g2, 0xC2

		fcmple16        %f34, %f44, %g3
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f22
		faligndata      %f28, %f20, %f48
		add             %o1, 8, %o1
		stda            %f44, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f12
		faligndata      %f18, %f10, %f38
		add             %o2, 8, %o2
		stda            %f34, [%o0] %g3, 0xC2

		fcmple16        %f36, %f46, %g5
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f24
		faligndata      %f20, %f22, %f40
		add             %o1, 8, %o1
		stda            %f46, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f14
		faligndata      %f10, %f12, %f30
		add             %o2, 8, %o2
		stda            %f36, [%o0] %g5, 0xC2

		fcmple16        %f38, %f48, %o5
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f26
		faligndata      %f22, %f24, %f42
		add             %o1, 8, %o1
		stda            %f48, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f16
		faligndata      %f12, %f14, %f32
		add             %o2, 8, %o2
		bg,pt           %icc, mlib_ImageLineMin_SrcNa2_loop
		stda            %f38, [%o0] %o5, 0xC2
mlib_ImageLineMin_SrcNa2_tail:
		add             %o3, 8*5 - 2, %o3

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f30, %f40, %g1
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f28
		faligndata      %f24, %f26, %f44
		add             %o1, 8, %o1
		stda            %f40, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f18
		faligndata      %f14, %f16, %f34
		add             %o2, 8, %o2
		and             %g1, %o4, %g1
		bl              %icc, mlib_ImageLineMin_SrcNa2_exit
		stda            %f30, [%o0] %g1, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f32, %f42, %g2
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f20
		faligndata      %f26, %f28, %f46
		add             %o1, 8, %o1
		stda            %f42, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f10
		faligndata      %f16, %f18, %f36
		add             %o2, 8, %o2
		and             %g2, %o4, %g2
		bl              %icc, mlib_ImageLineMin_SrcNa2_exit
		stda            %f32, [%o0] %g2, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f34, %f44, %g3
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f22
		faligndata      %f28, %f20, %f48
		add             %o1, 8, %o1
		stda            %f44, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f12
		faligndata      %f18, %f10, %f38
		add             %o2, 8, %o2
		and             %g3, %o4, %g3
		bl              %icc, mlib_ImageLineMin_SrcNa2_exit
		stda            %f34, [%o0] %g3, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f36, %f46, %g5
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f24
		faligndata      %f20, %f22, %f40
		add             %o1, 8, %o1
		stda            %f46, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f14
		faligndata      %f10, %f12, %f30
		add             %o2, 8, %o2
		and             %g5, %o4, %g5
		bl              %icc, mlib_ImageLineMin_SrcNa2_exit
		stda            %f36, [%o0] %g5, 0xC2

		edge16          %g0, %o3, %o4
		fcmple16        %f38, %f48, %o5
		add             %o0, 8, %o0
		ldd             [%o1 + 32], %f26
		faligndata      %f22, %f24, %f42
		add             %o1, 8, %o1
		stda            %f48, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f16
		faligndata      %f12, %f14, %f32
		add             %o2, 8, %o2
		and             %o5, %o4, %o5
		stda            %f38, [%o0] %o5, 0xC2
mlib_ImageLineMin_SrcNa2_exit:
		retl
		nop
		.size mlib_ImageLineMin_SrcNa2, (.-mlib_ImageLineMin_SrcNa2)

!***************************************************************
!
! void mlib_ImageLineMin1(double *dp, int n);
!
! DESCRIPTION:
!
! for (i = 0; i < n; i++) {
!   ((short*)dp)[i] = Min(((short*)dp)[i], ((short*)dp)[i + 1]);
! }
!

		.align 32
		.global mlib_ImageLineMin1
mlib_ImageLineMin1:
		ldd             [%o0], %f10
		or              %g0, 2, %o2
		ldd             [%o0 + 8], %f12
		alignaddr       %g0, %o2, %o2
		ldd             [%o0 + 16], %f14
		sll             %o1, 1, %o1
		ldd             [%o0 + 24], %f16
		subcc           %o1, 8*5, %o1
		faligndata      %f10, %f12, %f20
		faligndata      %f12, %f14, %f22
		fcmple16        %f20, %f10, %g1
		ble,pn          %icc, mlib_ImageLineMin1_tail
		sub             %o0, 8, %o0
mlib_ImageLineMin1_loop:
		ldd             [%o0 + 40], %f18
		fcmple16        %f22, %f12, %g2
		faligndata      %f14, %f16, %f24
		add             %o0, 8, %o0
		subcc           %o1, 8*5, %o1
		stda            %f20, [%o0] %g1, 0xC2

		ldd             [%o0 + 40], %f10
		fcmple16        %f24, %f14, %g3
		faligndata      %f16, %f18, %f26
		add             %o0, 8, %o0
		stda            %f22, [%o0] %g2, 0xC2

		ldd             [%o0 + 40], %f12
		fcmple16        %f26, %f16, %g5
		faligndata      %f18, %f10, %f28
		add             %o0, 8, %o0
		stda            %f24, [%o0] %g3, 0xC2

		ldd             [%o0 + 40], %f14
		fcmple16        %f28, %f18, %o5
		faligndata      %f10, %f12, %f20
		add             %o0, 8, %o0
		stda            %f26, [%o0] %g5, 0xC2

		ldd             [%o0 + 40], %f16
		fcmple16        %f20, %f10, %g1
		faligndata      %f12, %f14, %f22
		add             %o0, 8, %o0
		bg,pt           %icc, mlib_ImageLineMin1_loop
		stda            %f28, [%o0] %o5, 0xC2
mlib_ImageLineMin1_tail:
		add             %o1, 8*5 - 2, %o1

		ldd             [%o0 + 40], %f18
		fcmple16        %f22, %f12, %g2
		faligndata      %f14, %f16, %f24
		add             %o0, 8, %o0
		edge16          %g0, %o1, %o3
		subcc           %o1, 8, %o1
		and             %g1, %o3, %g1
		bl              %icc, mlib_ImageLineMin1_exit
		stda            %f20, [%o0] %g1, 0xC2

		ldd             [%o0 + 40], %f10
		fcmple16        %f24, %f14, %g3
		faligndata      %f16, %f18, %f26
		add             %o0, 8, %o0
		edge16          %g0, %o1, %o3
		subcc           %o1, 8, %o1
		and             %g2, %o3, %g2
		bl              %icc, mlib_ImageLineMin1_exit
		stda            %f22, [%o0] %g2, 0xC2

		ldd             [%o0 + 40], %f12
		fcmple16        %f26, %f16, %g5
		faligndata      %f18, %f10, %f28
		add             %o0, 8, %o0
		edge16          %g0, %o1, %o3
		subcc           %o1, 8, %o1
		and             %g3, %o3, %g3
		bl              %icc, mlib_ImageLineMin1_exit
		stda            %f24, [%o0] %g3, 0xC2

		ldd             [%o0 + 40], %f14
		fcmple16        %f28, %f18, %o5
		faligndata      %f10, %f12, %f20
		add             %o0, 8, %o0
		edge16          %g0, %o1, %o3
		subcc           %o1, 8, %o1
		and             %g5, %o3, %g5
		bl              %icc, mlib_ImageLineMin1_exit
		stda            %f26, [%o0] %g5, 0xC2

		ldd             [%o0 + 40], %f16
		fcmple16        %f20, %f10, %g1
		faligndata      %f12, %f14, %f22
		add             %o0, 8, %o0
		edge16          %g0, %o1, %o3
		and             %o5, %o3, %o5
		stda            %f28, [%o0] %o5, 0xC2
mlib_ImageLineMin1_exit:
		retl
		nop
		.size mlib_ImageLineMin1, (.-mlib_ImageLineMin1)

!***************************************************************
!
! void mlib_ImageLineMin_SrcNa(double *dp, double *tp, short *sp, int n);
!
! DESCRIPTION:
!
! for (i = 0; i < n; i++) {
!   ((short*)dp)[i] = Min(((short*)tp)[i], sp[i]);
! }
!

		.align 32
		.global mlib_ImageLineMin_SrcNa
mlib_ImageLineMin_SrcNa:
		alignaddr       %o2, %g0, %o2
		sll             %o3, 1, %o3
		ldd             [%o2], %f10
		or              %g0, 15, %o4
		subcc           %o3, 8*5, %o3
		ldd             [%o1], %f20
		ldd             [%o2 + 8], %f12
		ldd             [%o1 + 8], %f22
		ldd             [%o2 + 16], %f14
		ldd             [%o1 + 16], %f24
		ldd             [%o2 + 24], %f16
		faligndata      %f10, %f12, %f30
		ldd             [%o1 + 24], %f26
		faligndata      %f12, %f14, %f32
		ble,pn          %icc, mlib_ImageLineMin_SrcNa_tail
		sub             %o0, 8, %o0
mlib_ImageLineMin_SrcNa_loop:
		fcmple16        %f30, %f20, %g1
		ldd             [%o1 + 32], %f28
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f20, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f18
		faligndata      %f14, %f16, %f34
		add             %o2, 8, %o2
		subcc           %o3, 8*5, %o3
		stda            %f30, [%o0] %g1, 0xC2

		fcmple16        %f32, %f22, %g2
		ldd             [%o1 + 32], %f20
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f22, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f10
		faligndata      %f16, %f18, %f36
		add             %o2, 8, %o2
		stda            %f32, [%o0] %g2, 0xC2

		fcmple16        %f34, %f24, %g3
		ldd             [%o1 + 32], %f22
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f24, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f12
		faligndata      %f18, %f10, %f38
		add             %o2, 8, %o2
		stda            %f34, [%o0] %g3, 0xC2

		fcmple16        %f36, %f26, %g5
		ldd             [%o1 + 32], %f24
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f26, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f14
		faligndata      %f10, %f12, %f30
		add             %o2, 8, %o2
		stda            %f36, [%o0] %g5, 0xC2

		fcmple16        %f38, %f28, %o5
		ldd             [%o1 + 32], %f26
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f28, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f16
		faligndata      %f12, %f14, %f32
		add             %o2, 8, %o2
		bg,pt           %icc, mlib_ImageLineMin_SrcNa_loop
		stda            %f38, [%o0] %o5, 0xC2
mlib_ImageLineMin_SrcNa_tail:
		add             %o3, 8*5 - 2, %o3

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f30, %f20, %g1
		ldd             [%o1 + 32], %f28
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f20, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f18
		faligndata      %f14, %f16, %f34
		add             %o2, 8, %o2
		and             %g1, %o4, %g1
		bl              %icc, mlib_ImageLineMin_SrcNa_exit
		stda            %f30, [%o0] %g1, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f32, %f22, %g2
		ldd             [%o1 + 32], %f20
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f22, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f10
		faligndata      %f16, %f18, %f36
		add             %o2, 8, %o2
		and             %g2, %o4, %g2
		bl              %icc, mlib_ImageLineMin_SrcNa_exit
		stda            %f32, [%o0] %g2, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f34, %f24, %g3
		ldd             [%o1 + 32], %f22
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f24, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f12
		faligndata      %f18, %f10, %f38
		add             %o2, 8, %o2
		and             %g3, %o4, %g3
		bl              %icc, mlib_ImageLineMin_SrcNa_exit
		stda            %f34, [%o0] %g3, 0xC2

		edge16          %g0, %o3, %o4
		subcc           %o3, 8, %o3
		fcmple16        %f36, %f26, %g5
		ldd             [%o1 + 32], %f24
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f26, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f14
		faligndata      %f10, %f12, %f30
		add             %o2, 8, %o2
		and             %g5, %o4, %g5
		bl              %icc, mlib_ImageLineMin_SrcNa_exit
		stda            %f36, [%o0] %g5, 0xC2

		edge16          %g0, %o3, %o4
		fcmple16        %f38, %f28, %o5
		ldd             [%o1 + 32], %f26
		add             %o0, 8, %o0
		add             %o1, 8, %o1
		stda            %f28, [%o0] %o4, 0xC2
		ldd             [%o2 + 32], %f16
		faligndata      %f12, %f14, %f32
		add             %o2, 8, %o2
		and             %o5, %o4, %o5
		stda            %f38, [%o0] %o5, 0xC2
mlib_ImageLineMin_SrcNa_exit:
		retl
		nop
		.size mlib_ImageLineMin_SrcNa, (.-mlib_ImageLineMin_SrcNa)

!***************************************************************
