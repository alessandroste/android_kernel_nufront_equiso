/* arch/arm/mach-tl7689/include/mach/dma.h
 *
 * Copyright (C) 2003-2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Nusmart DMA support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef  _DMA_H_
#define _DMA_H_

enum tl7689_dma_buffresult {
    TL7689_RES_OK,
    TL7689_RES_ERR,
    TL7689_RES_ABORT
};

enum tl7689_dmasrc {
    TL7689_DMASRC_HW,		/* source is memory */
    TL7689_DMASRC_MEM		/* source is hardware */
};

/* enum tl7689_chan_op
 *
 * operation codes passed to the DMA code by the user, and also used
 * to inform the current channel owner of any changes to the system state
 */

enum tl7689_chan_op {
    TL7689_DMAOP_START,
    TL7689_DMAOP_STOP,
    TL7689_DMAOP_PAUSE,
    TL7689_DMAOP_RESUME,
    TL7689_DMAOP_FLUSH,
    TL7689_DMAOP_TIMEOUT,		/* internal signal to handler */
    TL7689_DMAOP_STARTED,		/* indicate channel started */
    TL7689_DMAOP_NONE,
};

struct tl7689_dma_client {
    char                *name;
};

struct tl7689_dma_chan;
enum dma_ch;

/* tl7689_dma_cbfn_t
 *
 * buffer callback routine type
 */

typedef void (*tl7689_dma_cbfn_t)(struct tl7689_dma_chan *,
        void *buf, int size,
        enum tl7689_dma_buffresult result);

typedef int  (*tl7689_dma_opfn_t)(struct tl7689_dma_chan *,
        enum tl7689_chan_op );



/* tl7689_dma_request
 *
 * request a dma channel exclusivley
 */

extern int tl7689_dma_request(enum dma_ch channel,
        struct tl7689_dma_client *, void *dev);


/* tl7689_dma_ctrl
 *
 * change the state of the dma channel
 */

extern int tl7689_dma_ctrl(enum dma_ch channel, enum tl7689_chan_op op);

/* tl7689_dma_setflags
 *
 * set the channel's flags to a given state
 */

extern int tl7689_dma_setflags(enum dma_ch channel,
        unsigned int flags);

/* tl7689_dma_free
 *
 * free the dma channel (will also abort any outstanding operations)
 */

extern int tl7689_dma_free(enum dma_ch channel, struct tl7689_dma_client *);

/* tl7689_dma_enqueue
 *
 * place the given buffer onto the queue of operations for the channel.
 * The buffer must be allocated from dma coherent memory, or the Dcache/WB
 * drained before the buffer is given to the DMA system.
 */

extern int tl7689_dma_enqueue(enum dma_ch channel, void *id,
        dma_addr_t data, int size);

/* tl7689_dma_config
 *
 * configure the dma channel
 */

extern int tl7689_dma_config(enum dma_ch channel, int xferunit);

/* tl7689_dma_devconfig
 *
 * configure the device we're talking to
 */

extern int tl7689_dma_devconfig(enum dma_ch channel,
        enum tl7689_dmasrc source, unsigned long devaddr);

/* tl7689_dma_getposition
 *
 * get the position that the dma transfer is currently at
 */

extern int tl7689_dma_getposition(enum dma_ch channel,
        dma_addr_t *src, dma_addr_t *dest);

extern int tl7689_dma_set_opfn(enum dma_ch, tl7689_dma_opfn_t rtn);
extern int tl7689_dma_set_buffdone_fn(enum dma_ch, tl7689_dma_cbfn_t rtn);

#endif
