/*
 * MAC commands interface
 *
 * Copyright 2007-2012 Siemens AG
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Written by:
 * Sergey Lapin <slapin@ossfans.org>
 * Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 * Alexander Smirnov <alex.bluesman.smirnov@gmail.com>
 */

#include <linux/skbuff.h>
#include <linux/if_arp.h>

#include <net/ieee802154.h>
#include <net/ieee802154_netdev.h>
#include <net/wpan-phy.h>
#include <net/mac802154.h>
#include <net/nl802154.h>

#include "mac802154.h"

static int mac802154_cmd_assoc_req(struct sk_buff *skb)
{
	u8 cap;

	if (skb->len != 2)
		return -EINVAL;

	if (skb->pkt_type != PACKET_HOST)
		return 0;

	if (mac_cb(skb)->sa.addr_type != IEEE802154_ADDR_LONG ||
	    mac_cb(skb)->sa.pan_id != IEEE802154_PANID_BROADCAST)
		return -EINVAL;

	/*
	 * FIXME: check that we allow incoming ASSOC requests
	 * by consulting MIB
	 */

	cap = skb->data[1];

	return ieee802154_nl_assoc_indic(skb->dev, &mac_cb(skb)->sa, cap);
}

static int mac802154_cmd_assoc_resp(struct sk_buff *skb)
{
	u8 status;
	u16 short_addr;

	if (skb->len != 4)
		return -EINVAL;

	if (skb->pkt_type != PACKET_HOST)
		return 0;

	if (mac_cb(skb)->sa.addr_type != IEEE802154_ADDR_LONG ||
	    mac_cb(skb)->sa.addr_type != IEEE802154_ADDR_LONG ||
	    !(mac_cb(skb)->flags & MAC_CB_FLAG_INTRAPAN))
		return -EINVAL;

	/* FIXME: check that we requested association ? */

	status = skb->data[3];
	short_addr = skb->data[1] | (skb->data[2] << 8);
	pr_info("Received ASSOC-RESP status %x, addr %hx\n", status,
			short_addr);
	if (status) {
		mac802154_dev_set_short_addr(skb->dev,
				IEEE802154_ADDR_BROADCAST);
		mac802154_dev_set_pan_id(skb->dev,
				IEEE802154_PANID_BROADCAST);
	} else
		mac802154_dev_set_short_addr(skb->dev, short_addr);

	return ieee802154_nl_assoc_confirm(skb->dev, short_addr, status);
}

static int mac802154_cmd_disassoc_notify(struct sk_buff *skb)
{
	u8 reason;

	if (skb->len != 2)
		return -EINVAL;

	if (skb->pkt_type != PACKET_HOST)
		return 0;

	if (mac_cb(skb)->sa.addr_type != IEEE802154_ADDR_LONG ||
	    (mac_cb(skb)->da.addr_type != IEEE802154_ADDR_LONG &&
	     mac_cb(skb)->da.addr_type != IEEE802154_ADDR_SHORT) ||
	    mac_cb(skb)->sa.pan_id != mac_cb(skb)->da.pan_id)
		return -EINVAL;

	reason = skb->data[1];

	/* FIXME: checks if this was our coordinator and the disassoc us */
	/* FIXME: if we device, one should receive ->da and not ->sa */
	/* FIXME: the status should also help */

	return ieee802154_nl_disassoc_indic(skb->dev, &mac_cb(skb)->sa,
			reason);
}

int mac802154_process_cmd(struct net_device *dev, struct sk_buff *skb)
{
	u8 cmd;

	if (skb->len < 1) {
		pr_warning("Uncomplete command frame!\n");
		goto drop;
	}

	cmd = *(skb->data);
	pr_debug("(%s): command %02x on device %s\n",
					__func__, cmd, dev->name);

	switch (cmd) {
	case IEEE802154_CMD_ASSOCIATION_REQ:
		mac802154_cmd_assoc_req(skb);
		break;
	case IEEE802154_CMD_ASSOCIATION_RESP:
		mac802154_cmd_assoc_resp(skb);
		break;
	case IEEE802154_CMD_DISASSOCIATION_NOTIFY:
		mac802154_cmd_disassoc_notify(skb);
		break;
	case IEEE802154_CMD_BEACON_REQ:
	default:
		pr_debug("(%s): frame type is not supported yet\n", __func__);
		goto drop;
	}


	kfree_skb(skb);
	return NET_RX_SUCCESS;

drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}

