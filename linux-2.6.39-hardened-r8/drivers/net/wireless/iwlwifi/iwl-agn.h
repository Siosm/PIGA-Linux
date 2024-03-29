/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2008 - 2010 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called LICENSE.GPL.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
 * Copyright(c) 2005 - 2010 Intel Corporation. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef __iwl_agn_h__
#define __iwl_agn_h__

#include "iwl-dev.h"

/* configuration for the _agn devices */
extern struct iwl_cfg iwl4965_agn_cfg;
extern struct iwl_cfg iwl5300_agn_cfg;
extern struct iwl_cfg iwl5100_agn_cfg;
extern struct iwl_cfg iwl5350_agn_cfg;
extern struct iwl_cfg iwl5100_bgn_cfg;
extern struct iwl_cfg iwl5100_abg_cfg;
extern struct iwl_cfg iwl5150_agn_cfg;
extern struct iwl_cfg iwl5150_abg_cfg;
extern struct iwl_cfg iwl6005_2agn_cfg;
extern struct iwl_cfg iwl6005_2abg_cfg;
extern struct iwl_cfg iwl6005_2bg_cfg;
extern struct iwl_cfg iwl1030_bgn_cfg;
extern struct iwl_cfg iwl1030_bg_cfg;
extern struct iwl_cfg iwl6030_2agn_cfg;
extern struct iwl_cfg iwl6030_2abg_cfg;
extern struct iwl_cfg iwl6030_2bgn_cfg;
extern struct iwl_cfg iwl6030_2bg_cfg;
extern struct iwl_cfg iwl6000i_2agn_cfg;
extern struct iwl_cfg iwl6000i_2abg_cfg;
extern struct iwl_cfg iwl6000i_2bg_cfg;
extern struct iwl_cfg iwl6000_3agn_cfg;
extern struct iwl_cfg iwl6050_2agn_cfg;
extern struct iwl_cfg iwl6050_2abg_cfg;
extern struct iwl_cfg iwl6150_bgn_cfg;
extern struct iwl_cfg iwl1000_bgn_cfg;
extern struct iwl_cfg iwl1000_bg_cfg;
extern struct iwl_cfg iwl100_bgn_cfg;
extern struct iwl_cfg iwl100_bg_cfg;
extern struct iwl_cfg iwl130_bgn_cfg;
extern struct iwl_cfg iwl130_bg_cfg;
extern struct iwl_cfg iwl2000_2bgn_cfg;
extern struct iwl_cfg iwl2000_2bg_cfg;
extern struct iwl_cfg iwl2030_2bgn_cfg;
extern struct iwl_cfg iwl2030_2bg_cfg;
extern struct iwl_cfg iwl6035_2agn_cfg;
extern struct iwl_cfg iwl6035_2abg_cfg;
extern struct iwl_cfg iwl6035_2bg_cfg;
extern struct iwl_cfg iwl200_bg_cfg;
extern struct iwl_cfg iwl200_bgn_cfg;
extern struct iwl_cfg iwl230_bg_cfg;
extern struct iwl_cfg iwl230_bgn_cfg;

extern struct iwl_mod_params iwlagn_mod_params;
extern const struct iwl_hcmd_ops iwlagn_hcmd;
extern const struct iwl_hcmd_ops iwlagn_bt_hcmd;
extern const struct iwl_hcmd_utils_ops iwlagn_hcmd_utils;

extern struct ieee80211_ops iwlagn_hw_ops;
extern struct ieee80211_ops iwl4965_hw_ops;

int iwl_reset_ict(struct iwl_priv *priv);
void iwl_disable_ict(struct iwl_priv *priv);
int iwl_alloc_isr_ict(struct iwl_priv *priv);
void iwl_free_isr_ict(struct iwl_priv *priv);
irqreturn_t iwl_isr_ict(int irq, void *data);

/* tx queue */
void iwlagn_set_wr_ptrs(struct iwl_priv *priv,
		     int txq_id, u32 index);
void iwlagn_tx_queue_set_status(struct iwl_priv *priv,
			     struct iwl_tx_queue *txq,
			     int tx_fifo_id, int scd_retry);
