/* linux/arch/arm/mach-tl7689/nusmart-pl330.c
 *
 * Copyright (C) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <asm/hardware/pl330.h>

#include <mach/nusmart-pl330-pdata.h>

/**
 * struct nusmart_pl330_dmac - Logical representation of a PL330 DMAC.
 * @busy_chan: Number of channels currently busy.
 * @peri: List of IDs of peripherals this DMAC can work with.
 * @node: To attach to the global list of DMACs.
 * @pi: PL330 configuration info for the DMAC.
 * @kmcache: Pool to quickly allocate xfers for all channels in the dmac.
 * @clk: Pointer of DMAC operation clock.
 */
struct nusmart_pl330_dmac {
    unsigned		busy_chan;
    enum dma_ch		*peri;
    struct list_head	node;
    struct pl330_info	*pi;
    struct kmem_cache	*kmcache;
    struct clk		*clk;
};

/**
 * struct nusmart_pl330_xfer - A request submitted by NUSMART DMA clients.
 * @token: Xfer ID provided by the client.
 * @node: To attach to the list of xfers on a channel.
 * @px: Xfer for PL330 core.
 * @chan: Owner channel of this xfer.
 */
struct nusmart_pl330_xfer {
    void			*token;
    struct list_head	node;
    struct pl330_xfer	px;
    struct nusmart_pl330_chan	*chan;
};

/**
 * struct nusmart_pl330_chan - Logical channel to communicate with
 * 	a Physical peripheral.
 * @pl330_chan_id: Token of a hardware channel thread of PL330 DMAC.
 * 	NULL if the channel is available to be acquired.
 * @id: ID of the peripheral that this channel can communicate with.
 * @options: Options specified by the client.
 * @sdaddr: Address provided via tl7689_dma_devconfig.
 * @node: To attach to the global list of channels.
 * @lrq: Pointer to the last submitted pl330_req to PL330 core.
 * @xfer_list: To manage list of xfers enqueued.
 * @req: Two requests to communicate with the PL330 engine.
 * @callback_fn: Callback function to the client.
 * @rqcfg: Channel configuration for the xfers.
 * @xfer_head: Pointer to the xfer to be next executed.
 * @dmac: Pointer to the DMAC that manages this channel, NULL if the
 * 	channel is available to be acquired.
 * @client: Client of this channel. NULL if the
 * 	channel is available to be acquired.
 */
struct nusmart_pl330_chan {
    void				*pl330_chan_id;
    enum dma_ch			id;
    unsigned int			options;
    unsigned long			sdaddr;
    struct list_head		node;
    struct pl330_req		*lrq;
    struct list_head		xfer_list;
    struct pl330_req		req[2];
    tl7689_dma_cbfn_t		callback_fn;
    struct pl330_reqcfg		rqcfg;
    struct nusmart_pl330_xfer		*xfer_head;
    struct nusmart_pl330_dmac		*dmac;
    struct tl7689_dma_client	*client;
};

/* All DMACs in the platform */
static LIST_HEAD(dmac_list);

/* All channels to peripherals in the platform */
static LIST_HEAD(chan_list);

/*
 * Since we add resources(DMACs and Channels) to the global pool,
 * we need to guard access to the resources using a global lock
 */
static DEFINE_SPINLOCK(res_lock);

/* Returns the channel with ID 'id' in the chan_list */
static struct nusmart_pl330_chan *id_to_chan(const enum dma_ch id)
{
    struct nusmart_pl330_chan *ch;

    list_for_each_entry(ch, &chan_list, node)
        if (ch->id == id)
            return ch;

    return NULL;
}

/* Allocate a new channel with ID 'id' and add to chan_list */
static void chan_add(const enum dma_ch id)
{
    struct nusmart_pl330_chan *ch = id_to_chan(id);

    /* Return if the channel already exists */
    if (ch)
        return;

    ch = kmalloc(sizeof(*ch), GFP_KERNEL);
    /* Return silently to work with other channels */
    if (!ch)
        return;

    ch->id = id;
    ch->dmac = NULL;

    list_add_tail(&ch->node, &chan_list);
}

/* If the channel is not yet acquired by any client */
static bool chan_free(struct nusmart_pl330_chan *ch)
{
    if (!ch)
        return false;

    /* Channel points to some DMAC only when it's acquired */
    return ch->dmac ? false : true;
}

/*
 * Returns 0 is peripheral i/f is invalid or not present on the dmac.
 * Index + 1, otherwise.
 */
