#ifndef PRCM_H___
#define PRCM_H___


// Section-1: PRCM Base Address Definition
#include <mach/board-tl7689.h>
#define PRCM_BASE          (0x05821000)
#define LOCAL_TIMER        (0x05040600)
#define TIMER_ADDR         (0x06150000)
#define TIMER_CLK_MHZ      12


// Section-2: PRCM Register Address Definition
#define PRCM_SYS_CLK_CTRL           ((PRCM_BASE) + 0x00)
#define PRCM_SYS_RST_CTRL           ((PRCM_BASE) + 0x04)
#define PRCM_SYS_MODE		    ((PRCM_BASE) + 0x30)
#define PRCM_SYS_PWR_CTRL           ((PRCM_BASE) + 0x40)
#define PRCM_SYS_PWR_STATUS         ((PRCM_BASE) + 0x44)
#define PRCM_NUSTAR_PWR_CTRL        ((PRCM_BASE) + 0x48)
#define PRCM_NUSTAR_PWR_STATUS      ((PRCM_BASE) + 0x4C)
#define PRCM_WAKEUP_IRQ_CTRL1       ((PRCM_BASE) + 0x50)
#define PRCM_WAKEUP_IRQ_CTRL2       ((PRCM_BASE) + 0x54)
//#define PRCM_ACK_ISO                ((PRCM_BASE) + 0x18)
//                                  ((PRCM_BASE) + 0x1C)
#define PRCM_MISC_REG0              ((PRCM_BASE) + 0x70)
#define PRCM_MISC_REG1              ((PRCM_BASE) + 0x74)
#define PRCM_MISC_REG2              ((PRCM_BASE) + 0x78)
#define PRCM_MISC_REG3              ((PRCM_BASE) + 0x7C)
//                                  ((PRCM_BASE) + 0x30)
//                                  ((PRCM_BASE) + 0x34)
#define PRCM_SYS_PLL_CTRL           ((PRCM_BASE) + 0x80)
#define PRCM_SYS_PLL_STATUS         ((PRCM_BASE) + 0x84)
#define PRCM_PLL0_CFG0              ((PRCM_BASE) + 0x88)
#define PRCM_PLL0_CFG1              ((PRCM_BASE) + 0x8c)
#define PRCM_PLL1_CFG0              ((PRCM_BASE) + 0x90)
#define PRCM_PLL1_CFG1              ((PRCM_BASE) + 0x94)
#define PRCM_PLL2_CFG0              ((PRCM_BASE) + 0x98)
#define PRCM_PLL2_CFG1              ((PRCM_BASE) + 0x9c)
#define PRCM_PLL3_CFG0              ((PRCM_BASE) + 0xa0)
#define PRCM_PLL3_CFG1              ((PRCM_BASE) + 0xa4)
#define PRCM_PLL4_CFG0              ((PRCM_BASE) + 0xa8)
#define PRCM_PLL4_CFG1              ((PRCM_BASE) + 0xac)
#define PRCM_DDR_PLL_CFG0           ((PRCM_BASE) + 0xb0)
#define PRCM_DDR_PLL_CFG1           ((PRCM_BASE) + 0xb4)
//#define PRCM_PLL5_CFG0            ((PRCM_BASE) + 0x68)
//#define PRCM_PLL5_CFG1            ((PRCM_BASE) + 0x6C)
//#define PRCM_PLL6_CFG0            ((PRCM_BASE) + 0x70)
//#define PRCM_PLL6_CFG1            ((PRCM_BASE) + 0x74)
//                                  ((PRCM_BASE) + 0x78)
//                                  ((PRCM_BASE) + 0x7C)
#define PRCM_CPU_CLK_DIV            ((PRCM_BASE) + 0x100)
#define PRCM_CPU_CLK_SWITCH         ((PRCM_BASE) + 0x104)
#define PRCM_CORESIGHT_CLK_CTRL     ((PRCM_BASE) + 0x108)
#define PRCM_DDR_CLK_CTRL           ((PRCM_BASE) + 0x10C)
#define PRCM_BUS_CLK_CTRL           ((PRCM_BASE) + 0x110)
#define PRCM_DMA_CLK_CTRL           ((PRCM_BASE) + 0x114)
#define PRCM_CORE_MEM_CLK_CTRL      ((PRCM_BASE) + 0x118)
#define PRCM_WKUP_MEM_CLK_CTRL      ((PRCM_BASE) + 0x11c)
#define PRCM_OUT_CLK_CTRL           ((PRCM_BASE) + 0x120)
#define PRCM_MALI_CLK_CTRL          ((PRCM_BASE) + 0x140)
#define PRCM_GC300_CLK_CTRL         ((PRCM_BASE) + 0x144)
#define PRCM_VPU_G1_CLK_CTRL        ((PRCM_BASE) + 0x148)
#define PRCM_VPU_H1_CLK_CTRL        ((PRCM_BASE) + 0x14c)
#define PRCM_ISP_CLK_CTRL           ((PRCM_BASE) + 0x150)
#define PRCM_NAND_CLK_CTRL          ((PRCM_BASE) + 0x180)
#define PRCM_SD_CLK_CTRL            ((PRCM_BASE) + 0x184)
//#define PRCM_ZSP_CLK_CTRL           ((PRCM_BASE) + 0xB0)
#define PRCM_USB_CLK_CTRL           ((PRCM_BASE) + 0x188)
#define PRCM_CM3_CLK_CTRL           ((PRCM_BASE) + 0x18c)
#define PRCM_SPI_CLK_CTRL           ((PRCM_BASE) + 0x190)
#define PRCM_I2C_CLK_CTRL           ((PRCM_BASE) + 0x194)
#define PRCM_UART_CLK_CTRL0         ((PRCM_BASE) + 0x198)
#define PRCM_UART_CLK_CTRL1         ((PRCM_BASE) + 0x19c)
#define PRCM_PWM_CLK_CTRL           ((PRCM_BASE) + 0x1a0)
#define PRCM_GPIO_CORE_CLK_CTRL     ((PRCM_BASE) + 0x1a8)
#define PRCM_GPIO_WKUP_CLK_CTRL     ((PRCM_BASE) + 0x1ac)
#define PRCM_TIMER_CORE_CLK_CTRL    ((PRCM_BASE) + 0x1b0)
#define PRCM_TIMER_WKUP_CLK_CTRL    ((PRCM_BASE) + 0x1b4)
#define PRCM_EFUSE_CLK_CTRL         ((PRCM_BASE) + 0x1b8)
#define PRCM_AUDIO_CLK_CTRL         ((PRCM_BASE) + 0x200)
#define PRCM_DIS0_CLK_CTRL          ((PRCM_BASE) + 0x204)
#define PRCM_DIS1_CLK_CTRL          ((PRCM_BASE) + 0x208)
//#define PRCM_I2S_CLK_CTRL           ((PRCM_BASE) + 0xCC)
//#define PRCM_CORE_MEM_CLK_CTRL      ((PRCM_BASE) + 0xD4)
//#define PRCM_WKUP_MEM_CLK_CTRL      ((PRCM_BASE) + 0xE0)
//#define PRCM_ALT_CLK_CTRL           ((PRCM_BASE) + 0xEC)
//#define PRCM_AUX_CLK_CTRL           ((PRCM_BASE) + 0xF0)


// Section-3: PRCM Register Bit(s) Definition

// PRCM_SYS_CLK_CTRL           
#define PRCM_SYS_CLK_CTRL_PRESERVE_12M                      (1 << 15)
#define PRCM_SYS_CLK_CTRL_STABLE_CNT_MASK                   (0x7F << 8)
#define PRCM_SYS_CLK_CTRL_GPIO_12M_SEL                      (1 << 1)
#define PRCM_SYS_CLK_CTRL_SYSCLK_32KEN                      (1 << 0)

// PRCM_SYS_RST_CTRL       
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_DEBUG1_RST             (1 << 11)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_DEBUG0_RST             (1 << 10)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_WATCHDOG1_RST          (1 << 9)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_WATCHDOG0_RST          (1 << 8)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_NEON1_RST              (1 << 7)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_NEON0_RST              (1 << 6)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_CPU1_RST               (1 << 5)
#define PRCM_SYS_RST_CTRL_NUSTAR_RST_CPU0_RST               (1 << 4)
#define PRCM_SYS_RST_CTRL_WATCHDOG_RST_EN                   (1 << 1)
#define PRCM_SYS_RST_CTRL_GLB_SW_RST                        (1 << 0)

