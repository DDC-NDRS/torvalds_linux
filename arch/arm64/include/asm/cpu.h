/* SPDX-License-Identifier: GPL-2.0-only */
/*
  * Copyright (C) 2014 ARM Ltd.
 */
#ifndef __ASM_CPU_H
#define __ASM_CPU_H

#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/percpu.h>

/*
 * Records attributes of an individual CPU.
 */
struct cpuinfo_32bit {
	u32		reg_id_dfr0;
	u32		reg_id_dfr1;
	u32		reg_id_isar0;
	u32		reg_id_isar1;
	u32		reg_id_isar2;
	u32		reg_id_isar3;
	u32		reg_id_isar4;
	u32		reg_id_isar5;
	u32		reg_id_isar6;
	u32		reg_id_mmfr0;
	u32		reg_id_mmfr1;
	u32		reg_id_mmfr2;
	u32		reg_id_mmfr3;
	u32		reg_id_mmfr4;
	u32		reg_id_mmfr5;
	u32		reg_id_pfr0;
	u32		reg_id_pfr1;
	u32		reg_id_pfr2;

	u32		reg_mvfr0;
	u32		reg_mvfr1;
	u32		reg_mvfr2;
};

struct cpuinfo_arm64 {
	struct kobject	kobj;
	u64		reg_ctr;
	u64		reg_cntfrq;
	u64		reg_dczid;
	u64		reg_midr;
	u64		reg_revidr;
	u64		reg_aidr;
	u64		reg_gmid;
	u64		reg_smidr;
	u64		reg_mpamidr;

	u64		reg_id_aa64dfr0;
	u64		reg_id_aa64dfr1;
	u64		reg_id_aa64isar0;
	u64		reg_id_aa64isar1;
	u64		reg_id_aa64isar2;
	u64		reg_id_aa64isar3;
	u64		reg_id_aa64mmfr0;
	u64		reg_id_aa64mmfr1;
	u64		reg_id_aa64mmfr2;
	u64		reg_id_aa64mmfr3;
	u64		reg_id_aa64mmfr4;
	u64		reg_id_aa64pfr0;
	u64		reg_id_aa64pfr1;
	u64		reg_id_aa64pfr2;
	u64		reg_id_aa64zfr0;
	u64		reg_id_aa64smfr0;
	u64		reg_id_aa64fpfr0;

	struct cpuinfo_32bit	aarch32;
};

DECLARE_PER_CPU(struct cpuinfo_arm64, cpu_data);

void cpuinfo_store_cpu(void);
void __init cpuinfo_store_boot_cpu(void);

void __init init_cpu_features(struct cpuinfo_arm64 *info);
void update_cpu_features(int cpu, struct cpuinfo_arm64 *info,
				 struct cpuinfo_arm64 *boot);

#endif /* __ASM_CPU_H */
