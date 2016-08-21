/*
 * arch/arm/mach-tl7689/cpu-tl7689.c
 *
 * CPU auto-hotplug for TL7689 CPUs
 *
 * Copyright (c) 2011, NUFRONT Corporation.
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
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/cpu.h>

#define INITIAL_STATE		HP_DISABLED
#define UP0_DELAY_MS		200
#define DOWN_DELAY_MS		2000

static struct workqueue_struct *hotplug_wq;
static struct delayed_work hotplug_work;
static struct mutex * tl7689_lock;

static unsigned long up_delay;
static unsigned long down_delay;

static unsigned int idle_top_freq;
static unsigned int idle_bottom_freq;

enum {
	HP_DISABLED = 0,
	HP_IDLE,
	HP_DOWN,
	HP_UP,
};
static int hp_state;
static int hp_save;

static void tl7689_auto_hotplug_work_func(struct work_struct *work)
{
	bool up = false;
	bool change = false;

	mutex_lock(tl7689_lock);
	switch (hp_state) {
	case HP_DISABLED:
	case HP_IDLE:
		break;
	case HP_DOWN:
		up = false;
		change = true;
		break;
	case HP_UP:
		up = true;
		change = true;
		break;
	default:
		pr_err("%s: invalid tl7689 hotplug state %d\n",
		       __func__, hp_state);
	}
	mutex_unlock(tl7689_lock);
	/*pr_info("%s:stat %d\n", __func__, hp_state);*/
	if(change) {
		if (up)
			cpu_up(1);
		else
			cpu_down(1);
	}
}

void tl7689_auto_hotplug_governor(unsigned int cpu_freq, bool suspend)
{
	unsigned long top_freq, bottom_freq;

	if (suspend && (hp_state != HP_DISABLED)) {
		hp_state = HP_IDLE;
		return;
	}

	top_freq = idle_top_freq;
	bottom_freq = idle_bottom_freq;

	switch (hp_state) {
	case HP_DISABLED:
		break;
	case HP_IDLE:
		if (cpu_freq >= top_freq) {
			hp_state = HP_UP;
			queue_delayed_work(
				hotplug_wq, &hotplug_work, up_delay);
		} else if (cpu_freq <= bottom_freq) {
			hp_state = HP_DOWN;
			queue_delayed_work(
				hotplug_wq, &hotplug_work, down_delay);
		}
		break;
	case HP_DOWN:
		if (cpu_freq >= top_freq) {
			hp_state = HP_UP;
			queue_delayed_work(
				hotplug_wq, &hotplug_work, up_delay);
		} else if (cpu_freq > bottom_freq) {
			hp_state = HP_IDLE;
		}
		break;
	case HP_UP:
		if (cpu_freq <= bottom_freq) {
			hp_state = HP_DOWN;
			queue_delayed_work(
				hotplug_wq, &hotplug_work, down_delay);
		} else if (cpu_freq < top_freq) {
			hp_state = HP_IDLE;
		}
		break;
	default:
		pr_err("%s: invalid tl7689 hotplug state %d\n",
		       __func__, hp_state);
		BUG();
	}
	/*pr_info("%s:stat %d\n", __func__, hp_state);*/
}

int tl7689_auto_hotplug_init(struct mutex *cpu_lock, unsigned int top, unsigned int bottom)
{
	/*
	 * Not bound to the issuer CPU (=> high-priority), has rescue worker
	 * task, single-threaded, freezable.
	 */
	hotplug_wq = alloc_workqueue(
		"cpu-tl76893", WQ_UNBOUND | WQ_RESCUER | WQ_FREEZABLE, 1);
	if (!hotplug_wq)
		return -ENOMEM;
	INIT_DELAYED_WORK(&hotplug_work, tl7689_auto_hotplug_work_func);

	up_delay = msecs_to_jiffies(UP0_DELAY_MS);
	down_delay = msecs_to_jiffies(DOWN_DELAY_MS);

	idle_top_freq = top;
	idle_bottom_freq = bottom;

	hp_state = INITIAL_STATE;
	hp_save = INITIAL_STATE;
	tl7689_lock = cpu_lock;
	return 0;
}

void tl7689_auto_hotplug_suspend(void)
{
	hp_save = hp_state;
	hp_state = HP_DISABLED;
}

void tl7689_auto_hotplug_resume(void)
{
	hp_state = hp_save;
}

void tl7689_auto_hotplug_enable(bool flag)
{
	hp_state = (flag?HP_IDLE:HP_DISABLED);
}

void tl7689_auto_hotplug_exit(void)
{
	if(hotplug_wq) {
		destroy_workqueue(hotplug_wq);
		hotplug_wq = NULL;
	}
}