// PRCM_SYS_PWR_CTRL       
//#define PRCM_SYS_PWR_CTRL_SYSTEM_PWR_CTRL_EN                (1 << 31)
//#define PRCM_SYS_PWR_CTRL_BP_EXT_SUS                        (1 << 30)
//#define PRCM_SYS_PWR_CTRL_INTERNAL_SUS                      (1 << 29)
#define PRCM_SYS_PWR_CTRL_PLL_PWR_STATE_MASK                (0x1F << 8)
#define PRCM_SYS_PWR_CTRL_ISP_PWR_STATE                     (1 << 7)
#define PRCM_SYS_PWR_CTRL_VPU_H1_PWR_STATE                  (1 << 6)
#define PRCM_SYS_PWR_CTRL_VPU_G1_PWR_STATE                  (1 << 5)
#define PRCM_SYS_PWR_CTRL_GC300_PWR_STATE                   (1 << 4)
#define PRCM_SYS_PWR_CTRL_MALI_PWR_STATE_PP1                (1 << 3)
#define PRCM_SYS_PWR_CTRL_MALI_PWR_STATE_PP                 (1 << 2)
#define PRCM_SYS_PWR_CTRL_MALI_PWR_STATE_L2C                (1 << 1)
#define PRCM_SYS_PWR_CTRL_MALI_PWR_STATE_GP                 (1 << 0)

// PRCM_NUSTAR_PWR_CTRL        
#define PRCM_NUSTAR_PWR_CTRL_BP_CLKSTOPPED                  (1 << 31)
#define PRCM_NUSTAR_PWR_CTRL_BP_PWRCTRLO_MASK               (0x3 << 29)
#define PRCM_NUSTAR_PWR_CTRL_BP_IRQ_OUT_MASK                (0x3 << 27)
#define PRCM_NUSTAR_PWR_CTRL_NUSTAR_LS_BP                   (1 << 26)
#define PRCM_NUSTAR_PWR_CTRL_L2_PD                          (1 << 25)
#define PRCM_NUSTAR_PWR_CTRL_NUSTAR_SLEEP_EN                (1 << 8)
#define PRCM_NUSTAR_PWR_CTRL_NUSTAR_PWR_STATE_MASK          (0xFF << 0)

// PRCM_SYS_PWR_STATUS         
#define PRCM_SYS_PWR_STATUS_PLL_PWRDN_MASK                  (0x1F << 8)
#define PRCM_SYS_PWR_STATUS_ISP_STATUS                      (1 << 7)
#define PRCM_SYS_PWR_STATUS_VPU_H1_STATUS                   (1 << 6)
#define PRCM_SYS_PWR_STATUS_VPU_G1_STATUS                   (1 << 5)
#define PRCM_SYS_PWR_STATUS_GC300_PWR_STATUS                (1 << 4)
//#define PRCM_SYS_PWR_STATUS_MALI_PWR_STATUS_TOTAL           (1 << 11)
#define PRCM_SYS_PWR_STATUS_MALI_PWR_STATUS_PP1             (1 << 3)
#define PRCM_SYS_PWR_STATUS_MALI_PWR_STATUS_PP              (1 << 2)
#define PRCM_SYS_PWR_STATUS_MALI_PWR_STATUS_L2C             (1 << 1)
#define PRCM_SYS_PWR_STATUS_MALI_PWR_STATUS_GP              (1 << 0)

// PRCM_WAKEUP_IRQ_CTRL    
#define PRCM_WAKEUP_IRQ_CTRL_WAKEUP_CYCLE_MASK              (0x3FF << 20)
#define PRCM_WAKEUP_IRQ_CTRL_WAKEUP_IRQ_EN_MASK             (0x1FFFF << 0)

// PRCM_ACK_ISO     
#define PRCM_ACK_ISO_NUSTAR_ACK_ISO_MASK                    (0x1FF << 8)
#define PRCM_ACK_ISO_APP_ACK_ISO_MASK                       (0xFF << 0)

// PRCM_MISC_REG0   
// PRCM_MISC_REG1    
// PRCM_MISC_REG2    
// PRCM_MISC_REG3

// PRCM_SYS_PLL_CTRL           
//#define PRCM_SYS_PLL_CTRL_PLL2_SW_CFG_EN                    (1 << 26)
//#define PRCM_SYS_PLL_CTRL_PLL1_SW_CFG_EN                    (1 << 25)
//#define PRCM_SYS_PLL_CTRL_PLL0_SW_CFG_EN                    (1 << 24)
//#define PRCM_SYS_PLL_CTRL_PLL6_CLKEN                        (1 << 22)
//#define PRCM_SYS_PLL_CTRL_PLL5_CLKEN                        (1 << 21)
#define PRCM_SYS_PLL_CTRL_PLL4_CLKEN                        (1 << 12)
#define PRCM_SYS_PLL_CTRL_PLL3_CLKEN                        (1 << 11)
#define PRCM_SYS_PLL_CTRL_PLL2_CLKEN                        (1 << 10)
#define PRCM_SYS_PLL_CTRL_PLL1_CLKEN                        (1 << 9)
#define PRCM_SYS_PLL_CTRL_PLL0_CLKEN                        (1 << 8)
//#define PRCM_SYS_PLL_CTRL_PLL6_RST                          (1 << 14)
//#define PRCM_SYS_PLL_CTRL_PLL5_RST                          (1 << 13)
#define PRCM_SYS_PLL_CTRL_DDR_PLL_RST                       (1 << 5)
#define PRCM_SYS_PLL_CTRL_PLL4_RST                          (1 << 4)
#define PRCM_SYS_PLL_CTRL_PLL3_RST                          (1 << 3)
#define PRCM_SYS_PLL_CTRL_PLL2_RST                          (1 << 2)
#define PRCM_SYS_PLL_CTRL_PLL1_RST                          (1 << 1)
#define PRCM_SYS_PLL_CTRL_PLL0_RST                          (1 << 0)
//#define PRCM_SYS_PLL_CTRL_PLL6_PWR_DN                       (1 << 6)
//#define PRCM_SYS_PLL_CTRL_PLL5_PWR_DN                       (1 << 5)
//#define PRCM_SYS_PLL_CTRL_PLL4_PWR_DN                       (1 << 4)
//#define PRCM_SYS_PLL_CTRL_PLL3_PWR_DN                       (1 << 3)
//#define PRCM_SYS_PLL_CTRL_PLL2_PWR_DN                       (1 << 2)
//#define PRCM_SYS_PLL_CTRL_PLL1_PWR_DN                       (1 << 1)
//#define PRCM_SYS_PLL_CTRL_PLL0_PWR_DN                       (1 << 0)

// PRCM_SYS_PLL_STATUS    
//#define PRCM_SYS_PLL_STATUS_PLL6_STABLE                     (1 << 6)
//#define PRCM_SYS_PLL_STATUS_PLL5_STABLE                     (1 << 5)
#define PRCM_SYS_PLL_STATUS_PLL4_STABLE                     (1 << 4)
#define PRCM_SYS_PLL_STATUS_PLL3_STABLE                     (1 << 3)
#define PRCM_SYS_PLL_STATUS_PLL2_STABLE                     (1 << 2)
#define PRCM_SYS_PLL_STATUS_PLL1_STABLE                     (1 << 1)
#define PRCM_SYS_PLL_STATUS_PLL0_STABLE                     (1 << 0)

