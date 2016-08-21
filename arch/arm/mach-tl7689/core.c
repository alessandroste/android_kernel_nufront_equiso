/*
 * linux/arch/arm/mach-tl7689/core.c
 *
 * Copyright (C) 2010 NUFRONT Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
//#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/cnt32_to_63.h>
#include <linux/io.h>
#include <linux/smsc911x.h>
#include <linux/ata_platform.h>
#include <linux/dw_dmac.h>
#include <linux/amba/mmci.h>
#include <linux/amba/pl330.h>
#include <linux/rfkill.h>
#include <linux/rfkill-gpio.h>
#ifdef CONFIG_MFD_RICOH583
#include <linux/mfd/ricoh583.h>
#endif
#include <linux/of_address.h>
//#include <linux/power/power_switch.h>

#include <asm/clkdev.h>
#include <asm/system.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/smp_twd.h>
#include <asm/mach-types.h>
#include <asm/hardware/arm_timer.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/mach/time.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include <asm/hardware/gic.h>

#include <mach/platform.h>
#include <mach/irqs.h>
#include <mach/mmc.h>
#include <mach/i2c.h>
#include <mach/board-tl7689.h>
#include <mach/camera.h>
#include <mach/system.h>

#ifdef CONFIG_GENERIC_GPIO
#include <mach/gpio.h>
#endif

#include <asm/pmu.h>

#include <linux/serial_8250.h>
#include <linux/serial_reg.h>

#include <linux/mmc/dw_mmc.h>

//#include <linux/android_pmem.h>
#include <mach/memory.h>
#include "core.h"
#include "prcm.h"
#include <mach/get_bootargs.h>
#include <linux/input.h>
#include <linux/nusmart_gpio_keys.h>
#include <asm/cacheflush.h>

/* used by entry-macro.S and platsmp.c */
//void __iomem *gic_cpu_base_addr;
void (*__tl7689_reset)(char mode);

struct tl7689_backlight_config {
	int default_intensity;
	int (*set_power)(struct device *dev, int state);
};

struct timed_gpio {
	const char	*name;
	unsigned	gpio;
	int		max_timeout;
	u8		active_low;
};

struct timed_gpio_platform_data {
	int			num_gpios;
	struct timed_gpio	*gpios;
};

/*
 * Adjust the zones if there are restrictions for DMA access.
 */
#if 0
#ifdef CONFIG_ZONE_DMA
void __init tl7689_adjust_zones(int node, unsigned long *size,
		unsigned long *hole)
{
	unsigned long dma_size = SZ_256M >> PAGE_SHIFT;
#ifdef CONFIG_MACH_TL7689_PAD_TEST
	if (!machine_is_tl7689_pad_test() || node || (size[0] <= dma_size))
#endif
		return;

	size[ZONE_NORMAL] = size[0] - dma_size;
	size[ZONE_DMA] = dma_size;
	hole[ZONE_NORMAL] = hole[0];
	hole[ZONE_DMA] = 0;
}
#endif
#endif

/*
 *  cpufreq configure for each board.
 */

struct tl7689_cpufreq_config* tl7689_cpufreq_cfg = NULL;

/*
 *tl7689 io table
 */