void iwlagn_txq_update_byte_cnt_tbl(struct iwl_priv *priv,
				    struct iwl_tx_queue *txq,
				    u16 byte_cnt);
void iwlagn_txq_inval_byte_cnt_tbl(struct iwl_priv *priv,
				   struct iwl_tx_queue *txq);
int iwlagn_txq_agg_enable(struct iwl_priv *priv, int txq_id,
			  int tx_fifo, int sta_id, int tid, u16 ssn_idx);
int iwlagn_txq_agg_disable(struct iwl_priv *priv, u16 txq_id,
			   u16 ssn_idx, u8 tx_fifo);
void iwlagn_txq_set_sched(struct iwl_priv *priv, u32 mask);
void iwl_free_tfds_in_queue(struct iwl_priv *priv,
			    int sta_id, int tid, int freed);

/* RXON */
int iwlagn_commit_rxon(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
void iwlagn_set_rxon_chain(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
int iwlagn_mac_config(struct ieee80211_hw *hw, u32 changed);
void iwlagn_bss_info_changed(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     struct ieee80211_bss_conf *bss_conf,
			     u32 changes);

/* uCode */
int iwlagn_load_ucode(struct iwl_priv *priv);
void iwlagn_rx_calib_result(struct iwl_priv *priv,
			 struct iwl_rx_mem_buffer *rxb);
void iwlagn_rx_calib_complete(struct iwl_priv *priv,
			   struct iwl_rx_mem_buffer *rxb);
void iwlagn_init_alive_start(struct iwl_priv *priv);
int iwlagn_alive_notify(struct iwl_priv *priv);
int iwl_verify_ucode(struct iwl_priv *priv);
void iwlagn_send_bt_env(struct iwl_priv *priv, u8 action, u8 type);
void iwlagn_send_prio_tbl(struct iwl_priv *priv);

/* lib */
void iwl_check_abort_status(struct iwl_priv *priv,
			    u8 frame_count, u32 status);
void iwlagn_rx_handler_setup(struct iwl_priv *priv);
void iwlagn_setup_deferred_work(struct iwl_priv *priv);
int iwlagn_hw_valid_rtc_data_addr(u32 addr);
int iwlagn_send_tx_power(struct iwl_priv *priv);
void iwlagn_temperature(struct iwl_priv *priv);
u16 iwlagn_eeprom_calib_version(struct iwl_priv *priv);
const u8 *iwlagn_eeprom_query_addr(const struct iwl_priv *priv,
				   size_t offset);
void iwlagn_rx_queue_reset(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_rx_init(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_hw_nic_init(struct iwl_priv *priv);
int iwlagn_wait_tx_queue_empty(struct iwl_priv *priv);
int iwlagn_txfifo_flush(struct iwl_priv *priv, u16 flush_control);
void iwlagn_dev_txfifo_flush(struct iwl_priv *priv, u16 flush_control);
void iwl_dump_csr(struct iwl_priv *priv);
int iwl_dump_fh(struct iwl_priv *priv, char **buf, bool display);

/* rx */
void iwlagn_rx_queue_restock(struct iwl_priv *priv);
void iwlagn_rx_allocate(struct iwl_priv *priv, gfp_t priority);
void iwlagn_rx_replenish(struct iwl_priv *priv);
void iwlagn_rx_replenish_now(struct iwl_priv *priv);
void iwlagn_rx_queue_free(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_rxq_stop(struct iwl_priv *priv);
int iwlagn_hwrate_to_mac80211_idx(u32 rate_n_flags, enum ieee80211_band band);
void iwl_setup_rx_handlers(struct iwl_priv *priv);

/* tx */
void iwl_hw_txq_free_tfd(struct iwl_priv *priv, struct iwl_tx_queue *txq);
int iwl_hw_txq_attach_buf_to_tfd(struct iwl_priv *priv,
				 struct iwl_tx_queue *txq,
				 dma_addr_t addr, u16 len, u8 reset, u8 pad);
int iwl_hw_tx_queue_init(struct iwl_priv *priv,
			 struct iwl_tx_queue *txq);
void iwlagn_hwrate_to_tx_control(struct iwl_priv *priv, u32 rate_n_flags,
			      struct ieee80211_tx_info *info);
int iwlagn_tx_skb(struct iwl_priv *priv, struct sk_buff *skb);
int iwlagn_tx_agg_start(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u16 *ssn);
int iwlagn_tx_agg_stop(struct iwl_priv *priv, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta, u16 tid);
int iwlagn_txq_check_empty(struct iwl_priv *priv,
			   int sta_id, u8 tid, int txq_id);
void iwlagn_rx_reply_compressed_ba(struct iwl_priv *priv,
				struct iwl_rx_mem_buffer *rxb);
int iwlagn_tx_queue_reclaim(struct iwl_priv *priv, int txq_id, int index);
void iwlagn_hw_txq_ctx_free(struct iwl_priv *priv);
int iwlagn_txq_ctx_alloc(struct iwl_priv *priv);
void iwlagn_txq_ctx_reset(struct iwl_priv *priv);
void iwlagn_txq_ctx_stop(struct iwl_priv *priv);

static inline u32 iwl_tx_status_to_mac80211(u32 status)
{
	status &= TX_STATUS_MSK;

	switch (status) {
	case TX_STATUS_SUCCESS:
	case TX_STATUS_DIRECT_DONE:
		return IEEE80211_TX_STAT_ACK;
	case TX_STATUS_FAIL_DEST_PS:
		return IEEE80211_TX_STAT_TX_FILTERED;
	default:
		return 0;
	}
}

static inline bool iwl_is_tx_success(u32 status)
{
	status &= TX_STATUS_MSK;
	return (status == TX_STATUS_SUCCESS) ||
	       (status == TX_STATUS_DIRECT_DONE);
}

u8 iwl_toggle_tx_ant(struct iwl_priv *priv, u8 ant_idx, u8 valid);

/* scan */
int iwlagn_request_scan(struct iwl_priv *priv, struct ieee80211_vif *vif);
void iwlagn_post_scan(struct iwl_priv *priv);

/* station mgmt */
int iwlagn_manage_ibss_station(struct iwl_priv *priv,
			       struct ieee80211_vif *vif, bool add);

/* hcmd */
int iwlagn_send_rxon_assoc(struct iwl_priv *priv,
			   struct iwl_rxon_context *ctx);
int iwlagn_send_tx_ant_config(struct iwl_priv *priv, u8 valid_tx_ant);
int iwlagn_send_beacon_cmd(struct iwl_priv *priv);

/* bt coex */
void iwlagn_send_advance_bt_config(struct iwl_priv *priv);
void iwlagn_bt_coex_profile_notif(struct iwl_priv *priv,
				  struct iwl_rx_mem_buffer *rxb);
void iwlagn_bt_rx_handler_setup(struct iwl_priv *priv);
void iwlagn_bt_setup_deferred_work(struct iwl_priv *priv);
void iwlagn_bt_cancel_deferred_work(struct iwl_priv *priv);

#ifdef CONFIG_IWLWIFI_DEBUG
const char *iwl_get_tx_fail_reason(u32 status);
const char *iwl_get_agg_tx_fail_reason(u16 status);
#else
static inline const char *iwl_get_tx_fail_reason(u32 status) { return ""; }
static inline const char *iwl_get_agg_tx_fail_reason(u16 status) { return ""; }
#endif

/* station management */
int iwlagn_alloc_bcast_station(struct iwl_priv *priv,
			       struct iwl_rxon_context *ctx);
int iwlagn_add_bssid_station(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			     const u8 *addr, u8 *sta_id_r);
int iwl_remove_default_wep_key(struct iwl_priv *priv,
			       struct iwl_rxon_context *ctx,
			       struct ieee80211_key_conf *key);
int iwl_set_default_wep_key(struct iwl_priv *priv,
			    struct iwl_rxon_context *ctx,
			    struct ieee80211_key_conf *key);
int iwl_restore_default_wep_keys(struct iwl_priv *priv,
				 struct iwl_rxon_context *ctx);
int iwl_set_dynamic_key(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			struct ieee80211_key_conf *key, u8 sta_id);
int iwl_remove_dynamic_key(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			   struct ieee80211_key_conf *key, u8 sta_id);
void iwl_update_tkip_key(struct iwl_priv *priv,
			 struct iwl_rxon_context *ctx,
			 struct ieee80211_key_conf *keyconf,
			 struct ieee80211_sta *sta, u32 iv32, u16 *phase1key);
int iwl_sta_tx_modify_enable_tid(struct iwl_priv *priv, int sta_id, int tid);
int iwl_sta_rx_agg_start(struct iwl_priv *priv, struct ieee80211_sta *sta,
			 int tid, u16 ssn);
int iwl_sta_rx_agg_stop(struct iwl_priv *priv, struct ieee80211_sta *sta,
			int tid);
void iwl_sta_modify_sleep_tx_count(struct iwl_priv *priv, int sta_id, int cnt);
int iwl_update_bcast_stations(struct iwl_priv *priv);
void iwlagn_mac_sta_notify(struct ieee80211_hw *hw,
			   struct ieee80211_vif *vif,
			   enum sta_notify_cmd cmd,
			   struct ieee80211_sta *sta);

/* rate */
static inline u32 iwl_ant_idx_to_flags(u8 ant_idx)
{
	return BIT(ant_idx) << RATE_MCS_ANT_POS;
}

static inline u8 iwl_hw_get_rate(__le32 rate_n_flags)
{
	return le32_to_cpu(rate_n_flags) & 0xFF;
}

static inline __le32 iwl_hw_set_rate_n_flags(u8 rate, u32 flags)
{
	return cpu_to_le32(flags|(u32)rate);
}

/* eeprom */
void iwlcore_eeprom_enhanced_txpower(struct iwl_priv *priv);
void iwl_eeprom_get_mac(const struct iwl_priv *priv, u8 *mac);
int iwlcore_eeprom_acquire_semaphore(struct iwl_priv *priv);
void iwlcore_eeprom_release_semaphore(struct iwl_priv *priv);

/* notification wait support */
void __acquires(wait_entry)
iwlagn_init_notification_wait(struct iwl_priv *priv,
			      struct iwl_notification_wait *wait_entry,
			      void (*fn)(struct iwl_priv *priv,
					 struct iwl_rx_packet *pkt),
			      u8 cmd);
signed long __releases(wait_entry)
iwlagn_wait_notification(struct iwl_priv *priv,
			 struct iwl_notification_wait *wait_entry,
			 unsigned long timeout);
void __releases(wait_entry)
iwlagn_remove_notification(struct iwl_priv *priv,
			   struct iwl_notification_wait *wait_entry);

/* mac80211 handlers (for 4965) */
void iwlagn_mac_tx(struct ieee80211_hw *hw, struct sk_buff *skb);
int iwlagn_mac_start(struct ieee80211_hw *hw);
void iwlagn_mac_stop(struct ieee80211_hw *hw);
void iwlagn_configure_filter(struct ieee80211_hw *hw,
			     unsigned int changed_flags,
			     unsigned int *total_flags,
			     u64 multicast);
int iwlagn_mac_set_key(struct ieee80211_hw *hw, enum set_key_cmd cmd,
		       struct ieee80211_vif *vif, struct ieee80211_sta *sta,
		       struct ieee80211_key_conf *key);
void iwlagn_mac_update_tkip_key(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				struct ieee80211_key_conf *keyconf,
				struct ieee80211_sta *sta,
				u32 iv32, u16 *phase1key);
int iwlagn_mac_ampdu_action(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    enum ieee80211_ampdu_mlme_action action,
			    struct ieee80211_sta *sta, u16 tid, u16 *ssn,
			    u8 buf_size);
int iwlagn_mac_sta_add(struct ieee80211_hw *hw,
		       struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta);
void iwlagn_mac_channel_switch(struct ieee80211_hw *hw,
			       struct ieee80211_channel_switch *ch_switch);
void iwlagn_mac_flush(struct ieee80211_hw *hw, bool drop);

#endif /* __iwl_agn_h__ */
