/*
 * arch/arm/mach-tl7689/include/mach/board-tl7689.h
 *
 * Copyright (C) 2010 NUFRONT Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __ASM_ARCH_BOARD_TL7689_H
#define __ASM_ARCH_BOARD_TL7689_H

#include <mach/platform.h>

/*
 * Internal Memory
 */
#define TL7689_BOOTROM_BASE              0x00000000 /* BootROM 32K */
#define TL7689_ROMCTRL_BASE              0x03000000 /* ROM CTRL 64K */
#define TL7689_RAMCTRL_BASE              0x03010000 /* RAM CTRL 64K */

/*
 * External Memory
 */
#define TL7689_DDR_BASE                  0x10000000 /* LPDDR2/DDR3 4G */

/*
 * Debug APB
 */
#define TL7689_DEBUG_ROM                 0x04000000
#define TL7689_ETB                       0x04001000
#define TL7689_CTI                       0x04002000
#define TL7689_IPIU                      0x04003000
#define TL7689_FUNNEL                    0x04004000
#define TL7689_ITM                       0x04005000
#define TL7689_SWO                       0x04006000
#define TL7689_A9_DEBUG_ROM              0x04100000
#define TL7689_COREDBG0                  0x04110000
#define TL7689_PMU0                      0x04111000
#define TL7689_COREDBG1                  0x04112000
#define TL7689_PMU1                      0x04113000
#define TL7689_CTI0                      0x04118000
#define TL7689_CTI1                      0x04119000
#define TL7689_PTM0                      0x0411C000
#define TL7689_PTM1                      0x0411D000

/*
 * ARM MPU
 */
#define TL7689_SCU_BASE                  0x05040000 /* MPCore-A9 8K */
#define TL7689_GIC_CPU_BASE              (TL7689_SCU_BASE + 0x100)
#define TL7689_GTIMER_BASE               (TL7689_SCU_BASE + 0x200)
#define TL7689_TWD_BASE                  (TL7689_SCU_BASE + 0x600)
#define TL7689_TWD_SIZE                  0x00000100
#define TL7689_TWD_PERCPU_BASE           (TL7689_SCU_BASE + 0x700)
#define TL7689_GIC_DIST_BASE             (TL7689_SCU_BASE + 0x1000)      
#define TL7689_L220_BASE                 0x05042000

/*
 * L4_CFG(0x05000000 ~ 0x05800000)
 */
#define TL7689_DISPCTR0_BASE             0x05010000
#define TL7689_DISPCTR1_BASE             0x05011000
#define TL7689_DMA_330_BASE              0x05020000
#define TL7689_DMA_330S_BASE             0x05021000
#define TL7689_MALI_BASE                 0x05030000 /* MALI 64K */
#define TL7689_SDMMC0_BASE               0x05070000 /* SD/MMC0 4K */
#define TL7689_SDMMC1_BASE               0x05071000 /* SD/MMC1 4K */
#define TL7689_SDMMC2_BASE               0x05072000 /* SD/MMC2 4K */
#define TL7689_DSI_BASE               	 0x05078000 /* DSI 4K */
#define TL7689_CSI_BASE               	 0x05079000 /* CSI 4K */
#define TL7689_USB_OHCI_BASE             0x05080000 /* USB OHCI 64K */
#define TL7689_USB_EHCI_BASE             0x05090000 /* USB EHCI 64K */
#define TL7689_USB_OTG_BASE              0x050A0000 /* USB OTG 64K */
#define TL7689_ON2_DEC_BASE              0x050C0000 /* ON2 Decoder 64K */
#define TL7689_ON2_ENC_BASE              0x050D0000 /* ON2 Encoder 64K */
#define TL7689_DRAMCTRL_BASE             0x050E0000 /* DRAM controller 4K */
#define TL7689_TWOD_BASE                 0x05140000 /* GC300 256K */
#define TL7689_CAMERIC_BASE              0x05180000 /* CamerIC 128K */

/*
 * L4_CFG_WAKEUP(0x05800000 ~ 0x05A00000)
 */
#define TL7689_SARRAM_BASE               0x05800000 /* SARRAM 4K */
#define TL7689_SARRAM_CTRL               0x05820000 /* SARRAM controller 4K */
#define TL7689_PRCM_BASE                 0x05821000 /* PRCM 4K */
#define TL7689_SCM_BASE                  0x05822000 /* SCM 4K */
#define TL7689_GPIO0_BASE                0x05823000 /* GPIO 0 4K */
#define TL7689_TIMER0_BASE               0x05824000 /* Timer 0 4K*/

/*
 * L4_PER(0x06000000 ~ 0x07000000)
 */
#define TL7689_I2S0_BASE                 0x06060000 /* I2S 0 4K */
#define TL7689_IISFIFO                   0x0800
#define TL7689_UART0_BASE                0x06090000 /* UART 0 64K */
#define TL7689_UART1_BASE                0x060A0000 /* UART 1 64K */
#define TL7689_UART2_BASE                0x060B0000 /* UART 2 64K */
#define TL7689_UART3_BASE                0x060C0000 /* UART 3 64K */
#define TL7689_SPI0_BASE                 0x060D0000 /* SPI 0 64K */
#define TL7689_SPI1_BASE                 0x060E0000 /* SPI 1 64K */
#define TL7689_SPI2_BASE                 0x060F0000 /* SPI 2 64K */
#define TL7689_I2C0_BASE                 0x06100000 /* I2c 0 64K */
#define TL7689_I2C1_BASE                 0x06110000 /* I2c 1 64K */
#define TL7689_I2C2_BASE                 0x06120000 /* I2c 2 64K */
#define TL7689_I2C3_BASE                 0x06130000 /* I2c 3 64K */
#define TL7689_GPIO1_BASE                0x06140000 /* GPIO 1 4K */
#define TL7689_GPIO_INT0_BASE          	 0x06140000 /* GPIO_INT0 4K */
#define TL7689_GPIO_INT1_BASE            0x06140000 /* GPIO_INT1 4K */
#define TL7689_TIMER1_BASE               0x06150000 /* Timer 1 64K */
#define TL7689_AUX_BASE                  0x06160000 /* AUX 4K */
#define TL7689_PWM_BASE                  0x06160000 /* PWM 4K */
#define TL7689_EFUSE_BASE                0x06161000 /* EFUSE 4K */
#define TL7689_SPI3_BASE                 0x06170000 /* SPI 3 64K */

/*
 * L3(0x07000000 ~ 0x08000000)
 */
#define TL7689_IRAM_BASE               	0x07000000 /* IRAM 64K*/
#define TL7689_L3_ISS_TA_BASE		0x07205C00 /* L3 ta_l3_iss.ta 1K */
#define TL7689_CAMERIC_TA_BASE		0x05407C00 /* L4_CFG cameric.ta 1K */

/*
 * L3M(0x08000000 ~ 0x09000000)
 */
#define TL7689_MEM_MAX0_BASE             0x08008000 /* MemMax0 1K */

#define CONTEXT_SIZE_WORDS              1024
#endif /*__ASM_ARCH_BOARD_TL7689_H*/