// PRCM_PLL0_CFG0              
#define PRCM_PLL0_CFG0_PLL0_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL0_CFG0_PLL0_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL0_CFG1              
#define PRCM_PLL0_CFG1_PLL0_TEST                            (1 << 19)
#define PRCM_PLL0_CFG1_PLL0_ENSAT                           (1 << 18)
#define PRCM_PLL0_CFG1_PLL0_FASTEN                          (1 << 17)
#define PRCM_PLL0_CFG1_PLL0_BYPASS                          (1 << 16)
#define PRCM_PLL0_CFG1_PLL0_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL0_CFG1_PLL0_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL1_CFG0              
#define PRCM_PLL1_CFG0_PLL1_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL1_CFG0_PLL1_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL1_CFG1              
#define PRCM_PLL1_CFG1_PLL1_TEST                            (1 << 19)
#define PRCM_PLL1_CFG1_PLL1_ENSAT                           (1 << 18)
#define PRCM_PLL1_CFG1_PLL1_FASTEN                          (1 << 17)
#define PRCM_PLL1_CFG1_PLL1_BYPASS                          (1 << 16)
#define PRCM_PLL1_CFG1_PLL1_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL1_CFG1_PLL1_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL2_CFG0              
#define PRCM_PLL2_CFG0_PLL2_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL2_CFG0_PLL2_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL2_CFG1              
#define PRCM_PLL2_CFG1_PLL2_TEST                            (1 << 19)
#define PRCM_PLL2_CFG1_PLL2_ENSAT                           (1 << 18)
#define PRCM_PLL2_CFG1_PLL2_FASTEN                          (1 << 17)
#define PRCM_PLL2_CFG1_PLL2_BYPASS                          (1 << 16)
#define PRCM_PLL2_CFG1_PLL2_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL2_CFG1_PLL2_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL3_CFG0              
#define PRCM_PLL3_CFG0_PLL3_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL3_CFG0_PLL3_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL3_CFG1              
#define PRCM_PLL3_CFG1_PLL3_TEST                            (1 << 19)
#define PRCM_PLL3_CFG1_PLL3_ENSAT                           (1 << 18)
#define PRCM_PLL3_CFG1_PLL3_FASTEN                          (1 << 17)
#define PRCM_PLL3_CFG1_PLL3_BYPASS                          (1 << 16)
#define PRCM_PLL3_CFG1_PLL3_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL3_CFG1_PLL3_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL4_CFG0              
#define PRCM_PLL4_CFG0_PLL4_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL4_CFG0_PLL4_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL4_CFG1              
#define PRCM_PLL4_CFG1_PLL4_TEST                            (1 << 19)
#define PRCM_PLL4_CFG1_PLL4_ENSAT                           (1 << 18)
#define PRCM_PLL4_CFG1_PLL4_FASTEN                          (1 << 17)
#define PRCM_PLL4_CFG1_PLL4_BYPASS                          (1 << 16)
#define PRCM_PLL4_CFG1_PLL4_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL4_CFG1_PLL4_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL5_CFG0              
#define PRCM_PLL5_CFG0_PLL5_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL5_CFG0_PLL5_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL5_CFG1              
#define PRCM_PLL5_CFG1_PLL5_TEST                            (1 << 19)
#define PRCM_PLL5_CFG1_PLL5_ENSAT                           (1 << 18)
#define PRCM_PLL5_CFG1_PLL5_FASTEN                          (1 << 17)
#define PRCM_PLL5_CFG1_PLL5_BYPASS                          (1 << 16)
#define PRCM_PLL5_CFG1_PLL5_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL5_CFG1_PLL5_CLKOD_MASK                      (0xF << 0)

// PRCM_PLL6_CFG0              
#define PRCM_PLL6_CFG0_PLL6_CLKF_MASK                       (0x1FFF << 8)
#define PRCM_PLL6_CFG0_PLL6_CLKR_MASK                       (0x3F << 0)

// PRCM_PLL6_CFG1              
#define PRCM_PLL6_CFG1_PLL6_TEST                            (1 << 19)
#define PRCM_PLL6_CFG1_PLL6_ENSAT                           (1 << 18)
#define PRCM_PLL6_CFG1_PLL6_FASTEN                          (1 << 17)
#define PRCM_PLL6_CFG1_PLL6_BYPASS                          (1 << 16)
#define PRCM_PLL6_CFG1_PLL6_BWADJ_MASK                      (0xFFF << 4)
#define PRCM_PLL6_CFG1_PLL6_CLKOD_MASK                      (0xF << 0)

// PRCM_CPU_CLK_DIV            
#define PRCM_CPU_CLK_DIV_CPU_BUS_DIV_EN                     (1 << 20)
#define PRCM_CPU_CLK_DIV_CPU_BUS_DIV_MASK                   (0x3 << 16)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_EN                        (1 << 15)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_MASK                      (0x1F << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_EN                        (1 << 7)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_MASK                      (0x1F << 0)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_2                         (0x2 << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_3                         (0x3 << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_4                         (0x4 << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_8                         (0x8 << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV1_15                        (0xf << 8)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_2                         (0x2 << 0)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_4                         (0x4 << 0)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_8                         (0x8 << 0)
#define PRCM_CPU_CLK_DIV_CPU_DIV0_15                        (0xf << 0)