static unsigned iface_of_dmac(struct nusmart_pl330_dmac *dmac, enum dma_ch ch_id)
{
    enum dma_ch *id = dmac->peri;
    int i;

    /* Discount invalid markers */
    if (ch_id == DMACH_MAX)
        return 0;

    for (i = 0; i < PL330_MAX_PERI; i++)
        if (id[i] == ch_id)
            return i + 1;

    return 0;
}

/* If all channel threads of the DMAC are busy */
static inline bool dmac_busy(struct nusmart_pl330_dmac *dmac)
{
    struct pl330_info *pi = dmac->pi;

    return (dmac->busy_chan < pi->pcfg.num_chan) ? false : true;
}

/*
 * Returns the number of free channels that
 * can be handled by this dmac only.
 */
static unsigned ch_onlyby_dmac(struct nusmart_pl330_dmac *dmac)
{
    enum dma_ch *id = dmac->peri;
    struct nusmart_pl330_dmac *d;
    struct nusmart_pl330_chan *ch;
    unsigned found, count = 0;
    enum dma_ch p;
    int i;

    for (i = 0; i < PL330_MAX_PERI; i++) {
        p = id[i];
        ch = id_to_chan(p);

        if (p == DMACH_MAX || !chan_free(ch))
            continue;

        found = 0;
        list_for_each_entry(d, &dmac_list, node) {
            if (d != dmac && iface_of_dmac(d, ch->id)) {
                found = 1;
                break;
            }
        }
        if (!found)
            count++;
    }

    return count;
}

/*
 * Measure of suitability of 'dmac' handling 'ch'
 *
 * 0 indicates 'dmac' can not handle 'ch' either
 * because it is not supported by the hardware or
 * because all dmac channels are currently busy.
 *
 * >0 vlaue indicates 'dmac' has the capability.
 * The bigger the value the more suitable the dmac.
 */
#define MAX_SUIT	UINT_MAX
#define MIN_SUIT	0

static unsigned suitablility(struct nusmart_pl330_dmac *dmac,
        struct nusmart_pl330_chan *ch)
{
    struct pl330_info *pi = dmac->pi;
    enum dma_ch *id = dmac->peri;
    struct nusmart_pl330_dmac *d;
    unsigned s;
    int i;

    s = MIN_SUIT;
    /* If all the DMAC channel threads are busy */
    if (dmac_busy(dmac))
        return s;

    for (i = 0; i < PL330_MAX_PERI; i++)
        if (id[i] == ch->id)
            break;

    /* If the 'dmac' can't talk to 'ch' */
    if (i == PL330_MAX_PERI)
        return s;

    s = MAX_SUIT;
    list_for_each_entry(d, &dmac_list, node) {
        /*
         * If some other dmac can talk to this
         * peri and has some channel free.
         */
        if (d != dmac && iface_of_dmac(d, ch->id) && !dmac_busy(d)) {
            s = 0;
            break;
        }
    }
    if (s)
        return s;

    s = 100;

    /* Good if free chans are more, bad otherwise */
    s += (pi->pcfg.num_chan - dmac->busy_chan) - ch_onlyby_dmac(dmac);

    return s;
}

/* More than one DMAC may have capability to transfer data with the
 * peripheral. This function assigns most suitable DMAC to manage the
 * channel and hence communicate with the peripheral.
 */
static struct nusmart_pl330_dmac *map_chan_to_dmac(struct nusmart_pl330_chan *ch)
{
    struct nusmart_pl330_dmac *d, *dmac = NULL;
    unsigned sn, sl = MIN_SUIT;

    list_for_each_entry(d, &dmac_list, node) {
        sn = suitablility(d, ch);

        if (sn == MAX_SUIT)
            return d;

        if (sn > sl)
            dmac = d;
    }

    return dmac;
}

/* Acquire the channel for peripheral 'id' */
static struct nusmart_pl330_chan *chan_acquire(const enum dma_ch id)
{
    struct nusmart_pl330_chan *ch = id_to_chan(id);
    struct nusmart_pl330_dmac *dmac;

    /* If the channel doesn't exist or is already acquired */
    if (!ch || !chan_free(ch)) {
        ch = NULL;
        printk(KERN_EMERG "the channel doesn't exist or is already acquired\n");
        goto acq_exit;
    }

    dmac = map_chan_to_dmac(ch);
    /* If couldn't map */
    if (!dmac) {
        ch = NULL;
        printk(KERN_EMERG "the channel could not map\n");
        goto acq_exit;
    }

    dmac->busy_chan++;
    ch->dmac = dmac;

    //printk(KERN_EMERG "the channel is acquired, busy_chan is %d\n", dmac->busy_chan);
acq_exit:
    return ch;
}

