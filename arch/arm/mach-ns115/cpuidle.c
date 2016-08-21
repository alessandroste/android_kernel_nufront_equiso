/*
 * arch/arm/mach-ns115/cpuidle.c
 *
 * CPU idle driver for NS115 CPUs
 *
 * Copyright (c) 2010, NUFRONT Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/sched.h>
#include <linux/cpuidle.h>
#include <linux/hrtimer.h>
#include <linux/cpu.h>
#include <linux/io.h>
#include <linux/tick.h>
#include <linux/interrupt.h>
#include <linux/suspend.h>
#include <asm/proc-fns.h>

static int ns115_idle_enter_wfi(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index);

struct cpuidle_driver ns115_idle_driver = {
	.name = "ns115_idle",
	.owner = THIS_MODULE,
	.state_count = 1,
	.states = {
		[0] = {
			.enter			= ns115_idle_enter_wfi,
			.exit_latency		= 10,
			.target_residency	= 0,
			.power_usage		= 600,
			.flags			= CPUIDLE_FLAG_TIME_VALID,
			.name			= "LP_WFI",
			.desc			= "CPU wfi",
		},
	},
};

static DEFINE_PER_CPU(struct cpuidle_device, ns115_idle_device);

static int ns115_idle_enter_wfi(struct cpuidle_device *dev,
	struct cpuidle_driver *drv, int index)
{
	ktime_t enter, exit;
	s64 us;

	local_irq_disable();
	local_fiq_disable();

	enter = ktime_get();

	cpu_do_idle();

	exit = ktime_get();
	enter = ktime_sub(exit, enter);
	us = ktime_to_us(enter);

	local_fiq_enable();
	local_irq_enable();

	dev->last_residency = us;

	return index;
}

static int __init ns115_cpuidle_init(void)
{
	int ret;
	unsigned int cpu;
	struct cpuidle_device *dev;
	struct cpuidle_driver *drv = &ns115_idle_driver;

	ret = cpuidle_register_driver(&ns115_idle_driver);
	if (ret) {
		pr_err("CPUidle driver registration failed\n");
		return ret;
	}

	for_each_possible_cpu(cpu) {
		dev = &per_cpu(ns115_idle_device, cpu);
		dev->cpu = cpu;

		dev->state_count = drv->state_count;
		ret = cpuidle_register_device(dev);
		if (ret) {
			pr_err("CPU%u: CPUidle device registration failed\n",
				cpu);
			return ret;
		}
	}
	return 0;
}
device_initcall(ns115_cpuidle_init);
