/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CORTEX_HAYES_H
#define CORTEX_HAYES_H

#define CORTEX_HAYES_MIDR					U(0x410FD800)

/*******************************************************************************
 * CPU Extended Control register specific definitions
 ******************************************************************************/
#define CORTEX_HAYES_CPUECTLR_EL1				S3_0_C15_C1_4

/*******************************************************************************
 * CPU Power Control register specific definitions
 ******************************************************************************/
#define CORTEX_HAYES_CPUPWRCTLR_EL1				S3_0_C15_C2_7
#define CORTEX_HAYES_CPUPWRCTLR_EL1_CORE_PWRDN_BIT		U(1)

#endif /* CORTEX_HAYES_H */