/* Delete xfer from the queue */
static inline void del_from_queue(struct nusmart_pl330_xfer *xfer)
{
    struct nusmart_pl330_xfer *t;
    struct nusmart_pl330_chan *ch;
    int found;

    if (!xfer)
        return;

    ch = xfer->chan;

    /* Make sure xfer is in the queue */
    found = 0;
    list_for_each_entry(t, &ch->xfer_list, node)
        if (t == xfer) {
            found = 1;
            break;
        }

    if (!found)
        return;

    /* If xfer is last entry in the queue */
    if (xfer->node.next == &ch->xfer_list)
        t = list_entry(ch->xfer_list.next,
                struct nusmart_pl330_xfer, node);
    else
        t = list_entry(xfer->node.next,
                struct nusmart_pl330_xfer, node);

    /* If there was only one node left */
    if (t == xfer)
        ch->xfer_head = NULL;
    else if (ch->xfer_head == xfer)
        ch->xfer_head = t;

    list_del(&xfer->node);
}

/* Provides pointer to the next xfer in the queue.
 * If CIRCULAR option is set, the list is left intact,
 * otherwise the xfer is removed from the list.
 * Forced delete 'pluck' can be set to override the CIRCULAR option.
 */
static struct nusmart_pl330_xfer *get_from_queue(struct nusmart_pl330_chan *ch,
        int pluck)
{
    struct nusmart_pl330_xfer *xfer = ch->xfer_head;

    if (!xfer)
        return NULL;

    /* If xfer is last entry in the queue */
    if (xfer->node.next == &ch->xfer_list)
        ch->xfer_head = list_entry(ch->xfer_list.next,
                struct nusmart_pl330_xfer, node);
    else
        ch->xfer_head = list_entry(xfer->node.next,
                struct nusmart_pl330_xfer, node);

    if (pluck || !(ch->options & TL7689_DMAF_CIRCULAR))
        del_from_queue(xfer);

    return xfer;
}

static inline void add_to_queue(struct nusmart_pl330_chan *ch,
        struct nusmart_pl330_xfer *xfer, int front)
{
    struct pl330_xfer *xt;

    /* If queue empty */
    if (ch->xfer_head == NULL)
        ch->xfer_head = xfer;

    xt = &ch->xfer_head->px;
    /* If the head already submitted (CIRCULAR head) */
    if (ch->options & TL7689_DMAF_CIRCULAR &&
            (xt == ch->req[0].x || xt == ch->req[1].x))
        ch->xfer_head = xfer;

    /* If this is a resubmission, it should go at the head */
    if (front) {
        ch->xfer_head = xfer;
        list_add(&xfer->node, &ch->xfer_list);
    } else {
        list_add_tail(&xfer->node, &ch->xfer_list);
    }
}

static inline void _finish_off(struct nusmart_pl330_xfer *xfer,
        enum tl7689_dma_buffresult res, int ffree)
{
    struct nusmart_pl330_chan *ch;

    if (!xfer)
        return;

    ch = xfer->chan;

    /* Do callback */
    if (ch->callback_fn)
        ch->callback_fn(NULL, xfer->token, xfer->px.bytes, res);

    /* Force Free or if buffer is not needed anymore */
    if (ffree || !(ch->options & TL7689_DMAF_CIRCULAR))
        kmem_cache_free(ch->dmac->kmcache, xfer);
}

static inline int nusmart_pl330_submit(struct nusmart_pl330_chan *ch,
        struct pl330_req *r)
{
    struct nusmart_pl330_xfer *xfer;
    int ret = 0;

    /* If already submitted */
    if (r->x)
        return 0;

    xfer = get_from_queue(ch, 0);
    if (xfer) {
        r->x = &xfer->px;

        /* Use max bandwidth for M<->M xfers */
        if (r->rqtype == MEMTOMEM) {
            struct pl330_info *pi = xfer->chan->dmac->pi;
            int burst = 1 << ch->rqcfg.brst_size;
            u32 bytes = r->x->bytes;
            int bl;

            bl = pi->pcfg.data_bus_width / 8;
            bl *= pi->pcfg.data_buf_dep;
            bl /= burst;

            /* src/dst_burst_len can't be more than 16 */
            if (bl > 16)
                bl = 16;

            while (bl > 1) {
                if (!(bytes % (bl * burst)))
                    break;
                bl--;
            }

            ch->rqcfg.brst_len = bl;
        } else {
            ch->rqcfg.brst_len = 1;
        }

        ret = pl330_submit_req(ch->pl330_chan_id, r);

        /* If submission was successful */
        if (!ret) {
            ch->lrq = r; /* latest submitted req */
            return 0;
        }

        r->x = NULL;

        /* If both of the PL330 ping-pong buffers filled */
        if (ret == -EAGAIN) {
            dev_err(ch->dmac->pi->dev, "%s:%d!\n",
                    __func__, __LINE__);
            /* Queue back again */
            add_to_queue(ch, xfer, 1);
            ret = 0;
        } else {
            dev_err(ch->dmac->pi->dev, "%s:%d!\n",
                    __func__, __LINE__);
            _finish_off(xfer, TL7689_RES_ERR, 0);
        }
    }