// PRCM_CPU_CLK_SWITCH         
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_EN                 (1 << 28)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_MASK               (0x7 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_MODE               (1 << 20)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX_SWITCH                  (1 << 16)
#define PRCM_CPU_CLK_SWITCH_CPU_MU1_SEL_EN                  (1 << 12)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_MASK               (0x7 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MU0_SEL_EN                  (1 << 4)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_MASK               (0x7 << 0)

#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_0                  (0x0 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_1                  (0x1 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_2                  (0x2 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_3                  (0x3 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_AUTO_SEL_4                  (0x4 << 24)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_0                  (0x0 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_1                  (0x1 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_2                  (0x2 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_3                  (0x3 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX1_SEL_4                  (0x4 << 8)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_0                  (0x0 << 0)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_1                  (0x1 << 0)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_2                  (0x2 << 0)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_3                  (0x3 << 0)
#define PRCM_CPU_CLK_SWITCH_CPU_MUX0_SEL_4                  (0x4 << 0)

// PRCM_CORESIGHT_CLK_CTRL     
#define PRCM_CORESIGHT_CLK_CTRL_CS_RSTN                     (1 << 12)
#define PRCM_CORESIGHT_CLK_CTRL_CS_AT_DIV_EN                (1 << 8)
#define PRCM_CORESIGHT_CLK_CTRL_CS_AT_DIV_MASK              (0xF << 4)
#define PRCM_CORESIGHT_CLK_CTRL_CS_ATCLKEN                  (1 << 1)
#define PRCM_CORESIGHT_CLK_CTRL_CS_HCLKEN                   (1 << 0)

// PRCM_DDR_CLK_CTRL           
#define PRCM_DDR_CLK_CTRL_DDR_RSTN                          (1 << 8)
#define PRCM_DDR_CLK_CTRL_DDR_HCLKEN                        (1 << 5)
#define PRCM_DDR_CLK_CTRL_DDR_CLKEN                         (1 << 4)
#define PRCM_DDR_CLK_CTRL_DDR_DIV_EN                        (1 << 3)
#define PRCM_DDR_CLK_CTRL_DDR_DIV_MASK                      (0x7 << 0)

// PRCM_BUS_CLK_CTRL           
#define PRCM_BUS_CLK_CTRL_BUS_RATIO_EN                      (1 << 12)
#define PRCM_BUS_CLK_CTRL_BUS_RATIO_MASK                    (0x7 << 8)

// PRCM_DMA_CLK_CTRL           
#define PRCM_DMA_CLK_CTRL_DMA_RSTN                          (1 << 4)
#define PRCM_DMA_CLK_CTRL_DMA_ACLKEN                        (1 << 0)

// PRCM_MALI_CLK_CTRL          
#define PRCM_MALI_CLK_CTRL_MALI_RSTN                        (1 << 20)
#define PRCM_MALI_CLK_CTRL_MALI_CLKEN                       (1 << 17)
#define PRCM_MALI_CLK_CTRL_MALI_CLK_SEL                     (1 << 16)
//#define PRCM_MALI_CLK_CTRL_MALI_DIV1_EN                     (1 << 12)
//#define PRCM_MALI_CLK_CTRL_MALI_DIV1_MASK                   (0xF << 8)
#define PRCM_MALI_CLK_CTRL_MALI_DIV0_EN                     (1 << 4)
#define PRCM_MALI_CLK_CTRL_MALI_DIV0_MASK                   (0xF << 0)

// PRCM_GC300_CLK_CTRL         
#define PRCM_GC300_CLK_CTRL_GC300_RSTN                      (1 << 16)
#define PRCM_GC300_CLK_CTRL_GC300_CLK_SEL                   (1 << 12)
#define PRCM_GC300_CLK_CTRL_GC300_HCLKEN                    (1 << 10)
#define PRCM_GC300_CLK_CTRL_GC300_ACLKEN                    (1 << 9)
#define PRCM_GC300_CLK_CTRL_GC300_CLKEN                     (1 << 8)
#define PRCM_GC300_CLK_CTRL_GC300_DIV_EN                    (1 << 4)
#define PRCM_GC300_CLK_CTRL_GC300_DIV_MASK                  (0xF << 0)

// PRCM_VPU_G1_CLK_CTRL        
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_RSTN                    (1 << 24)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_CLKEN                   (1 << 20)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_CLK_SEL                 (1 << 16)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_DIV1_EN                 (1 << 12)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_DIV1_MASK               (0xF << 8)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_DIV0_EN                 (1 << 4)
#define PRCM_VPU_G1_CLK_CTRL_VPU_G1_DIV0_MASK               (0xF << 0)

// PRCM_VPU_H1_CLK_CTRL        
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_RSTN                    (1 << 24)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_CLKEN                   (1 << 20)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_CLK_SEL                 (1 << 16)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_DIV1_EN                 (1 << 12)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_DIV1_MASK               (0xF << 8)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_DIV0_EN                 (1 << 4)
#define PRCM_VPU_H1_CLK_CTRL_VPU_H1_DIV0_MASK               (0xF << 0)

// PRCM_ISP_CLK_CTRL           
#define PRCM_ISP_CLK_CTRL_ISP_RSTN                          (1 << 12)
#define PRCM_ISP_CLK_CTRL_ISP_S_HCLKEN                      (1 << 11)
#define PRCM_ISP_CLK_CTRL_ISP_M_HCLKEN                      (1 << 10)
#define PRCM_ISP_CLK_CTRL_ISP_CLKEN                         (1 << 9)
#define PRCM_ISP_CLK_CTRL_ISP_JPEGCLKEN                     (1 << 8)
#define PRCM_ISP_CLK_CTRL_ISP_JPEG_DIV_EN                   (1 << 4)
#define PRCM_ISP_CLK_CTRL_ISP_JPEG_DIV_MASK                 (0xF << 0)

// PRCM_SD_CLK_CTRL            
#define PRCM_SD_CLK_CTRL_SD_RSTN                            (1 << 12)
#define PRCM_SD_CLK_CTRL_SD_HCLKEN                          (1 << 9)
#define PRCM_SD_CLK_CTRL_SD_CLKEN                           (1 << 8)
#define PRCM_SD_CLK_CTRL_SD_DIV_EN                          (1 << 4)
#define PRCM_SD_CLK_CTRL_SD_DIV_MASK                        (0xF << 0)

// PRCM_ZSP_CLK_CTRL           
#define PRCM_ZSP_CLK_CTRL_ZSP_RSTN                          (1 << 14)
#define PRCM_ZSP_CLK_CTRL_ZSP_ARSTN                         (1 << 13)
#define PRCM_ZSP_CLK_CTRL_ZSP_SYS_RSTN                      (1 << 12)
#define PRCM_ZSP_CLK_CTRL_ZSP_CLKEN                         (1 << 9)
#define PRCM_ZSP_CLK_CTRL_ZSP_ACLKEN                        (1 << 8)

// PRCM_SPI_CLK_CTRL           
#define PRCM_SPI_CLK_CTRL_SPI3_RSTN                         (1 << 31)
#define PRCM_SPI_CLK_CTRL_SPI2_RSTN                         (1 << 30)
#define PRCM_SPI_CLK_CTRL_SPI1_RSTN                         (1 << 29)
#define PRCM_SPI_CLK_CTRL_SPI0_RSTN                         (1 << 28)
#define PRCM_SPI_CLK_CTRL_SPI3_PCLKEN                       (1 << 27)
#define PRCM_SPI_CLK_CTRL_SPI2_PCLKEN                       (1 << 26)
#define PRCM_SPI_CLK_CTRL_SPI1_PCLKEN                       (1 << 25)
#define PRCM_SPI_CLK_CTRL_SPI0_PCLKEN                       (1 << 24)
#define PRCM_SPI_CLK_CTRL_SPI3_CLKEN                        (1 << 23)
#define PRCM_SPI_CLK_CTRL_SPI2_CLKEN                        (1 << 22)
#define PRCM_SPI_CLK_CTRL_SPI1_CLKEN                        (1 << 21)
#define PRCM_SPI_CLK_CTRL_SPI0_CLKEN                        (1 << 20)
#define PRCM_SPI_CLK_CTRL_SPI2_DIV_EN                       (1 << 16)
#define PRCM_SPI_CLK_CTRL_SPI2_DIV_MASK                     (0x7 << 12)
#define PRCM_SPI_CLK_CTRL_SPI01_DIV_EN                      (1 << 8)
#define PRCM_SPI_CLK_CTRL_SPI01_DIV_MASK                    (0x3F << 0)

// PRCM_I2C_CLK_CTRL           
#define PRCM_I2C_CLK_CTRL_I2C3_RSTN                         (1 << 31)
#define PRCM_I2C_CLK_CTRL_I2C2_RSTN                         (1 << 30)
#define PRCM_I2C_CLK_CTRL_I2C1_RSTN                         (1 << 29)
#define PRCM_I2C_CLK_CTRL_I2C0_RSTN                         (1 << 28)
#define PRCM_I2C_CLK_CTRL_I2C3_PCLKEN                       (1 << 27)
#define PRCM_I2C_CLK_CTRL_I2C2_PCLKEN                       (1 << 26)
#define PRCM_I2C_CLK_CTRL_I2C1_PCLKEN                       (1 << 25)
#define PRCM_I2C_CLK_CTRL_I2C0_PCLKEN                       (1 << 24)
#define PRCM_I2C_CLK_CTRL_I2C3_CLKEN                        (1 << 23)
#define PRCM_I2C_CLK_CTRL_I2C2_CLKEN                        (1 << 22)
#define PRCM_I2C_CLK_CTRL_I2C1_CLKEN                        (1 << 21)
#define PRCM_I2C_CLK_CTRL_I2C0_CLKEN                        (1 << 20)
#define PRCM_I2C_CLK_CTRL_I2C_HS_DIV_EN                     (1 << 16)
#define PRCM_I2C_CLK_CTRL_I2C_HS_DIV_MASK                   (0xF << 12)
#define PRCM_I2C_CLK_CTRL_I2C_FS_DIV_EN                     (1 << 8)
#define PRCM_I2C_CLK_CTRL_I2C_FS_DIV_MASK                   (0x3F << 0)

// PRCM_UART_CLK_CTRL0         
#define PRCM_UART_CLK_CTRL0_UART3_DIV_EN                    (1 << 21)
#define PRCM_UART_CLK_CTRL0_UART3_DIV_MASK                  (0x1F << 16)
#define PRCM_UART_CLK_CTRL0_UART2_DIV_EN                    (1 << 13)
#define PRCM_UART_CLK_CTRL0_UART2_DIV_MASK                  (0x1F << 8)
#define PRCM_UART_CLK_CTRL0_UART01_DIV_EN                   (1 << 5)
#define PRCM_UART_CLK_CTRL0_UART01_DIV_MASK                 (0x1F << 0)

// PRCM_UART_CLK_CTRL1         
#define PRCM_UART_CLK_CTRL1_UART3_RSTN                      (1 << 11)
#define PRCM_UART_CLK_CTRL1_UART2_RSTN                      (1 << 10)
#define PRCM_UART_CLK_CTRL1_UART1_RSTN                      (1 << 9)
#define PRCM_UART_CLK_CTRL1_UART0_RSTN                      (1 << 8)
#define PRCM_UART_CLK_CTRL1_UART3_PCLKEN                    (1 << 7)
#define PRCM_UART_CLK_CTRL1_UART2_PCLKEN                    (1 << 6)
#define PRCM_UART_CLK_CTRL1_UART1_PCLKEN                    (1 << 5)
#define PRCM_UART_CLK_CTRL1_UART0_PCLKEN                    (1 << 4)
#define PRCM_UART_CLK_CTRL1_UART3_CLKEN                     (1 << 3)
#define PRCM_UART_CLK_CTRL1_UART2_CLKEN                     (1 << 2)
#define PRCM_UART_CLK_CTRL1_UART1_CLKEN                     (1 << 1)
#define PRCM_UART_CLK_CTRL1_UART0_CLKEN                     (1 << 0)

// PRCM_DIS0_CLK_CTRL          
#define PRCM_DIS0_CLK_CTRL_LCDC0_RSTN                       (1 << 11)
#define PRCM_DIS0_CLK_CTRL_RGBO_INV_CLK_SEL                 (1 << 13)
#define PRCM_DIS0_CLK_CTRL_RGBO_CLKEN                       (1 << 12)
#define PRCM_DIS0_CLK_CTRL_LCDC0_HCLKEN                     (1 << 10)
#define PRCM_DIS0_CLK_CTRL_LCDC0_ACLKEN                     (1 << 9)
#define PRCM_DIS0_CLK_CTRL_LCDC0_CLKEN                      (1 << 8)
#define PRCM_DIS0_CLK_CTRL_PXL0_DIV_EN                      (1 << 4)
#define PRCM_DIS0_CLK_CTRL_PXL0_DIV_MASK                    (0xF << 0)

// PRCM_DIS1_CLK_CTRL          
#define PRCM_DIS1_CLK_CTRL_HDMI_RSTN                        (1 << 17)
#define PRCM_DIS1_CLK_CTRL_LCDC1_RSTN                       (1 << 16)
#define PRCM_DIS1_CLK_CTRL_HDMI_CEC_CLKEN                   (1 << 14)
#define PRCM_DIS1_CLK_CTRL_HDMI_BCLKEN                      (1 << 13)
#define PRCM_DIS1_CLK_CTRL_HDMI_PXL_CLKEN                   (1 << 12)
#define PRCM_DIS1_CLK_CTRL_LCDC1_HCLKEN                     (1 << 10)
#define PRCM_DIS1_CLK_CTRL_LCDC1_ACLKEN                     (1 << 9)
#define PRCM_DIS1_CLK_CTRL_LCDC1_CLKEN                      (1 << 8)
#define PRCM_DIS1_CLK_CTRL_PXL1_DIV_EN                      (1 << 4)
#define PRCM_DIS1_CLK_CTRL_PXL1_DIV_MASK                    (0xF << 0)

// PRCM_I2S_CLK_CTRL           
#define PRCM_I2S_CLK_CTRL_MCLKEN                            (1 << 16)
#define PRCM_I2S_CLK_CTRL_I2S1_RSTN                         (1 << 13)
#define PRCM_I2S_CLK_CTRL_I2S0_RSTN                         (1 << 12)
#define PRCM_I2S_CLK_CTRL_I2S1_PCLKEN                       (1 << 11)
#define PRCM_I2S_CLK_CTRL_I2S0_PCLKEN                       (1 << 10)
#define PRCM_I2S_CLK_CTRL_I2S1_CLKEN                        (1 << 9)
#define PRCM_I2S_CLK_CTRL_I2S0_CLKEN                        (1 << 8)
#define PRCM_I2S_CLK_CTRL_I2S_DIV_EN                        (1 << 4)
#define PRCM_I2S_CLK_CTRL_I2S_DIV_MASK                      (0xF << 0)

// PRCM_USB_CLK_CTRL           
#define PRCM_USB_CLK_CTRL_USB_RST_READY                     (0xF << 28)
#define PRCM_USB_CLK_CTRL_HSIC_PHY_RSTN                     (1 << 14)
#define PRCM_USB_CLK_CTRL_USB_PHY_RSTN_MASK                 (0x3 << 12)
#define PRCM_USB_CLK_CTRL_OTG_HRSTN                         (1 << 9)
#define PRCM_USB_CLK_CTRL_USB2_HRSTN                        (1 << 8)
#define PRCM_USB_CLK_CTRL_UTMI_PHY_CLKEN                    (1 << 5)
#define PRCM_USB_CLK_CTRL_OTG_HCLKEN                        (1 << 4)
#define PRCM_USB_CLK_CTRL_USB2_HCLKEN                       (1 << 3)
#define PRCM_USB_CLK_CTRL_USB_REF_CLKEN                     (1 << 2)
#define PRCM_USB_CLK_CTRL_OHCI_48M_CLKEN                    (1 << 1)
#define PRCM_USB_CLK_CTRL_HSIC_480M_CLKEN                   (1 << 0)

// PRCM_CORE_MEM_CLK_CTRL      
#define PRCM_CORE_MEM_CLK_CTRL_ROM_RSTN                     (1 << 4)
#define PRCM_CORE_MEM_CLK_CTRL_ROM_HCLKEN                   (1 << 0)

// PRCM_GPIO_CORE_CLK_CTRL     
#define PRCM_GPIO_CORE_CLK_CTRL_GPIO_CORE_RSTN              (1 << 4)
#define PRCM_GPIO_CORE_CLK_CTRL_GPIO_CORE_PCLKEN            (1 << 4)

// PRCM_TIMER_CORE_CLK_CTRL    
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER_CORE_RSTN_MASK       (0xFF << 20)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER_CORE_PCLKEN          (1 << 16)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER7_CORE_CLK_SEL        (1 << 15)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER6_CORE_CLK_SEL        (1 << 14)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER5_CORE_CLK_SEL        (1 << 13)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER4_CORE_CLK_SEL        (1 << 12)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER3_CORE_CLK_SEL        (1 << 11)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER2_CORE_CLK_SEL        (1 << 10)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER1_CORE_CLK_SEL        (1 << 9)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER0_CORE_CLK_SEL        (1 << 8)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER7_CORE_CLKEN          (1 << 7)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER6_CORE_CLKEN          (1 << 6)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER5_CORE_CLKEN          (1 << 5)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER4_CORE_CLKEN          (1 << 4)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER3_CORE_CLKEN          (1 << 3)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER2_CORE_CLKEN          (1 << 2)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER1_CORE_CLKEN          (1 << 1)
#define PRCM_TIMER_CORE_CLK_CTRL_TIMER0_CORE_CLKEN          (1 << 0)

// PRCM_WKUP_MEM_CLK_CTRL      
#define PRCM_WKUP_MEM_CLK_CTRL_RAM_WKUP_RSTN                (1 << 4)
#define PRCM_WKUP_MEM_CLK_CTRL_RAM_WKUP_HCLKEN              (1 << 0)

// PRCM_GPIO_WKUP_CLK_CTRL     
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_WKUP_RSTN              (1 << 17)
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_DBCLK_32K_DIV_EN       (1 << 16)
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_DBCLK_32K_DIV_MASK     (0xFF << 8)
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_WKUP_PCLK_SEL          (1 << 2)
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_DBCLKEN                (1 << 1)
#define PRCM_GPIO_WKUP_CLK_CTRL_GPIO_WKUP_PCLKEN            (1 << 0)

// PRCM_TIMER_WKUP_CLK_CTRL    
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER_WKUP_RSTN_MASK       (0x3 << 8)
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER1_WKUP_CLK_SEL        (1 << 5)
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER0_WKUP_CLK_SEL        (1 << 4)
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER_WKUP_PCLKEN          (1 << 2)
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER1_WKUP_CLKEN          (1 << 1)
#define PRCM_TIMER_WKUP_CLK_CTRL_TIMER0_WKUP_CLKEN          (1 << 0)

// PRCM_ALT_CLK_CTRL           
#define PRCM_ALT_CLK_CTRL_ALT1_CLKEN                        (1 << 29)
#define PRCM_ALT_CLK_CTRL_ALT0_CLKEN                        (1 << 28)
#define PRCM_ALT_CLK_CTRL_ALT1_32K_CLK_SEL                  (1 << 24)
#define PRCM_ALT_CLK_CTRL_ALT1_DIV_EN                       (1 << 23)
#define PRCM_ALT_CLK_CTRL_ALT1_DIV_MASK                     (0x7FF << 12)
#define PRCM_ALT_CLK_CTRL_ALT0_DIV_EN                       (1 << 11)
#define PRCM_ALT_CLK_CTRL_ALT0_DIV_MASK                     (0x7FF << 0)

// PRCM_AUX_CLK_CTRL           
#define PRCM_AUX_CLK_CTRL_AUX_PRSTN                         (1 << 5)
#define PRCM_AUX_CLK_CTRL_AUX_PCLKEN                        (1 << 4)
#define PRCM_AUX_CLK_CTRL_EFUSE_RSTN                        (1 << 3)
#define PRCM_AUX_CLK_CTRL_PWM_RSTN                          (1 << 2)
#define PRCM_AUX_CLK_CTRL_EFUSE_CLKEN                       (1 << 1)
#define PRCM_AUX_CLK_CTRL_PWM_CLKEN                         (1 << 0)


// Section-4: PRCM Configure Parameter Definition
// PLL configuration
// 50MHz
#define PRCM_PLL_NR_50MHZ                                          6
#define PRCM_PLL_NF_50MHZ                                          400
#define PRCM_PLL_OD_50MHZ                                          16  
#define PRCM_PLL_NB_50MHZ                                          200
// 200MHz
#define PRCM_PLL_NR_200MHZ                                         3
#define PRCM_PLL_NF_200MHZ                                         300
#define PRCM_PLL_OD_200MHZ                                         6   
#define PRCM_PLL_NB_200MHZ                                         150
// 250MHz
#define PRCM_PLL_NR_250MHZ                                         1
#define PRCM_PLL_NF_250MHZ                                         125
#define PRCM_PLL_OD_250MHZ                                         6   
#define PRCM_PLL_NB_250MHZ                                         62
// 297MHz
#define PRCM_PLL_NR_297MHZ                                         1
#define PRCM_PLL_NF_297MHZ                                         99
#define PRCM_PLL_OD_297MHZ                                         4   
#define PRCM_PLL_NB_297MHZ                                         50
// 300MHz
#define PRCM_PLL_NR_300MHZ                                         1
#define PRCM_PLL_NF_300MHZ                                         100
#define PRCM_PLL_OD_300MHZ                                         4   
#define PRCM_PLL_NB_300MHZ                                         50
// 350MHz
#define PRCM_PLL_NR_350MHZ                                         3
#define PRCM_PLL_NF_350MHZ                                         350
#define PRCM_PLL_OD_350MHZ                                         4   
#define PRCM_PLL_NB_350MHZ                                         175
// 400MHz
#define PRCM_PLL_NR_400MHZ                                         3
#define PRCM_PLL_NF_400MHZ                                         200
#define PRCM_PLL_OD_400MHZ                                         2   
#define PRCM_PLL_NB_400MHZ                                         100
// 450MHz
#define PRCM_PLL_NR_450MHZ                                         1
#define PRCM_PLL_NF_450MHZ                                         75
#define PRCM_PLL_OD_450MHZ                                         2   
#define PRCM_PLL_NB_450MHZ                                         38
// 480MHz
#define PRCM_PLL_NR_480MHZ                                         1
#define PRCM_PLL_NF_480MHZ                                         80
#define PRCM_PLL_OD_480MHZ                                         2   
#define PRCM_PLL_NB_480MHZ                                         40
// 500MHz
#define PRCM_PLL_NR_500MHZ                                         3
#define PRCM_PLL_NF_500MHZ                                         250
#define PRCM_PLL_OD_500MHZ                                         2   
#define PRCM_PLL_NB_500MHZ                                         125
// 533MHz
#define PRCM_PLL_NR_533MHZ                                         6
#define PRCM_PLL_NF_533MHZ                                         533
#define PRCM_PLL_OD_533MHZ                                         2   
#define PRCM_PLL_NB_533MHZ                                         266
// 550MHz
#define PRCM_PLL_NR_550MHZ                                         3
#define PRCM_PLL_NF_550MHZ                                         275
#define PRCM_PLL_OD_550MHZ                                         2   
#define PRCM_PLL_NB_550MHZ                                         138
// 600MHz
#define PRCM_PLL_NR_600MHZ                                         1
#define PRCM_PLL_NF_600MHZ                                         100
#define PRCM_PLL_OD_600MHZ                                         2   
#define PRCM_PLL_NB_600MHZ                                         50
// 650MHz
#define PRCM_PLL_NR_650MHZ                                         3
#define PRCM_PLL_NF_650MHZ                                         325
#define PRCM_PLL_OD_650MHZ                                         2   
#define PRCM_PLL_NB_650MHZ                                         162
// 666MHz
#define PRCM_PLL_NR_666MHZ                                         1
#define PRCM_PLL_NF_666MHZ                                         111
#define PRCM_PLL_OD_666MHZ                                         2   
#define PRCM_PLL_NB_666MHZ                                         56
// 700MHz
#define PRCM_PLL_NR_700MHZ                                         3
#define PRCM_PLL_NF_700MHZ                                         350
#define PRCM_PLL_OD_700MHZ                                         2   
#define PRCM_PLL_NB_700MHZ                                         175
// 720MHz 
#define PRCM_PLL_NR_720MHZ                                         1 
#define PRCM_PLL_NF_720MHZ                                         120 
#define PRCM_PLL_OD_720MHZ                                         2    
#define PRCM_PLL_NB_720MHZ                                         60 
// 750MHz
#define PRCM_PLL_NR_750MHZ                                         1
#define PRCM_PLL_NF_750MHZ                                         125
#define PRCM_PLL_OD_750MHZ                                         2   
#define PRCM_PLL_NB_750MHZ                                         62
// 800MHz
#define PRCM_PLL_NR_800MHZ                                         3
#define PRCM_PLL_NF_800MHZ                                         200
#define PRCM_PLL_OD_800MHZ                                         1   
#define PRCM_PLL_NB_800MHZ                                         100
// 850MHz
#define PRCM_PLL_NR_850MHZ                                         6
#define PRCM_PLL_NF_850MHZ                                         425
#define PRCM_PLL_OD_850MHZ                                         1   
#define PRCM_PLL_NB_850MHZ                                         212
// 891MHz
#define PRCM_PLL_NR_891MHZ                                         4
#define PRCM_PLL_NF_891MHZ                                         297
#define PRCM_PLL_OD_891MHZ                                         1   
#define PRCM_PLL_NB_891MHZ                                         148
// 900MHz
#define PRCM_PLL_NR_900MHZ                                         1
#define PRCM_PLL_NF_900MHZ                                         75
#define PRCM_PLL_OD_900MHZ                                         1   
#define PRCM_PLL_NB_900MHZ                                         38
// 950MHz
#define PRCM_PLL_NR_950MHZ                                         6
#define PRCM_PLL_NF_950MHZ                                         475
#define PRCM_PLL_OD_950MHZ                                         1   
#define PRCM_PLL_NB_950MHZ                                         238
// 983.04MHz
#define PRCM_PLL_NR_983M04HZ                                       25
#define PRCM_PLL_NF_983M04HZ                                       2048
#define PRCM_PLL_OD_983M04HZ                                       1   
#define PRCM_PLL_NB_983M04HZ                                       1024
// 1GHz
#define PRCM_PLL_NR_1000MHZ                                        3
#define PRCM_PLL_NF_1000MHZ                                        250
#define PRCM_PLL_OD_1000MHZ                                        1   
#define PRCM_PLL_NB_1000MHZ                                        125
// 1.05GHz
#define PRCM_PLL_NR_1050MHZ                                        2
#define PRCM_PLL_NF_1050MHZ                                        175
#define PRCM_PLL_OD_1050MHZ                                        1   
#define PRCM_PLL_NB_1050MHZ                                        88
// 1.1GHz
#define PRCM_PLL_NR_1100MHZ                                        3
#define PRCM_PLL_NF_1100MHZ                                        275
#define PRCM_PLL_OD_1100MHZ                                        1   
#define PRCM_PLL_NB_1100MHZ                                        138
// 1.15GHz
#define PRCM_PLL_NR_1150MHZ                                        6
#define PRCM_PLL_NF_1150MHZ                                        575
#define PRCM_PLL_OD_1150MHZ                                        1   
#define PRCM_PLL_NB_1150MHZ                                        288
// 1.2GHz
#define PRCM_PLL_NR_1200MHZ                                        1
#define PRCM_PLL_NF_1200MHZ                                        100
#define PRCM_PLL_OD_1200MHZ                                        1   
#define PRCM_PLL_NB_1200MHZ                                        50
// 1.25GHz
#define PRCM_PLL_NR_1250MHZ                                        6
#define PRCM_PLL_NF_1250MHZ                                        625
#define PRCM_PLL_OD_1250MHZ                                        1   
#define PRCM_PLL_NB_1250MHZ                                        312
// 1.3GHz
#define PRCM_PLL_NR_1300MHZ                                        3
#define PRCM_PLL_NF_1300MHZ                                        325
#define PRCM_PLL_OD_1300MHZ                                        1   
#define PRCM_PLL_NB_1300MHZ                                        162
// 1.35GHz
#define PRCM_PLL_NR_1350MHZ                                        2
#define PRCM_PLL_NF_1350MHZ                                        225
#define PRCM_PLL_OD_1350MHZ                                        1   
#define PRCM_PLL_NB_1350MHZ                                        112
// 1.4GHz
#define PRCM_PLL_NR_1400MHZ                                        3
#define PRCM_PLL_NF_1400MHZ                                        350
#define PRCM_PLL_OD_1400MHZ                                        1   
#define PRCM_PLL_NB_1400MHZ                                        175
// 1.45GHz
#define PRCM_PLL_NR_1450MHZ                                        6
#define PRCM_PLL_NF_1450MHZ                                        725
#define PRCM_PLL_OD_1450MHZ                                        1   
#define PRCM_PLL_NB_1450MHZ                                        362
// 1.5GHz
#define PRCM_PLL_NR_1500MHZ                                        1
#define PRCM_PLL_NF_1500MHZ                                        125
#define PRCM_PLL_OD_1500MHZ                                        1   
#define PRCM_PLL_NB_1500MHZ                                        62

// LCDC used
//25.175MHz*2
#define PRCM_PLL_NR_50M35HZ                                        15
#define PRCM_PLL_NF_50M35HZ                                        1007
#define PRCM_PLL_OD_50M35HZ                                        16   
#define PRCM_PLL_NB_50M35HZ                                        504
//29.232MHz*2
#define PRCM_PLL_NR_58M464HZ                                       25
#define PRCM_PLL_NF_58M464HZ                                       1218
#define PRCM_PLL_OD_58M464HZ                                       10
#define PRCM_PLL_NB_58M464HZ                                       609
//33.264MHz*2
#define PRCM_PLL_NR_66M528HZ                                       25
#define PRCM_PLL_NF_66M528HZ                                       1386
#define PRCM_PLL_OD_66M528HZ                                       10
#define PRCM_PLL_NB_66M528HZ                                       693
//40.000MHz*2
#define PRCM_PLL_NR_80MHZ                                          3
#define PRCM_PLL_NF_80MHZ                                          329
#define PRCM_PLL_OD_80MHZ                                          16
#define PRCM_PLL_NB_80MHZ                                          160
//51.2MHz*2
#define PRCM_PLL_NR_102M4HZ                                        15
#define PRCM_PLL_NF_102M4HZ                                        1792
#define PRCM_PLL_OD_102M4HZ                                        14
#define PRCM_PLL_NB_102M4HZ                                        896
//51.2064MHz*2
#define PRCM_PLL_NR_102M4128HZ                                     27
#define PRCM_PLL_NF_102M4128HZ                                     3226
#define PRCM_PLL_OD_102M4128HZ                                     14
#define PRCM_PLL_NB_102M4128HZ                                     1613
//65.000MHz*2
#define PRCM_PLL_NR_130MHZ                                         1
#define PRCM_PLL_NF_130MHZ                                         65
#define PRCM_PLL_OD_130MHZ                                         6
#define PRCM_PLL_NB_130MHZ                                         32
//85.500MHz*2
#define PRCM_PLL_NR_171MHZ                                         1
#define PRCM_PLL_NF_171MHZ                                         114
#define PRCM_PLL_OD_171MHZ                                         8   
#define PRCM_PLL_NB_171MHZ                                         57
//85.506MHz*2
#define PRCM_PLL_NR_171M012HZ                                      47
#define PRCM_PLL_NF_171M012HZ                                      4019
#define PRCM_PLL_OD_171M012HZ                                      6
#define PRCM_PLL_NB_171M012HZ                                      2010
//88.750MHz*2
#define PRCM_PLL_NR_177M5HZ                                        3
#define PRCM_PLL_NF_177M5HZ                                        355
#define PRCM_PLL_OD_177M5HZ                                        8  
#define PRCM_PLL_NB_177M5HZ                                        178
//106.507MHz*2
#define PRCM_PLL_NR_213M014HZ                                      37
#define PRCM_PLL_NF_213M014HZ                                      3941
#define PRCM_PLL_OD_213M014HZ                                      6
#define PRCM_PLL_NB_213M014HZ                                      1970
//106.5MHz*2
#define PRCM_PLL_NR_213MHZ                                         1
#define PRCM_PLL_NF_213MHZ                                         71
#define PRCM_PLL_OD_213MHZ                                         4
#define PRCM_PLL_NB_213MHZ                                         36
//162.000MHz*2
#define PRCM_PLL_NR_324MHZ                                         1
#define PRCM_PLL_NF_324MHZ                                         108
#define PRCM_PLL_OD_324MHZ                                         4   
#define PRCM_PLL_NB_324MHZ                                         54
//119.000MHz*2
#define PRCM_PLL_NR_238MHZ                                         1
#define PRCM_PLL_NF_238MHZ                                         119 
#define PRCM_PLL_OD_238MHZ                                         6   
#define PRCM_PLL_NB_238MHZ                                         60
//43.750MHz*2
#define PRCM_PLL_NR_87M5HZ                                         2
#define PRCM_PLL_NF_87M5HZ                                         175
#define PRCM_PLL_OD_87M5HZ                                         12
#define PRCM_PLL_NB_87M5HZ                                         88
//50.4MHz*2
#define PRCM_PLL_NR_100M8HZ                                        5
#define PRCM_PLL_NF_100M8HZ                                        588
#define PRCM_PLL_OD_100M8HZ                                        14
#define PRCM_PLL_NB_100M8HZ                                        294
//148.50MHz*2
//#define PRCM_PLL_NR_297MHZ                                        1
//#define PRCM_PLL_NF_297MHZ                                        99
//#define PRCM_PLL_OD_297MHZ                                        4   
//#define PRCM_PLL_NB_297MHZ                                       50
//74.250MHz*2
#define PRCM_PLL_NR_148M5HZ                                        1
#define PRCM_PLL_NF_148M5HZ                                        99
#define PRCM_PLL_OD_148M5HZ                                        8 
#define PRCM_PLL_NB_148M5HZ                                        50 
//83.50MHz*2
#define PRCM_PLL_NR_167MHZ                                         2
#define PRCM_PLL_NF_167MHZ                                         167
#define PRCM_PLL_OD_167MHZ                                         6  
#define PRCM_PLL_NB_167MHZ                                         84 
//71.000MHz*2
#define PRCM_PLL_NR_142MHZ                                          1
#define PRCM_PLL_NF_142MHZ                                          71
#define PRCM_PLL_OD_142MHZ                                          6 
#define PRCM_PLL_NB_142MHZ                                          36 
//72.432MHz*2 (calc)
#define PRCM_PLL_NR_144M864HZ                                       25
#define PRCM_PLL_NF_144M864HZ                                       3018
#define PRCM_PLL_OD_144M864HZ                                       10
#define PRCM_PLL_NB_144M864HZ                                       1500
//72MHz*2
#define PRCM_PLL_NR_144MHZ                                          1
#define PRCM_PLL_NF_144MHZ                                          120
#define PRCM_PLL_OD_144MHZ                                          10
#define PRCM_PLL_NB_144MHZ                                          60
//83.001MHz*2 (calc)
#define PRCM_PLL_NR_166M002HZ                                       1
#define PRCM_PLL_NF_166M002HZ                                       83
#define PRCM_PLL_OD_166M002HZ                                       6
#define PRCM_PLL_NB_166M002HZ                                       42
//65.002MHz*2 (calc)
#define PRCM_PLL_NR_130M004HZ                                       1
#define PRCM_PLL_NF_130M004HZ                                       65
#define PRCM_PLL_OD_130M004HZ                                       6
#define PRCM_PLL_NB_130M004HZ                                       32
//146.028MHz*2 (calc)
#define PRCM_PLL_NR_292M5HZ                                         2
#define PRCM_PLL_NF_292M5HZ                                         195
#define PRCM_PLL_OD_292M5HZ                                         4
#define PRCM_PLL_NB_292M5HZ                                         98
//162.022MHz*2 (calc)
#define PRCM_PLL_NR_324M044HZ                                       37
#define PRCM_PLL_NF_324M044HZ                                       3997
#define PRCM_PLL_OD_324M044HZ                                       4
#define PRCM_PLL_NB_324M044HZ                                       1998

//28.000MHz*2
#define PRCM_PLL_NR_56MHZ                                          3
#define PRCM_PLL_NF_56MHZ                                          224
#define PRCM_PLL_OD_56MHZ                                          16   
#define PRCM_PLL_NB_56MHZ                                          112
// Section-5: PRCM Task/Function(s) Prototype Definition

extern void prcm_glb_soft_reset(void);
extern void cpu_reset_all(void);

extern struct clk;

extern void reg_clr_bits(void __iomem *addr, unsigned int mask);
extern void reg_set_bits(void __iomem *addr, unsigned int set);

extern int tl7689_coresight_clk_enable(struct clk *clk);
extern int tl7689_coresight_clk_disable(struct clk *clk);
extern int tl7689_ddr_clk_enable(struct clk *clk);
extern int tl7689_ddr_clk_disable(struct clk *clk);
extern int tl7689_pl330dma_clk_enable(struct clk *clk);
extern int tl7689_pl330dma_clk_disable(struct clk *clk);
extern int tl7689_mali_clk_enable(struct clk *clk);
extern int tl7689_mali_clk_disable(struct clk *clk);
extern int tl7689_gc300_clk_enable(struct clk *clk);
extern int tl7689_gc300_clk_disable(struct clk *clk);
extern int tl7689_vpu_g1_clk_enable(struct clk *clk);
extern int tl7689_vpu_g1_clk_disable(struct clk *clk);
extern int tl7689_vpu_h1_clk_enable(struct clk *clk);
extern int tl7689_vpu_h1_clk_disable(struct clk *clk);
extern int tl7689_isp_jpeg_clk_enable(struct clk *clk);
extern int tl7689_isp_jpeg_clk_disable(struct clk *clk);
extern int tl7689_isp_clk_enable(struct clk *clk);
extern int tl7689_isp_clk_disable(struct clk *clk);
extern int tl7689_sd_clk_enable(struct clk *clk);
extern int tl7689_sd_clk_disable(struct clk *clk);
extern int tl7689_zsp_clk_enable(struct clk *clk);
extern int tl7689_zsp_clk_disable(struct clk *clk);
extern int tl7689_spi0_clk_enable(struct clk *clk);
extern int tl7689_spi0_clk_disable(struct clk *clk);
extern int tl7689_spi1_clk_enable(struct clk *clk);
extern int tl7689_spi1_clk_disable(struct clk *clk);
extern int tl7689_spi2_clk_enable(struct clk *clk);
extern int tl7689_spi2_clk_disable(struct clk *clk);
extern int tl7689_spi3_clk_enable(struct clk *clk);
extern int tl7689_spi3_clk_disable(struct clk *clk);
extern int tl7689_i2c0_clk_enable(struct clk *clk);
extern int tl7689_i2c0_clk_disable(struct clk *clk);
extern int tl7689_i2c1_clk_enable(struct clk *clk);
extern int tl7689_i2c1_clk_disable(struct clk *clk);
extern int tl7689_i2c2_clk_enable(struct clk *clk);
extern int tl7689_i2c2_clk_disable(struct clk *clk);
extern int tl7689_i2c3_clk_enable(struct clk *clk);
extern int tl7689_i2c3_clk_disable(struct clk *clk);
extern int tl7689_uart0_clk_enable(struct clk *clk);
extern int tl7689_uart0_clk_disable(struct clk *clk);
extern int tl7689_uart1_clk_enable(struct clk *clk);
extern int tl7689_uart1_clk_disable(struct clk *clk);
extern int tl7689_uart2_clk_enable(struct clk *clk);
extern int tl7689_uart2_clk_disable(struct clk *clk);
extern int tl7689_uart3_clk_enable(struct clk *clk);
extern int tl7689_uart3_clk_disable(struct clk *clk);
extern int tl7689_rgbo_clk_enable(struct clk *clk);
extern int tl7689_rgbo_clk_disable(struct clk *clk);
extern int tl7689_lcdc0_clk_enable(struct clk *clk);
extern int tl7689_lcdc0_clk_disable(struct clk *clk);
extern int tl7689_lcdc0_bus_clk_enable(struct clk *clk);
extern int tl7689_lcdc0_bus_clk_disable(struct clk *clk);
extern int tl7689_lcdc1_clk_enable(struct clk *clk);
extern int tl7689_lcdc1_clk_disable(struct clk *clk);
extern int tl7689_lcdc1_bus_clk_enable(struct clk *clk);
extern int tl7689_lcdc1_bus_clk_disable(struct clk *clk);
extern int tl7689_hdmi_pxl_clk_enable(struct clk *clk);
extern int tl7689_hdmi_pxl_clk_disable(struct clk *clk);
extern int tl7689_i2s0_clk_enable(struct clk *clk);
extern int tl7689_i2s0_clk_disable(struct clk *clk);
extern int tl7689_i2s1_clk_enable(struct clk *clk);
extern int tl7689_i2s1_clk_disable(struct clk *clk);
extern int tl7689_mclk_clk_enable(struct clk *clk);
extern int tl7689_mclk_clk_disable(struct clk *clk);
extern int tl7689_usb_otg_clk_enable(struct clk *clk);
extern int tl7689_usb_otg_clk_disable(struct clk *clk);
extern int tl7689_usb_utmi_clk_enable(struct clk *clk);
extern int tl7689_usb_utmi_clk_disable(struct clk *clk);
extern int tl7689_usb_ohci_clk_enable(struct clk *clk);
extern int tl7689_usb_ohci_clk_disable(struct clk *clk);
extern int tl7689_usb_hsic_clk_enable(struct clk *clk);
extern int tl7689_usb_hsic_clk_disable(struct clk *clk);
extern int tl7689_core_mem_clk_enable(struct clk *clk);
extern int tl7689_core_mem_clk_disable(struct clk *clk);
extern int tl7689_gpio_core_clk_enable(struct clk *clk);
extern int tl7689_gpio_core_clk_disable(struct clk *clk);
extern int tl7689_timer0_core_clk_enable(struct clk *clk);
extern int tl7689_timer0_core_clk_disable(struct clk *clk);
extern int tl7689_timer1_core_clk_enable(struct clk *clk);
extern int tl7689_timer1_core_clk_disable(struct clk *clk);
extern int tl7689_timer2_core_clk_enable(struct clk *clk);
extern int tl7689_timer2_core_clk_disable(struct clk *clk);
extern int tl7689_timer3_core_clk_enable(struct clk *clk);
extern int tl7689_timer3_core_clk_disable(struct clk *clk);
extern int tl7689_timer4_core_clk_enable(struct clk *clk);
extern int tl7689_timer4_core_clk_disable(struct clk *clk);
extern int tl7689_timer5_core_clk_enable(struct clk *clk);
extern int tl7689_timer5_core_clk_disable(struct clk *clk);
extern int tl7689_timer6_core_clk_enable(struct clk *clk);
extern int tl7689_timer6_core_clk_disable(struct clk *clk);
extern int tl7689_timer7_core_clk_enable(struct clk *clk);
extern int tl7689_timer7_core_clk_disable(struct clk *clk);
extern int tl7689_wkup_mem_clk_enable(struct clk *clk);
extern int tl7689_wkup_mem_clk_disable(struct clk *clk);
extern int tl7689_gpio_wkup_clk_enable(struct clk *clk);
extern int tl7689_gpio_wkup_clk_disable(struct clk *clk);
extern int tl7689_timer0_wkup_clk_enable(struct clk *clk);
extern int tl7689_timer0_wkup_clk_disable(struct clk *clk);
extern int tl7689_timer1_wkup_clk_enable(struct clk *clk);
extern int tl7689_timer1_wkup_clk_disable(struct clk *clk);
extern int tl7689_alt0_clk_enable(struct clk *clk);
extern int tl7689_alt0_clk_disable(struct clk *clk);
extern int tl7689_alt1_clk_enable(struct clk *clk);
extern int tl7689_alt1_clk_disable(struct clk *clk);
extern int tl7689_aux_clk_enable(struct clk *clk);
extern int tl7689_aux_clk_disable(struct clk *clk);
extern int tl7689_efuse_clk_enable(struct clk *clk);
extern int tl7689_efuse_clk_disable(struct clk *clk);
extern int tl7689_pwm_clk_enable(struct clk *clk);
extern int tl7689_pwm_clk_disable(struct clk *clk);
extern int tl7689_coresight_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll0_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll1_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll2_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll3_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll4_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pll5_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_ddr_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_mali_pll2_set_rate(unsigned long rate);
extern int tl7689_mali_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_gc300_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_vpu_g1_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_vpu_h1_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_isp_jpeg_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_sd_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_spi01_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_spi23_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_i2c_fs_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_i2c_hs_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_uart01_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_uart2_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_uart3_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pxl0_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_pxl1_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_i2s_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer0_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer1_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer2_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer3_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer4_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer5_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer6_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer7_core_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_gpio_wkup_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer0_wkup_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_timer1_wkup_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_alt0_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_alt1_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);
extern int tl7689_cpu_clk_set_rate(struct clk *c, unsigned long rate, unsigned int nouse);

#endif //PRCM_H__
