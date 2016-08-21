/* linux/arch/arm/mach-tl7689/include/mach/dma-tl7689.h
 *
 * Copyright (C) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Nufront DMA support - per SoC functions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <mach/dma-core.h>

extern struct sysdev_class dma_sysclass;
extern struct tl7689_dma_chan tl7689_chans[S3C_DMA_CHANNELS];

#define DMA_CH_VALID		(1<<31)
#define DMA_CH_NEVER		(1<<30)

struct tl7689_dma_addr {
    unsigned long		from;
    unsigned long		to;
};

/* struct tl7689_dma_map
 *
 * this holds the mapping information for the channel selected
 * to be connected to the specified device
 */

struct tl7689_dma_map {
    const char		*name;
    struct tl7689_dma_addr  hw_addr;

    unsigned long		 channels[S3C_DMA_CHANNELS];
    unsigned long		 channels_rx[S3C_DMA_CHANNELS];
};

struct tl7689_dma_selection {
    struct tl7689_dma_map	*map;
    unsigned long		 map_size;
    unsigned long		 dcon_mask;

    void	(*select)(struct tl7689_dma_chan *chan,
            struct tl7689_dma_map *map);

    void	(*direction)(struct tl7689_dma_chan *chan,
            struct tl7689_dma_map *map,
            enum tl7689_dmasrc dir);
};

extern int tl7689_dma_init_map(struct tl7689_dma_selection *sel);

/* struct tl7689_dma_order_ch
 *
 * channel map for one of the `enum dma_ch` dma channels. the list
 * entry contains a set of low-level channel numbers, orred with
 * DMA_CH_VALID, which are checked in the order in the array.
 */

struct tl7689_dma_order_ch {
    unsigned int	list[S3C_DMA_CHANNELS];	/* list of channels */
    unsigned int	flags;				/* flags */
};

/* struct tl7689_dma_order
 *
 * information provided by either the core or the board to give the
 * dma system a hint on how to allocate channels
 */

struct tl7689_dma_order {
    struct tl7689_dma_order_ch	channels[DMACH_MAX];
};

extern int tl7689_dma_order_set(struct tl7689_dma_order *map);

/* DMA init code, called from the cpu support code */

extern int tl7689_dma_init(void);

extern int tl7689_dma_init(unsigned int channels, unsigned int irq,
        unsigned int stride);