    return ret;
}

static void nusmart_pl330_rq(struct nusmart_pl330_chan *ch,
        struct pl330_req *r, enum pl330_op_err err)
{
    unsigned long flags;
    struct nusmart_pl330_xfer *xfer;
    struct pl330_xfer *xl = r->x;
    enum tl7689_dma_buffresult res;

    spin_lock_irqsave(&res_lock, flags);

    r->x = NULL;

    nusmart_pl330_submit(ch, r);

    spin_unlock_irqrestore(&res_lock, flags);

    /* Map result to NUSMART DMA API */
    if (err == PL330_ERR_NONE)
        res = TL7689_RES_OK;
    else if (err == PL330_ERR_ABORT)
        res = TL7689_RES_ABORT;
    else
        res = TL7689_RES_ERR;

    /* If last request had some xfer */
    if (xl) {
        xfer = container_of(xl, struct nusmart_pl330_xfer, px);
        _finish_off(xfer, res, 0);
    } else {
        dev_info(ch->dmac->pi->dev, "%s:%d No Xfer?!\n",
                __func__, __LINE__);
    }
}

static void nusmart_pl330_rq0(void *token, enum pl330_op_err err)
{
    struct pl330_req *r = token;
    struct nusmart_pl330_chan *ch = container_of(r,
            struct nusmart_pl330_chan, req[0]);
    nusmart_pl330_rq(ch, r, err);
}

static void nusmart_pl330_rq1(void *token, enum pl330_op_err err)
{
    struct pl330_req *r = token;
    struct nusmart_pl330_chan *ch = container_of(r,
            struct nusmart_pl330_chan, req[1]);
    nusmart_pl330_rq(ch, r, err);
}

/* Release an acquired channel */
static void chan_release(struct nusmart_pl330_chan *ch)
{
    struct nusmart_pl330_dmac *dmac;

    if (chan_free(ch))
        return;

    dmac = ch->dmac;
    ch->dmac = NULL;
    dmac->busy_chan--;
}

int tl7689_dma_ctrl(enum dma_ch id, enum tl7689_chan_op op)
{
    struct nusmart_pl330_xfer *xfer;
    enum pl330_chan_op pl330op;
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int idx, ret;

    //printk(KERN_EMERG "tl7689_dma_ctrl with id=%d, op=%d\n", id, op);
    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch)) {
        ret = -EINVAL;
        goto ctrl_exit;
    }

    switch (op) {
        case TL7689_DMAOP_START:
            /* Make sure both reqs are enqueued */
            idx = (ch->lrq == &ch->req[0]) ? 1 : 0;
            nusmart_pl330_submit(ch, &ch->req[idx]);
            nusmart_pl330_submit(ch, &ch->req[1 - idx]);
            pl330op = PL330_OP_START;
            break;

        case TL7689_DMAOP_STOP:
            pl330op = PL330_OP_ABORT;
            break;

        case TL7689_DMAOP_FLUSH:
            pl330op = PL330_OP_FLUSH;
            break;

        case TL7689_DMAOP_PAUSE:
        case TL7689_DMAOP_RESUME:
        case TL7689_DMAOP_TIMEOUT:
        case TL7689_DMAOP_STARTED:
            spin_unlock_irqrestore(&res_lock, flags);
            return 0;

        default:
            spin_unlock_irqrestore(&res_lock, flags);
            return -EINVAL;
    }

    ret = pl330_chan_ctrl(ch->pl330_chan_id, pl330op);
    //printk(KERN_EMERG "***************pl330_chan_ctrl with ret = %d, id=%d, op=%d\n", ret, ch->pl330_chan_id, pl330op);

    if (pl330op == PL330_OP_START) {
        spin_unlock_irqrestore(&res_lock, flags);
        return ret;
    }

    idx = (ch->lrq == &ch->req[0]) ? 1 : 0;

    /* Abort the current xfer */
    if (ch->req[idx].x) {
        xfer = container_of(ch->req[idx].x,
                struct nusmart_pl330_xfer, px);

        /* Drop xfer during FLUSH */
        if (pl330op == PL330_OP_FLUSH)
            del_from_queue(xfer);

        ch->req[idx].x = NULL;

        spin_unlock_irqrestore(&res_lock, flags);
        _finish_off(xfer, TL7689_RES_ABORT,
                pl330op == PL330_OP_FLUSH ? 1 : 0);
        spin_lock_irqsave(&res_lock, flags);
    }

    /* Flush the whole queue */
    if (pl330op == PL330_OP_FLUSH) {

        if (ch->req[1 - idx].x) {
            xfer = container_of(ch->req[1 - idx].x,
                    struct nusmart_pl330_xfer, px);

            del_from_queue(xfer);

            ch->req[1 - idx].x = NULL;

            spin_unlock_irqrestore(&res_lock, flags);
            _finish_off(xfer, TL7689_RES_ABORT, 1);
            spin_lock_irqsave(&res_lock, flags);
        }

        /* Finish off the remaining in the queue */
        xfer = ch->xfer_head;
        while (xfer) {

            del_from_queue(xfer);

            spin_unlock_irqrestore(&res_lock, flags);
            _finish_off(xfer, TL7689_RES_ABORT, 1);
            spin_lock_irqsave(&res_lock, flags);

            xfer = ch->xfer_head;
        }
    }