struct map_desc tl7689_io_desc[] __initdata = {
	{
		.virtual	= IO_ADDRESS(TL7689_GIC_CPU_BASE),
		.pfn		= __phys_to_pfn(TL7689_GIC_CPU_BASE),
		.length		= SZ_256,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_GIC_DIST_BASE),
		.pfn		= __phys_to_pfn(TL7689_GIC_DIST_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_L220_BASE),
		.pfn		= __phys_to_pfn(TL7689_L220_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_PRCM_BASE),
		.pfn		= __phys_to_pfn(TL7689_PRCM_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_SCM_BASE),
		.pfn		= __phys_to_pfn(TL7689_SCM_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_SARRAM_BASE),
		.pfn		= __phys_to_pfn(TL7689_SARRAM_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_CAMERIC_BASE),
		.pfn		= __phys_to_pfn(TL7689_CAMERIC_BASE),
		.length		= SZ_128K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_DISPCTR0_BASE),
		.pfn		= __phys_to_pfn(TL7689_DISPCTR0_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_DISPCTR1_BASE),
		.pfn		= __phys_to_pfn(TL7689_DISPCTR1_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_GPIO0_BASE),
		.pfn		= __phys_to_pfn(TL7689_GPIO0_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_TIMER0_BASE),
		.pfn		= __phys_to_pfn(TL7689_TIMER0_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_TIMER1_BASE),
		.pfn		= __phys_to_pfn(TL7689_TIMER1_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_GPIO1_BASE),
		.pfn		= __phys_to_pfn(TL7689_GPIO1_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_AUX_BASE),
		.pfn		= __phys_to_pfn(TL7689_AUX_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},

	{
		.virtual	= IO_ADDRESS(TL7689_I2S0_BASE),
		.pfn		= __phys_to_pfn(TL7689_I2S0_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_UART0_BASE),
		.pfn		= __phys_to_pfn(TL7689_UART0_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_UART1_BASE),
		.pfn		= __phys_to_pfn(TL7689_UART1_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_UART2_BASE),
		.pfn		= __phys_to_pfn(TL7689_UART2_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_UART3_BASE),
		.pfn		= __phys_to_pfn(TL7689_UART3_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_DMA_330S_BASE),
		.pfn		= __phys_to_pfn(TL7689_DMA_330S_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_I2C0_BASE),
		.pfn		= __phys_to_pfn(TL7689_I2C0_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_I2C1_BASE),
		.pfn		= __phys_to_pfn(TL7689_I2C1_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_I2C2_BASE),
		.pfn		= __phys_to_pfn(TL7689_I2C2_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_I2C3_BASE),
		.pfn		= __phys_to_pfn(TL7689_I2C3_BASE),
		.length		= SZ_64K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_DRAMCTRL_BASE),
		.pfn		= __phys_to_pfn(TL7689_DRAMCTRL_BASE),
		.length		= SZ_4K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_MEM_MAX0_BASE),
		.pfn		= __phys_to_pfn(TL7689_MEM_MAX0_BASE),
		.length		= SZ_1K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_L3_ISS_TA_BASE),
		.pfn		= __phys_to_pfn(TL7689_L3_ISS_TA_BASE),
		.length		= SZ_1K,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= IO_ADDRESS(TL7689_CAMERIC_TA_BASE),
		.pfn		= __phys_to_pfn(TL7689_CAMERIC_TA_BASE),
		.length		= SZ_1K,
		.type		= MT_DEVICE,
	},
};

void __init tl7689_map_io(void)
{
	iotable_init(tl7689_io_desc, ARRAY_SIZE(tl7689_io_desc));
}

static int tl7689_set_wake(struct irq_data *d, unsigned int on)
{
	void __iomem * wake_addr = __io_address(TL7689_PRCM_BASE + 0x14);
	unsigned int val = 0;

	if (d->irq < GPIO_IRQ_BASE1 && d->irq >= (GPIO_IRQ_BASE0 + INT_NUM0)) {
		val = readl(wake_addr);
		if (on) {
			val |= (1 << (d->irq - GPIO_IRQ_BASE0));
			val |= (1 << 16);
		} else {
			val &= ~(1 << (d->irq - GPIO_IRQ_BASE0));
		}
		writel(val, wake_addr);
	} else {
		return -ENXIO;
	}

	return 0;
}

void __init gic_init_irq(void)
{
	gic_arch_extn.irq_set_wake = tl7689_set_wake;
	gic_init(0, 29, __io_address(TL7689_GIC_DIST_BASE),
			__io_address(TL7689_GIC_CPU_BASE));
}

void __init of_gic_init_irq(struct of_device_id *match)
{
	struct device_node *node = NULL;

	gic_arch_extn.irq_set_wake = tl7689_set_wake;

	node = of_find_matching_node_by_address(NULL, match, TL7689_GIC_DIST_BASE);
	of_gic_init(0, 29, __io_address(TL7689_GIC_DIST_BASE),
		__io_address(TL7689_GIC_CPU_BASE), node);
}

DEFINE_TWD_LOCAL_TIMER(tl7689_tlt, TL7689_TWD_BASE, IRQ_LOCALTIMER)

static void __init tl7689_all_timer_init(void)
{

#ifdef CONFIG_LOCAL_TIMERS
	twd_local_timer_register(&tl7689_tlt);
#endif

	tl7689_timer_init();
}

struct sys_timer tl7689_timer = {
	.init = tl7689_all_timer_init,
};
#if 0
static void tl7689_reset(char mode)
{
	prcm_glb_soft_reset();
}
#endif
static void tl7689_power_off(void)
{
#ifdef CONFIG_MFD_RICOH583
	ricoh583_power_off();
#endif
}

/*
 * When call cpu_v7_proc_fin, cpu can hung-up and system restart fail!!!
 * Root cause is unknown!!! <2.6.33 not exist this bug>
 */
static void tl7689_restart(char mode, const char *cmd)
{
	unsigned long flags;
	extern void setup_mm_for_reboot(char mode);

	local_irq_save(flags);
	__cpuc_flush_kern_all();
	__cpuc_flush_user_all();

	//cpu_v7_proc_fin();

	setup_mm_for_reboot(mode);
	arch_reset(mode, cmd);

}

