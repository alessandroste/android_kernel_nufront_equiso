#
# Makefile for the linux kernel.
#

obj-y					:= timer-dw.o core.o clock.o prcm.o get_bootargs.o common.o extend.o nusmart-dbg.o

obj-$(CONFIG_MACH_TL7689_PAD_REF)	+= tl7689-pad-ref.o pad_ref_pinmux.o nusmart-pl330.o dma.o soc_power_ctrl.o
#obj-$(CONFIG_MACH_TL7689_PAD_TEST)	+= tl7689-pad-test.o pad_test_pinmux.o nusmart-pl330.o dma.o soc_power_ctrl.o 
#obj-$(CONFIG_MACH_TL7689_PAD_REF)	+= tl7689-pad-ref.o pad_ref_pinmux.o nusmart-pl330.o dma.o soc_power_ctrl.o 
obj-$(CONFIG_SMP)			+= platsmp.o headsmp.o tl7689-save.o suspend.o suspend-macro.o
obj-$(CONFIG_HOTPLUG_CPU)		+= hotplug.o
#obj-$(CONFIG_LOCAL_TIMERS)		+= localtimer.o
obj-$(CONFIG_PM)			+= tl7689-save.o suspend.o suspend-macro.o wake_timer.o
#obj-$(CONFIG_CPU_IDLE)			+= cpuidle.o
obj-$(CONFIG_CPU_FREQ)			+= cpufreq.o efuse.o
obj-$(CONFIG_AUTO_HOTPLUG)		+= cpu-tl7689.o
obj-$(CONFIG_GENERIC_GPIO)		+= gpio.o