ctrl_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    return ret;
}
EXPORT_SYMBOL(tl7689_dma_ctrl);

int tl7689_dma_enqueue(enum dma_ch id, void *token,
        dma_addr_t addr, int size)
{
    struct nusmart_pl330_chan *ch;
    struct nusmart_pl330_xfer *xfer;
    unsigned long flags;
    int idx, ret = 0;

    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    /* Error if invalid or free channel */
    if (!ch || chan_free(ch)) {
        ret = -EINVAL;
        goto enq_exit;
    }

    /* Error if size is unaligned */
    if (ch->rqcfg.brst_size && size % (1 << ch->rqcfg.brst_size)) {
        ret = -EINVAL;
        goto enq_exit;
    }

    xfer = kmem_cache_alloc(ch->dmac->kmcache, GFP_ATOMIC);
    if (!xfer) {
        ret = -ENOMEM;
        goto enq_exit;
    }

    xfer->token = token;
    xfer->chan = ch;
    xfer->px.bytes = size;
    xfer->px.next = NULL; /* Single request */

    /* For NUSMART DMA API, direction is always fixed for all xfers */
    if (ch->req[0].rqtype == MEMTODEV) {
        xfer->px.src_addr = addr;
        xfer->px.dst_addr = ch->sdaddr;
    } else {
        xfer->px.src_addr = ch->sdaddr;
        xfer->px.dst_addr = addr;
    }

    add_to_queue(ch, xfer, 0);

    /* Try submitting on either request */
    idx = (ch->lrq == &ch->req[0]) ? 1 : 0;

    if (!ch->req[idx].x)
        nusmart_pl330_submit(ch, &ch->req[idx]);
    else
        nusmart_pl330_submit(ch, &ch->req[1 - idx]);

    spin_unlock_irqrestore(&res_lock, flags);

    if (ch->options & TL7689_DMAF_AUTOSTART)
        tl7689_dma_ctrl(id, TL7689_DMAOP_START);

    return 0;

enq_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    //printk(KERN_EMERG "tl7689_dma_enqueue return with %d\n", ret);
    return ret;
}
EXPORT_SYMBOL(tl7689_dma_enqueue);

int tl7689_dma_request(enum dma_ch id,
        struct tl7689_dma_client *client,
        void *dev)
{
    struct nusmart_pl330_dmac *dmac;
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int ret = 0;

    spin_lock_irqsave(&res_lock, flags);
    //printk(KERN_EMERG "2815_dma_request in\n");
    ch = chan_acquire(id);
    if (!ch) {
        ret = -EBUSY;
        goto req_exit;
    }

    //printk(KERN_EMERG "2815_dma_request channel acquired\n");
    dmac = ch->dmac;

    ch->pl330_chan_id = pl330_request_channel(dmac->pi);
    if (!ch->pl330_chan_id) {
        chan_release(ch);
        ret = -EBUSY;
        goto req_exit;
    }
    //printk(KERN_EMERG "2815_dma_request channel requested\n");

    ch->client = client;
    ch->options = 0; /* Clear any option */
    ch->callback_fn = NULL; /* Clear any callback */
    ch->lrq = NULL;

    ch->rqcfg.brst_size = 2; /* Default word size */
    ch->rqcfg.swap = SWAP_NO;
    ch->rqcfg.scctl = SCCTRL0; /* Noncacheable and nonbufferable */
    ch->rqcfg.dcctl = DCCTRL0; /* Noncacheable and nonbufferable */
    ch->rqcfg.privileged = 0;
    ch->rqcfg.insnaccess = 0;

    /* Set invalid direction */
    ch->req[0].rqtype = DEVTODEV;
    ch->req[1].rqtype = ch->req[0].rqtype;

    ch->req[0].cfg = &ch->rqcfg;
    ch->req[1].cfg = ch->req[0].cfg;

    ch->req[0].peri = iface_of_dmac(dmac, id) - 1; /* Original index */
    ch->req[1].peri = ch->req[0].peri;

    ch->req[0].token = &ch->req[0];
    ch->req[0].xfer_cb = nusmart_pl330_rq0;
    ch->req[1].token = &ch->req[1];
    ch->req[1].xfer_cb = nusmart_pl330_rq1;

    ch->req[0].x = NULL;
    ch->req[1].x = NULL;

    /* Reset xfer list */
    INIT_LIST_HEAD(&ch->xfer_list);
    ch->xfer_head = NULL;

req_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    //printk(KERN_EMERG "2815_dma_request channel return with %d\n", ret);
    return ret;
}
EXPORT_SYMBOL(tl7689_dma_request);

