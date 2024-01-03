/*
 * tx/rx interfaces for XRadio drivers
 *
 * Copyright (c) 2013
 * Xradio Technology Co., Ltd. <www.xradiotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef XRADIO_TXRX_H
#define XRADIO_TXRX_H

#include <linux/list.h>
#include "wsm.h"

/* extern */ struct ieee80211_hw;
/* extern */ struct sk_buff;
/* extern */ struct xradio_txpriv;
/* extern */ struct xradio_vif;

#ifdef SUPPORT_HT40

#ifdef CONFIG_XRADIO_DEBUGFS
/*for tx rates debug.*/
extern u8  rates_dbg_en;
extern u16 rates_debug[6];
extern u8  Ratecnt_dbg;
extern u8  retry_dbg;
#endif

#else

#ifdef CONFIG_XRADIO_DEBUGFS
/*for tx rates debug.*/
extern u8 rates_dbg_en;
extern u32 rates_debug[3];
extern u8 maxRate_dbg;
extern u8 retry_dbg;
#endif

#endif

#ifdef SUPPORT_HT40

/* ModemType in RateEntry */
#define MODEMTYPE_SHIFT     14
#define MODEMTYPE_MASK      0x03
#define RATE_MODEM_LEGACY   0x00
#define RATE_MODEM_HTOFDM   0x01
#define RATE_MODEM_VHTOFDM  0x02

/* Bandwidth in RateEntry */
#define BANDWIDTH_SHIFT     12
#define BANDWIDTH_MASK      0x03
#define RATE_BANDWIDTH_20M  0x00
#define RATE_BANDWIDTH_40M  0x01
#define RATE_BANDWIDTH_80M  0x02

/* FormatFlag in RateEntry flags */
#define RATE_F_MASK         0x0F00
#define RATE_F_PREAMBLE_S   0x0100
#define RATE_F_SGI          0x0200
#define RATE_F_GF           0x0400
#define RATE_F_STBC         0x0800

#define MAKE_PREAMBLE_S(b)   ((!!(b))<<8)
#define MAKE_SGI(b)          ((!!(b))<<9)
#define MAKE_GF(b)           ((!!(b))<<10)
#define MAKE_STBC(b)         ((!!(b))<<11)

#define RATEINDEX_SHIFT     4
#define RATEINDEX_MASK      0xf

#define MCS_RATES_OFF  14
#define MAX_RATES_IDX  21
#define A_RATES_OFF    6
#define B_RATE_INDEX   0
#define AG_RATE_INDEX  A_RATES_OFF

#define XRADIO_INVALID_RATE_ID     (0xFF)
#define XRADIO_INVALID_RATE_ENTRY   0x00F0

/* ModemType in RateEntry */
#define FW_RATE_MODEM_SHIFT     14
#define FW_RATE_MODEM_MASK      0xC000
#define FW_RATE_MODEM_LEGACY    0x00
#define FW_RATE_MODEM_HTOFDM    0x01
#define FW_RATE_MODEM_VHTOFDM   0x02

/* Bandwidth in RateEntry */
#define FW_RATE_BW_SHIFT        12
#define FW_RATE_BW_MASK         0x3000
#define FW_RATE_BW_20M          0x00
#define FW_RATE_BW_40M          0x01
#define FW_RATE_BW_80M          0x02

/* FormatFlag in RateEntry flags */
#define FW_RATE_F_SHIFT         8
#define FW_RATE_F_MASK          0x0F00
#define FW_RATE_F_SPRE          0x1
#define FW_RATE_F_SGI           0x2
#define FW_RATE_F_GF            0x4
#define FW_RATE_F_STBC          0x8

/* RateIndex */
#define FW_RATE_INDEX_SHIFT     4
#define FW_RATE_INDEX_MASK      0x00F0
#define FW_RATE_USE_DEFAULT     0xF

/* MaxRetry */
#define FW_RATE_MAXRETRY_SHIFT  0
#define FW_RATE_MAXRETRY_MASK   0x000F