void arch_reset(char mode, const char *cmd)
{
#ifdef CONFIG_MFD_RICOH583
	ricoh583_restart(mode, cmd);
#else
	if (__tl7689_reset)
		__tl7689_reset(mode);
#endif
}

#ifdef CONFIG_CACHE_L2X0
int __init tl7689_l2x0_init(void)
{
	void __iomem *l2x0_base = __io_address(TL7689_L220_BASE);
	unsigned int val;

	if (!(readl(l2x0_base + L2X0_CTRL) & 1)) {
		/* set RAM latencies to 3 cycle for eASIC */
		writel(0x777, l2x0_base + L2X0_TAG_LATENCY_CTRL);
		writel(0x777, l2x0_base + L2X0_DATA_LATENCY_CTRL);
		//writel(0x00000003, l2x0_base + L2X0_PREFETCH_CTRL);
		writel(0x70000007, l2x0_base + L2X0_PREFETCH_CTRL);
		writel(0x00000001, l2x0_base + L2X0_ADDR_FILTER_START);
		writel(0x10000000, l2x0_base + L2X0_ADDR_FILTER_END);
	}

	/* 64KB way size, 16-way associativity, parity enable, event monitor bus enable,
	   full line of zero enable, shared attribute internally ignored
	   early BRESP enable, data prefetch & instruction prefetch enable,
	 */

	/* Those features are not clear in linux: value = 0x0c771000
	   non-secure interrupt access control, non-secure lockdown enable
	 */

	val = 0x7C460001;
	l2x0_init(l2x0_base, val, 0xc200ffff);

	return 0;
}

early_initcall(tl7689_l2x0_init);
#endif


/************************************************************
 *	Bus priority-level set
 ************************************************************/
static DEFINE_SPINLOCK(prio_lock);

static inline void bus_prio_set(unsigned int offs, unsigned int val_l, unsigned int val_h)
{
	unsigned long flag;
	void * __iomem addr = __io_address(TL7689_MEM_MAX0_BASE);

	spin_lock_irqsave(&prio_lock, flag);
	writel(val_l, addr + offs);
	writel(val_h, addr + offs + 4);
	spin_unlock_irqrestore(&prio_lock, flag);
}

static inline void bus_prio_clear(unsigned int offs)
{
	unsigned long flag;
	void * __iomem addr = __io_address(TL7689_MEM_MAX0_BASE);

	spin_lock_irqsave(&prio_lock, flag);
	writel(0, addr + offs);
	spin_unlock_irqrestore(&prio_lock, flag);
}

#define THREAD_4_OFFSET		0x60
#define WORDS_PER_PERIOD_BIT	16

static void cam_priority_set(int flag)
{
	unsigned int val_l, val_h;
	val_l = 0x2;
	val_h = (0x100 << WORDS_PER_PERIOD_BIT) | 0x4;

	if (flag)
		bus_prio_set(THREAD_4_OFFSET, val_l, val_h);
	else
		bus_prio_clear(THREAD_4_OFFSET);
}

/*
static struct resource pmu_resources[] = {
	[0] = {
		.start	= IRQ_TL7689_OSPREY_PMUIRQ,
		.end	= IRQ_TL7689_OSPREY_PMUIRQ,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_TL7689_OSPREY_PMUIRQ1,
		.end	= IRQ_TL7689_OSPREY_PMUIRQ1,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.num_resources	= ARRAY_SIZE(pmu_resources),
	.resource	= pmu_resources,
};

static struct resource pmu_resources[] = {
	[0] = {
		.start	= IRQ_TL7689_OSPREY_PMUIRQ,
		.end	= IRQ_TL7689_OSPREY_PMUIRQ,
		.flags	= IORESOURCE_IRQ,
	},
	[1] = {
		.start	= IRQ_TL7689_OSPREY_PMUIRQ1,
		.end	= IRQ_TL7689_OSPREY_PMUIRQ1,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.num_resources	= ARRAY_SIZE(pmu_resources),
	.resource	= pmu_resources,
};
*/

/*##############################################################*/
/*		TL7689 SD/MMC controller device			*/
/*##############################################################*/