int tl7689_dma_free(enum dma_ch id, struct tl7689_dma_client *client)
{
    struct nusmart_pl330_chan *ch;
    struct nusmart_pl330_xfer *xfer;
    unsigned long flags;
    int ret = 0;
    unsigned idx;

    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch))
        goto free_exit;

    /* Refuse if someone else wanted to free the channel */
    if (ch->client != client) {
        ret = -EBUSY;
        goto free_exit;
    }

    /* Stop any active xfer, Flushe the queue and do callbacks */
    pl330_chan_ctrl(ch->pl330_chan_id, PL330_OP_FLUSH);

    /* Abort the submitted requests */
    idx = (ch->lrq == &ch->req[0]) ? 1 : 0;

    if (ch->req[idx].x) {
        xfer = container_of(ch->req[idx].x,
                struct nusmart_pl330_xfer, px);

        ch->req[idx].x = NULL;
        del_from_queue(xfer);

        spin_unlock_irqrestore(&res_lock, flags);
        _finish_off(xfer, TL7689_RES_ABORT, 1);
        spin_lock_irqsave(&res_lock, flags);
    }

    if (ch->req[1 - idx].x) {
        xfer = container_of(ch->req[1 - idx].x,
                struct nusmart_pl330_xfer, px);

        ch->req[1 - idx].x = NULL;
        del_from_queue(xfer);

        spin_unlock_irqrestore(&res_lock, flags);
        _finish_off(xfer, TL7689_RES_ABORT, 1);
        spin_lock_irqsave(&res_lock, flags);
    }

    /* Pluck and Abort the queued requests in order */
    do {
        xfer = get_from_queue(ch, 1);

        spin_unlock_irqrestore(&res_lock, flags);
        _finish_off(xfer, TL7689_RES_ABORT, 1);
        spin_lock_irqsave(&res_lock, flags);
    } while (xfer);

    ch->client = NULL;

    pl330_release_channel(ch->pl330_chan_id);

    ch->pl330_chan_id = NULL;

    chan_release(ch);

free_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    return ret;
}
EXPORT_SYMBOL(tl7689_dma_free);

int tl7689_dma_config(enum dma_ch id, int xferunit)
{
    struct nusmart_pl330_chan *ch;
    struct pl330_info *pi;
    unsigned long flags;
    int i, dbwidth, ret = 0;

    //printk(KERN_EMERG "tl7689_dma_config IN");
    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch)) {
        ret = -EINVAL;
        goto cfg_exit;
    }

    pi = ch->dmac->pi;
    dbwidth = pi->pcfg.data_bus_width / 8;

    /* Max size of xfer can be pcfg.data_bus_width */
    if (xferunit > dbwidth) {
        ret = -EINVAL;
        goto cfg_exit;
    }

    i = 0;
    while (xferunit != (1 << i))
        i++;

    /* If valid value */
    if (xferunit == (1 << i))
        ch->rqcfg.brst_size = i;
    else
        ret = -EINVAL;

cfg_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    return ret;
}
EXPORT_SYMBOL(tl7689_dma_config);

/* Options that are supported by this driver */
#define NUSMART_PL330_FLAGS (TL7689_DMAF_CIRCULAR | TL7689_DMAF_AUTOSTART)

int tl7689_dma_setflags(enum dma_ch id, unsigned int options)
{
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int ret = 0;

    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch) || options & ~(NUSMART_PL330_FLAGS))
        ret = -EINVAL;
    else
        ch->options = options;

    spin_unlock_irqrestore(&res_lock, flags);

    return 0;
}
EXPORT_SYMBOL(tl7689_dma_setflags);

