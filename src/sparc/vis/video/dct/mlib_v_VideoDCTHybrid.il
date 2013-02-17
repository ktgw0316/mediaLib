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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_VideoDCTHybrid.il	9.3	07/11/05 SMI"

! Inline templates required for hybrid VIS/Int IDCT
! -------------------------------------------------

! void intStoreV9*(unsigned long long, unsigned long *);
!
        .inline intStoreV9_0_0,12
        st      %o0,[%o1+0x28]
        srlx    %o0,32,%o0
        st      %o0,[%o1+0x8]
        .end

! void intStoreV8*(unsigned long, unsigned long, unsigned long *);
!
        .inline intStoreV8_0_0,12
        st      %o0,[%o2+0x28]
        st      %o1,[%o2+0x8]
        .end
!-----------
        .inline intStoreV9_0_1,12
        st      %o0,[%o1+0x2c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0xc]
        .end

        .inline intStoreV8_0_1,12
        st      %o0,[%o2+0x2c]
        st      %o1,[%o2+0xc]
        .end
!-----------
        .inline intStoreV9_0_2,12
        st      %o0,[%o1+0x38]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x018]
        .end

        .inline intStoreV8_0_2,12
        st      %o0,[%o2+0x38]
        st      %o1,[%o2+0x018]
        .end
!-----------
        .inline intStoreV9_0_3,12
        st      %o0,[%o1+0x3c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x1c]
        .end

        .inline intStoreV8_0_3,12
        st      %o0,[%o2+0x3c]
        st      %o1,[%o2+0x1c]
        .end
!-----------
        .inline intStoreV9_1_0,12
        st      %o0,[%o1+0x68]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x48]
        .end

        .inline intStoreV8_1_0,12
        st      %o0,[%o2+0x68]
        st      %o1,[%o2+0x48]
        .end
!-----------
        .inline intStoreV9_1_1,12
        st      %o0,[%o1+0x6c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x4c]
        .end

        .inline intStoreV8_1_1,12
        st      %o0,[%o2+0x6c]
        st      %o1,[%o2+0x4c]
        .end
!-----------
        .inline intStoreV9_1_2,12
        st      %o0,[%o1+0x78]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x58]
        .end

        .inline intStoreV8_1_2,12
        st      %o0,[%o2+0x78]
        st      %o1,[%o2+0x58]
        .end
!-----------
        .inline intStoreV9_1_3,12
        st      %o0,[%o1+0x7c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x5c]
        .end

        .inline intStoreV8_1_3,12
        st      %o0,[%o2+0x7c]
        st      %o1,[%o2+0x5c]
        .end
!-----------
        .inline intStoreV9_2_0,12
        st      %o0,[%o1+0x20]

        srlx    %o0,32,%o0
        st      %o0,[%o1]
        .end

        .inline intStoreV8_2_0,12
        st      %o0,[%o2+0x20]
        st      %o1,[%o2]
        .end
!-----------
        .inline intStoreV9_2_1,12
        st      %o0,[%o1+0x24]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x4]
        .end

        .inline intStoreV8_2_1,12
        st      %o0,[%o2+0x24]
        st      %o1,[%o2+0x4]
        .end
!-----------
        .inline intStoreV9_2_2,12
        st      %o0,[%o1+0x30]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x10]
        .end

        .inline intStoreV8_2_2,12
        st      %o0,[%o2+0x30]
        st      %o1,[%o2+0x10]
        .end
!-----------
        .inline intStoreV9_2_3,12
        st      %o0,[%o1+0x34]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x14]
        .end

        .inline intStoreV8_2_3,12
        st      %o0,[%o2+0x34]
        st      %o1,[%o2+0x14]
        .end
!-----------
        .inline intStoreV9_3_0,12
        st      %o0,[%o1+0x28]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x8]
        .end

        .inline intStoreV8_3_0,12
        st      %o0,[%o2+0x28]
        st      %o1,[%o2+0x8]
        .end
!-----------
        .inline intStoreV9_3_1,12
        st      %o0,[%o1+0x2c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0xc]
        .end

        .inline intStoreV8_3_1,12
        st      %o0,[%o2+0x2c]
        st      %o1,[%o2+0xc]
        .end
!-----------
        .inline intStoreV9_3_2,12
        st      %o0,[%o1+0x38]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x18]
        .end

        .inline intStoreV8_3_2,12
        st      %o0,[%o2+0x38]
        st      %o1,[%o2+0x18]
        .end
!-----------
        .inline intStoreV9_3_3,12
        st      %o0,[%o1+0x3c]

        srlx    %o0,32,%o0
        st      %o0,[%o1+0x1c]
        .end

        .inline intStoreV8_3_3,12
        st      %o0,[%o2+0x3c]
        st      %o1,[%o2+0x1c]
        .end
!-----------

