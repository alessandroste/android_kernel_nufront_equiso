/*
 * arch/arm/mach-tl7689/include/mach/irqs-tl7689.h
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

#ifndef __MACH_IRQS_TL7689_H
#define __MACH_IRQS_TL7689_H

/*
 * Irqs
 */
#define IRQ_TL7689_GIC_START 32


/*
 * TL7689 on-board gic irq sources
 */
/* Sonics fabic_err width 1 active high */
#define IRQ_TL7689_FABIC_ERR                     (IRQ_TL7689_GIC_START + 191)
/* bp_int_ap width 20 active high */
#define IRQ_TL7689_BP_INT_AP_19			(IRQ_TL7689_GIC_START + 190)
#define IRQ_TL7689_BP_INT_AP_18			(IRQ_TL7689_GIC_START + 189)
#define IRQ_TL7689_BP_INT_AP_17			(IRQ_TL7689_GIC_START + 188)
#define IRQ_TL7689_BP_INT_AP_16			(IRQ_TL7689_GIC_START + 187)
#define IRQ_TL7689_BP_INT_AP_15			(IRQ_TL7689_GIC_START + 186)
#define IRQ_TL7689_BP_INT_AP_14			(IRQ_TL7689_GIC_START + 185)
#define IRQ_TL7689_BP_INT_AP_13			(IRQ_TL7689_GIC_START + 184)
#define IRQ_TL7689_BP_INT_AP_12			(IRQ_TL7689_GIC_START + 183)
#define IRQ_TL7689_BP_INT_AP_11			(IRQ_TL7689_GIC_START + 182)
#define IRQ_TL7689_BP_INT_AP_10			(IRQ_TL7689_GIC_START + 181)
#define IRQ_TL7689_BP_INT_AP_9			(IRQ_TL7689_GIC_START + 180)
#define IRQ_TL7689_BP_INT_AP_8			(IRQ_TL7689_GIC_START + 179)
#define IRQ_TL7689_BP_INT_AP_7			(IRQ_TL7689_GIC_START + 178)
#define IRQ_TL7689_BP_INT_AP_6			(IRQ_TL7689_GIC_START + 177)
#define IRQ_TL7689_BP_INT_AP_5			(IRQ_TL7689_GIC_START + 176)
#define IRQ_TL7689_BP_INT_AP_4			(IRQ_TL7689_GIC_START + 175)
#define IRQ_TL7689_BP_INT_AP_3			(IRQ_TL7689_GIC_START + 174)
#define IRQ_TL7689_BP_INT_AP_2			(IRQ_TL7689_GIC_START + 173)
#define IRQ_TL7689_BP_INT_AP_1			(IRQ_TL7689_GIC_START + 172)
#define IRQ_TL7689_BP_INT_AP_0			(IRQ_TL7689_GIC_START + 171)
/* mipi-dsi width 2 active high */
#define IRQ_TL7689_DSI_INT0                      (IRQ_TL7689_GIC_START + 170)
#define IRQ_TL7689_DSI_INT1                      (IRQ_TL7689_GIC_START + 169)
/* mipi-csi width 2 active high */
#define IRQ_TL7689_CSI_INT0                      (IRQ_TL7689_GIC_START + 168)
#define IRQ_TL7689_CSI_INT1                      (IRQ_TL7689_GIC_START + 167)
/* nandflash width 1 active high */
#define IRQ_TL7689_NANDFLASH                     (IRQ_TL7689_GIC_START + 166)
/* cortex-m3 width 2 active high */
#define IRQ_TL7689_M3TOCPU1                      (IRQ_TL7689_GIC_START + 165)
#define IRQ_TL7689_M3TOCPU0                      (IRQ_TL7689_GIC_START + 164)
/* Nufront wakeup_irq width 1 active high */
#define IRQ_TL7689_WAKEUP                        (IRQ_TL7689_GIC_START + 163)
/* Nufront Nustar2 active high */
#define IRQ_TL7689_COMMRX_1                      (IRQ_TL7689_GIC_START + 162)
#define IRQ_TL7689_COMMRX_0                      (IRQ_TL7689_GIC_START + 161)
#define IRQ_TL7689_COMMTX_1                      (IRQ_TL7689_GIC_START + 160)
#define IRQ_TL7689_COMMTX_0                      (IRQ_TL7689_GIC_START + 159)
#define IRQ_TL7689_DEFLAGS1_6                    (IRQ_TL7689_GIC_START + 158)
#define IRQ_TL7689_DEFLAGS1_5                    (IRQ_TL7689_GIC_START + 157)
#define IRQ_TL7689_DEFLAGS1_4                    (IRQ_TL7689_GIC_START + 156)
#define IRQ_TL7689_DEFLAGS1_3                    (IRQ_TL7689_GIC_START + 155)
#define IRQ_TL7689_DEFLAGS1_2                    (IRQ_TL7689_GIC_START + 154)
#define IRQ_TL7689_DEFLAGS1_1                    (IRQ_TL7689_GIC_START + 153)
#define IRQ_TL7689_DEFLAGS1_0                    (IRQ_TL7689_GIC_START + 152)
#define IRQ_TL7689_DEFLAGS0_6                    (IRQ_TL7689_GIC_START + 151)
#define IRQ_TL7689_DEFLAGS0_5                    (IRQ_TL7689_GIC_START + 150)
#define IRQ_TL7689_DEFLAGS0_4                    (IRQ_TL7689_GIC_START + 149)
#define IRQ_TL7689_DEFLAGS0_3                    (IRQ_TL7689_GIC_START + 148)
#define IRQ_TL7689_DEFLAGS0_2                    (IRQ_TL7689_GIC_START + 147)
#define IRQ_TL7689_DEFLAGS0_1                    (IRQ_TL7689_GIC_START + 146)
#define IRQ_TL7689_DEFLAGS0_0                    (IRQ_TL7689_GIC_START + 145)
#define IRQ_TL7689_PMUIRQ_1                      (IRQ_TL7689_GIC_START + 144)
#define IRQ_TL7689_PMUIRQ_0                      (IRQ_TL7689_GIC_START + 143)
#define IRQ_TL7689_SCU_EV_ABORT                  (IRQ_TL7689_GIC_START + 142)
/* Nufront Nustar2 (L2C) active high */
#define IRQ_TL7689_L2_DECERRINTR                 (IRQ_TL7689_GIC_START + 141)
#define IRQ_TL7689_L2_ECNTRINTR                  (IRQ_TL7689_GIC_START + 140)
#define IRQ_TL7689_L2_ERRRDINTR                  (IRQ_TL7689_GIC_START + 139)
#define IRQ_TL7689_L2_ERRRTINTR                  (IRQ_TL7689_GIC_START + 138)
#define IRQ_TL7689_L2_ERRWDINTR                  (IRQ_TL7689_GIC_START + 137)
#define IRQ_TL7689_L2_ERRWTINTR                  (IRQ_TL7689_GIC_START + 136)
#define IRQ_TL7689_L2_L2CCINTR                   (IRQ_TL7689_GIC_START + 135)
#define IRQ_TL7689_L2_SLVERRINTR                 (IRQ_TL7689_GIC_START + 134)
/* AXI3RID MERGE active high */
#define IRQ_TL7689_MALI_RID_ERR_INT              (IRQ_TL7689_GIC_START + 133)
/* ARM DMA330 active high */
#define IRQ_TL7689_DMA330_INTR27                 (IRQ_TL7689_GIC_START + 132)
#define IRQ_TL7689_DMA330_INTR26                 (IRQ_TL7689_GIC_START + 131)
#define IRQ_TL7689_DMA330_INTR25                 (IRQ_TL7689_GIC_START + 130)
#define IRQ_TL7689_DMA330_INTR24                 (IRQ_TL7689_GIC_START + 129)
#define IRQ_TL7689_DMA330_INTR23                 (IRQ_TL7689_GIC_START + 128)
#define IRQ_TL7689_DMA330_INTR22                 (IRQ_TL7689_GIC_START + 127)
#define IRQ_TL7689_DMA330_INTR21                 (IRQ_TL7689_GIC_START + 126)
#define IRQ_TL7689_DMA330_INTR20                 (IRQ_TL7689_GIC_START + 125)
#define IRQ_TL7689_DMA330_INTR19                 (IRQ_TL7689_GIC_START + 124)
#define IRQ_TL7689_DMA330_INTR18                 (IRQ_TL7689_GIC_START + 123)
#define IRQ_TL7689_DMA330_INTR17                 (IRQ_TL7689_GIC_START + 122)
#define IRQ_TL7689_DMA330_INTR16                 (IRQ_TL7689_GIC_START + 121)
#define IRQ_TL7689_DMA330_INTR15                 (IRQ_TL7689_GIC_START + 120)
#define IRQ_TL7689_DMA330_INTR14                 (IRQ_TL7689_GIC_START + 119)
#define IRQ_TL7689_DMA330_INTR13                 (IRQ_TL7689_GIC_START + 118)
#define IRQ_TL7689_DMA330_INTR12                 (IRQ_TL7689_GIC_START + 117)
#define IRQ_TL7689_DMA330_INTR11                 (IRQ_TL7689_GIC_START + 116)
#define IRQ_TL7689_DMA330_INTR10                 (IRQ_TL7689_GIC_START + 115)
#define IRQ_TL7689_DMA330_INTR9                  (IRQ_TL7689_GIC_START + 114)
#define IRQ_TL7689_DMA330_INTR8                  (IRQ_TL7689_GIC_START + 113)
#define IRQ_TL7689_DMA330_INTR7                  (IRQ_TL7689_GIC_START + 112)
#define IRQ_TL7689_DMA330_INTR6                  (IRQ_TL7689_GIC_START + 111)
#define IRQ_TL7689_DMA330_INTR5                  (IRQ_TL7689_GIC_START + 110)
#define IRQ_TL7689_DMA330_INTR4                  (IRQ_TL7689_GIC_START + 109)
#define IRQ_TL7689_DMA330_INTR3                  (IRQ_TL7689_GIC_START + 108)
#define IRQ_TL7689_DMA330_INTR2                  (IRQ_TL7689_GIC_START + 107)
#define IRQ_TL7689_DMA330_INTR1                  (IRQ_TL7689_GIC_START + 106)
#define IRQ_TL7689_DMA330_INTR0                  (IRQ_TL7689_GIC_START + 105)
#define IRQ_TL7689_DMA330_ABORT                  (IRQ_TL7689_GIC_START + 104)
/* ARM MALI400 active high */
#define IRQ_TL7689_MALI_IRQPPMMU1                (IRQ_TL7689_GIC_START + 103)
#define IRQ_TL7689_MALI_IRQPP1                   (IRQ_TL7689_GIC_START + 102)
#define IRQ_TL7689_MALI_IRQPPMMU0                (IRQ_TL7689_GIC_START + 101)
#define IRQ_TL7689_MALI_IRQPP0                   (IRQ_TL7689_GIC_START + 100)
#define IRQ_TL7689_MALI_IRQGPMMU                 (IRQ_TL7689_GIC_START + 99)
#define IRQ_TL7689_MALI_IRQGP                    (IRQ_TL7689_GIC_START + 98)
/* Hantro video decoder active high */
#define IRQ_TL7689_XINTDEC                       (IRQ_TL7689_GIC_START + 97)
/* Hantro video encoder active high */
#define IRQ_TL7689_HINTENC                       (IRQ_TL7689_GIC_START + 96)
/* Nufront eFuse controller active high */
#define IRQ_TL7689_EFUSE_INTC                    (IRQ_TL7689_GIC_START + 95)
/* Denali ddr controller active high */
#define IRQ_TL7689_DDR_CTRL                      (IRQ_TL7689_GIC_START + 94)
/* SIMG camerIC-12MP active high */
#define IRQ_TL7689_CAMERIC_MI                    (IRQ_TL7689_GIC_START + 93)
#define IRQ_TL7689_CAMERIC_ISP                   (IRQ_TL7689_GIC_START + 92)
/* SIMG camerIC-12MP active high */
#define IRQ_TL7689_MIPI_CSI                   	(IRQ_TL7689_GIC_START + 91)
/* Nufront display controller active high */
#define IRQ_TL7689_DISPCTR1_HINT                 (IRQ_TL7689_GIC_START + 90)
#define IRQ_TL7689_DISPCTR1_VINT                 (IRQ_TL7689_GIC_START + 89)
#define IRQ_TL7689_DISPCTR1_ERR                  (IRQ_TL7689_GIC_START + 88)
#define IRQ_TL7689_DISPCTR0_HINT                 (IRQ_TL7689_GIC_START + 87)
#define IRQ_TL7689_DISPCTR0_VINT                 (IRQ_TL7689_GIC_START + 86)
#define IRQ_TL7689_DISPCTR0_ERR                  (IRQ_TL7689_GIC_START + 85)
/* Synopsys USB host controller ehci/ohci active high/low */
#define IRQ_TL7689_USB_EHCI                      (IRQ_TL7689_GIC_START + 84)
#define IRQ_TL7689_USB_OHCI                      (IRQ_TL7689_GIC_START + 83)
/* Synopsys USB OTG controller active high */
#define IRQ_TL7689_USB_OTG                       (IRQ_TL7689_GIC_START + 82)
/* Vivante GC300 active high */
#define IRQ_TL7689_GC300_INTR                    (IRQ_TL7689_GIC_START + 81)
/* Evatronix SD/MMC x 3 active high */
#define IRQ_TL7689_SDMMC2_INTR                   (IRQ_TL7689_GIC_START + 80)
#define IRQ_TL7689_SDMMC2_WAKEUP                 (IRQ_TL7689_GIC_START + 79)
#define IRQ_TL7689_SDMMC1_INTR                   (IRQ_TL7689_GIC_START + 78)
#define IRQ_TL7689_SDMMC1_WAKEUP                 (IRQ_TL7689_GIC_START + 77)
#define IRQ_TL7689_SDMMC0_INTR                    (IRQ_TL7689_GIC_START + 76)
#define IRQ_TL7689_SDMMC0_WAKEUP                  (IRQ_TL7689_GIC_START + 75)
/* Synopsys I2C x 4 active high */
#define IRQ_TL7689_I2C3_INTR                     (IRQ_TL7689_GIC_START + 74)
#define IRQ_TL7689_I2C2_INTR                     (IRQ_TL7689_GIC_START + 73)
#define IRQ_TL7689_I2C1_INTR                     (IRQ_TL7689_GIC_START + 72)
#define IRQ_TL7689_I2C0_INTR                     (IRQ_TL7689_GIC_START + 71)
/* Synopsys SPI x 4 active high */
#define IRQ_TL7689_SPI3_INTR                     (IRQ_TL7689_GIC_START + 70)
#define IRQ_TL7689_SPI2_INTR                     (IRQ_TL7689_GIC_START + 69)
#define IRQ_TL7689_SPI1_INTR                     (IRQ_TL7689_GIC_START + 68)
#define IRQ_TL7689_SPI0_INTR                     (IRQ_TL7689_GIC_START + 67)
/* Synopsys UART x 4 active high */
#define IRQ_TL7689_UART3_INTR                    (IRQ_TL7689_GIC_START + 66)
#define IRQ_TL7689_UART2_INTR                    (IRQ_TL7689_GIC_START + 65)
#define IRQ_TL7689_UART1_INTR                    (IRQ_TL7689_GIC_START + 64)
#define IRQ_TL7689_UART0_INTR                    (IRQ_TL7689_GIC_START + 63)
/* Evatronix I2S x 2 active high */
#define IRQ_TL7689_I2S1_INTR                     (IRQ_TL7689_GIC_START + 62)
#define IRQ_TL7689_I2S0_INTR                     (IRQ_TL7689_GIC_START + 61)
/* Synopsys TIMER active high */
#define IRQ_TL7689_TIMER1_INTR7                  (IRQ_TL7689_GIC_START + 60)
#define IRQ_TL7689_TIMER1_INTR6                  (IRQ_TL7689_GIC_START + 59)
#define IRQ_TL7689_TIMER1_INTR5                  (IRQ_TL7689_GIC_START + 58)
#define IRQ_TL7689_TIMER1_INTR4                  (IRQ_TL7689_GIC_START + 57)
#define IRQ_TL7689_TIMER1_INTR3                  (IRQ_TL7689_GIC_START + 56)
#define IRQ_TL7689_TIMER1_INTR2                  (IRQ_TL7689_GIC_START + 55)
#define IRQ_TL7689_TIMER1_INTR1                  (IRQ_TL7689_GIC_START + 54)
#define IRQ_TL7689_TIMER1_INTR0                  (IRQ_TL7689_GIC_START + 53)
/* Synopsys GPIO active high */
#define IRQ_TL7689_GPIO1_16_31_FLAG              (IRQ_TL7689_GIC_START + 52)
#define IRQ_TL7689_GPIO1_INTR31                  (IRQ_TL7689_GIC_START + 51)
#define IRQ_TL7689_GPIO1_INTR30                  (IRQ_TL7689_GIC_START + 50)
#define IRQ_TL7689_GPIO1_INTR29                  (IRQ_TL7689_GIC_START + 49)
#define IRQ_TL7689_GPIO1_INTR28                  (IRQ_TL7689_GIC_START + 48)
#define IRQ_TL7689_GPIO1_INTR27                  (IRQ_TL7689_GIC_START + 47)
#define IRQ_TL7689_GPIO1_INTR26                  (IRQ_TL7689_GIC_START + 46)
#define IRQ_TL7689_GPIO1_INTR25                  (IRQ_TL7689_GIC_START + 45)
#define IRQ_TL7689_GPIO1_INTR24                  (IRQ_TL7689_GIC_START + 44)
#define IRQ_TL7689_GPIO1_INTR23                  (IRQ_TL7689_GIC_START + 43)
#define IRQ_TL7689_GPIO1_INTR22                  (IRQ_TL7689_GIC_START + 42)
#define IRQ_TL7689_GPIO1_INTR21                  (IRQ_TL7689_GIC_START + 41)
#define IRQ_TL7689_GPIO1_INTR20                  (IRQ_TL7689_GIC_START + 40)
#define IRQ_TL7689_GPIO1_INTR19                  (IRQ_TL7689_GIC_START + 39)
#define IRQ_TL7689_GPIO1_INTR18                  (IRQ_TL7689_GIC_START + 38)
#define IRQ_TL7689_GPIO1_INTR17                  (IRQ_TL7689_GIC_START + 37)
#define IRQ_TL7689_GPIO1_INTR16			(IRQ_TL7689_GIC_START + 36)
#define IRQ_TL7689_GPIO1_0_15_FLAG               (IRQ_TL7689_GIC_START + 35)
#define IRQ_TL7689_GPIO1_INTR15                  (IRQ_TL7689_GIC_START + 34)
#define IRQ_TL7689_GPIO1_INTR14                  (IRQ_TL7689_GIC_START + 33)
#define IRQ_TL7689_GPIO1_INTR13                  (IRQ_TL7689_GIC_START + 32)
#define IRQ_TL7689_GPIO1_INTR12                  (IRQ_TL7689_GIC_START + 31)
#define IRQ_TL7689_GPIO1_INTR11                  (IRQ_TL7689_GIC_START + 30)
#define IRQ_TL7689_GPIO1_INTR10                  (IRQ_TL7689_GIC_START + 29)
#define IRQ_TL7689_GPIO1_INTR9                   (IRQ_TL7689_GIC_START + 28)
#define IRQ_TL7689_GPIO1_INTR8                   (IRQ_TL7689_GIC_START + 27)
#define IRQ_TL7689_GPIO1_INTR7                   (IRQ_TL7689_GIC_START + 26)
#define IRQ_TL7689_GPIO1_INTR6                   (IRQ_TL7689_GIC_START + 25)
#define IRQ_TL7689_GPIO1_INTR5                   (IRQ_TL7689_GIC_START + 24)
#define IRQ_TL7689_GPIO1_INTR4                   (IRQ_TL7689_GIC_START + 23)
#define IRQ_TL7689_GPIO1_INTR3                   (IRQ_TL7689_GIC_START + 22)
#define IRQ_TL7689_GPIO1_INTR2                   (IRQ_TL7689_GIC_START + 21)
#define IRQ_TL7689_GPIO1_INTR1                   (IRQ_TL7689_GIC_START + 20)
#define IRQ_TL7689_GPIO1_INTR0                   (IRQ_TL7689_GIC_START + 19)
/* Synopsys TIMER wakeup active high */
#define IRQ_TL7689_TIMER0_WAKEUP_1               (IRQ_TL7689_GIC_START + 18)
#define IRQ_TL7689_TIMER0_WAKEUP_0               (IRQ_TL7689_GIC_START + 17)
/* Synopsys GPIO wakeup active high */
#define IRQ_TL7689_GPIO0_WAKEUP_FLAG             (IRQ_TL7689_GIC_START + 16)
#define IRQ_TL7689_GPIO0_WAKEUP_15               (IRQ_TL7689_GIC_START + 15)
#define IRQ_TL7689_GPIO0_WAKEUP_14               (IRQ_TL7689_GIC_START + 14)
#define IRQ_TL7689_GPIO0_WAKEUP_13               (IRQ_TL7689_GIC_START + 13)
#define IRQ_TL7689_GPIO0_WAKEUP_12               (IRQ_TL7689_GIC_START + 12)
#define IRQ_TL7689_GPIO0_WAKEUP_11               (IRQ_TL7689_GIC_START + 11)
#define IRQ_TL7689_GPIO0_WAKEUP_10               (IRQ_TL7689_GIC_START + 10)
#define IRQ_TL7689_GPIO0_WAKEUP_9                (IRQ_TL7689_GIC_START + 9)
#define IRQ_TL7689_GPIO0_WAKEUP_8                (IRQ_TL7689_GIC_START + 8)
#define IRQ_TL7689_GPIO0_WAKEUP_7                (IRQ_TL7689_GIC_START + 7)
#define IRQ_TL7689_GPIO0_WAKEUP_6                (IRQ_TL7689_GIC_START + 6)
#define IRQ_TL7689_GPIO0_WAKEUP_5                (IRQ_TL7689_GIC_START + 5)
#define IRQ_TL7689_GPIO0_WAKEUP_4                (IRQ_TL7689_GIC_START + 4)
#define IRQ_TL7689_GPIO0_WAKEUP_3                (IRQ_TL7689_GIC_START + 3)
#define IRQ_TL7689_GPIO0_WAKEUP_2                (IRQ_TL7689_GIC_START + 2)
#define IRQ_TL7689_GPIO0_WAKEUP_1                (IRQ_TL7689_GIC_START + 1)
#define IRQ_TL7689_GPIO0_WAKEUP_0                (IRQ_TL7689_GIC_START + 0)


#define NR_GIC_TL7689 1
/*
 * Only define NR_IRQS if less than NR_IRQS_TL7689
 */
#define NR_IRQS_TL7689 (IRQ_TL7689_GIC_START + 192)

#if !defined(NR_IRQS) || (NR_IRQS < NR_IRQS_TL7689)
#undef NR_IRQS
#define NR_IRQS NR_IRQS_TL7689
#endif

#if !defined(MAX_GIC_NR) || (MAX_GIC_NR < NR_GIC_TL7689)
#undef MAX_GIC_NR
#define MAX_GIC_NR NR_GIC_TL7689
#endif

#endif/* __MACH_IRQS_TL7689_H */