int tl7689_dma_set_buffdone_fn(enum dma_ch id, tl7689_dma_cbfn_t rtn)
{
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int ret = 0;

    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch))
        ret = -EINVAL;
    else
        ch->callback_fn = rtn;

    spin_unlock_irqrestore(&res_lock, flags);

    return ret;
}
EXPORT_SYMBOL(tl7689_dma_set_buffdone_fn);

int tl7689_dma_devconfig(enum dma_ch id, enum tl7689_dmasrc source,
        unsigned long address)
{
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int ret = 0;

    spin_lock_irqsave(&res_lock, flags);

    ch = id_to_chan(id);

    if (!ch || chan_free(ch)) {
        ret = -EINVAL;
        goto devcfg_exit;
    }

    switch (source) {
        case TL7689_DMASRC_HW: /* P->M */
            ch->req[0].rqtype = DEVTOMEM;
            ch->req[1].rqtype = DEVTOMEM;
            ch->rqcfg.src_inc = 0;
            ch->rqcfg.dst_inc = 1;
            break;
        case TL7689_DMASRC_MEM: /* M->P */
            ch->req[0].rqtype = MEMTODEV;
            ch->req[1].rqtype = MEMTODEV;
            ch->rqcfg.src_inc = 1;
            ch->rqcfg.dst_inc = 0;
            break;
        default:
            ret = -EINVAL;
            goto devcfg_exit;
    }

    ch->sdaddr = address;

devcfg_exit:
    spin_unlock_irqrestore(&res_lock, flags);

    return ret;
}
EXPORT_SYMBOL(tl7689_dma_devconfig);

int tl7689_dma_getposition(enum dma_ch id, dma_addr_t *src, dma_addr_t *dst)
{
    struct nusmart_pl330_chan *ch = id_to_chan(id);
    struct pl330_chanstatus status;
    int ret;

    //printk(KERN_EMERG "tl7689_dma_getposition IN\n");
    if (!ch || chan_free(ch))
        return -EINVAL;

    ret = pl330_chan_status(ch->pl330_chan_id, &status);
    if (ret < 0)
        return ret;

    *src = status.src_addr;
    *dst = status.dst_addr;

    //printk(KERN_EMERG "tl7689_dma_getposition src = 0x%x, dst = 0x%x\n", status.src_addr, status.dst_addr);

    return 0;
}
EXPORT_SYMBOL(tl7689_dma_getposition);

static irqreturn_t pl330_irq_handler(int irq, void *data)
{
    if (pl330_update(data))
        return IRQ_HANDLED;
    else
        return IRQ_NONE;
}

static int pl330_probe(struct platform_device *pdev)
{
    struct nusmart_pl330_dmac *nusmart_pl330_dmac;
    struct nusmart_pl330_platdata *pl330pd;
    struct pl330_info *pl330_info;
    struct resource *res;
    int i, ret, irq;

    pl330pd = pdev->dev.platform_data;

    /* Can't do without the list of _32_ peripherals */
    if (!pl330pd || !pl330pd->peri) {
        dev_err(&pdev->dev, "platform data missing!\n");
        return -ENODEV;
    }

    printk(KERN_EMERG "pl330pd->peri as %d\n", *pl330pd->peri);

    pl330_info = kzalloc(sizeof(*pl330_info), GFP_KERNEL);
    if (!pl330_info)
        return -ENOMEM;

    pl330_info->pl330_data = NULL;
    pl330_info->dev = &pdev->dev;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        ret = -ENODEV;
        goto probe_err1;
    }

    request_mem_region(res->start, resource_size(res), pdev->name);

    pl330_info->base = ioremap(res->start, resource_size(res));
    if (!pl330_info->base) {
        ret = -ENXIO;
        goto probe_err2;
    }

    irq = platform_get_irq(pdev, 0);
    if (irq < 0) {
        ret = irq;
        goto probe_err3;
    }

    ret = request_irq(irq, pl330_irq_handler, 0,
            dev_name(&pdev->dev), pl330_info);
    if (ret)
        goto probe_err4;

    /* Allocate a new DMAC */
    nusmart_pl330_dmac = kmalloc(sizeof(*nusmart_pl330_dmac), GFP_KERNEL);
    if (!nusmart_pl330_dmac) {
        ret = -ENOMEM;
        goto probe_err5;
    }

    /* Get operation clock and enable it */
    //nusmart_pl330_dmac->clk = clk_get(&pdev->dev, "pdma");
    //if (IS_ERR(nusmart_pl330_dmac->clk)) {
    //	dev_err(&pdev->dev, "Cannot get operation clock.\n");
    //	ret = -EINVAL;
    //	goto probe_err6;
    //}
    //clk_enable(nusmart_pl330_dmac->clk);

    ret = pl330_add(pl330_info);
    if (ret)
        goto probe_err7;

    /* Hook the info */
    nusmart_pl330_dmac->pi = pl330_info;

    /* No busy channels */
    nusmart_pl330_dmac->busy_chan = 0;

    nusmart_pl330_dmac->kmcache = kmem_cache_create(dev_name(&pdev->dev),
            sizeof(struct nusmart_pl330_xfer), 0, 0, NULL);

    if (!nusmart_pl330_dmac->kmcache) {
        ret = -ENOMEM;
        goto probe_err8;
    }

    /* Get the list of peripherals */
    nusmart_pl330_dmac->peri = pl330pd->peri;

    /* Attach to the list of DMACs */
    list_add_tail(&nusmart_pl330_dmac->node, &dmac_list);

    /* Create a channel for each peripheral in the DMAC
     * that is, if it doesn't already exist
     */
    for (i = 0; i < PL330_MAX_PERI; i++)
        //for (i = DMACH_I2S0_RX; i < PL330_MAX_PERI; i++)
        if (nusmart_pl330_dmac->peri[i] != DMACH_MAX)
            chan_add(nusmart_pl330_dmac->peri[i]);

    printk(KERN_INFO
            "Loaded driver for PL330 DMAC-%d %s\n",	pdev->id, pdev->name);
    printk(KERN_INFO
            "\tDBUFF-%ux%ubytes Num_Chans-%u Num_Peri-%u Num_Events-%u\n",
            pl330_info->pcfg.data_buf_dep,
            pl330_info->pcfg.data_bus_width / 8, pl330_info->pcfg.num_chan,
            pl330_info->pcfg.num_peri, pl330_info->pcfg.num_events);

    return 0;