static int mac802154_send_cmd(struct net_device *dev,
		struct ieee802154_addr *addr, struct ieee802154_addr *saddr,
		const u8 *buf, int len)
{
	struct sk_buff *skb;
	int err;
	int hlen = LL_RESERVED_SPACE(dev), tlen = dev->needed_tailroom;

	BUG_ON(dev->type != ARPHRD_IEEE802154);

	skb = alloc_skb(hlen + tlen + len, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	skb_reserve(skb, hlen);

	skb_reset_network_header(skb);

	mac_cb(skb)->flags = IEEE802154_FC_TYPE_MAC_CMD | MAC_CB_FLAG_ACKREQ;
	mac_cb(skb)->seq = ieee802154_mlme_ops(dev)->get_dsn(dev);
	err = dev_hard_header(skb, dev, ETH_P_IEEE802154, addr, saddr, len);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

	skb_reset_mac_header(skb);
	memcpy(skb_put(skb, len), buf, len);

	skb->dev = dev;
	skb->protocol = htons(ETH_P_IEEE802154);

	return dev_queue_xmit(skb);
}

static int mac802154_mlme_assoc_req(struct net_device *dev,
		struct ieee802154_addr *addr, u8 channel, u8 page, u8 cap)
{
	struct ieee802154_addr saddr;
	u8 buf[2];
	int pos = 0;

	saddr.addr_type = IEEE802154_ADDR_LONG;
	saddr.pan_id = IEEE802154_PANID_BROADCAST;
	memcpy(saddr.hwaddr, dev->dev_addr, IEEE802154_ADDR_LEN);


	/* FIXME: set PIB/MIB info */
	mac802154_dev_set_pan_id(dev, addr->pan_id);
	mac802154_dev_set_page_channel(dev, page, channel);
	mac802154_dev_set_ieee_addr(dev);

	buf[pos++] = IEEE802154_CMD_ASSOCIATION_REQ;
	buf[pos++] = cap;

	return mac802154_send_cmd(dev, addr, &saddr, buf, pos);
}

static int mac802154_mlme_assoc_resp(struct net_device *dev,
		struct ieee802154_addr *addr, u16 short_addr, u8 status)
{
	struct ieee802154_addr saddr;
	u8 buf[4];
	int pos = 0;

	saddr.addr_type = IEEE802154_ADDR_LONG;
	saddr.pan_id = addr->pan_id;
	memcpy(saddr.hwaddr, dev->dev_addr, IEEE802154_ADDR_LEN);

	buf[pos++] = IEEE802154_CMD_ASSOCIATION_RESP;
	buf[pos++] = short_addr;
	buf[pos++] = short_addr >> 8;
	buf[pos++] = status;

	return mac802154_send_cmd(dev, addr, &saddr, buf, pos);
}

static int mac802154_mlme_disassoc_req(struct net_device *dev,
		struct ieee802154_addr *addr, u8 reason)
{
	struct ieee802154_addr saddr;
	u8 buf[2];
	int pos = 0;
	int ret;

	saddr.addr_type = IEEE802154_ADDR_LONG;
	saddr.pan_id = addr->pan_id;
	memcpy(saddr.hwaddr, dev->dev_addr, IEEE802154_ADDR_LEN);

	buf[pos++] = IEEE802154_CMD_DISASSOCIATION_NOTIFY;
	buf[pos++] = reason;

	ret = mac802154_send_cmd(dev, addr, &saddr, buf, pos);

	/* FIXME: this should be after the ack receved */
	mac802154_dev_set_pan_id(dev, 0xffff);
	mac802154_dev_set_short_addr(dev, 0xffff);
	ieee802154_nl_disassoc_confirm(dev, 0x00);

	return ret;
}

static int mac802154_mlme_start_req(struct net_device *dev,
				struct ieee802154_addr *addr,
				u8 channel, u8 page,
				u8 bcn_ord, u8 sf_ord, u8 pan_coord, u8 blx,
				u8 coord_realign)
{
	BUG_ON(addr->addr_type != IEEE802154_ADDR_SHORT);

	mac802154_dev_set_pan_id(dev, addr->pan_id);
	mac802154_dev_set_short_addr(dev, addr->short_addr);
	mac802154_dev_set_ieee_addr(dev);
	mac802154_dev_set_page_channel(dev, page, channel);

	/*
	 * FIXME: add validation for unused parameters to be sane
	 * for SoftMAC
	 */

	if (pan_coord)
		dev->priv_flags |= IFF_IEEE802154_COORD;
	else
		dev->priv_flags &= ~IFF_IEEE802154_COORD;

	mac802154_dev_set_pan_coord(dev);
	ieee802154_nl_start_confirm(dev, IEEE802154_SUCCESS);

	return 0;
}

struct ieee802154_reduced_mlme_ops mac802154_mlme_reduced = {
	.get_phy = mac802154_get_phy,
};

struct ieee802154_mlme_ops mac802154_mlme_wpan = {
	.assoc_req = mac802154_mlme_assoc_req,
	.assoc_resp = mac802154_mlme_assoc_resp,
	.disassoc_req = mac802154_mlme_disassoc_req,
	.start_req = mac802154_mlme_start_req,

	.get_phy = mac802154_get_phy,

	.get_pan_id = mac802154_dev_get_pan_id,
	.get_short_addr = mac802154_dev_get_short_addr,
	.get_dsn = mac802154_dev_get_dsn,
	.get_bsn = mac802154_dev_get_bsn,
};