static struct resource tl7689_sdmmc_resource[] = {
	[0] = {
		.start	= TL7689_SDMMC0_BASE,
		.end	= TL7689_SDMMC0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_SDMMC0_INTR,
		.end	= IRQ_TL7689_SDMMC0_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_sdmmc_device = {
	.name		= "tl7689-sdmmc",
	.id		= 0,
	.num_resources	= 2,
	.resource	= tl7689_sdmmc_resource,
	.dev		= {
		.platform_data = NULL,//&nusmart_sdmmc_data,
	},
};

/*##############################################################*/
/*		TL7689 I2C controller device			*/
/*##############################################################*/
static struct resource tl7689_i2c0_resource[] = {
	[0] = {
		.start	= TL7689_I2C0_BASE,
		.end	= TL7689_I2C0_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_I2C0_INTR,
		.end	= IRQ_TL7689_I2C0_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};
static struct resource tl7689_i2c1_resource[] = {
	[0] = {
		.start	= TL7689_I2C1_BASE,
		.end	= TL7689_I2C1_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_I2C1_INTR,
		.end	= IRQ_TL7689_I2C1_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};
static struct resource tl7689_i2c2_resource[] = {
	[0] = {
		.start	= TL7689_I2C2_BASE,
		.end	= TL7689_I2C2_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_I2C2_INTR,
		.end	= IRQ_TL7689_I2C2_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};
static struct resource tl7689_i2c3_resource[] = {
	[0] = {
		.start	= TL7689_I2C3_BASE,
		.end	= TL7689_I2C3_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_I2C3_INTR,
		.end	= IRQ_TL7689_I2C3_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};

struct nusmart_i2c_platform_data nusmart_i2c_data[] = {
	{
		.speed	= I2C_SPEED_STD,
		.clk_id	= "tl7689_i2c0",
	},
	{
		.speed	= I2C_SPEED_STD,
		.clk_id	= "tl7689_i2c1",
	},
	{
		.speed	= I2C_SPEED_STD,
		.clk_id	= "tl7689_i2c2",
	},
#ifdef TOUCHSCREEN_I2C_BUS_FAST_MODE
	{
		.speed	= I2C_SPEED_FAST,
		.clk_id	= "tl7689_i2c3",
	},
#else
	{
		.speed	= I2C_SPEED_STD,
		.clk_id	= "tl7689_i2c3",
	},
#endif
};

struct platform_device tl7689_i2c_device[] = {
	{
		.name		= "i2c_designware",
		.id		= I2C_BUS_0,
		.num_resources	= 2,
		.resource	= tl7689_i2c0_resource,
		.dev		= {
			.platform_data = &nusmart_i2c_data[0],
		},
	},
	{
		.name		= "i2c_designware",
		.id		= I2C_BUS_1,
		.num_resources	= 2,
		.resource	= tl7689_i2c1_resource,
		.dev		= {
			.platform_data = &nusmart_i2c_data[1],
		},
	},
	{
		.name		= "i2c_designware",
		.id		= I2C_BUS_2,
		.num_resources	= 2,
		.resource	= tl7689_i2c2_resource,
		.dev		= {
			.platform_data = &nusmart_i2c_data[2],
		},
	},
	{
		.name		= "i2c_designware",
		.id		= I2C_BUS_3,
		.num_resources	= 2,
		.resource	= tl7689_i2c3_resource,
		.dev		= {
			.platform_data = &nusmart_i2c_data[3],
		},
	},
};


/*##############################################################*/
/*		TL7689 usb device controller device		*/
/*##############################################################*/
static struct resource tl7689_udc_resource[] = {
	[0] = {
		.start	= TL7689_USB_OTG_BASE,
		.end	= TL7689_USB_OTG_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_USB_OTG,
		.end	= IRQ_TL7689_USB_OTG,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_udc_device = {
	.name		= "dwc_otg",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(tl7689_udc_resource),
	.resource	= tl7689_udc_resource,
};

/*##############################################################*/
/*		TL7689 uart controller device			*/
/*##############################################################*/
static struct plat_serial8250_port serial_platform_data[] = {
	{
		.mapbase	= TL7689_UART0_BASE,
		.irq		= IRQ_TL7689_UART0_INTR,
		.flags		= UPF_FIXED_TYPE | UPF_IOREMAP | UPF_SKIP_TEST,
		.type		= PORT_16550A,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= 66000000,
	},{
		.mapbase	= TL7689_UART1_BASE,
		.irq		= IRQ_TL7689_UART1_INTR,
		.flags		= UPF_FIXED_TYPE | UPF_IOREMAP | UPF_SKIP_TEST,
		.type		= PORT_16550A,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= 66000000,
	},/*{
		.mapbase	= TL7689_UART2_BASE,
		.irq		= IRQ_TL7689_UART2_INTR,
		.flags		= UPF_FIXED_TYPE | UPF_IOREMAP | UPF_SKIP_TEST,
		.type		= PORT_NF_16550A,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= 66000000,
	},{
		.mapbase	= TL7689_UART3_BASE,
		.irq		= IRQ_TL7689_UART3_INTR,
		.flags		= UPF_FIXED_TYPE | UPF_IOREMAP | UPF_SKIP_TEST,
		.type		= PORT_NF_16550A,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= 66000000,
	},*/{
		.flags		= 0,
	},
};

struct platform_device tl7689_serial_device = {
	.name	= "serial8250",
	.id	= PLAT8250_DEV_PLATFORM,
	.dev	= {
		.platform_data = serial_platform_data,
	},
};


/*##############################################################*/
/*		TL7689 i2s controller device			*/
/*##############################################################*/

static struct resource tl7689_i2s0_resource[] = {
	[0] = {
		.start	= TL7689_I2S0_BASE,
		.end	= TL7689_I2S0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_I2S0_INTR,
		.end	= IRQ_TL7689_I2S0_INTR,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_i2s_device[] = {
	{
		.name		= "tl7689-i2s",
		.id		= -1,
		.num_resources	= 2,
		.resource	= tl7689_i2s0_resource,
	},
};

struct platform_device tl7689_i2s_plat_device = {
	.name		= "tl7689-i2s-plat",
	.id		= -1,
	.num_resources	= 2,
	.resource	= tl7689_i2s0_resource,
};

struct platform_device tl7689_pcm_device = {
	.name		= "nusmart-pcm-audio",
	.id		= -1,
	.num_resources	= 0,
	.resource	= NULL,
};

/*##############################################################*/
/*		TL7689 usb controller device			*/
/*##############################################################*/

/*
 * USB ehci register define
 */

static struct resource tl7689_ehci_resources[] = {
	[0] = {
		.start	= TL7689_USB_EHCI_BASE,
		.end	= TL7689_USB_EHCI_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_USB_EHCI,
		.end	= IRQ_TL7689_USB_EHCI,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_usb_ehci_device = {
	.name		= "tl7689-ehci",
	.num_resources	= 2,
	.resource	= tl7689_ehci_resources,
	.dev		= {
		.coherent_dma_mask = ~0,
	},
};


/*
 * USB ohci register define
 */

static struct resource tl7689_ohci_resources[] = {
	[0] = {
		.start	= TL7689_USB_OHCI_BASE,
		.end	= TL7689_USB_OHCI_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_USB_OHCI,
		.end	= IRQ_TL7689_USB_OHCI,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_usb_ohci_device = {
	.name		= "tl7689-ohci",
	.num_resources	= 2,
	.resource	= tl7689_ohci_resources,
	.dev		= {
		.coherent_dma_mask = ~0,
	},
};

/*##############################################################*/
/*		TL7689 lcd controller device			*/
/*##############################################################*/
static struct resource tl7689fb0_resources[] = {
	[0] = {
		.start	= TL7689_DISPCTR0_BASE,
		.end	= TL7689_DISPCTR0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_DISPCTR0_ERR,
		.end	= IRQ_TL7689_DISPCTR0_HINT,
		.flags	= IORESOURCE_IRQ,
	},
};
static struct resource tl7689fb1_resources[] = {
	[0] = {
		.start	= TL7689_DISPCTR1_BASE,
		.end	= TL7689_DISPCTR1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_DISPCTR1_ERR,
		.end	= IRQ_TL7689_DISPCTR1_HINT,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_clcd_device[] = {
	{
		.name = "nusmartfb",
		.id = 0,
		.dev = {
			.coherent_dma_mask = DMA_BIT_MASK(32),
		},
		.resource = tl7689fb0_resources,
		.num_resources =2,
	},
	{
		.name = "nusmartfb1",
		.id = 1,
		.dev = {
			.coherent_dma_mask = DMA_BIT_MASK(32),
		},
		.resource = tl7689fb1_resources,
		.num_resources =2,
	},
};

/*##############################################################*/
/*		TL7689 wakeup timer device			*/
/*##############################################################*/
#define WAKEUP_TIMER_NAME "tl7689_wk_timer"
static struct resource tl7689_wk_timer_resources[] = {
	[0] = {
		.start	= TL7689_TIMER0_BASE,
		.end	= TL7689_TIMER0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_TIMER0_WAKEUP_0,
		.end	= IRQ_TL7689_TIMER0_WAKEUP_1,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_wk_timer_device = {
	.name		= WAKEUP_TIMER_NAME,
	.id		= -1,
	.num_resources	= ARRAY_SIZE(tl7689_wk_timer_resources),
	.resource	= tl7689_wk_timer_resources,
};

/*##############################################################*/
/*             TL7689 CamerIC controller device                    */
/*##############################################################*/
static struct mrv_camera_pdata camera_host_data = {
	.flags		= MRV_CAM_PLATFORM_DEFAULT,
	.mclk_10khz	= 20000,
	.prio_set	= &cam_priority_set,
};

static struct resource tl7689_camera_resources[] = {
	[0] = {
		.start	= TL7689_CAMERIC_BASE,
		.end	= TL7689_CAMERIC_BASE + SZ_128K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_CAMERIC_ISP,
		.end	= IRQ_TL7689_CAMERIC_MI,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_camera_device = {
	.name		= "mrv_camera",
	.id		= 0,
	.num_resources	= 2,
	.resource	= tl7689_camera_resources,
	.dev		= {
		.platform_data = &camera_host_data,
	},
};

/*##############################################################*/
/*             TL7689 Camera sensor device                    */
/*##############################################################*/
struct platform_device gc0329_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 0,
};

struct platform_device ov5640_camera_device = {
	.name	= "soc-camera-pdrv",
	.id	= 1,
};

/*m@nufront: start TODO*/
/*##############################################################*/
/*             TL7689 ON2 G1v2 controller device                    */
/*##############################################################*/
static struct resource tl7689_vpu_dec_resource[] = {
	[0] = {
		.start	= TL7689_ON2_DEC_BASE,
		.end	= TL7689_ON2_DEC_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_XINTDEC,
		.end	= IRQ_TL7689_XINTDEC,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_vpu_dec_device = {
	.name		= "tl7689-hx170",
	.id		= -1,
	.num_resources	= 2,
	.resource	= tl7689_vpu_dec_resource,
};

/*##############################################################*/
/*             TL7689 ON2 H1enc controller device                    */
/*##############################################################*/

static struct resource tl7689_vpu_enc_resource[] = {
	[0] = {
		.start	= TL7689_ON2_ENC_BASE,
		.end	= TL7689_ON2_ENC_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TL7689_HINTENC,
		.end	= IRQ_TL7689_HINTENC,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device tl7689_vpu_enc_device = {
	.name		= "tl7689-hx280",
	.id		= -1,
	.num_resources	= 2,
	.resource	= tl7689_vpu_enc_resource,
};

/*m@nufront: end TODO*/

#ifdef CONFIG_MACH_TL7689_PBX_BOARD
static struct smsc911x_platform_config smsc911x_config = {
	.flags		= SMSC911X_USE_32BIT,
	.irq_polarity	= SMSC911X_IRQ_POLARITY_ACTIVE_HIGH,
	.irq_type	= SMSC911X_IRQ_TYPE_PUSH_PULL,
	.phy_interface	= PHY_INTERFACE_MODE_MII,
};

static struct resource realview_pbx_smsc911x_resources[] = {
	[0] = {
		.start	= REALVIEW_PBX_ETH_BASE,
		.end	= REALVIEW_PBX_ETH_BASE + SZ_64K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_PBX_ETH,
		.end	= IRQ_PBX_ETH,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device realview_eth_device = {
	.name		= "smsc911x",
	.id		= 0,
	.num_resources	= 2,
	.resource	= realview_pbx_smsc911x_resources,
	.dev		= {
		.platform_data = &smsc911x_config,
	},
};
#endif

int board_device_register(struct platform_device *pdev, void * data)
{
	pdev->dev.platform_data = data;
	return platform_device_register(pdev);
}

int soc_plat_register_devices(struct soc_plat_dev *sdev, int size)
{
	int idx = 0, ret = 0;
	for (idx = 0; idx < size; idx++) {
		if (sdev[idx].data)
			sdev[idx].pdev->dev.platform_data = sdev[idx].data;
		ret = platform_device_register(sdev[idx].pdev);
		if (ret < 0)
			return ret;
	}
	return 0;
}
#if 0
int tl7689_system_pm_init(void)
{
	__tl7689_reset = tl7689_reset;
	arm_pm_restart = tl7689_restart;
	pm_power_off = tl7689_power_off;
	return 0;
}
#endif
void soc_amba_register_devices(struct amba_device **adev, int size)
{
	int idx = 0;
	for (idx = 0; idx < size; idx++) {
		amba_device_register(adev[idx], &iomem_resource);
	}
}

#ifdef CONFIG_DRM_MALI
struct platform_device tl7689_mali_drm_device = {
	.name	= "mali_drm",
	.id	= -1,
};
#endif

/*##############################################################*/
/*              TL7689 PWM  controller device                   */
/*##############################################################*/

struct tl7689_backlight_config mistral_bl_data = {
	.default_intensity = 6,
};

struct platform_device tl7689_backlight_device = {
	.name	= "tl7689-bl",
	.id	= -1,
	.dev	= {
		.platform_data = &mistral_bl_data,
	},
};

/*##############################################################*/
/*             TL7689 PL330 DMA device                          */
/*##############################################################*/
#ifdef CONFIG_PL330_DMA

#define TL7689_DMA_330S_IRQ IRQ_tl7689_DMA330_INTR0

static struct dma_pl330_peri pl330_plat_data_peri[]={
	[0] = {
		.peri_id	= 0,
		.rqtype		= MEMTOMEM,
	},
};
static struct dma_pl330_platdata pl330_plat_data ={
	.nr_valid_peri	= 1,
	.mcbuf_sz	= SZ_64K,
	.peri		= &pl330_plat_data_peri,
};
AMBA_DEVICE(pl330_dma, "dma-pl330", TL7689_DMA_330S, SZ_4K, &pl330_plat_data);
#endif

/*************************************************************
 *            NUSMART  GPIO KEYS Devices
 *            Author   xujing jing.xu@nufront.com
 *            Date     2012-04-05
 **************************************************************/
static struct gpio_keys_button nusmart_buttons_data[KEY_NUM] = {
	{
		.code		= KEY_VOLUMEDOWN,
		.irq		= IRQ_TL7689_GPIO1_INTR18,
		.key_pressed	= PRESSED_IS_LOWLEVEL,
		.desc		= "tl7689_keys",
	},
	{
		.code		= KEY_VOLUMEUP,
		.irq		= IRQ_TL7689_GPIO1_INTR20,
		.key_pressed	= PRESSED_IS_LOWLEVEL,
		.desc		= "tl7689_keys",
	}
};
static struct gpio_keys_platform_data nusmart_gpio_keys_data = {
	.buttons	= nusmart_buttons_data,
	.nbuttons	= KEY_NUM,
	.name		= "tl7689-keys",
};


struct platform_device nusmart_gpio_keys_device = {
	.name		= "nusmart-gpio-keys",
	.id		= 0,
	.dev		= {
		.platform_data = &nusmart_gpio_keys_data,
	},
};

struct rfkill_gpio_platform_data tl7689ref_bt_rfkill_data = {
	.name		= "bcm_rfkill",
#ifdef CONFIG_MACH_TL7689_PAD_REF
	.reset_gpio	= 53,
	.shutdown_gpio	= 65,
#else
	.reset_gpio	= 8 + 32 + 13,
	.shutdown_gpio	= 8 + 32 + 25,
#endif
	.power_clk_name	= NULL,
	.type		= RFKILL_TYPE_BLUETOOTH,
};

struct platform_device tl7689ref_bt_rfkill_device = {
	.name	= "rfkill_gpio",
	.id	= -1,
	.dev	= {
		.platform_data = &tl7689ref_bt_rfkill_data,
	},
};
#ifdef CONFIG_BT_BCM_LOWPOWER

#ifdef CONFIG_MACH_TL7689_PAD_REF
	#define BT2AP_WAKE IRQ_TL7689_GPIO0_WAKEUP_6
	#define AP2BT_WAKE 54
#else
	#define BT2AP_WAKE 92
	#define AP2BT_WAKE 91
#endif

static struct resource bluesleep_resources[] = {
	{
		.name	= "gpio_host_wake",
		.start	= BT2AP_WAKE,
		.end	= BT2AP_WAKE,
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= BT2AP_WAKE,
		.end	= BT2AP_WAKE,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "gpio_ext_wake",
		.start	= AP2BT_WAKE,
		.end	= AP2BT_WAKE,
		.flags	= IORESOURCE_IO,
	},
};

struct platform_device brcm_bluesleep_device = {
	.name = "bluesleep",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(bluesleep_resources),
	.resource	= bluesleep_resources,
};

#endif

#ifdef CONFIG_SND_SOC_ALC5631
struct tl7689ref_rt5631_jd_platform_data {
	unsigned int irq;
};

struct tl7689ref_rt5631_jd_platform_data tl7689ref_rt5631_jd_data = {
	.irq = IRQ_TL7689_GPIO0_WAKEUP_2,
};

struct platform_device tl7689ref_rt5631_jd_device = {
	.name	= "rt5631-jd",
	.dev	= {
		.platform_data = &tl7689ref_rt5631_jd_data,
	},
};
#endif

struct timed_gpio gpios_config[] = {
	[0] = {
		.name		= "vibrator",
		.gpio		= 35,
		.max_timeout	= 5000000,
		.active_low	= 0,
	}
};

struct timed_gpio_platform_data tl7689_vibrator_pdata = {
	.num_gpios	= 1,
	.gpios		= gpios_config,
};

struct platform_device tl7689_vibrator_device = {
	.name	= "timed-gpio",
	.id	= -1,
	.dev	= {
		.platform_data = &tl7689_vibrator_pdata,
	},
};

#if 0
/************************************************************
 *	Power switch of soc
 ************************************************************/
#define ZSP_OFFSET	7
#define ISP_OFFSET	6
#define VPU_H1_OFFSET	5
#define VPU_G1_OFFSET	4
#define GC300_OFFSET	3
#define MALI_PP_OFFSET	2
#define MALI_L2_OFFSET	1
#define MALI_GP_OFFSET	0

#define MALI_MASK	BIT(MALI_GP_OFFSET) | BIT(MALI_L2_OFFSET) |\
	BIT(MALI_PP_OFFSET)

static DEFINE_SPINLOCK(switch_lock);

static inline void sync_set(unsigned int mask)
{
	unsigned long flag;
	void * __iomem addr = __io_address(PRCM_SYS_PWR_CTRL);
	unsigned int regval = 0;

	spin_lock_irqsave(&switch_lock, flag);
	regval = readl(addr);
	regval |= mask;
	writel(regval, addr);
	spin_unlock_irqrestore(&switch_lock, flag);
}

static inline void sync_clear(unsigned int mask)
{
	unsigned long flag;
	void * __iomem addr = __io_address(PRCM_SYS_PWR_CTRL);
	unsigned int regval = 0;

	spin_lock_irqsave(&switch_lock, flag);
	regval = readl(addr);
	regval &= ~mask;
	writel(regval, addr);
	spin_unlock_irqrestore(&switch_lock, flag);
}

static inline int get_state(unsigned int mask)
{
	void * __iomem addr = __io_address(PRCM_SYS_PWR_CTRL);
	unsigned int regval = 0;

	regval = readl(addr);
	regval &= mask;
	return (regval == 0 ? 0 : 1);
}

int zsp_power_on(void * dev_id)
{
	sync_set(BIT(ZSP_OFFSET));
	return 0;
}
int zsp_power_off(void * dev_id)
{
	sync_clear(BIT(ZSP_OFFSET));
	return 0;
}
int zsp_get_state(void * dev_id)
{
	return get_state(BIT(ZSP_OFFSET));
}
struct power_switch_ops zsp_switch = {
	.power_on	= zsp_power_on,
	.power_off	= zsp_power_off,
	.get_state	= zsp_get_state,
};

int isp_power_on(void * dev_id)
{
	sync_set(BIT(ISP_OFFSET));
	return 0;
}
int isp_power_off(void * dev_id)
{
	sync_clear(BIT(ISP_OFFSET));
	return 0;
}
int isp_get_state(void * dev_id)
{
	return get_state(BIT(ISP_OFFSET));
}
struct power_switch_ops isp_switch = {
	.power_on	= isp_power_on,
	.power_off	= isp_power_off,
	.get_state	= isp_get_state,
};

int vpu_h1_power_on(void * dev_id)
{
	sync_set(BIT(VPU_H1_OFFSET));
	return 0;
}
int vpu_h1_power_off(void * dev_id)
{
	sync_clear(BIT(VPU_H1_OFFSET));
	return 0;
}
int vpu_h1_get_state(void * dev_id)
{
	return get_state(BIT(VPU_H1_OFFSET));
}
struct power_switch_ops vpu_h1_switch = {
	.power_on	= vpu_h1_power_on,
	.power_off	= vpu_h1_power_off,
	.get_state	= vpu_h1_get_state,
};

int vpu_g1_power_on(void * dev_id)
{
	sync_set(BIT(VPU_G1_OFFSET));
	return 0;
}
int vpu_g1_power_off(void * dev_id)
{
	sync_clear(BIT(VPU_G1_OFFSET));
	return 0;
}
int vpu_g1_get_state(void * dev_id)
{
	return get_state(BIT(VPU_G1_OFFSET));
}
struct power_switch_ops vpu_g1_switch = {
	.power_on	= vpu_g1_power_on,
	.power_off	= vpu_g1_power_off,
	.get_state	= vpu_g1_get_state,
};

int gc300_power_on(void * dev_id)
{
	sync_set(BIT(GC300_OFFSET));
	return 0;
}
int gc300_power_off(void * dev_id)
{
	sync_clear(BIT(GC300_OFFSET));
	return 0;
}
int gc300_get_state(void * dev_id)
{
	return get_state(BIT(GC300_OFFSET));
}
struct power_switch_ops gc300_switch = {
	.power_on	= gc300_power_on,
	.power_off	= gc300_power_off,
	.get_state	= gc300_get_state,
};

int mali_power_on(void * dev_id)
{
	sync_set(MALI_MASK);
	return 0;
}
int mali_power_off(void * dev_id)
{
	sync_clear(MALI_MASK);
	return 0;
}
int mali_get_state(void * dev_id)
{
	return get_state(MALI_MASK);
}
struct power_switch_ops mali_switch = {
	.power_on	= mali_power_on,
	.power_off	= mali_power_off,
	.get_state	= mali_get_state,
};

struct timed_gpio gpios_config[] = {
	[0] = {
		.name		= "vibrator",
		.gpio		= 35,
		.max_timeout	= 5000000,
		.active_low	= 0,
	}
};

struct timed_gpio_platform_data tl7689_vibrator_pdata = {
	.num_gpios	= 1,
	.gpios		= gpios_config,
};

struct platform_device tl7689_vibrator_device = {
	.name	= "timed-gpio",
	.id	= -1,
	.dev	= {
		.platform_data = &tl7689_vibrator_pdata,
	},
};
#endif