probe_err8:
    pl330_del(pl330_info);
probe_err7:
    //clk_disable(nusmart_pl330_dmac->clk);
    //clk_put(nusmart_pl330_dmac->clk);
probe_err6:
    kfree(nusmart_pl330_dmac);
probe_err5:
    free_irq(irq, pl330_info);
probe_err4:
probe_err3:
    iounmap(pl330_info->base);
probe_err2:
    release_mem_region(res->start, resource_size(res));
probe_err1:
    kfree(pl330_info);

    return ret;
}

static int pl330_remove(struct platform_device *pdev)
{
    struct nusmart_pl330_dmac *dmac, *d;
    struct nusmart_pl330_chan *ch;
    unsigned long flags;
    int del, found;

    if (!pdev->dev.platform_data)
        return -EINVAL;

    spin_lock_irqsave(&res_lock, flags);

    found = 0;
    list_for_each_entry(d, &dmac_list, node)
        if (d->pi->dev == &pdev->dev) {
            found = 1;
            break;
        }

    if (!found) {
        spin_unlock_irqrestore(&res_lock, flags);
        return 0;
    }

    dmac = d;

    /* Remove all Channels that are managed only by this DMAC */
    list_for_each_entry(ch, &chan_list, node) {

        /* Only channels that are handled by this DMAC */
        if (iface_of_dmac(dmac, ch->id))
            del = 1;
        else
            continue;

        /* Don't remove if some other DMAC has it too */
        list_for_each_entry(d, &dmac_list, node)
            if (d != dmac && iface_of_dmac(d, ch->id)) {
                del = 0;
                break;
            }

        if (del) {
            spin_unlock_irqrestore(&res_lock, flags);
            tl7689_dma_free(ch->id, ch->client);
            spin_lock_irqsave(&res_lock, flags);
            list_del(&ch->node);
            kfree(ch);
        }
    }

    /* Disable operation clock */
    //clk_disable(dmac->clk);
    //clk_put(dmac->clk);

    /* Remove the DMAC */
    list_del(&dmac->node);
    kfree(dmac);

    spin_unlock_irqrestore(&res_lock, flags);

    return 0;
}

static struct platform_driver pl330_driver = {
    .driver		= {
        .owner	= THIS_MODULE,
        .name	= "nusmart-pl330",
    },
    .probe		= pl330_probe,
    .remove		= pl330_remove,
};

static int __init pl330_init(void)
{
    return platform_driver_register(&pl330_driver);
}
module_init(pl330_init);

static void __exit pl330_exit(void)
{
    platform_driver_unregister(&pl330_driver);
    return;
}
module_exit(pl330_exit);

MODULE_AUTHOR("Jaswinder Singh <jassi.brar@samsung.com>");
MODULE_DESCRIPTION("Driver for PL330 DMA Controller");
MODULE_LICENSE("GPL");