#define GET_RATE_ENTRY_MODEM(_RateEntry)	(((_RateEntry)&FW_RATE_MODEM_MASK)>>FW_RATE_MODEM_SHIFT)
#define GET_RATE_ENTRY_BANDWIDTH(_RateEntry)	(((_RateEntry)&FW_RATE_BW_MASK)>>FW_RATE_BW_SHIFT)
#define GET_RATE_ENTRY_FLAGS(_RateEntry)	(((_RateEntry)&FW_RATE_F_MASK)>>FW_RATE_F_SHIFT)
#define GET_RATE_ENTRY_RATEINDEX(_RateEntry)	(((_RateEntry)&FW_RATE_INDEX_MASK)>>FW_RATE_INDEX_SHIFT)
#define GET_RATE_ENTRY_MAXRETRY(_RateEntry)	(((_RateEntry)&FW_RATE_MAXRETRY_MASK)>>FW_RATE_MAXRETRY_SHIFT)

#define PUT_RATE_ENTRY_MODEM(_Value)		(((_Value)<<FW_RATE_MODEM_SHIFT)&FW_RATE_MODEM_MASK)
#define PUT_RATE_ENTRY_BANDWIDTH(_Value)	(((_Value)<<FW_RATE_BW_SHIFT)&FW_RATE_BW_MASK)
#define PUT_RATE_ENTRY_FLAGS(_Value)		(((_Value)<<FW_RATE_F_SHIFT)&FW_RATE_F_MASK)
#define PUT_RATE_ENTRY_RATEINDEX(_Value)	(((_Value)<<FW_RATE_INDEX_SHIFT)&FW_RATE_INDEX_MASK)
#define PUT_RATE_ENTRY_MAXRETRY(_Value)		(((_Value)<<FW_RATE_MAXRETRY_SHIFT)&FW_RATE_MAXRETRY_MASK)

#endif
struct tx_policy {
	union {

#ifdef SUPPORT_HT40

		__le16 rate_entrys[MAX_RATES_STAGE];

#else

		__le32 tbl[3];

#endif
		u8 raw[12];
	};
	u8  defined;		/* TODO: u32 or u8, profile and select best */
	u8  usage_count;	/* --// -- */
	u8  retry_count;	/* --// -- */
	u8  uploaded;
};

struct tx_policy_cache_entry {
	struct tx_policy policy;
	struct list_head link;
};

struct tx_policy_cache {
	struct tx_policy_cache_entry cache[TX_POLICY_CACHE_SIZE];
	struct list_head used;
	struct list_head free;
	spinlock_t lock;
	bool   queue_locked;
};

/* ******************************************************************** */
/* TX policy cache							*/
/* Intention of TX policy cache is an overcomplicated WSM API.
 * Device does not accept per-PDU tx retry sequence.
 * It uses "tx retry policy id" instead, so driver code has to sync
 * linux tx retry sequences with a retry policy table in the device.
 */
void tx_policy_init(struct xradio_common *hw_priv);
void tx_policy_upload_work(struct work_struct *work);

/* ******************************************************************** */
/* TX implementation							*/

u32 xradio_rate_mask_to_wsm(struct xradio_common *hw_priv,
			       u32 rates);
void xradio_tx(struct ieee80211_hw *dev, struct ieee80211_tx_control *control, struct sk_buff *skb);

void xradio_skb_dtor(struct xradio_common *hw_priv,
		     struct sk_buff *skb,
		     const struct xradio_txpriv *txpriv);

/* ******************************************************************** */

#ifdef SUPPORT_HT40

/* Convert hw_value to rate entry */
u16 xradio_get_rate_entry(const struct xradio_common *hw_priv,
			u8 Bandwidth, u16 FormatFlag, u8 hw_value);

#endif

/* WSM callbacks							*/

void xradio_tx_confirm_cb(struct xradio_common *hw_priv,
			  struct wsm_tx_confirm *arg);
void xradio_rx_cb(struct xradio_vif *priv,
		  struct wsm_rx *arg,
		  struct sk_buff **skb_p);

/* ******************************************************************** */
/* Timeout								*/

void xradio_tx_timeout(struct work_struct *work);

/* ******************************************************************** */
/* Security								*/
int xradio_alloc_key(struct xradio_common *hw_priv);
void xradio_free_key(struct xradio_common *hw_priv, int idx);
void xradio_free_keys(struct xradio_common *hw_priv);
int xradio_upload_keys(struct xradio_vif *priv);

/* ******************************************************************** */
/* Workaround for WFD test case 6.1.10					*/
#if defined(CONFIG_XRADIO_USE_EXTENSIONS)
void xradio_link_id_reset(struct work_struct *work);
#endif

#endif /* XRADIO_TXRX_H */
